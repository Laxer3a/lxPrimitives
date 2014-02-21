/**	
	License :
	
	I make no claim about ownership or creative work from my side with this code.
	While there are some stuff that I have indeed added and created myself, those tricks may be/are well known anyway.
	
	I just believe that gathering all this knowledge and put it into a :
		- Strictly validated test framework.
		- Put it into an easy to use and aggregated form.
	is where the added value of my work is.

	Those bits and pieces are in different places over the net, most copy paste type of operation,
	no test showing the limits of the functions.
	
	The sources are :
	http://aggregate.org/MAGIC/#Absolute%20Value%20of%20a%20Float
	http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
	http://bits.stephan-brumme.com/
	http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
	
	This library provide a lot of hack and tricks to perform operations :
	1 - Some operations do ON PURPOSE break specification / compliance, please understand why before using them :
		- Loss of precision.
		- Operation broken for a specific value or range of values.
		
	2 - Some operations are 100% functionnal but the compiler may provide specific implementation
	which are more efficient for the platform, I do not claim that the following implementation is the fastest.
	Just that on some architecture it may do.

	The goal here is to :
	1 - Provide an alternative, without spending endless hours reinventing the wheel for
	things that have already been done. 
	2 - Provide an algorithm that use the intrinsic structure of number on computers to get the job done asap without having
	to worry about testing and validation. (Sanity check is performed)
	3 - Provide a base which could be modified to fit your needs.
	
	One example 	: ARM cpu have shift operation mostly for free, but no integer divide.
	
	Another example	: Some CPU such as old ARM core do not provide a FPU and FPU compliance is made using software implementation.
	                  In such cases, gain in performance may be obtained at the price of loss in performance or compliance.
	
	Important note :
	The performance gain may vary a lot between architecture for some of the functions.
*/
#ifndef LX_HACK_H
#define LX_HACK_H

#include "lxTypes.h"

#define optinline inline
 
namespace lx {
	
	// TODO SIMD implementation if we can provide / trick to do the same things too.

	// =================================================================
	//   Gray Code
	// =================================================================
	class lxGray {
	public:
		optinline static
		u32 gray2b(u32 gray) {
			gray ^= (gray >> 16);
			gray ^= (gray >>  8);
			gray ^= (gray >>  4);
			gray ^= (gray >>  2);
			gray ^= (gray >>  1);
			return(gray);
		}
	};
	
	// =================================================================
	//   Signed Integer
	//	 TODO : 64 bit signed integer versions.
	// =================================================================
	class lxSignedInt32 {
	public:
		optinline static
		s32 Ceil(s32 val, s32 divider)
		{	return (val + divider - 1) / divider;	}
		
		optinline static
		s32 RoundNearest(s32 val, s32 divider) 
		{	return (val+(divider>>1)) / divider;	}
		
		#define	WORDBITS	(32)
		
		optinline static
		s32 BranchlessMin(s32 x, s32 y) 
		{	return x+((((s32)(y-x))>>(WORDBITS-1))&(y-x));							}
		
		optinline static
		s32 BranchlessMax(s32 x, s32 y) 
		{	return x-((((s32)(x-y))>>(WORDBITS-1))&(x-y));							}
		
		optinline static
		s32 SelectBranchlessALessThanB(s32 A, s32 B, s32 resIfA, s32 resIfB) 
		{	return (((((s32)(A-B)) >> (WORDBITS-1)) & (resIfA^resIfB)) ^ resIfB);	}
		
		optinline static
		s32 Abs(s32 x) 
		{	const int sign = x >> (WORDBITS-1);	return (x ^ sign) - sign;			}
		
		#undef WORDBITS
	};
	
	// =================================================================
	//   Unsigned integer
	//   TODO : 64 bit unsigned versions.
	// =================================================================	
	// TODO SIMD, Additive, Substractive math
	// TODO 64 bit int and double support
	
	class lxUnsignedInt32 {
	public:
		optinline static
		u32 ui_isPowerOf2(u32 x)
		{	return ((x&(x-1)) == 0);		}
	
