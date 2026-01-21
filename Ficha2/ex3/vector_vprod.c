#include "vector.h"
vector* vector_vprod(vector* a, vector* b){
	return vector_new(a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x);
}