(define survives?
  (lambda (pos n)
    (if (<= n 2)
        #t
        (if (= pos 3)
            #f
            (survives?
              (modulo (+ n (- pos 3)) n)
              (- n 1))))))

