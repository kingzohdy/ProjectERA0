#ifndef __MD5_H__
#define __MD5_H__

void Md5Calc(char output[16], const char *input, size_t length);
bool Md5Verify(const char target[16], const char *input, size_t length);

#endif
