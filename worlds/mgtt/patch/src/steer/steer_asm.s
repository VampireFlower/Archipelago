#80417b00
.global steer_asm

steer_asm:
stwu sp,-0x10(sp)
mflr r0
stw  r0,0x14(sp)
stw  r3,8(sp) # backup arg


lwz  r0,0x48(r3) #flight context
cmpwi r0,1

bne .ret



addi r3,r3,0x50 # &ball->Velocity



lis r11,0x804e
ori r11,r11,0xcde0 # Player **

lwz r11,0(r11)   # Player *


lwz r11,0x4f14(r11) # Player->controller

lfs f1,0x20(r11) # controller->left_stick


#lis r12,ball_steer@ha
#addi r12,r12,ball_steer@l
#mtrctr r12
#bctrl
bl ball_steer


.ret:
lwz r3,8(sp) # restore result


lwz  r0,0x14(sp)
mtlr r0


addi sp,sp,0x10

# default instruction
stwu sp,-0x2b8(sp)



b steer_hook + 4
