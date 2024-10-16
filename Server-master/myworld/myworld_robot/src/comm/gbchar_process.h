#ifndef GBCHAR_PROCESS_H
#define GBCHAR_PROCESS_H


#define HZ_GB 0
#define HZ_BIG5 1
#define HZ_JPN 2
#define HZ_GBK 3
#define HZ_TGB 4
#define HZ_TSIGN 5
#define HZ_TJPN 7

#define INLINE

static INLINE int	ISDBCSLEAD(int CodeType,unsigned char c)
{
	switch(CodeType){
	case HZ_GB:
		return(c>0xa0 && c<0xff);
	case HZ_BIG5:
		return(c>0xa0 && c<0xff);
	case HZ_JPN:
		return((c>0x80 && c<0xa0)||(c>0xdf && c<0xfd));
	case HZ_GBK:
		return(c>0x80 && c<0xff);
	case HZ_TGB:
		return(c>0xaf && c<0xf8);
	case HZ_TSIGN:
		return(c>0xa0 && c<0xa5);
	case HZ_TJPN:
		return((c == 0xa4)||(c==0xa5));
	}
	return	0;
}

static INLINE int	ISDBCSNEXT(int CodeType,unsigned char c)
{
	switch(CodeType){
	case HZ_GB:
		return(c>0xa0 && c<0xff);
	case HZ_BIG5:
		return((c>0x3f && c<0x7f)||(c>0xa0 && c<0xff));
	case HZ_JPN:
		return(c>0x3f && c<0xfd);
	case HZ_GBK:
		return((c>0x3f && c<0x7f)||(c>0x7f && c<0xff));
	case HZ_TGB:
		return(c>0xa0 && c<0xff);
	case HZ_TSIGN:
		return(c>0xa0 && c<0xff);
	case HZ_TJPN:
		return(c>0xa0 && c<0xf4);
	}
	return	0;
}

extern INLINE int Isspace(unsigned char ch)
{
	return (int) memchr(" \t\n\r", ch ,4);
}

extern INLINE int ishtmlalpha(unsigned char ch)
{
	return (int) memchr("<>&:/\\",ch,6);
}

extern INLINE int isUnPrintableChnChar(unsigned char first,unsigned char next)
{
	if ((first == 0xd7) && (next >0xf9))
		return 1;
	return 0;
}

extern INLINE unsigned char convGbAlphaToEng(unsigned char first,unsigned char next)
{
	return (next-0x80);
}

extern INLINE int isGbAlpha(unsigned char first,unsigned char next)
{
	if ((first==0xa3) && ((next>=0xc1 && next <=0xda) || (next>=0xe1 && next <=0xfa)))
		return 1;
	return 0;
}

extern INLINE int isGbDigit(unsigned char first,unsigned char next)
{
	if ((first==0xa3) && (next>=0xb0 && next <=0xb9))
		return 1;
	return 0;
}

extern INLINE unsigned char convGbDigitToEng(unsigned char first,unsigned char next)
{
	return (next-0x80);
}

extern INLINE unsigned char convGbUrlCharToEng(unsigned char first,unsigned char next)
{
	unsigned char ret = 0;
	if (first == 0xa1 && next == 0xa3)
		ret = '.';
	else if (first == 0xa1 && next == 0xc3)
		ret = ':';
	else if (first == 0xa1 && next == 0xaf)
		ret = '/';
	return ret;	
}

extern INLINE int isGbUrlChar(unsigned char first,unsigned char next)
{
	if (first == 0xa1 && next == 0xa3)
		return 1;
	else if (first == 0xa1 && next == 0xc3)
		return 1;
	else if (first == 0xa1 && next == 0xaf)
		return 1;
	return 0;
}

extern INLINE int isUrlChar(unsigned char ch)
{
	if (ch == '.' || ch == ':' || ch == '/')
		return 1;
	return 0;
}



#endif
