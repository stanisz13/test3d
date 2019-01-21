#include "maths_FA.h"

float absFloat(float v)
{
    if (v < 0.0f)
    {
        return -v;
    }

    return v;
}

int absInt(int v)
{
    if (v < 0)
    {
        return -v;
    }

    return v;
}

double absDouble(double v)
{
    if (v < 0.0f)
    {
        return -v;
    }

    return v;
}

float minFloat(float v, float w)
{
    if (v - w < EPSILON)
    {
        return v;
    }

    return w;
}

float maxFloat(float v, float w)
{
    if (v - w > EPSILON)
    {
        return v;
    }

    return w;
}

double minDouble(double v, double w)
{
    if (v - w < EPSILON_DOUBLE)
    {
        return v;
    }

    return w;
}

double maxDouble(double v, double w)
{
    if (v - w > EPSILON_DOUBLE)
    {
        return v;
    }

    return w;
}

double maxDouble(double v, double w);

int maxInt(int v, int w)
{
    if (v > w)
    {
        return v;
    }

    return w;
}

int minInt(int v, int w)
{
    if (v < w)
    {
        return v;
    }

    return w;
}

unsigned minUnsigned(unsigned v, unsigned w)
{
    if (v < w)
    {
        return v;
    }

    return w;
}

unsigned maxUnsigned(unsigned v, unsigned w)
{
    if (v > w)
    {
        return v;
    }

    return w;
}

float degreesToRadians(float deg)
{
    return ((deg * PI) / 180.0f);
}

float radiansToDegrees(float rad)
{
    return((rad * 180.0f) / PI);
}

unsigned xorshift()
{
    unsigned x = randomSeries_FA.seed;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    randomSeries_FA.seed = x;

    return x;
}

void setRandomSeed(unsigned s)
{
    randomSeries_FA.seed = s;
}

void setRandomSeedTime()
{
    randomSeries_FA.seed = (unsigned)time(0);
}

float getRandomFloatZeroOne()
{
    float res;

    unsigned r = xorshift();
    
    res = (float)r / UINT_MAX;

    return res;    
}

double getRandomDoubleZeroOne()
{
    double res;

    unsigned r = xorshift();

    res = (double)r / UINT_MAX;

    return res;
}

unsigned getRandomUnsignedOnInterval(unsigned a, unsigned b)
{
    unsigned res = xorshift();

    res = res % (b - a + 1);
    res += a;

    return res;
}

float getRandomFloatOnInterval(float a, float b)
{
    float res = getRandomFloatZeroOne();

    res *= absFloat(b - a);
    res += minFloat(a, b);

    return res;
}

double getRandomDoubleOnInterval(double a, double b)
{
    double res = getRandomDoubleZeroOne();

    res *= absDouble(b - a);
    res += minDouble(a, b);

    return res;
}

int getRandomIntOnInterval(int a, int b)
{
    int res = xorshift();
    res = absInt(res);

    res = res % absInt(b - a + 1);
    res += minInt(a, b);

    return res;
}

FVec2 getRandomFVec2OnInterval(float a, float b)
{
    FVec2 res;

    res.x = getRandomFloatOnInterval(a, b);
    res.y = getRandomFloatOnInterval(a, b);

    return res;
}

DVec2 getRandomDVec2OnInterval(double a, double b)
{
    DVec2 res;

    res.x = getRandomDoubleOnInterval(a, b);
    res.y = getRandomDoubleOnInterval(a, b);

    return res;
}

UVec2 getRandomUVec2OnInterval(unsigned a, unsigned b)
{
    UVec2 res;

    res.x = getRandomUnsignedOnInterval(a, b);
    res.y = getRandomUnsignedOnInterval(a, b);

    return res;
}

IVec2 getRandomIVec2OnInterval(int a, int b)
{
    IVec2 res;

    res.x = getRandomIntOnInterval(a, b);
    res.y = getRandomIntOnInterval(a, b);

    return res;
}

FVec3 getRandomFVec3OnInterval(float a, float b)
{
    FVec3 res;

    res.x = getRandomFloatOnInterval(a, b);
    res.y = getRandomFloatOnInterval(a, b);
    res.z = getRandomFloatOnInterval(a, b);
    
    return res;
}

