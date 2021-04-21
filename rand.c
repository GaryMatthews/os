#include <rand.h>

uint32_t random_seed = 1;

uint32_t rand() {
	random_seed = random_seed * 1103515245 + 12345;
	return (uint32_t)(random_seed / 63336) % 4294967295;
}

uint32_t maxrand(uint32_t seed, uint32_t max) {
	random_seed = random_seed + seed * 1103515245 + 12345;
	return (uint32_t)(random_seed / 65536) % (max+1);
}
