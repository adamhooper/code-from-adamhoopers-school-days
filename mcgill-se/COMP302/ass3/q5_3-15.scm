; Add booleans, changing the grammar
;
; Steps to take:
; 1. Modify the grammar
;      (expression
;        ("if" bool-exp "then" expression "else" expression)
;        if-exp)
;      (bool-exp
;        (expression)
;        bool-exp)
; 2. Add an expression datatype
;      (bool-exp
;        (expr expression?))
; 3. Add bool-exp to eval-expression
;      (bool-exp (expr)
;        (eval-bool-exp expr env))
; 4. Create eval-bool-exp, which returns #t or #f
;      (define eval-bool-exp
;        (lambda (expr env)
;          (let ((ans (eval-expression expr env)))
;            (if (boolean? ans)
;              ans
;              (eopl:error 'eval-bool-exp
;                          "Not a boolean expression: ~s" expr)))))
; 5. Remove true-value?, and remove its call from eval-expression's if-exp
;      (if-exp (test-exp true-exp false-exp)
;        (if (eval-expression test-exp env)
;          (eval-expression true-exp env)
;          (eval-expression false-exp env)))
; 6. Modify our primitives again
;      (equal?-prim () (equal? (car args) (cadr args)))
;      (zero?-prim () (zero? (car args)))
;      (greater?-prim () (> (car args) (cadr args)))
;      (less?-prim () (< (car args) (cadr args)))
;
; What follows is the modified interpreter.

; Copied from EOPL, starting at page 73
; This one adds Section 3.3, Conditional Evaluation
; It also adds all the things in q4_3-11.scm

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
  (bool-exp
    (expr expression?))
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
	(if (eval-expression test-exp env)
	  (eval-expression true-exp env)
	  (eval-expression false-exp env)))
      (bool-exp (expr)
	(eval-bool-exp expr env))
      )))

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
      (equal?-prim () (equal? (car args) (cadr args)))
      (zero?-prim () (zero? (car args)))
      (greater?-prim () (> (car args) (cadr args)))
      (less?-prim () (< (car args) (cadr args)))
      )))

(define init-env
  (lambda ()
    (extend-env
      '(i v x)
      '(1 5 10)
      (empty-env))))

(define eval-bool-exp
  (lambda (expr env)
    (let ((ans (eval-expression expr env)))
      (if (boolean? ans)
	ans
	(eopl:error 'eval-bool-exp
		    "Not a boolean expression: ~s" expr)))))

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
      ("if" bool-exp "then" expression "else" expression)
      if-exp)
    (bool-exp
      (expression)
      bool-exp)
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
