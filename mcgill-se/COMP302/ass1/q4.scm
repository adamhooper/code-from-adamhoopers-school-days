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

