(load "q4.scm")

(define filtered-accumulate
  (lambda (p comb empty f m next n)
    (if (> m n)
      empty
      (comb (if (p m) (f m) empty)
            (filtered-accumulate p comb empty f (next m) next n)))))

