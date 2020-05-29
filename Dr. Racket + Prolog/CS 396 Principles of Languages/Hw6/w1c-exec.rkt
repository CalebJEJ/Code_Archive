#lang racket

(require "w1c-ast.rkt")

(provide env-empty env-get env-update t-cfg nt-cfg exec run)

; ========== W1C Execution Engine =========

; === Environments ===

; binding?
(define binding? (lambda (e)
  (and (pair? e) (symbol? (car e)) (integer? (cdr e)))))

; env?
(define env? (lambda (l)
  (cond
    ((null? l) #t)
    (else (and (binding? (car l))
               (env? (cdr l)))))))

; env-empty
(define env-empty null)

; env-get
(define env-get
  (lambda (env var)
    (if (null? env)
        0
        (if (symbol=? var (car (car env)))
            (cdr(car env))
            (env-get (cdr env) var)
            ))))

; env-remove
(define env-remove
  (lambda (env var)
    (if (null? env)
        null
        (if (symbol=? var (car (car env)))
            (cdr env)
            (cons (car env) (env-remove (cdr env) var))))))

; env-update
(define env-update
  (lambda (env var val)
    (cons (cons var val)(env-remove env var))))

; === The Semantic Function A ===

(define (get-op op)
  (cond
    ((symbol=? op 'add) +)
    ((symbol=? op 'sub) -)
    ((symbol=? op 'mul) *)
    ((symbol=? op 'eq)  (lambda (x y)(if (= x y) 1 0)))
    ((symbol=? op 'and) *)
    ((symbol=? op 'lt) (lambda (x y) (if (< x y) 1 0)))
    (else (lambda (n)(if (= n 0) 1 0)))))

; Evaluation of expressions
(define (A exp env)
  (cond
    ((var? exp) (env-get env exp)) ; case (1)
    ((num? exp) exp)               ; case (2)
    ((unop? (car exp))
     ((get-op (car exp)) (A (cadr exp) env)))
    ((binop? (car exp))    ; all other cases
     ((get-op (car exp))
      (A (cadr exp) env)   ;corresponds to e1
      (A (caddr exp) env)));corresponds to e2
    (else (begin (display exp)raise "cannot happen"))
    ))

; === Configurations ===

; A configuration is:
; - non-terminal < com, env >: in Scheme (list 'nt rcom renv)
; - terminal     env:          in Scheme (list 't renv)  

 (define (cfg? cfg)
  (and (list? cfg)
       (or (and (= 2 (length cfg)) (symbol? (car cfg))
                (symbol=? 't (car cfg))(env? (cadr cfg))))
           (and (= 3 (length cfg)) (symbol? (car cfg))
                (symbol=? 'nt (car cfg))
                (com? (cadr cfg))(env? (caddr cfg)))))

; To build a terminal configuration
(define (t-cfg env)
  (list 't env))

; To build a non terminal configuration
(define (nt-cfg com env)
  (list 'nt com env))

; To check if a configuration is terminal
(define (t-cfg? cfg)
  (symbol=? (car cfg) 't))

; To get the command (resp. environment),
; assuming cfg is a valid configuration
(define (get-com cfg)(cadr cfg))
(define (get-env cfg)
  (if (t-cfg? cfg)
      (cadr cfg)
      (caddr cfg)))

; === Print and Input ===

; a printing function
(define (print-var var val)
  (begin
    (display var)
    (display " = ")
    (displayln val)
    ))

; an input function
(define (input-var var)
  (begin
    (display var)
    (display "? ")
    (let ((n (read)))
      (if (integer? n)
          n
          (begin
            (displayln "Error: integer expected")
            (input-var var))))))

; === Execution of One Step ===

(define (exec config)
  (if (t-cfg? config)
      config
      (let ((com (get-com config))
            (env (get-env config)))
        (cond
          ; skip case
          ((skip? com) (t-cfg env))
          ; assign case
          ((assign? com) 
           (let* ((x (get-var com))
                  (e (get-expr com))
                  (n (A e env)))
            (t-cfg (env-update env x n))))
          ; sequence case
          ((seq? com)
           (let* ((com1 (get-fst-com com))
                  (com2 (get-snd-com com))
                  (new_config (exec (nt-cfg com1 env))))
              (if (t-cfg? new_config)
                  (nt-cfg com2 (get-env new_config))
                  (let ((new_com1 (get-com new_config))
                        (new_env (get-env new_config)))
                    (nt-cfg (mk-seq new_com1 com2)
                            new_env)))))
           ; condition case
           ((if? com)
            (let ((exp  (get-cond com))
                  (com1 (get-then com))
                  (com2 (get-else com)))
              (if (= (A exp env) 0)
                  (nt-cfg com2 env)
                  (nt-cfg com1 env))))
           ; loop case
           ((while? com)
            (let ((exp (get-cond com))
                  (com1 (get-body com)))
              (nt-cfg
               (mk-if exp (mk-seq com1 com) 'skip)
               env)))
           ; print case
           ((print? com)
            (let ((x (get-var com)))
              (begin
                (print-var x (A x env))
                (t-cfg env))))
           ; input case
           ((input? com)
            (let* ((x (get-var com))
                   (n (input-var x)))
              (t-cfg (env-update env x n))))
           
           ; =====Implementation for Project =========
           ; repeat case
           ((repeat? com)
            (let* ((exp (get-var com))
                   (com (get-body com)))
              (nt-cfg
               (mk-seq exp (mk-if com 'skip (mk-repeat exp com)))
               env)))
           ; Error
           (else (error "Error: command not yet implemented or unknown"))
        ))))

; === Execution of Several Steps ===

; We need to repeat calling exec until
; we reach a terminal configuration.
; Let's abstract this behavior for
; a function f starting from a value
; init and stopping when the predicate
; stop? is satisfied:
(define (iter f init stop?)
  (if (stop? init)
      init
      (iter f (f init) stop?)))

; The Interpreter Backend
(define (run com)
  (iter exec (nt-cfg com env-empty) t-cfg?))