#include <stdio.h>
#include <stdexcept>

#include "SocketUtils.h"

#include "ErrorLog.h"

#define HEADSIZE 8

SocketUtils::SocketUtils(boost::asio::io_service& io)
:io_srv(io), sock_read_timeout_sec_(DEFAULT_SOCK_READ_TIMEOUT)
{/*{{{*/

}/*}}}*/

SocketUtils::~SocketUtils()
{/*{{{*/

}/*}}}*/

acceptor_ptr SocketUtils::ListenRemote(short port)
{/*{{{*/
    try
    {
        boost::asio::ip::tcp::acceptor *acc_ = new boost::asio::ip::tcp::acceptor(io_srv,
                                    boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
        acc_->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	    acceptor_ptr acceptor(acc_);
        return acceptor;
    }
    catch (std::exception& e)
    {
        ErrorLog(LEVEL_ERROR,"socke listen on port failed with Exception:[%s]",e.what());
        exit(0);
    }
}/*}}}*/

bool SocketUtils::AcceptRemote(acceptor_ptr acceptor,socket_ptr& socket)
{/*{{{*/
    try
    {
        socket = socket_ptr(new boost::asio::ip::tcp::socket(io_srv));
        boost::system::error_code ec;
        acceptor->accept(*socket,ec);
        if (ec)
        {
            ErrorLog(LEVEL_ERROR,"AcceptRemote error!");
            return false;
        }

#ifdef WIN32
		/*boost::asio::socket_base::linger option(true, 30);
        socket->set_option(option);*/

		unsigned int native_socket = socket->native();
		int result = -1;
		struct timeval timeout = {30000,0}; 
		if (native_socket != 0)
		{
			if ((result = setsockopt(native_socket, SOL_SOCKET, SO_SNDTIMEO,(char *)&timeout, sizeof(struct timeval))) != 0)
				ErrorLog(LEVEL_ERROR, "Set send timeout failed with result !", result);
			if ((result = setsockopt(native_socket, SOL_SOCKET, SO_RCVTIMEO,(char *)&timeout, sizeof(struct timeval))) != 0)
				ErrorLog(LEVEL_ERROR, "Set recv timeout failed with result !", result);
		}
#endif

        return true;
    }
    catch(std::exception& e)
	{
        ErrorLog(LEVEL_ERROR,"socke accept Exception:[%s]",e.what());
	}
    return false;
}/*}}}*/

bool SocketUtils::ConnectRemote(const char* strIP, const char* strPort,socket_ptr& socket, uint32_t retry_times)
{/*{{{*/
	socket = socket_ptr(new boost::asio::ip::tcp::socket(io_srv));
	boost::system::error_code error;
	boost::asio::ip::tcp::resolver resolver(io_srv);
	boost::asio::ip::tcp::resolver::query query(strIP, strPort);
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	for (uint32_t i = 0; i <= retry_times; i++)
	{
		socket->connect(*endpoint_iterator, error);
		if (!error)
		{
#ifdef WIN32
			/*boost::asio::socket_base::linger option(true, 30);
			socket->set_option(option);*/

			unsigned int native_socket = socket->native();
			int result = -1;
			struct timeval timeout = {30000,0};
			if (native_socket != 0)
			{
				if ((result = setsockopt(native_socket, SOL_SOCKET, SO_SNDTIMEO,(char *)&timeout, sizeof(timeout))) != 0)
					ErrorLog(LEVEL_ERROR, "Set send timeout failed with result !", result);
				if ((result = setsockopt(native_socket, SOL_SOCKET, SO_RCVTIMEO,(char *)&timeout, sizeof(timeout))) != 0)
					ErrorLog(LEVEL_ERROR, "Set recv timeout failed with result !", result);
			}
#endif

			return true;
		}
		else
        {
			#ifdef WIN32
					Sleep(1000);
			#else
					sleep(1);
			#endif            
        }
	}

	ErrorLog(LEVEL_ERROR,"ConnectRemote ip[%s], port[%s] failed after retring [%d] times", strIP, strPort, retry_times);
	return false;
}/*}}}*/
static int sock_readable(int fd, int timeout_sec) 
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval timeo = {timeout_sec, 0};
    return select(fd + 1, &fds, NULL, NULL, &timeo);
}

static int readAll(int fd, char *buf, size_t ntotal, int timeout_sec)
{
    int n2read = ntotal;
    int nread = 0;
    int nbytes = 0;
    while (1)
    {
        if (sock_readable(fd, timeout_sec) <= 0)
            return -1;
        nbytes = read(fd, buf+nread, n2read);
        if (nbytes < 0)
            return -1;
        n2read -= nbytes;
        nread += nbytes;
        if ((size_t)nread >= ntotal)
            return nread;
    }
    return 0;
}

