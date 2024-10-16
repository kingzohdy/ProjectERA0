#ifndef GRAY_RSA_TYPES_H
#define GRAY_RSA_TYPES_H

struct RSABigNumber
{
	int size;
	const unsigned char * bin;
};

// RSAË½Ô¿
struct RSAPrivateKey
{
	int num_bits;
	int padding;
	
	struct RSABigNumber RSA_n;
	struct RSABigNumber RSA_d;
	struct RSABigNumber RSA_p;
	struct RSABigNumber RSA_q;
	struct RSABigNumber RSA_dmp1;
	struct RSABigNumber RSA_dmq1;
	struct RSABigNumber RSA_iqmp;
};

// RSA¹«Ô¿
struct RSAPublicKey
{
	int num_bits;
	int padding;
	
	struct RSABigNumber RSA_e;
	struct RSABigNumber RSA_n;
};


#endif // GRAY_RSA_TYPES_H

