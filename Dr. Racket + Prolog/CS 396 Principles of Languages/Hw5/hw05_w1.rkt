; Type predicates for W1 AST

(define (num? n) (integer? n))

(define (var? v)
  (symbol? v))

(define (aexp? e)
  (or (num? e)
      (var? e)
      (and (list? e)
           (= (length e) 3) 
           (and (symbol? (car e))
                (or (symbol=? (car e) 'add)
                    (symbol=? (car e) 'mul)
                    (symbol=? (car e) 'sub)))
           (aexp? (car(cdr e)));shortcut:(cadr e) 
           (aexp? (caddr e)))))

(define (get-op op)
  (cond
    ((symbol=? op 'add) +)
    ((symbol=? op 'sub) -)
    ((symbol=? op 'mul) *)))


(define (A exp env)
  (cond
    ((var? exp) (env exp)) ; case (1)
    ((num? exp) exp)       ; case (2)
    (else
     ((get-op (car exp))
      (A (cadr exp) env) ;corresponds to e1
      (A (caddr exp) env)));corresponds to e2
    ))

; Environments

; The empty environment
(define env-empty
  (lambda (x) 0))

; Take a symbol and an environement and returns
; the value assiocated with this symbol in the
; environment
(define (env-get env x)
  (env x))

; Updates value of x in the environment env
; with value v
(define (env-update env x v)
  (lambda (y)
    (if (symbol=? x y)
        v
        (env y))))
