; Defines
;
; Steps to take:
; 1. Add to the grammar
;    (form
;      ("define" identifier "=" expression)
;      define-form)
;    (form
;      (expression)
;      exp-form)
; 2. Add a datatype for forms
;  (define-datatype form form?
;    (define-form
;      (id symbol?)
;      (exp expression?))
;    (exp-form
;      (exp expression?)))
; 3. Change the way the environment works. We need a new type of environment,
;    the "init-env-record" type, a pair of lists. All environments will be
;    extended from init-env-record, so when we modify it everything will be
;    affected.
;
;      (define-datatype environment environment?
;        (init-env-record
;          (syms (list-of symbol?))
;          (vals (list-of (lambda (x) #t))))
;        (extended-env-record
;          (syms (list-of symbol?))
;          (vals vector?)
;          (env environment?)))
;
;      (define init-env
;        (lambda ()
;          (init-env-record '(i v x) '(1 5 10))))
;      
;      (define list-set!
;        (lambda (lst pos val)
;          (if (= pos 0)
;            (set-car! lst val)
;            (list-set! (cdr lst) (- pos 1) val))))
;      
;      (define list-append!
;        (lambda (lst val)
;          (if (null? (cdr lst))
;            (set-cdr! lst (cons val '()))
;            (list-append! (cdr lst) val))))
;      
;      (define modify-init-env
;        (lambda (env sym exp) ; evaluate exp in initial environment
;          (cases environment env
;            (extended-env-record (syms vals env)
;              (modify-init-env sym val env))
;            (init-env-record (syms vals)
;              (let ((pos (list-find-position sym syms))
;                    (val (eval-expression exp env)))
;                (if (number? pos)
;                  (list-set! vals pos val)
;                  (begin
;                    (list-append! syms sym)
;                    (list-append! vals val))))))))
;
;    And finally, add the special case to apply-env-ref. It won't actually
;    return a reference, it will return a copy.
;      (init-env-record (syms vals)
;        (let ((pos (list-find-position sym syms)))
;          (if (number? pos)
;            (a-ref 0 (make-vector 1 (list-ref vals pos)))
;            (eopl:error 'apply-env-ref "Could not find sym ~s" sym))))
;
; 4. Write eval-form
;      (define eval-form
;        (lambda (frm env)
;          (cases form frm
;            (define-form (id exp)
;              (modify-init-env env id exp))
;            (exp-form (exp)
;              (eval-expression exp env)))))
;
; 5. Change read-eval-print, inserting an environment
;      (define read-eval-print
;        (let ((env (init-env)))
;          (sllgen:make-rep-loop "--> "
;            (lambda (x)
;              (let ((y (eval-form x env)))
;                (if (void? y)
;                  'okay ; FIXME: shouldn't print anything
;      	           y)))
;            (sllgen:make-stream-parser
;              scanner-spec-3-1
;              grammar-3-1))))
;
;    NOTE: I couldn't find a way to make the loop print *nothing*; I could only
;          get it to print something meaningless. I figure since that has
;          nothing to do with the question, really, that the oversight is okay.
;
; 6. Remove all reference to "empty-env".
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
  (init-env-record
    (syms (list-of symbol?))
    (vals (list-of (lambda (x) #t))))
  (extended-env-record
    (syms (list-of symbol?))
    (vals vector?)
    (env environment?)))

(define init-env
  (lambda ()
    (init-env-record '(i v x) '(1 5 10))))

(define list-set!
  (lambda (lst pos val)
    (if (= pos 0)
      (set-car! lst val)
      (list-set! (cdr lst) (- pos 1) val))))

(define list-append!
  (lambda (lst val)
    (if (null? (cdr lst))
      (set-cdr! lst (cons val '()))
      (list-append! (cdr lst) val))))

(define modify-init-env
  (lambda (env sym exp) ; evaluate exp in initial environment
    (cases environment env
      (extended-env-record (syms vals env)
	(modify-init-env sym val env))
      (init-env-record (syms vals)
	(let ((pos (list-find-position sym syms))
	      (val (eval-expression exp env)))
	  (if (number? pos)
	    (list-set! vals pos val)
	    (begin
	      (list-append! syms sym)
	      (list-append! vals val))))))))

(define extend-env
  (lambda (syms vals env)
    (extended-env-record syms (list->vector vals) env)))

(define apply-env
  (lambda (env sym)
    (deref (apply-env-ref env sym))))

(define apply-env-ref
  (lambda (env sym)
    (cases environment env
      (init-env-record (syms vals)
	(let ((pos (list-find-position sym syms)))
	  (if (number? pos)
	    (a-ref 0 (make-vector 1 (list-ref vals pos)))
	    (eopl:error 'apply-env-ref "Could not find sym ~s" sym))))
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

(define-datatype form form?
  (define-form
    (id symbol?)
    (exp expression?))
  (exp-form
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

(define eval-form
  (lambda (frm env)
    (cases form frm
      (define-form (id exp)
	(modify-init-env env id exp))
      (exp-form (exp)
	(eval-expression exp env)))))

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
      )))

(define apply-procval
  (lambda (proc args)
    (cases procval proc
      (closure (ids body env)
	(eval-expression body (extend-env ids args env))))))

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
  '(;(program
    ;  (expression)
    ;  a-program)
    (form
      ("define" identifier "=" expression)
      define-form)
    (form
      (expression)
      exp-form)
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
  (let ((env (init-env)))
    (sllgen:make-rep-loop "--> "
      (lambda (x)
	(let ((y (eval-form x env)))
	  (if (void? y)
	    'okay ; FIXME: shouldn't print anything
	    y)))
      (sllgen:make-stream-parser
        scanner-spec-3-1
        grammar-3-1))))
