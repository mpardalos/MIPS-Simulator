.text
        li $v0, 1
        jal .t

.rt:    jr $0

.t:     xori $v0, $ra, .rt
        jr $0