DVec3 getRandomDVec3OnInterval(double a, double b)
{
    DVec3 res;

    res.x = getRandomDoubleOnInterval(a, b);
    res.y = getRandomDoubleOnInterval(a, b);
    res.z = getRandomDoubleOnInterval(a, b);
    
    return res;
}

UVec3 getRandomUVec3OnInterval(unsigned a, unsigned b)
{
    UVec3 res;

    res.x = getRandomUnsignedOnInterval(a, b);
    res.y = getRandomUnsignedOnInterval(a, b);
    res.z = getRandomUnsignedOnInterval(a, b);
    
    return res;
}

IVec3 getRandomIVec3OnInterval(int a, int b)
{
    IVec3 res;

    res.x = getRandomIntOnInterval(a, b);
    res.y = getRandomIntOnInterval(a, b);
    res.z = getRandomIntOnInterval(a, b);
    
    return res;
}

FVec4 getRandomFVec4OnInterval(float a, float b)
{
    FVec4 res;

    res.x = getRandomFloatOnInterval(a, b);
    res.y = getRandomFloatOnInterval(a, b);
    res.z = getRandomFloatOnInterval(a, b);
    res.w = getRandomFloatOnInterval(a, b);
    
    return res;
}

DVec4 getRandomDVec4OnInterval(double a, double b)
{
    DVec4 res;

    res.x = getRandomDoubleOnInterval(a, b);
    res.y = getRandomDoubleOnInterval(a, b);
    res.z = getRandomDoubleOnInterval(a, b);
    res.w = getRandomDoubleOnInterval(a, b);
    
    return res;
}

UVec4 getRandomUVec4OnInterval(unsigned a, unsigned b)
{
    UVec4 res;

    res.x = getRandomUnsignedOnInterval(a, b);
    res.y = getRandomUnsignedOnInterval(a, b);
    res.z = getRandomUnsignedOnInterval(a, b);
    res.w = getRandomUnsignedOnInterval(a, b);
    
    return res;
}

IVec4 getRandomIVec4OnInterval(int a, int b)
{
    IVec4 res;

    res.x = getRandomIntOnInterval(a, b);
    res.y = getRandomIntOnInterval(a, b);
    res.z = getRandomIntOnInterval(a, b);
    res.w = getRandomIntOnInterval(a, b);
    
    return res;
}

FVec2 scaleFVec2(FVec2 v, float s)
{
    FVec2 res = v;

    res.x *= s;
    res.y *= s;

    return res;
}

DVec2 scaleDVec2(DVec2 v, double s)
{
    DVec2 res = v;

    res.x *= s;
    res.y *= s;

    return res;
}

UVec2 scaleUVec2(UVec2 v, unsigned s)
{
    UVec2 res = v;

    res.x *= s;
    res.y *= s;

    return res;
}

IVec2 scaleIVec2(IVec2 v, int s)
{
    IVec2 res = v;

    res.x *= s;
    res.y *= s;

    return res;
}

FVec3 scaleFVec3(FVec3 v, float s)
{
    FVec3 res = v;

    res.x *= s;
    res.y *= s;
    res.z *= s;
    
    return res;
}

DVec3 scaleDVec3(DVec3 v, double s)
{
    DVec3 res = v;

    res.x *= s;
    res.y *= s;
    res.z *= s;
    
    return res;
}

UVec3 scaleUVec3(UVec3 v, unsigned s)
{
    UVec3 res = v;

    res.x *= s;
    res.y *= s;
    res.z *= s;
    
    return res;
}

IVec3 scaleIVec3(IVec3 v, int s)
{
    IVec3 res = v;

    res.x *= s;
    res.y *= s;
    res.z *= s;

    return res;
}

FVec4 scaleFVec4(FVec4 v, float s)
{
    FVec4 res = v;

    res.x *= s;
    res.y *= s;
    res.z *= s;
    res.w *= s;
    
    return res;
}

DVec4 scaleDVec4(DVec4 v, double s)
{
    DVec4 res = v;

    res.x *= s;
    res.y *= s;
    res.z *= s;
    res.w *= s;

    return res;
}

UVec4 scaleUVec4(UVec4 v, unsigned s)
{
    UVec4 res = v;

    res.x *= s;
    res.y *= s;
    res.z *= s;
    res.w *= s;

    return res;
}

