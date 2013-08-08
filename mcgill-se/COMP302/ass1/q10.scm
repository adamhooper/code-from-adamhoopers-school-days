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

