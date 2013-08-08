; Modifying the interpreter to report primitive calls with wrong number of args
;
; Change apply-primitive to the following:
;   (define apply-primitive
;     (lambda (prim args)
;       (cases primitive prim
;         (add-prim ()
;   	(if (eq? (length args) 2)
;   	  (+ (car args) (cadr args))
;   	  (eopl:error 'add-prim "Wrong number of arguments")))
;         (subtract-prim ()
;   	(if (eq? (length args) 2)
;   	  (- (car args) (cadr args))
;   	  (eopl:error 'subtract-prim "Wrong number of arguments")))
;         (mult-prim ()
;   	(if (eq? (length args) 2)
;   	  (* (car args) (cadr args))
;   	  (eopl:error 'mult-prim "Wrong number of arguments")))
;         (incr-prim ()
;   	(if (eq? (length args) 1)
;   	  (+ (car args) 1)
;   	  (eopl:error 'incr-prim "Wrong number of arguments")))
;         (decr-prim ()
;   	(if (eq? (length args) 1)
;   	  (- (car args) 1)
;   	  (eopl:error 'decr-prim "Wrong number of arguments")))
;         )))
;
; FIXME: Implement statically?
;
; What follows is the implementation.

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
  (decr-prim))

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
      (add-prim ()
	(if (eq? (length args) 2)
	  (+ (car args) (cadr args))
	  (eopl:error 'add-prim "Wrong number of arguments")))
      (subtract-prim ()
	(if (eq? (length args) 2)
	  (- (car args) (cadr args))
	  (eopl:error 'subtract-prim "Wrong number of arguments")))
      (mult-prim ()
	(if (eq? (length args) 2)
	  (* (car args) (cadr args))
	  (eopl:error 'mult-prim "Wrong number of arguments")))
      (incr-prim ()
	(if (eq? (length args) 1)
	  (+ (car args) 1)
	  (eopl:error 'incr-prim "Wrong number of arguments")))
      (decr-prim ()
	(if (eq? (length args) 1)
	  (- (car args) 1)
	  (eopl:error 'decr-prim "Wrong number of arguments")))
      )))

(define init-env
  (lambda ()
    (extend-env
      '(i v x)
      '(1 5 10)
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
      decr-prim)))

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
