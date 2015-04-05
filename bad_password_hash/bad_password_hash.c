#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
/* bad_password_hash: a 60-bit password hash that's reversible for passwords up to 9 characters.
*  bad_password_hash is not suitable for any purpose and its use is officially discouraged.
*  This software was written by Karl Magdsick and placed in the public domain.  Use at your own risk.*/

/* pow(94,8) and pow(94, 9) */
#define POW94_8 0x0015A7FF487A8100ull
#define POW94_9 0x07F3AFBC9CFB5E00ull

static char alphabet64[] = "+/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

typedef struct key {
  uint64_t multiplier;
  uint64_t addend;
} key_t;

/* Reads in key in either standard or websafe base64 encoding, 20 characters */
int read_key(key_t* key, const char* b64key) {
  const char* pos;
  uint64_t low, high, carry;
  int i;
  high = low = 0;

  for (i=0; i < 20; ++i) {
    high = (high << 6) ^ (low >> 58);
    low <<= 6;
    pos = strchr(alphabet64, b64key[i]);
    if (!pos || !b64key[i]) {return 0;}
    low ^= 0x3F & (pos-alphabet64-2);
  }

  /* Mix key entropy. x^120 + x^88 + x^46 + x^9 + 1 is a primitive polynomial (mod 2) */
  for (i=360; i > 0; --i) {
    carry = low & 1;
    low = (low >> 1) ^ (high << 63) ^ (carry << 8);
    high = (high >> 1) ^ (carry << 55) ^ (carry << 23);
  }

  key->addend = low % POW94_9;
  key->multiplier =  high + ((low / POW94_9) << 56);
  if (key->multiplier < POW94_8) {key->multiplier += POW94_8;} /* Ensure carry into the high base94 digit */
  return 1;
}

uint64_t password_hash(const char* password, const key_t* key) {
  uint64_t result;
  int i;

  for (result=0; *password; ++password) {
    result = ((result / 94) + (key->multiplier * (result % 94))) % POW94_9;
    result = (result + (*password % 94) * POW94_8) % POW94_9;
  }

  /* Mix entropy using LFSR to make password lengths less obvious */
  result <<= 4;
  for (i=60; i > 0; --i) {
    result = (result << 1) ^ ((result >> 63) * 0xA856975F2C585210ull);
  }
  result >>= 4;

  return (result + key->addend) & 0x0FFFFFFFFFFFFFFFull;
}

void password_unhash(char* password, uint64_t hash_value, const key_t* key) {
  int i;
  char c;

  hash_value = (hash_value - key->addend) & 0x0FFFFFFFFFFFFFFFull;
  for (i=60; i > 0; --i) {
    hash_value = (hash_value >> 1) ^ ((hash_value & 1) * 0x0D42B4BAF962C290ull);
  }

  while (hash_value != 0 && (hash_value % 94) == 0) {hash_value /= 94;}
  while (hash_value != 0) {
    c = hash_value % 94;
    hash_value /= 94;
    if (c<33) {c += 94;} /* Output only printable ASCII characters. */
    *(password++) = c;;
  }
}

int main(int argc, const char** argv) {
  key_t key;
  uint64_t hash;
  const char *password, *base64_key;
  char password_out[10];

  if( argc != 3 ) {printf("Usage: bad_password_hash base64_key password\n"); return 2;}
  password = argv[2];
  base64_key = argv[1];
  if (!read_key(&key, base64_key)) {printf("Bad base64 key %s. Try dd if=/dev/urandom bs=15 count=1 | base64\n", base64_key); return 3;}
  hash = password_hash(password, &key);
  printf("hash:%016llX\n", hash);
  memset(password_out, 0, sizeof(password_out));
  password_unhash(password_out, hash, &key);
  printf("password: %s\n", password_out);
}
