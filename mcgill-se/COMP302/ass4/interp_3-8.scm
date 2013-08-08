; Copied from EOPL, starting at page 73
; This one adds Section 3.3, Conditional Evaluation
; It also adds Section 3.4, Local binding
; It also adds Section 3.5, Procedures
; It also adds Section 3.6, Recursion
; It also adds Section 3.7, Variable Assignment
; It also adds "begin-exp" as in Exercise 3.39
; It also adds Section 3.8's pass-by-reference implementation

(require (lib "eopl.ss" "eopl"))

(define-datatype environment environment?
  (empty-env-record)
  (extended-env-record
    (syms (list-of symbol?))
    (vals vector?)
    (env environment?))
  (recursively-extended-env-record
    (proc-names (list-of symbol?))
    (idss (list-of (list-of symbol?)))
    (bodies (list-of expression?))
    (env environment?)))

(define empty-env
  (lambda ()
    (empty-env-record)))

(define extend-env
  (lambda (syms vals env)
    (extended-env-record syms (list->vector vals) env)))

(define extend-env-recursively
  (lambda (proc-names idss bodies old-env)
    (recursively-extended-env-record proc-names idss bodies old-env)))

(define apply-env
  (lambda (env sym)
    (deref (apply-env-ref env sym))))

(define apply-env-ref
  (lambda (env sym)
    (cases environment env
      (empty-env-record ()
	(eopl:error 'apply-env-ref "No binding for ~s" sym))
      (extended-env-record (syms vals env)
	(let ((pos (rib-find-position sym syms)))
	  (if (number? pos)
	    (a-ref pos vals)
	    (apply-env-ref env sym))))
      (recursively-extended-env-record (proc-names idss
					bodies old-env)
	(let ((pos (list-find-position sym proc-names)))
	  (if (number? pos)
	    (a-ref 0
		   (make-vector
		     1
		     (direct-target
		       (closure
			 (list-ref idss pos)
			 (list-ref bodies pos)
			 env))))
	    (apply-env-ref old-env sym)))))))

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
  (let-exp
    (ids (list-of symbol?))
    (rands (list-of expression?))
    (body expression?))
  (letrec-exp
    (proc-names (list-of symbol?))
    (idss (list-of (list-of symbol?)))
    (bodies (list-of expression?))
    (letrec-body expression?))
  (varassign-exp
    (id symbol?)
    (rhs-exp expression?))
  (begin-exp
    (exp expression?)
    (exps (list-of expression?)))
  (proc-exp
    (ids (list-of symbol?))
    (body expression?))
  (app-exp
    (rator expression?)
    (rands (list-of expression?)))
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
  (less?-prim))

(define-datatype procval procval?
  (closure
    (ids (list-of symbol?))
    (body expression?)
    (env environment?)))

(define-datatype reference reference?
  (a-ref
    (position integer?)
    (vec vector?)))

(define-datatype target target?
  (direct-target
    (expval expval?))
  (indirect-target
    (ref ref-to-direct-target?)))

(define expval?
  (lambda (x)
    (or (number? x) (procval? x))))

(define ref-to-direct-target?
  (lambda (x)
    (and
      (reference? x)
      (cases reference x
	(a-ref (pos vec)
	  (cases target (vector-ref vec pos)
	    (direct-target (v) #t)
	    (indirect-target (v) #f)))))))

(define primitive-deref
  (lambda (ref)
    (cases reference ref
      (a-ref (pos vec) (vector-ref vec pos)))))

(define primitive-setref!
  (lambda (ref val)
    (cases reference ref
      (a-ref (pos vec) (vector-set! vec pos val)))))

(define deref
  (lambda (ref)
    (cases target (primitive-deref ref)
      (direct-target (expval) expval)
      (indirect-target (ref1)
	(cases target (primitive-deref ref1)
	  (direct-target (expval) expval)
	  (indirect-target (p)
	    (eopl:error 'deref
	      "Illegal reference: ~s" ref1)))))))

(define setref!
  (lambda (ref expval)
    (let ((ref (cases target (primitive-deref ref)
		  (direct-target (expval1) ref)
		  (indirect-target (ref1) ref1))))
      (primitive-setref! ref (direct-target expval)))))

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
      (varassign-exp (id rhs-exp)
	(begin
	  (setref!
	    (apply-env-ref env id)
	    (eval-expression rhs-exp env))
	  1))
      (primapp-exp (prim rands)
	(let ((args (eval-primapp-exp-rands rands env)))
	  (apply-primitive prim args)))
      (if-exp (test-exp true-exp false-exp)
	(if (true-value? (eval-expression test-exp env))
	  (eval-expression true-exp env)
	  (eval-expression false-exp env)))
      (let-exp (ids rands body)
	(let ((args (eval-let-exp-rands rands env)))
	  (eval-expression body (extend-env ids args env))))
      (letrec-exp (proc-names idss bodies letrec-body)
	(eval-expression letrec-body
	  (extend-env-recursively proc-names idss bodies env)))
      (begin-exp (exp exps)
	(if (null? exps)
	  (eval-expression exp env)
	  (begin
	    (eval-expression exp env)
	    (eval-expression (begin-exp (car exps) (cdr exps)) env))))
      (proc-exp (ids body) (closure ids body env))
      (app-exp (rator rands)
	(let ((proc (eval-expression rator env))
	      (args (eval-rands rands env)))
	  (if (procval? proc)
	    (apply-procval proc args)
	    (eopl:error 'eval-expression
	      "Attempt to apply non-procedure ~s" proc))))
      )))

(define eval-primapp-exp-rands
  (lambda (rands env)
    (map (lambda (x) (eval-expression x env)) rands)))

(define eval-let-exp-rands
  (lambda (rands env)
    (map (lambda (x) (eval-let-exp-rand x env)) rands)))

(define eval-let-exp-rand
  (lambda (rand env)
    (direct-target (eval-expression rand env))))

(define eval-rands
  (lambda (rands env)
    (map (lambda (x) (eval-rand x env)) rands)))

(define eval-rand
  (lambda (rand env)
    (cases expression rand
      (var-exp (id)
	(indirect-target
	  (let ((ref (apply-env-ref env id)))
	    (cases target (primitive-deref ref)
	      (direct-target (expval) ref)
	      (indirect-target (ref1) ref1)))))
      (else
	(direct-target (eval-expression rand env))))))

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
      )))

(define apply-procval
  (lambda (proc args)
    (cases procval proc
      (closure (ids body env)
	       (eval-expression body (extend-env ids args env))))))

(define init-env
  (lambda ()
    (extend-env
      '(i v x)
      '(1 5 10)
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
      ("let" (arbno identifier "=" expression) "in" expression)
      let-exp)
    (expression
      ("letrec" (arbno identifier "(" (separated-list identifier ",") ")"
		       "=" expression)
       "in" expression)
      letrec-exp)
    (expression
      ("set" identifier "=" expression)
      varassign-exp)
    (expression
      ("begin" expression (arbno ";" expression) "end")
      begin-exp)
    (expression
      ("proc" "(" (separated-list identifier ",") ")" expression)
      proc-exp)
    (expression
      ("(" expression (arbno expression) ")")
      app-exp)
    (primitive ("+") add-prim)
    (primitive ("-") subtract-prim)
    (primitive ("*") mult-prim)
    (primitive ("add1") incr-prim)
    (primitive ("sub1") decr-prim)
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
