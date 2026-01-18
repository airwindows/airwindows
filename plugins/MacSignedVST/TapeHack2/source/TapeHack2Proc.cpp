/* ========================================
 *  TapeHack2 - TapeHack2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TapeHack2_H
#include "TapeHack2.h"
#endif

void TapeHack2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale*2.0);
	if (spacing < 2) spacing = 2; if (spacing > 32) spacing = 32;
	
	double inputGain = A*10.0;
	double outputGain = B*0.9239;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		inputSampleL *= inputGain;
		inputSampleR *= inputGain;
		double darkSampleL = inputSampleL;
		double darkSampleR = inputSampleR;
		if (avgPos > 31) avgPos = 0;
		if (spacing > 31) {
			avg32L[avgPos] = darkSampleL; avg32R[avgPos] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 32; x++) {darkSampleL += avg32L[x]; darkSampleR += avg32R[x];}
			darkSampleL /= 32.0; darkSampleR /= 32.0;
		} if (spacing > 15) {
			avg16L[avgPos%16] = darkSampleL; avg16R[avgPos%16] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 16; x++) {darkSampleL += avg16L[x]; darkSampleR += avg16R[x];}
			darkSampleL /= 16.0; darkSampleR /= 16.0;
		} if (spacing > 7) {
			avg8L[avgPos%8] = darkSampleL; avg8R[avgPos%8] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 8; x++) {darkSampleL += avg8L[x]; darkSampleR += avg8R[x];}
			darkSampleL /= 8.0; darkSampleR /= 8.0;
		} if (spacing > 3) {
			avg4L[avgPos%4] = darkSampleL; avg4R[avgPos%4] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 4; x++) {darkSampleL += avg4L[x]; darkSampleR += avg4R[x];}
			darkSampleL /= 4.0; darkSampleR /= 4.0;
		} if (spacing > 1) {
			avg2L[avgPos%2] = darkSampleL; avg2R[avgPos%2] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 2; x++) {darkSampleL += avg2L[x]; darkSampleR += avg2R[x];}
			darkSampleL /= 2.0; darkSampleR /= 2.0; 
		} //only update avgPos after the post-distortion filter stage
		double avgSlewL = fmin(fabs(lastDarkL-inputSampleL)*0.12*overallscale,1.0);
		avgSlewL = 1.0-(1.0-avgSlewL*1.0-avgSlewL);
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		lastDarkL = darkSampleL;
		double avgSlewR = fmin(fabs(lastDarkR-inputSampleR)*0.12*overallscale,1.0);
		avgSlewR = 1.0-(1.0-avgSlewR*1.0-avgSlewR);
		inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
		lastDarkR = darkSampleR;
		
		inputSampleL = fmax(fmin(inputSampleL,2.305929007734908),-2.305929007734908);
		double addtwo = inputSampleL * inputSampleL;
		double empower = inputSampleL * addtwo; // inputSampleL to the third power
		inputSampleL -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleL += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleL -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleL += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleL -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleR = fmax(fmin(inputSampleR,2.305929007734908),-2.305929007734908);
		addtwo = inputSampleR * inputSampleR;
		empower = inputSampleR * addtwo; // inputSampleR to the third power
		inputSampleR -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleR += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleR -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleR += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleR -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		
		darkSampleL = inputSampleL;
		darkSampleR = inputSampleR;
		if (avgPos > 31) avgPos = 0;
		if (spacing > 31) {
			post32L[avgPos] = darkSampleL; post32R[avgPos] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 32; x++) {darkSampleL += post32L[x]; darkSampleR += post32R[x];}
			darkSampleL /= 32.0; darkSampleR /= 32.0;
		} if (spacing > 15) {
			post16L[avgPos%16] = darkSampleL; post16R[avgPos%16] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 16; x++) {darkSampleL += post16L[x]; darkSampleR += post16R[x];}
			darkSampleL /= 16.0; darkSampleR /= 16.0;
		} if (spacing > 7) {
			post8L[avgPos%8] = darkSampleL; post8R[avgPos%8] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 8; x++) {darkSampleL += post8L[x]; darkSampleR += post8R[x];}
			darkSampleL /= 8.0; darkSampleR /= 8.0;
		} if (spacing > 3) {
			post4L[avgPos%4] = darkSampleL; post4R[avgPos%4] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 4; x++) {darkSampleL += post4L[x]; darkSampleR += post4R[x];}
			darkSampleL /= 4.0; darkSampleR /= 4.0;
		} if (spacing > 1) {
			post2L[avgPos%2] = darkSampleL; post2R[avgPos%2] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 2; x++) {darkSampleL += post2L[x]; darkSampleR += post2R[x];}
			darkSampleL /= 2.0; darkSampleR /= 2.0; 
		} avgPos++;
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
		//use the previously calculated depth of the filter
		
		inputSampleL = (inputSampleL * outputGain * wet) + (drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * outputGain * wet) + (drySampleR * (1.0-wet));
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void TapeHack2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale*2.0);
	if (spacing < 2) spacing = 2; if (spacing > 32) spacing = 32;

	double inputGain = A*10.0;
	double outputGain = B*0.9239;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL *= inputGain;
		inputSampleR *= inputGain;
		double darkSampleL = inputSampleL;
		double darkSampleR = inputSampleR;
		if (avgPos > 31) avgPos = 0;
		if (spacing > 31) {
			avg32L[avgPos] = darkSampleL; avg32R[avgPos] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 32; x++) {darkSampleL += avg32L[x]; darkSampleR += avg32R[x];}
			darkSampleL /= 32.0; darkSampleR /= 32.0;
		} if (spacing > 15) {
			avg16L[avgPos%16] = darkSampleL; avg16R[avgPos%16] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 16; x++) {darkSampleL += avg16L[x]; darkSampleR += avg16R[x];}
			darkSampleL /= 16.0; darkSampleR /= 16.0;
		} if (spacing > 7) {
			avg8L[avgPos%8] = darkSampleL; avg8R[avgPos%8] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 8; x++) {darkSampleL += avg8L[x]; darkSampleR += avg8R[x];}
			darkSampleL /= 8.0; darkSampleR /= 8.0;
		} if (spacing > 3) {
			avg4L[avgPos%4] = darkSampleL; avg4R[avgPos%4] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 4; x++) {darkSampleL += avg4L[x]; darkSampleR += avg4R[x];}
			darkSampleL /= 4.0; darkSampleR /= 4.0;
		} if (spacing > 1) {
			avg2L[avgPos%2] = darkSampleL; avg2R[avgPos%2] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 2; x++) {darkSampleL += avg2L[x]; darkSampleR += avg2R[x];}
			darkSampleL /= 2.0; darkSampleR /= 2.0; 
		} //only update avgPos after the post-distortion filter stage
		double avgSlewL = fmin(fabs(lastDarkL-inputSampleL)*0.12*overallscale,1.0);
		avgSlewL = 1.0-(1.0-avgSlewL*1.0-avgSlewL);
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		lastDarkL = darkSampleL;
		double avgSlewR = fmin(fabs(lastDarkR-inputSampleR)*0.12*overallscale,1.0);
		avgSlewR = 1.0-(1.0-avgSlewR*1.0-avgSlewR);
		inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
		lastDarkR = darkSampleR;
		
		inputSampleL = fmax(fmin(inputSampleL,2.305929007734908),-2.305929007734908);
		double addtwo = inputSampleL * inputSampleL;
		double empower = inputSampleL * addtwo; // inputSampleL to the third power
		inputSampleL -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleL += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleL -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleL += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleL -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleR = fmax(fmin(inputSampleR,2.305929007734908),-2.305929007734908);
		addtwo = inputSampleR * inputSampleR;
		empower = inputSampleR * addtwo; // inputSampleR to the third power
		inputSampleR -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleR += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleR -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleR += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleR -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		
		
		darkSampleL = inputSampleL;
		darkSampleR = inputSampleR;
		if (avgPos > 31) avgPos = 0;
		if (spacing > 31) {
			post32L[avgPos] = darkSampleL; post32R[avgPos] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 32; x++) {darkSampleL += post32L[x]; darkSampleR += post32R[x];}
			darkSampleL /= 32.0; darkSampleR /= 32.0;
		} if (spacing > 15) {
			post16L[avgPos%16] = darkSampleL; post16R[avgPos%16] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 16; x++) {darkSampleL += post16L[x]; darkSampleR += post16R[x];}
			darkSampleL /= 16.0; darkSampleR /= 16.0;
		} if (spacing > 7) {
			post8L[avgPos%8] = darkSampleL; post8R[avgPos%8] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 8; x++) {darkSampleL += post8L[x]; darkSampleR += post8R[x];}
			darkSampleL /= 8.0; darkSampleR /= 8.0;
		} if (spacing > 3) {
			post4L[avgPos%4] = darkSampleL; post4R[avgPos%4] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 4; x++) {darkSampleL += post4L[x]; darkSampleR += post4R[x];}
			darkSampleL /= 4.0; darkSampleR /= 4.0;
		} if (spacing > 1) {
			post2L[avgPos%2] = darkSampleL; post2R[avgPos%2] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 2; x++) {darkSampleL += post2L[x]; darkSampleR += post2R[x];}
			darkSampleL /= 2.0; darkSampleR /= 2.0; 
		} avgPos++;
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
		//use the previously calculated depth of the filter
		
		inputSampleL = (inputSampleL * outputGain * wet) + (drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * outputGain * wet) + (drySampleR * (1.0-wet));
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
