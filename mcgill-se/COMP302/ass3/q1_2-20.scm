; Exercise 2.20 on page 61

; The abstract syntax tree lends itself more to adding a new method, since the
; existing functions do not need to be modified. I've copied the original
; environment from the book and then added has-association? to the end.

(require (lib "eopl.ss" "eopl"))

(define-datatype environment environment?
  (empty-env-record)
  (extended-env-record
    (syms (list-of symbol?))
    (vals (list-of scheme-value?))
    (env environment?)))

(define scheme-value? (lambda (v) #t))

(define empty-env
  (lambda ()
    (empty-env-record)))

(define extend-env
  (lambda (syms vals env)
    (extended-env-record syms vals env)))

(define apply-env
  (lambda (env sym)
    (cases environment env
      (empty-env-record ()
	(eopl:error 'apply-env "No binding for ~s" sym))
      (extended-env-record (syms vals env)
	(let ((pos (list-find-position sym syms)))
	  (if (number? pos)
	    (list-ref vals pos)
	    (apply-env env sym)))))))

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

; Here it is, in all its glory
(define has-association?
  (lambda (env sym)
    (cases environment env
      (empty-env-record () #f)
      (extended-env-record (syms vals env)
	(or (number? (list-find-position sym syms))
	    (has-association? env sym))))))
