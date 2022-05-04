/* ECB IMPLEMENTATION OF SM4*/

/*	OPTIMIZATION:
*	- multithread
*	- look up table
*	- SIMD (parallelly look up table)
*	- SIMD (better F func)
*/

// NOTE: THIS IMPLEMENTATION BASE ON LE ASSUMPRION
// your machine should be LE


#include <thread>
#include <vector>
#include <intrin.h>
#include "sm4.h"
#include "lkup_table.h"


static inline __m256i sm4_func_T(__m256i in)
{
	__m256i	mask = _mm256_set1_epi32(0xff);
	__m256i tmp3 = _mm256_and_si256(_mm256_srli_epi32(in, 24), mask);
	__m256i tmp2 = _mm256_and_si256(_mm256_srli_epi32(in, 16), mask);
	__m256i tmp1 = _mm256_and_si256(_mm256_srli_epi32(in, 8), mask);
	in = _mm256_and_si256(in, mask);

#ifdef _debug

	for (size_t i = 0; i < 8; i++)
	{
		printf("0x%08x\n", mask.m256i_i32[i]);
	}

	for (size_t i = 0; i < 8; i++)
	{
		printf("0x%08x\n", in.m256i_i32[i]);
	}
	for (size_t i = 0; i < 8; i++)
	{
		printf("0x%08x\n", tmp1.m256i_i32[i]);
	}
	for (size_t i = 0; i < 8; i++)
	{
		printf("0x%08x\n", tmp2.m256i_i32[i]);
	}
#endif

	tmp3 = _mm256_i32gather_epi32((int*)Table3, tmp3, 4);
	tmp2 = _mm256_i32gather_epi32((int*)Table2, tmp2, 4);
	tmp1 = _mm256_i32gather_epi32((int*)Table1, tmp1, 4);
	in = _mm256_i32gather_epi32((int*)Table0, in, 4);


	return 	_mm256_xor_si256(_mm256_xor_si256(in, tmp1), _mm256_xor_si256(tmp3, tmp2));
}


static inline uint32_t sm4_func_T_key(uint32_t in)
{
	uint8_t* p = (uint8_t*)&in;
	uint32_t T0 = T0_key[*(p)];
	uint32_t T1 = T1_key[*(p + 1)];
	uint32_t T2 = T2_key[*(p + 2)];
	uint32_t T3 = T3_key[*(p + 3)];
	return (T3 ^ T2) ^ (T1 ^ T0);
}

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
//}


