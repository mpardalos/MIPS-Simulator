.text
    addi $t1, $t1, 10
    addi $t2, $t2, -5
    bne $t1, $t2, nequal
    addi $2, $2, -1
    jr $0

nequal:
    addi $2, $2, 1
    jr $0