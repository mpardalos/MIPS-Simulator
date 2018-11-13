.text
        jal .ok
        nop

        li $v0, 1
        jr $0

.ok:    li $v0, 10
        jr $0