IVec4 scaleIVec4(IVec4 v, int s)
{
    IVec4 res = v;

    res.x *= s;
    res.y *= s;
    res.z *= s;
    res.w *= s;

    return res;
}

FVec2 addFVec2(FVec2 v, FVec2 w)
{
    FVec2 res = v;
    res.x += w.x;
    res.y += w.y;

    return res;
}

DVec2 addDVec2(DVec2 v, DVec2 w)
{
    DVec2 res = v;
    res.x += w.x;
    res.y += w.y;

    return res;
}

UVec2 addUVec2(UVec2 v, UVec2 w)
{
    UVec2 res = v;
    res.x += w.x;
    res.y += w.y;

    return res;
}

IVec2 addIVec2(IVec2 v, IVec2 w)
{
    IVec2 res = v;
    res.x += w.x;
    res.y += w.y;

    return res;
}

FVec3 addFVec3(FVec3 v, FVec3 w)
{
    FVec3 res = v;
    res.x += w.x;
    res.y += w.y;
    res.z += w.z;
    
    return res;
}
DVec3 addDVec3(DVec3 v, DVec3 w)
{
    DVec3 res = v;
    res.x += w.x;
    res.y += w.y;
    res.z += w.z;
    
    return res;
}

UVec3 addUVec3(UVec3 v, UVec3 w)
{
    UVec3 res = v;
    res.x += w.x;
    res.y += w.y;
    res.z += w.z;
    
    return res;
}

IVec3 addIVec3(IVec3 v, IVec3 w)
{
    IVec3 res = v;
    res.x += w.x;
    res.y += w.y;
    res.z += w.z;
    
    return res;
}

FVec4 addFVec4(FVec4 v, FVec4 w)
{
    FVec4 res = v;
    res.x += w.x;
    res.y += w.y;
    res.z += w.z;
    res.w += w.w;
    
    return res;
}

DVec4 addDVec4(DVec4 v, DVec4 w)
{
    DVec4 res = v;
    res.x += w.x;
    res.y += w.y;
    res.z += w.z;
    res.w += w.w;
    
    return res;

}

UVec4 addUVec4(UVec4 v, UVec4 w)
{
    UVec4 res = v;
    res.x += w.x;
    res.y += w.y;
    res.z += w.z;
    res.w += w.w;
    
    return res;

}

IVec4 addIVec4(IVec4 v, IVec4 w)
{
    IVec4 res = v;
    res.x += w.x;
    res.y += w.y;
    res.z += w.z;
    res.w += w.w;
    
    return res;
}

FVec2 hadamardFVec2(FVec2 v, FVec2 w)
{
    FVec2 res = v;
    res.x *= w.x;
    res.y *= w.y;

    return res;
}

DVec2 hadamardDVec2(DVec2 v, DVec2 w)
{
    DVec2 res = v;
    res.x *= w.x;
    res.y *= w.y;

    return res;
}

UVec2 hadamardUVec2(UVec2 v, UVec2 w)
{
    UVec2 res = v;
    res.x *= w.x;
    res.y *= w.y;

    return res;
}

IVec2 hadamardIVec2(IVec2 v, IVec2 w)
{
    IVec2 res = v;
    res.x *= w.x;
    res.y *= w.y;

    return res;
}

FVec3 hadamardFVec3(FVec3 v, FVec3 w)
{
    FVec3 res = v;
    res.x *= w.x;
    res.y *= w.y;
    res.z *= w.z;
    
    return res;
}

DVec3 hadamardDVec3(DVec3 v, DVec3 w)
{
    DVec3 res = v;
    res.x *= w.x;
    res.y *= w.y;
    res.z *= w.z;
    
    return res;
}

UVec3 hadamardUVec3(UVec3 v, UVec3 w)
{
    UVec3 res = v;
    res.x *= w.x;
    res.y *= w.y;
    res.z *= w.z;
    
    return res;
}

IVec3 hadamardIVec3(IVec3 v, IVec3 w)
{
    IVec3 res = v;
    res.x *= w.x;
    res.y *= w.y;
    res.z *= w.z;
    
    return res;
}

FVec4 hadamardFVec4(FVec4 v, FVec4 w)
{
    FVec4 res = v;
    res.x *= w.x;
    res.y *= w.y;
    res.z *= w.z;
    res.w *= w.w;
    
    return res;
}

