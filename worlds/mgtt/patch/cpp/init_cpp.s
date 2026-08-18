.global __init_cpp
__init_cpp:
    lis   r31,(ctors-4)@ha
    addi  r31,r31,(ctors-4)@l

loop:
    lwzu  r12,4(r31)
    cmpwi r12,0
    beq-  ret
    mtctr r12
    bctrl
    b     loop

ret:
    b     cpp_dynamic_initializers+4