		optinline static
		u32 ui_closestPowerOf2(u32 x) {
			x |= (x >> 1);
			x |= (x >> 2);
			x |= (x >> 4);
			x |= (x >> 8);
			x |= (x >> 16);
			return(x+1);
		}
	
		optinline static
		u32 ui_moduloPower2(u32 x, u32 power2Value) 
		{	return x & (power2Value-1);		}
	};
	
	// =================================================================
	//   Bits
	// =================================================================

	class lxBit {
	public:
		optinline static
		u32 countOnes(u32 x) {
				/*	32-bit recursive reduction using SWAR...
					but first step is mapping 2-bit values
					into sum of 2 1-bit values in sneaky way */
				x -= ((x >> 1) & 0x55555555);
				x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
				x = (((x >> 4) + x) & 0x0f0f0f0f);
				x += (x >> 8);
				x += (x >> 16);
				return(x & 0x0000003f);
		}

		optinline static
		u32 leadCountZero(u32 x) {
			x |= (x >> 1);
			x |= (x >> 2);
			x |= (x >> 4);
			x |= (x >> 8);
			x |= (x >> 16);
			#define WORDBITS	(32)
			return(WORDBITS - countOnes(x));
			#undef WORDBITS
		}
		
		optinline static
		u32 trailingCountZero(s32 x)
		{	return(countOnes((x & -x) - 1));}
		
		optinline static
		u32 leastSignificant1Bit(u32 x) 
		{	return (x&(u32)(-(s32)x));		}

		optinline static
		u32 mostSignificant1Bit(u32 x) {
			x |= (x >> 1);
			x |= (x >> 2);
			x |= (x >> 4);
			x |= (x >> 8);
			x |= (x >> 16);
			return(x & ~(x >> 1));
		}
				
		optinline static
		u32 set(u32 x, u8 position)
		{	return x | (1 << position);		}

		optinline static
		u32 clear(u32 x, u8 position)
		{	return x & ~(1 << position);	}

		optinline static
		u32 modify(u32 x, u8 position, u8 bitValue)
		{	u32 mask = 1 << position; s32 state = bitValue;
			return (x & ~mask) | (-state & mask);			}

		optinline static
		u32 flip(u32 x, u8 position)
		{	return x ^ (1 << position);		}

		optinline static
		u32 isBitSet(u32 x, u8 position)
		{	return ((x >>= position) & 1);	}
		
		optinline static
		u32 reverse_A(u32 x) {
			x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
			x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
			x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
			x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
			return((x >> 16) | (x << 16));
		}

		optinline static
		u32 reverse_B(u32 x) {
			u32 y = 0x55555555;
			x = (((x >> 1) & y) | ((x & y) << 1));
			y = 0x33333333;
			x = (((x >> 2) & y) | ((x & y) << 2));
			y = 0x0f0f0f0f;
			x = (((x >> 4) & y) | ((x & y) << 4));
			y = 0x00ff00ff;
			x = (((x >> 8) & y) | ((x & y) << 8));
			return((x >> 16) | (x << 16));
		}
		
		optinline static
		u32 countOnes32(u32 x) {
			/* 
			32-bit recursive reduction using SWAR...
			but first step is mapping 2-bit values
			into sum of 2 1-bit values in sneaky way
			*/
			x -= ((x >> 1) & 0x55555555);
			x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
			x = (((x >> 4) + x) & 0x0f0f0f0f);
			x += (x >> 8);
			x += (x >> 16);
			return(x & 0x0000003f);
		}
		
		/** Upper bit are shifted to the left, 0 bit is inserted */
		optinline static
		u32 insert1Bit(u32 x, u32 bitPosition) {
			// 2 OPS : ADD and AND if constant.
			// normal masking would require 2 AND and 1 OR.
			return x + (x & (~((1<<bitPosition)-1)));
		}
		