DVec4 hadamardDVec4(DVec4 v, DVec4 w)
{
    DVec4 res = v;
    res.x *= w.x;
    res.y *= w.y;
    res.z *= w.z;
    res.w *= w.w;
    
    return res;
}

UVec4 hadamardUVec4(UVec4 v, UVec4 w)
{
    UVec4 res = v;
    res.x *= w.x;
    res.y *= w.y;
    res.z *= w.z;
    res.w *= w.w;
    
    return res;
}

IVec4 hadamardIVec4(IVec4 v, IVec4 w)
{
    IVec4 res = v;
    res.x *= w.x;
    res.y *= w.y;
    res.z *= w.z;
    res.w *= w.w;
    
    return res;
}

float dotProductFVec2(FVec2 v, FVec2 w)
{
    float res = 0;

    res += v.x * w.x;
    res += v.y * w.y;

    return res;
}

double dotProductDVec2(DVec2 v, DVec2 w)
{
    double res = 0;

    res += v.x * w.x;
    res += v.y * w.y;

    return res;
}

unsigned dotProductUVec2(UVec2 v, UVec2 w)
{
    unsigned res = 0;

    res += v.x * w.x;
    res += v.y * w.y;

    return res;
}

int dotProductIVec2(IVec2 v, IVec2 w)
{
    int res = 0;

    res += v.x * w.x;
    res += v.y * w.y;

    return res;
}

float dotProductFVec3(FVec3 v, FVec3 w)
{
    float res = 0;

    res += v.x * w.x;
    res += v.y * w.y;
    res += v.z * w.z;
    
    return res;
}

double dotProductDVec3(DVec3 v, DVec3 w)
{
    double res = 0;

    res += v.x * w.x;
    res += v.y * w.y;
    res += v.z * w.z;
    
    return res;
}

unsigned dotProductUVec3(UVec3 v, UVec3 w)
{
    unsigned res = 0;

    res += v.x * w.x;
    res += v.y * w.y;
    res += v.z * w.z;
    
    return res;
}

int dotProductIVec3(IVec3 v, IVec3 w)
{
    int res = 0;

    res += v.x * w.x;
    res += v.y * w.y;
    res += v.z * w.z;
    
    return res;
}

float dotProductFVec4(FVec4 v, FVec4 w)
{
    float res = 0;

    res += v.x * w.x;
    res += v.y * w.y;
    res += v.z * w.z;
    res += v.w * w.w;
    
    return res;
}

double dotProductDVec4(DVec4 v, DVec4 w)
{
    double res = 0;

    res += v.x * w.x;
    res += v.y * w.y;
    res += v.z * w.z;
    res += v.w * w.w;
    
    return res;
}

unsigned dotProductUVec4(UVec4 v, UVec4 w)
{
    unsigned res = 0;

    res += v.x * w.x;
    res += v.y * w.y;
    res += v.z * w.z;
    res += v.w * w.w;
    
    return res;
}

int dotProductIVec4(IVec4 v, IVec4 w)
{
    int res = 0;

    res += v.x * w.x;
    res += v.y * w.y;
    res += v.z * w.z;
    res += v.w * w.w;
    
    return res;    
}

FVec3 crossProductFVec3(FVec3 a, FVec3 b)
{
    FVec3 res;

    res.x = a.y * b.z - a.z * b.y;
    res.y = a.x * b.z - a.z * b.x;
    res.z = a.x * b.y - a.y * b.x;
    
    return res;
}

float lengthSquaredFVec2(FVec2 v)
{
    float res = v.x * v.x
        + v.y * v.y; 
    
    return res;
}

double lengthSquaredDVec2(DVec2 v)
{
    double res = v.x * v.x
        + v.y * v.y; 
    
    return res;
}
unsigned lengthSquaredUVec2(UVec2 v)
{
    unsigned res = v.x * v.x
        + v.y * v.y; 
    
    return res;
}

unsigned lengthSquaredIVec2(IVec2 v)
{
    unsigned res = v.x * v.x
        + v.y * v.y; 
    
    return res;
}

float lengthSquaredFVec3(FVec3 v)
{
    float res = v.x * v.x
        + v.y * v.y
        + v.z * v.z;

    return res;
}

double lengthSquaredDVec3(DVec3 v)
{
    double res = v.x * v.x
        + v.y * v.y
        + v.z * v.z;

    return res;
}

