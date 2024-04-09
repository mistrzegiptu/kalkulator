#include "mathTrig.h"
#include <stdio.h>

const float PInum = 3.14159265359;
const float epsilon = 0.001;

float sine(float x)
{
    x = FMod(x, PInum*2);
    unsigned long long factorial = 1;
    int i = 1, sign = 1, resultSign = 1;
    float prev = -1, result = 0;
    if(x > PInum)
    {
        resultSign = -1;
        x -= PInum;
    }
    while(FAbs(result-prev) > epsilon)
    {
        prev = result;
        result += pow(x,i)/factorial * sign;
        i += 2;
        factorial *= i * (i-1);
        sign = -sign;
    }
    return result*resultSign;
}
float cosine(float x)
{
    return sin(x+PInum/2.0);
}
float tangent(float x)
{
    x = FMod(x, PInum);
    float cosVal = cosine(x);
    if(cosVal == 0)
        return INFINITY;
    if(x < PInum/2)
        return sine(x)/cosVal;
    else
        return -cotangent(x-PInum/2);
}
float cotangent(float x)
{
    x = FMod(x, PInum);
    float sineVal = sine(x);
    if(sineVal == 0)
        return INFINITY;
    if(x < PInum/2)
        return cosine(x)/sineVal;
    else
        return -tangent(x-PInum/2);
}
float asine(float x)
{
    if(x > 1 || x < -1)
        return NAN;
    return x + pow(x,3) / 6.0 + pow(x, 5)*3 / 40.0 + pow(x, 7)*5 / 112.0 + pow(x,9)*35 / 1152.0;
}
float acosine(float x)
{
    if(x > 1 || x < -1)
        return NAN;
    return PInum/2-asine(x);
}
float atangent(float x)
{
    if(x >= 1)
    {
        return PInum/2 - 1/x + 1/3 * pow(x, -3) - 1/5 * pow(x, -5) + 1/7 * pow(x, -7);
    }
    else if(x <= -1)
    {
        return -(PInum/2 - 1/x + 1/3 * pow(x, -3) - 1/5 * pow(x, -5) + 1/7 * pow(x, -7));
    }
    else
    {
        return x - pow(x, 3) / 3 + pow(x,5) / 5 - pow(x, 7) / 7;
    }
}
float acotangent(float x)
{
    return atangent(1/x);
}
float DegToRad(float x)
{
    return x / 180 * PInum;
}
float RadToDeg(float rad)
{
    return rad / PInum * 180;
}
float FMod(float x, float y)
{
    return x - (int)(x/y)*y;
}
float FAbs(float x)
{
    return x >= 0 ? x : -x;
}