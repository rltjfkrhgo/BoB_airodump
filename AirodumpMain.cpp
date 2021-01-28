// AirodumpMain.cpp

#include <pcap.h>
#include <stdio.h>
#include <pthread.h>
#include "Airodump.h"

void usage() {
    printf("syntax: airodump <interface>\n");
    printf("sample: airodump mon0\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        usage();
        return -1;
    }

    char* dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    //pcap_t* handle = pcap_open_offline(dev, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "pcap_open_live(%s) return nullptr - %s\n", dev, errbuf);
        return -1;
    }

    pthread_t displayThread;
    pthread_create(&displayThread, NULL, display, NULL);

    while (true)
    {
        struct pcap_pkthdr* header;
        const u_char* packet;
        int res = pcap_next_ex(handle, &header, &packet);  // 패킷 수신
        if (res == 0) continue;
        if (res == -1 || res == -2) {
            printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(handle));
            break;
        }
        airodump(packet, header->caplen);
    }

    pthread_join(displayThread, NULL);
    pcap_close(handle);
}