(define x 7)

(define set-x (lambda (a) (set! x a)))

(define get-x (lambda () x))

(set-x 12)

(get-x ())

(define x 22)

(get-x ())

(define get-new-x (lambda () x))

(set-x 37)

(get-new-x ())

(define y x)

(define get-y (lambda () y))

(define set-y (lambda (a) (set! y a)))

(define product (lambda () (* x y)))

(define get-product (lambda () (* (get-x ()) (get-y ()))))

(set-y 783)

(set! x 543)

