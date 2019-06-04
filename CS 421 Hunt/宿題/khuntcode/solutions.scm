(define (pair xs ys)
  (cond ((or (null? xs) (null? ys)) '())
        (else (cons (list (car xs) (car ys)) (pair (cdr xs) (cdr ys))))))

(define (repeat x n)
  (cond ((= n 0) '())
        (else (cons x (repeat x (- n 1))))))

(define (double xs v)
  (cond ((null? xs) '())
        ((equal? (car xs) v)
         (append (repeat (v xs) 2) (double (cdr xs) v)))
        (else (cons (car xs) (double (cdr xs) v)))))

(define (list-replace xs old new)
  (cond ((null? xs) '())
        ((equal? (car xs) old) (cons new (list-replace (cdr xs) old new)))
        ((list? (car xs) (cons (list-replace (car xs) old new)
                               (list-replace (cdr xs) old new))))
        (else (cons (car xs) (list-replace (cdr xs) old new)))))
  

(define (rld pairs)
  (cond ((null? pairs) '())
        (else (append (repeat (car (car pairs)) (cadr (car pairs)))
                      (rld (cdr pairs))))))

(define (rle xs)
  (cond ((null? xs) '())
        (else (rle-helper (cdr xs) (car xs) 1))))

(define (rle-helper xs prev-item prev-count)
  (cond ((null? xs) (list (list prev-item prev-count)))
        ((equal? (car xs) prev-item) (rle-helper (cdr xs) prev-item (+ prev-count 1)))
        (else (cons (list prev-item prev-count) (rle-helper (cdr xs) (car xs) 1)))))

(define (infix exp)
  (cond ((or (number? exp) (symbol? exp)) exp)
    (else (list (infix (cadr exp)) (car exp) (infix (caddr exp))))))

(define (pick values n)
  (pick-helper values n 0))

(define (pick-helper values n count)
  (cond ((null? values) '())
        ((= count 0) (cons (car values) (pick-helper (cdr values) n n)))
        (else (pick-helper (cdr values) n (- count 1)))))

(define (symbol->procedure op)
  (cond ((equal? op '+) +)
        ((equal? op '-) -)
        ((equal? op '*) *)
        ((equal? op '/) /)
        ((equal? op '%) modulo)
        ((equal? op '^) expt)))

(define (evaluate exp)
  (evaluate-helper '() exp))

(define (apply-op op x y)
  (cond ((or (equal? x 'undefined)
             (equal? y 'undefined)) 'undefined)
        (else ((symbol->procedure op) x y))))

;;; env1 is a list of pairs
;;; env2 is a list of pairs that are being added to env1
;;; the values in env2 are interpreted with respect to env1
(define (add-declarations env decls)
  (cond ((null? decls) env)
        (else (let* ((first-decl (car decls))
                     (first-decl-var (car first-decl))
                     (first-decl-exp (cadr first-decl))
                     (rest-decls (cdr decls)))
                (cons (list first-decl-var (evaluate-helper env first-decl-exp)) (add-declarations env rest-decls))))))

;;; environment is a list of pairs of the form ((var val) ...)
(define (lookup env var)
  (cond ((null? env) 'undefined)
        ((equal? (car (car env)) var) (car (cdr (car env))))
        (else (lookup (cdr env) var))))

(define (evaluate-helper env exp)
  (cond ((number? exp) exp)
        ((symbol? exp) (lookup env exp))
        ((list? (car exp)) 
         (evaluate-helper (add-declarations env (car exp)) (cadr exp)))
        (else (apply-op (car exp)
                (evaluate-helper env (cadr exp)) 
                (evaluate-helper env (caddr exp))))))