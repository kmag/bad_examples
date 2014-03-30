all: bad_password_hash

bad_password_hash: bad_password_hash.c
	gcc -O2 -Wall -o bad_password_hash bad_password_hash.c
