#ifndef _CODECONVERT_H_
#define _CODECONVERT_H_

#ifdef __cplusplus
extern "C" {
#endif

int GB2312ToUTF8(char * pOut, size_t iBufSize, char *pText, size_t iLen);
int UTF8ToGB2312(char * pOut, size_t iBufSize, char *pText, size_t iLen);

#ifdef __cplusplus
}
#endif

#endif /* _CODECONVERT_H_ */
