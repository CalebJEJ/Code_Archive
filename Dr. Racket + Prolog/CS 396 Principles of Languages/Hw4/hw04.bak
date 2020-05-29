(define name "Caleb Johnson")

(define binding? (
                  lambda (pair) (
                                 and (symbol? (car pair))
                                     (integer? (cdr pair))
                                 )
                  
                  )
  )

(define env? (
              lambda (l) (
                          if(null? l)
                            #t
                            (and (binding? (car l))
                                 (env? (cdr l))
                             )
                          )
               )
  )

(define env-get (
                 lambda (l g) 
                            (cond 
                             ((eqv? (car (car l)) g) (cdr (car l)))
                             ((not (null? l)) (env-get (cdr l) g))
                             (else 0))
                            
                 )
  )

(define comment "remove is a constant and can't be overwritten")
(define env-remove (
                 lambda (e name) 
                            (cond 
                             ((eqv? (car (car e)) name) (cdr e))
                             (else (cons (car e)(remove (cdr e) name))))
                            
                 )
  )

(define env-update (
                 lambda (l name update) 
                            (cond 
                             ((eqv? (car (car l)) name) (cons (cons name update) (cdr l)))
                             (else (cons (car l)(remove (cdr l) name))))
                            
                 )
  )