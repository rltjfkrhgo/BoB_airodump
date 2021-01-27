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

    auto iter = map.find(bssid);
    if(iter == map.end())  // 없다면
    {
        Stat* ptr = &map[bssid];
        ptr->beacons++;
        memcpy(ptr->ssid, &(beacon->ssid), beacon->len);
        ptr->ssid[beacon->len] = '\0';
    }
    else
    {
        iter->second.beacons++;
    }
}

void display()
{
    system("clear");
    std::cout << "BSSID\t\t\tBeacons\tESSID\n";
    for(auto it = map.begin(); it != map.end(); it++)
    {
        std::cout << 
        std::string(it->first) << "\t" << 
        it->second.beacons << "\t" << 
        it->second.ssid << std::endl;
    }
}