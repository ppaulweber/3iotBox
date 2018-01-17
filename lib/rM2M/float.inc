/* Float arithmetic
 *
 * (c) Copyright 1999, Artran, Inc.
 * Written by Greg Garner (gmg@artran.com)
 * Modified in March 2001 to include user defined
 * operators for the floating point functions.
 * Highly modified in July 2012 to reflect a standard
 * C math implementation.
 *
 * This file is provided as is (no warranties).
 */
#pragma library Float

/* Useful constants (copied from GNU C Library version 2.16 math.h, see http://www.gnu.org/software/libc/) */
#define M_E        2.7182818284590452354   /* e */
#define M_LOG2E    1.4426950408889634074   /* log_2 e */
#define M_LOG10E   0.43429448190325182765  /* log_10 e */
#define M_LN2      0.69314718055994530942  /* log_e 2 */
#define M_LN10     2.30258509299404568402  /* log_e 10 */
#define M_PI       3.14159265358979323846  /* pi */
#define M_PI_2     1.57079632679489661923  /* pi/2 */
#define M_PI_4     0.78539816339744830962  /* pi/4 */
#define M_1_PI     0.31830988618379067154  /* 1/pi */
#define M_2_PI     0.63661977236758134308  /* 2/pi */
#define M_2_SQRTPI 1.12837916709551257390  /* 2/sqrt(pi) */
#define M_SQRT2    1.41421356237309504880  /* sqrt(2) */
#define M_SQRT1_2  0.70710678118654752440  /* 1/sqrt(2) */

/* Round a float into an integer value */
native fround(Float:x);

/* wrappers for ANSI C math.h library */
native Float:sin(Float:x);
native Float:cos(Float:x);
native Float:tan(Float:x);
native Float:asin(Float:x);
native Float:acos(Float:x);
native Float:atan(Float:x);
native Float:atan2(Float:y, Float:x);
native Float:sinh(Float:x);
native Float:cosh(Float:x);
native Float:tanh(Float:x);
native Float:exp(Float:x);
native Float:log(Float:x);
native Float:log10(Float:x);
native Float:pow(Float:x, Float:y);
native Float:sqrt(Float:x);
native Float:ceil(Float:x);
native Float:floor(Float:x);
native Float:fabs(Float:x);
native Float:ldexp(Float:x, n);
native Float:frexp(Float:x, &n);
native Float:modf(Float:x, &Float:ip);
native Float:fmod(Float:x, Float:y);
native       isnan(Float:x);


/**************************************************/
#pragma rational Float

/* user defined operators */
native Float:floatmul(Float:oper1, Float:oper2);
native Float:floatdiv(Float:oper1, Float:oper2);
native Float:floatadd(Float:oper1, Float:oper2);
native Float:floatsub(Float:oper1, Float:oper2);
native Float:float(oper);
native       floatint(Float:oper);
native       floatcmp(Float:oper1, Float:oper2);

native Float:operator*(Float:oper1, Float:oper2) = floatmul;
native Float:operator/(Float:oper1, Float:oper2) = floatdiv;
native Float:operator+(Float:oper1, Float:oper2) = floatadd;
native Float:operator-(Float:oper1, Float:oper2) = floatsub;
native Float:operator=(oper) = float;
native       operator=(Float:oper) = floatint;

stock Float:operator++(Float:oper)
    return oper+1.0;

stock Float:operator--(Float:oper)
    return oper-1.0;

stock Float:operator-(Float:oper)
    return oper^Float:cellmin;                  /* IEEE values are sign/magnitude */

stock Float:operator*(Float:oper1, oper2)
    return floatmul(oper1, float(oper2));       /* "*" is commutative */

stock Float:operator/(Float:oper1, oper2)
    return floatdiv(oper1, float(oper2));

stock Float:operator/(oper1, Float:oper2)
    return floatdiv(float(oper1), oper2);

stock Float:operator+(Float:oper1, oper2)
    return floatadd(oper1, float(oper2));       /* "+" is commutative */

stock Float:operator-(Float:oper1, oper2)
    return floatsub(oper1, float(oper2));

stock Float:operator-(oper1, Float:oper2)
    return floatsub(float(oper1), oper2);

stock bool:operator==(Float:oper1, Float:oper2)
    return floatcmp(oper1, oper2) == 0;

stock bool:operator==(Float:oper1, oper2)
    return floatcmp(oper1, float(oper2)) == 0;  /* "==" is commutative */

stock bool:operator!=(Float:oper1, Float:oper2)
    return floatcmp(oper1, oper2) != 0;

stock bool:operator!=(Float:oper1, oper2)
    return floatcmp(oper1, float(oper2)) != 0;  /* "!=" is commutative */

stock bool:operator>(Float:oper1, Float:oper2)
    return floatcmp(oper1, oper2) > 0;

stock bool:operator>(Float:oper1, oper2)
    return floatcmp(oper1, float(oper2)) > 0;

stock bool:operator>(oper1, Float:oper2)
    return floatcmp(float(oper1), oper2) > 0;

stock bool:operator>=(Float:oper1, Float:oper2)
    return floatcmp(oper1, oper2) >= 0;

stock bool:operator>=(Float:oper1, oper2)
    return floatcmp(oper1, float(oper2)) >= 0;

stock bool:operator>=(oper1, Float:oper2)
    return floatcmp(float(oper1), oper2) >= 0;

stock bool:operator<(Float:oper1, Float:oper2)
    return floatcmp(oper1, oper2) < 0;

stock bool:operator<(Float:oper1, oper2)
    return floatcmp(oper1, float(oper2)) < 0;

stock bool:operator<(oper1, Float:oper2)
    return floatcmp(float(oper1), oper2) < 0;

stock bool:operator<=(Float:oper1, Float:oper2)
    return floatcmp(oper1, oper2) <= 0;

stock bool:operator<=(Float:oper1, oper2)
    return floatcmp(oper1, float(oper2)) <= 0;

stock bool:operator<=(oper1, Float:oper2)
    return floatcmp(float(oper1), oper2) <= 0;

stock bool:operator!(Float:oper)
    return (_:oper & cellmax) == 0;

/* forbidden operations */
forward operator%(Float:oper1, Float:oper2);
forward operator%(Float:oper1, oper2);
forward operator%(oper1, Float:oper2);

