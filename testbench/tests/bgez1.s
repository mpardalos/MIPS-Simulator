.text
    addi $t1, $t1, 6
    bgez $t1, greater
    addi $2, $2, -1
    jr $0

greater:
    addi $2, $2, 1
    jr $0