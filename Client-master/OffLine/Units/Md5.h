#pragma once

class CMd5
{
private:
	unsigned int	m_pCount[2];		/* message length in bits, lsw first */
	unsigned int	m_pDigestBuffer[4];	/* digest buffer */
	unsigned char	m_pBuffer[64];		/* accumulate block */

	void Process(const unsigned char *pData);
public:

	void Begin();
	void Append(const unsigned char *pData,unsigned int uLength);
	void End(unsigned char pDigest[16]);

	// ���õ���ԭʼMD5��ת��ΪBASE16��ʽ���ַ���
	void GetMD5Base16Str( char* pBase16 );
};
