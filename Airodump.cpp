// Airodump.cpp

#include "Airodump.h"
#include <iostream>

void airodump(const u_char* packet, u_int len)
{
    RadiotapHdr* radiotapHdr = (RadiotapHdr*)packet;
    int offset = radiotapHdr->it_len;

    uint8_t type = *(packet+offset);

    if(type != BeaconFrame::BEACON_FRAME_TYPE)
        return;

    // Beacon Frame processing...
    BeaconFrame* beacon = (BeaconFrame*)(packet+offset);
    Mac bssid(beacon->bssid);
    
    char ssid[256];
    uint8_t ssidLen = beacon->len;
    memcpy(ssid, &(beacon->ssid), ssidLen);
    ssid[ssidLen] = '\0';

    std::cout << std::string(bssid) << "\t" << ssid << std::endl;
}