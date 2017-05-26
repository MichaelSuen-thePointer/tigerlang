TEXT ·Out(SB), $72-8
MOVQ DX, BX
SUBQ $40, BX
LEAQ param1-16(SP), AX
MOVQ (AX), CX
MOVQ $8, (CX)
LEAQ param2-24(SP), BX
MOVQ (BX), CX
LEAQ param1-16(SP), AX
MOVQ (AX), AX
MOVQ AX, len-72(SP)
MOVQ $0, val-64(SP)
CALL ·builtin_initarray(SB)
MOVQ ret-56(SP), BX
MOVQ BX, (CX)
LEAQ param3-32(SP), AX
MOVQ (AX), CX
LEAQ param1-16(SP), BX
MOVQ (BX), BX
MOVQ BX, len-72(SP)
MOVQ $0, val-64(SP)
CALL ·builtin_initarray(SB)
MOVQ ret-56(SP), AX
MOVQ AX, (CX)
LEAQ param4-40(SP), BX
MOVQ (BX), CX
LEAQ param1-16(SP), AX
MOVQ (AX), BX
MOVQ (AX), CX
LEAQ param1-16(SP), AX
ADDQ (AX), CX
MOVQ CX, BX
MOVQ BX, AX
SUBQ $1, AX
MOVQ AX, len-72(SP)
MOVQ $0, val-64(SP)
CALL ·builtin_initarray(SB)
MOVQ ret-56(SP), BX
MOVQ BX, (CX)
LEAQ param5-48(SP), AX
MOVQ (AX), CX
LEAQ param1-16(SP), BX
MOVQ (BX), AX
MOVQ (BX), CX
LEAQ param1-16(SP), BX
ADDQ (BX), CX
MOVQ CX, AX
MOVQ AX, BX
SUBQ $1, BX
MOVQ BX, len-72(SP)
MOVQ $0, val-64(SP)
CALL ·builtin_initarray(SB)
MOVQ ret-56(SP), AX
MOVQ AX, (CX)
LEAQ irfp+8(SP), CX
MOVQ CX, staticlink-72(SP)
MOVQ param0-8(SP), BX
MOVQ BX, param1-64(SP)
MOVQ $0, param2-56(SP)
CALL ·try20_5_13(SB)
MOVQ ret-48(SP), AX
MOVQ DX, BX
ADDQ $-40, BX
RET
