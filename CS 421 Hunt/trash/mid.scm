#lang racket
(define (listify xs)
  (cond ((null? xs) xs)
        ((= (length xs) 1) (list xs))
        ((> (car xs) (cadr xs)) (cons (car xs) (listify (cdr xs))))
        (else (list (cons (car xs) (car (listify (cdr xs)))) (listify (cddr xs))))))

(define (last xs)
  (display xs) (newline)
  (cond ((null? xs) xs)
        ((null? (cdr xs)) (car xs))
        (else (last (cdr xs)))))

(define (inc xs ys)
  (cond ((null? xs) ys)
        ((null? ys) xs)
        ((< (last xs) (car ys)) (inc (append xs (list (car ys))) (cdr ys)))
        (else (cons xs (inc (list (car ys)) (cdr ys))))))

(define (listy xs)
  (cond ((null? xs) xs)
        ((= (length xs) 1) xs)
        (else (inc (list (car xs)) (cdr xs)))))


