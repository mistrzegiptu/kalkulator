#include <stdio.h>
#include "mathTrig.h"


int main()
{
    for(int i = 0; i <= (int)(PInum*2*100); i += 10)
    {
        printf("sin %f --> %f\n", (i / 100.0), sine(i / 100.0));
        printf("cos %f --> %f\n",(i/100.0), cosine(i/100.0));
        printf("tan %f --> %f\n",(i/100.0), tangent(i/100.0));
        printf("cot %f --> %f\n",(i/100.0), cotangent(i/100.0));
        printf("asin %f --> %f\n", (i / 100.0), asin(i / 100.0));
        printf("acos %f --> %f\n",(i/100.0), acosine(i/100.0));
        printf("atan %f --> %f\n",(i/100.0), atangent(i/100.0));
        printf("acot %f --> %f\n",(i/100.0), acotangent(i/100.0));
    }
    return 0;
}
/*float arcsine(float value)
{
    return value+pow(value, 3)*0.1666666 + pow(value, 5)*0.075 + pow(value, 7) *0.04464285714;
}
float asine(float value)
{
    if(value==0.5)
        return 0.523598776;
    return atangent(value/sqrt(1-pow(value,2)));
}
float acosine(float value)
{
    return PInum/2-asine(value);
}
float arctangent(float value)
{
    if(value >= 1)
    {
        return PInum/2 - 1/value + 1/3 * pow(value, -3) - 1/5 * pow(value, -5) + 1/7 * pow(value, -7);
    }
    else if(value <= -1)
    {
        return -(PInum/2 - 1/value + 1/3 * pow(value, -3) - 1/5 * pow(value, -5) + 1/7 * pow(value, -7));
    }
    else
    {
        return value - pow(value, 3) / 3 + pow(value,5) / 5 - pow(value, 7) / 7;
    }
}
float atangent(float value)
{
    float x = value, xx;
    float a0=1.0,
    a1 = 0.33288950512027,
    a2 = -0.08467922817644,
    a3 = 0.03252232640125,
    a4 = -0.00749305860992;
    if(x>=1)
    {
        xx = pow(1/x,2);
        return PInum/2-(float)(1/x)/(float)polyNom(xx,a0,a1,a2,a3,a4);
    }
    else if(x<=-1)
    {
        xx = pow(1/x,2);
        return -(PInum/2-(float)(1/x))/(float)polyNom(xx,a0,a1,a2,a3,a4);
    }
    else
    {
        xx = pow(x,2);
        return x/(float)polyNom(xx,a0,a1,a2,a3,a4);
    }
}
float acotangent(float value)
{
    return atangent(1/value);
}
float polyNom(float x, float a0, float a1, float a2, float a3, float a4)
{
    return a0+(a1+(a2+(a3+a4*x)*x)*x)*x;
}*/