
(define (from-to n1 n2)
  (if (< n2 n1)
      null
      (cons n1 (from-to (+ 1 n1) n2))))

(define (r-char _)
  (integer->char (+ 65 (random 26))))

(define (r-string size _)
  (apply string (map r-char (from-to 1 size))))

(define (r-num _)
  (/ (random 10) (+ 1 (random 2))))

(define (r-symbol _)
  (string->symbol
   ((if (< 50 (random 100)) string-downcase (lambda (x) x))
    (r-string (+ 1 (random 5)) ()))))

(define (test1 n)
  (and (integer? (cdr n)) (symbol? (car n))))

(define (r-choice l)
  (let* ((n (length l))(i (random n)))
    (list-ref l i)))

(define (gen-nb _)
  ((r-choice (list r-num r-symbol r-char)) ()))

(define (gen-b _)
  (cons ((r-choice (list r-num r-symbol r-char))()) (r-num())))

(define (gen-tb _)
  (cons (r-symbol()) (random 100)))

(define (no-dup cmp l)
  (if (null? l)
      null
      (let ((h (car l))(t (cdr l)))
        (cons h (filter (lambda (x) (not(cmp x h))) (no-dup cmp t))))))

(define (gen-e _)
  (no-dup (lambda (x y) (equal? (car x)(car y)))
          (map gen-tb (from-to 1 (+ 1 (random 10))))))

(define (sum l) (foldl + 0 l))

(define vbricks
  (list
   'true
   'false
   (list 'not 1 'B not)
   (list 'and 2 'B (lambda(x y)(and x y)))
   (list 'and 2 'B (lambda(x y)(and x y)))
   (list 'lt 2 'A <)
   (list 'eq 2 'A =))
  )

(define (gen-bexp threshold _)
  (if (< (random 100) threshold)
      (let ((r (r-choice vbricks)))
        (if (symbol? r)
            (list r #t (if (symbol=? r 'true) #t #f))
            (let ((s (car r))
                  (ar (cadr r))
                  (ty (caddr r))
                  (f (cadddr r))
                  )
              (let*((args-b-v 
                     (if (symbol=? ty 'A)
                         (list (map (lambda(_)(random 10)) (from-to 1 ar)) #t 'notBool)
                         (let ((lr (map (lambda(_)(gen-bexp threshold _)) (from-to 1 ar))))
                          (list
                           (map car lr)
                           (foldl (lambda (x y)(and x y)) #t (map cadr lr))
                           (map caddr lr)                           
                           ))))
                (args (car args-b-v))
                (b (cadr args-b-v))
                (v (caddr args-b-v)))
              (if
               (symbol=? ty 'A)
               (list (cons s args) b (apply f args))
               (list (cons s args) b (apply f v)))))))
      (list ((r-choice (list (lambda(x)(r-string 2 x)) r-char)) ()) #f 'notBool)
      ))


(define (compare p1 p2)
  (cond ((and (symbol? p1) (symbol? p2) (eq? p1 p2)))
        ((and (pair? p1)(pair? p2)) (and (compare(car p1)(car p2))(compare(cdr p1)(cdr p2))))
        ((and (boolean? p1) (boolean? p2) (eqv? p1 p2)))
        ((and (number? p1) (number? p2) (= p1 p2)))
        ((and (string? p1) (string? p2) (string=? p1 p2)))
        ((and (list? p1) (list? p2) (= (length p1) (length p2)))
         (foldl (lambda (x y)(and x y)) #t (map compare p1 p2)))
        (else #f))
  )
; print-errors: exn -> void
; (print-errors exn) print a message
; corresponding to the exception exn 
(define print-errors
  (lambda (exn)
    (begin
      (display "FAILED (")
      (display (exn-message exn))
      (display ")")
      (newline)
      #f
      )))

; header: string symbol any boolean -> void
; (header name test-expr result mode)
; prints the header of test messages
; - name is the title of the test
; - test-expr is the quoted expression to test
; - result is the expected result
; - mode is the mode in which to print the result

(define (header name test-expr result err-mode)
  (begin
    (display "== Test ")(display name)(display "  ==")
    (newline)
    (display "Tested expression: ")(write test-expr)
    (newline)
    (display "Expected result:   ")
    ((if err-mode display write)result)
    (newline)
    (display "Test: ")
    ))
; test-error: string symbol -> void
; (test-error name test-expr)
; evaluates the quoted expression test-expr
; that is supposed to fail
(define (test-error header name test-expr)
  (with-handlers ((exn:fail? (lambda (exn) (begin (displayln "Test: PASSED")#t))))
    (begin (let ((test-result (eval test-expr)))
             (begin
               (header name test-expr "the execution should raise an error" #f)
               (display "FAILED (result is ")
               (display test-result)
               (displayln ")")
               #f)))))
; test: string symbol any predicate -> void
; (test-error name test-expr result pred?)
; evaluates the quoted expression test-expr
; that is supposed evaluate to result.
; The values are compared using the predicate pred?
(define (test header name test-expr result equiv?)
  (with-handlers ((exn:fail? print-errors))
    (let ((test-result (eval test-expr)))
      (if (equiv? test-result result)
          (begin (displayln "Test: PASSED") #t)
          (begin (header name test-expr result #f)
                 (display "FAILED (unexpected result: ")
                 (display test-result)
                 (displayln ")")
                 #f))))
  )

;; --------- The tests -------------

(define (grade-header name test-expr result err-mode)
  (begin
    (displayln "Test: ")
    (display "\tTested expression: ")(write test-expr)
    (display "\n")
    (display "\tExpected result:   ")
    ((if err-mode display write)result)
    (display "\n\t")
    ))

(define (tests name points test-list)
  (begin
    (display "PROCEDURE: ")(displayln name)
    (define score 
      (sum (map (lambda (t) (if t points 0))
                (map (lambda (l)(if (= (length l) 4)
                                    (apply test grade-header l)
                                    (apply test-error grade-header l)))
                       test-list))))
    (display "Points: ")(display score)
    (newline)
    score
    )
  )

(define (final_grade scores)
  (begin
    (newline)
    (display "GRADE: ")
    (displayln (sum scores)))
  )
;==========================================

(load "hw05.rkt")

(begin
  (displayln "---------------------------------")
  (displayln "Assignment #05")
  (displayln "---------------------------------")
  (if
   (with-handlers
       ((exn:fail? (lambda (exn)
                     (begin
                       (displayln "Missing definition 'name'")
                       (final_grade null)
                       #f
                       ))))
     (display "Student: ")
     (displayln name)
     #t)
   (begin
     (displayln "---------------------------------")
     (newline)
     (final_grade
      (list
       (tests
        "bexp?" 0.5
         (let ((le (map (lambda(x)(gen-bexp 80 x)) (from-to 1 20))))
           (map (lambda (e)(let ((exp (car e))(res(cadr e)))
                  (list "" `(bexp? ',exp) res compare)))
                le)
         ))
        (tests
         "B" 0.5
         (let ((le (map (lambda(x)(gen-bexp 100 x)) (from-to 1 20))))
           (map (lambda (e)(let ((exp (car e))(res(caddr e)))
                  (list "" `(B ',exp env-empty) res compare)))
                le)
         ))
       )
      )
     )
   )
  )