		/** Work only when mBit > nBit and mBit <= nBit*2 
			ie : 4 bit to 8 bit OK, 4 bit to 9 bit : Fail
			
			But multiple call can work.
			ie : Normalize from 2 bit to 8 bit
			bitNormalizeNToMBit(bitNormalizeNToMBit(x,2,4),4,8)
		 */
		optinline static
		u32 normalizeNtoMBit(u32 x, u32 inputBit, u32 outputBit) {
			return x | (x<<(outputBit-inputBit));
		}
		
		optinline static
		u32 signExtension(u32 x, u32 bitPosition) {
			u32 mask = 1<<(bitPosition-1);
			return ((x ^ mask) - mask);
		}
	};
	
	// =================================================================
	//  Float
	//	Those float function will allow to perform work on float
	//  without having to use the FPU and have the impact of the latency
	//  associated with it. (loading, waiting for result, storing)
	//  
	//  But it is important to understand that those function are NOT
	//  efficient in the middle of real float computation work.
	//  It will force the CPU to wait for the FPU to complete, modify
	//  the values, and then load them again into the FPU pipeline.
	//  Modern CPU having multiple integer ALU path per cycle.
	//
	//  So performance wise, you can use them in big loops as long as
	//  it does not involve other FPU work.
	//  
	//  The only exception here is the Reciproqual Square Root
	//  which is an approximation of the real function and use both FPU and INT data path.
	//
	// =================================================================
	
	#define FasI(f)  (*((int *) &(f)))
	#define FasUI(f) (*((unsigned int *) &(f)))
	
	class lxFloat {
	public:
		optinline static
		int LessThan0(float f) {
			return (FasUI(f) > 0x80000000U);
		}

		optinline static
		int LessEqual0(float f) {
			return (FasI(f) <= 0);
		}
		
		optinline static
		int GreaterThan0(float f) {
			return (FasI(f) > 0);
		}

		optinline static
		int GreaterEqual0(float f) {
			return (FasUI(f) <= 0x80000000U);
		}
		
		optinline static
		int ComparePositiveOnly_lessThan(float a, float b) {
			return (*(s32*)&a < *(int*)&b);
		}

		optinline static
		int ComparePositiveOnly_lessOrEqual(float a, float b) {
			return (*(s32*)&a <= *(int*)&b);
		}
		
		optinline static
		float ApproxReciproqualSQRT(float number) {
			/* Source : Wikipedia / Quake Arena III 
			   Worst case error : 0.175%
			   Chris Lomont work : http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
			 */
			u32 i;
			float x2, y;

			x2 = number * 0.5f;
			y  = number;
			i  = * ((u32*) &y);
			i  = 0x5f375a86 /*0x5f3759df*/ - ( i >> 1 ); // Using Chris Lomont new constant instead.
			y  = * ( float * ) &i;
			y  = y * ( 1.5f - ( x2 * y * y ) );	// 1st iteration newton raphson.
			// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

			return y;
		}

		optinline static
		// TODO error range max, min, average
		float ApproxInverseA(float a) {
			s32 _i = 2 * 0x3F800000 - *(s32 *)&(a);
			float r = *(float *)&_i;
			return (r) * (2.0f - (a) * (r));
		}

		optinline static
		// TODO error range max, min, average
		float ApproxInverseB(float x) {
			// re-interpret as a 32 bit integer
			u32 *i = (u32*)&x;

			// adjust exponent
			*i = 0x7F000000 - *i; // Keep inverse 1 as 1 but generate more errors.
			return x;
		}
		
		optinline static
		// TODO error range max, min, average
		float ApproxInverseC(float x) {
			// re-interpret as a 32 bit integer
			u32 *i = (u32*)&x;

			// adjust exponent
			*i = 0x7EEEEEEE - *i; // Keep inverse 1 as 1 but generate more errors.
			return x;
		}
		
		optinline static
		float Fabs(float f) {
			// Use inverse constant in case some HW have better storage for such constant at the cost of a NOT instruction.
			// Compiler will decide the most efficient implementation.
			s32 i= (*((s32*)&f)) & (~0x80000000);	
			return *((float*)&i);
		}

		optinline static
		float Neg(float f) {
			s32 i=((*(int*)&f)^0x80000000);
			return *((float*)&i);
		}

