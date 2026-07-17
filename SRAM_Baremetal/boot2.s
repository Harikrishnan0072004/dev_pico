.cpu cortex-m0plus
.thumb

.section .boot2, "ax"
    ldr r0, =XIP_SSI_SSIENR
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =XIP_SSI_CTRLR0
    ldr r1, =0x001F0300
    str r1, [r0]

    ldr r0, =XIP_SSI_BAUDR
    ldr r1, =0x00000008
    str r1, [r0]

    ldr r0, =XIP_SSI_SPI_CTRLR0
    ldr r1, =0x03000218
    str r1, [r0]

    ldr r0, =XIP_SSI_CTRLR1
    ldr r1, =0x00000000
    str r1, [r0]

    ldr r0, =XIP_SSI_SSIENR
    ldr r1, =0x00000001
    str r1, [r0]

    ;@ Standard handoff: read the app's vector table at 0x10000100.
    ;@ word0 = initial SP, word1 = Reset_Handler address (thumb bit set)
    ldr r0, =0x10000100
    ldmia r0!, {r1, r2}
    msr   msp, r1
    bx    r2

.set XIP_SSI_BASE,       0x18000000
.set XIP_SSI_CTRLR0,     XIP_SSI_BASE + 0x00
.set XIP_SSI_CTRLR1,     XIP_SSI_BASE + 0x04
.set XIP_SSI_SSIENR,     XIP_SSI_BASE + 0x08
.set XIP_SSI_BAUDR,      XIP_SSI_BASE + 0x14
.set XIP_SSI_SPI_CTRLR0, XIP_SSI_BASE + 0xF4

.end
