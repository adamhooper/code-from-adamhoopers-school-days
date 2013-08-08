; Environment based on datatype representation of Section 2.3.3
; Adding extend-env-recursively from Section 3.6

(require (lib "eopl.ss" "eopl"))

(define-datatype environment environment?
  (empty-env-record)
  (extended-env-record
    (syms (list-of symbol?))
    (vals (list-of scheme-value?))
    (env environment?))
  (recursively-extended-env-record
    (proc-names (list-of symbol?))
    (idss (list-of (list-of symbol?)))
    (bodies (list-of expression?))
    (env environment?)))

(define scheme-value? (lambda (v) #t))

(define empty-env
  (lambda ()
    (empty-env-record)))

(define extend-env
  (lambda (syms vals env)
    (extended-env-record syms vals env)))

(define extend-env-recursively
  (lambda (proc-names idss bodies old-env)
    (recursively-extended-env-record proc-names idss bodies old-env)))

(define apply-env
  (lambda (env sym)
    (cases environment env
      (empty-env-record ()
	(eopl:error 'apply-env "No binding for ~s" sym))
      (extended-env-record (syms vals env)
	(let ((pos (rib-find-position sym syms)))
	  (if (number? pos)
	    (list-ref vals pos)
	    (apply-env env sym))))
      (recursively-extended-env-record (proc-names idss
					 bodies old-env)
	(let ((pos (rib-find-position sym proc-names)))
	  (if (number? pos)
	    (closure
	      (list-ref idss pos)
	      (list-ref bodies pos)
	      env)
	    (apply-env old-env sym)))))))

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

(define rib-find-position list-find-position)
