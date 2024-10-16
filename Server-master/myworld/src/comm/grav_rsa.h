#ifndef GRAY_RSA_H
#define GRAY_RSA_H


// �ڲ�����,ʹ����ֻ�ܿ�������
typedef struct RSAPrivateKey RSA_PRIVATE_KEY;
typedef struct RSAPublicKey  RSA_PUBLIC_KEY;
typedef struct rsa_st RSA;

#ifdef __cplusplus
extern "C"
{
#endif

// �ӹ�Կ/˽Կ����RSA����.
RSA * rsa_from_public_key( const RSA_PUBLIC_KEY * pstKey );
RSA * rsa_from_private_key( const RSA_PRIVATE_KEY * pstKey );

void rsa_free( RSA * pstRSA );

// Ԥ�����ĵĳ���.
int rsa_cryptograph_len( RSA * pstRSA, int iTextLen );

// �ǶԳƼ���: rsa_private_encrypt -> rsa_public_decrypt
//             rsa_public_encrypt  -> rsa_private_decrypt
int rsa_private_encrypt( RSA * pstRSA, int iNum, const void * pvFrom, void * pvTo );
int rsa_private_decrypt( RSA * pstRSA, int iNum, const void * pvFrom, void * pvTo );

int rsa_public_encrypt( RSA * pstRSA, int iNum, const void * pvFrom, void * pvTo );
int rsa_public_decrypt( RSA * pstRSA, int iNum, const void * pvFrom, void * pvTo );

#ifdef __cplusplus
}
#endif

#endif // GRAY_RSA_H

