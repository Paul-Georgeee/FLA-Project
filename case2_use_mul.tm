;states
#Q = {init,plus23,shift2,preforcmp,cmp14,clear4,back1,add2,add3,back23,halt,accept,reject,true0,true1,true2,true3,false0,false1,false2,false3,false4}

#S = {1}
#G = {1,_,t,r,u,e,f,a,l,s}
#q0 = init  ;init state
#B = _
#F = {halt}
#N = 4      ;first one is input
            ;second and third used to plus
            ;fourth used to save the plus result

;state init
init 1___ 111_ **** plus23
init ____ _11_ **** accept  ;empty

;state plus23: to compute the plus
plus23 111_ 1111 *r*r plus23
plus23 1_1_ 1_1_ *l** shift2
plus23 11__ 11__ **ll preforcmp     ;finish compare the result

;state shift2
shift2 111_ 111_ *l** shift2
shift2 1_1_ 1_1_ *rr* plus23

;state preforcmp
preforcmp 1111 1111 ***l preforcmp  ;shift the 4th tape's head to left
preforcmp 111_ 111_ ***r cmp14

;state cmp14
cmp14 1111 1111 r**r cmp14
cmp14 111_ 111_ ***l clear4         ;less than input, continue to try
cmp14 _11_ _11_ l*** accept         ;equal to input, accept
cmp14 _111 _111 l*** reject         ;greater than input, reject

;state clear4
clear4 1111 111_ ***l clear4        ;clear the 4th tape
clear4 111_ 111_ ***r back1

;state back1
back1 111_ 111_ l*** back1          ;shift the 1th tape's head to left
back1 _11_ _11_ r*** add2

;state add2
add2 111_ 111_ *r** add2            ;add one on 2nd tape
add2 1_1_ 111_ **** add3

;state add3
add3 111_ 111_ **r* add3            ;add one on 3rd tape
add3 11__ 111_ **** back23

;state back23
back23 111_ 111_ *ll* back23        ;shift 2nd and 3rd tape back to left
back23 1___ 1___ *rr* plus23

;state accept 
accept 111_ _11_ l*** accept
accept _11_ _11_ r*** true0

;state reject
reject 1111 _111 l*** reject
reject _111 _111 r*** false0

true0 _11_ t11_ r*** true1
true1 _11_ r11_ r*** true2
true2 _11_ u11_ r*** true3
true3 _11_ e11_ **** halt

false0 _111 f111 r*** false1
false1 _111 a111 r*** false2
false2 _111 l111 r*** false3
false3 _111 s111 r*** false4
false4 _111 e111 **** halt

