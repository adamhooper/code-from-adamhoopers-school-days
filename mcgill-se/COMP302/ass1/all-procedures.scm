(define bigger2
  (lambda (x y z)
    (if (> x y)
        (if (> z y)
            (list x z)
            (list x y)
            )
        (if (> x z)
            (list x y)
            (list y z)
            )
        )))
        
(define square
  (lambda (x)
    (* x x)))

(define sum-square-list
  (lambda (lst)
    (if (eq? lst '())
        0
        (+ (square (car lst)) (sum-square-list (cdr lst))))))

(define sum-bigger2
  (lambda (x y z)
    (sum-square-list (bigger2 x y z))))

(define survives?
  (lambda (pos n)
    (if (<= n 2)
        #t
        (if (= pos 3)
            #f
            (survives?
              (modulo (+ n (- pos 3)) n)
              (- n 1))))))


(define next-person
  (lambda (pos n)
    (+ 1 (modulo pos n))))

(define next-survivor
  (lambda (pos n)
    (if (survives? (next-person pos n) n)
        (next-person pos n)
        (next-survivor (next-person pos n) n))))

(define survivors
  (lambda (n)
    (list (next-survivor 0 n) (next-survivor (next-survivor 0 n) n))))

(define accumulate
  (lambda (comb empty f m next n)
    (if (> m n)
      empty
      (comb (f m)
            (accumulate comb empty f (next m) next n)))))

(define sum
  (lambda (f m next n)
    (accumulate + 0 f m next n)))

(define product
  (lambda (f m next n)
    (accumulate * 1 f m next n)))


(define filtered-accumulate
  (lambda (p comb empty f m next n)
    (if (> m n)
      empty
      (comb (if (p m) (f m) empty)
            (filtered-accumulate p comb empty f (next m) next n)))))


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

(define first-half-helper
  (lambda (lst num)
    (if (= num 0)
      null
      (cons (car lst)
            (first-half-helper (cdr lst) (- num 1))))))

(define first-half
  (lambda (lst)
    (first-half-helper lst (ceiling (/ (length lst) 2)))))

(define second-half-helper
  (lambda (lst num)
    (if (= num 0)
      lst
      (second-half-helper (cdr lst) (- num 1)))))

(define second-half
  (lambda (lst)
    (second-half-helper lst (ceiling (/ (length lst) 2)))))

(define shuffle-helper
  (lambda (lst1 lst2 fromlst1)
    (if (null? (if fromlst1 lst1 lst2))
      null
      (cons (car (if fromlst1 lst1 lst2))
            (shuffle-helper
              (if fromlst1 (cdr lst1) lst1)
              (if fromlst1 lst2 (cdr lst2))
              (not fromlst1))))))

(define shuffle
  (lambda (lst)
    (shuffle-helper (first-half lst)
                    (second-half lst)
                    #t)))


(define same-list?
  (lambda (lst1 lst2)
    (if (null? lst1)
      (null? lst2)
      (if (null? lst2)
        #f
        (and (equal? (car lst1) (car lst2))
             (same-list? (cdr lst1) (cdr lst2)))))))

(define shuffle-number-helper
  (lambda (origlst lst num)
    (if (same-list? origlst lst)
      num
      (shuffle-number-helper origlst (shuffle lst) (+ num 1)))))

(define shuffle-number
  (lambda (lst)
    (shuffle-number-helper lst (shuffle lst) 1)))

(define apply-all
  (lambda (lst val)
    (if (null? lst)
      null
      (cons ((car lst) val)
            (apply-all (cdr lst) val)))))

(define append
  (lambda (lst item)
    (if (null? lst)
      (cons item null)
      (cons (car lst) (append (cdr lst) item)))))

;(define reverse
;  (lambda (lst)
;    (if (null? (cdr lst))
;      lst
;      (append (reverse (cdr lst)) (car lst)))))

(define deep-reverse
  (lambda (lst)
    (if (and (list? lst) (not (null? lst)))
      (append (deep-reverse (cdr lst)) (deep-reverse (car lst)))
      lst)))

