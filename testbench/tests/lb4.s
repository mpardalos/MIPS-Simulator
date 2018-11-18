    .text
        li $t0, 0x20000000
        
        li $t1, 0x12352451
        sw $t1, ($t0)
        lb $v0, 2($t0)

        jr $0