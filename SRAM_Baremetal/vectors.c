extern unsigned long _stack_top;
extern void Reset_Handler(void);

/* Minimal 2-word vector table: [0]=initial SP, [1]=Reset_Handler.
   Boot2 now reads this pair at 0x10000100 (see updated boot2.s). */
__attribute__((section(".vectors")))
const unsigned long vector_table[2] = {
    (unsigned long)&_stack_top,
    (unsigned long)Reset_Handler
};

extern unsigned long __text_load__, __text_start__, __text_end__;
extern unsigned long __data_load__, __data_start__, __data_end__;
extern unsigned long __bss_start__, __bss_end__;

extern int main(void);

#define XIP_SSI_SSIENR (*(volatile unsigned int*)0x18000008)

/* Runs from FLASH (placed in .reset, right after the vector table).
   Copies the app to RAM, disables XIP, then jumps into main(). */
__attribute__((section(".reset")))
void Reset_Handler(void)
{
    unsigned long *src, *dst;

    src = &__text_load__; dst = &__text_start__;
    while (dst < &__text_end__) *dst++ = *src++;

    src = &__data_load__; dst = &__data_start__;
    while (dst < &__data_end__) *dst++ = *src++;

    dst = &__bss_start__;
    while (dst < &__bss_end__) *dst++ = 0;

    XIP_SSI_SSIENR = 0;   /* disable XIP — safe: vector table + code now in RAM */

    main();
    while (1) { }
}