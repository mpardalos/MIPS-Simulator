.text
    # 0x0000FFFF - unsigned (correct)
    # 0xFFFFFFFF - signed (wrong)
    ori $t0, 0xFFFF

    # 0x0000000F - unsigned (correct)
    # 0x000FFFFF - signed (wrong)
    srl $v0, $t0, 12

    jr $0