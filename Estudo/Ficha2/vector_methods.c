#include "vector.h"
#include <stdlib.h>
#include <math.h>

vector* vector_new(double a, double b, double c){
	vector* v= (vector*) malloc(sizeof(vector));
	v->x=a;
	v->y=b;
	v->z=c;
	return v;
}

vector* vector_add(vector* a, vector* b){
	return vector_new(a->x+b->x,a->y+b->y,a->z+b->z);
}

vector* vector_sub(vector* a, vector* b){
	return vector_new(a->x-b->x,a->y-b->y,a->z-b->z);
}

vector* vector_scale(double a, vector* b){
	return vector_new(a*b->x,a*b->y,a*b->z);
}

vector* vector_vprod(vector* a, vector* b) {
    return vector_new(a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x);
}

double vector_sprod(vector* a, vector* b){
	return a->x*b->x+a->y*b->y+a->z*b->z;
}

double vector_mod(vector* a){
	return sqrt(a->x*a->x+a->y*a->y+a->z*a->z);
}
