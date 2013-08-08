; Adding deref and setref primitives, then a new production...
;
; Steps to take:
; 1. Add the primitives to the primitive datatype
;      (deref-prim)
;      (setref-prim)
;
; 2. Add to apply-primitive
;      (deref-prim () (deref (car args)))
;      (setref-prim () (setref (car args) (cadr args)))
;
; 3. Add to the grammar
;      (primitive ("deref") deref-prim)
;      (primitive ("setref") setref-prim)
;
; 4. Add to the grammar
;      (expression
;        ("ref" identifier)
;        ref-exp)
;
; 5. Add to the expression datatype
;      (ref-exp
;        (id symbol?))
;
; 6. Add to eval-expression
;      (ref-exp (id)
;        (apply-env-ref (env id)))
;
; One of the tests is from the book:
; let a = 3
;     b = 4
;     swap = proc (x,y)
;              let temp = deref(x)
;              in begin
;                   setref(x,deref(y));
;                   setref(y,temp)
;                 end
; in begin
;      (swap ref a ref b);
;      -(a,b)
;    end
;
; The answer: a and b are swapped, so a = 4 and b = 3. a - b is thus 1.
;
; The expressed and denoted values now include references to other values
; (including other references, like C).
;
; The rest of this file is the implementation.

; Copied from EOPL, starting at page 73
; This one adds Section 3.3, Conditional Evaluation
; It also adds Section 3.4, Local binding
; It also adds Section 3.5, Procedures
; It does NOT add Section 3.6, Recursion
; It also adds Section 3.7, Variable Assignment
; It also adds "begin-exp" as in Exercise 3.39

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
  (ref-exp
    (id symbol?))
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
  (less?-prim)
  (deref-prim)
  (setref-prim)
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
  (lambda (ref val)
    (primitive-setref! ref val)))

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
      (ref-exp (id)
	(apply-env-ref env id))
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
      (equal?-prim () (if (equal? (car args) (cadr args)) 1 0))
      (zero?-prim () (if (zero? (car args)) 1 0))
      (greater?-prim () (if (> (car args) (cadr args)) 1 0))
      (less?-prim () (if (< (car args) (cadr args)) 1 0))
      (deref-prim () (deref (car args)))
      (setref-prim () (setref! (car args) (cadr args)))
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
      ("ref" identifier)
      ref-exp)
    (primitive ("+") add-prim)
    (primitive ("-") subtract-prim)
    (primitive ("*") mult-prim)
    (primitive ("add1") incr-prim)
    (primitive ("sub1") decr-prim)
    (primitive ("equal?") equal?-prim)
    (primitive ("zero?") zero?-prim)
    (primitive ("greater?") greater?-prim)
    (primitive ("less?") less?-prim)
    (primitive ("deref") deref-prim)
    (primitive ("setref") setref-prim)
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
