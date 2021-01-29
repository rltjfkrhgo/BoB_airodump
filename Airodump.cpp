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
        Mac apMac(beacon->bssid);

        apValue* ptr = &apMap[apMac];
        ptr->beacons++;
        memcpy(ptr->bssid, &(beacon->ssid), beacon->len);
        ptr->bssid[beacon->len] = '\0';
    }

    // Data Frame processing...
    else if(type & DataFrame::TYPE)
    {
        DataFrame* data = (DataFrame*)(packet+offset);

        Mac reciever(data->receiver);
        Mac transmitter(data->transmitter);

        // reciever = AP
        // transmitter = Station
        if(data->flags & DataFrame::FLAG_TO_DS)
        {
            staValue* staPtr = &staMap[transmitter];
            staPtr->frames++;
            staPtr->bssid = reciever;

            if(type == DataFrame::SUBTYPE_NULL ||
            type == DataFrame::SUBTYPE_QOS_NULL)
                return;

            apValue* apPtr = &apMap[reciever];
            apPtr->data++;
        }
        
        // reciever = Station
        // transmitter = AP
        else
        {
            apValue* apPtr = &apMap[transmitter];
            apPtr->data++;

            if(isMulticast(reciever) ||
               isBroadcast(reciever))
               return;
            
            staValue* staPtr = &staMap[reciever];
            staPtr->frames++;
            staPtr->bssid = transmitter;
        }
    }
}

void* display(void* ptr)
{
    while(true)
    {
        system("clear");
        printf("\nBSSID              Beacons  #Data  ESSID\n\n");
        for(auto it = apMap.begin(); it != apMap.end(); it++)
        {
            printf("%s  %7d  %5d  %s\n", 
            std::string(it->first).c_str(), 
            it->second.beacons, 
            it->second.data, 
            it->second.bssid);
        }
        printf("\nBSSID              STATION            Frames\n\n");
        for(auto it = staMap.begin(); it != staMap.end(); it++)
        {
            printf("%s  %s  %6d\n", 
            std::string(it->first).c_str(), 
            std::string(it->second.bssid).c_str(), 
            it->second.frames);
        }
        usleep(500000);
    }
}

bool isMulticast(const Mac& mac)
{
    // 01:00:5e:xx:xx:xx는 멀티캐스트용 주소
    if(mac[0] == 0x01 &&
       mac[1] == 0x00 &&
       mac[2] == 0x5e)
       return true;
    else
        return false;
}
bool isBroadcast(const Mac& mac)
{
    for(int i = 0; i < Mac::SIZE; i++)
    {
        if(mac[i] != 0xff)
            return false;
    }
    return true;
}