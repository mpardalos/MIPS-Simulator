.text
    li $t1, 97 # Data

    li $t5, 0x20000004 # Data address

    sw $t1, 0($t5)
    lw $v0, 0($t5)

    jr $0
