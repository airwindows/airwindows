/* ========================================
 *  ZOutputStage - ZOutputStage.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ZOutputStage_H
#include "ZOutputStage.h"
#endif

void ZOutputStage::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	//opamp stuff
	double inTrim = A*10.0;
	inTrim *= inTrim; inTrim *= inTrim;
	double outPad = B;
	double iirAmountA = 0.00069/overallscale;
	biquadF[0] = biquadE[0] = 15500.0 / getSampleRate();
    biquadF[1] = biquadE[1] = 0.935;
	double K = tan(M_PI * biquadE[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadE[1] + K * K);
	biquadE[2] = K * K * norm;
	biquadE[3] = 2.0 * biquadE[2];
	biquadE[4] = biquadE[2];
	biquadE[5] = 2.0 * (K * K - 1.0) * norm;
	biquadE[6] = (1.0 - K / biquadE[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) biquadF[x] = biquadE[x];
	//end opamp stuff	
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (inTrim != 1.0) {inputSampleL *= inTrim; inputSampleR *= inTrim;}

		//opamp stage
		if (fabs(iirSampleAL)<1.18e-37) iirSampleAL = 0.0;
		iirSampleAL = (iirSampleAL * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
		inputSampleL -= iirSampleAL;
		if (fabs(iirSampleAR)<1.18e-37) iirSampleAR = 0.0;
		iirSampleAR = (iirSampleAR * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
		inputSampleR -= iirSampleAR;
		
		double outSample = biquadE[2]*inputSampleL+biquadE[3]*biquadE[7]+biquadE[4]*biquadE[8]-biquadE[5]*biquadE[9]-biquadE[6]*biquadE[10];
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

void ZOutputStage::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	//opamp stuff
	double inTrim = A*10.0;
	inTrim *= inTrim; inTrim *= inTrim;
	double outPad = B;
	double iirAmountA = 0.00069/overallscale;
	biquadF[0] = biquadE[0] = 15500.0 / getSampleRate();
    biquadF[1] = biquadE[1] = 0.935;
	double K = tan(M_PI * biquadE[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadE[1] + K * K);
	biquadE[2] = K * K * norm;
	biquadE[3] = 2.0 * biquadE[2];
	biquadE[4] = biquadE[2];
	biquadE[5] = 2.0 * (K * K - 1.0) * norm;
	biquadE[6] = (1.0 - K / biquadE[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) biquadF[x] = biquadE[x];
	//end opamp stuff	
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (inTrim != 1.0) {inputSampleL *= inTrim; inputSampleR *= inTrim;}
		
		//opamp stage
		if (fabs(iirSampleAL)<1.18e-37) iirSampleAL = 0.0;
		iirSampleAL = (iirSampleAL * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
		inputSampleL -= iirSampleAL;
		if (fabs(iirSampleAR)<1.18e-37) iirSampleAR = 0.0;
		iirSampleAR = (iirSampleAR * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
		inputSampleR -= iirSampleAR;
		
		double outSample = biquadE[2]*inputSampleL+biquadE[3]*biquadE[7]+biquadE[4]*biquadE[8]-biquadE[5]*biquadE[9]-biquadE[6]*biquadE[10];
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
