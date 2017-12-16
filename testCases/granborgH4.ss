;if n is a number, square it.
(define (square n)
  (if (number? n)
      (* n n)
      'invalid_parameter
      )
  )

;if n is a number, return 1/n 
(define (reciprocal n)
  (if (and (number? n) (not (= n 0)))
      (/ 1 n)
      'invalid_parameter
      )
  )

;copies a list
(define (list_copy ls)
  (if (list? ls)
      ls
      'list_copy1_requires_a_list_argument
      )
  )

;copies all elements in a list in odd indices of the list, first index is list[1]
(define (odd_copy ls)
  (if (list? ls)
      (if (null? ls)
	  '()
	  (if (null? (cdr ls))
		     (cons (car ls) '())
		     (cons (car ls) (odd_copy (cddr ls)))
		     )
	  )
      'odd_copy_requires_a_list_argument
      )
      )
  
;copies all elements in a list in even indices of the list, first index is list[1]
(define (even_copy myList)
  (if (list? myList)
      (if (null? myList)
	  '()
	  (if (null? (cdr myList)) ;if the list has one element
		     '()      ;return a null list
		     (cons(car (cdr myList)) (even_copy (cddr myList))) ;else return the 2nd element and even elements of the cdr
		     )
	  )
      'even_copy_requires_list_argument
     )
  )

;sums all numbers that appear at any depth in a list
(define (list_sum mylist)
  (cond ((not (list? mylist)) 0) ;were we given a list? if not return 0
	((null? mylist) 0) ;Is the list null? return 0
       	((number? (car mylist)) (+ (car mylist) (list_sum (cdr mylist))))
	((list_sum (cdr mylist)))
	)
  )

;returns the last element of a list
(define (last mylist)
  (cond ((not (list? mylist)) 'last_requires_a_list_as_an_arg)
	((null? mylist) '()) ;Null list? return null
	((null? (cdr mylist)) (car mylist)) ;if the cdr is null, but the list isn't the first element == last element
	((last (cdr mylist))) ;we're not at the last element, keep going.
	)
  )

;inserts element myvalue at the end of mylist
(define (insert_last myvalue mylist)
  (cond ((not (list? mylist)) 'insert_last_requires_a_list)
	((null? mylist) (cons myvalue mylist)) ;are we adding to a null list? Just add it in then
        ((cons (car mylist) (insert_last myvalue (cdr mylist)))) ;remove the first element of the list, recurse and add it back in afterwards. 
	)
  )

;removes the first element of a list
(define (remove_first mylist)
  (cond ((not (list? mylist)) 'cannot_remove_first_element_of_not_a_list) ;error if there's not a list
	((null? mylist) mylist);return null if list is null
	((cdr mylist)) 
	)
  )

;removes the last element of a list
(define (remove_last mylist)
  (cond ((not (list? mylist)) 'cannot_remove_last_element_of_not_a_list)
	((null? mylist) '()) ;if list is null, we can't remove anything.
	((null? (cdr mylist)) '()) ;if the cdr of the list is null, we only have a single element. Return a null list.
	((cons (car mylist) (remove_last (cdr mylist))));append the first element of our list to the rest of our list, after removing the last element.
	)
  )

;reverse the given list
(define (list_reverse mylist)
  (cond ((not (list? mylist)) 'cannot_reverse_non_list)
	((null? mylist) '() );a null list is the reverse of a null list
	((insert_last (car mylist) (list_reverse (cdr mylist))));insert the first element of a list to the end of the reversed sublist  
	)
  )

;finds the square root of n, with candidate a being the current test and b being the 1/value that we're iterating a by
(define (sqrtHelper n a b)
  (cond ((<= 1000000 b) (if (> n (square (+ a (* 5 (reciprocal b))))) (+ a (reciprocal (/ b 10))) a));if we've found 5 sigfigs, check if (a + .000005)^2 <= n, and rounding if needed.
	((< n (square a)) (sqrtHelper n (- a (reciprocal b)) (* b 10)));if a > (sqrt n), decrement a by b and multiply b by 10, to start checking next decimal place
	((sqrtHelper n (+ a  (reciprocal b)) b)); a^2 < n, iterate a by b
	)
  )

(define (square_root n)
  (cond ((not (number? n)) 'cannot_sqrt_non-number);If it's not a number, error
	((zero? n) '0) ;sqrt 0 is 0
	((* (sqrtHelper n 0 1) 1.0));multiply integer division by a float to convert.
	;This prevents printing ex: 2.236069999999995 instead of 2.23607, which would occur using fp in helper.  
	)
  )

(define (first_half mylist)
  (cond ((not (list? mylist)) 'cannot_find_first_half_of_not_a_list)
	 ((null? mylist) '()) 
	 ((null? (cdr mylist)) mylist )
	 ((cons (car mylist) (first_half (cdr (list_reverse (cdr (list_reverse mylist))))))) ;save the first element in the list, remove the last, then recurse
	 )
  )

(define (second_half mylist)
  (cond ((not (list? mylist)) 'cannot_find_second_half_of_not_a_list)
	((list_reverse(first_half (list_reverse (cdr mylist))))) ;remove the first element from the list, then find the first half of the reverse. Reverse again for answer
	)
  )

(define (mergesort ls)
  (if (or (null? ls) (null? (cdr ls)))
      ls
      (merge  (mergesort (first_half ls))
	      (mergesort (second_half ls))
	      )
      )
  )

(define (merge L M)
  (if (null? L) M
      (if (null? M) L
	  (if (< (car L) (car M))
	      (cons (car L) (merge (cdr L) M))
	      (cons (car M) (merge (cdr M) L))
	      )
	  )
      )
  )
