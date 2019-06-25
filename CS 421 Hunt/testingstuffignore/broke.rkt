#lang racket

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; UTILITY FUNCTIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Converts an s-expression into a string
;; INPUT: an S-expression EXP
;; OUTPUT: a SCHEME String corresponding to EXP
(define (exp->string exp)
  (cond ((number? exp) (number->string exp))
        ((symbol? exp) (symbol->string exp))
        ((list? exp) (exp->string (car exp)))))

;; INPUT: a list of lists
;; OUTPUT: a list containing all elements of the first-level lists
(define (flatten list-of-lists)
  (cond ((null? list-of-lists) '())
        (else (append (car list-of-lists) (flatten (cdr list-of-lists))))))

;; this is for all error handling.
;; programmers don't use this function but
;; the interpreter calls this function to
;; signal some type of programmer error
(define (error msg)
  (display "ERROR: ")
  (display msg)
  (newline))

;; THERE ARE TWO SUPPORTED TYPES: 'int and 'booleanean
;; INPUT: an element of the ART-C language
;; OUTPUT: the type of that element
(define (type-of val)
  (cond ((number? val) 'int)
        ((boolean? val) 'boolean)))

;; A MAP is a list of key-value pairs
;; INPUT: a MAP and a KEY
;; OUTPUT: The value associated with the key or 'error
(define (map-get map x)
  (cond ((null? map) 'error)
        ((equal? (car (car map)) x) (cadr (car map)))
        (else (map-get (cdr map) x))))

;; INPUT : A MAP AND KEY
;; OUTPUT : true if the key is in the map and false otherwise
(define (map-contains map x)
  (cond ((null? map) #f)
        ((equal? (car (car map)) x) #t)
        (else (map-contains (cdr map) x))))

;; INPUT : A MAP, KEY and VALUE
;; OUTPUT: The map that results from replacing the key with the new value.  If
;; the map doesn't contain KEY, then 'error is returned
(define (map-replace map key val)
  (cond ((null? map) 'error)
        ((equal? (car (car map)) key)
         (cons (list key val) (cdr map)))
        (else (cons (car map) (map-replace (cdr map) key val)))))

;; INPUT : A MAP, Key and Value
;; OUTPUT : The map that results from adding a key-value pair.  This
;; allows for duplicate keys (the most-recently added is nearer the front of the list
(define (map-add map key val)
  (cons (list key val) map))

;; INPUT: A MAP and KEY
;; OUTPUT: The map that results from deleting the key.  No errors occur if the map
;; doesn't contain the key
(define (map-delete map key)
  (cond ((null? map) map)
        ((equal? (car (car map)) key) (cdr map))
        (else (cons (car map)
                    (map-delete (cdr map) key)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; TYPEMAP : A SEMANTIC DOMAIN DATA TYPE
;; A typemap is a list of block-level declarations.
;; FORM: (((var1 type1) (var2 type2)) ((var3 type3) (var4 type4) (var5 type5)) ... )
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT: NONE
;; OUTPUT: AN empty typemap
(define (typemap-create-empty) '())

;; INPUT: A TYPEMAP
;; OUTPUT: The largest address in use in the ENVIRONMENT
(define (typemap-type-of tm x)
  (map-get tm x))

;; INPUT: A TYPEMAP
;; OUTPUT: THE TYPEMAP THAT RESULTS FROM INSERTING A DECLARATIONS
(define (typemap-add tm decl)
  (map-add tm (cadr decl) (car decl)))

(define (typemap-delete tm key)
  (map-delete tm key))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; STATE : A SEMANTIC DOMAIN DATA TYPE
;; A LIST OF (VAR, VALUE) pairs
;; FORM :  ( (var1 val1) (var2 val2) ... )
;; NOTE: A map can contain duplicate keys but innermost KEYS occur
;;       before outermost KEYS and hide them
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; INPUT : NONE
;; OUTPUT: AN EMPTY STATE
(define (state-create-empty) '())
  
;; INPUT: STATE and ID
;; OUTPUT: a new state such that the innermost scope now contains a
;;         new binding for the specified ID.  The bindings value is 'undefined.
(define (state-add state id)
  (map-add state id 'undefined))

;; INPUT : STATE and ID
;; OUTPUT: A new state such that the innermost id is removed
(define (state-delete state id)
  (map-delete state id))

;; INPUT: STATE and ID
;; OUTPUT: The value associated with the specified ID in the given state
(define (state-get-value state id)
  (map-get state id))

;; INPUT: STATE and ID
;; OUTPUT: A new state that results from changing the mapping from id->value in
;;         the specified state
(define (state-update state id value)
  (map-replace state id value))

;; INPUT: STATE and LIST-OF-IDS (VARIABLES)
;; OUTPUT: A new state that results from deleting all ids (the variables) from
;;         the specified state
(define (state-delete-all state variables)
  (cond ((null? variables) state)
        (else (state-delete-all (state-delete state (car variables)) (cdr variables)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; THESE CLASSES CORRESPOND TO THE ABSTRACT SYNTAX SUCH THAT A "PROGRAM"
;; REPRESENT A PARSE-TREE.  THESE FUNCTIONS OPERATE AT THE 'SYNTACTIC' LEVEL
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; (PROGRAM BODY)
(define (program-get-body stmt)
  (cadr stmt))

;; (BLOCK S1...SN)
(define (block-get-body stmt)
  (filter (lambda (x) (not (is-declaration? x))) (cdr stmt)))
  
(define (block-get-declarations stmt)
  (filter (lambda (x) (is-declaration? x)) (cdr stmt)))

;; (DECLARE TYPE VAR)
(define (declaration-get-type stmt)
  (cadr stmt))

(define (declaration-get-var stmt)
  (caddr stmt))

(define (is-declaration? stmt)
  (and (list? stmt) 
       (equal? (car stmt) 'declare)))

(define (is-block? stmt)
  (and (list? stmt) 
       (equal? (car stmt) 'block)))

(define (is-if? stmt)
  (and (list? stmt) 
       (equal? (car stmt) 'if)))

(define (is-while? stmt)
  (and (list? stmt) 
       (equal? (car stmt) 'while)))

(define (is-assign? stmt)
  (and (list? stmt)
       (equal? (car stmt) ':=)))

(define (is-sprint? stmt)
  (and (list? stmt) 
       (equal? (car stmt) 'sprint)))

;; (:= VAR EXP)
(define (assignment-get-var stmt)
  (cadr stmt))

(define (assignment-get-exp stmt)
  (caddr stmt))

;; (IF TEST THEN [ELSE])
(define (if-get-test stmt)
  (cadr stmt))

(define (if-get-then stmt)
  (caddr stmt))

(define (if-has-else? stmt)
  (= (length stmt) 4))

(define (if-get-else stmt)
  (cadddr stmt))

;; (WHILE TEST BODY)
(define (while-get-test stmt)
  (cadr stmt))

(define (while-get-body stmt)
  (caddr stmt))

;; (SPRINT LABEL EXP)
(define (sprint-has-exp? stmt)
  (and (list? stmt)
       (= (length stmt) 3)))

(define (sprint-get-label stmt)
  (cadr stmt))

(define (sprint-get-exp stmt)
  (caddr stmt))

;; INPUT: an expression EXP
;; OUTPUT: the operator of EXP (an element of ART-C)
(define (exp-get-operator exp)
  (car exp))

;; INPUT: an expression EXP
;; OUTPUT: the left-operand (an expression) of EXP
(define (exp-get-left-operand exp)
  (car (cdr exp)))

;; INPUT: an expression EXP
;; OUTPUT: the exp-get-right-operand (an expression) of EXP
(define (exp-get-right-operand exp)
  (car (cdr (cdr exp))))

;; INPUT: an expression EXP
;; OUTPUT: #t if the expression is a boolean literal and #f otherwise
(define (bool? exp)
  (or (equal? exp #t)
      (equal? exp #f)
      (equal? exp 'true)
      (equal? exp 'false)))

;; INPUT: a symbol
;; OUTPUT: #t if the symbol is 'true and #f if it is 'false and 'void' if neither
(define (symbol->bool sym)
  (cond ((equal? sym 'true) #t)
        ((equal? sym 'false) #f)))

;; INPUT: A PROGRAM
;; A PROGRAM has syntactic structure (program stmt)
;; OUTPUT: THE STATE that results from executing the program
;;         in an empty state.
(define (interpret-program pgm)
  (interpret (program-get-body pgm) (state-create-empty)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This is the INTERPRETER class                          
;; An INTERPRETER is simply a collection of functions that
;; operates on TYPES, STATES, BINDING, SCOPES and PROGRAMS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; INPUT: STATEMENT and STATE
;; OUTPUT: The state that results from executing STATEMENT in STATE
(define (interpret stmt state)
  (let ((kind (car stmt)))
    (cond ((equal? kind 'block) (interpret-block stmt state))
          ((equal? kind ':=) (interpret-assignment stmt state))
          ((equal? kind 'if) (interpret-if stmt state))
          ((equal? kind 'sprint) (interpret-sprint stmt state))
          ((equal? kind 'while) (interpret-while stmt state))
          (else (error (string-append "statement expected but saw (" (exp->string stmt) "...) instead."))))))

(define (initialize-vartype stmt state)
  (cond ((not (valid-decl? stmt state)) state)
        ((null? stmt) state)
        (else (map-add state (caddr stmt) (cadr stmt)))))

(define (only-blocks? stmt)
  (cond ((null? stmt) #t)
        ((equal? 'block (car stmt)) (only-blocks? (cdr stmt)))
        (else #f)))

;;INTERPRET BLOCK
(define (interpret-block stmt state)
  (let ([decls (block-get-declarations stmt)]
        [body (block-get-body stmt)])
    (cond ((only-blocks? stmt) state)
          (else (interpret (cons 'block (cdr body)) (interpret (car body) (interpret-declarations decls state)))))))

;;INTERPRET DECLARATION
(define (interpret-declarations stmt state)
  (cond ((null? stmt) state)
        (else (interpret-declarations (cdr stmt) (initialize-vartype (car stmt) state)))))

(define (state->type state)
  (cond ((null? state) '())
        (else (cons (list (car (car state))
                          (type-of-val (cadr (car state)))) (state->type (cdr state))))))

(define (type-of-val val)
  (cond ((or (equal? 'int val) (int? val)) 'int)
        ((or (equal? 'boolean val) (boolean? val)) 'boolean)))
  
;;INTERPRET ASSIGNMENT
(define (interpret-assignment stmt state)
  (state-update state (assignment-get-var stmt) (eval-exp (assignment-get-exp stmt) state)))

;;INTERPRET IF
(define (interpret-if stmt state)
  (let ([test (eval-exp (if-get-test stmt) state)]
        [then (if-get-then stmt)])
    (cond ((and (not test) (if-has-else? stmt)) (interpret (if-get-else stmt) state))
          (test (interpret then state))
          (else state))))

;;INTERPRET WHILE
(define (interpret-while stmt state)
  (let ([test (eval-exp (while-get-test stmt) state)]
        [body (while-get-body stmt)])
    (cond (test (interpret-while stmt (interpret body state)))
          (else state))))

(define (printstringexp stmt state)
  (display (sprint-get-label stmt)) (display (eval-exp (sprint-get-exp stmt) state)) (newline)
  state)

(define (printstring stmt state)
  (display (sprint-get-label stmt)) (newline)
  (display state) (newline)
  state)

;;INTERPRET SPRINT
(define (interpret-sprint stmt state)
  (cond ((sprint-has-exp? stmt) (printstringexp stmt state))
        (else (printstring stmt state))))

(define (other symbol)
  (if (equal? 'int symbol) "boolean" "int"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;                      EVALUATION
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (eval-exp exp state)
  (cond ((not (valid-exp? exp (state->type state))) 'undefined)
        ((int? exp) exp)
        ((bool? exp) (symbol->bool exp))
        ((and (map-contains state exp)
              (not (or (equal? 'boolean (state-get-value state exp))
                       (equal? 'int (state-get-value state exp))))) (state-get-value state exp))
        ((unary? exp) (eval-unary exp state))
        (else (eval-binary exp state))))

;;EVALUATE BINARY
(define (eval-binary exp state)
  (cond ((equal? '+ (exp-get-operator exp)) (+ 
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '- (exp-get-operator exp)) (-
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '* (exp-get-operator exp)) (*
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '/ (exp-get-operator exp)) (/ 
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '@ (exp-get-operator exp)) (expt 
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '? (exp-get-operator exp)) (modulo
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '< (exp-get-operator exp)) (< 
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '> (exp-get-operator exp)) (> 
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '= (exp-get-operator exp)) (=
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '<= (exp-get-operator exp)) (<=
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '>= (exp-get-operator exp)) (>=
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '& (exp-get-operator exp)) (and
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        ((equal? '% (exp-get-operator exp)) (or
                                              (eval-exp (exp-get-left-operand exp) state)
                                              (eval-exp (exp-get-right-operand exp) state)))
        (else 'syntaxman)))

;;EVALUATE UNARY
(define (eval-unary exp state) 
  (cond ((equal? '~ (exp-get-operator exp)) (not (eval-exp (cadr exp) state)))
        ((equal? '- (exp-get-operator exp)) (* -1 (eval-exp (cadr exp) state)))
        (else 'syntaxman)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;                      VALIDATION
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;AUXILIARIES
(define (keyword? var) 
  (or (equal? var 'boolean)
      (equal? var 'int)
      (equal? var 'declare)
      (equal? var 'block)
      (equal? var 'program)
      (equal? var 'if)
      (equal? var 'while)
      (equal? var 'sprint)
      (equal? var 'true)
      (equal? var 'false)))

(define (exp-type? op)
  (cond ((equal? op '+) 'int)
        ((equal? op '-) 'int)
        ((equal? op '*) 'int)
        ((equal? op '/) 'int)
        ((equal? op '@) 'int)
        ((equal? op '?) 'int)
        ((equal? op '<) 'boolean)
        ((equal? op '>) 'boolean)
        ((equal? op '=) 'boolean)
        ((equal? op '<=) 'boolean)
        ((equal? op '>=) 'boolean)
        ((equal? op '&) 'boolean)
        ((equal? op '%) 'boolean)
        ((equal? op '~) 'boolean)))

;;questionable
(define (type-of? exp tmap)
  (cond ((int? exp) 'int)
        ((bool? exp) 'boolean)
        ((map-contains tmap exp) (typemap-type-of tmap exp))
        ((valid-op? exp tmap) (exp-type? (exp-get-operator exp)))
        (else 'undefined)))

(define (type-match? type exp tmap)
  (if (equal? type (type-of? exp tmap)) #t (error "fail")))

(define (unary? exp)
  (and (list? exp)
       (= 2 (length exp))
       (or (equal? '~ (exp-get-operator exp))
           (equal? '- (exp-get-operator exp)))))

(define (valid-op? exp tmap)
  (if (unary? exp) (valid-unary? exp tmap) (valid-binary? exp tmap)))

;;VALID INT
(define (int? num) 
  (and (number? num)
       (>  num -2147483648)
       (< num  2147483647)))

;;VALID UNARY
(define (valid-unary? exp tmap)
  (cond ((equal? '- (car exp)) (type-match? 'int (cadr exp) tmap))
        ((equal? '~ (car exp)) (type-match? 'boolean (cadr exp) tmap))))

;;VALID BINARY
(define (valid-binary? exp tmap)
  (let ([op (exp-get-operator exp)]
        [lopr (exp-get-left-operand exp)]
        [ropr (exp-get-right-operand exp)])
    (cond ((or (equal? op '&) (equal? op '?)) (and (type-match? 'boolean lopr tmap)
                                                   (type-match? 'boolean ropr tmap)))
          (else (and (type-match? 'int lopr tmap)
                     (type-match? 'int ropr tmap))))))

;;VALID EXPRESSION
(define (valid-exp? exp tmap)
  (cond ((map-contains tmap exp) #t)
        (else (or (int? exp)
                  (bool? exp)
                  (valid-op? exp tmap)))))

;;VALID ASSIGN
(define (valid-assign? assign tmap)
  (let ([var (assignment-get-var assign)]
        [exp (assignment-get-exp assign)]
        [type (typemap-type-of tmap (assignment-get-var assign))])
    (and (map-contains tmap var)
         (type-match? type exp tmap))))

;;VALID WHILE
(define (valid-while? stmt tmap)
  (let ([test (while-get-test stmt)]
        [body (while-get-body stmt)])
    (and (type-match? 'boolean test tmap)
         (valid-stmt? body tmap))))

;;VALID SPRINT
(define (valid-sprint? stmt tmap)
  (cond ((sprint-has-exp? stmt) (and (string? (sprint-get-label stmt))
                                     (valid-exp? (sprint-get-exp stmt) tmap)))
        (else (string? (sprint-get-label stmt)))))

;;VALID IF
(define (valid-if? stmt tmap)
  (let ([test (if-get-test stmt)]
        [then (if-get-then stmt)])
    (cond ((if-has-else? stmt) (and (valid-stmt? then tmap)
                                    (valid-stmt? (if-get-else stmt) tmap)))
          (else (and (type-match? 'boolean test tmap)
                     (valid-stmt? then tmap))))))

;;VALID STATEMENT
(define (valid-stmt? stmt tmap)
  (cond ((is-block? stmt) (valid-block? stmt tmap))
        ((is-assign? stmt) (valid-assign? stmt tmap))
        ((is-if? stmt) (valid-if? stmt tmap))
        ((is-while? stmt) (valid-while? stmt tmap))
        ((is-sprint? stmt) (valid-sprint? stmt tmap))
        (else #f)))

;;VALID BODY
(define (valid-body? body tmap)
  (cond ((null? body) #t)
        ((valid-stmt? (car body) tmap) (valid-body? (cdr body) tmap))
        (else #f)))

;;VALID DECL
(define (valid-decl? dec tmap)
  (and (not (keyword? (declaration-get-var dec)))
       (not (map-contains tmap (declaration-get-var dec)))))

;;VALID DECLARATIONS
(define (valid-declarations? dec tmap)
  (cond ((null? dec) tmap)
        ((valid-decl? (car dec) tmap) (valid-declarations? (cdr dec) (typemap-add tmap (cdr (car dec)))))
        (else (error "fail"))))

;;VALID BLOCK
(define (valid-block? blk tmap)
  (let ((update (valid-declarations? (block-get-declarations blk) tmap)))
    (cond ((equal? #f update) #f)
          (else (valid-body? (block-get-body blk) update)))))

;;VALID PROGRAM
(define (is-program-valid? pgm)
       (valid-block? (cadr pgm) (typemap-create-empty)))

(define pgm '(program 
              (block
               (declare int n)
               (declare boolean error)
               (declare int result)   
               (:= error false)
               (:= result 1)
               (block 
                (declare int local)
                (:= n 5)
                (:= local #f)
                (while (> local 0)
                       (block
                        (:= result (* result local))
                        (:= local (- local 1)))))
              (sprint "result: " result)
              (if (~ error) (sprint "a") (sprint "b")))))


(is-program-valid? pgm)


(interpret-program pgm)