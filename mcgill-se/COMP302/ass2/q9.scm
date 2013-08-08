; Let expression:
; (let ((<var1> <expr1>)
;       (<var2> <expr2>)
;       ...
;      )
;  <body>)
;
; Equivalent lambda:
; ((lambda (<var1> <var2> ...) <body>)
;   <expr1> <expr2> ...)

; Translates a let expression into a lambda expression. Assumes expr is of the
; "let" form above.
(define let->app
  (lambda (expr)
    (cons
      (list 'lambda (map car (cadr expr))
	    (caddr expr))
      (map cadr (cadr expr)))))
