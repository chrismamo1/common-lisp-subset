(define + (lambda (a b) (plus a b)))

(define - (lambda (a b) (minus a b)))

(define * (lambda (a b) (times a b)))

(define / (lambda (a b) (over a b)))

(define gt (lambda (a b) (if (= 0 (- a b)) f (if (ispos (- a b)) t f ))))

(define and (lambda (a b) (if a (if b t f) f)))

(define not (lambda (x) (if x f t)))

(define or (lambda (a b) (if a t (if b t f))))

(define lt (lambda (a b) (if (or (= a b) (> a b)) f t)))

(define gte (lambda (a b) (or (gt a b) (= a b))))

(define lte (lambda (a b) (not (gt a b)))i)

(define <= lte)

(define >= gte)

(define > gt)

(define < lt)

(define compose (lambda (f g)  (lambda (x)  (f (g x))))i)

(define repeat (lambda (f)  (compose f f)))

(define fact (lambda (n)  (if (<= n 1) 1  (* n (fact (- n 1))))))

(define car (lambda (seq) (_car seq)))

(define cdr (lambda (seq) (_cdr seq)))

(define length (lambda (seq) (if (= (cdr seq) nil) 1 (+ 1 (length (cdr seq))))))

(define last (lambda (seq) (if (= (cdr seq) nil) (car seq) (last (cdr seq)))))

(define null? (lambda (x) (= x nil)))