// key <-- 4 * 32 = 128 
static void sm4_gen_rk(uint32_t* rk, uint32_t* key)
{
	uint32_t k0, k1, k2, k3;

	k0 = key[0] ^ FK[0];
	k1 = key[1] ^ FK[1];
	k2 = key[2] ^ FK[2];
	k3 = key[3] ^ FK[3];


	rk[0] = k0 ^ (sm4_func_T_key(k1 ^ k2 ^ k3 ^ CK[0]));
	rk[1] = k1 ^ (sm4_func_T_key(k2 ^ k3 ^ rk[0] ^ CK[1]));
	rk[2] = k2 ^ (sm4_func_T_key(k3 ^ rk[0] ^ rk[1] ^ CK[2]));
	rk[3] = k3 ^ (sm4_func_T_key(rk[0] ^ rk[1] ^ rk[2] ^ CK[3]));
	rk[4] = rk[0] ^ (sm4_func_T_key(rk[1] ^ rk[2] ^ rk[3] ^ CK[4]));
	rk[5] = rk[1] ^ (sm4_func_T_key(rk[2] ^ rk[3] ^ rk[4] ^ CK[5]));
	rk[6] = rk[2] ^ (sm4_func_T_key(rk[3] ^ rk[4] ^ rk[5] ^ CK[6]));
	rk[7] = rk[3] ^ (sm4_func_T_key(rk[4] ^ rk[5] ^ rk[6] ^ CK[7]));
	rk[8] = rk[4] ^ (sm4_func_T_key(rk[5] ^ rk[6] ^ rk[7] ^ CK[8]));
	rk[9] = rk[5] ^ (sm4_func_T_key(rk[6] ^ rk[7] ^ rk[8] ^ CK[9]));
	rk[10] = rk[6] ^ (sm4_func_T_key(rk[7] ^ rk[8] ^ rk[9] ^ CK[10]));
	rk[11] = rk[7] ^ (sm4_func_T_key(rk[8] ^ rk[9] ^ rk[10] ^ CK[11]));
	rk[12] = rk[8] ^ (sm4_func_T_key(rk[9] ^ rk[10] ^ rk[11] ^ CK[12]));
	rk[13] = rk[9] ^ (sm4_func_T_key(rk[10] ^ rk[11] ^ rk[12] ^ CK[13]));
	rk[14] = rk[10] ^ (sm4_func_T_key(rk[11] ^ rk[12] ^ rk[13] ^ CK[14]));
	rk[15] = rk[11] ^ (sm4_func_T_key(rk[12] ^ rk[13] ^ rk[14] ^ CK[15]));
	rk[16] = rk[12] ^ (sm4_func_T_key(rk[13] ^ rk[14] ^ rk[15] ^ CK[16]));
	rk[17] = rk[13] ^ (sm4_func_T_key(rk[14] ^ rk[15] ^ rk[16] ^ CK[17]));
	rk[18] = rk[14] ^ (sm4_func_T_key(rk[15] ^ rk[16] ^ rk[17] ^ CK[18]));
	rk[19] = rk[15] ^ (sm4_func_T_key(rk[16] ^ rk[17] ^ rk[18] ^ CK[19]));
	rk[20] = rk[16] ^ (sm4_func_T_key(rk[17] ^ rk[18] ^ rk[19] ^ CK[20]));
	rk[21] = rk[17] ^ (sm4_func_T_key(rk[18] ^ rk[19] ^ rk[20] ^ CK[21]));
	rk[22] = rk[18] ^ (sm4_func_T_key(rk[19] ^ rk[20] ^ rk[21] ^ CK[22]));
	rk[23] = rk[19] ^ (sm4_func_T_key(rk[20] ^ rk[21] ^ rk[22] ^ CK[23]));
	rk[24] = rk[20] ^ (sm4_func_T_key(rk[21] ^ rk[22] ^ rk[23] ^ CK[24]));
	rk[25] = rk[21] ^ (sm4_func_T_key(rk[22] ^ rk[23] ^ rk[24] ^ CK[25]));
	rk[26] = rk[22] ^ (sm4_func_T_key(rk[23] ^ rk[24] ^ rk[25] ^ CK[26]));
	rk[27] = rk[23] ^ (sm4_func_T_key(rk[24] ^ rk[25] ^ rk[26] ^ CK[27]));
	rk[28] = rk[24] ^ (sm4_func_T_key(rk[25] ^ rk[26] ^ rk[27] ^ CK[28]));
	rk[29] = rk[25] ^ (sm4_func_T_key(rk[26] ^ rk[27] ^ rk[28] ^ CK[29]));
	rk[30] = rk[26] ^ (sm4_func_T_key(rk[27] ^ rk[28] ^ rk[29] ^ CK[30]));
	rk[31] = rk[27] ^ (sm4_func_T_key(rk[28] ^ rk[29] ^ rk[30] ^ CK[31]));

	//for (uint32_t i = 0; i < 28; i++)
	//{
	//	rk[i + 4] = rk[i] ^ (sm4_func_T_key(rk[i + 1] ^ rk[i + 2] ^ rk[i + 3] ^ CK[i + 4]));
	//}

#ifdef _debug
	for (size_t i = 0; i < 32; i++)
	{
		printf("rk[%lld] = %02x\n", i, rk[i]);
	}
#endif
}





