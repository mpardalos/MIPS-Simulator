.text
        li $t1, 0x20000000
        li $t2, 0x12345678
        sw $t2, ($t1)

        lb $v0, 2($t1)
        jr $0