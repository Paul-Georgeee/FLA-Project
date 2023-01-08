;states
#Q = {init,cmp,add2,add2_1,shift2left,accept,reject,true0,true1,true2,true3,false0,false1,false2,false3,false4,halt}

#S = {1}
#G = {1,_,t,r,u,e,f,a,l,s}
#q0 = init  ;init state
#B = _
#F = {halt}
#N = 2      ;first one is input
            ;second one used to add

;state init
init 1_ 11 ** cmp
init __ __ ** accept  ;empty input

;state cmp
cmp 11 11 rr cmp
cmp 1_ 1_ ** add2       ;bigger, continue to add
cmp __ __ l* accept     ;equal, accept
cmp _1 _1 l* reject     ;smaller, reject

;state add2
add2 1_ 11 *r add2_1

;state add2_1
add2_1 1_ 11 ** shift2left

;shift2left
shift2left 11 11 *l shift2left
shift2left 1_ 1_ *r cmp

;state accept 
accept 1_ __ l* accept
accept __ __ r* true0

;state reject
reject 11 _1 l* reject
reject _1 _1 r* false0

true0 __ t_ r* true1
true1 __ r_ r* true2
true2 __ u_ r* true3
true3 __ e_ ** halt

false0 _1 f1 r* false1
false1 _1 a1 r* false2
false2 _1 l1 r* false3
false3 _1 s1 r* false4
false4 _1 e1 ** halt

