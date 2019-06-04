#lang racket
(provide bst-create-empty bst-insert bst-contains)
;export funcs for simplify

(define (bst-create-empty) '())

(define (bst-create root left right)
  (list root left right))

(define (bst-insert bst f x)
  (cond ((null? bst) (bst-create x '() '()))
        ((f (car bst) x) (list (car bst) (cadr bst) (bst-insert (caddr bst) f x)))
        (else (list (car bst) (bst-insert (cadr bst) f x) (caddr bst)))))

(define (bst-contains bst f g x)
  (cond ((null? bst) #f)
        ((f (car bst) x) #t)
        ((g (car bst) x) (bst-contains (caddr bst) f g x))
        (else (bst-contains (cadr bst) f g x))))

(define (bst-min bst)
  (cond ((null? (cadr bst)) (car bst))
         (else (bst-min (cadr bst)))))

(define (nochildren bst) (and (null? (cadr bst)) (null? (caddr bst))))

(define (leftonly bst) (and (null? (caddr bst)) (not (null? (cadr bst)))))

(define (rightonly bst) (and (null? (cadr bst)) (not (null? (caddr bst)))))

;cases
;root == x (sub cases for children amounts
;root lessthan x (remove from the left tree
;else root notlessthan x  (remove from( the right tree
(define (bst-remove bst f g x)
   (cond ((not (bst-contains bst f g x)) bst)
         ((f (car bst) x) (cond ((nochildren bst) '())
                                ((rightonly bst) (caddr bst))
                                ((leftonly bst) (cadr bst))
                                (else (list
                                       (bst-min (caddr bst))
                                       (cadr bst)
                                       (bst-remove (caddr bst) f g (bst-min (caddr bst)))))))
          ((g (car bst) x) (list
                           (car bst)
                           (cadr bst)
                           (bst-remove (caddr bst) f g x)))
         (else (list
                (car bst)
                (bst-remove (cadr bst) f g x)
                (caddr bst)))))

(define (bst-pre-elements bst)
  (cond ((nochildren bst) (cons (car bst) '()))
        ((rightonly bst) (cons (car bst) (bst-pre-elements (caddr bst))))
        ((leftonly bst) (cons (car bst) (bst-pre-elements (cadr bst))))
        (else (cons
               (car bst)
               (append
                (bst-pre-elements (cadr bst))
                (bst-pre-elements (caddr bst)))))))

(define (bst-in-elements bst)
  (cond ((nochildren bst) (cons (car bst) '()))
        ((rightonly bst) (cons (car bst) (bst-in-elements (caddr bst))))
        ((leftonly bst) (cons (car bst) (bst-in-elements (cadr bst))))
        (else (append
               (append
                (bst-in-elements (cadr bst))
                (list (car bst)))
               (bst-in-elements (caddr bst))))))

(define (bst-post-elements bst)
  (cond ((nochildren bst) (cons (car bst) '()))
        ((rightonly bst) (cons (car bst) (bst-post-elements (caddr bst))))
        ((leftonly bst) (cons (car bst) (bst-post-elements (cadr bst))))
        (else (append
               (append
                (bst-post-elements (cadr bst))
                (bst-post-elements (caddr bst)))
               (list (car bst))))))

(define (list->bsthelper bst xs f)
  (cond ((null? xs) bst)
        (else (list->bsthelper (bst-insert bst f (car xs)) (cdr xs) f))))
          
(define (list->bst xs f)
  (cond ((null? xs) xs)
        (else (list->bsthelper (bst-create (car xs) '() '()) (cdr xs) f))))
