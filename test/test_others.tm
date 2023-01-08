#Q = {0,cp,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject} ;isndisjdksahkh

#S = {0,1}


;Test: the blank symbol is not existed
;#G = {0,1,t,r,u,e,f,a,l,s}
#G = {0,1,_,t,r,u,e,f,a,l,s}

;Test state that is not existed
#q0 = error
;#q0 = *
#q0 = 0

;Test blank symbol must _
;#B = 1
;#B = a
;#B = 11
#B = _

;Test state that is not existed
;#F = {halt_accept,abc}
#F = {halt_accept}

; the number of tapes
;#N = 200-
#N = 2

; the transition functions

; State 0: start state
0 0* 0_ ** cp
;Test state or symbol not existed
;abcd 0* 00 ** cp
;0 0* 00 ** cppp
;0 0* 0_ ab cp
;0 ab 0_ ** cp
0 0*0 000 *** cp


0 0_ 0_ ** cp
0 1_ 1_ ** cp


