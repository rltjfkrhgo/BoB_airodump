// Airodump.cpp

#include "Airodump.h"

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

        Stat* ptr = &apMap[bssid];
        ptr->beacons++;
        memcpy(ptr->ssid, &(beacon->ssid), beacon->len);
        ptr->ssid[beacon->len] = '\0';
    }

    // Data Frame processing...
    else if(type & DataFrame::TYPE)
    {
        if(type == DataFrame::SUBTYPE_NULL ||
           type == DataFrame::SUBTYPE_QOS_NULL)
            return;

        DataFrame* data = (DataFrame*)(packet+offset);
        Mac reciever(data->receiver);
        Mac transmitter(data->transmitter);

        Stat* ptr = &apMap[transmitter];
        ptr->data++;
        ptr = &apMap[reciever];
        ptr->data++;
    }
}

void* display(void* ptr)
{
    while(true)
    {
        system("clear");
        printf("BSSID              Beacons  #Data  ESSID\n\n");
        for(auto it = apMap.begin(); it != apMap.end(); it++)
        {
            printf("%s  %7d  %5d  %s\n", 
            std::string(it->first).c_str(), 
            it->second.beacons, 
            it->second.data, 
            it->second.ssid);
        }
        usleep(500000);
    }
}