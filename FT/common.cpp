#include "common.h"

float distance(glm::vec3 pos1, glm::vec3 pos2) {
	return sqrt(
		pow(pos1.y - pos2.y, 2) +
		pow(pos1.x - pos2.x, 2) +
		pow(pos1.z - pos2.z, 2)
	);
}

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
	tHash vl1 = hashNumber( round( v.x ) );
	tHash vl2 = hashNumber( hashNumber( round( v.y ) ) );
	tHash vl3 = hashNumber( hashNumber( hashNumber( round( v.z ) ) ) );
	return hashNumber( vl1 + vl2 + vl3 );
}
