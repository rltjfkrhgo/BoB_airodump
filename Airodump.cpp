// Airodump.cpp

#include "Airodump.h"
#include <iostream>

void airodump(const u_char* packet, u_int len)
{
    RadiotapHdr* radiotapHdr = (RadiotapHdr*)packet;
    int offset = radiotapHdr->it_len;
    Frame* frame = (Frame*)(packet+offset);

    if(frame->type != Frame::BEACON_FRAME_TYPE)
        return;

    Mac bssid(frame->bssid);

    offset += 24 + 12;
    uint8_t* tagged = (uint8_t*)packet+offset;
    
    char ssid[256];
    uint8_t ssidLen = tagged[1];
    memcpy(ssid, &tagged[2], ssidLen);
    ssid[ssidLen] = '\0';

    std::cout << std::string(bssid) << "\t" << ssid << std::endl;
}