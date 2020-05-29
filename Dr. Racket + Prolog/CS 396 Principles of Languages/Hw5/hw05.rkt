(define name "Caleb Johnson")

(load "hw05_w1.rkt")

(define bexp? (lambda (arg) (cond
                             ((list? arg) (cond
                                               ((<= (length arg) 3) (cond
                                                          ((symbol=? (car arg) 'not) (bexp? (cadr arg)))
                                                          ((symbol=? (car arg) 'lt) (and (num? (cadr arg)) (num? (caddr arg))))
                                                          ((symbol=? (car arg) 'eq) (and (num? (cadr arg)) (num? (caddr arg))))
                                                          ((symbol=? (car arg) 'and) (and (bexp? (cadr arg)) (bexp? (caddr arg))))
                                                          (else #f)
                                                          )
                                                )
                                               (else #f)
                                          )
                              )

                             ;not a list therefore evaluate the single thing given
                             (else (cond
                                     ((symbol? arg)
                                       (cond
                                     ((symbol=? arg 'true) #t)
                                     ((symbol=? arg 'false) #t)
                                     (else #f)
                                     ))
                                       (else #f)
                                   )
                              )
                     )
               )
  )


(define B (lambda (be env) (cond
                                 ((symbol? be) (symbol=? 'true be))
                                 ((symbol=? (car be) 'not) (if (B (cadr be) env)
                                                                  #f
                                                                  #t))
                                 ((symbol=? (car be) 'and) (and (B (cadr be) env) (B (caddr be) env)))
                                 ((symbol=? (car be) 'lt) (< (assign (cadr be) env) (assign (caddr be) env)))
                                 ((symbol=? (car be) 'eq) (= (assign (cadr be) env) (assign (caddr be) env)))
                                                          )
            )
  )

(define assign (lambda (arg env) (cond
                               ((num? arg) arg)
                               (else env-get env arg)
                               )
                 )
  )