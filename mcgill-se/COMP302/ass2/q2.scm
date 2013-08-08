; Given the BNF:
; <expr> ::= number
;            | ( number <expr> <op> <expr> )
; <op> ::= + | - | * | /

(define evaluate
  (lambda (expr)
    (if (number? expr)
      expr
      ((parse-op (cadr expr))
       (evaluate (car expr))
       (evaluate (caddr expr))))))

(define parse-op
  (lambda (op)
    (cond
      ((eq? op '+) +)
      ((eq? op '-) -)
      ((eq? op '*) *)
      ((eq? op '/) /)
      (else (error "Invalid operand")))))
