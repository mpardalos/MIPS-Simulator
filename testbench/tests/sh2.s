    .text
        li $t0, 0x20000004 

        li $t1, 0x62BF
        sh $t1, ($t0)
        li $t2, 0x1234
        sh $t2, 2($t0)

        lw $v0, ($t0)
        jr $0