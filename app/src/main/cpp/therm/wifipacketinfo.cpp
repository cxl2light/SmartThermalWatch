//#include "stdafx.h"
#include "wifipacketinfo.h"

//WifiPacketInfo::WifiPacketInfo(PACKET_INFO& packetInfo)
//{
//	m_pkt = 0;
//	m_packet_info = packet_info;
//
//	m_pkt = &m_packet_info;
//	m_pkt->packet_init(m_pkt);
//
//	//重置
//	m_pkt->nReadPos = 0;
//	m_pkt->nWritePos = 0;
//	m_pkt->nCurrPkt = 0;
//}

WifiPacketInfo::WifiPacketInfo(bool bSmall)
{
    if (bSmall)
    {
        m_pkt = 0;
        m_packet_info = packet_info_small;

        m_pkt = &m_packet_info;
        m_pkt->packet_init(m_pkt);

        //重置
        m_pkt->nReadPos = 0;
        m_pkt->nWritePos = 0;
        m_pkt->nCurrPkt = 0;

    }else{

        m_pkt = 0;
        m_packet_info = packet_info_big;

        m_pkt = &m_packet_info;
        m_pkt->packet_init(m_pkt);

        //重置
        m_pkt->nReadPos = 0;
        m_pkt->nWritePos = 0;
        m_pkt->nCurrPkt = 0;
    }


}

WifiPacketInfo::~WifiPacketInfo(void)
{
    if(m_pkt)
    {
        m_pkt->packet_uninit(m_pkt);
    }
    m_pkt = 0;
}

void WifiPacketInfo::RestBuff()
{
    if (m_pkt)
    {
        for (int i = 0; i < RECV_PACKET_NUM; i ++)
        {
            m_pkt->packet[i].nStatus = RECV_BUFFER_STATUS_IDLE;
        }

        //重置
        m_pkt->nReadPos = 0;
        m_pkt->nWritePos = 0;
        m_pkt->nCurrPkt = 0;
    }
}
