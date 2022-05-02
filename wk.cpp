//#include<bits\stdc++.h>
//
//using namespace std;
//
//int main() {
//	for (int i = 0; i < 32; i++)
//	{
//		//printf("rk[%lld] = rk[%lld]^ (sm4_func_T_key(rk[%lld] ^ rk[%lld] ^ rk[%lld] ^ CK[%lld]));\n", i + 4, i, i + 1, i + 2, i + 3, i + 4);
//		printf("x%d = _mm256_xor_si256(x%d, sm4_func_T(_mm256_xor_si256(_mm256_xor_si256(x%d, x%d), _mm256_xor_si256(x%d, _mm256_set1_epi32(rk[%d])))));\n", (i + 4) % 5, (i + 0) % 5, (i + 1) % 5, (i + 2) % 5, (i + 3) % 5, (i + 0) );
//
//	}
//}