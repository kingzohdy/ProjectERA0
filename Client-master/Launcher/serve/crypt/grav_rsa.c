#include "grav_rsa_types.h"
#include "grav_rsa.h"
#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>

RSA * rsa_from_public_key( const struct RSAPublicKey * pstKey );
RSA * rsa_from_private_key( const struct RSAPrivateKey * pstKey );

void rsa_free( RSA * pstRSA );

int rsa_cryptograph_len( RSA * rsa, int text_len );

int rsa_private_encrypt( RSA * pstRSA, int iNum, const void * pvFrom, void * pvTo );
int rsa_private_decrypt( RSA * pstRSA, int iNum, const void * pvFrom, void * pvTo );

int rsa_public_encrypt( RSA * pstRSA, int iNum, const void * pvFrom, void * pvTo );
int rsa_public_decrypt( RSA * pstRSA, int iNum, const void * pvFrom, void * pvTo );

#define UNUSED( var ) ((void)var)
#define ALIGN( size, boundary ) (((size) + ((boundary) - 1)) & ~((boundary) - 1))


RSA * rsa_from_public_key( const struct RSAPublicKey * key )
{
	RSA * rsa = NULL;
	
	rsa = RSA_new();
	if( !rsa )
		return NULL;
		
	rsa->e = BN_bin2bn( key->RSA_e.bin, key->RSA_e.size, rsa->e );
	if( !rsa->e )
		goto error;
	rsa->n = BN_bin2bn( key->RSA_n.bin, key->RSA_n.size, rsa->n );
	if( !rsa->n )
		goto error;

	return rsa;
error:
	if( rsa )
		RSA_free( rsa );
	rsa = NULL;
	return NULL;
}

RSA * rsa_from_private_key( const struct RSAPrivateKey * key )
{
	RSA * rsa = NULL;
	
	rsa = RSA_new();
	if( !rsa )
		return NULL;

	rsa->n = BN_bin2bn( key->RSA_n.bin, key->RSA_n.size, rsa->n );
	if( !rsa->n )
		goto error;
	rsa->d = BN_bin2bn( key->RSA_d.bin, key->RSA_d.size, rsa->d );
	if( !rsa->d )
		goto error;
	rsa->p = BN_bin2bn( key->RSA_p.bin, key->RSA_p.size, rsa->p );
	if( !rsa->p )
		goto error;
	rsa->q = BN_bin2bn( key->RSA_q.bin, key->RSA_q.size, rsa->q );
	if( !rsa->q )
		goto error;
	rsa->dmp1 = BN_bin2bn( key->RSA_dmp1.bin, key->RSA_dmp1.size, rsa->dmp1 );
	if( !rsa->dmp1 )
		goto error;
	rsa->dmq1 = BN_bin2bn( key->RSA_dmq1.bin, key->RSA_dmq1.size, rsa->dmq1 );
	if( !rsa->dmq1 )
		goto error;
	rsa->iqmp = BN_bin2bn( key->RSA_iqmp.bin, key->RSA_iqmp.size, rsa->iqmp );
	if( !rsa->iqmp )
		goto error;
		
	return rsa;
error:
	if( rsa )
		RSA_free( rsa );
	rsa = NULL;
	return NULL;
}

int rsa_cryptograph_len( RSA * rsa, int text_len )
{
	int num_bytes;
	
	if( !rsa || !rsa->n )
		return 0;
	
	num_bytes = BN_num_bytes( rsa->n );
	if( num_bytes <= 0 || text_len > num_bytes )
		return 0;

	return num_bytes;
}

int rsa_private_encrypt( RSA * rsa, int len, const void * from, void * to )
{
	return RSA_private_encrypt( len, from, to, rsa, RSA_PKCS1_PADDING );
}

int rsa_private_decrypt( RSA * rsa, int len, const void * from, void * to )
{
	return RSA_private_decrypt( len, from, to, rsa, RSA_PKCS1_PADDING );
}

int rsa_public_encrypt( RSA * rsa, int len, const void * from, void * to )
{
	return RSA_public_encrypt( len, from, to, rsa, RSA_PKCS1_PADDING );
}

int rsa_public_decrypt( RSA * rsa, int len, const void * from, void * to )
{
	return RSA_public_decrypt( len, from, to, rsa, RSA_PKCS1_PADDING );
}

void rsa_free( RSA * rsa )
{
	RSA_free( rsa );
}
