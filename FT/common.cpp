#include "common.h"

tHash hashNumber(long v) {
	tHash h = v;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

tHash hashVec3(glm::vec3 v) {
	tHash vl1 = hashNumber( floor( v.x ) );
	tHash vl2 = hashNumber( hashNumber( floor( v.y ) ) );
	tHash vl3 = hashNumber( hashNumber( hashNumber( floor( v.z ) ) ) );
	return hashNumber( vl1 + vl2 + vl3 );
}
