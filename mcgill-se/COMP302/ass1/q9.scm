(define apply-all
  (lambda (lst val)
    (if (null? lst)
      null
      (cons ((car lst) val)
            (apply-all (cdr lst) val)))))

