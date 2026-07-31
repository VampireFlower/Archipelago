#80456e94
.global steer_camera_asm
steer_camera_asm:
fmr f23, f1      # default instruction

bl SeekingCamera

cmpwi r3,0       # camera was not modified
beq orig

li r9,0          # camera was modified
b ret            # disable via switch(0)

orig:
lwz r9,-0xed8(r24)

ret:
b steer_camera_hook + 4
