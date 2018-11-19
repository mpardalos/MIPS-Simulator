.text
    li $t1, 97 # Data

    li $t5, 0x20000004 # Data address

    sh $t1, 2($t5)
    lh $v0, 2($t5)

    jr $0
