(define (from-to n1 n2)
  (if (<= n1 n2)
      (cons n1 (from-to (+ 1 n1) n2))
      null))

(define (aux n1 n acc)
  (if (< n n1) acc (aux n1 (- n 1) (cons n acc))))

(define (from_to n1 n2)
    (aux n1 n2 null))