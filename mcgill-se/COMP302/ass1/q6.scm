(load "q5.scm")

(define has-complement-factor
  (lambda (f start n)
    (if (> start n)
      #f
      (if (= n (* f start))
        #t
        (has-complement-factor f (+ 1 start) n)))))

(define has-factor
  (lambda (f n)
    (if (> f n)
      #f
      (if (has-complement-factor f 2 n)
        #t
        (has-factor (+ 1 f) n)))))

(define prime?
  (lambda (n)
    (if (< n 2)
      #f
      (if (= n 2)
        #t
        (not (has-factor 2 n))))))

(define sum-primes-squared
  (lambda (m n)
    (filtered-accumulate
      prime?
      +
      0
      (lambda (x) (* x x))
      m
      (lambda (x) (+ 1 x))
      n)))

(define product-non-divisible
  (lambda (n)
    (filtered-accumulate
      (lambda (x) (= 1 (gcd x n)))
      *
      1
      (lambda (x) x)
      1
      (lambda (x) (+ 1 x))
      n)))

