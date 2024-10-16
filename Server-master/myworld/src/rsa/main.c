#include "grav_rsa.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define UNUSED( var ) ((void)var)
#define ALIGN( size, boundary ) (((size) + ((boundary) - 1)) & ~((boundary) - 1))


extern RSA_PUBLIC_KEY * IDRSAPublicKey;
extern RSA_PRIVATE_KEY * IDRSAPrivateKey;


int print_hex( const void * blob, int n )
{
	const unsigned char * raw = blob;
	int i;

	puts("-----------------------------------------------------");
	for( i = 0; i < n; i++ )
		printf( "%02x ", *raw++ );
	printf( "\n" );
	puts("-----------------------------------------------------");
	return n;
}


int main( int argc, char * argv[] )
{
	RSA * public_rsa = NULL, * private_rsa = NULL;

	char text[] = "I was born for this!", decr[sizeof(text)], * encr = NULL;
	int n;

	UNUSED( argc );
	UNUSED( argv );

	public_rsa = rsa_from_public_key( IDRSAPublicKey );
	private_rsa = rsa_from_private_key( IDRSAPrivateKey );
	if( !public_rsa || !private_rsa )
		goto error;

	memset( decr, 0, sizeof(decr) );

	n = rsa_cryptograph_len( private_rsa, sizeof(text) );
	encr = calloc( n, 1 );
	if( !encr )
		goto error;
	
	puts( "\nprivate-encrypt ==> public-decrypt" );
	print_hex( text, sizeof(text) );
	n = rsa_private_encrypt( private_rsa, sizeof(text), text, encr );
	print_hex( encr, n );
	n = rsa_public_decrypt(  public_rsa, n, encr, decr );
	print_hex( decr, sizeof(text) );

	puts( "\npublic-encrypt ==> private-decrypt" );
	print_hex( text, sizeof(text) );
	n = rsa_public_encrypt( public_rsa, sizeof(text), text, encr );
	print_hex( encr, n );
	n = rsa_private_decrypt( private_rsa, n, encr, decr );
	print_hex( decr, sizeof(text) );

error:
	if( private_rsa )
		rsa_free( private_rsa );
	if( public_rsa )
		rsa_free( public_rsa );
	if( encr )
		free( encr );
	return 0;
}

// End of line
