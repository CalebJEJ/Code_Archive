% ====== The AM Interpreter ======
name('Caleb Johnson').

% ---------------------------------------------------
% predicate find/3

find(K, V, [K, V | List]).

% ---------------------------------------------------
% Boolean values
boolean(true).
boolean(false).
and(B, false, false) :- boolean(B).
and(false, B, false) :- boolean(B).
and(true, true, true).
not(true, false).
not(false, true).
or(true, B, true) :- boolean(B).
or(B, true, true) :- boolean(B).
or(false, false, false).

% ---------------------------------------------------

am( [ add | C ], [ N1, N2 | E], S,
    C          , [ N | E ],     S) :- N is N1+N2.

am( [ mul | C ], [ N1, N2 | E], S,
    C          , [ N | E ],     S) :- N is N1*N2.

am( [ sub | C ], [ N1, N2 | E], S,
    C          , [ N | E ],     S) :- N is N1-N2.

am( [ push(N) | C ],  E       , S,
    C              , [ N | E ], S) :- number(N).

am( [ fetch(X) | C ], E        , S,
    C               , [ N | E] , S) :- find(X, N, S).

% TO COMPLETE
am([false | C], E, S, C, ['F' | E], S).
am([true | C], E, S, C, ['T' | E], S).

am([not | C], ['T' | E], S, C, ['F' | E], S).
am([not | C], ['F' | E], S, C, ['T' | E], S).

am([and | C], ['T', 'T' | E], S, C, ['T' | E], S).
am([and | C], ['F', 'T' | E], S, C, ['F' | E], S).
am([and | C], ['T', 'F' | E], S, C, ['F' | E], S).
am([and | C], ['F', 'F' | E], S, C, ['F' | E], S).

am([eq | C], [B, B | E], S, C, ['T' | E], S).
am([eq | C], [B, Bx | E], S, C, ['F' | E], S).

am([lt | C], [X, X | E], S, C, ['F' | E], S).
am([lt | C], [s(X), X | E], S, C, ['T' | E], S).
am([lt | C], [s(Y), X | E], S, C, ['T' | E], S) :-
    am([lt | C], [Y, X | E], S, C, ['T' | E], S).






% ---------------------------------------------------

run([ ], E, S, [ ], E, S).
run(C1, E1, S1, C2, E2, S2) :-
    am(C1, E1, S1, C, E, S),
    run(C, E, S, C2, E2, S2).
