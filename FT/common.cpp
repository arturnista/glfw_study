#include "common.h"

unsigned long hashNumber(long v) {
	unsigned long h = v;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

unsigned long hashVec3(glm::vec3 v) {
	unsigned long vl1 = hashNumber( floor( v.x ) );
	unsigned long vl2 = hashNumber( hashNumber( floor( v.y ) ) );
	unsigned long vl3 = hashNumber( hashNumber( hashNumber( floor( v.z ) ) ) );
	return hashNumber( vl1 + vl2 + vl3 );
}
