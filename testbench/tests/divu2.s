.text
    li $t1, -98 
    li $t2, 4

    # emit the division instruction directly 
    # because gas also emits division by zero checking code
    # .word 0x12a001b # t1 / t2
    divu $t1, $t2
    mflo $v0

    jr $0
