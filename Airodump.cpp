// Airodump.cpp

#include "Airodump.h"
#include <iostream>

void airodump(const u_char* packet, u_int len)
{
    RadiotapHdr* radiotapHdr = (RadiotapHdr*)packet;
    uint16_t radiotapHdrLen = radiotapHdr->it_len;
    Frame* frame = (Frame*)(packet+radiotapHdrLen);

    if(frame->type != Frame::BEACON_FRAME_TYPE)
        return;

    Mac bssid(frame->bssid);
}