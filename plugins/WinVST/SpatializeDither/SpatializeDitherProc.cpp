/* ========================================
 *  SpatializeDither - SpatializeDither.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SpatializeDither_H
#include "SpatializeDither.h"
#endif

void SpatializeDither::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double contingentRnd;
	double absSample;
	double contingent;
	double randyConstant = 1.61803398874989484820458683436563811772030917980576;
	double omegaConstant = 0.56714329040978387299996866221035554975381578718651;
	double expConstant = 0.06598803584531253707679018759684642493857704825279;
	int processing = (VstInt32)( A * 1.999 );
	bool highres = false;
	if (processing == 1) highres = true;
	float scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	float derez = B;
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	float outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
    
	while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		if (inputSampleL > 0) inputSampleL += 0.383;
		if (inputSampleL < 0) inputSampleL -= 0.383;
		if (inputSampleR > 0) inputSampleR += 0.383;
		if (inputSampleR < 0) inputSampleR -= 0.383;
		//adjusting to permit more information drug outta the noisefloor
		
		contingentRnd = (double(fpdL)/UINT32_MAX);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		contingentRnd += ((double(fpdL)/UINT32_MAX)-1.0);
		contingentRnd *= randyConstant; //produce TPDF dist, scale
        contingentRnd -= contingentErrL*omegaConstant; //include err
		absSample = fabs(inputSampleL);
		contingentErrL = absSample - floor(absSample); //get next err
		contingent = contingentErrL * 2.0; //scale of quantization levels
		if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
		else contingent = (contingent * omegaConstant) + expConstant;
		//zero is next to a quantization level, one is exactly between them
		if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
		else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
		inputSampleL += (contingentRnd * contingent);
		//Contingent Dither
		inputSampleL = floor(inputSampleL);
		
		contingentRnd = (double(fpdR)/UINT32_MAX);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		contingentRnd += ((double(fpdR)/UINT32_MAX)-1.0);
		contingentRnd *= randyConstant; //produce TPDF dist, scale
        contingentRnd -= contingentErrR*omegaConstant; //include err
		absSample = fabs(inputSampleR);
		contingentErrR = absSample - floor(absSample); //get next err
		contingent = contingentErrR * 2.0; //scale of quantization levels
		if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
		else contingent = (contingent * omegaConstant) + expConstant;
		//zero is next to a quantization level, one is exactly between them
		if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
		else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
		inputSampleR += (contingentRnd * contingent);
		//Contingent Dither
		inputSampleR = floor(inputSampleR);
		
		//note: this does not dither for values exactly the same as 16 bit values-
		//which forces the dither to gate at 0.0. It goes to digital black,
		//and does a teeny parallel-compression thing when almost at digital black.
		flip = !flip;
		
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//pseudorandom number updater

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void SpatializeDither::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double contingentRnd;
	double absSample;
	double contingent;
	double randyConstant = 1.61803398874989484820458683436563811772030917980576;
	double omegaConstant = 0.56714329040978387299996866221035554975381578718651;
	double expConstant = 0.06598803584531253707679018759684642493857704825279;
	int processing = (VstInt32)( A * 1.999 );
	bool highres = false;
	if (processing == 1) highres = true;
	float scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	float derez = B;
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	float outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		if (inputSampleL > 0) inputSampleL += 0.383;
		if (inputSampleL < 0) inputSampleL -= 0.383;
		if (inputSampleR > 0) inputSampleR += 0.383;
		if (inputSampleR < 0) inputSampleR -= 0.383;
		//adjusting to permit more information drug outta the noisefloor
		
		contingentRnd = (double(fpdL)/UINT32_MAX);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		contingentRnd += ((double(fpdL)/UINT32_MAX)-1.0);
		contingentRnd *= randyConstant; //produce TPDF dist, scale
        contingentRnd -= contingentErrL*omegaConstant; //include err
		absSample = fabs(inputSampleL);
		contingentErrL = absSample - floor(absSample); //get next err
		contingent = contingentErrL * 2.0; //scale of quantization levels
		if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
		else contingent = (contingent * omegaConstant) + expConstant;
		//zero is next to a quantization level, one is exactly between them
		if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
		else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
		inputSampleL += (contingentRnd * contingent);
		//Contingent Dither
		inputSampleL = floor(inputSampleL);
		
		contingentRnd = (double(fpdR)/UINT32_MAX);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		contingentRnd += ((double(fpdR)/UINT32_MAX)-1.0);
		contingentRnd *= randyConstant; //produce TPDF dist, scale
        contingentRnd -= contingentErrR*omegaConstant; //include err
		absSample = fabs(inputSampleR);
		contingentErrR = absSample - floor(absSample); //get next err
		contingent = contingentErrR * 2.0; //scale of quantization levels
		if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
		else contingent = (contingent * omegaConstant) + expConstant;
		//zero is next to a quantization level, one is exactly between them
		if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
		else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
		inputSampleR += (contingentRnd * contingent);
		//Contingent Dither
		inputSampleR = floor(inputSampleR);
		
		//note: this does not dither for values exactly the same as 16 bit values-
		//which forces the dither to gate at 0.0. It goes to digital black,
		//and does a teeny parallel-compression thing when almost at digital black.
		flip = !flip;
		
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//pseudorandom number updater

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}