int SocketUtils::socketRead(socket_ptr socket,char *buf)
{/*{{{*/
	int nread=-1;
    char* tmpData=NULL;
    char head[HEADSIZE+1]={0};
    char tmphead[HEADSIZE]={0};
    try
    {
        if (socket->native() < 0) 
        {
            ErrorLog(LEVEL_ERROR, "invalid socket descriptor!");
            return -1;
        }

        // check the socket is ready for reading
        if (sock_readable(socket->native(), sock_read_timeout_sec_) <= 0) 
        {
            ErrorLog(LEVEL_ERROR,"socketRead no data in %d seconds: %s!", 
                    sock_read_timeout_sec_, strerror(errno));
            return -1;
        }

#ifdef WIN32
        boost::system::error_code error;
        nread = boost::asio::read(*socket, boost::asio::buffer(tmphead), 
                boost::asio::transfer_at_least(1), error);
        strncpy(head,tmphead,HEADSIZE);
        if (error == boost::asio::error::eof)
        {
            ErrorLog(LEVEL_ERROR,"socketRead error!");
            return -1; // Connection closed cleanly by peer.
        }
		else if (error)
        {
            ErrorLog(LEVEL_ERROR,"socketRead An error occurred!");
			throw boost::system::system_error(error); // Some other error.
        }
#else
        nread = read(socket->native(), head, HEADSIZE);
        if (nread <= 0)
        {
            ErrorLog(LEVEL_ERROR,"system read error[%s]", strerror(errno));
            return -1;
        }
        head[HEADSIZE] = 0;
#endif

        if(nread != HEADSIZE)
        {
            ErrorLog(LEVEL_ERROR,"socketRead read head error!");
            return -1; 
        }

        tmpData = (char*) malloc(atoi(head) * sizeof(char));
        if(tmpData == NULL)
        {
            ErrorLog(LEVEL_ERROR,"socketRead tmpData malloc error!");
            return -1;
        }
        memset(tmpData,0,atoi(head) * sizeof(char));

        // check packet body is ready
        if (sock_readable(socket->native(), sock_read_timeout_sec_) <= 0) 
        {
            ErrorLog(LEVEL_ERROR,"socketRead no data in %d seconds: %s!", 
                    sock_read_timeout_sec_, strerror(errno));
            free(tmpData);
            return -1;
        }

#ifdef WIN32
        nread = boost::asio::read(*socket, boost::asio::buffer(tmpData, 
                    atoi(head)), boost::asio::transfer_all(), error);
        if (error == boost::asio::error::eof)
        {
            ErrorLog(LEVEL_ERROR,"socketRead error!");
            nread = -1;
            goto socketRead_OUT;
        }
        else if (error)
        {
            ErrorLog(LEVEL_ERROR,"socketRead An error occurred!");
            throw boost::system::system_error(error); // Some other error.
        }
#else
        nread = readAll(socket->native(), tmpData, (size_t)atoi(head), sock_read_timeout_sec_);
        if (nread <= 0)
        {
            ErrorLog(LEVEL_ERROR,"system read error[%s]", strerror(errno));
            goto socketRead_OUT;
        }
#endif
        if (nread != atoi(head))
        {
            ErrorLog(LEVEL_ERROR,"socketRead nread=[%d] != head=[%d]",nread,atoi(head));
            nread = -1;
            goto socketRead_OUT;
        }
		strncpy(buf,tmpData,atoi(head));
    }
	catch(std::exception& e)
	{
        ErrorLog(LEVEL_ERROR,"socketRead Exception:[%s]",e.what());
        nread = -1;
        goto socketRead_OUT;
	}
socketRead_OUT:
    if(tmpData != NULL)
    {
        free(tmpData);
    }
    return nread;
}/*}}}*/

int SocketUtils::socketWrite(socket_ptr socket, char *buf, uint32_t len)
{/*{{{*/
	int nwrite=-1;
    char head[HEADSIZE+1]={0};
	try{
        sprintf(head, "%08lu", strlen(buf));
		nwrite = boost::asio::write(*socket, boost::asio::buffer(head,strlen(head)));
        if(nwrite <= 0)
        {
            ErrorLog(LEVEL_ERROR,"socketWrite error!");
            return -1;
        }
		nwrite = boost::asio::write(*socket, boost::asio::buffer(buf,len));
        if (nwrite != atoi(head))
        {
            ErrorLog(LEVEL_ERROR,"socketWrite nwrite=[%d] != head=[%d]",nwrite,atoi(head));
            return -1;
        }
	}
	catch(std::exception& e)
	{
        ErrorLog(LEVEL_ERROR,"socketWrite Exception:[%s]",e.what());
        return -1;
	}
	return nwrite;
}/*}}}*/
