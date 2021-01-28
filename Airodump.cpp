// Airodump.cpp

#include "Airodump.h"
#include <iostream>

void airodump(const u_char* packet, u_int len)
{
    RadiotapHdr* radiotapHdr = (RadiotapHdr*)packet;
    int offset = radiotapHdr->it_len;

    uint8_t type = *(packet+offset);

    // Beacon Frame processing...
    if(type == BeaconFrame::TYPE)
    {
        BeaconFrame* beacon = (BeaconFrame*)(packet+offset);
        Mac bssid(beacon->bssid);

        Stat* ptr = &map[bssid];
        ptr->beacons++;
        memcpy(ptr->ssid, &(beacon->ssid), beacon->len);
        ptr->ssid[beacon->len] = '\0';
    }

    // Data Frame processing...
    else if(type & DataFrame::TYPE)
    {
        if(type == DataFrame::SUBTYPE_NULL)
            return;

        DataFrame* data = (DataFrame*)(packet+offset);
        Mac reciever(data->receiver);
        Mac transmitter(data->transmitter);

        Stat* ptr = &map[transmitter];
        ptr->data++;
        ptr = &map[reciever];
        ptr->data++;
    }
}

void* display(void* ptr)
{
    time_t  prev = time(NULL);

    while(true)
    {
        time_t  now = time(NULL);
        if(now - prev < 1)
            continue;

        prev = now;  // 시간 갱신
        system("clear");
        printf("BSSID              Beacons  #Data  ESSID\n\n");
        for(auto it = map.begin(); it != map.end(); it++)
        {
            printf("%s  %7d  %5d  %s\n", 
            std::string(it->first).c_str(), 
            it->second.beacons, 
            it->second.data, 
            it->second.ssid);
        }
    }
}