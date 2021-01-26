// Airodump.cpp

#include "Airodump.h"
#include <iostream>

void airodump(const u_char* packet, u_int len)
{
    RadiotapHdr* radiotapHdr = (RadiotapHdr*)packet;

    uint16_t radiotapHdrLen = radiotapHdr->it_len;
    printf("%d\n", radiotapHdrLen);

    u_char* frame = (u_char*)(packet+radiotapHdrLen);

    for(int i = 0; i < 10; i++)
    {
        printf("%02x ", frame[i] );
    }
    printf("\n");

    //Mac mac(frame->bssid);
    //std::cout << mac << std::endl;
}