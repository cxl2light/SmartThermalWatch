#include "ClientThread.h"
#include "IR_APP_Define.h"
#include "wifipacketinfo.h"
#include "zs_Types.h"
//#include<errno.h>
#include "string.h"
#include "KTime.h"

// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netinet/tcp.h>

#define MAX_JSON_BUFF_SIZE (32 * 1024)
#define MAX_Complete_BUF_SIZE (64 * 1024)


#include"IR_APP_SDK.h"
#include"IR_APP_Private.h"


ClientThread::ClientThread()
{

    _handle = INVALID_SOCKET;

    _pJsonDevConfig = new char[MAX_JSON_BUFF_SIZE];
    _pJsonCamera = new char[MAX_JSON_BUFF_SIZE];
    _upgradeRet = 0;
    _pJsonAnalyseResult = new char[MAX_JSON_BUFF_SIZE];

    _funGetJsonCamera = 0;
    _funGetJsonDevConfig = 0;
    _funGetUpgrade = 0;
    _pJsonAnalyseResult = 0;

    _channelNo = 0;

    set_stop(false);
}

ClientThread::~ClientThread()
{
    set_stop(true);
    Join();

    SAFE_DELETE(_pJsonDevConfig);
    SAFE_DELETE(_pJsonCamera);
    SAFE_DELETE(_pJsonAnalyseResult);

    _channelNo = 0;

    _funGetJsonCamera = 0;
    _funGetJsonDevConfig = 0;
    _funGetUpgrade = 0;
    _pJsonAnalyseResult = 0;
}

void ClientThread::Routine()
{
    LOG("%s%s\n", LOG_Prefix, "enter thread\n");

    set_stop(false);

    do
    {

        if (INVALID_SOCKET == _handle)
        {
            break;
        }

        AppTool::Socket socket;

        socket.Attach(_handle);

        char buff[4 * 1024] = {0};

        WifiPacketInfo *pTcpWifiPacketInfo = new WifiPacketInfo(false);

        char szCompleteBuf[MAX_Complete_BUF_SIZE] = {0};

        // KTime ktime;

        // ktime.start();

        while (!get_stop())
        {
            /* code */
            if (AppTool::Socket::SOCKET_Select(_handle, 1, true))
            {
                int recvLen = socket.Recv(buff, sizeof(buff));

                if (recvLen > 0)
                {
                    if (pTcpWifiPacketInfo)
                    {
                        pTcpWifiPacketInfo->m_pkt->process_packet(pTcpWifiPacketInfo->m_pkt, (unsigned char *)buff, recvLen);

                        int retRawLen = 0;
                        unsigned int packetType = 0;

                        int ret = pTcpWifiPacketInfo->m_pkt->get_a_packet(
                            pTcpWifiPacketInfo->m_pkt, (unsigned char *)szCompleteBuf, &retRawLen, &packetType);

                        if (ret >= 0 && retRawLen > 0)
                        {
                            ProcessTheCompletePackage(szCompleteBuf, retRawLen, packetType);
                        }
                    }
                }
                else
                {

                    LOG("socket error = %s\n", strerror(errno));

                    if (IR_APP_IsChannelAvailable(_channelNo))
                    {
                        int index = No2index(_channelNo);

                        g_session[index].tcpConnect = 0;
                    }

                    break;
                }
            }

            // if (ktime.elapsed() > 1.0)
            // {
            //     ktime.start();

            //     struct tcp_info info;
            //     int len = sizeof(info);
            //     getsockopt(_handle, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
            //     if ((info.tcpi_state == TCP_ESTABLISHED))
            //     {
            //         LOG("socket state : %s\n","TCP_ESTABLISHED");
            //     }else{
            //         LOG("socket error = %s\n", strerror(errno));
            //         break;
            //     }
            // }

            Thread::Sleep(1);
        }

        socket.Detach();

        SAFE_DELETE(pTcpWifiPacketInfo);
    } while (0);

    set_stop(true);

    LOG("%s%s\n", LOG_Prefix, "leave thread\n");
}

void ClientThread::set_stop(bool bStop)
{
    m_bStop = bStop;
}

bool ClientThread::get_stop()
{
    return m_bStop;
}

void ClientThread::set_socket(SOCKET handle)
{
    _handle = handle;
}

void ClientThread::ProcessTheCompletePackage(char *pMem, int len, int type)
{

    if (type == RES_GET_CAMERA)
    {
        if (_funGetJsonCamera)
        {
            _funGetJsonCamera(_channelNo, pMem);
        }
    }
    else if (type == RES_GET_DEVCONFIG)
    {
        if (_funGetJsonDevConfig)
        {
            if(len>0&&len<MAX_Complete_BUF_SIZE)
            {
                pMem[len] =0;
            }
            _funGetJsonDevConfig(_channelNo, pMem);
        }
    }
    else if (type == RES_UPGRADE)
    {
        if (_funGetUpgrade)
        {
            int *ret = (int *)pMem;

            _upgradeRet = *ret;

            _funGetUpgrade(_channelNo, _upgradeRet);
        }
    }
    else if (type == RES_ANALYSERESULT)
    {
        if (_funGetJsonAnalyseResult)
        {
            if(len>0&&len<MAX_Complete_BUF_SIZE)
            {
                pMem[len] =0;
            }
            _funGetJsonAnalyseResult(_channelNo, pMem);
        }
    }

}
