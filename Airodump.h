// Airodump.h

#pragma once

#include <sys/types.h>
#include <map>
#include "mac.h"

struct ieee80211_radiotap_header {
        u_int8_t        it_version;     /* set to 0 */
        u_int8_t        it_pad;
        u_int16_t       it_len;         /* entire length */
        u_int32_t       it_present;     /* fields present */
} __attribute__((__packed__));

struct ieee80211_frame
{
    const static u_int8_t BEACON_FRAME_TYPE = 0x80;

    u_int8_t  type;
    u_int8_t  padding[3];
    u_int8_t  dst[6];
    u_int8_t  src[6];
    u_int8_t  bssid[6];
    u_int8_t  padding2[2];
} __attribute__((__packed__));

typedef struct ieee80211_radiotap_header RadiotapHdr;
typedef struct ieee80211_frame Frame;
typedef struct Mac Mac;

void airodump(const u_char* packet, u_int len);
