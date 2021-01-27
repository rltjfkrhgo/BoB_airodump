// Airodump.h

#pragma once

#include <map>
#include "mac.h"

struct ieee80211_radiotap_header {
    uint8_t   it_version;     /* set to 0 */
    uint8_t   it_pad;
    uint16_t  it_len;         /* entire length */
    uint32_t  it_present;     /* fields present */
} __attribute__((__packed__));

struct beacon_frame
{
    const static uint8_t BEACON_FRAME_TYPE = 0x80;

    uint8_t   type;
    uint8_t   flags;
    uint16_t  duration;
    uint8_t   receiver[6];
    uint8_t   transmitter[6];
    uint8_t   bssid[6];
    uint16_t  seq;

    // fixed parameters
    uint64_t  timestamp;
    uint16_t  interval;
    uint16_t  capabilities;

    // tagged parameters
    // 여기서 부터는 가변적
    uint8_t   num;
    uint8_t   len;
    uint8_t   ssid;
} __attribute__((__packed__));

struct data_frame
{
    const static uint8_t DATA_FRAME_TYPE = 0x08;

    uint8_t   type;
    uint8_t   flags;
    uint16_t  duration;
    uint8_t   receiver[6];     // destination
    uint8_t   transmitter[6];  // bssid
    uint8_t   source[6];
    uint16_t  seq;
} __attribute__((__packed__));

typedef struct
{
    int pwr;
    int beacons;
    int data;
    char ssid[256];
}
Stat;

typedef struct ieee80211_radiotap_header RadiotapHdr;
typedef struct beacon_frame BeaconFrame;
typedef struct data_frame DataFrame;
typedef struct Mac Mac;

static std::map<Mac, Stat> map;

void airodump(const u_char* packet, u_int len);
void display();