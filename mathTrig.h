#ifndef MATH_TRIG
#define MATH_TRIG
#include <math.h>

//SOME CONST FOR CALCULATING TRIGONOMETRIC FUNCTIONS
extern const float PInum;
extern const float epsilon;

//TRIGONOMETRIC FUNCTIONS THAT ARE CALCULATED WITH TAYLOR'S EXPANSION
float sine(float x);
float cosine(float x);
float tangent(float x);
float cotangent(float x);

//ARC FUNCTIONS THAT ARE ALSO CALCULATED WITH TAYLOR'S EXPANSION
float asine(float x);
float acosine(float x);
float atangent(float x);
float acotangent(float x);

float DegToRad(float x);
float RadToDeg(float rad);
float FMod(float x, float y);
float FAbs(float x);

#endif