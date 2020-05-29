#lang racket

(require racket/cmdline racket/exn "w1c-exec.rkt" "w1c-parser.rkt")

; =========== W1C Interpreter =============

(define (interpreter filename)
  (with-handlers
      ([exn:fail:filesystem:errno? (lambda (exn) (begin (display "System error: ")(displayln exn)))]
       [exn:fail? (lambda (exn) (displayln (vector-ref (struct->vector exn) 1)))])
    (let* ((in (open-input-file filename)))
      (begin
        (let ((prog (parse in)))
          (run prog))
        (close-input-port in)))))

(define args
  (vector->list (current-command-line-arguments)))

(if (= 1 (length args))
    (interpreter (car args))
    (displayln "Error: filename required"))