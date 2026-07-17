#define PUT32(address,value)  (*((volatile unsigned int*)address))=value
#define GET32(address) *(volatile unsigned int*)address

int main( void )
{
    PUT32( 0x4000f000, ( 1 << 5 ) );

    while( ( GET32(0x4000c008) & ( 1 << 5 ) ) == 0 );   // parens added — original had an operator-precedence bug

    PUT32( 0x400140cc, 0x05 );
    PUT32( 0xd0000020, ( 1 << 25 ) );

    while( 1 )
    {
        PUT32( 0xd000001c, ( 1 << 25 ) );
        for( volatile unsigned int a = 20000; a > 0; a-- );
    }

    return 0;
}