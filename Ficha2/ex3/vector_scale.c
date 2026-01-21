#include "vector.h"
vector* vector_scale(double b, vector* a){
	return vector_new(a->x * b, a->y * b, a->z * b);
}
