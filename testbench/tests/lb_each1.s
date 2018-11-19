.set taddr, 0x20000004
.text
        li $t1, 0x12345678
        sw $t1, taddr
        lb $v0, taddr

        jr $0
