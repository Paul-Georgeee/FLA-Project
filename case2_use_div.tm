;states
#Q = {init,accept,div,letf2,add2,compare23,preforclear3,clear3,compare12,left1,reject,preforclear1_f,clear1_f,preforclear1_t,clear1_t,true0,true1,true2,true3,false0,fasle1,fasle2,false3,fasle4,halt}

#S = {1}
#G = {1,_,t,r,u,e,f,a,l,s}
#q0 = init
#B = _
#F = {}
#N = 3

;state init
init 1__ 11_ *** div        ;prepare for div
init ___ ___ *** accept     ;empty input

;state div
div 11_ 11_ rr* div
div 1__ 1_1 *lr letf2       ;tape3's count + 1
div ___ __1 ll* compare23   ;finish div
div _1_ _11 l** add2        ;remainder is not 0, try next

;state letf2
letf2 11_ 11_ *l* letf2
letf2 1__ 1__ *r* div       ;continue div

;state compare23
compare23 111 111 *ll compare23
compare23 1_1 1_1 *r* add2  ;not equal, try next
compare23 11_ 11_ **r add2  ;not equal, try next
compare23 1__ 1__ *** accept;

;state add2
add2 111 111 *r* add2
add2 1_1 111 *** preforclear3

;state preforclear3
preforclear3 111 111 **r preforclear3
preforclear3 11_ 11_ **l clear3

;state clear3
clear3 111 11_ **l clear3
clear3 11_ 11_ **r compare12

;state compare12
compare12 11_ 11_ ll* compare12
compare12 _1_ 111 r** reject
compare12 1__ 1__ *r* left1
compare12 ___ 111 r** reject

;state left1
left1 11_ 11_ l** left1
left1 _1_ _1_ r** div

;state accept
accept ___ _11 *** true0
accept 1__ 111 *** preforclear1_t

;state preforclear1_t
preforclear1_t 111 111 r** preforclear1_t
preforclear1_t _11 _11 l** clear1_t

;state clear1_t
clear1_t 111 _11 l** clear1_t
clear1_t _11 _11 *** true0

;state reject
reject 111 111 *** preforclear1_f

;state preforclear1_f
preforclear1_f 111 111 r** preforclear1_f
preforclear1_f _11 _11 l** clear1_f

;state clear1_f
clear1_f 111 _11 l** clear1_f
clear1_f _11 _11 *** false0

true0 _11 t11 r** true1
true0 *11 t11 r** true1
true1 _11 r11 r** true2
true1 *11 r11 r** true2
true2 _11 u11 r** true3
true2 *11 u11 r** true3
true3 _11 e11 *** halt
true3 *11 e11 *** halt

false0 _11 f11 r** fasle1
fasle1 _11 a11 r** fasle2
fasle2 _11 l11 r** false3
false3 _11 s11 r** fasle4
fasle4 _11 e11 *** halt