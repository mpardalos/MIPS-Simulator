.text
    li $t1, 97 # Data
    li $t2, 80

    li $t5, 0x20000004 # Data address

    # break

    sb $t1, 0($t5)
    sb $t2, 1($t5)
    lb $v0, 0($t5)

    jr $0
