    .text
        li $t0, 0x20000004 

        li $t1, 0xBF
        sb $t1, ($t0)
        li $t1, 0x34
        sb $t1, 2($t0)

        lw $v0, ($t0)
        srl $v0, $v0, 8
        jr $0