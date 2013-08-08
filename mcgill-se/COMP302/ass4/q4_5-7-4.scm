; Store host class name instead of superclass name
;
; Steps to take:
; 1. Change "super-name" in method's define-datatype to "class-name"
; 2. Change "super-name" to "class-name" in all "cases method" clauses
;    method->super-name thus changes to this:
;	(define method->super-name
;	  (lambda (m)
;	    (cases method m
;	      (a-method (method-decl class-name field-ids)
;		(class-name->super-name class-name)))))
; 3. Modify roll-up-method-decls
;	(define roll-up-method-decls
;	  (lambda (c-decl class-name field-ids)
;	    (merge-methods
;	      (class-name->methods (class-decl->super-name c-decl))
;	      (map
;		(lambda (m-decl)
;		  (a-method m-decl class-name field-ids))
;		(class-decl->method-decls c-decl)))))
;    Also, change its call from elaborate-class-decl! to conform
;	    (roll-up-method-decls
;	      c-decl (class-decl->class-name c-decl) field-ids)))))))
; 4. Change apply-method to remove "super-name" from the let clause
;	(define apply-method
;	  (lambda (method host-name self args)
;	    (let ((ids (method->ids method))
;		  (body (method->body method))
;		  (field-ids (method->field-ids method))
;		  (fields (object->fields self)))
;	      (eval-expression
;		body
;		(extend-env
;		  (cons '%super (cons 'self ids))
;		  (cons (method->super-name method) (cons self args))
;		  (extend-env-refs field-ids fields (empty-env)))))))
;
; The rest of this file is the implementation

; Copied from EOPL, starting at page 73
; This one adds Section 3.3, Conditional Evaluation
; It also adds Section 3.4, Local binding
; It also adds Section 3.5, Procedures
; It also implements lists as in Exercise 3.17
; It does NOT add Section 3.6, Recursion
; It also adds Section 3.7, Variable Assignment
; It also adds "begin-exp" as in Exercise 3.39
; It does NOT add Section 3.8's pass-by-reference implementation
; It also adds Chapter 5's "flat method environments" object-oriented design

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

(define rib-find-position
  (lambda (name symbols)
    (list-find-last-position name symbols)))

(define list-find-last-position
  (lambda (sym los)
    (list-last-index (lambda (sym1) (eqv? sym1 sym)) los)))

