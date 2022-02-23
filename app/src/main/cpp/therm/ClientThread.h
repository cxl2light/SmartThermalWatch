#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include"Thread.h"
#include"Socket.h"
#include"IR_APP_Define.h"

using namespace AppTool;

class ClientThread : public Thread
{

public:
    ClientThread();
    virtual ~ClientThread();

    virtual void Routine();

protected:
    void ProcessTheCompletePackage( char *pMem, int len, int type);

public:
    FunGetJsonCamera _funGetJsonCamera;
    FunGetJsonDevConfig _funGetJsonDevConfig;
    FunGetUpgrade _funGetUpgrade;
    FunGetJsonAnalyseResult _funGetJsonAnalyseResult;

    int _channelNo;
protected:
    SOCKET _handle;

protected:
    volatile bool m_bStop;

    char* _pJsonDevConfig;
    char* _pJsonCamera;
    int _upgradeRet;
    char* _pJsonAnalyseResult;

public:
    void set_stop(bool bStop);
    bool get_stop();

    void set_socket(SOCKET handle);
};


#endif