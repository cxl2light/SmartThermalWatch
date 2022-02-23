#ifndef WIFIPACKETINFO_H
#define WIFIPACKETINFO_H


#include "wifi_decode_packet.h"

class WifiPacketInfo{
public:
    //WifiPacketInfo(PACKET_INFO& packetInfo);
    WifiPacketInfo(bool bSmall);
    ~WifiPacketInfo();

    struct PACKET_INFO m_packet_info;
public:
    struct PACKET_INFO* m_pkt;

    void RestBuff();
};



#endif

