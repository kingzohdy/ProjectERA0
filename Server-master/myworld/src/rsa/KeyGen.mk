CC=gcc
C_ARGS=-Wall -Wextra -g -c -mtune=i686 -march=i686 -I../../comm_release/include/
C_LIBS=-Wl,-Bstatic -L../../comm_release/lib/ -lcrypto -Wl,-Bdynamic -ldl


genkey : grav_rsa_keygen.o
	$(CC) grav_rsa_keygen.o -o genkey $(C_LIBS)

grav_rsa_keygen.o : grav_rsa_keygen.c grav_rsa_types.h
	$(CC) $(C_ARGS) grav_rsa_keygen.c

clean:
	rm *.o
	rm genkey

