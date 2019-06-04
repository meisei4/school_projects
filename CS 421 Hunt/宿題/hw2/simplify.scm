#lang racket
(require "bst.scm")
;uses bst in footprint func

(define (commutative? exp)
  (or (equal? '* (car exp))
      (equal? '+ (car exp))))

(define (sort exp)
  (cond ((atom? exp) exp)
        ((and (commutative? exp) (lessthan? (cadr exp) (caddr exp))) (swap exp))
        (else exp)))

(define (swap exp)
  (list (car exp)
        (caddr exp)
        (cadr exp)))

(define (lessthan? x y)
  (cond ((isop? x) (not (symbol<? x y)))
        ((and (symbol? x) (symbol? y)) (symbol<? y x))
        ((and (number? x) (symbol? y)) #t)
        ((and (list? x) (atom? y)) #t)
        ((and (list? x) (list? y)) (lessthan? (car x) (car y)))
        (else #f)))

(define (atom? x) 
  (and (not (pair? x))
       (not (null? x))))

(define (isop? x)
  (or (equal? x '*)
      (equal? x '+)
      (equal? x '/)
      (equal? x '-)))

(define (numbersonly? exp)
  (and (number? (cadr exp))
       (number? (caddr exp))))

(define (ERROR? exp)
  (or (equal? 'ERROR (cadr exp))
      (equal? 'ERROR (caddr exp))))

(define (ruleseval exp)
  (cond ((atom? exp) exp)
        ((ERROR? exp) 'ERROR)
        ((equal? '+ (car exp)) (cond
                                 ((equal? 0 (cadr exp)) (caddr exp))
                                 ((equal? 0 (caddr exp)) (cadr exp))
                                 ((numbersonly? exp) (eval exp))
                                 (else exp)))
        ((equal? '* (car exp)) (cond
                                 ((equal? 0 (cadr exp)) 0)
                                 ((equal? 0 (caddr exp)) 0)
                                 ((equal? 1 (caddr exp)) (cadr exp))
                                 ((equal? 1 (cadr exp)) (caddr exp))
                                 ((numbersonly? exp) (eval exp))
                                 (else exp)))
        ((equal? '- (car exp)) (cond
                                 ((equal? 0 (caddr exp)) (cadr exp))
                                 ((equal? (cadr exp) (caddr exp)) 0)
                                 ((numbersonly? exp) (eval exp))
                                 (else exp)))
        ((equal? '/ (car exp)) (cond
                                 ((equal? 0 (caddr exp)) 'ERROR)
                                 ((equal? (cadr exp) (caddr exp)) 1)
                                 ((equal? 1 (caddr exp)) (cadr exp))
                                 ((equal? 0 (cadr exp)) 0)
                                 ((numbersonly? exp) (eval exp))
                                 (else exp)))
        (else exp)))

(define (simplify exp)
  (cond ((atom? exp) exp)
        (else (sort (ruleseval (list (car exp)
                                     (simplify (ruleseval (cadr exp)))
                                     (simplify (ruleseval (caddr exp)))))))))

(define (isvar? x)
  (and (not (isop? x))
       (not (number? x))))

;f? boolean type comparison function
;g? boolean equality comparison function
;x? boolean lessthan function
(define (uniquethings f? g? x? exp bst)
  (cond ((equal? 'ERROR exp) 0)
        ((null? exp) 0)
        ((list? (car exp)) (+ (uniquethings f? g? x? (car exp) bst) (uniquethings f? g? x? (cdr exp) bst)))
        ((and (f? (car exp))
              (not (bst-contains bst g? x? (car exp)))) (+ 1 (uniquethings f? g? x? (cdr exp) (bst-insert bst x? (car exp)))))
        (else (uniquethings f? g? x? (cdr exp) bst))))

(define (footprint exp)
  (* 4 (+ (uniquethings number? = < exp (bst-create-empty))
          (uniquethings isvar? equal? symbol<? (simplify exp) (bst-create-empty)))))