(define list-last-index
  (lambda (pred ls)
    (if (null? ls)
      #f
      (let ((list-index-r (list-last-index pred (cdr ls))))
	(if (number? list-index-r)
	  (+ list-index-r 1)
	  (if (pred (car ls))
	    0
	    #f))))))

(define the-class-env '())

(define elaborate-class-decls!
  (lambda (c-decls)
    (initialize-class-env!)
    (for-each elaborate-class-decl! c-decls)))

(define elaborate-class-decl!
  (lambda (c-decl)
    (let ((super-name (class-decl->super-name c-decl)))
      (let ((field-ids (append
			 (class-name->field-ids super-name)
			 (class-decl->field-ids c-decl))))
	(add-to-class-env!
	  (a-class
	    (class-decl->class-name c-decl)
	    super-name
	    (length field-ids)
	    field-ids
	    (roll-up-method-decls
	      c-decl (class-decl->class-name c-decl) field-ids)))))))

(define initialize-class-env!
  (lambda ()
    (set! the-class-env '())))

(define add-to-class-env!
  (lambda (cls)
    (set! the-class-env (cons cls the-class-env))))

(define roll-up-method-decls
  (lambda (c-decl class-name field-ids)
    (merge-methods
      (class-name->methods (class-decl->super-name c-decl))
      (map
	(lambda (m-decl)
	  (a-method m-decl class-name field-ids))
	(class-decl->method-decls c-decl)))))

(define merge-methods
  (lambda (super-methods methods)
    (cond
      ((null? super-methods) methods)
      (else
	(let ((overriding-method
		(lookup-method
		  (method->method-name (car super-methods))
		  methods)))
	  (if (method? overriding-method)
	    (cons overriding-method
	      (merge-methods (cdr super-methods)
		(remove-method overriding-method methods)))
	    (cons (car super-methods)
	      (merge-methods (cdr super-methods)
		methods))))))))

(define remove-method
  (lambda (m lst)
    (if (eq? m (car lst))
      (cdr lst)
      (cons (car lst) (remove-method (cdr lst))))))

(define lookup-class
  (lambda (name)
    (letrec ((loop (lambda (env)
		     (cond
		       ((null? env)
			(eopl:error 'lookup-class "Unknown class ~s" name))
		       ((eqv? (class->class-name (car env)) name)
			(car env))
		       (else (loop (cdr env)))))))
      (loop the-class-env))))

(define new-object
  (lambda (class-name)
    (an-object
      class-name
      (make-vector (roll-up-field-length class-name)))))

(define roll-up-field-length
  (lambda (class-name)
    (if (eqv? class-name 'object)
      0
      (+
	(roll-up-field-length
	  (class-name->super-name class-name))
	(length (class-name->field-ids class-name))))))

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

(define-datatype method method?
  (a-method
    (method-decl method-decl?)
    (class-name symbol?)
    (field-ids (list-of symbol?))))

(define method->method-decl
  (lambda (m)
    (cases method m
      (a-method (method-decl class-name field-ids)
	method-decl))))

(define method->method-name
  (lambda (m)
    (cases method m
      (a-method (method-decl class-name field-ids)
	(method-decl->method-name method-decl)))))

(define method->super-name
  (lambda (m)
    (cases method m
      (a-method (method-decl class-name field-ids)
	(class-name->super-name class-name)))))

(define method->field-ids
  (lambda (m)
    (cases method m
      (a-method (method-decl class-name field-ids)
	field-ids))))

(define method->ids
  (lambda (m)
    (cases method m
      (a-method (method-decl class-name field-ids)
	(method-decl->ids method-decl)))))

(define method->body
  (lambda (m)
    (cases method m
      (a-method (method-decl class-name field-ids)
	(method-decl->body method-decl)))))

(define-datatype class class?
  (a-class
    (class-name symbol?)
    (super-name symbol?)
    (field-length integer?)
    (field-ids (list-of symbol?))
    (methods method-environment?)))

(define method-environment? (list-of method?))

(define class->class-name
  (lambda (cls)
    (cases class cls
      (a-class (class-name super-name field-length field-ids methods)
	class-name))))

(define class->super-name
  (lambda (cls)
    (cases class cls
      (a-class (class-name super-name field-length field-ids methods)
	super-name))))

(define class->field-ids
  (lambda (cls)
    (cases class cls
      (a-class (class-name super-name field-length field-ids methods)
	field-ids))))

(define class->methods
  (lambda (cls)
    (cases class cls
      (a-class (class-name super-name field-length field-ids methods)
	methods))))

(define class-name->super-name
  (lambda (class-name)
    (class->super-name (lookup-class class-name))))

(define class-name->field-ids
  (lambda (class-name)
    (if (eq? class-name 'object)
      '()
      (class->field-ids (lookup-class class-name)))))

(define class-name->methods
  (lambda (class-name)
    (if (eq? class-name 'object)
      '()
      (class->methods (lookup-class class-name)))))

(define-datatype object object?
  (an-object
    (class-name symbol?)
    (fields vector?)))

(define object->class-name
  (lambda (obj)
    (cases object obj
      (an-object (class-name fields)
	class-name))))

(define object->fields
  (lambda (obj)
    (cases object obj
      (an-object (class-name fields)
	fields))))

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
    (let ((method (lookup-method m-name
		    (class-name->methods host-name))))
      (if (method? method)
	(apply-method method host-name self args)
	(eopl:error 'find-method-and-apply "No method for name ~s" m-name)))))

(define lookup-method
  (lambda (m-name methods)
    (let ((pos (list-last-index
		 (lambda (x) (eq?
			       (method-decl->method-name
				 (method->method-decl x))
			       m-name))
		 methods)))
      (if (number? pos)
	(list-ref methods pos)
	#f))))

(define apply-method
  (lambda (method host-name self args)
    (let ((ids (method->ids method))
	  (body (method->body method))
	  (field-ids (method->field-ids method))
	  (fields (object->fields self)))
      (eval-expression
	body
	(extend-env
	  (cons '%super (cons 'self ids))
	  (cons (method->super-name method) (cons self args))
	  (extend-env-refs field-ids fields (empty-env)))))))

(define roll-up-field-ids
  (lambda (class-name)
    (if (eqv? class-name 'object)
      '()
      (append
	(roll-up-field-ids
	  (class-name->super-name class-name))
	(class-name->field-ids class-name)))))

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
