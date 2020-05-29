(define name "Caleb Johnson")

(define binding? (
                  lambda (pair) (
                                 if (cons? pair)
                                 (and (symbol? (car pair))
                                     (integer? (cdr pair)))
                                 #f
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
                             ((null? l) 0)
                             ((symbol=? (car (car l)) g) (cdr (car l)))
                             ((not (null? l)) (env-get (cdr l) g))
                             (else 0))
                            
                 )
  )

(define comment "remove is a constant and can't be overwritten")
(define env-remove (
                 lambda (e name) 
                            (cond
                             ((null? e) (list))
                             ((symbol=? (car (car e)) name) (cdr e))
                             (else (cons (car e) (env-remove (cdr e) name)))
                            )
                            
                 )
  )

(define env-update (
                 lambda (l name update) 
                            (cons (cons name update) (env-remove l name))
                            
                 )
  )