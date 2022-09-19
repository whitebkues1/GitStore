#include "utils.h"



/*****************************************************************************************************/
// converts 4 byte MSB ordered array to uint32
uint32_t convertArrayToLong( uint8_t *addr )
{
    struct ul {
        uint8_t uc1;
        uint8_t uc2;
        uint8_t uc3;
        uint8_t uc4;
    };

    union ulu {
        struct ul sul;
        uint32_t longvar;
    } theUnion;

    // Align The Individual Bytes In The Proper Order
    theUnion.sul.uc4 = *addr++;
    theUnion.sul.uc3 = *addr++;
    theUnion.sul.uc2 = *addr++;
    theUnion.sul.uc1 = *addr;

    // Return The Value As A Unsigned Long
    return( theUnion.longvar );
}
/*****************************************************************************************************/
