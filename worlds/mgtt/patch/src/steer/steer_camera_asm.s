#8045bb64
.global BallFlightCameraUpdate_asm
BallFlightCameraUpdate_asm:
fmr f27, f1      # default instruction


lis r11,    0x804f
ori r11,r11,0xf240 # Player **
lwz r11,0(r11)     # Player *
lwz r3, 8(r11)     # Player->golfBall


bl SeekingCamera

xori  3,r3,1         # !SeekingCamera();
lwz   r9,0x3f88(r25) # load switch value
mullw r9,r9,r3

# if seekingcamera returns true: r9 * 0 = 0
# if seekingcamera returns false r9 * 1 = r9

ret:
b BallFlightCameraUpdate_asm_hook + 4



#8045867c
.global SelectBallFlightCamera_asm
SelectBallFlightCamera_asm:

# a comparison is already live in cr0: `cmpwi r3,0`
# continue the rest of the function if r3 is 1 (replay)
bne continue

# epilogue
lfd  f31,0x70(sp)
lmw  r25,0x54(sp)
lwz  r0,0x7c(sp)
mtlr r0
addi sp,sp,0x78
blr

continue:
b SelectBallFlightCamera_asm_hook + 4
