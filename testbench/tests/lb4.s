    .text
        li $t0, 0x20000000
        
        li $t1, 20
        sb $t1, ($t0)

        li $t1, 0xBF
        sb $t1, 2($t0)

        lb $v0, 2($t0)

        jr $0