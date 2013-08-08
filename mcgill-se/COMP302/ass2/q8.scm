(require (lib "eopl.ss" "eopl"))

; Copied from page 46 of EoPL:
(define-datatype bintree bintree?
  (leaf-node
    (datum number?))
  (interior-node
    (key symbol?)
    (left bintree?)
    (right bintree?)))

(define leaf-sum
  (lambda (tree)
    (cases bintree tree
      (leaf-node (datum) datum)
      (interior-node (key left right)
	(+ (leaf-sum left) (leaf-sum right))))))

; Now... my stuff!

(define max-ignore-null-aux
  (lambda (lst ret)
    (cond
      ((null? lst) ret)
      ((null? (car lst)) (max-ignore-null-aux (cdr lst) ret))
      ((> (car lst) ret) (max-ignore-null-aux (cdr lst) (car lst)))
      (else (max-ignore-null-aux (cdr lst) ret)))))

; Returns the maximum non-null value of the given arguments
; Assumes at least one non-null value
(define max-ignore-null
  (lambda lst
    (max-ignore-null-aux (cdr lst) (car lst))))

; Returns the maximum interior sum in the given tree
(define max-interior-sum
  (lambda (tree)
    (cases bintree tree
      (leaf-node (datum) null)
      (interior-node (key left right)
	(max-ignore-null (leaf-sum tree)
			 (max-interior-sum left)
			 (max-interior-sum right))))))

; returns the symbol corresponding to the result of max-interior-sum
(define max-interior
  (lambda (tree)
    (cases bintree tree
      (leaf-node (datum) null)
      (interior-node (key left right)
	(let ((sum-of-tree (leaf-sum tree))
	      (sum-of-left (max-interior-sum left))
	      (sum-of-right (max-interior-sum right))
	      (target-sum (max-interior-sum tree)))
	  (cond
	    ((eqv? sum-of-tree target-sum) key)
	    ((eqv? sum-of-left target-sum) (max-interior left))
	    ((eqv? sum-of-right target-sum) (max-interior right))
	    (else (error "broken max-interior-sum"))))))))
