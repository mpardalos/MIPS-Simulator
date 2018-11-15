.text
    addi $t1, $t1, 0
    bgez $t1, greater
    addi $2, $2, -1
    jr $0

greater:
    addi $2, $2, 1
    jr $0