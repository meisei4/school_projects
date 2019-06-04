;;; Helper functions
(define bst-create list)

(define bst-root car)

(define bst-left cadr)

(define bst-right caddr)

(define bst-is-empty? null?)

;;; Required functions
(define (bst-create-empty) '())

(define (bst-insert bst f x)
  (cond ((bst-is-empty? bst) (bst-create x
                                         (bst-create-empty)
                                         (bst-create-empty)))
        ((f x (bst-root bst)) (bst-create (bst-root bst)
                                          (bst-insert (bst-left bst) f x)
                                          (bst-right bst)))
        (else (bst-create (bst-root bst)
                          (bst-left bst)
                          (bst-insert (bst-right bst) f x)))))

(define (bst-pre-elements bst)
  (cond ((bst-is-empty? bst) '())
        (else (cons (bst-root bst)
                    (append (bst-pre-elements (bst-left bst))
                            (bst-pre-elements (bst-right bst)))))))

(define (bst-in-elements bst)
  (cond ((bst-is-empty? bst) '())
        (else (append (bst-in-elements (bst-left bst))
                      (list (bst-root bst))
                      (bst-in-elements (bst-right bst))))))

(define (bst-post-elements bst)
  (cond ((bst-is-empty? bst) '())
        (else (append (bst-post-elements (bst-left bst))
                      (bst-post-elements (bst-right bst))
                      (list (bst-root bst))))))

(define (bst-contains bst eq lt val)
  (cond ((bst-is-empty? bst) #f)
        ((eq (bst-root bst) val) #t)
        ((lt val (bst-root bst)) (bst-contains (bst-left bst) eq lt val))
        (else (bst-contains (bst-right bst) eq lt val))))

(define (bst-min bst)
  (cond ((bst-is-empty? (bst-left bst)) (bst-root bst))
        (else (bst-min (bst-left bst)))))

(define (bst-remove bst eq lt val)
  (cond ((bst-is-empty? bst) bst)
        ((lt val (bst-root bst))
         (bst-create (bst-root bst) (bst-remove (bst-left bst) eq lt val) (bst-right bst)))
        ((eq val (bst-root bst))
         (cond ((null? (bst-left bst)) (bst-right bst))
               ((null? (bst-right bst)) (bst-left bst))
               (else (let* ((min (bst-min (bst-right bst))))
                       (bst-create min (bst-left bst) (bst-remove (bst-right bst) eq lt min))))))
        (else (bst-create (bst-root bst) (bst-left bst) (bst-remove (bst-right bst) eq lt val)))))

(define (list->bst xs f)
  (list->bst->helper (bst-create-empty) xs f))

(define (list->bst->helper bst xs f)
  (cond ((null? xs) bst)
        (else (list->bst->helper (bst-insert bst f (car xs)) (cdr xs) f))))

;;; Some testing
(let ((tree (list->bst '(5 1 3 5 6 0 12 -3 -1 3 500) <)))
  (list
   (bst-pre-elements tree)
   (bst-post-elements tree)
   (bst-in-elements tree)))


(let ((tree (list->bst '(5 1 3 5 6 0 12 -3 -1 3 500) >)))
  (list
   (bst-pre-elements tree)
   (bst-post-elements tree)
   (bst-in-elements tree)))

(let ((tree (list->bst '("cat" "hat" "zebra" "uwl" "cs" "MSE") string<?)))
  (list
   (bst-pre-elements tree)
   (bst-post-elements tree)
   (bst-in-elements tree)))

(let ((tree (list->bst '((1 2 3) (6) (-3 2 1 1 1) (5 3 1 1 1 1)) (lambda (a b) (< (length a) (length b))))))
  (list
   (bst-pre-elements tree)
   (bst-post-elements tree)
   (bst-in-elements tree)))

(let ((tree (list->bst '((1 2 3) (6) (-3 2 1 1 1) (5 3 1 1 1 1)) (lambda (a b) (< (car a) (car b))))))
  (list
   (bst-pre-elements tree)
   (bst-post-elements tree)
   (bst-in-elements tree)))