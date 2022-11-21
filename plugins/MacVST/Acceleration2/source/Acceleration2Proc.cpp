/* ========================================
 *  Acceleration2 - Acceleration2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Acceleration2_H
#include "Acceleration2.h"
#endif

void Acceleration2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double intensity = pow(A,3)*32;
	double wet = B;	
	int spacing = (int)(1.73*overallscale)+1;
	if (spacing > 16) spacing = 16;
	
	biquadA[0] = (20000.0 * (1.0-(A*0.618033988749894848204586))) / getSampleRate();
	biquadB[0] = 20000.0 / getSampleRate();
    biquadA[1] = 0.7071;
	biquadB[1] = 0.7071;
	
	double K = tan(M_PI * biquadA[0]);
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadB[0]);
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
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double tempSample = (inputSampleL * biquadA[2]) + biquadA[7];
		biquadA[7] = (inputSampleL * biquadA[3]) - (tempSample * biquadA[5]) + biquadA[8];
		biquadA[8] = (inputSampleL * biquadA[4]) - (tempSample * biquadA[6]);
		double smoothL = tempSample; //like mono AU, 7 and 8 store L channel
		
		tempSample = (inputSampleR * biquadA[2]) + biquadA[9];
		biquadA[9] = (inputSampleR * biquadA[3]) - (tempSample * biquadA[5]) + biquadA[10];
		biquadA[10] = (inputSampleR * biquadA[4]) - (tempSample * biquadA[6]);
		double smoothR = tempSample; //note: 9 and 10 store the R channel
		
		for(int count = spacing*2; count >= 0; count--) {sL[count+1] = sL[count]; sR[count+1] = sR[count];}
		sL[0] = inputSampleL; sR[0] = inputSampleR;

		m1L = (sL[0]-sL[spacing])*(fabs(sL[0]-sL[spacing]));
		m2L = (sL[spacing]-sL[spacing*2])*(fabs(sL[spacing]-sL[spacing*2]));
		double senseL = (intensity*intensity*fabs(m1L-m2L));
		if (senseL > 1.0) senseL = 1.0;
		inputSampleL = (inputSampleL * (1.0-senseL)) + (smoothL*senseL);
		
		m1R = (sR[0]-sR[spacing])*(fabs(sR[0]-sR[spacing]));
		m2R = (sR[spacing]-sR[spacing*2])*(fabs(sR[spacing]-sR[spacing*2]));
		double senseR = (intensity*intensity*fabs(m1R-m2R));
		if (senseR > 1.0) senseR = 1.0;
		inputSampleR = (inputSampleR * (1.0-senseR)) + (smoothR*senseR);		
		
		tempSample = (inputSampleL * biquadB[2]) + biquadB[7];
		biquadB[7] = (inputSampleL * biquadB[3]) - (tempSample * biquadB[5]) + biquadB[8];
		biquadB[8] = (inputSampleL * biquadB[4]) - (tempSample * biquadB[6]);
		inputSampleL = tempSample; //like mono AU, 7 and 8 store L channel
		
		tempSample = (inputSampleR * biquadB[2]) + biquadB[9];
		biquadB[9] = (inputSampleR * biquadB[3]) - (tempSample * biquadB[5]) + biquadB[10];
		biquadB[10] = (inputSampleR * biquadB[4]) - (tempSample * biquadB[6]);
		inputSampleR = tempSample; //note: 9 and 10 store the R channel
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Acceleration2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double intensity = pow(A,3)*32;
	double wet = B;	
	int spacing = (int)(1.73*overallscale)+1;
	if (spacing > 16) spacing = 16;
	
	biquadA[0] = (20000.0 * (1.0-(A*0.618033988749894848204586))) / getSampleRate();
	biquadB[0] = 20000.0 / getSampleRate();
    biquadA[1] = 0.7071;
	biquadB[1] = 0.7071;
	
	double K = tan(M_PI * biquadA[0]);
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadB[0]);
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
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double tempSample = (inputSampleL * biquadA[2]) + biquadA[7];
		biquadA[7] = (inputSampleL * biquadA[3]) - (tempSample * biquadA[5]) + biquadA[8];
		biquadA[8] = (inputSampleL * biquadA[4]) - (tempSample * biquadA[6]);
		double smoothL = tempSample; //like mono AU, 7 and 8 store L channel
		
		tempSample = (inputSampleR * biquadA[2]) + biquadA[9];
		biquadA[9] = (inputSampleR * biquadA[3]) - (tempSample * biquadA[5]) + biquadA[10];
		biquadA[10] = (inputSampleR * biquadA[4]) - (tempSample * biquadA[6]);
		double smoothR = tempSample; //note: 9 and 10 store the R channel
		
		for(int count = spacing*2; count >= 0; count--) {sL[count+1] = sL[count]; sR[count+1] = sR[count];}
		sL[0] = inputSampleL; sR[0] = inputSampleR;
		
		m1L = (sL[0]-sL[spacing])*(fabs(sL[0]-sL[spacing]));
		m2L = (sL[spacing]-sL[spacing*2])*(fabs(sL[spacing]-sL[spacing*2]));
		double senseL = (intensity*intensity*fabs(m1L-m2L));
		if (senseL > 1.0) senseL = 1.0;
		inputSampleL = (inputSampleL * (1.0-senseL)) + (smoothL*senseL);
		
		m1R = (sR[0]-sR[spacing])*(fabs(sR[0]-sR[spacing]));
		m2R = (sR[spacing]-sR[spacing*2])*(fabs(sR[spacing]-sR[spacing*2]));
		double senseR = (intensity*intensity*fabs(m1R-m2R));
		if (senseR > 1.0) senseR = 1.0;
		inputSampleR = (inputSampleR * (1.0-senseR)) + (smoothR*senseR);		
		
		tempSample = (inputSampleL * biquadB[2]) + biquadB[7];
		biquadB[7] = (inputSampleL * biquadB[3]) - (tempSample * biquadB[5]) + biquadB[8];
		biquadB[8] = (inputSampleL * biquadB[4]) - (tempSample * biquadB[6]);
		inputSampleL = tempSample; //like mono AU, 7 and 8 store L channel
		
		tempSample = (inputSampleR * biquadB[2]) + biquadB[9];
		biquadB[9] = (inputSampleR * biquadB[3]) - (tempSample * biquadB[5]) + biquadB[10];
		biquadB[10] = (inputSampleR * biquadB[4]) - (tempSample * biquadB[6]);
		inputSampleR = tempSample; //note: 9 and 10 store the R channel
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
