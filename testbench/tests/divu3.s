.text
    addi $t1, $t1, -98 
    addi $t2, $t2, 0

    .word 0x012a001b # divu $t1, $t2

    li $v0, 10

    jr $0
