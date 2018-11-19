    .text
        li $t0, 0x20000004 

        li $t1, 0x000A000B
        sw $t1, ($t0)

        lh $v0, 2($t0)
        jr $0