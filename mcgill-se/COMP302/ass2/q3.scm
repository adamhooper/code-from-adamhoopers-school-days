(define set-comprehend
  (lambda (x) x))

(define set-member?
  (lambda (x set) (set x)))

(define set-union
  (lambda (set1 set2)
    (lambda (x)
      (or (set-member? x set1) (set-member? x set2)))))

(define set-intersection
  (lambda (set1 set2)
    (lambda (x)
      (and (set-member? x set1) (set-member? x set2)))))

(define set-complement
  (lambda (set)
    (lambda (x)
      (not (set-member? x set)))))
