#ifndef __APEXPROXY_LIB__
#define __APEXPROXY_LIB__

#define s_OK(code)		((CH_RESULT)(code) >= 0)
#define s_FAIL(code)	((CH_RESULT)(code) < 0)


#ifdef __cplusplus
extern "C" {
#endif

	//s_start
	//comment:	Start apex server.
	//returns:
	//		0		Success.
	int		s_start(const char *szConfigPath);

	//s_end
	//comment:	Stop apex server.
	void		s_end();	

	//s_sendData_ToApex
	//comment:	Game Server use this function to send data to apex server.
	//parameters:
	//			cMsgId		Message type which send to apex server.
	//						'L'		User login, content is player name which provided by GameServer.
	//						'G'		User logout, content is player name which provided by GameServer.
	//						'S'		User ip, content is player ip which provided by GameServer.
	//						'T'		Apex data from Game Client, content is data which received from Game Client.
	//			nUserId		The unique id of each user which created by Game Server.
	//			pBuffer		The buffer pointer.
	//			nLen		The buffer length.
	void		s_sendData_ToApex( char cMsgId, unsigned int nUserId, const char * pBuff, int nBuffLen );
	
	//s_getData_FromApex
	//comment:	Game Server use this function to receive data from apex server.
	//parameters:
	//			pBuff		The buffer pointer.
	//			nMaxBuffLen	The buffer length.
	//			pcMsgId		Message type which receive from apex server.
	//						'D'		Data need be sent to Apex Client, content is data from Apex.
	//						'K'		The user with pUserId need be killed, content is data from Apex.
	//			pUserId		The unique id of each player which created by Game Server.
	//			pRetLen		The length of the data which need to be received.
	//returns:
	//			0		Success.
	//			-3		Have no ApexProxy.dll
	int		s_getData_FromApex(char* pBuff, int nMaxBuffLen, char * pcMsgId, unsigned int * pUserId, int * pRetLen );

#ifdef __cplusplus
}
#endif


#endif  //eof __APEXPROXY_LIB__



