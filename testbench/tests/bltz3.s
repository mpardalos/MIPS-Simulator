.text
    addi $t1, $t1, 0
    bltz $t1, less
    addi $2, $2, -1
    jr $0

less:
    addi $2, $2, 1
    jr $0