/* ========================================
 *  Noise - Noise.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Noise_H
#include "Noise.h"
#endif

void Noise::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double cutoffL;
	double cutoffR;
	double cutofftarget = (A*3.5);
	double rumblecutoff = cutofftarget * 0.005;
	double invcutoffL;
	double invcutoffR;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	double highpass = C*38.0;
	int lowcut = floor(highpass);
	int dcut;
	if (lowcut > 37) {dcut= 1151;}
	if (lowcut == 37) {dcut= 1091;}
	if (lowcut == 36) {dcut= 1087;}
	if (lowcut == 35) {dcut= 1031;}
	if (lowcut == 34) {dcut= 1013;}
	if (lowcut == 33) {dcut= 971;}
	if (lowcut == 32) {dcut= 907;}
	if (lowcut == 31) {dcut= 839;}
	if (lowcut == 30) {dcut= 797;}
	if (lowcut == 29) {dcut= 733;}
	if (lowcut == 28) {dcut= 719;}
	if (lowcut == 27) {dcut= 673;}
	if (lowcut == 26) {dcut= 613;}
	if (lowcut == 25) {dcut= 593;}
	if (lowcut == 24) {dcut= 541;}
	if (lowcut == 23) {dcut= 479;}
	if (lowcut == 22) {dcut= 431;}
	if (lowcut == 21) {dcut= 419;}
	if (lowcut == 20) {dcut= 373;}
	if (lowcut == 19) {dcut= 311;}
	if (lowcut == 18) {dcut= 293;}
	if (lowcut == 17) {dcut= 233;}
	if (lowcut == 16) {dcut= 191;}
	if (lowcut == 15) {dcut= 173;}
	if (lowcut == 14) {dcut= 131;}
	if (lowcut == 13) {dcut= 113;}
	if (lowcut == 12) {dcut= 71;}
	if (lowcut == 11) {dcut= 53;}
	if (lowcut == 10) {dcut= 31;}
	if (lowcut == 9) {dcut= 27;}
	if (lowcut == 8) {dcut= 23;}
	if (lowcut == 7) {dcut= 19;}
	if (lowcut == 6) {dcut= 17;}
	if (lowcut == 5) {dcut= 13;}
	if (lowcut == 4) {dcut= 11;}
	if (lowcut == 3) {dcut= 7;}
	if (lowcut == 2) {dcut= 5;}
	if (lowcut < 2) {dcut= 3;}
	highpass = B * 22.0;
	lowcut = floor(highpass)+1;
	
	double decay = 0.001 - ((1.0-pow(1.0-D,3))*0.001);
	if (decay == 0.001) decay = 0.1;
	double wet = F;
	//removed extra dry variable
	wet *= 0.01; //correct large gain issue
	double correctionSample;
	double accumulatorSampleL;
	double accumulatorSampleR;
	double overallscale = (E*9.0)+1.0;
	double gain = overallscale;
	
	if (gain > 1.0) {f[0] = 1.0; gain -= 1.0;} else {f[0] = gain; gain = 0.0;}
	if (gain > 1.0) {f[1] = 1.0; gain -= 1.0;} else {f[1] = gain; gain = 0.0;}
	if (gain > 1.0) {f[2] = 1.0; gain -= 1.0;} else {f[2] = gain; gain = 0.0;}
	if (gain > 1.0) {f[3] = 1.0; gain -= 1.0;} else {f[3] = gain; gain = 0.0;}
	if (gain > 1.0) {f[4] = 1.0; gain -= 1.0;} else {f[4] = gain; gain = 0.0;}
	if (gain > 1.0) {f[5] = 1.0; gain -= 1.0;} else {f[5] = gain; gain = 0.0;}
	if (gain > 1.0) {f[6] = 1.0; gain -= 1.0;} else {f[6] = gain; gain = 0.0;}
	if (gain > 1.0) {f[7] = 1.0; gain -= 1.0;} else {f[7] = gain; gain = 0.0;}
	if (gain > 1.0) {f[8] = 1.0; gain -= 1.0;} else {f[8] = gain; gain = 0.0;}
	if (gain > 1.0) {f[9] = 1.0; gain -= 1.0;} else {f[9] = gain; gain = 0.0;}
	//there, now we have a neat little moving average with remainders
	
	if (overallscale < 1.0) overallscale = 1.0;
	f[0] /= overallscale;
	f[1] /= overallscale;
	f[2] /= overallscale;
	f[3] /= overallscale;
	f[4] /= overallscale;
	f[5] /= overallscale;
	f[6] /= overallscale;
	f[7] /= overallscale;
	f[8] /= overallscale;
	f[9] /= overallscale;
	//and now it's neatly scaled, too
	
	
	
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		if (surgeL<fabs(inputSampleL))
		{
			surgeL += (double(fpdL)/UINT32_MAX)*(fabs(inputSampleL)-surgeL);
			if (surgeL > 1.0) surgeL = 1.0;
		}
		else
		{
			surgeL -= ((double(fpdL)/UINT32_MAX)*(surgeL-fabs(inputSampleL))*decay);
			if (surgeL < 0.0) surgeL = 0.0;
		}
		
		cutoffL = pow((cutofftarget*surgeL),5);
		if (cutoffL > 1.0) cutoffL = 1.0;
		invcutoffL = 1.0 - cutoffL;
		//set up modified cutoff L
		
		if (surgeR<fabs(inputSampleR))
		{
			surgeR += (double(fpdR)/UINT32_MAX)*(fabs(inputSampleR)-surgeR);
			if (surgeR > 1.0) surgeR = 1.0;
		}
		else
		{
			surgeR -= ((double(fpdR)/UINT32_MAX)*(surgeR-fabs(inputSampleR))*decay);
			if (surgeR < 0.0) surgeR = 0.0;
		}
		
		cutoffR = pow((cutofftarget*surgeR),5);
		if (cutoffR > 1.0) cutoffR = 1.0;
		invcutoffR = 1.0 - cutoffR;
		//set up modified cutoff R
		
		flipL = !flipL;
		flipR = !flipR;
		filterflip = !filterflip;
		quadratic -= 1;
		if (quadratic < 0)
		{
			position += 1;		
			quadratic = position * position;
			quadratic = quadratic % 170003; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % 17011; //% is C++ mod operator
			quadratic *= quadratic;
			//quadratic = quadratic % 1709; //% is C++ mod operator
			//quadratic *= quadratic;
			quadratic = quadratic % dcut; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % lowcut;
			//sets density of the centering force
			if (noiseAL < 0) {flipL = true;}
			else {flipL = false;}
			if (noiseAR < 0) {flipR = true;}
			else {flipR = false;}
		}
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		
		if (flipL) noiseAL += (double(fpdL)/UINT32_MAX);
		else noiseAL -= (double(fpdL)/UINT32_MAX);
		if (flipR) noiseAR += (double(fpdR)/UINT32_MAX);
		else noiseAR -= (double(fpdR)/UINT32_MAX);
		
		if (filterflip)
		{
			noiseBL *= invcutoffL; noiseBL += (noiseAL*cutoffL);
			inputSampleL = noiseBL+noiseCL;
			rumbleAL *= (1.0-rumblecutoff);
			rumbleAL += (inputSampleL*rumblecutoff);
			
			noiseBR *= invcutoffR; noiseBR += (noiseAR*cutoffR);
			inputSampleR = noiseBR+noiseCR;
			rumbleAR *= (1.0-rumblecutoff);
			rumbleAR += (inputSampleR*rumblecutoff);
		}
		else 
		{
			noiseCL *= invcutoffL; noiseCL += (noiseAL*cutoffL);
			inputSampleL = noiseBL+noiseCL;
			rumbleBL *= (1.0-rumblecutoff);
			rumbleBL += (inputSampleL*rumblecutoff);
			
			noiseCR *= invcutoffR; noiseCR += (noiseAR*cutoffR);
			inputSampleR = noiseBR+noiseCR;
			rumbleBR *= (1.0-rumblecutoff);
			rumbleBR += (inputSampleR*rumblecutoff);
		}
		
		inputSampleL -= (rumbleAL+rumbleBL);
		inputSampleL *= (1.0-rumblecutoff);

		inputSampleR -= (rumbleAR+rumbleBR);
		inputSampleR *= (1.0-rumblecutoff);

		inputSampleL *= wet;
		inputSampleL += (drySampleL * (1.0-wet));
		
		inputSampleR *= wet;
		inputSampleR += (drySampleR * (1.0-wet));
		//apply the dry to the noise
		
		bL[9] = bL[8]; bL[8] = bL[7]; bL[7] = bL[6]; bL[6] = bL[5];
		bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1];
		bL[1] = bL[0]; bL[0] = accumulatorSampleL = inputSampleL;
		
		bR[9] = bR[8]; bR[8] = bR[7]; bR[7] = bR[6]; bR[6] = bR[5];
		bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1];
		bR[1] = bR[0]; bR[0] = accumulatorSampleR = inputSampleR;
		
		accumulatorSampleL *= f[0];
		accumulatorSampleL += (bL[1] * f[1]);
		accumulatorSampleL += (bL[2] * f[2]);
		accumulatorSampleL += (bL[3] * f[3]);
		accumulatorSampleL += (bL[4] * f[4]);
		accumulatorSampleL += (bL[5] * f[5]);
		accumulatorSampleL += (bL[6] * f[6]);
		accumulatorSampleL += (bL[7] * f[7]);
		accumulatorSampleL += (bL[8] * f[8]);
		accumulatorSampleL += (bL[9] * f[9]);
		//we are doing our repetitive calculations on a separate value
		accumulatorSampleR *= f[0];
		accumulatorSampleR += (bR[1] * f[1]);
		accumulatorSampleR += (bR[2] * f[2]);
		accumulatorSampleR += (bR[3] * f[3]);
		accumulatorSampleR += (bR[4] * f[4]);
		accumulatorSampleR += (bR[5] * f[5]);
		accumulatorSampleR += (bR[6] * f[6]);
		accumulatorSampleR += (bR[7] * f[7]);
		accumulatorSampleR += (bR[8] * f[8]);
		accumulatorSampleR += (bR[9] * f[9]);
		//we are doing our repetitive calculations on a separate value
		
		correctionSample = inputSampleL - accumulatorSampleL;
		//we're gonna apply the total effect of all these calculations as a single subtract
		//(formerly a more complicated algorithm)
		inputSampleL -= correctionSample;
		//applying the distance calculation to both the dry AND the noise output to blend them
		correctionSample = inputSampleR - accumulatorSampleR;
		//we're gonna apply the total effect of all these calculations as a single subtract
		//(formerly a more complicated algorithm)
		inputSampleR -= correctionSample;
		//applying the distance calculation to both the dry AND the noise output to blend them
		//sometimes I'm really tired and can't do stuff, and I remember trying to simplify this
		//and breaking it somehow. So, there ya go, strange obtuse code.
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Noise::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double cutoffL;
	double cutoffR;
	double cutofftarget = (A*3.5);
	double rumblecutoff = cutofftarget * 0.005;
	double invcutoffL;
	double invcutoffR;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	double highpass = C*38.0;
	int lowcut = floor(highpass);
	int dcut;
	if (lowcut > 37) {dcut= 1151;}
	if (lowcut == 37) {dcut= 1091;}
	if (lowcut == 36) {dcut= 1087;}
	if (lowcut == 35) {dcut= 1031;}
	if (lowcut == 34) {dcut= 1013;}
	if (lowcut == 33) {dcut= 971;}
	if (lowcut == 32) {dcut= 907;}
	if (lowcut == 31) {dcut= 839;}
	if (lowcut == 30) {dcut= 797;}
	if (lowcut == 29) {dcut= 733;}
	if (lowcut == 28) {dcut= 719;}
	if (lowcut == 27) {dcut= 673;}
	if (lowcut == 26) {dcut= 613;}
	if (lowcut == 25) {dcut= 593;}
	if (lowcut == 24) {dcut= 541;}
	if (lowcut == 23) {dcut= 479;}
	if (lowcut == 22) {dcut= 431;}
	if (lowcut == 21) {dcut= 419;}
	if (lowcut == 20) {dcut= 373;}
	if (lowcut == 19) {dcut= 311;}
	if (lowcut == 18) {dcut= 293;}
	if (lowcut == 17) {dcut= 233;}
	if (lowcut == 16) {dcut= 191;}
	if (lowcut == 15) {dcut= 173;}
	if (lowcut == 14) {dcut= 131;}
	if (lowcut == 13) {dcut= 113;}
	if (lowcut == 12) {dcut= 71;}
	if (lowcut == 11) {dcut= 53;}
	if (lowcut == 10) {dcut= 31;}
	if (lowcut == 9) {dcut= 27;}
	if (lowcut == 8) {dcut= 23;}
	if (lowcut == 7) {dcut= 19;}
	if (lowcut == 6) {dcut= 17;}
	if (lowcut == 5) {dcut= 13;}
	if (lowcut == 4) {dcut= 11;}
	if (lowcut == 3) {dcut= 7;}
	if (lowcut == 2) {dcut= 5;}
	if (lowcut < 2) {dcut= 3;}
	highpass = B * 22.0;
	lowcut = floor(highpass)+1;
	
	double decay = 0.001 - ((1.0-pow(1.0-D,3))*0.001);
	if (decay == 0.001) decay = 0.1;
	double wet = F;
	//removed extra dry variable
	wet *= 0.01; //correct large gain issue
	double correctionSample;
	double accumulatorSampleL;
	double accumulatorSampleR;
	double overallscale = (E*9.0)+1.0;
	double gain = overallscale;
	
	if (gain > 1.0) {f[0] = 1.0; gain -= 1.0;} else {f[0] = gain; gain = 0.0;}
	if (gain > 1.0) {f[1] = 1.0; gain -= 1.0;} else {f[1] = gain; gain = 0.0;}
	if (gain > 1.0) {f[2] = 1.0; gain -= 1.0;} else {f[2] = gain; gain = 0.0;}
	if (gain > 1.0) {f[3] = 1.0; gain -= 1.0;} else {f[3] = gain; gain = 0.0;}
	if (gain > 1.0) {f[4] = 1.0; gain -= 1.0;} else {f[4] = gain; gain = 0.0;}
	if (gain > 1.0) {f[5] = 1.0; gain -= 1.0;} else {f[5] = gain; gain = 0.0;}
	if (gain > 1.0) {f[6] = 1.0; gain -= 1.0;} else {f[6] = gain; gain = 0.0;}
	if (gain > 1.0) {f[7] = 1.0; gain -= 1.0;} else {f[7] = gain; gain = 0.0;}
	if (gain > 1.0) {f[8] = 1.0; gain -= 1.0;} else {f[8] = gain; gain = 0.0;}
	if (gain > 1.0) {f[9] = 1.0; gain -= 1.0;} else {f[9] = gain; gain = 0.0;}
	//there, now we have a neat little moving average with remainders
	
	if (overallscale < 1.0) overallscale = 1.0;
	f[0] /= overallscale;
	f[1] /= overallscale;
	f[2] /= overallscale;
	f[3] /= overallscale;
	f[4] /= overallscale;
	f[5] /= overallscale;
	f[6] /= overallscale;
	f[7] /= overallscale;
	f[8] /= overallscale;
	f[9] /= overallscale;
	//and now it's neatly scaled, too
	
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		if (surgeL<fabs(inputSampleL))
		{
			surgeL += (double(fpdL)/UINT32_MAX)*(fabs(inputSampleL)-surgeL);
			if (surgeL > 1.0) surgeL = 1.0;
		}
		else
		{
			surgeL -= ((double(fpdL)/UINT32_MAX)*(surgeL-fabs(inputSampleL))*decay);
			if (surgeL < 0.0) surgeL = 0.0;
		}
		
		cutoffL = pow((cutofftarget*surgeL),5);
		if (cutoffL > 1.0) cutoffL = 1.0;
		invcutoffL = 1.0 - cutoffL;
		//set up modified cutoff L
		
		if (surgeR<fabs(inputSampleR))
		{
			surgeR += (double(fpdR)/UINT32_MAX)*(fabs(inputSampleR)-surgeR);
			if (surgeR > 1.0) surgeR = 1.0;
		}
		else
		{
			surgeR -= ((double(fpdR)/UINT32_MAX)*(surgeR-fabs(inputSampleR))*decay);
			if (surgeR < 0.0) surgeR = 0.0;
		}
		
		cutoffR = pow((cutofftarget*surgeR),5);
		if (cutoffR > 1.0) cutoffR = 1.0;
		invcutoffR = 1.0 - cutoffR;
		//set up modified cutoff R
		
		flipL = !flipL;
		flipR = !flipR;
		filterflip = !filterflip;
		quadratic -= 1;
		if (quadratic < 0)
		{
			position += 1;		
			quadratic = position * position;
			quadratic = quadratic % 170003; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % 17011; //% is C++ mod operator
			quadratic *= quadratic;
			//quadratic = quadratic % 1709; //% is C++ mod operator
			//quadratic *= quadratic;
			quadratic = quadratic % dcut; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % lowcut;
			//sets density of the centering force
			if (noiseAL < 0) {flipL = true;}
			else {flipL = false;}
			if (noiseAR < 0) {flipR = true;}
			else {flipR = false;}
		}
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		
		if (flipL) noiseAL += (double(fpdL)/UINT32_MAX);
		else noiseAL -= (double(fpdL)/UINT32_MAX);
		if (flipR) noiseAR += (double(fpdR)/UINT32_MAX);
		else noiseAR -= (double(fpdR)/UINT32_MAX);
		
		if (filterflip)
		{
			noiseBL *= invcutoffL; noiseBL += (noiseAL*cutoffL);
			inputSampleL = noiseBL+noiseCL;
			rumbleAL *= (1.0-rumblecutoff);
			rumbleAL += (inputSampleL*rumblecutoff);
			
			noiseBR *= invcutoffR; noiseBR += (noiseAR*cutoffR);
			inputSampleR = noiseBR+noiseCR;
			rumbleAR *= (1.0-rumblecutoff);
			rumbleAR += (inputSampleR*rumblecutoff);
		}
		else 
		{
			noiseCL *= invcutoffL; noiseCL += (noiseAL*cutoffL);
			inputSampleL = noiseBL+noiseCL;
			rumbleBL *= (1.0-rumblecutoff);
			rumbleBL += (inputSampleL*rumblecutoff);
			
			noiseCR *= invcutoffR; noiseCR += (noiseAR*cutoffR);
			inputSampleR = noiseBR+noiseCR;
			rumbleBR *= (1.0-rumblecutoff);
			rumbleBR += (inputSampleR*rumblecutoff);
		}
		
		inputSampleL -= (rumbleAL+rumbleBL);
		inputSampleL *= (1.0-rumblecutoff);
		
		inputSampleR -= (rumbleAR+rumbleBR);
		inputSampleR *= (1.0-rumblecutoff);
		
		inputSampleL *= wet;
		inputSampleL += (drySampleL * (1.0-wet));
		
		inputSampleR *= wet;
		inputSampleR += (drySampleR * (1.0-wet));
		//apply the dry to the noise
		
		bL[9] = bL[8]; bL[8] = bL[7]; bL[7] = bL[6]; bL[6] = bL[5];
		bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1];
		bL[1] = bL[0]; bL[0] = accumulatorSampleL = inputSampleL;
		
		bR[9] = bR[8]; bR[8] = bR[7]; bR[7] = bR[6]; bR[6] = bR[5];
		bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1];
		bR[1] = bR[0]; bR[0] = accumulatorSampleR = inputSampleR;
		
		accumulatorSampleL *= f[0];
		accumulatorSampleL += (bL[1] * f[1]);
		accumulatorSampleL += (bL[2] * f[2]);
		accumulatorSampleL += (bL[3] * f[3]);
		accumulatorSampleL += (bL[4] * f[4]);
		accumulatorSampleL += (bL[5] * f[5]);
		accumulatorSampleL += (bL[6] * f[6]);
		accumulatorSampleL += (bL[7] * f[7]);
		accumulatorSampleL += (bL[8] * f[8]);
		accumulatorSampleL += (bL[9] * f[9]);
		//we are doing our repetitive calculations on a separate value
		accumulatorSampleR *= f[0];
		accumulatorSampleR += (bR[1] * f[1]);
		accumulatorSampleR += (bR[2] * f[2]);
		accumulatorSampleR += (bR[3] * f[3]);
		accumulatorSampleR += (bR[4] * f[4]);
		accumulatorSampleR += (bR[5] * f[5]);
		accumulatorSampleR += (bR[6] * f[6]);
		accumulatorSampleR += (bR[7] * f[7]);
		accumulatorSampleR += (bR[8] * f[8]);
		accumulatorSampleR += (bR[9] * f[9]);
		//we are doing our repetitive calculations on a separate value
		
		correctionSample = inputSampleL - accumulatorSampleL;
		//we're gonna apply the total effect of all these calculations as a single subtract
		//(formerly a more complicated algorithm)
		inputSampleL -= correctionSample;
		//applying the distance calculation to both the dry AND the noise output to blend them
		correctionSample = inputSampleR - accumulatorSampleR;
		//we're gonna apply the total effect of all these calculations as a single subtract
		//(formerly a more complicated algorithm)
		inputSampleR -= correctionSample;
		//applying the distance calculation to both the dry AND the noise output to blend them
		//sometimes I'm really tired and can't do stuff, and I remember trying to simplify this
		//and breaking it somehow. So, there ya go, strange obtuse code.
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}