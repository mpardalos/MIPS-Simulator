.text
    li		$t1, 0x10500000		# $t1 = 0x10500000
    li		$t2, 0x12345678		# $t2 = 0x12345678

    sh		$t2, 2($t2)