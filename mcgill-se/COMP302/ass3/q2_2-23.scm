; Exercise 2.23 on page 63

; We were asked to implement an environment interface with a "single rib" --
; i.e., a single list of values and a single list of values. Extending the
; environment implies prepending new values to both these lists.

(define empty-env
  (lambda ()
    (cons '() '())))

(define extend-env
  (lambda (syms vals env)
    (cons
      (merge syms (car env))
      (merge vals (cdr env)))))

(define apply-env
  (lambda (env sym)
    (let ((pos (list-find-position sym (car env))))
      (if (number? pos)
	(list-ref (cdr env) pos)
	(eopl:error 'apply-env "No binding for ~s" sym)))))

(define has-association?
  (lambda (env sym)
    (number? (list-find-position sym (car lst)))))

; copy a list
(define copy
  (lambda (lst)
    (if (null? lst)
      '()
      (cons (car lst) (copy (cdr lst))))))

; append the second list to the first; return a copy
(define merge
  (lambda (lst1 lst2)
    (if (null? lst1)
      (copy lst2)
      (cons (car lst1) (merge (cdr lst1) lst2)))))

; List stuff, copied from EOPL
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