unsigned lengthSquaredUVec3(UVec3 v)
{
    unsigned res = v.x * v.x
        + v.y * v.y
        + v.z * v.z;

    return res;
}

unsigned lengthSquaredIVec3(IVec3 v)
{
    unsigned res = v.x * v.x
        + v.y * v.y
        + v.z * v.z;

    return res;
}

float lengthSquaredFVec4(FVec4 v)
{
    float res = v.x * v.x
        + v.y * v.y
        + v.z * v.z
        + v.w * v.w;

    return res;
}

double lengthSquaredDVec4(DVec4 v)
{
    double res = v.x * v.x
        + v.y * v.y
        + v.z * v.z
        + v.w * v.w;

    return res;
}

unsigned lengthSquaredUVec4(UVec4 v)
{
    unsigned res = v.x * v.x
        + v.y * v.y
        + v.z * v.z
        + v.w * v.w;

    return res;
}

unsigned lengthSquaredIVec4(IVec4 v)
{
    unsigned res = v.x * v.x
        + v.y * v.y
        + v.z * v.z
        + v.w * v.w;

    return res;
}        

float lengthFVec2(FVec2 v)
{
    float res = sqrt((float)lengthSquaredFVec2(v));

    return res;
}

double lengthDVec2(DVec2 v)
{
    double res = sqrt((double)lengthSquaredDVec2(v));

    return res;
}

float lengthUVec2(UVec2 v)
{
    float res = sqrt((float)lengthSquaredUVec2(v));

    return res;
}

float lengthIVec2(IVec2 v)
{
    float res = sqrt((float)lengthSquaredIVec2(v));

    return res;
}

float lengthFVec3(FVec3 v)
{
    float res = sqrt((float)lengthSquaredFVec3(v));

    return res;
}    

double lengthDVec3(DVec3 v)
{
    double res = sqrt((double)lengthSquaredDVec3(v));

    return res;
}

float lengthUVec3(UVec3 v)
{
    float res = sqrt((float)lengthSquaredUVec3(v));

    return res;
}

float lengthIVec3(IVec3 v)
{
    float res = sqrt((float)lengthSquaredIVec3(v));

    return res;
}

float lengthFVec4(FVec4 v)
{
    float res = sqrt((float)lengthSquaredFVec4(v));

    return res;
}

double lengthDVec4(DVec4 v)
{
    double res = sqrt((double)lengthSquaredDVec4(v));

    return res;
}

float lengthUVec4(UVec4 v)
{
    float res = sqrt((float)lengthSquaredUVec4(v));

    return res;
}

float lengthIVec4(IVec4 v)
{
    float res = sqrt((float)lengthSquaredIVec4(v));

    return res;
}

FVec2 normalizeFVec2(FVec2 v)
{
    FVec2 res;

    res = scaleFVec2(v, 1.0f / lengthFVec2(v));

    return res;
}

DVec2 normalizeDVec2(DVec2 v)
{
    DVec2 res;

    res = scaleDVec2(v, 1.0f / lengthDVec2(v));

    return res;
}

UVec2 normalizeUVec2(UVec2 v)
{
    UVec2 res;

    res = scaleUVec2(v, 1.0f / lengthUVec2(v));

    return res;
}

IVec2 normalizeIVec2(IVec2 v)
{
    IVec2 res;

    res = scaleIVec2(v, 1.0f / lengthIVec2(v));

    return res;
}

FVec3 normalizeFVec3(FVec3 v)
{
    FVec3 res;

    res = scaleFVec3(v, 1.0f / lengthFVec3(v));

    return res;
}

DVec3 normalizeDVec3(DVec3 v)
{
    DVec3 res;

    res = scaleDVec3(v, 1.0f / lengthDVec3(v));

    return res;
}

UVec3 normalizeUVec3(UVec3 v)
{
    UVec3 res;

    res = scaleUVec3(v, 1.0f / lengthUVec3(v));

    return res;
}

IVec3 normalizeIVec3(IVec3 v)
{
    IVec3 res;

    res = scaleIVec3(v, 1.0f / lengthIVec3(v));

    return res;
}

FVec4 normalizeFVec4(FVec4 v)
{
    FVec4 res;

    res = scaleFVec4(v, 1.0f / lengthFVec4(v));

    return res;
}

