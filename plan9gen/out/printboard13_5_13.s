TEXT ·printboard13_5_13(SB), $32-8
printboard13_5_13:
MOVQ DX, BX
SUBQ $8, BX
LEAQ param1-16(SP), AX
MOVQ (AX), CX
MOVQ $0, (CX)
forStart14_9_12:
LEAQ param1-16(SP), BX
LEAQ param2+0(FP), AX
MOVQ (AX), CX
ADDQ $-8, CX
MOVQ CX, BX
MOVQ (BX), AX
SUBQ $1, AX
MOVQ (BX), BX
CMPQ BX, AX
MOVQ DX, BX
SUBQ $8, BX
LEAQ param2-24(SP), AX
MOVQ (AX), CX
MOVQ $0, (CX)
forStart15_7_10:
LEAQ param2-24(SP), BX
LEAQ param2+0(FP), AX
MOVQ (AX), CX
ADDQ $-8, CX
MOVQ CX, BX
MOVQ (BX), AX
SUBQ $1, AX
MOVQ (BX), BX
CMPQ BX, AX
LEAQ param2+0(FP), BX
MOVQ (BX), CX
ADDQ $-24, CX
MOVQ CX, AX
MOVQ (AX), CX
LEAQ param1-16(SP), BX
MOVQ (BX), AX
IMULQ $8, AX
ADDQ AX, CX
MOVQ CX, BX
LEAQ param2-24(SP), AX
MOVQ (BX), BX
MOVQ (AX), AX
CMPQ BX, AX
ifTrue16_17_19:
MOVQ DX, CX
LEAQ string16_34_38(SB), BX
MOVQ $2, CX
MOVQ BX, (CX)
ifFalse16_17_19:
MOVQ DX, CX
LEAQ string16_44_48(SB), AX
MOVQ $2, CX
MOVQ AX, (CX)
ifEnd16_17_19:
MOVQ DX, str-32(SP)
MOVQ CX, strlen-24(SP)
CALL ·prints(SB)
forEnd15_7_10:
MOVQ DX, BX
ADDQ $8, BX
LEAQ string17_13_17(SB), AX
MOVQ $1, CX
MOVQ AX, str-32(SP)
MOVQ CX, strlen-24(SP)
CALL ·prints(SB)
forEnd14_9_12:
MOVQ DX, BX
ADDQ $8, BX
LEAQ string18_16_20(SB), AX
MOVQ $1, CX
MOVQ AX, str-32(SP)
MOVQ CX, strlen-24(SP)
CALL ·prints(SB)
RET