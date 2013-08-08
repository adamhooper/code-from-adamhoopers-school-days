; Syntax for Lambda-1:
;
; <expr> ::= number
;            | identifier
;            | (lambda (<identifier>) <expr>)
;            | (<expr> <expr>)

; Inserts sym into lst, but ignores dupes.
; Inserts in alphabetical order.
(define list-insert
  (lambda (lst sym)
    (cond
      ((null? lst) (list sym))
      ((eqv? (car lst) sym) lst)
      ((string<? (symbol->string sym) (symbol->string (car lst)))
       (cons sym lst))
      (else
	(cons (car lst) (list-insert (cdr lst) sym))))))

; Returns #t if sym is in lst
(define in-list
  (lambda (lst sym)
    (cond
      ((null? lst) #f)
      ((eqv? (car lst) sym) #t)
      (else (in-list (cdr lst) sym)))))

; returns free_vars with free variables from expr inserted
(define free-vars-aux
  (lambda (expr free_vars available_vars)
    (cond
      ((number? expr) free_vars)
      ((symbol? expr)
       (if (in-list available_vars expr)
	 free_vars
	 (list-insert free_vars expr)))
      ((eqv? 'lambda (car expr))
       (free-vars-aux (caddr expr)
		      free_vars
		      (cons (caadr expr) available_vars)))
      (else
	(free-vars-aux (cadr expr)
		       (free-vars-aux (car expr) free_vars available_vars)
		       available_vars)))))

; returns a list of free variables used in expr
(define free-vars
  (lambda (expr)
    (free-vars-aux expr null null)))

; returns bound_vars with bound variables from expr inserted
(define bound-vars-aux
  (lambda (expr bound_vars available_vars)
    (cond
      ((number? expr) bound_vars)
      ((symbol? expr)
       (if (in-list available_vars expr)
	 (list-insert bound_vars expr)
	 bound_vars))
      ((eqv? 'lambda (car expr))
       (bound-vars-aux (caddr expr)
		       bound_vars
		       (cons (caadr expr) available_vars)))
      (else
	(bound-vars-aux (cadr expr)
			(bound-vars-aux (car expr) bound_vars available_vars)
			available_vars)))))

; Returns a list of the bound variables used in expr
(define bound-vars
  (lambda (expr)
    (bound-vars-aux expr null null)))
