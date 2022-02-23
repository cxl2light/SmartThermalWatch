
#ifndef _AppTool_Socket_H_
#define _AppTool_Socket_H_

#include <string>

// windows相关头文件
#ifdef _WIN32_AppTool
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32")
#pragma warning(disable : 4996)

// linux相关头文件、宏
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
typedef int SOCKET;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#define ioctlsocket ioctl
#define WSAGetLastError() errno


#endif

namespace AppTool
{

/////////////////////////////////// SockAddr ///////////////////////////////////////
class SockAddr
{
public:
	explicit SockAddr(void);
	explicit SockAddr(const std::string & ip, unsigned short port);

	void SetAddr(const sockaddr_in & addr);

	void SetPort(unsigned short port);
	unsigned short GetPort() const;

	void SetIP(const std::string & ip);
	std::string GetIP() const;

private:	
	sockaddr_in m_addr;
};

///////////////////////////////// Socket /////////////////////////////////////////
class Socket
{
public:
	Socket(void);
	Socket(SOCKET s);

	// 析构函数将会关闭 SOCKET，若不想关闭，调用Detach
	virtual ~Socket(void);

	SOCKET Create(int domain = AF_INET, int type = SOCK_DGRAM, int protocol = IPPROTO_IP);

	SOCKET Handle();
	bool IsValid();
	void Attach(SOCKET s);
	void Detach();

	// 关闭方式：0-reading 1-writing 2-both
	void Shutdown(int how);
	void Close();
	
	int Bind(const SockAddr & addr);
	int Listen(int backlog);
	SOCKET Accept(SockAddr * peer = NULL);
	int Connect(const SockAddr & peer);
	bool ConnectTimeOut(const SockAddr & peer, unsigned int intTimeoutMS);

	int SendTo(const char * buf, int len, const SockAddr & peer);
	int RecvFrom(char * buf, int len, SockAddr * peer = NULL);
	int Send(const char * buf, int len);
	int Recv(char * buf, int len);

	int IoBlocked(bool blocked = false);
	int SetSockOpt(int level, int optname, const char * optval, socklen_t optlen);
	int GetSockOpt( int level, int optname, char * optval, socklen_t * optlen);

	int SetBlocked(bool blocked = false);

	int GetPeerName(SockAddr & peer);
	int GetSockName(SockAddr & addr);

	// 组播相关操作
	int AddMembership(const std::string & multip, const std::string & localip);
	int DropMembership(const std::string & multip, const std::string & localip);

	// SOCKET属性快速设置函数
	int ReuseAddr(bool reuse = true);
	int SetRecvBufSize(int sz);
	int SetSendBufSize(int sz);
	int SetRecvTimeout(int ms);
	int SetSendTimeout(int ms);

	//默认发送超时.仅仅提供select使用 ,毫秒
	static const int s_nSendTimeOut = 100; 

#ifdef _WIN32_AppTool
	static BOOL SOCKET_Select(SOCKET hSocket, int nTimeOut, BOOL bRead);  //毫秒  检测可读性/检测可写性
#else
	static int SOCKET_Select(int hSocket, int nTimeOut, int bRead);
#endif

protected:
	SOCKET m_hSocket;
};

};	// namespace AppTool

#endif //_AppTool_Socket_H_

