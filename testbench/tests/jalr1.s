.text
        li $t1, .ok
        li $t2, 0x10000000
        add $t1, $t1, $t2
        jalr $t1
        nop

        li $v0, 1
        jr $0

.ok:    li $v0, 10
        jr $0
