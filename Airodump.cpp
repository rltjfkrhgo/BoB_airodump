// Airodump.cpp

#include "Airodump.h"
#include <iostream>

void airodump(const u_char* packet, u_int len)
{
    RadiotapHdr* radiotapHdr = (RadiotapHdr*)packet;
    int offset = radiotapHdr->it_len;

    uint8_t type = *(packet+offset);

    // Beacon Frame processing...
    if(type == BeaconFrame::BEACON_FRAME_TYPE)
    {
        BeaconFrame* beacon = (BeaconFrame*)(packet+offset);
        Mac bssid(beacon->bssid);

        Stat* ptr = &map[bssid];
        ptr->beacons++;
        memcpy(ptr->ssid, &(beacon->ssid), beacon->len);
        ptr->ssid[beacon->len] = '\0';
    }

    // Data Frame processing...
    else if(type & DataFrame::DATA_FRAME_TYPE)
    {
        DataFrame* data = (DataFrame*)(packet+offset);
        Mac bssid(data->transmitter);

        Stat* ptr = &map[bssid];
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
        std::cout << "BSSID\t\tBeacons\t#Data\tESSID\n" << std::endl;
        for(auto it = map.begin(); it != map.end(); it++)
        {
            std::cout << 
            std::string(it->first) << "  " << 
            it->second.beacons << "\t" << 
            it->second.data << "\t" << 
            it->second.ssid << std::endl;
        }
    }
}