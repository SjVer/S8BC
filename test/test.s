start:
    psh #1
    psh #2
    psh #3

    lda @2

    hlt

$fffe: .word start
