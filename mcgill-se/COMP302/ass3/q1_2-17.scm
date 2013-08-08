; Exercise 2.17 on page 59

; Instead of representing an environment by a single procedure, we represent
; it using two: the first returning the value (as on page 57) and the second
; returning a boolean (whether the value exists).
;
; The two procedures are passed in a pair. The car is identical to the original
; procedural representation; the cdr is our new procedure which returns true
; if the value exists.

(define empty-env
  (lambda ()
    (cons
      (lambda (sym)
	(eopl:error 'apply-env "No binding for ~s" sym))
      (lambda (sym)
	#f))))

(define extend-env
  (lambda (syms vals env)
    (cons
      (lambda (sym)
	(let ((pos (list-find-position sym syms)))
	  (if (number? pos)
	    (list-ref vals pos)
	    (apply-env env sym))))
      (lambda (sym)
	(number? (list-find-position sym syms))))))

(define apply-env
  (lambda (env sym)
    ((car env) sym)))

(define has-association?
  (lambda (env sym)
    ((cdr env) sym)))

(define list-find-position
  (lambda (sym los)
    (list-index (lambda (sym1) (eqv? sym1 sym)) los)))

(define list-index
  (lambda (pred ls)
    (cond
      ((null? ls) #f)
      ((pred (car ls)) 0)
      (else (let ((list-index-r (list-index pred (cdr ls))))
	      (if (number? list-index-r)
		(+ list-index-r 1)
		#f))))))
