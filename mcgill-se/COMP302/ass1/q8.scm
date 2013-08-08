(load "q7.scm")

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

