.text
    addi $t1, $t1, -6
    bltz $t1, .t
    addi $2, $2, -1
    jr $0

.t:
    addi $2, $2, 1
    jr $0