DVec4 normalizeDVec4(DVec4 v)
{
    DVec4 res;

    res = scaleDVec4(v, 1.0f / lengthDVec4(v));

    return res;
}

UVec4 normalizeUVec4(UVec4 v)
{
    UVec4 res;

    res = scaleUVec4(v, 1.0f / lengthUVec4(v));

    return res;
}

IVec4 normalizeIVec4(IVec4 v)
{
    IVec4 res;

    res = scaleIVec4(v, 1.0f / lengthIVec4(v));

    return res;
}

FVec2 initFVec2(float a, float b)
{
    FVec2 res;

    res.x = a;
    res.y = b;

    return res;
}

FVec3 initFVec3(float a, float b, float c)
{
    FVec3 res;

    res.x = a;
    res.y = b;
    res.z = c;
    
    return res;
}

FVec4 initFVec4(float a, float b, float c, float d)
{
    FVec4 res;

    res.x = a;
    res.y = b;
    res.z = c;
    res.w = d;
    
    return res;
}

FMat2 addFMat2(FMat2 a, FMat2 b)
{
    FMat2 res = a;

    for (unsigned i = 0; i < 2; ++i)
    {
        for (unsigned j = 0; j < 2; ++j)
        {
            float* resAcc = accessFMat2(&res, i, j);
            float* bAcc = accessFMat2(&b, i, j);
            
            *resAcc += *bAcc;
        }
    }

    return res;
}

FMat3 addFMat3(FMat3 a, FMat3 b)
{
    FMat3 res = a;

    for (unsigned i = 0; i < 3; ++i)
    {
        for (unsigned j = 0; j < 3; ++j)
        {
            float* resAcc = accessFMat3(&res, i, j);
            float* bAcc = accessFMat3(&b, i, j);
            
            *resAcc += *bAcc;
        }
    }

    return res;
}

FMat4 addFMat4(FMat4 a, FMat4 b)
{
    FMat4 res = a;
    
    for (unsigned i = 0; i < 4; ++i)
    {
        for (unsigned j = 0; j < 4; ++j)
        {
            float* resAcc = accessFMat4(&res, i, j);
            float* bAcc = accessFMat4(&b, i, j);
            
            *resAcc += *bAcc;
        }
    }

    return res;
}

FMat2 scaleFMat2(FMat2 a, float b)
{
    FMat2 res = a;

    for (unsigned i = 0; i < 2; ++i)
    {
        for (unsigned j = 0; j < 2; ++j)
        {
            float* resAcc = accessFMat2(&res, i, j);

            *resAcc *= b;
        }
    }

    return res;    
}

FMat3 scaleFMat3(FMat3 a, float b)
{
    FMat3 res = a;

    for (unsigned i = 0; i < 3; ++i)
    {
        for (unsigned j = 0; j < 3; ++j)
        {
            float* resAcc = accessFMat3(&res, i, j);

            *resAcc *= b;
        }
    }

    return res;    
}

FMat4 scaleFMat4(FMat4 a, float b)
{
    FMat4 res = a;

    for (unsigned i = 0; i < 4; ++i)
    {
        for (unsigned j = 0; j < 4; ++j)
        {
            float* resAcc = accessFMat4(&res, i, j);

            *resAcc *= b;
        }
    }

    return res;    
}

FMat2 identityFMat2()
{
    FMat2 res = {};

    for (unsigned i = 0; i < 2; ++i)
    {
        float* resAcc = accessFMat2(&res, i, i);
        *resAcc = 1.0f;
    }
    
    return res;
}

FMat3 identityFMat3()
{
    FMat3 res = {};

    for (unsigned i = 0; i < 3; ++i)
    {
        float* resAcc = accessFMat3(&res, i, i);
        *resAcc = 1.0f;
    }
    
    return res;
}

FMat4 identityFMat4()
{
    FMat4 res = {};

    for (unsigned i = 0; i < 4; ++i)
    {
        float* resAcc = accessFMat4(&res, i, i);
        *resAcc = 1.0f;
    }
    
    return res;
}

float* accessFMat2(FMat2* m, unsigned x, unsigned y)
{
    float* res = 0;

    res = m->mem + x * 2 + y;
    
    return res;
}

float* accessFMat3(FMat3* m, unsigned x, unsigned y)
{
    float* res = 0;

    res = m->mem + x * 3 + y;

    return res;
}