		optinline static
		float ForceNeg(float f) {
			s32 i=((*(int*)&f)|0x80000000);
			return *((float*)&i);
		}

		optinline static
		s32 Sgn(float f) {
			// -1,0,+1
			// (((*(int*)&f)>>31)<<1) : 0,-2 if neg
			return 1+(((*(int*)&f)>>31)<<1);	// TODO float >=0 compare with int, fabs, fneg, fsign, ...
		}
	};

	#undef FasI
	#undef FasUI
	
	/*
		// Initial AlmostEqualULPs version - fast and simple, but
		// some limitations.
		bool AlmostEqualUlps(float A, float B, int maxUlps)
		{
			assert(sizeof(float) == sizeof(int));
			if (A == B)
				return true;
			int intDiff = abs(*(int*)&A - *(int*)&B);
			if (intDiff <= maxUlps)
				return true;
			return false;
		}	
		

		Limitations

		maxUlps cannot be arbitrarily large. If maxUlps is four million or greater then there is a risk of finding large negative floats equal to NANs. If maxUlps is sixteen million or greater then the largest positive floats will compare as equal to the largest negative floats.
		As a practical matter such large maxUlps values should not be needed. A maxUlps of sixteen million means that numbers 100% larger and 50% smaller should count as equal. A maxUlps of four million means that numbers 25% larger and 12.5% smaller should count as equal. If these large maxUlps values are needed then separate checking for wrap-around above infinity to NANs or numbers of the opposite sign will be needed. To prevent accidental usage of huge maxUlps values the comparison routines assert that maxUlps is in a safe range.
		AlmostEqual2sComplement is very reliant on the IEEE floating point math format, and assumes twos-complement integers of the same size. These limitations are the norm on the majority of machines, especially consumer machines, but there are machines out there that use different formats. For this reason, and because the techniques used are tricky and non-obvious, it is important to encapsulate the behavior in a function where appropriate documentation, asserts, and conditional checks can be placed.
		Summary

		AlmostEqual2sComplement is an effective way of handling floating point comparisons. Its behavior does not map perfectly to AlmostEqualRelative, but in many ways its behavior is arguably superior. To summarize, AlmostEqual2sComplement has these characteristics:

			Measures whether two floats are eclosef to each other, where close is defined by ulps, also interpreted as how many floats there are in-between the numbers
			Treats infinity as being close to FLT_MAX
			Treats NANs as being four million ulps away from everything (assuming the default NAN value for x87), except other NANs
			Accepts greater relative error as numbers gradually underflow to subnormals
			Treats tiny negative numbers as being close to tiny positive numbers
			
		If the special characteristics of AlmostEqual2sComplement are not desirable then they can selectively be checked for. A version with the necessary checks, #ifdefed for easy control of the behavior, is available here.
		AlmostEqual2sComplement works best on machines that can transfer values quickly between the floating point and integer units. This often requires going through memory and can be quite slow. This function can be implemented efficiently on machines with vector units that can do integer or floating point operations on the same registers. This allows reinterpreting the values without going through memory.
		The same techniques can be applied to doubles, mapping them to 64-bit integers. Because doubles have a 53-bit mantissa a one ulp error implies a relative error of between 1/4,000,000,000,000,000 and 1/8,000,000,000,000,000.
		
		
		// Usable AlmostEqual function
		bool AlmostEqual2sComplement(float A, float B, int maxUlps)
		{
			// Make sure maxUlps is non-negative and small enough that the
			// default NAN won't compare as equal to anything.
			assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
			int aInt = *(int*)&A;
			// Make aInt lexicographically ordered as a twos-complement int
			if (aInt < 0)
				aInt = 0x80000000 - aInt;

			// Make bInt lexicographically ordered as a twos-complement int
			int bInt = *(int*)&B;

			if (bInt < 0)
				bInt = 0x80000000 - bInt;

			int intDiff = abs(aInt - bInt);
			if (intDiff <= maxUlps)
				return true;

			return false;
		}
		
	*/
} // End namespace

#endif // LX_HACK_H
