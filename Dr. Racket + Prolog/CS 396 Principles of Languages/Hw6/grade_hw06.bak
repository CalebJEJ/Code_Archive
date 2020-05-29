;#lang racket
(require "w1c-ast.rkt" "w1c-exec.rkt")

(define (from-to n1 n2)
  (if (< n2 n1)
      null
      (cons n1 (from-to (+ 1 n1) n2))))

(define (sum l) (foldl + 0 l))

; Random generation

(define (r-char . _)
  (integer->char (+ 65 (random 26))))

(define (r-string size . _)
  (apply string (map r-char (from-to 1 size))))

(define (r-num . _)
  (/ (random 10) (+ 1 (random 2))))

(define (r-symbol . _)
  (string->symbol
   ((if (< 50 (random 100)) string-downcase (lambda (x) x))
    (r-string (+ 1 (random 5))))))

(define (test1 n)
  (and (integer? (cdr n)) (symbol? (car n))))

(define (r-choice l)
  (let* ((n (length l))(i (random n)))
    (list-ref l i)))

(define (gen-nb . _)
  ((r-choice (list (r-num) (r-symbol) (r-char)))))

(define (r-com . _)
  (let* [(coms (list 'skip 'assign 'seq 'print 'input))
         (com (r-choice coms))]
    (cond
      ((symbol=? com 'assign)
       (list 'assign (r-symbol) (random 100)))
      ((symbol=? com 'seq)
       (list 'seq (r-com) (r-com)))
      ((or (symbol=? com 'input) (symbol=? com 'print))
       (list com (r-symbol)))
      (else 'skip))))

(define (r-exp . args)
  (if (null? args)
      (r-exp 3)
      (let ((n (car args)))
     (if (or (= 0 n)(< (random 100) 70))
         (r-choice
          (list (random 10)
                (- (random 10))
                (r-symbol)))
         (list (r-choice (list 'eq 'and 'lt 'add 'mul 'sub))
                         (r-exp (- n 1))(r-exp (- n 1)))))))

(define (r-prog1 . _)
  (r-choice
   (list
    (cons (mk-repeat (r-com) (r-exp)) #t)
    (cons (mk-repeat (r-exp) (r-exp)) #f)
    (cons (list 'repeat (r-com)) #f)
    (cons (list 'repeat (map r-com (from-to 1 (random 4)))) #f))))

(define (r-prog2 . _)
  (r-choice
   (list
    (cons (mk-repeat (r-com) (r-exp)) #t)
    (cons (mk-repeat (r-exp) (r-exp)) #f)
    (cons (list 'repeat (r-com)) #f)
    (cons (list 'repeat (map r-com (from-to 1 (random 4)))) #f)
    (cons (r-com) #t)
    (cons (mk-while (r-exp) (r-com)) #t))))

(define (r-prog3 . _)
  (r-choice
   (list
    (cons (mk-repeat (r-com) (r-exp)) #t)
    (cons (mk-repeat (r-com) (r-exp)) #t)
    (cons (r-com) #f)
    (cons (mk-while (r-exp) (r-com)) #f))))


(define (r-env . _)
  (map (lambda (_) (cons (r-symbol) (random 100)))
       (from-to 1 (+ 1 (random 3)))))

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

(begin
  (displayln "---------------------------------")
  (displayln "Assignment #06")
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
        "com-repeat?" 0.5
        (map (lambda (_)
               (let* [(r (r-prog1))
                      (prog (car r))
                      (res  (cdr r))]
                 (list "" `(com-repeat? ',prog) res compare)))
             (from-to 1 8)))
       (tests
        "com?" 0.5
        (map (lambda (_)
               (let* [(r (r-prog2))
                      (prog (car r))
                      (res  (cdr r))]
                 (list "" `(com? ',prog) res compare)))
             (from-to 1 6)))
       (tests
        "repeat?" 0.5
        (map (lambda (_)
               (let* [(r (r-prog3))
                      (prog (car r))
                      (res  (cdr r))]
                 (list "" `(repeat? ',prog) res compare)))
             (from-to 1 6)))
       (tests
        "exec" 1
        (map (lambda (_)
               (let* [(c (r-com))
                      (e (r-exp))
                      (prog (mk-repeat c e))
                      (env (r-env))
                      (res (mk-seq c (mk-if e 'skip prog)))
                      (cfg (nt-cfg prog env))]
                 (list "" `(exec ',cfg) (nt-cfg res env) compare)))
             (from-to 1 10)))
        )
       )
      )
     )
   (void)
   )