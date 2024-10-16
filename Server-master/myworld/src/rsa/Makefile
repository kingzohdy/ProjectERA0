CC=gcc
C_ARGS=-Wall -Wextra -g -c -mtune=i686 -march=i686 -I../../comm_release/include/
C_LIBS=-Wl,-Bstatic -L../../comm_release/lib/ -lcrypto -Wl,-Bdynamic -ldl

test : main.o id_rsa.o id_rsa_pub.o grav_rsa.o
	$(CC) main.o id_rsa.o id_rsa_pub.o grav_rsa.o -o test $(C_LIBS)

main.o : main.c grav_rsa.h
	$(CC) $(C_ARGS) main.c

grav_rsa.o : grav_rsa.c grav_rsa.h
	$(CC) $(C_ARGS) grav_rsa.c

id_rsa.o : id_rsa.c grav_rsa_types.h
	$(CC) $(C_ARGS) id_rsa.c

id_rsa_pub.o : id_rsa_pub.c grav_rsa_types.h
	$(CC) $(C_ARGS) id_rsa_pub.c

id_rsa.c : genkey
	./genkey 2048

id_rsa_pub.c : genkey
	./genkey 2048

#genkey : grav_rsa_keygen.c
#	$(CC) -Wall -Wextra -g -march=i686 grav_rsa_keygen.c -o genkey $(C_LIBS)
genkey : grav_rsa_keygen.c
	make -f KeyGen.mk

clean:
	rm *.o test genkey id_rsa.c id_rsa_pub.c

