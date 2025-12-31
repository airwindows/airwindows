/* ========================================
 *  BitShiftGain - BitShiftGain.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */
/* ========================================
 *  "Improvement" by EverythingDilettante. If it works, it should obviate problematic behaviour which multiplications can exhibit around subnormal numbers, even in power-of-two operations (causes around -300dB crossover distortion on 32-bit floats (this is only partially a joke)).
 *  Copyright (c) 2025 airwindows, if he wants it... especially since he will probably have to fix it anyways... if he does not want it, then it is MIT (and probably if he does).
 *  I also have no way to test this plugin, lacking the same development environment as Chris...
 * ======================================== */

#ifndef __BitShiftGain_H
#include "BitShiftGain.h"
#endif

void BitShiftGain::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) {
	static short clippingFor1 = 0;
	static uint32_t storedMant1 = 0;

    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	int bitshiftGain = (A * 32)-16;
	uint32_t bitStream1 = *reinterpret_cast<uint32_t*>(&in1); // Evil bit-level floating-point hacking, to better handle subnormal/near-normal values (and maybe speed?)
	uint32_t bitStream2 = *reinterpret_cast<uint32_t*>(&in2);
	bool sign1 = bitStream1 & 0x80000000; // First bit for the sign.
	bool sign2 = bitstream2 & 0x80000000;
	uint8_t exp1 = (bitStream1 & 0x7F800000) >> 23; // Next eight bits for the exponent.
	uint8_t exp2 = (bitStream2 & 0x7F800000) >> 23;
	uint32_t mant1 = bitStream1 & 0x7FFFFF; // Last 23 bits for the mantissa.
	uint32_t mant2 = bitStream2 & 0x7FFFFF;
	bool switchedNormality1 = false;
	bool switchedNormality2 = false;

	if (bitShiftGain < 0) {
		short bitShiftGain1 = bitShiftGain;
		short bitShiftGain2 = bitShiftGain;
		short i;
		for (i = 0; exp1 > 0 && bitShiftGain1; bitShiftGain1++, exp1--, i++) { } // When volume-shifts can be done using the exponent instead of messing with the mantissa, and there are shifts to do, do them on the exponent.
		if (!exp1 && i) { // We need to know if the number BECAME subnormal, because this requires some extra calculation.
				switchedNormality1 = true;
		}
		for (i = 0; exp2 > 0 && bitShiftGain2; bitShiftGain2++, exp2--, i++) { }
		if (!exp2 && i) {
				switchedNormality2 = true;
		}
		
		{ // The second stage, after the exponent decrements have been exhausted, pumps any remaining reductions into the mantissa. This is the part that is sonically improved over regular multiplication, which usually just zeroes out (and in any case does not noise-shape).
			if (bitShiftGain1 || bitShiftGain2 || switchedNormality1 || switchedNormality2) {
				double floatMantL = static_cast<double>(mant1);  // I am going to try to shove in Spatialize for the subnormals, and probably fail... Granted, another dither might work better, such as Ten Nines into Dark, but I have never actually used any dithers yet...
				double floatMantR = static_cast<double>(mant2); // Because I have never worked with Airwindows code, I do not trust mine, so Chris will DEFINITELY want to review this before approving the commit. Also... is L = 1; R = 2 correct?
				if (switchedNormality1) {
					floatMantL *= 0.5; // Going subnormal does not halve the number like every other exponent reduction does, so we have to do this ourselves.
					floatMantL += 4'194'304.0; // It does, however, remove the implicit 1, so we add 0.5 to the mantissa.
				}
				if (switchedNormality2) {
					floatMantR *= 0.5;
					floatMantR += 4'194'304.0;
				}
				if (bitShiftGain1) {
					floatMantL /= 1 << (bitShiftGain1);
				}
				if (bitShiftGain2) {
					floatMantR /= 1 << (bitShiftGain2);
				}
				double absSample;
				double contingent;
				double randyConstant = 1.61803398874989484820458683436563811772030917980576;
				const double omegaConstant = 0.56714329040978387299996866221035554975381578718651;
				const double expConstant = 0.06598803584531253707679018759684642493857704825279;
				const float scaleFactor = 8'388'608.0; // Interesting thought: An integer is technically just a subnormal float with an exponent bias equal to the inverse of the number of non-sign bits.
				
				double inputSampleL = floatMantL * scaleFactor; // Can omit "if (fabs(inputSampleL)<1.18e-23)..." because the mantissa will never be that small.
				double inputSampleR = floatMantR * scaleFactor;
				if (inputSampleL > 0) inputSampleL += 0.383;
				if (inputSampleL < 0) inputSampleL -= 0.383;
				if (inputSampleR > 0) inputSampleR += 0.383;
				if (inputSampleR < 0) inputSampleR -= 0.383;
					
				contingentRnd = (double(fpdL)/UINT32_MAX);
				fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
				contingentRnd += ((double(fpdL)/UINT32_MAX)-1.0);
				contingentRnd *= randyConstant; //produce TPDF dist, scale
       			contingentRnd -= contingentErrL*omegaConstant; //include err
				absSample = fabs(inputSampleL);
				contingentErrL = absSample - floor(absSample); //get next err
				contingent = contingentErrL * 2.0; //scale of quantization levels
				if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
				else contingent = (contingent * omegaConstant) + expConstant; //zero is next to a quantization level, one is exactly between them
				if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
				else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
				inputSampleL += (contingentRnd * contingent); //Contingent Dither
				inputSampleL = floor(inputSampleL);
					
				contingentRnd = (double(fpdR)/UINT32_MAX);
				fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
				contingentRnd += ((double(fpdR)/UINT32_MAX)-1.0)
				contingentRnd *= randyConstant; //produce TPDF dist, scale
       			contingentRnd -= contingentErrR*omegaConstant; //include err
				absSample = fabs(inputSampleR);
				contingentErrR = absSample - floor(absSample); //get next err
				contingent = contingentErrR * 2.0; //scale of quantization levels
				if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
				else contingent = (contingent * omegaConstant) + expConstant; //zero is next to a quantization level, one is exactly between them
				if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
				else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
				inputSampleR += (contingentRnd * contingent); //Contingent Dither
				inputSampleR = floor(inputSampleR);
					
				flip = !flip;
		
				inputSampleL /= outScale;
				inputSampleR /= outScale;

				do {
					fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
					fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
				} while (fabs(fpDL - fpDR) < 0.5); // Is 0.5 the correct number? I do not know what the scale of these variables is...
				//pseudorandom number updater (I tried to also hex in the same-number discard from the "wide" series of dithers.)

				mant1 = inputSampleL;
				mant2 = inputSampleR;
			}
		}
	} else if (bitShiftGain > 0) {
		for (; bitShiftGain1 && !(exp1 & 0x80); mant1 <<= 1, bitShiftGain1--) { } // Are there shifts left? If so, then use them on the mantissa as long as it has range (MSB is zero).
		for (; bitShiftGain2 && !(exp2 & 0x80); mant2 <<= 1, bitShiftGain2--) { }
		
		if (bitShiftGain1) {
			mant1 = (mant1 << 1) - 4'194'304;
			exp1++;
			bitShiftGain1--;
		}
		if (bitShiftGain2) {
			mant2 = (mant2 << 1) - 4'194'304;
			exp2++;
			bitShiftGain2--;
		}
		
		for (; !bitShiftGain1 && exp1 < 255; exp1++, bitShiftGain--) { }
		for (; !bitShiftGain2 && exp2 < 255; exp2++, bitShiftGain--) { }

		if (exp1 == 255 && clippingFor1 < 3) { // 3 seemed to fit at 96k... simply changing the length of the buffer would not make it scale with sample rate, anyways. And nobody should have audio at +200dB.
			clippingFor1++;
			exp1 = 254; // If we somehow receive +/-Infinity or NaN, we will be courteous.
			storedMant1 = 8'388'607 - (8'388'607 - (storedMant1 >> 1));
			mant1 = storedMant1;
		} else if (exp1 == 255) {
			exp1 = 254;
			mant1 = 8'388'607;
			clippingFor1 = 3;
		} else if (exp < 255) {
			clippingFor1 = 0;

		if (exp2 == 255 && clippingFor1 < 3) { // 3 seemed to fit at 96k... simply changing the length of the buffer would not make it scale with sample rate, anyways. And nobody should have audio at +200dB.
			clippingFor2++;
			exp2 = 254; // If we somehow receive +/-Infinity or NaN, we will be courteous.
			storedMant2 = 8'388'607 - (8'388'607 - (storedMant1 >> 1));
			mant2 = storedMant1;
		} else if (exp2 == 255) {
			exp2 = 254;
			mant2 = 8'388'607;
			clippingFor2 = 3;
		} else if (exp < 255) {
			clippingFor2 = 0;
		}
	} else {
		if (exp1 == 255 && clippingFor1 < 3) { // 3 seemed to fit at 96k... simply changing the length of the buffer would not make it scale with sample rate, anyways. And nobody should have audio at +200dB.
			clippingFor1++;
			exp1 = 254; // If we somehow receive +/-Infinity or NaN, we will be courteous.
			storedMant1 = 8'388'607 - (8'388'607 - (storedMant1 >> 1));
			mant1 = storedMant1;
		} else if (exp1 == 255) {
			exp1 = 254;
			mant1 = 8'388'607;
			clippingFor1 = 3;
		} else if (exp < 255) {
			clippingFor1 = 0;

		if (exp2 == 255 && clippingFor1 < 3) { // 3 seemed to fit at 96k... simply changing the length of the buffer would not make it scale with sample rate, anyways. And nobody should have audio at +200dB.
			clippingFor2++;
			exp2 = 254; // If we somehow receive +/-Infinity or NaN, we will be courteous.
			storedMant2 = 8'388'607 - (8'388'607 - (storedMant1 >> 1));
			mant2 = storedMant1;
		} else if (exp2 == 255) {
			exp2 = 254;
			mant2 = 8'388'607;
			clippingFor2 = 3;
		} else if (exp < 255) {
			clippingFor2 = 0;
		}
	}

	uint32_t newBits1 = mant1 + (exp1 << 23) + (sign1 << 31);
	uint32_t newBits2 = mant2 + (exp2 << 23) + (sign2 << 31); // If I had a plan from the beginning, I would have used an array...
	
    while (--sampleFrames >= 0)
    {
		*out1 = *reinterpret_cast<float*>(newBits1);
		*out2 = *reinterpret_cast<float*>(newBits2);

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void BitShiftGain::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) { // I probably messed something up when changing the bit-depth.
	static short clippingFor1 = 0;
	static uint64_t storedMant1 = 0;

    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	int bitshiftGain = (A * 32)-16;
	uint64_t bitStream1 = *reinterpret_cast<uint32_t*>(&in1); // Evil bit-level floating-point hacking, to better handle subnormal/near-normal values (and maybe speed?)
	uint64_t bitStream2 = *reinterpret_cast<uint32_t*>(&in2);
	bool sign1 = bitStream1 & 0x8000000000000000; // First bit for the sign.
	bool sign2 = bitstream2 & 0x8000000000000000;
	uint16_t exp1 = (bitStream1 & 0x7FF0000000000000) >> 52; // Next eleven bits for the exponent.
	uint16_t exp2 = (bitStream2 & 0x7FF0000000000000) >> 52;
	uint64_t mant1 = bitStream1 & 0xFFFFFFFFFFFFF; // Last 52 bits for the mantissa.
	uint64_t mant2 = bitStream2 & 0xFFFFFFFFFFFFF;
	bool switchedNormality1 = false;
	bool switchedNormality2 = false;

	if (bitShiftGain < 0) {
		short bitShiftGain1 = bitShiftGain;
		short bitShiftGain2 = bitShiftGain;
		short i;
		for (i = 0; exp1 > 0 && bitShiftGain1; bitShiftGain1++, exp1--, i++) { } // When volume-shifts can be done using the exponent instead of messing with the mantissa, and there are shifts to do, do them on the exponent.
		if (!exp1 && i) { // We need to know if the number BECAME subnormal, because this requires some extra calculation.
				switchedNormality1 = true;
		}
		for (i = 0; exp2 > 0 && bitShiftGain2; bitShiftGain2++, exp2--, i++) { }
		if (!exp2 && i) {
				switchedNormality2 = true;
		}
		
		{ // The second stage, after the exponent decrements have been exhausted, pumps any remaining reductions into the mantissa. This is the part that is sonically improved over regular multiplication, which usually just zeroes out (and in any case does not noise-shape).
			if (bitShiftGain1 || bitShiftGain2 || switchedNormality1 || switchedNormality2) {
				long double floatMantL = static_cast<double>(mant1);  // I am going to try to shove in Spatialize for the subnormals, and probably fail... Granted, another dither might work better, such as Ten Nines into Dark, but I have never actually used any dithers yet...
				long double floatMantR = static_cast<double>(mant2); // Because I have never worked with Airwindows code, I do not trust mine, so Chris will DEFINITELY want to review this before approving the commit. Also... is L = 1; R = 2 correct?
				if (switchedNormality1) {
					floatMantL *= 0.5; // Going subnormal does not halve the number like every other exponent reduction does, so we have to do this ourselves.
					floatMantL += 2'251'799'813'685'248.0; // It does, however, remove the implicit 1, so we add 0.5 to the mantissa.
				}
				if (switchedNormality2) {
					floatMantR *= 0.5;
					floatMantR += 2'251'799'813'685'248.0;
				}
				if (bitShiftGain1) {
					floatMantL /= 1 << (bitShiftGain1);
				}
				if (bitShiftGain2) {
					floatMantR /= 1 << (bitShiftGain2);
				}
				double absSample;
				double contingent;
				double randyConstant = 1.61803398874989484820458683436563811772030917980576;
				const double omegaConstant = 0.56714329040978387299996866221035554975381578718651;
				const double expConstant = 0.06598803584531253707679018759684642493857704825279;
				const double scaleFactor = 2'251'799'813'685'248.0; // I... hope this works...
				
				long double inputSampleL = floatMantL * scaleFactor; // Can omit "if (fabs(inputSampleL)<1.18e-23)..." because the mantissa will never be that small.
				long double inputSampleR = floatMantR * scaleFactor;
				if (inputSampleL > 0) inputSampleL += 0.383;
				if (inputSampleL < 0) inputSampleL -= 0.383;
				if (inputSampleR > 0) inputSampleR += 0.383;
				if (inputSampleR < 0) inputSampleR -= 0.383;
					
				contingentRnd = (double(fpdL)/UINT32_MAX);
				fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
				contingentRnd += ((double(fpdL)/UINT32_MAX)-1.0);
				contingentRnd *= randyConstant; //produce TPDF dist, scale
       			contingentRnd -= contingentErrL*omegaConstant; //include err
				absSample = fabs(inputSampleL);
				contingentErrL = absSample - floor(absSample); //get next err
				contingent = contingentErrL * 2.0; //scale of quantization levels
				if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
				else contingent = (contingent * omegaConstant) + expConstant; //zero is next to a quantization level, one is exactly between them
				if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
				else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
				inputSampleL += (contingentRnd * contingent); //Contingent Dither
				inputSampleL = floor(inputSampleL);
					
				contingentRnd = (double(fpdR)/UINT32_MAX);
				fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
				contingentRnd += ((double(fpdR)/UINT32_MAX)-1.0)
				contingentRnd *= randyConstant; //produce TPDF dist, scale
       			contingentRnd -= contingentErrR*omegaConstant; //include err
				absSample = fabs(inputSampleR);
				contingentErrR = absSample - floor(absSample); //get next err
				contingent = contingentErrR * 2.0; //scale of quantization levels
				if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
				else contingent = (contingent * omegaConstant) + expConstant; //zero is next to a quantization level, one is exactly between them
				if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
				else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
				inputSampleR += (contingentRnd * contingent); //Contingent Dither
				inputSampleR = floor(inputSampleR);
					
				flip = !flip;
		
				inputSampleL /= outScale;
				inputSampleR /= outScale;

				do {
					fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
					fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
				} while (fabs(fpDL - fpDR) < 0.5); // Is 0.5 the correct number? I do not know what the scale of these variables is...
				//pseudorandom number updater (I tried to also hex in the same-number discard from the "wide" series of dithers.)

				mant1 = inputSampleL;
				mant2 = inputSampleR;
			}
		}
	} else if (bitShiftGain > 0) {
		for (; bitShiftGain1 && !(exp1 & 0x80); mant1 <<= 1, bitShiftGain1--) { } // Are there shifts left? If so, then use them on the mantissa as long as it has range (MSB is zero).
		for (; bitShiftGain2 && !(exp2 & 0x80); mant2 <<= 1, bitShiftGain2--) { }
		
		if (bitShiftGain1) {
			mant1 = (mant1 << 1) - 4'194'304;
			exp1++;
			bitShiftGain1--;
		}
		if (bitShiftGain2) {
			mant2 = (mant2 << 1) - 4'194'304;
			exp2++;
			bitShiftGain2--;
		}
		
		for (; !bitShiftGain1 && exp1 < 255; exp1++, bitShiftGain--) { }
		for (; !bitShiftGain2 && exp2 < 255; exp2++, bitShiftGain--) { }

		if (exp1 == 255 && clippingFor1 < 3) { // 3 seemed to fit at 96k... simply changing the length of the buffer would not make it scale with sample rate, anyways. And nobody should have audio at +200dB.
			clippingFor1++;
			exp1 = 254; // If we somehow receive +/-Infinity or NaN, we will be courteous.
			storedMant1 = 8'388'607 - (8'388'607 - (storedMant1 >> 1));
			mant1 = storedMant1;
		} else if (exp1 == 255) {
			exp1 = 254;
			mant1 = 8'388'607;
			clippingFor1 = 3;
		} else if (exp < 255) {
			clippingFor1 = 0;

		if (exp2 == 255 && clippingFor1 < 3) { // 3 seemed to fit at 96k... simply changing the length of the buffer would not make it scale with sample rate, anyways. And nobody should have audio at +200dB.
			clippingFor2++;
			exp2 = 254; // If we somehow receive +/-Infinity or NaN, we will be courteous.
			storedMant2 = 8'388'607 - (8'388'607 - (storedMant1 >> 1));
			mant2 = storedMant1;
		} else if (exp2 == 255) {
			exp2 = 254;
			mant2 = 8'388'607;
			clippingFor2 = 3;
		} else if (exp < 255) {
			clippingFor2 = 0;
		}
	} else {
		if (exp1 == 255 && clippingFor1 < 3) { // 3 seemed to fit at 96k... simply changing the length of the buffer would not make it scale with sample rate, anyways. And nobody should have audio at +200dB.
			clippingFor1++;
			exp1 = 254; // If we somehow receive +/-Infinity or NaN, we will be courteous.
			storedMant1 = 8'388'607 - (8'388'607 - (storedMant1 >> 1));
			mant1 = storedMant1;
		} else if (exp1 == 255) {
			exp1 = 254;
			mant1 = 8'388'607;
			clippingFor1 = 3;
		} else if (exp < 255) {
			clippingFor1 = 0;

		if (exp2 == 255 && clippingFor1 < 3) { // 3 seemed to fit at 96k... simply changing the length of the buffer would not make it scale with sample rate, anyways. And nobody should have audio at +200dB.
			clippingFor2++;
			exp2 = 254; // If we somehow receive +/-Infinity or NaN, we will be courteous.
			storedMant2 = 8'388'607 - (8'388'607 - (storedMant1 >> 1));
			mant2 = storedMant1;
		} else if (exp2 == 255) {
			exp2 = 254;
			mant2 = 8'388'607;
			clippingFor2 = 3;
		} else if (exp < 255) {
			clippingFor2 = 0;
		}
	}

	uint64_t newBits1 = mant1 + (exp1 << 52) + (sign1 << 63);
	uint64_t newBits2 = mant2 + (exp2 << 52) + (sign2 << 63); // If I had a plan from the beginning, I would have used an array...
	
    while (--sampleFrames >= 0)
    {
		*out1 = *reinterpret_cast<double*>(newBits1);
		*out2 = *reinterpret_cast<double*>(newBits2);

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
