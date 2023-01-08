;Test for wrong format state, symbol
#Q = {0,c_p,c_,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject} ;isndisjdksahkh
;#Q = {0,cp,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject} ;isndisjdksahkh

;TEST:
;#S = {0,11,2,33}
;#S = {1,;}
;#S = {2,3,*}
#S = {0,1}

;TEST
;#G = {0,1,_,t,r,u,e,f,a,l,s,*}
#G = {0,1,_,t,r,u,e,f,a,l,s}

#q0 = 0

#B = _

;#F = {halt_accept,_}
#F = {halt_accept}

; the number of tapes
;#N = 200-
#N = 2

; the transition functions

; State 0: start state
0 0* 0_ ** cp
;0 0{ 0_ ** cp
;0 000 0 ** cp

0 0_ 0_ ** cp
0 1_ 1_ ** cp


