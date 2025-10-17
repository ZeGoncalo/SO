#include "vector.h"
double vector_sprod(vector* a, vector* b){
	return a->x * b->x + a->y * b->y +a->z * b->z;
}
