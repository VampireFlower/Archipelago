.global sincosf
sincosf:
stwu    sp,-0x18(sp)
stmw    r29,0x8(sp)
mflr    r31
stfs    f1,0x14(sp)

mr      r29,r3 # sin*
mr      r30,r4 # cos*

bl      sinf

stfs    f1,0x0(r29) # *s = sinf(x)

lfs     f1,0x14(sp)

bl      cosf

stfs    f1,0x0(r30) # *c = cosf(x)


mtlr    r31
lmw     r29,0x8(sp)
addi    sp,sp,0x18
blr