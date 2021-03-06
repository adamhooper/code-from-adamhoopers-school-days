; Add cond:
; <expression> ::= cond {<expression> ==> <expression>}* end
; cond-exp (test-exps conseq-exps)
;
; Steps to take:
; 1. Add to the expression datatype:
;      (cond-exp
;        (test-exps (list-of expression?))
;        (conseq-exps (list-of expression?)))
; 2. Add to eval-expression:
;      (cond-exp (test-exps conseq-exps)
;        (eval-cond test-exps conseq-exps))
; 3. Create eval-cond:
;      (define eval-cond
;        (lambda (test-exps conseq-exps env)
;          (if (null? test-exps)
;            0
;            (if (true-value? (eval-expression (car test-exps) env))
;              (eval-expression (car conseq-exps) env)
;              (eval-cond (cdr test-exps) (cdr conseq-exps) env)))))
; 4. Add to the grammar:
;      (expression
;        ("cond" (arbno expression "==>" expression) "end")
;        cond-exp)
;
; The rest of this file is the implementation

; Copied from EOPL, starting at page 73
; This one adds Section 3.3, Conditional Evaluation
; It also adds all the things in q4_3-12.scm

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
    (rands (list-of expression?)))
  (if-exp
    (test-exp expression?)
    (true-exp expression?)
    (false-exp expression?))
  (cond-exp
    (test-exps (list-of expression?))
    (conseq-exps (list-of expression?)))
  )

(define-datatype primitive primitive?
  (add-prim)
  (subtract-prim)
  (mult-prim)
  (incr-prim)
  (decr-prim)
  (equal?-prim)
  (zero?-prim)
  (greater?-prim)
  (less?-prim)
  (cons-prim)
  (car-prim)
  (cdr-prim)
  (list-prim)
  (null?-prim)
  )

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
	  (apply-primitive prim args)))
      (if-exp (test-exp true-exp false-exp)
	(if (true-value? (eval-expression test-exp env))
	  (eval-expression true-exp env)
	  (eval-expression false-exp env)))
      (cond-exp (test-exps conseq-exps)
	(eval-cond test-exps conseq-exps env))
      )))

(define eval-rands
  (lambda (rands env)
    (map (lambda (x) (eval-rand x env)) rands)))

(define eval-rand
  (lambda (rand env)
    (eval-expression rand env)))

(define eval-cond
  (lambda (test-exps conseq-exps env)
    (if (null? test-exps)
      0
      (if (true-value? (eval-expression (car test-exps) env))
	(eval-expression (car conseq-exps) env)
	(eval-cond (cdr test-exps) (cdr conseq-exps) env)))))

(define apply-primitive
  (lambda (prim args)
    (cases primitive prim
      (add-prim () (+ (car args) (cadr args)))
      (subtract-prim () (- (car args) (cadr args)))
      (mult-prim () (* (car args) (cadr args)))
      (incr-prim () (+ (car args) 1))
      (decr-prim () (- (car args) 1))
      (equal?-prim () (if (equal? (car args) (cadr args)) 1 0))
      (zero?-prim () (if (zero? (car args)) 1 0))
      (greater?-prim () (if (> (car args) (cadr args)) 1 0))
      (less?-prim () (if (< (car args) (cadr args)) 1 0))
      (cons-prim () (cons (car args) (cadr args)))
      (car-prim () (car (car args)))
      (cdr-prim () (cdr (car args)))
      (list-prim () args)
      (null?-prim () (if (null? (car args)) 1 0))
      )))

(define init-env
  (lambda ()
    (extend-env
      '(i v x emptylist)
      '(1 5 10 '())
      (empty-env))))

(define true-value?
  (lambda (x)
    (not (zero? x))))

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
    (expression
      ("if" expression "then" expression "else" expression)
      if-exp)
    (expression
      ("cond" (arbno expression "==>" expression) "end")
      cond-exp)
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
    (primitive ("equal?") equal?-prim)
    (primitive ("zero?") zero?-prim)
    (primitive ("greater?") greater?-prim)
    (primitive ("less?") less?-prim)
    (primitive ("cons") cons-prim)
    (primitive ("car") car-prim)
    (primitive ("cdr") cdr-prim)
    (primitive ("list") list-prim)
    (primitive ("null?") null?-prim)
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