float* accessFMat4(FMat4* m, unsigned x, unsigned y)
{
    float* res= 0;

    res = m->mem + x * 4 + y;

    return res;
}

FMat2 setFMat2ByElements(const float* data)
{
    FMat2 res;

    memcpy(res.mem, data, sizeof(res.mem));
    
    return res;
}

FMat3 setFMat3ByElements(const float* data)
{
    FMat3 res;

    memcpy(res.mem, data, sizeof(res.mem));
    
    return res;
}

FMat4 setFMat4ByElements(const float* data)
{
    FMat4 res;

    memcpy(res.mem, data, sizeof(res.mem));
    
    return res;
}

FMat2 setFMat2ByVectors(FVec2 a, FVec2 b)
{
    FMat2 res;

    res.col1 = a;
    res.col2 = b;
    
    return res;
}

FMat3 setFMat3ByVectors(FVec3 a, FVec3 b, FVec3 c)
{
    FMat3 res;

    res.col1 = a;
    res.col2 = b;
    res.col3 = c;

    return res;
}

FMat4 setFMat4ByVectors(FVec4 a, FVec4 b, FVec4 c, FVec4 d)
{
    FMat4 res;

    res.col1 = a;
    res.col2 = b;
    res.col3 = c;
    res.col4 = d;
    
    return res;
}

FMat2 mulFMat2(FMat2 a, FMat2 b)
{
    FMat2 res = {};

    for (unsigned i = 0; i < 2; ++i)
    {
        for (unsigned j = 0; j < 2; ++j)
        {
            float* curResElem = accessFMat2(&res, i, j);
            for (unsigned k = 0; k < 2; ++k)
            {
                float* curAElem = accessFMat2(&a, k, j);
                float* curBElem = accessFMat2(&b, i, k);
                *curResElem += *curAElem * *curBElem;
            }
        }
    }
    return res;
}

FMat3 mulFMat3(FMat3 a, FMat3 b)
{
    FMat3 res = {};

    for (unsigned i = 0; i < 3; ++i)
    {
        for (unsigned j = 0; j < 3; ++j)
        {
            float* curResElem = accessFMat3(&res, i, j);

            for (unsigned k = 0; k < 3; ++k)
            {
                float* curAElem = accessFMat3(&a, k, j);
                float* curBElem = accessFMat3(&b, i, k);
                
                *curResElem += *curAElem * *curBElem;
            }
        }
    }
    return res;
}

FMat4 mulFMat4(FMat4 a, FMat4 b)
{
    FMat4 res = {};

    for (unsigned i = 0; i < 4; ++i)
    {
        for (unsigned j = 0; j < 4; ++j)
        {
            float* curResElem = accessFMat4(&res, i, j);

            for (unsigned k = 0; k < 4; ++k)
            {
                float* curAElem = accessFMat4(&a, k, j);
                float* curBElem = accessFMat4(&b, i, k);
                
                *curResElem += *curAElem * *curBElem;
            }
        }
    }
    return res;
}

FVec2 mulFMat2ByFVec2(FMat2 a, FVec2 b)
{
    FVec2 res = {};

    for (unsigned i = 0; i < 2; ++i)
    {
        float* resAcc = &res.mem[i];

        for (unsigned k = 0; k < 2 ; ++k)
        {
            float* matAcc = accessFMat2(&a, k, i);
            
            *resAcc += b.mem[k] * *matAcc;
        }
    }
    
    return res;
}

FVec3 mulFMat3ByFVec3(FMat3 a, FVec3 b)
{
    FVec3 res = {};

    for (unsigned i = 0; i < 3; ++i)
    {
        float* resAcc = &res.mem[i];

        for (unsigned k = 0; k < 3 ; ++k)
        {
            float* matAcc = accessFMat3(&a, k, i);
            
            *resAcc += b.mem[k] * *matAcc;
        }
    }
    
    return res;    
}

FVec4 mulFMat4ByFVec4(FMat4 a, FVec4 b)
{
    FVec4 res = {};

    for (unsigned i = 0; i < 4; ++i)
    {
        float* resAcc = &res.mem[i];

        for (unsigned k = 0; k < 4 ; ++k)
        {
            float* matAcc = accessFMat4(&a, k, i);
            
            *resAcc += b.mem[k] * *matAcc;
        }
    }
    
    return res;
}

