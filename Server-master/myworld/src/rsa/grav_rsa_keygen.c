#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>

#define UNUSED( var ) ((void)var)

static int fprint_bytes_hex( const unsigned char * bin, int n, FILE * fp )
{
	int i, ret = 0;

	ret += fprintf( fp, "\t" );
	for( i = 0; i < n; i++ )
	{
		ret += fprintf( fp, "0x%02x, ", *bin++ );
		if( !( ( i + 1 ) % 24 ) )
			ret += fprintf( fp, "\n\t" );
	}

	ret += fprintf( fp, "\n" );
	return ret;
}

static int fprint_bignumber_hex( const BIGNUM * bn, FILE * fp )
{
	int n;
	unsigned char * bin = NULL;

	n = BN_num_bytes( bn );
	bin = calloc( n, 1 );
	if( !bin )
		return 0;

	BN_bn2bin( bn, bin );
	n = fprint_bytes_hex( bin, n, fp );

	free( bin );
	bin = NULL;

	return n;
}

static int write_head( FILE * fp )
{
	int ret = 0;

	ret += fputs( "//! This file is genterated by program.\n", fp );
	ret += fputs( "//! Please don't modify it.\n", fp );
	ret += fputs( "#include \"grav_rsa_types.h\"\n\n\n", fp );

	return ret;
}

static int store_public_key( RSA * rsa, FILE * fp )
{
	int ret = 0;

	ret += fputs( "static const unsigned char key_e[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->e, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static const unsigned char key_n[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->n, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static struct RSAPublicKey dummy =\n{\n", fp );
	ret += fprintf( fp, "\t%d,\n\t%d,\n", BN_num_bits(rsa->n), rsa->pad );
	ret += fputs( "\t{ sizeof(key_e), key_e, },\n", fp );
	ret += fputs( "\t{ sizeof(key_n), key_n, },\n", fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "struct RSAPublicKey * IDRSAPublicKey = &dummy;\n", fp );
	return ret;
}

static int store_private_key( RSA * rsa, FILE * fp )
{
	int ret = 0;

	ret += fputs( "static const unsigned char key_n[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->n, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static const unsigned char key_d[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->d, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static const unsigned char key_p[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->p, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static const unsigned char key_q[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->q, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static const unsigned char key_dmp1[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->dmp1, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static const unsigned char key_dmq1[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->dmq1, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static const unsigned char key_iqmp[] =\n{\n", fp );
	ret += fprint_bignumber_hex( rsa->iqmp, fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "static struct RSAPrivateKey dummy =\n{\n", fp );
	ret += fprintf( fp, "\t%d,\n\t%d,\n", BN_num_bits(rsa->n), rsa->pad );
	ret += fputs( "\t{ sizeof(key_n), key_n, },\n", fp );
	ret += fputs( "\t{ sizeof(key_d), key_d, },\n", fp );
	ret += fputs( "\t{ sizeof(key_p), key_p, },\n", fp );
	ret += fputs( "\t{ sizeof(key_q), key_q, },\n", fp );
	ret += fputs( "\t{ sizeof(key_dmp1), key_dmp1, },\n", fp );
	ret += fputs( "\t{ sizeof(key_dmq1), key_dmq1, },\n", fp );
	ret += fputs( "\t{ sizeof(key_iqmp), key_iqmp, },\n", fp );
	ret += fputs( "};\n\n", fp );

	ret += fputs( "struct RSAPrivateKey * IDRSAPrivateKey = &dummy;\n", fp );
	return ret;
}

int main( int argc, char * argv[] )
{
	FILE * fp = NULL;
	RSA * rsa = NULL;
	int num_bits = 1024, n = 0;

	if( argc > 1 )
		num_bits = atoi( argv[1] );
	if( num_bits <= 0 )
		num_bits = 1024;


	printf( "generating %d bit RSA key ...\n", num_bits );
	rsa = RSA_generate_key( num_bits, RSA_F4, NULL, NULL );
	if( !rsa )
	{
		fprintf( stderr, "Can not generate RSA key.\n" );
		goto error;
	}

	if( !( fp = fopen( "id_rsa_pub.c", "w" ) ) )
	{
		fprintf( stderr, "Can not open file : id_rsa_pub.c.\n" );
		goto error;
	}
	
	n = 0;
	n += write_head( fp );
	n += store_public_key( rsa, fp );
	printf( "public key .c file : %d bytes.\n", n );

	if( !( fp = fopen( "id_rsa.c", "w" ) ) )
	{
		fprintf( stderr, "Can not open file : id_rsa.c.\n" );
		goto error;
	}
	
	n = 0;
	n += write_head( fp );
	n += store_private_key( rsa, fp );
	printf( "private key .c file : %d bytes.\n", n );

error:
	if( fp )
		fclose( fp );
	if( rsa )
		RSA_free( rsa );
	return 0;
}
