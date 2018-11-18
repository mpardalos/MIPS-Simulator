    .text
        li $t0, 0x20000004 

        li $t1, 0x123562BF
        sw $t1, ($t0)

        lbu $v0, 2($t0)
        jr $0