FMat4 perspectiveFMat4(float near, float far,
                      float aRatio, float FOVradians)
{
    FMat4 res = {};

    float top = tan(FOVradians / 2.0f) * near;
    float bottom = -1.0f * top;
    float right = top * aRatio;
    float left = - right;
    
    res.col1.x = (2.0f * near) / (right - left);

    res.col2.y = (2.0f * near) / (top - bottom);

    res.col3.x = (right + left) / (right - left);
    res.col3.y = (top + bottom) / (top - bottom);
    res.col3.z = (-far - near) / (far - near);
    res.col3.w = -1.0f;

    res.col4.z = (-2.0f * far * near) / (far - near);
    
    return res;
}

FMat4 rotationFMat4(float angleRadians, FVec3 rotationAxis)
{
    FMat4 res;

    float lSq = lengthSquaredFVec3(rotationAxis);
    float diff = lSq - 1.0f; 

    if (diff > EPSILON)
    {
        rotationAxis = normalizeFVec3(rotationAxis);
    }
        
    float s = sin(angleRadians);
    float c = cos(angleRadians);

    float oneMinusC = 1.0f - c;
    
    res.col1.x = c + rotationAxis.x * rotationAxis.x * oneMinusC;
    res.col1.y = rotationAxis.x * rotationAxis.y * oneMinusC + rotationAxis.z * s;
    res.col1.z = rotationAxis.z * rotationAxis.x * oneMinusC - rotationAxis.y * s;
    res.col1.w = 0.0f;

    res.col2.x = rotationAxis.x * rotationAxis.y * oneMinusC - rotationAxis.z * s;
    res.col2.y = c + rotationAxis.y * rotationAxis.y * oneMinusC;
    res.col2.z = rotationAxis.z * rotationAxis.y * oneMinusC + rotationAxis.x * s;
    res.col2.w = 0.0f;

    res.col3.x = rotationAxis.x * rotationAxis.z * oneMinusC + rotationAxis.y * s;
    res.col3.y = rotationAxis.y * rotationAxis.z * oneMinusC - rotationAxis.x * s;
    res.col3.z = c + rotationAxis.z * rotationAxis.z * oneMinusC;
    res.col3.w = 0.0f;
    
    res.col4.x = 0.0f;
    res.col4.y = 0.0f;
    res.col4.z = 0.0f;
    res.col4.w = 1.0f;
   
    return res;
}

FMat4 translationFMat4(FVec3 v)
{
    FMat4 res = identityFMat4();

    res.col4.x = v.x;
    res.col4.y = v.y;
    res.col4.z = v.z;
    
    return res;
}

FMat4 scalingFMat4(FVec3 v)
{
    FMat4 res = {};

    res.col1.x = v.x;
    res.col2.y = v.y;
    res.col3.z = v.z;
    res.col4.w = 1.0f;
    
    return res;
}

FMat4 lookAt()
{
    FMat4 res;

    FVec3 absoluteUp = initFVec3(0.0f, 1.0f, 0.0f);
    
    FVec3 dir = normalizeFVec3(addFVec3(camera_FA.pos, scaleFVec3(camera_FA.target, -1.0f)));
    FVec3 right = normalizeFVec3(crossProductFVec3(absoluteUp, dir));
    FVec3 up = crossProductFVec3(dir, right);
    
    res.col1.x = right.x;
    res.col1.y = up.x;
    res.col1.z = dir.x;
    res.col1.w = 0.0f;

    res.col2.x = right.y;
    res.col2.y = up.y;
    res.col2.z = dir.y;
    res.col2.w = 0.0f;

    res.col3.x = right.z;
    res.col3.y = up.z;
    res.col3.z = dir.z;
    res.col3.w = 0.0f;

    res.col4.x = -camera_FA.pos.x * right.x - camera_FA.pos.y * right.y - camera_FA.pos.z * right.z;
    res.col4.y = -camera_FA.pos.x * up.x - camera_FA.pos.y * up.y - camera_FA.pos.z * up.z;
    res.col4.z = -camera_FA.pos.x * dir.x - camera_FA.pos.y * dir.y - camera_FA.pos.z * dir.z;
    res.col4.w = 1.0f;
    
    return res;
}