static inline void sm4_crypt_enc(const uint32_t rk[32], const uint32_t input[32], uint32_t output[32])
{

	// load 8 plains
	// each plain can be divided into 4 segs 
	// each reg has been loaded with same seg in each plain

	__m256i vindex = _mm256_setr_epi32(0, 4, 8, 12, 16, 20, 24, 28);
	__m256i x0, x1, x2, x3, x4;
	x3 = _mm256_i32gather_epi32((int*)(input + 3), vindex, 4);
	x2 = _mm256_i32gather_epi32((int*)(input + 2), vindex, 4);
	x1 = _mm256_i32gather_epi32((int*)(input + 1), vindex, 4);
	x0 = _mm256_i32gather_epi32((int*)(input + 0), vindex, 4);



	x4 = _mm256_xor_si256(x0, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x1, x2), _mm256_xor_si256(x3, _mm256_set1_epi32(rk[0])))));
	x0 = _mm256_xor_si256(x1, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x2, x3), _mm256_xor_si256(x4, _mm256_set1_epi32(rk[1])))));
	x1 = _mm256_xor_si256(x2, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x3, x4), _mm256_xor_si256(x0, _mm256_set1_epi32(rk[2])))));
	x2 = _mm256_xor_si256(x3, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x4, x0), _mm256_xor_si256(x1, _mm256_set1_epi32(rk[3])))));
	x3 = _mm256_xor_si256(x4, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x0, x1), _mm256_xor_si256(x2, _mm256_set1_epi32(rk[4])))));
	x4 = _mm256_xor_si256(x0, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x1, x2), _mm256_xor_si256(x3, _mm256_set1_epi32(rk[5])))));
	x0 = _mm256_xor_si256(x1, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x2, x3), _mm256_xor_si256(x4, _mm256_set1_epi32(rk[6])))));
	x1 = _mm256_xor_si256(x2, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x3, x4), _mm256_xor_si256(x0, _mm256_set1_epi32(rk[7])))));
	x2 = _mm256_xor_si256(x3, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x4, x0), _mm256_xor_si256(x1, _mm256_set1_epi32(rk[8])))));
	x3 = _mm256_xor_si256(x4, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x0, x1), _mm256_xor_si256(x2, _mm256_set1_epi32(rk[9])))));
	x4 = _mm256_xor_si256(x0, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x1, x2), _mm256_xor_si256(x3, _mm256_set1_epi32(rk[10])))));
	x0 = _mm256_xor_si256(x1, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x2, x3), _mm256_xor_si256(x4, _mm256_set1_epi32(rk[11])))));
	x1 = _mm256_xor_si256(x2, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x3, x4), _mm256_xor_si256(x0, _mm256_set1_epi32(rk[12])))));
	x2 = _mm256_xor_si256(x3, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x4, x0), _mm256_xor_si256(x1, _mm256_set1_epi32(rk[13])))));
	x3 = _mm256_xor_si256(x4, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x0, x1), _mm256_xor_si256(x2, _mm256_set1_epi32(rk[14])))));
	x4 = _mm256_xor_si256(x0, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x1, x2), _mm256_xor_si256(x3, _mm256_set1_epi32(rk[15])))));
	x0 = _mm256_xor_si256(x1, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x2, x3), _mm256_xor_si256(x4, _mm256_set1_epi32(rk[16])))));
	x1 = _mm256_xor_si256(x2, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x3, x4), _mm256_xor_si256(x0, _mm256_set1_epi32(rk[17])))));
	x2 = _mm256_xor_si256(x3, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x4, x0), _mm256_xor_si256(x1, _mm256_set1_epi32(rk[18])))));
	x3 = _mm256_xor_si256(x4, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x0, x1), _mm256_xor_si256(x2, _mm256_set1_epi32(rk[19])))));
	x4 = _mm256_xor_si256(x0, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x1, x2), _mm256_xor_si256(x3, _mm256_set1_epi32(rk[20])))));
	x0 = _mm256_xor_si256(x1, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x2, x3), _mm256_xor_si256(x4, _mm256_set1_epi32(rk[21])))));
	x1 = _mm256_xor_si256(x2, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x3, x4), _mm256_xor_si256(x0, _mm256_set1_epi32(rk[22])))));
	x2 = _mm256_xor_si256(x3, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x4, x0), _mm256_xor_si256(x1, _mm256_set1_epi32(rk[23])))));
	x3 = _mm256_xor_si256(x4, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x0, x1), _mm256_xor_si256(x2, _mm256_set1_epi32(rk[24])))));
	x4 = _mm256_xor_si256(x0, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x1, x2), _mm256_xor_si256(x3, _mm256_set1_epi32(rk[25])))));
	x0 = _mm256_xor_si256(x1, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x2, x3), _mm256_xor_si256(x4, _mm256_set1_epi32(rk[26])))));
	x1 = _mm256_xor_si256(x2, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x3, x4), _mm256_xor_si256(x0, _mm256_set1_epi32(rk[27])))));
	x2 = _mm256_xor_si256(x3, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x4, x0), _mm256_xor_si256(x1, _mm256_set1_epi32(rk[28])))));
	x3 = _mm256_xor_si256(x4, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x0, x1), _mm256_xor_si256(x2, _mm256_set1_epi32(rk[29])))));
	x4 = _mm256_xor_si256(x0, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x1, x2), _mm256_xor_si256(x3, _mm256_set1_epi32(rk[30])))));
	x0 = _mm256_xor_si256(x1, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x2, x3), _mm256_xor_si256(x4, _mm256_set1_epi32(rk[31])))));


	// x0
	// x4
	// x3
	// x2


	// now the vec is 
	//					a0 b0 ... h0
	//						  ...
	//					a3 b3 ... h3

	// the expected vec is
	//					a0 a1 a2 a3 b0 b1 b2 b3
	//						...


	_mm256_storeu_si256(((__m256i*)output) + 0, MM256_PACK0_EPI32(x0, x4, x3, x2));
	_mm256_storeu_si256(((__m256i*)output) + 1, MM256_PACK1_EPI32(x0, x4, x3, x2));
	_mm256_storeu_si256(((__m256i*)output) + 2, MM256_PACK2_EPI32(x0, x4, x3, x2));
	_mm256_storeu_si256(((__m256i*)output) + 3, MM256_PACK3_EPI32(x0, x4, x3, x2));

}





