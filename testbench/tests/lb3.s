    .text
        li $t0, 0x20000004 
        
        li $t1, 20
        sb $t1, ($t0)

        li $t1, 0xBF
        sb $t1, 4($t0)

        lb $v0, 4($t0)
        srl $v0, $v0, 1

        jr $0