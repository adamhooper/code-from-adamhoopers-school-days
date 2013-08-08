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

