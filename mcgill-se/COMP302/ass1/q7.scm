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

