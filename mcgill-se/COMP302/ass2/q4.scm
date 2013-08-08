(require (lib "eopl.ss" "eopl"))

(define-datatype set set?
  (comprehended-set (p procedure?))
  (unioned-set (set1 set?) (set2 set?))
  (intersected-set (set1 set?) (set2 set?))
  (complemented-set (s set?)))

(define set-comprehend
  (lambda (p)
    (comprehended-set p)))

(define set-member?
  (lambda (x s)
    (cases set s
      (comprehended-set (p) (p x))
      (unioned-set (set1 set2)
	(or (set-member? x set1)
	    (set-member? x set2)))
      (intersected-set (set1 set2)
	(and (set-member? x set1)
	     (set-member? x set2)))
      (complemented-set (s)
	(not (set-member? x s))))))

(define set-union
  (lambda (set1 set2)
    (unioned-set set1 set2)))

(define set-intersection
  (lambda (set1 set2)
    (intersected-set set1 set2)))

(define set-complement
  (lambda (s)
    (complemented-set s)))
