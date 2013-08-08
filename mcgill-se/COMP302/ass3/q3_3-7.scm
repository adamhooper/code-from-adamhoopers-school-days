; Add list processing: cons, car, cdr, list, emptylist
;
; Steps to take:
; 1. Add the primitives to the primitive datatype (cons-prim, car-prim,
;    cdr-prim, list-prim)
; 2. Add the following to apply-primitive:
;      (cons-prim () (cons (car args) (cadr args)))
;      (car-prim () (car (car args)))
;      (cdr-prim () (cdr (car args)))
;      (list-prim () args)
; 3. Add the following to the grammar:
;      (primitive ("cons") cons-prim)
;      (primitive ("car") car-prim)
;      (primitive ("cdr") cdr-prim)
;      (primitive ("list") list-prim)
; 4. Add emptylist to the initial environment:
;      (define init-env
;        (lambda ()
;          (extend-env
;            '(i v x emptylist)
;            '(1 5 10 ())
;            (empty-env))))
;
; The set of possible expressed values now includes lists. The set of denoted
; values is extended to include the empty list (since the language can't assign
; arbitrary values to arbitrary variables, no other lists can be denoted).
;
; The rest of this file contains the new language.

(require (lib "eopl.ss" "eopl"))

; Copy the environment from q1_2-20.scm (any q1 or q2 answers would be fine)
(load "q1_2-20.scm")

(define-datatype program program?
  (a-program
    (exp expression?)))

(define-datatype expression expression?
  (lit-exp
    (datum number?))
  (var-exp
    (id symbol?))
  (primapp-exp
    (prim primitive?)
    (rands (list-of expression?))))

(define-datatype primitive primitive?
  (add-prim)
  (subtract-prim)
  (mult-prim)
  (incr-prim)
  (decr-prim)
  (cons-prim)
  (car-prim)
  (cdr-prim)
  (list-prim))

(define eval-program
  (lambda (pgm)
    (cases program pgm
      (a-program (body)
	(eval-expression body (init-env))))))

(define eval-expression
  (lambda (exp env)
    (cases expression exp
      (lit-exp (datum) datum)
      (var-exp (id) (apply-env env id))
      (primapp-exp (prim rands)
	(let ((args (eval-rands rands env)))
	  (apply-primitive prim args))))))

(define eval-rands
  (lambda (rands env)
    (map (lambda (x) (eval-rand x env)) rands)))

(define eval-rand
  (lambda (rand env)
    (eval-expression rand env)))

(define apply-primitive
  (lambda (prim args)
    (cases primitive prim
      (add-prim () (+ (car args) (cadr args)))
      (subtract-prim () (- (car args) (cadr args)))
      (mult-prim () (* (car args) (cadr args)))
      (incr-prim () (+ (car args) 1))
      (decr-prim () (- (car args) 1))
      (cons-prim () (cons (car args) (cadr args)))
      (car-prim () (car (car args)))
      (cdr-prim () (cdr (car args)))
      (list-prim () args)
      )))

(define init-env
  (lambda ()
    (extend-env
      '(i v x emptylist)
      '(1 5 10 ())
      (empty-env))))

(define scanner-spec-3-1
  '((white-sp
      (whitespace)				skip)
    (comment
      ("%" (arbno (not #\newline)))		skip)
    (identifier
      (letter (arbno (or letter digit "?")))	symbol)
    (number
      (digit (arbno digit))			number)))

(define grammar-3-1
  '((program
      (expression)
      a-program)
    (expression
      (number)
      lit-exp)
    (expression
      (identifier)
      var-exp)
    (expression
      (primitive "(" (separated-list expression ",") ")" )
      primapp-exp)
    (primitive ("+")
      add-prim)
    (primitive ("-")
      subtract-prim)
    (primitive ("*")
      mult-prim)
    (primitive ("add1")
      incr-prim)
    (primitive ("sub1")
      decr-prim)
    (primitive ("cons") cons-prim)
    (primitive ("car") car-prim)
    (primitive ("cdr") cdr-prim)
    (primitive ("list") list-prim)
    ))

(define scan&parse
  (sllgen:make-string-parser
    scanner-spec-3-1
    grammar-3-1))
;(sllgen:make-define-datatypes scanner-spec-3-1 grammar-3-1)
(define run
  (lambda (string)
    (eval-program
      (scan&parse string))))
(define read-eval-print
  (sllgen:make-rep-loop "--> " eval-program
    (sllgen:make-stream-parser
      scanner-spec-3-1
      grammar-3-1)))
