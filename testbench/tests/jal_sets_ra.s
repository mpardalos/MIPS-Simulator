.text
        li $v0, 1
        jal .t
.rt:    nop

.t:     xori $v0, $ra, .rt
        jr $0
