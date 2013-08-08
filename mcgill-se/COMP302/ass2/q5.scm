(define list-index-aux
  (lambda (lst sym ret)
    (if (null? lst)
      -1
      (if (eqv? (car lst) sym)
	ret
	(list-index-aux (cdr lst) sym (+ ret 1))))))

; Returns the index of 'sym in lst (-1 if not found)
(define list-index
  (lambda (lst sym)
    (list-index-aux lst sym 0)))

(define address-lookup-aux
  (lambda (vars sym level)
    (if (null? vars)
      '(free)
      (let ((index (list-index (car vars) sym)))
	(if (not (= index -1))
	  (list ': level index)
	  (address-lookup-aux (cdr vars) sym (+ level 1)))))))

; Returns a list, '(sym : level position), specifying 'sym in vars, a list of
;lists. Returns the list '(sym free) if 'sym is not in vars
(define address-lookup
  (lambda (vars sym)
    (cons sym (address-lookup-aux vars sym 0))))

; Recursively travels through expression, holding in "vars" a list of scopes.
; Each scope is a list of symbols corresponding to variables, so a lookup with
; address-lookup will find a variable's lexical address.
;
; The function follows the following BNF:
; <expresssion> ::= <identifier>
;                   | (if <expression> <expression> <expression>)
;                   | (lambda ({<identifier>}*) <expression>)
;                   | ({<expression>}*+)
(define lexical-address-aux
  (lambda (expression vars)
    (cond
      ((null? expression)
       null)
      ((symbol? expression)
       (address-lookup vars expression))
      ((eq? (car expression) 'if)
       (list
	 'if
	 (lexical-address-aux (cadr expression) vars)
	 (lexical-address-aux (caddr expression) vars)
	 (lexical-address-aux (cadddr expression) vars)
	 ))
      ((eq? (car expression) 'lambda)
       (list
	 'lambda
	 (cadr expression)
	 (lexical-address-aux
	   (caddr expression)
	   (cons (cadr expression) vars))))
      (else
	(cons (lexical-address-aux (car expression) vars)
	      (lexical-address-aux (cdr expression) vars))
	))))

(define lexical-address
  (lambda (lst)
    (lexical-address-aux lst null)))
