(load "q2.scm")

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

