#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;

#define circular_shift(bits,word)      (((word) << (bits)) | ((word) >> (32-(bits))))

uint32_t A, B, C, D, E;
uint32_t H0 = 0x67452301;
uint32_t H1 = 0xEFCDAB89;
uint32_t H2 = 0x98BADCFE;
uint32_t H3 = 0x10325476;
uint32_t H4 = 0xC3D2E1F0;
uint32_t W[80];
uint32_t TEMP;

const uint32_t K[] = { 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };

uint32_t f00(uint32_t B, uint32_t C, uint32_t D) {
	return (B & C) | ((~B) & D);
}

uint32_t f20(uint32_t B, uint32_t C, uint32_t D) {
	return B ^ C ^ D;
}

uint32_t f40(uint32_t B, uint32_t C, uint32_t D) {
	return (B & C) | (B & D) | (C & D);
}

uint32_t f60(uint32_t B, uint32_t C, uint32_t D) {
	return B ^ C ^ D;
}

void dump(uint32_t x) {
	printf("%x ", (uint)(((x >> 24) & 0xff)));
	printf("%x ", (uint)(((x >> 16) & 0xff)));
	printf("%x ", (uint)(((x >> 8) & 0xff)));
	printf("%x ", (uint)((x & 0xff)));
}

int main() {
	unsigned char str[] = "a";
	long len = sizeof(str) / sizeof(char) - 1;
	int n = floor(len / 64.0) + 1;
	int nbuf = n * 64;
	unsigned char block[nbuf];
	for (int i = 0; i < len; i++) {
		block[i] = str[i];
	}
	// padding
	for (int i = len; i < nbuf; i++) {
		if (i == len)
			block[i] = 0x80;
		else if (i < nbuf - 8)
			block[i] = 0;
		else if (nbuf - 8 <= i) {
			block[i] = ((len * 8) >> (8 * (nbuf - i - 1))) & 0xff;
		}
	}

	for (int t = 0; t < 16; t++) {
		W[t] = block[t * 4] << 24;
		W[t] |= block[t * 4 + 1] << 16;
		W[t] |= block[t * 4 + 2] << 8;
		W[t] |= block[t * 4 + 3];
	}
	for (int t = 16; t < 80; t++) {
		W[t] = circular_shift(1,
				W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
	}

	A = H0;
	B = H1;
	C = H2;
	D = H3;
	E = H4;

	int t;
	for (t = 0; t < 20; t++) {
		TEMP = circular_shift(5, A) + f00(B, C, D) + E + W[t] + K[0];
		E = D;
		D = C;
		C = circular_shift(30, B);
		B = A;
		A = TEMP;
	}

	for (t = 20; t < 40; t++) {
		TEMP = circular_shift(5, A) + f20(B, C, D) + E + W[t] + K[1];
		E = D;
		D = C;
		C = circular_shift(30, B);
		B = A;
		A = TEMP;
	}

	for (t = 40; t < 60; t++) {
		TEMP = circular_shift(5, A) + f40(B, C, D) + E + W[t] + K[2];
		E = D;
		D = C;
		C = circular_shift(30, B);
		B = A;
		A = TEMP;
	}

	for (t = 60; t < 80; t++) {
		TEMP = circular_shift(5, A) + f60(B, C, D) + E + W[t] + K[3];
		E = D;
		D = C;
		C = circular_shift(30, B);
		B = A;
		A = TEMP;
	}

	H0 += A;
	H1 += B;
	H2 += C;
	H3 += D;
	H4 += E;

	dump(H0);
	dump(H1);
	dump(H2);
	dump(H3);
	dump(H4);

	return 0;
}
