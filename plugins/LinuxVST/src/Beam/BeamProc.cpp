/* ========================================
 *  Beam - Beam.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Beam_H
#include "Beam.h"
#endif

void Beam::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	int processing = (VstInt32)( A * 1.999 );
	float sonority = B * 1.618033988749894848204586;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int depth = (int)(17.0*overallscale);
	if (depth < 3) depth = 3;
	if (depth > 98) depth = 98;
	bool highres = false;
	if (processing == 1) highres = true;
	float scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	float derez = C;
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	float outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		
		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		//We are doing it first Left, then Right, because the loops may run faster if
		//they aren't too jammed full of variables. This means re-running code.
		
		//begin left
		int quantA = floor(inputSampleL);
		int quantB = floor(inputSampleL+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		float expectedSlewA = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlewA += (lastSampleL[x+1] - lastSampleL[x]);
		}
		float expectedSlewB = expectedSlewA;
		expectedSlewA += (lastSampleL[0] - quantA);
		expectedSlewB += (lastSampleL[0] - quantB);
		//now we have a collection of all slews, averaged and left at total scale
		
		float clamp = sonority;
		if (fabs(inputSampleL) < sonority) clamp = fabs(inputSampleL);

		float testA = fabs(fabs(expectedSlewA)-clamp);
		float testB = fabs(fabs(expectedSlewB)-clamp);
		//doing this means the result will be lowest when it's reaching the target slope across
		//the desired time range, either positively or negatively. Should produce the same target
		//at whatever sample rate, as high rate stuff produces smaller increments.
		
		if (testA < testB) inputSampleL = quantA;
		else inputSampleL = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			lastSampleL[x+1] = lastSampleL[x];
		}
		lastSampleL[0] = inputSampleL;
		//end left
		
		//begin right
		quantA = floor(inputSampleR);
		quantB = floor(inputSampleR+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		expectedSlewA = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlewA += (lastSampleR[x+1] - lastSampleR[x]);
		}
		expectedSlewB = expectedSlewA;
		expectedSlewA += (lastSampleR[0] - quantA);
		expectedSlewB += (lastSampleR[0] - quantB);
		//now we have a collection of all slews, averaged and left at total scale
		
		clamp = sonority;
		if (fabs(inputSampleR) < sonority) clamp = fabs(inputSampleR);

		testA = fabs(fabs(expectedSlewA)-clamp);
		testB = fabs(fabs(expectedSlewB)-clamp);
		//doing this means the result will be lowest when it's reaching the target slope across
		//the desired time range, either positively or negatively. Should produce the same target
		//at whatever sample rate, as high rate stuff produces smaller increments.
		
		if (testA < testB) inputSampleR = quantA;
		else inputSampleR = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			lastSampleR[x+1] = lastSampleR[x];
		}
		lastSampleR[0] = inputSampleR;
		//end right
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Beam::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	int processing = (VstInt32)( A * 1.999 );
	float sonority = B * 1.618033988749894848204586;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int depth = (int)(17.0*overallscale);
	if (depth < 3) depth = 3;
	if (depth > 98) depth = 98;
	bool highres = false;
	if (processing == 1) highres = true;
	float scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	float derez = C;
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	float outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		
		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither

		//We are doing it first Left, then Right, because the loops may run faster if
		//they aren't too jammed full of variables. This means re-running code.
		
		//begin left
		int quantA = floor(inputSampleL);
		int quantB = floor(inputSampleL+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		float expectedSlewA = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlewA += (lastSampleL[x+1] - lastSampleL[x]);
		}
		float expectedSlewB = expectedSlewA;
		expectedSlewA += (lastSampleL[0] - quantA);
		expectedSlewB += (lastSampleL[0] - quantB);
		//now we have a collection of all slews, averaged and left at total scale
		
		float clamp = sonority;
		if (fabs(inputSampleL) < sonority) clamp = fabs(inputSampleL);

		float testA = fabs(fabs(expectedSlewA)-clamp);
		float testB = fabs(fabs(expectedSlewB)-clamp);
		//doing this means the result will be lowest when it's reaching the target slope across
		//the desired time range, either positively or negatively. Should produce the same target
		//at whatever sample rate, as high rate stuff produces smaller increments.
		
		if (testA < testB) inputSampleL = quantA;
		else inputSampleL = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			lastSampleL[x+1] = lastSampleL[x];
		}
		lastSampleL[0] = inputSampleL;
		//end left
		
		//begin right
		quantA = floor(inputSampleR);
		quantB = floor(inputSampleR+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		expectedSlewA = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlewA += (lastSampleR[x+1] - lastSampleR[x]);
		}
		expectedSlewB = expectedSlewA;
		expectedSlewA += (lastSampleR[0] - quantA);
		expectedSlewB += (lastSampleR[0] - quantB);
		//now we have a collection of all slews, averaged and left at total scale
		
		clamp = sonority;
		if (fabs(inputSampleR) < sonority) clamp = fabs(inputSampleR);
		
		testA = fabs(fabs(expectedSlewA)-clamp);
		testB = fabs(fabs(expectedSlewB)-clamp);
		//doing this means the result will be lowest when it's reaching the target slope across
		//the desired time range, either positively or negatively. Should produce the same target
		//at whatever sample rate, as high rate stuff produces smaller increments.
		
		if (testA < testB) inputSampleR = quantA;
		else inputSampleR = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			lastSampleR[x+1] = lastSampleR[x];
		}
		lastSampleR[0] = inputSampleR;
		//end right
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
