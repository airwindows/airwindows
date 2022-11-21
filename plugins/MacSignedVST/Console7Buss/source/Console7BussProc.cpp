/* ========================================
 *  Console7Buss - Console7Buss.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console7Buss_H
#include "Console7Buss.h"
#endif

void Console7Buss::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double inputgain = A * 1.03;
	
	if (gainchase != inputgain) chasespeed *= 2.0;
	if (chasespeed > sampleFrames) chasespeed = sampleFrames;
	if (gainchase < 0.0) gainchase = inputgain;
	
	biquadB[0] = biquadA[0] = 20000.0 / getSampleRate();
    biquadA[1] = 0.618033988749894848204586;
	biquadB[1] = 0.5;
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadA[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K * K * norm;
	biquadB[3] = 2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;	
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double outSampleL = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; inputSampleL = outSampleL; biquadA[10] = biquadA[9]; biquadA[9] = inputSampleL; //DF1 left
		
		double outSampleR = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
		biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; inputSampleR = outSampleR; biquadA[14] = biquadA[13]; biquadA[13] = inputSampleR; //DF1 right
		
		chasespeed *= 0.9999; chasespeed -= 0.01; if (chasespeed < 64.0) chasespeed = 64.0;
		//we have our chase speed compensated for recent fader activity
		gainchase = (((gainchase*chasespeed)+inputgain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		if (1.0 != gainchase) {inputSampleL *= sqrt(gainchase); inputSampleR *= sqrt(gainchase);}
		//done with trim control
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = ((asin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.618033988749894848204586)+(asin(inputSampleL)*0.381966011250105);
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = ((asin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.618033988749894848204586)+(asin(inputSampleR)*0.381966011250105);
		//this is an asin version of Spiral blended with regular asin ConsoleBuss.
		//It's blending between two different harmonics in the overtones of the algorithm
		
		outSampleL = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
		biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; inputSampleL = outSampleL; biquadB[10] = biquadB[9]; biquadB[9] = inputSampleL; //DF1 left
		
		outSampleR = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
		biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; inputSampleR = outSampleR; biquadB[14] = biquadB[13]; biquadB[13] = inputSampleR; //DF1 right
				
		if (1.0 != gainchase) {inputSampleL *= sqrt(gainchase); inputSampleR *= sqrt(gainchase);}
		//we re-amplify after the distortion relative to how much we cut back previously.
		
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

void Console7Buss::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double inputgain = A * 1.03;
	
	if (gainchase != inputgain) chasespeed *= 2.0;
	if (chasespeed > sampleFrames) chasespeed = sampleFrames;
	if (gainchase < 0.0) gainchase = inputgain;
	
	biquadB[0] = biquadA[0] = 20000.0 / getSampleRate();
    biquadA[1] = 0.618033988749894848204586;
	biquadB[1] = 0.5;
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadA[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K * K * norm;
	biquadB[3] = 2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;	
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double outSampleL = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; inputSampleL = outSampleL; biquadA[10] = biquadA[9]; biquadA[9] = inputSampleL; //DF1 left
		
		double outSampleR = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
		biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; inputSampleR = outSampleR; biquadA[14] = biquadA[13]; biquadA[13] = inputSampleR; //DF1 right
		
		chasespeed *= 0.9999; chasespeed -= 0.01; if (chasespeed < 64.0) chasespeed = 64.0;
		//we have our chase speed compensated for recent fader activity
		gainchase = (((gainchase*chasespeed)+inputgain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		if (1.0 != gainchase) {inputSampleL *= sqrt(gainchase); inputSampleR *= sqrt(gainchase);}
		//done with trim control
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = ((asin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.618033988749894848204586)+(asin(inputSampleL)*0.381966011250105);
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = ((asin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.618033988749894848204586)+(asin(inputSampleR)*0.381966011250105);
		//this is an asin version of Spiral blended with regular asin ConsoleBuss.
		//It's blending between two different harmonics in the overtones of the algorithm
		
		outSampleL = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
		biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; inputSampleL = outSampleL; biquadB[10] = biquadB[9]; biquadB[9] = inputSampleL; //DF1 left
		
		outSampleR = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
		biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; inputSampleR = outSampleR; biquadB[14] = biquadB[13]; biquadB[13] = inputSampleR; //DF1 right
				
		if (1.0 != gainchase) {inputSampleL *= sqrt(gainchase); inputSampleR *= sqrt(gainchase);}
		//we re-amplify after the distortion relative to how much we cut back previously.
		
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
