; Copied from EOPL, starting at page 73
; This one adds Section 3.3, Conditional Evaluation
; It also adds Section 3.4, Local binding
; It also adds Section 3.5, Procedures
; It also implements lists as in Exercise 3.17
; It does NOT add Section 3.6, Recursion
; It also adds Section 3.7, Variable Assignment
; It also adds "begin-exp" as in Exercise 3.39
; It does NOT add Section 3.8's pass-by-reference implementation
; It also adds Chapter 5's "simple" object-oriented design

(require (lib "eopl.ss" "eopl"))

(define-datatype environment environment?
  (empty-env-record)
  (extended-env-record
    (syms (list-of symbol?))
    (vals vector?)
    (env environment?)))

(define empty-env
  (lambda ()
    (empty-env-record)))

(define extend-env
  (lambda (syms vals env)
    (extended-env-record syms (list->vector vals) env)))

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
	    (apply-env-ref env sym)))))))

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

(define the-class-env '())

(define elaborate-class-decls!
  (lambda (c-decls)
    (set! the-class-env c-decls)))

(define lookup-class
  (lambda (name)
    (letrec ((loop (lambda (env)
		     (cond
		       ((null? env)
			(eopl:error 'lookup-class "Unknown class ~s" name))
		       ((eqv? (class-decl->class-name (car env)) name)
			(car env))
		       (else (loop (cdr env)))))))
      (loop the-class-env))))

(define new-object
  (lambda (class-name)
    (if (eqv? class-name 'object)
      '()
      (let ((c-decl (lookup-class class-name)))
	(cons
	  (make-first-part c-decl)
	  (new-object
	    (class-decl->super-name c-decl)))))))

(define make-first-part
  (lambda (c-decl)
    (a-part
      (class-decl->class-name c-decl)
      (make-vector
	(length (class-decl->field-ids c-decl))))))

(define object->class-name
  (lambda (obj)
    (part->class-name (car obj))))

(define build-field-env
  (lambda (parts)
    (if (null? parts)
      (empty-env)
      (extend-env-refs
	(part->field-ids (car parts))
	(part->fields (car parts))
	(build-field-env (cdr parts))))))

(define extend-env-refs
  (lambda (syms vec env)
    (extended-env-record syms vec env)))

(define-datatype program program?
  (a-program
    (class-decls (list-of class-decl?))
    (exp expression?)))

(define-datatype class-decl class-decl?
  (a-class-decl
    (class-name symbol?)
    (super-name symbol?)
    (field-ids (list-of symbol?))
    (method-decls (list-of method-decl?))))

(define class-decl->class-name
  (lambda (decl)
    (cases class-decl decl
      (a-class-decl (class-name super-name field-ids method-decls)
	class-name))))

(define class-decl->super-name
  (lambda (decl)
    (cases class-decl decl
      (a-class-decl (class-name super-name field-ids method-decls)
	super-name))))

(define class-decl->field-ids
  (lambda (decl)
    (cases class-decl decl
      (a-class-decl (class-name super-name field-ids method-decls)
	field-ids))))

(define class-decl->method-decls
  (lambda (decl)
    (cases class-decl decl
      (a-class-decl (class-name super-name field-ids method-decls)
	method-decls))))

(define class-name->super-name
  (lambda (class-name)
    (class-decl->super-name (lookup-class class-name))))

(define class-name->method-decls
  (lambda (class-name)
    (class-decl->method-decls (lookup-class class-name))))

(define-datatype method-decl method-decl?
  (a-method-decl
    (method-name symbol?)
    (ids (list-of symbol?))
    (body expression?)))

(define method-decl->method-name
  (lambda (decl)
    (cases method-decl decl
      (a-method-decl (method-name ids body)
	method-name))))

(define method-decl->ids
  (lambda (decl)
    (cases method-decl decl
      (a-method-decl (method-name ids body)
	ids))))

(define method-decl->body
  (lambda (decl)
    (cases method-decl decl
      (a-method-decl (method-name ids body)
	body))))

(define-datatype part part?
  (a-part
    (class-name symbol?)
    (fields vector?)))

(define part->class-name
  (lambda (p)
    (cases part p
      (a-part (class-name fields)
	class-name))))

(define part->fields
  (lambda (p)
    (cases part p
      (a-part (class-name fields)
	fields))))

(define part->field-ids
  (lambda (p)
    (class-decl->field-ids (lookup-class (part->class-name p)))))

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
  (new-object-exp
    (class-name symbol?)
    (rands (list-of expression?)))
  (method-app-exp
    (obj-exp expression?)
    (method-name symbol?)
    (rands (list-of expression?)))
  (super-call-exp
    (method-name symbol?)
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
  (less?-prim)
  (cons-prim)
  (car-prim)
  (cdr-prim)
  (list-prim)
  )

(define-datatype procval procval?
  (closure
    (ids (list-of symbol?))
    (body expression?)
    (env environment?)))

(define-datatype reference reference?
  (a-ref
    (position integer?)
    (vec vector?)))

(define expval?
  (lambda (x)
    (or (number? x) (procval? x))))

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
    (primitive-deref ref)))

(define setref!
  (lambda (ref expval)
    (primitive-setref! ref expval)))

(define eval-program
  (lambda (pgm)
    (cases program pgm
      (a-program (c-decls exp)
	(elaborate-class-decls! c-decls)
	(eval-expression exp (empty-env))))))

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
	(let ((args (eval-rands rands env)))
	  (apply-primitive prim args)))
      (if-exp (test-exp true-exp false-exp)
	(if (true-value? (eval-expression test-exp env))
	  (eval-expression true-exp env)
	  (eval-expression false-exp env)))
      (let-exp (ids rands body)
	(let ((args (eval-rands rands env)))
	  (eval-expression body (extend-env ids args env))))
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
      (method-app-exp (obj-exp method-name rands)
	(let ((args (eval-rands rands env))
	      (obj (eval-expression obj-exp env)))
	  (find-method-and-apply
	    method-name (object->class-name obj) obj args)))
      (super-call-exp (method-name rands)
	(let ((args (eval-rands rands env))
	      (obj (apply-env env 'self)))
	  (find-method-and-apply
	    method-name (apply-env env '%super) obj args)))
      (new-object-exp (class-name rands)
	(let ((args (eval-rands rands env))
	      (obj (new-object class-name)))
	  (find-method-and-apply
	    'initialize class-name obj args)
	  obj))
      )))

(define find-method-and-apply
  (lambda (m-name host-name self args)
    (if (eqv? host-name 'object)
      (eopl:error 'find-method-and-apply "No method for name ~s" m-name)
      (let ((m-decl (lookup-method-decl
		      m-name (class-name->method-decls host-name))))
	(if (method-decl? m-decl)
	  (apply-method m-decl host-name self args)
	  (find-method-and-apply
	    m-name (class-name->super-name host-name) self args))))))

(define lookup-method-decl
  (lambda (m-name decls)
    (if (null? decls)
      #f
      (if (eq? (method-decl->method-name (car decls)) m-name)
	(car decls)
	(lookup-method-decl m-name (cdr decls))))))

(define apply-method
  (lambda (m-decl host-name self args)
    (let ((ids (method-decl->ids m-decl))
	  (body (method-decl->body m-decl))
	  (super-name (class-name->super-name host-name)))
      (eval-expression
	body
	(extend-env
	  (cons '%super (cons 'self ids))
	  (cons super-name (cons self args))
	  (build-field-env (view-object-as self host-name)))))))

(define view-object-as
  (lambda (parts class-name)
    (if (eqv? (part->class-name (car parts)) class-name)
      parts
      (view-object-as (cdr parts) class-name))))

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
      (equal?-prim () (if (equal? (car args) (cadr args)) 1 0))
      (zero?-prim () (if (zero? (car args)) 1 0))
      (greater?-prim () (if (> (car args) (cadr args)) 1 0))
      (less?-prim () (if (< (car args) (cadr args)) 1 0))
      (cons-prim () (cons (car args) (cadr args)))
      (car-prim () (car (car args)))
      (cdr-prim () (cdr (car args)))
      (list-prim () args)
      )))

(define apply-procval
  (lambda (proc args)
    (cases procval proc
      (closure (ids body env)
	       (eval-expression body (extend-env ids args env))))))

(define true-value?
  (lambda (x)
    (not (zero? x))))

(define scanner-spec-5
  '((white-sp
      (whitespace)				skip)
    (comment
      ("%" (arbno (not #\newline)))		skip)
    (identifier
      (letter (arbno (or letter digit "?")))	symbol)
    (number
      (digit (arbno digit))			number)))

(define grammar-5
  '((program
      ((arbno class-decl)
       expression)
      a-program)
    (class-decl
      ("class" identifier "extends" identifier
       (arbno "field" identifier)
       (arbno method-decl))
      a-class-decl)
    (method-decl
      ("method" identifier
       "(" (separated-list identifier ",") ")"
       expression)
      a-method-decl)
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
    (expression
      ("new" identifier "(" (separated-list identifier ",") ")")
      new-object-exp)
    (expression
      ("send" expression identifier
       "(" (separated-list expression ",") ")")
      method-app-exp)
    (expression
      ("super" identifier "(" (separated-list identifier ",") ")")
      super-call-exp)
    (primitive ("+") add-prim)
    (primitive ("-") subtract-prim)
    (primitive ("*") mult-prim)
    (primitive ("add1") incr-prim)
    (primitive ("sub1") decr-prim)
    (primitive ("equal?") equal?-prim)
    (primitive ("zero?") zero?-prim)
    (primitive ("greater?") greater?-prim)
    (primitive ("less?") less?-prim)
    (primitive ("cons") cons-prim)
    (primitive ("car") car-prim)
    (primitive ("cdr") cdr-prim)
    (primitive ("list") list-prim)
    ))

(define scan&parse
  (sllgen:make-string-parser
    scanner-spec-5
    grammar-5))
;(sllgen:make-define-datatypes scanner-spec-3-1 grammar-3-1)
(define run
  (lambda (string)
    (eval-program
      (scan&parse string))))
(define read-eval-print
  (sllgen:make-rep-loop "--> " eval-program
    (sllgen:make-stream-parser
      scanner-spec-5
      grammar-5)))
