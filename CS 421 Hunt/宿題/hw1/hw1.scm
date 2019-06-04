(define (append x y)
    (cond ((null? x) y)
          (else (cons (car x) (append (cdr x) y)))))
 
(define (pair x y)
    (cond ((null? x) x)
          ((null? y) y)
          (else (cons (cons (car x) (list (car y))) (pair (cdr x) (cdr y))))))

(define (double x y)
    (cond ((null? x) x)
          ((equal? (car x) y) (cons y (cons y (double (cdr x) y))))
          (else (cons (car x) (double (cdr x) y)))))

(define (list-replace l x y)
    (cond ((null? l)          l)
          ((equal?(car l) x) (cons y (list-replace (cdr l) x y)))
          ((list? (car l))   (cons (list-replace (car l) x y) (list-replace (cdr l) x y)))
          (else              (cons (car l) (list-replace (cdr l) x y)))))

(define (repeat x y)
    (cond ((= 0 y) '())
          (else (cons x (repeat x (- y 1))))))

(define (count x xs)
    (cond ((null? xs) 0)
          ((equal? x (car xs)) (+ 1 (count x (cdr xs))))
           (else 0)))

(define (nextnot x xs)
    (cond ((null? xs) xs)
          ((equal? x (car xs)) (nextnot x (cdr xs)))
           (else xs)))

(define (rle x)
    (cond ((null? x) x)
           (else (cons (cons (count (car x) x) (list (car x))) (rle (nextnot (car x) x))))))

(define (flatten x)
    (cond ((null? x) x)
          ((list? (car x)) (append (cons (car (car x)) (flatten (cdr (car x)))) (flatten (cdr x))))
           (else (cons (car x) (flatten (cdr x))))))

(define (repeat-loop x)
    (cond ((null? x) x)
           (else (cons (repeat (car (car x)) (car (cdr (car x)))) (repeat-loop (cdr x))))))

(define (rld x)
    (cond ((null? x) x)
           (else (flatten (repeat-loop x)))))
(rld '((r 4) (t 6) (e 2)))

(define (pull xs n i)
    (cond ((null? xs) xs)
          ((= 0 (modulo i n)) (cons (car xs) (pull (cdr xs) n (+ i 1 ))))
           (else (pull (cdr xs) n (+ i 1)))))

(define (pick xs n)
    (cond ((null? xs) xs)
          ((<= n 0) '())
           (else (pull xs n 0))))

(define (infix xs)
    (cond ((null? xs) xs)
        ((list? xs) (list (infix (car (cdr xs))) (car xs) (infix (car (cddr xs)))))
        (else xs)))

(define (addId id scope)
    (cond ((null? scope) (cons id scope))
        ((equal? (car id) (car (car scope))) (cons id (cdr scope)))
        (else (cons (car scope) (addId id (cdr scope))))))

(define (getId id scope)
    (cond ((null? scope) 'undefined)
        ((equal? id (car (car scope))) (car (cdr (car scope))))
        (else (getId id (cdr scope)))))

;UNFINISHED

;(define (env? x) (list? (car x)))
  
;(define (evalEnv env)
;  (cond ((null? env) env)
;        ((op? (car (cdr (car env)))) (addId (cons (car (car env)) (evaluate (cdr (car env))))))
;        ((env? (car env)))
;         (addId (car env) (evalEnv (cdr env))))

;(define (op? x)
;  (cond ((equal? x '/) #t)
;        ((equal? x '*) #t)
;        ((equal? x '+) #t)
;        ((equal? x '-) #t)
;        (else #f)))
              
;(define (evaluate exp)
;  (cond ((op? (car exp)) (eval (cons (car exp) (evaluate (cadr exp)) (evaluate (caddr exp)))))
;        ((and (env? (car exp)) (char-alphabetic? (cdr exp))) (eval (evalEnv (car exp))
;        ((char-alphabetic? (car exp)) (getId (car exp))
