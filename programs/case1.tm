;states
#Q = {init,shift,back,final,accept}

#S = {0,1}
#G = {0,1,_}
#q0 = init
#B = _
#F = {accept}
#N = 2

;transition functions
;state init: prepare for shift
init __ __ ** accept        ;empty input
init 1_ _1 r* shift
init 0_ _0 r* shift

;state shift: shift the bits except the last one
shift 00 00 r* shift
shift 01 10 r* shift
shift 10 01 r* shift
shift 11 11 r* shift
shift _* _* l* back         ;reach the last one

;state back: go back to the left
back ** ** l* back
back _* _* ** final

;state final: put the last one on the first one and finish
final _0 0_ ** accept
final _1 1_ ** accept