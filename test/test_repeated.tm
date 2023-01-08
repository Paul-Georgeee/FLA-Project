
;TEST:
#Q = {0,cp,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject,cmp} ;isndisjdksahkh
#Q = {0,cp,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject} ;isndisjdksahkh

;TEST:
;#S = {0,1,2,2}
#S = {0,1}

;TEST
;#G = {0,1,_,t,r,u,e,f,a,l,s,r}
#G = {0,1,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
;TEST
;#F = {0,cp,cmp,cp}
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state
0 0_ 0_ ** cp
0 1_ 1_ ** cp
0 __ __ ** accept ; empty input
;TEST
;0 0_ 0_ ** cp

; State cp: copy the string to the 2nd tape 
cp 0_ 00 rr cp
cp 1_ 11 rr cp
cp __ __ ll mh

; State mh: move 1st head to the left
mh 00 00 l* mh
mh 01 01 l* mh
mh 10 10 l* mh
mh 11 11 l* mh
mh _0 _0 r* cmp
mh _1 _1 r* cmp
;TEST
;0 0_ 0_ ** cp

; State cmp: compare two strings
cmp 00 __ rl cmp
cmp 11 __ rl cmp
cmp 01 __ rl reject
cmp 10 __ rl reject
cmp __ __ ** accept

; State accept*: write 'true' on 1st tape
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State reject*: write 'false' on 1st tape
reject 00 __ rl reject
reject 01 __ rl reject
reject 10 __ rl reject
reject 11 __ rl reject
reject __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
;TEST
;mh _1 _1 r* cmp
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject
