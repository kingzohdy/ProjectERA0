#include "stdafx.h"
#include "Md5.h"
#define LITTLE_ENDIAN

static const unsigned int T1	=0xd76aa478;
static const unsigned int T2	=0xe8c7b756;
static const unsigned int T3	=0x242070db;
static const unsigned int T4	=0xc1bdceee;
static const unsigned int T5	=0xf57c0faf;
static const unsigned int T6	=0x4787c62a;
static const unsigned int T7	=0xa8304613;
static const unsigned int T8	=0xfd469501;
static const unsigned int T9	=0x698098d8;
static const unsigned int T10	=0x8b44f7af;
static const unsigned int T11	=0xffff5bb1;
static const unsigned int T12	=0x895cd7be;
static const unsigned int T13	=0x6b901122;
static const unsigned int T14	=0xfd987193;
static const unsigned int T15	=0xa679438e;
static const unsigned int T16	=0x49b40821;
static const unsigned int T17	=0xf61e2562;
static const unsigned int T18	=0xc040b340;
static const unsigned int T19	=0x265e5a51;
static const unsigned int T20	=0xe9b6c7aa;
static const unsigned int T21	=0xd62f105d;
static const unsigned int T22	=0x02441453;
static const unsigned int T23	=0xd8a1e681;
static const unsigned int T24	=0xe7d3fbc8;
static const unsigned int T25	=0x21e1cde6;
static const unsigned int T26	=0xc33707d6;
static const unsigned int T27	=0xf4d50d87;
static const unsigned int T28	=0x455a14ed;
static const unsigned int T29	=0xa9e3e905;
static const unsigned int T30	=0xfcefa3f8;
static const unsigned int T31	=0x676f02d9;
static const unsigned int T32	=0x8d2a4c8a;
static const unsigned int T33	=0xfffa3942;
static const unsigned int T34	=0x8771f681;
static const unsigned int T35	=0x6d9d6122;
static const unsigned int T36	=0xfde5380c;
static const unsigned int T37	=0xa4beea44;
static const unsigned int T38	=0x4bdecfa9;
static const unsigned int T39	=0xf6bb4b60;
static const unsigned int T40	=0xbebfbc70;
static const unsigned int T41	=0x289b7ec6;
static const unsigned int T42	=0xeaa127fa;
static const unsigned int T43	=0xd4ef3085;
static const unsigned int T44	=0x04881d05;
static const unsigned int T45	=0xd9d4d039;
static const unsigned int T46	=0xe6db99e5;
static const unsigned int T47	=0x1fa27cf8;
static const unsigned int T48	=0xc4ac5665;
static const unsigned int T49	=0xf4292244;
static const unsigned int T50	=0x432aff97;
static const unsigned int T51	=0xab9423a7;
static const unsigned int T52	=0xfc93a039;
static const unsigned int T53	=0x655b59c3;
static const unsigned int T54	=0x8f0ccc92;
static const unsigned int T55	=0xffeff47d;
static const unsigned int T56	=0x85845dd1;
static const unsigned int T57	=0x6fa87e4f;
static const unsigned int T58	=0xfe2ce6e0;
static const unsigned int T59	=0xa3014314;
static const unsigned int T60	=0x4e0811a1;
static const unsigned int T61	=0xf7537e82;
static const unsigned int T62	=0xbd3af235;
static const unsigned int T63	=0x2ad7d2bb;
static const unsigned int T64	=0xeb86d391;

static inline unsigned int ROTATE_LEFT(unsigned int x,unsigned int n)
{
	return (x<<n) | (x >> (32-n));
}

static inline unsigned int F(unsigned int x,unsigned int y,unsigned int z)
{
	return (x & y)	| (~x & z);
}

void CMd5::Process(const unsigned char	*pData)
{
	unsigned int
		a =	m_pDigestBuffer[0],	b =	m_pDigestBuffer[1],
		c =	m_pDigestBuffer[2],	d =	m_pDigestBuffer[3];
	unsigned int t;

#if	defined(BIG_ENDIAN)	//faster

	unsigned int X[16];
	const unsigned char	*xp	= pData;
	unsigned int i;

	for	(i = 0;	i <	16;	++i, xp	+= 4)
		X[i] = xp[0] + (xp[1] << 8)	+ (xp[2] <<	16)	+ (xp[3] <<	24);

#elif defined(LITTLE_ENDIAN)

	unsigned int xbuf[16];
	const unsigned int *X;

	if (!((pData - (const unsigned char	*)0) & 3))
	{
		/* pData are properly aligned */
		X =	(const unsigned int	*)pData;
	} else {
		/* not aligned */
		memcpy(xbuf, pData,	64);
		X =	xbuf;
	}
#else
#error Either LITTLE_ENDIAN	or BIG_ENDIAN must be defined to compile this file.(LITTLE_ENDIAN for intel	cpu.)
#endif

	/* Round 1.	*/
	/* Let [m_pDigestBuffer	k s	i] denote the operation
	a =	b +	((a	+ F(b,c,d) + X[k] +	T[i]) <<< s). */
#define	SET(a, b, c, d,	k, s, Ti)\
t =	a +	F(b,c,d) + X[k]	+ Ti;\
a =	ROTATE_LEFT(t, s) +	b
	/* Do the following	16 operations. */
	SET(a, b, c, d,	0, 7, T1);
	SET(d, a, b, c,	1, 12, T2);
	SET(c, d, a, b,	2, 17, T3);
	SET(b, c, d, a,	3, 22, T4);
	SET(a, b, c, d,	4, 7, T5);
	SET(d, a, b, c,	5, 12, T6);
	SET(c, d, a, b,	6, 17, T7);
	SET(b, c, d, a,	7, 22, T8);
	SET(a, b, c, d,	8, 7, T9);
	SET(d, a, b, c,	9, 12, T10);
	SET(c, d, a, b,	10,	17,	T11);
	SET(b, c, d, a,	11,	22,	T12);
	SET(a, b, c, d,	12,	7, T13);
	SET(d, a, b, c,	13,	12,	T14);
	SET(c, d, a, b,	14,	17,	T15);
	SET(b, c, d, a,	15,	22,	T16);
#undef SET

	/* Round 2.	*/
	/* Let [m_pDigestBuffer	k s	i] denote the operation
	a =	b +	((a	+ G(b,c,d) + X[k] +	T[i]) <<< s). */
#define	G(x, y,	z) (((x) & (z))	| ((y) & ~(z)))
#define	SET(a, b, c, d,	k, s, Ti)\
t =	a +	G(b,c,d) + X[k]	+ Ti;\
a =	ROTATE_LEFT(t, s) +	b
	/* Do the following	16 operations. */
	SET(a, b, c, d,	1, 5, T17);
	SET(d, a, b, c,	6, 9, T18);
	SET(c, d, a, b,	11,	14,	T19);
	SET(b, c, d, a,	0, 20, T20);
	SET(a, b, c, d,	5, 5, T21);
	SET(d, a, b, c,	10,	9, T22);
	SET(c, d, a, b,	15,	14,	T23);
	SET(b, c, d, a,	4, 20, T24);
	SET(a, b, c, d,	9, 5, T25);
	SET(d, a, b, c,	14,	9, T26);
	SET(c, d, a, b,	3, 14, T27);
	SET(b, c, d, a,	8, 20, T28);
	SET(a, b, c, d,	13,	5, T29);
	SET(d, a, b, c,	2, 9, T30);
	SET(c, d, a, b,	7, 14, T31);
	SET(b, c, d, a,	12,	20,	T32);
#undef SET
#undef G
	/* Round 3.	*/
	/* Let [m_pDigestBuffer	k s	t] denote the operation
	a =	b +	((a	+ H(b,c,d) + X[k] +	T[i]) <<< s). */
#define	H(x, y,	z) ((x)	^ (y) ^	(z))
#define	SET(a, b, c, d,	k, s, Ti)\
t =	a +	H(b,c,d) + X[k]	+ Ti;\
a =	ROTATE_LEFT(t, s) +	b
	/* Do the following	16 operations. */
	SET(a, b, c, d,	5, 4, T33);
	SET(d, a, b, c,	8, 11, T34);
	SET(c, d, a, b,	11,	16,	T35);
	SET(b, c, d, a,	14,	23,	T36);
	SET(a, b, c, d,	1, 4, T37);
	SET(d, a, b, c,	4, 11, T38);
	SET(c, d, a, b,	7, 16, T39);
	SET(b, c, d, a,	10,	23,	T40);
	SET(a, b, c, d,	13,	4, T41);
	SET(d, a, b, c,	0, 11, T42);
	SET(c, d, a, b,	3, 16, T43);
	SET(b, c, d, a,	6, 23, T44);
	SET(a, b, c, d,	9, 4, T45);
	SET(d, a, b, c,	12,	11,	T46);
	SET(c, d, a, b,	15,	16,	T47);
	SET(b, c, d, a,	2, 23, T48);
#undef SET
#undef H
	/* Round 4.	*/
	/* Let [m_pDigestBuffer	k s	t] denote the operation
	a =	b +	((a	+ I(b,c,d) + X[k] +	T[i]) <<< s). */
#define	I(x, y,	z) ((y)	^ ((x) | ~(z)))
#define	SET(a, b, c, d,	k, s, Ti)\
t =	a +	I(b,c,d) + X[k]	+ Ti;\
a =	ROTATE_LEFT(t, s) +	b
	/* Do the following	16 operations. */
	SET(a, b, c, d,	0, 6, T49);
	SET(d, a, b, c,	7, 10, T50);
	SET(c, d, a, b,	14,	15,	T51);
	SET(b, c, d, a,	5, 21, T52);
	SET(a, b, c, d,	12,	6, T53);
	SET(d, a, b, c,	3, 10, T54);
	SET(c, d, a, b,	10,	15,	T55);
	SET(b, c, d, a,	1, 21, T56);
	SET(a, b, c, d,	8, 6, T57);
	SET(d, a, b, c,	15,	10,	T58);
	SET(c, d, a, b,	6, 15, T59);
	SET(b, c, d, a,	13,	21,	T60);
	SET(a, b, c, d,	4, 6, T61);
	SET(d, a, b, c,	11,	10,	T62);
	SET(c, d, a, b,	2, 15, T63);
	SET(b, c, d, a,	9, 21, T64);
#undef SET
#undef I
	/* Then	perform	the	following additions. (That is increment	each
	of the four	registers by the value it had before this block
	was	started.) */
	m_pDigestBuffer[0] += a;
	m_pDigestBuffer[1] += b;
	m_pDigestBuffer[2] += c;
	m_pDigestBuffer[3] += d;
}

void CMd5::Begin()
{
	m_pCount[0]	= m_pCount[1] =	0;
	m_pDigestBuffer[0] = 0x67452301;
	m_pDigestBuffer[1] = 0xefcdab89;
	m_pDigestBuffer[2] = 0x98badcfe;
	m_pDigestBuffer[3] = 0x10325476;
}

void CMd5::Append(const unsigned char *pData, unsigned int	uLength)
{
	const unsigned char	*p = pData;
	unsigned int left =	uLength;
	unsigned int offset	= (m_pCount[0] >> 3) & 63;
	unsigned int nbits = (unsigned int)(uLength	<< 3);

	if (uLength	<= 0)
		return;

	/* Update the message length. */
	m_pCount[1]	+= uLength >> 29;
	m_pCount[0]	+= nbits;
	if (m_pCount[0]	< nbits)
		m_pCount[1]++;

	/* Process an initial partial block. */
	if (offset){
		unsigned int copy =	(offset	+ uLength >	64 ? 64	- offset : uLength);

		memcpy(m_pBuffer + offset, p, copy);
		if (offset + copy <	64)
			return;
		p += copy;
		left -=	copy;
		Process(m_pBuffer);
	}

	/* Process full	blocks.	*/
	for	(; left	>= 64; p +=	64,	left -=	64)
		Process(p);

	/* Process a final partial block. */
	if (left)
		memcpy(m_pBuffer, p, left);
}

void CMd5::End(unsigned char pDigest[16])
{
	static const unsigned char pad[64] =
	{
		0x80, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0,	0,
			0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0,
			0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0,
			0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0
	};
	unsigned char pData[8];
	unsigned int i;

	/* Save	the	length before padding. */
	for	(i = 0;	i <	8; ++i)
		pData[i] = (unsigned char)(m_pCount[i >> 2]	>> ((i & 3)	<< 3));
	/* Pad to 56 b
	ytes mod 64. */
	Append(pad,	((55 - (m_pCount[0]	>> 3)) & 63) + 1);
	/* Append the length. */
	Append(pData, 8);
	for	(i = 0;	i <	16;	++i)
		pDigest[i] = (unsigned char)(m_pDigestBuffer[i >> 2] >>	((i	& 3) <<	3));
}

static size_t ArkUint82Base16( const unsigned char* pUint8, size_t sizeCode, char* pBase16, size_t sizeBuf )
{
	size_t nPos = 0;
    const unsigned char * pTail = pUint8 + sizeCode;
	for( unsigned char* pBuf = (unsigned char*)pUint8; pBuf != pTail; nPos++ )
	{
		if(pBase16)
		{
			if( 2 * nPos + 1>= sizeBuf )
				break;
			pBase16[2 * nPos + 1] = pUint8[nPos] & 0x0f;
			pBase16[2 * nPos] = pUint8[nPos]>>4;
			pBase16[2 * nPos + 1] += pBase16[2 * nPos + 1] < 10 ? '0' : 'a' - 10;
			pBase16[2 * nPos] += pBase16[2 * nPos] < 10 ? '0' : 'a' - 10;
		}
	}
	if(pBase16 && 2 * nPos < sizeBuf)
		pBase16[2 * nPos] = '\0';

	return 2 * nPos + 1;
}


void CMd5::GetMD5Base16Str(char* pBase16)
{
	unsigned char pDigest[16];
	End( pDigest );
	ArkUint82Base16(pDigest, 16, pBase16, 33);
}
