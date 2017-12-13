;; Homework 4 Part 1 Step 10

;; This function will return the multiplicative reciprocal of a non-zero
;; numeric input value

(define (reciprocal n)
        (if (and (number? n) (not (= n 0)))
                (/ 1 n)
                'invalid_parameter
        )
)

(define (main)
	(display (reciprocal 7)) (newline)
	(display (reciprocal (/ 1 7))) (newline)
	(display (reciprocal .7)) (newline)
	(display (reciprocal 7.7)) (newline)
	(display (reciprocal (/ 50 7))) (newline)
	(display (reciprocal "Hello World")) (newline)
)