uint32_t key[4]{ 0x01234567,0x89abcdef,0xfedcba98,0x76543210 };
uint32_t rk[32]{};



constexpr size_t threads_num = 12;
constexpr size_t trials_per_thread = 1000000;
constexpr size_t trials = threads_num * trials_per_thread * 8;

// 4 elems make up a whole plain, 32 = 8 * 4
// so I reserve 8 plains

#define mutex
#ifdef mutex
uint32_t input[threads_num][32]{};
uint32_t output[threads_num][32]{};

#else
uint32_t input[32]{};
uint32_t output[32]{};
#endif


void task(int i) {
#ifdef mutex
	for (size_t ct = 0; ct < trials_per_thread; ct++)
	{
		sm4_crypt_enc(rk, input[i], output[i]);
	}
#else
	for (size_t ct = 0; ct < trials_per_thread; ct++)
	{
		sm4_crypt_enc(rk, input, input);
	}
#endif

}



int main(int argc, char** argv)
{


	uint32_t p0 = 0x01234567;
	uint32_t p1 = 0x89abcdef;
	uint32_t p2 = 0xfedcba98;
	uint32_t p3 = 0x76543210;

	for (size_t id = 0; id < threads_num; id++)
	{
		for (size_t i = 0; i < 32; )
		{
#ifdef mutex
			input[id][i++] = (p0++) + id;
			input[id][i++] = (p1++) + id;
			input[id][i++] = (p2++) + id;
			input[id][i++] = (p3++) + id;
#else
			input[i++] = 0x01234567;
			input[i++] = 0x89abcdef;
			input[i++] = 0xfedcba98;
			input[i++] = 0x76543210;
#endif
		}
	}


	sm4_gen_rk(rk, key);

	std::vector<std::thread> threads_lst;
	auto t1{ std::chrono::system_clock::now() };

	for (size_t i = 0; i < threads_num; i++)
	{
		threads_lst.emplace_back(
			std::thread(task, i)
		);
	}

	for (size_t i = 0; i < threads_num; i++)
	{
		threads_lst[i].join();
	}


	auto t2{ std::chrono::system_clock::now() };

	double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() * 1e-9;

	for (size_t j = 0; j < 8; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			printf("0x%08x ", output[0][j * 4 + i]);
		}
		dz;
	}

	printf("The encrpt times:%lld\n\n", trials);
	printf("Total Time:%.9fs\n\n", duration);
	printf("Latency:%.9fs\n\n", duration / double(trials));
	printf("Thoughoutput:%.9f MB/s \n\n", 16 * double(trials) / duration / 1024 / 1024);
}