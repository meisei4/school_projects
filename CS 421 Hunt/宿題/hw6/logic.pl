edge(1,2,10).
edge(2,3,10).
edge(3,4,10).
edge(1,3,10).
edge(2,4,10).

path(A, A, 0).
path(A, C, AC) :- 
    edge(A, B, AB),
    path(B, C, BC),
    AC is AB + BC.

path(A, A, 0, [A]).
path(A, C, AC, [A|PATH]) :-
    edge(A, B, AB),
    path(B, C, BC, PATH),
    AC is AB + BC.

grows([]).
grows([_]).
grows([X|[Y|Xs]]) :- 
    X < Y,
    not(member(X, Xs)),
    grows([Y|Xs]).

sumrunner([], []).
sumrunner([_], []).
sumrunner([X|[Y|Xs]], [Z|Zs]) :-
    Z is X + Y,
    sumrunner([Y|Xs], Zs).
