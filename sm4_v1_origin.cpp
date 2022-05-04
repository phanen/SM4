//
//#include "sm4.h"
//static inline uint8_t sm4_Sbox_get(uint8_t inch)
//{
//	uint8_t* pTable = (uint8_t*)SboxTable;
//	uint8_t retVal = (uint8_t)(pTable[inch]);
//	return retVal;
//}
//
//
////  T(.) := L(tau(.))
////  [in]: 32 bits unsigned value;
//static uint32_t sm4_func_T(uint32_t in)
//{
//	uint32_t bb = 0, c = 0;
//	uint8_t a[4], b[4];
//
//	// non-linear func-tau
//	W2B(in, a, 0);
//	b[0] = sm4_Sbox_get(a[0]);
//	b[1] = sm4_Sbox_get(a[1]);
//	b[2] = sm4_Sbox_get(a[2]);
//	b[3] = sm4_Sbox_get(a[3]);
//	B2W(b, bb, 0);
//	
//	// linear func-L 
//	c = bb ^ (ROT_SHL(bb, 2)) ^ (ROT_SHL(bb, 10)) ^ (ROT_SHL(bb, 18)) ^ (ROT_SHL(bb, 24));
//	return c;
//}
//
//// calc and get enc/dec result
//static inline uint32_t sm4_func_F(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3, uint32_t rk)
//{
//	return (x0 ^ sm4_func_T(x1 ^ x2 ^ x3 ^ rk));
//}
//
//// Calculating round encryption key.
//// args:    [in] a: a is a 32 bits unsigned value;
//// return: sk[i]: i{0,1,2,3,...31}
//static uint32_t sm4_func_T_key(uint32_t ka)
//{
//	uint32_t bb = 0;
//	uint32_t rk = 0;
//	uint8_t a[4];
//	uint8_t b[4];
//
//	// non-linear func-tau
//	W2B(ka, a, 0);
//	b[0] = sm4_Sbox_get(a[0]);
//	b[1] = sm4_Sbox_get(a[1]);
//	b[2] = sm4_Sbox_get(a[2]);
//	b[3] = sm4_Sbox_get(a[3]);
//	B2W(b, bb, 0);
//
//	// linear func-L'
//	rk = bb ^ (ROT_SHL(bb, 13)) ^ (ROT_SHL(bb, 23));
//	return rk;
//}
//
//
//// key <-- 16 * 8 = 128 
//static void sm4_gen_rk(uint32_t* rk, uint8_t* key)
//{
//	uint32_t MK[4];
//	uint32_t K[36];
//
//	B2W(key, MK[0], 0);
//	B2W(key, MK[1], 4);
//	B2W(key, MK[2], 8);
//	B2W(key, MK[3], 12);
//
//	K[0] = MK[0] ^ FK[0];
//	K[1] = MK[1] ^ FK[1];
//	K[2] = MK[2] ^ FK[2];
//	K[3] = MK[3] ^ FK[3];
//
//	for (uint32_t i = 0; i < 32; i++)
//	{
//		K[i + 4] = K[i] ^ (sm4_func_T_key(K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ CK[i]));
//		rk[i] = K[i + 4];
//	}
//
//#ifdef _debug
//	for (size_t i = 0; i < 32; i++)
//	{
//		printf("rk[%lld] = %02x\n", i, rk[i]);
//	}
//#endif
//}
//
//
//static void sm4_crypt_enc(uint32_t rk[32], uint8_t input[16], uint8_t output[16])
//{
//	uint32_t i = 0;
//	uint32_t ulbuf[36];
//
//	memset(ulbuf, 0, sizeof(ulbuf));
//
//	B2W(input, ulbuf[0], 0);
//	B2W(input, ulbuf[1], 4);
//	B2W(input, ulbuf[2], 8);
//	B2W(input, ulbuf[3], 12);
//
//	while (i < 32)
//	{
//		ulbuf[i + 4] = sm4_func_F(ulbuf[i], ulbuf[i + 1], ulbuf[i + 2], ulbuf[i + 3], rk[i]);
//#ifdef _debug
//		printf("rk(%02d) = 0x%08x,  X(%02d) = 0x%08x \n", i, rk[i], i, ulbuf[i + 4]);
//#endif
//		i++;
//	}
//
//	W2B(ulbuf[35], output, 0);
//	W2B(ulbuf[34], output, 4);
//	W2B(ulbuf[33], output, 8);
//	W2B(ulbuf[32], output, 12);
//}
//
//
//
//int main(int argc, char** argv)
//{
//	uint8_t key[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
//	uint8_t input[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
//	uint8_t output[16]{};
//
//	size_t i;
//	uint32_t rk[32];
//
//	// gen rk from key
//	sm4_gen_rk(rk, key);
//	sm4_crypt_enc(rk, input, output);
//
//	for (i = 0; i < 16; i++)
//		printf("%02x ", output[i]);
//	printf("\n");
//
//	i = 0;
//
//
//	size_t ct = 0, trials = 20000000;
//	auto t1{ std::chrono::system_clock::now() };
//	while (ct < trials)
//	{
//		sm4_crypt_enc(rk, input, input);
//		ct++;
//	}
//
//	auto t2{ std::chrono::system_clock::now() };
//	double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() * 1e-9;
//	dump_bx(input, 16); dz;
//
//
//	printf("The encrpt times:%lld\n\n", trials);
//	printf("Total Time:%.9fs\n\n", duration);
//	printf("Latency:%.9fs\n\n", duration / double(trials));
//	printf("Thoughoutput:%.9f MB/s \n\n", 16 * double(trials) / duration / 1024 / 1024);
//}