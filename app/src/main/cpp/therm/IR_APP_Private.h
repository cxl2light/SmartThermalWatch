#ifndef IR_APP_PRIVATE_H 
#define IR_APP_PRIVATE_H 

#include "string.h"
  //头文件内容 
#include "zs_CmdList.h"
#include "zs_CmdProcess.h"
#include "Socket.h"
#include "ClientThread.h"


#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

typedef struct TagSession
{
    int bUsing; //使用中

    int tcpConnect; //tcp链接状态

    char ipv4[32]; //ipv4 a

    AppTool::Socket *pSocket; //tcp 资源

    ClientThread *pClientThread; //线程

} Session;

extern Session g_session[];


int index2No(int value);
int No2index(int value);
bool isValidIndex(int index);

#endif 