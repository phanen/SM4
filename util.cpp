//
//
//
//// NOTE: THIS IMPLEMENTATION BASE ON LE ASSUMPRION
//// your machine should be LE
//
//#include "sm4.h"
//#include "lkup_table.h"
//
//
//static inline uint8_t sm4_Sbox_get(uint8_t inch)
//{
//	return SboxTable_1D[inch];
//}
//
//
////  T(.) := L(tau(.))
////  [in]: 32 bits unsigned value;
////  T(.) := L(tau(.))
////  [in]: 32 bits unsigned value;
//static inline uint32_t sm4_func_T(uint32_t in)
//{
//	uint32_t T3 = Table3[uint8_t(in >> 24)];
//	uint32_t T2 = Table2[uint8_t(in >> 16)];
//	uint32_t T1 = Table1[uint8_t(in >> 8)];
//	uint32_t T0 = Table0[uint8_t(in)];
//
//	return (T3 ^ T2) ^ (T1 ^ T0);
//}
//
//
//
//// Calculating round encryption key.
//// args:    [in] a: a is a 32 bits unsigned value;
//// return: sk[i]: i{0,1,2,3,...31}
//static inline uint32_t sm4_func_T_key(uint32_t in)
//{
//	uint32_t s_o = 0;
//
//	// non-linear func-tau
//	s_o |= sm4_Sbox_get(uint8_t(in >> 24)) << 24;
//	s_o |= sm4_Sbox_get(uint8_t(in >> 16)) << 16;
//	s_o |= sm4_Sbox_get(uint8_t(in >> 8)) << 8;
//	s_o |= sm4_Sbox_get(uint8_t(in));
//
//	// linear func-L'
//	return s_o ^ (ROT_SHL(s_o, 13)) ^ (ROT_SHL(s_o, 23));
//
//}
//
//
//// key <-- 4 * 32 = 128 
//static void sm4_gen_rk(uint32_t* rk, uint32_t* key)
//{
//	uint32_t k0, k1, k2, k3;
//
//	k0 = key[0] ^ FK[0];
//	k1 = key[1] ^ FK[1];
//	k2 = key[2] ^ FK[2];
//	k3 = key[3] ^ FK[3];
//
//
//	rk[0] = k0 ^ (sm4_func_T_key(k1 ^ k2 ^ k3 ^ CK[0]));
//	rk[1] = k1 ^ (sm4_func_T_key(k2 ^ k3 ^ rk[0] ^ CK[1]));
//	rk[2] = k2 ^ (sm4_func_T_key(k3 ^ rk[0] ^ rk[1] ^ CK[2]));
//	rk[3] = k3 ^ (sm4_func_T_key(rk[0] ^ rk[1] ^ rk[2] ^ CK[3]));
//	rk[4] = rk[0] ^ (sm4_func_T_key(rk[1] ^ rk[2] ^ rk[3] ^ CK[4]));
//	rk[5] = rk[1] ^ (sm4_func_T_key(rk[2] ^ rk[3] ^ rk[4] ^ CK[5]));
//	rk[6] = rk[2] ^ (sm4_func_T_key(rk[3] ^ rk[4] ^ rk[5] ^ CK[6]));
//	rk[7] = rk[3] ^ (sm4_func_T_key(rk[4] ^ rk[5] ^ rk[6] ^ CK[7]));
//	rk[8] = rk[4] ^ (sm4_func_T_key(rk[5] ^ rk[6] ^ rk[7] ^ CK[8]));
//	rk[9] = rk[5] ^ (sm4_func_T_key(rk[6] ^ rk[7] ^ rk[8] ^ CK[9]));
//	rk[10] = rk[6] ^ (sm4_func_T_key(rk[7] ^ rk[8] ^ rk[9] ^ CK[10]));
//	rk[11] = rk[7] ^ (sm4_func_T_key(rk[8] ^ rk[9] ^ rk[10] ^ CK[11]));
//	rk[12] = rk[8] ^ (sm4_func_T_key(rk[9] ^ rk[10] ^ rk[11] ^ CK[12]));
//	rk[13] = rk[9] ^ (sm4_func_T_key(rk[10] ^ rk[11] ^ rk[12] ^ CK[13]));
//	rk[14] = rk[10] ^ (sm4_func_T_key(rk[11] ^ rk[12] ^ rk[13] ^ CK[14]));
//	rk[15] = rk[11] ^ (sm4_func_T_key(rk[12] ^ rk[13] ^ rk[14] ^ CK[15]));
//	rk[16] = rk[12] ^ (sm4_func_T_key(rk[13] ^ rk[14] ^ rk[15] ^ CK[16]));
//	rk[17] = rk[13] ^ (sm4_func_T_key(rk[14] ^ rk[15] ^ rk[16] ^ CK[17]));
//	rk[18] = rk[14] ^ (sm4_func_T_key(rk[15] ^ rk[16] ^ rk[17] ^ CK[18]));
//	rk[19] = rk[15] ^ (sm4_func_T_key(rk[16] ^ rk[17] ^ rk[18] ^ CK[19]));
//	rk[20] = rk[16] ^ (sm4_func_T_key(rk[17] ^ rk[18] ^ rk[19] ^ CK[20]));
//	rk[21] = rk[17] ^ (sm4_func_T_key(rk[18] ^ rk[19] ^ rk[20] ^ CK[21]));
//	rk[22] = rk[18] ^ (sm4_func_T_key(rk[19] ^ rk[20] ^ rk[21] ^ CK[22]));
//	rk[23] = rk[19] ^ (sm4_func_T_key(rk[20] ^ rk[21] ^ rk[22] ^ CK[23]));
//	rk[24] = rk[20] ^ (sm4_func_T_key(rk[21] ^ rk[22] ^ rk[23] ^ CK[24]));
//	rk[25] = rk[21] ^ (sm4_func_T_key(rk[22] ^ rk[23] ^ rk[24] ^ CK[25]));
//	rk[26] = rk[22] ^ (sm4_func_T_key(rk[23] ^ rk[24] ^ rk[25] ^ CK[26]));
//	rk[27] = rk[23] ^ (sm4_func_T_key(rk[24] ^ rk[25] ^ rk[26] ^ CK[27]));
//	rk[28] = rk[24] ^ (sm4_func_T_key(rk[25] ^ rk[26] ^ rk[27] ^ CK[28]));
//	rk[29] = rk[25] ^ (sm4_func_T_key(rk[26] ^ rk[27] ^ rk[28] ^ CK[29]));
//	rk[30] = rk[26] ^ (sm4_func_T_key(rk[27] ^ rk[28] ^ rk[29] ^ CK[30]));
//	rk[31] = rk[27] ^ (sm4_func_T_key(rk[28] ^ rk[29] ^ rk[30] ^ CK[31]));
//
//	//for (uint32_t i = 0; i < 28; i++)
//	//{
//	//	rk[i + 4] = rk[i] ^ (sm4_func_T_key(rk[i + 1] ^ rk[i + 2] ^ rk[i + 3] ^ CK[i + 4]));
//	//}
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
//static void sm4_crypt_enc(const uint32_t rk[32], const uint32_t input[4], uint32_t output[4])
//{
//	uint32_t i = 0;
//	uint32_t x0, x1, x2, x3, x4;
//	x0 = input[0];
//	x1 = input[1];
//	x2 = input[2];
//	x3 = input[3];
//
//	x4 = x0 ^ sm4_func_T(x1 ^ x2 ^ x3 ^ rk[0]);
//	x0 = x1 ^ sm4_func_T(x2 ^ x3 ^ x4 ^ rk[1]);
//	x1 = x2 ^ sm4_func_T(x3 ^ x4 ^ x0 ^ rk[2]);
//	x2 = x3 ^ sm4_func_T(x4 ^ x0 ^ x1 ^ rk[3]);
//	x3 = x4 ^ sm4_func_T(x0 ^ x1 ^ x2 ^ rk[4]);
//	x4 = x0 ^ sm4_func_T(x1 ^ x2 ^ x3 ^ rk[5]);
//	x0 = x1 ^ sm4_func_T(x2 ^ x3 ^ x4 ^ rk[6]);
//	x1 = x2 ^ sm4_func_T(x3 ^ x4 ^ x0 ^ rk[7]);
//	x2 = x3 ^ sm4_func_T(x4 ^ x0 ^ x1 ^ rk[8]);
//	x3 = x4 ^ sm4_func_T(x0 ^ x1 ^ x2 ^ rk[9]);
//	x4 = x0 ^ sm4_func_T(x1 ^ x2 ^ x3 ^ rk[10]);
//	x0 = x1 ^ sm4_func_T(x2 ^ x3 ^ x4 ^ rk[11]);
//	x1 = x2 ^ sm4_func_T(x3 ^ x4 ^ x0 ^ rk[12]);
//	x2 = x3 ^ sm4_func_T(x4 ^ x0 ^ x1 ^ rk[13]);
//	x3 = x4 ^ sm4_func_T(x0 ^ x1 ^ x2 ^ rk[14]);
//	x4 = x0 ^ sm4_func_T(x1 ^ x2 ^ x3 ^ rk[15]);
//	x0 = x1 ^ sm4_func_T(x2 ^ x3 ^ x4 ^ rk[16]);
//	x1 = x2 ^ sm4_func_T(x3 ^ x4 ^ x0 ^ rk[17]);
//	x2 = x3 ^ sm4_func_T(x4 ^ x0 ^ x1 ^ rk[18]);
//	x3 = x4 ^ sm4_func_T(x0 ^ x1 ^ x2 ^ rk[19]);
//	x4 = x0 ^ sm4_func_T(x1 ^ x2 ^ x3 ^ rk[20]);
//	x0 = x1 ^ sm4_func_T(x2 ^ x3 ^ x4 ^ rk[21]);
//	x1 = x2 ^ sm4_func_T(x3 ^ x4 ^ x0 ^ rk[22]);
//	x2 = x3 ^ sm4_func_T(x4 ^ x0 ^ x1 ^ rk[23]);
//	x3 = x4 ^ sm4_func_T(x0 ^ x1 ^ x2 ^ rk[24]);
//	x4 = x0 ^ sm4_func_T(x1 ^ x2 ^ x3 ^ rk[25]);
//	x0 = x1 ^ sm4_func_T(x2 ^ x3 ^ x4 ^ rk[26]);
//	x1 = x2 ^ sm4_func_T(x3 ^ x4 ^ x0 ^ rk[27]);
//	x2 = x3 ^ sm4_func_T(x4 ^ x0 ^ x1 ^ rk[28]);
//	x3 = x4 ^ sm4_func_T(x0 ^ x1 ^ x2 ^ rk[29]);
//	x4 = x0 ^ sm4_func_T(x1 ^ x2 ^ x3 ^ rk[30]);
//	x0 = x1 ^ sm4_func_T(x2 ^ x3 ^ x4 ^ rk[31]);
//
//	output[0] = x0;
//	output[1] = x4;
//	output[2] = x3;
//	output[3] = x2;
//}
//
//
//int main(int argc, char** argv)
//{
//	uint32_t s_o = 0;
//
//
//	for (int in = 0; in < 0x100; in++)
//	{
//		if (in % 8 == 0) { dz; printf("    "); };
//		s_o = sm4_Sbox_get(uint8_t(in));
//
//		uint32_t val = s_o ^ (ROT_SHL(s_o, 13)) ^ (ROT_SHL(s_o, 23));
//		printf("0x%08x, ", val);
//	}
//
//}