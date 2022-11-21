/* ========================================
 *  ZLowpass - ZLowpass.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ZLowpass_H
#include "ZLowpass.h"
#endif

void ZLowpass::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	biquadA[0] = ((pow(B,3)*18930.0)/getSampleRate())+0.00162;
	double clipFactor = 1.212-((1.0-B)*0.496);
	
    biquadA[1] = 0.7071;
	double K = tan(M_PI * biquadA[0]);
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) {biquadD[x] = biquadC[x] = biquadB[x] = biquadA[x];}
	
	//opamp stuff
	double inTrim = A*10.0;
	inTrim *= inTrim; inTrim *= inTrim;
	double outPad = C*10.0;
	double iirAmountA = 0.00069/overallscale;
	biquadF[0] = biquadE[0] = 15500.0 / getSampleRate();
    biquadF[1] = biquadE[1] = 0.935;
	K = tan(M_PI * biquadE[0]); //lowpass
	norm = 1.0 / (1.0 + K / biquadE[1] + K * K);
	biquadE[2] = K * K * norm;
	biquadE[3] = 2.0 * biquadE[2];
	biquadE[4] = biquadE[2];
	biquadE[5] = 2.0 * (K * K - 1.0) * norm;
	biquadE[6] = (1.0 - K / biquadE[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) biquadF[x] = biquadE[x];
	//end opamp stuff	
	
 	double trim = 0.1+(3.712*biquadA[0]);
	double wet = pow(D,2);
	double aWet = 1.0;
	double bWet = 1.0;
	double cWet = 1.0;
	double dWet = wet*4.0;
	//four-stage wet/dry control using progressive stages that bypass when not engaged
	if (dWet < 1.0) {aWet = dWet; bWet = 0.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 2.0) {bWet = dWet - 1.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 3.0) {cWet = dWet - 2.0; dWet = 0.0;}
	else {dWet -= 3.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	double outSample = 0.0;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double overallDrySampleL = drySampleL;
		double overallDrySampleR = drySampleR;
		
		if (inTrim != 1.0) {inputSampleL *= inTrim; inputSampleR *= inTrim;}
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleL *= trim; inputSampleR *= trim;
		
		inputSampleL /= clipFactor;
		outSample = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; biquadA[10] = biquadA[9];
		if (outSample > 1.0) outSample = 1.0;
		if (outSample < -1.0) outSample = -1.0;
		drySampleL = inputSampleL = biquadA[9] = outSample; //DF1
		
		inputSampleR /= clipFactor;
		outSample = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
		biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; biquadA[14] = biquadA[13];
		if (outSample > 1.0) outSample = 1.0;
		if (outSample < -1.0) outSample = -1.0;
		drySampleR = inputSampleR = biquadA[13] = outSample; //DF1
		
		if (bWet > 0.0) {
			inputSampleL /= clipFactor;
			outSample = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; biquadB[10] = biquadB[9];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadB[9] = outSample; //DF1
			drySampleL = inputSampleL = (outSample * bWet) + (drySampleL * (1.0-bWet));
			
			inputSampleR /= clipFactor;
			outSample = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
			biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; biquadB[14] = biquadB[13];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadB[13] = outSample; //DF1
			drySampleR = inputSampleR = (outSample * bWet) + (drySampleR * (1.0-bWet));
		}
		if (cWet > 0.0) {
			inputSampleL /= clipFactor;
			outSample = biquadC[2]*inputSampleL+biquadC[3]*biquadC[7]+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
			biquadC[8] = biquadC[7]; biquadC[7] = inputSampleL; biquadC[10] = biquadC[9];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadC[9] = outSample; //DF1
			drySampleL = inputSampleL = (outSample * cWet) + (drySampleL * (1.0-cWet));
			
			inputSampleR /= clipFactor;
			outSample = biquadC[2]*inputSampleR+biquadC[3]*biquadC[11]+biquadC[4]*biquadC[12]-biquadC[5]*biquadC[13]-biquadC[6]*biquadC[14];
			biquadC[12] = biquadC[11]; biquadC[11] = inputSampleR; biquadC[14] = biquadC[13];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadC[13] = outSample; //DF1
			drySampleR = inputSampleR = (outSample * cWet) + (drySampleR * (1.0-cWet));
		}
		if (dWet > 0.0) {
			inputSampleL /= clipFactor;
			outSample = biquadD[2]*inputSampleL+biquadD[3]*biquadD[7]+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
			biquadD[8] = biquadD[7]; biquadD[7] = inputSampleL; biquadD[10] = biquadD[9];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadD[9] = outSample; //DF1
			drySampleL = inputSampleL = (outSample * dWet) + (drySampleL * (1.0-dWet));
			
			inputSampleR /= clipFactor;
			outSample = biquadD[2]*inputSampleR+biquadD[3]*biquadD[11]+biquadD[4]*biquadD[12]-biquadD[5]*biquadD[13]-biquadD[6]*biquadD[14];
			biquadD[12] = biquadD[11]; biquadD[11] = inputSampleR; biquadD[14] = biquadD[13];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadD[13] = outSample; //DF1
			drySampleR = inputSampleR = (outSample * dWet) + (drySampleR * (1.0-dWet));
		}
		
		inputSampleL /= clipFactor; inputSampleR /= clipFactor;
		//opamp stage
		if (fabs(iirSampleAL)<1.18e-37) iirSampleAL = 0.0;
		iirSampleAL = (iirSampleAL * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
		inputSampleL -= iirSampleAL;
		if (fabs(iirSampleAR)<1.18e-37) iirSampleAR = 0.0;
		iirSampleAR = (iirSampleAR * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
		inputSampleR -= iirSampleAR;
		
		outSample = biquadE[2]*inputSampleL+biquadE[3]*biquadE[7]+biquadE[4]*biquadE[8]-biquadE[5]*biquadE[9]-biquadE[6]*biquadE[10];
		biquadE[8] = biquadE[7]; biquadE[7] = inputSampleL; inputSampleL = outSample; biquadE[10] = biquadE[9]; biquadE[9] = inputSampleL; //DF1 left
		
		outSample = biquadE[2]*inputSampleR+biquadE[3]*biquadE[11]+biquadE[4]*biquadE[12]-biquadE[5]*biquadE[13]-biquadE[6]*biquadE[14];
		biquadE[12] = biquadE[11]; biquadE[11] = inputSampleR; inputSampleR = outSample; biquadE[14] = biquadE[13]; biquadE[13] = inputSampleR; //DF1 right
		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL -= (inputSampleL*inputSampleL*inputSampleL*inputSampleL*inputSampleL*0.1768);
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR -= (inputSampleR*inputSampleR*inputSampleR*inputSampleR*inputSampleR*0.1768);
		
		outSample = biquadF[2]*inputSampleL+biquadF[3]*biquadF[7]+biquadF[4]*biquadF[8]-biquadF[5]*biquadF[9]-biquadF[6]*biquadF[10];
		biquadF[8] = biquadF[7]; biquadF[7] = inputSampleL; inputSampleL = outSample; biquadF[10] = biquadF[9]; biquadF[9] = inputSampleL; //DF1 left
		
		outSample = biquadF[2]*inputSampleR+biquadF[3]*biquadF[11]+biquadF[4]*biquadF[12]-biquadF[5]*biquadF[13]-biquadF[6]*biquadF[14];
		biquadF[12] = biquadF[11]; biquadF[11] = inputSampleR; inputSampleR = outSample; biquadF[14] = biquadF[13]; biquadF[13] = inputSampleR; //DF1 right
		
		if (outPad != 1.0) {inputSampleL *= outPad; inputSampleR *= outPad;}		
		//end opamp stage
		
		if (aWet !=1.0) {
			inputSampleL = (inputSampleL * aWet) + (overallDrySampleL * (1.0-aWet));
			inputSampleR = (inputSampleR * aWet) + (overallDrySampleR * (1.0-aWet));
		}	
		
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

void ZLowpass::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	biquadA[0] = ((pow(B,3)*18930.0)/getSampleRate())+0.00162;
	double clipFactor = 1.212-((1.0-B)*0.496);
	
    biquadA[1] = 0.7071;
	double K = tan(M_PI * biquadA[0]);
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) {biquadD[x] = biquadC[x] = biquadB[x] = biquadA[x];}
	
	//opamp stuff
	double inTrim = A*10.0;
	inTrim *= inTrim; inTrim *= inTrim;
	double outPad = C*10.0;
	double iirAmountA = 0.00069/overallscale;
	biquadF[0] = biquadE[0] = 15500.0 / getSampleRate();
    biquadF[1] = biquadE[1] = 0.935;
	K = tan(M_PI * biquadE[0]); //lowpass
	norm = 1.0 / (1.0 + K / biquadE[1] + K * K);
	biquadE[2] = K * K * norm;
	biquadE[3] = 2.0 * biquadE[2];
	biquadE[4] = biquadE[2];
	biquadE[5] = 2.0 * (K * K - 1.0) * norm;
	biquadE[6] = (1.0 - K / biquadE[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) biquadF[x] = biquadE[x];
	//end opamp stuff	
	
 	double trim = 0.1+(3.712*biquadA[0]);
	double wet = pow(D,2);
	double aWet = 1.0;
	double bWet = 1.0;
	double cWet = 1.0;
	double dWet = wet*4.0;
	//four-stage wet/dry control using progressive stages that bypass when not engaged
	if (dWet < 1.0) {aWet = dWet; bWet = 0.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 2.0) {bWet = dWet - 1.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 3.0) {cWet = dWet - 2.0; dWet = 0.0;}
	else {dWet -= 3.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	double outSample = 0.0;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double overallDrySampleL = drySampleL;
		double overallDrySampleR = drySampleR;
		
		if (inTrim != 1.0) {inputSampleL *= inTrim; inputSampleR *= inTrim;}
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleL *= trim; inputSampleR *= trim;
		
		inputSampleL /= clipFactor;
		outSample = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; biquadA[10] = biquadA[9];
		if (outSample > 1.0) outSample = 1.0;
		if (outSample < -1.0) outSample = -1.0;
		drySampleL = inputSampleL = biquadA[9] = outSample; //DF1
		
		inputSampleR /= clipFactor;
		outSample = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
		biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; biquadA[14] = biquadA[13];
		if (outSample > 1.0) outSample = 1.0;
		if (outSample < -1.0) outSample = -1.0;
		drySampleR = inputSampleR = biquadA[13] = outSample; //DF1
		
		if (bWet > 0.0) {
			inputSampleL /= clipFactor;
			outSample = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; biquadB[10] = biquadB[9];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadB[9] = outSample; //DF1
			drySampleL = inputSampleL = (outSample * bWet) + (drySampleL * (1.0-bWet));
			
			inputSampleR /= clipFactor;
			outSample = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
			biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; biquadB[14] = biquadB[13];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadB[13] = outSample; //DF1
			drySampleR = inputSampleR = (outSample * bWet) + (drySampleR * (1.0-bWet));
		}
		if (cWet > 0.0) {
			inputSampleL /= clipFactor;
			outSample = biquadC[2]*inputSampleL+biquadC[3]*biquadC[7]+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
			biquadC[8] = biquadC[7]; biquadC[7] = inputSampleL; biquadC[10] = biquadC[9];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadC[9] = outSample; //DF1
			drySampleL = inputSampleL = (outSample * cWet) + (drySampleL * (1.0-cWet));
			
			inputSampleR /= clipFactor;
			outSample = biquadC[2]*inputSampleR+biquadC[3]*biquadC[11]+biquadC[4]*biquadC[12]-biquadC[5]*biquadC[13]-biquadC[6]*biquadC[14];
			biquadC[12] = biquadC[11]; biquadC[11] = inputSampleR; biquadC[14] = biquadC[13];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadC[13] = outSample; //DF1
			drySampleR = inputSampleR = (outSample * cWet) + (drySampleR * (1.0-cWet));
		}
		if (dWet > 0.0) {
			inputSampleL /= clipFactor;
			outSample = biquadD[2]*inputSampleL+biquadD[3]*biquadD[7]+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
			biquadD[8] = biquadD[7]; biquadD[7] = inputSampleL; biquadD[10] = biquadD[9];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadD[9] = outSample; //DF1
			drySampleL = inputSampleL = (outSample * dWet) + (drySampleL * (1.0-dWet));
			
			inputSampleR /= clipFactor;
			outSample = biquadD[2]*inputSampleR+biquadD[3]*biquadD[11]+biquadD[4]*biquadD[12]-biquadD[5]*biquadD[13]-biquadD[6]*biquadD[14];
			biquadD[12] = biquadD[11]; biquadD[11] = inputSampleR; biquadD[14] = biquadD[13];
			if (outSample > 1.0) outSample = 1.0;
			if (outSample < -1.0) outSample = -1.0;
			biquadD[13] = outSample; //DF1
			drySampleR = inputSampleR = (outSample * dWet) + (drySampleR * (1.0-dWet));
		}
		
		inputSampleL /= clipFactor; inputSampleR /= clipFactor;
		//opamp stage
		if (fabs(iirSampleAL)<1.18e-37) iirSampleAL = 0.0;
		iirSampleAL = (iirSampleAL * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
		inputSampleL -= iirSampleAL;
		if (fabs(iirSampleAR)<1.18e-37) iirSampleAR = 0.0;
		iirSampleAR = (iirSampleAR * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
		inputSampleR -= iirSampleAR;
		
		outSample = biquadE[2]*inputSampleL+biquadE[3]*biquadE[7]+biquadE[4]*biquadE[8]-biquadE[5]*biquadE[9]-biquadE[6]*biquadE[10];
		biquadE[8] = biquadE[7]; biquadE[7] = inputSampleL; inputSampleL = outSample; biquadE[10] = biquadE[9]; biquadE[9] = inputSampleL; //DF1 left
		
		outSample = biquadE[2]*inputSampleR+biquadE[3]*biquadE[11]+biquadE[4]*biquadE[12]-biquadE[5]*biquadE[13]-biquadE[6]*biquadE[14];
		biquadE[12] = biquadE[11]; biquadE[11] = inputSampleR; inputSampleR = outSample; biquadE[14] = biquadE[13]; biquadE[13] = inputSampleR; //DF1 right
		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL -= (inputSampleL*inputSampleL*inputSampleL*inputSampleL*inputSampleL*0.1768);
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR -= (inputSampleR*inputSampleR*inputSampleR*inputSampleR*inputSampleR*0.1768);
		
		outSample = biquadF[2]*inputSampleL+biquadF[3]*biquadF[7]+biquadF[4]*biquadF[8]-biquadF[5]*biquadF[9]-biquadF[6]*biquadF[10];
		biquadF[8] = biquadF[7]; biquadF[7] = inputSampleL; inputSampleL = outSample; biquadF[10] = biquadF[9]; biquadF[9] = inputSampleL; //DF1 left
		
		outSample = biquadF[2]*inputSampleR+biquadF[3]*biquadF[11]+biquadF[4]*biquadF[12]-biquadF[5]*biquadF[13]-biquadF[6]*biquadF[14];
		biquadF[12] = biquadF[11]; biquadF[11] = inputSampleR; inputSampleR = outSample; biquadF[14] = biquadF[13]; biquadF[13] = inputSampleR; //DF1 right
		
		if (outPad != 1.0) {inputSampleL *= outPad; inputSampleR *= outPad;}		
		//end opamp stage
		
		if (aWet !=1.0) {
			inputSampleL = (inputSampleL * aWet) + (overallDrySampleL * (1.0-aWet));
			inputSampleR = (inputSampleR * aWet) + (overallDrySampleR * (1.0-aWet));
		}	
		
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
