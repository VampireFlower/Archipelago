#80456e94
.global BallFlightCameraUpdate_asm
BallFlightCameraUpdate_asm:
fmr f23, f1      # default instruction

bl SeekingCamera

xori  3,r3,1         # !SeekingCamera();
lwz   r9,-0xed8(r24) # load switch value
mullw r9,r9,r3

# if seekingcamera returns true: r9 * 0 = 0
# if seekingcamera returns false r9 * 1 = r9

ret:
b BallFlightCameraUpdate_asm_hook + 4



#80455178
.global SelectBallFlightCamera_asm
SelectBallFlightCamera_asm:

# a comparison is already live in cr0: `cmpwi r3,0`
# continue the rest of the function if r3 is 1 (replay)
bne continue

# epilogue
lfd  f31,0x60(sp)
lmw  r26,0x48(sp)
lwz  r0,0x6c(sp)
mtlr r0
addi sp,sp,0x68
blr

continue:
b SelectBallFlightCamera_asm_hook + 4
