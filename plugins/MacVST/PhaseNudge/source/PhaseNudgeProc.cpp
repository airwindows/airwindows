/* ========================================
 *  PhaseNudge - PhaseNudge.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PhaseNudge_H
#include "PhaseNudge.h"
#endif

void PhaseNudge::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	
	int allpasstemp;
	double outallpass = 0.618033988749894848204586; //golden ratio!
	//if you see 0.6180 it's not a wild stretch to wonder whether you are working with a constant
	int maxdelayTarget = (int)(pow(A,3)*1501.0);
	double wet = B;
	//removed extra dry variable
	double bridgerectifier;	

	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL /= 4.0;
		inputSampleR /= 4.0;
		
		bridgerectifier = fabs(inputSampleL);
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		
		bridgerectifier = fabs(inputSampleR);
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		
		if (fabs(maxdelay - maxdelayTarget) > 1500) maxdelay = maxdelayTarget;
		
		if (maxdelay < maxdelayTarget) {
			maxdelay++;
			dL[maxdelay] = (dL[0]+dL[maxdelay-1]) / 2.0;
			dR[maxdelay] = (dR[0]+dR[maxdelay-1]) / 2.0;
		}
		
		if (maxdelay > maxdelayTarget) {
			maxdelay--;
			dL[maxdelay] = (dL[0]+dL[maxdelay]) / 2.0;
			dR[maxdelay] = (dR[0]+dR[maxdelay]) / 2.0;
		}
		
		allpasstemp = one - 1;
		
		if (allpasstemp < 0 || allpasstemp > maxdelay) allpasstemp = maxdelay;
		
		inputSampleL -= dL[allpasstemp]*outallpass;
		inputSampleR -= dR[allpasstemp]*outallpass;
		dL[one] = inputSampleL;
		dR[one] = inputSampleR;
		inputSampleL *= outallpass;
		inputSampleR *= outallpass;
		one--; if (one < 0 || one > maxdelay) {one = maxdelay;}
		inputSampleL += (dL[one]);
		inputSampleR += (dR[one]);

		
		bridgerectifier = fabs(inputSampleL);
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSampleL > 0) inputSampleL -= bridgerectifier;
		else inputSampleL += bridgerectifier;

		bridgerectifier = fabs(inputSampleR);
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSampleR > 0) inputSampleR -= bridgerectifier;
		else inputSampleR += bridgerectifier;

		inputSampleL *= 4.0;
		inputSampleR *= 4.0;

		if (wet < 1.0) {
			inputSampleL = (drySampleL * (1.0-wet))+(inputSampleL * wet);
			inputSampleR = (drySampleR * (1.0-wet))+(inputSampleR * wet);
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

void PhaseNudge::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	
	int allpasstemp;
	double outallpass = 0.618033988749894848204586; //golden ratio!
	//if you see 0.6180 it's not a wild stretch to wonder whether you are working with a constant
	int maxdelayTarget = (int)(pow(A,3)*1501.0);
	double wet = B;
	//removed extra dry variable
	double bridgerectifier;	
	
	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL /= 4.0;
		inputSampleR /= 4.0;
		
		bridgerectifier = fabs(inputSampleL);
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		
		bridgerectifier = fabs(inputSampleR);
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		
		if (fabs(maxdelay - maxdelayTarget) > 1500) maxdelay = maxdelayTarget;
		
		if (maxdelay < maxdelayTarget) {
			maxdelay++;
			dL[maxdelay] = (dL[0]+dL[maxdelay-1]) / 2.0;
			dR[maxdelay] = (dR[0]+dR[maxdelay-1]) / 2.0;
		}
		
		if (maxdelay > maxdelayTarget) {
			maxdelay--;
			dL[maxdelay] = (dL[0]+dL[maxdelay]) / 2.0;
			dR[maxdelay] = (dR[0]+dR[maxdelay]) / 2.0;
		}
		
		allpasstemp = one - 1;
		
		if (allpasstemp < 0 || allpasstemp > maxdelay) allpasstemp = maxdelay;
		
		inputSampleL -= dL[allpasstemp]*outallpass;
		inputSampleR -= dR[allpasstemp]*outallpass;
		dL[one] = inputSampleL;
		dR[one] = inputSampleR;
		inputSampleL *= outallpass;
		inputSampleR *= outallpass;
		one--; if (one < 0 || one > maxdelay) {one = maxdelay;}
		inputSampleL += (dL[one]);
		inputSampleR += (dR[one]);
		
		bridgerectifier = fabs(inputSampleL);
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSampleL > 0) inputSampleL -= bridgerectifier;
		else inputSampleL += bridgerectifier;
		
		bridgerectifier = fabs(inputSampleR);
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSampleR > 0) inputSampleR -= bridgerectifier;
		else inputSampleR += bridgerectifier;
		
		inputSampleL *= 4.0;
		inputSampleR *= 4.0;
		
		if (wet < 1.0) {
			inputSampleL = (drySampleL * (1.0-wet))+(inputSampleL * wet);
			inputSampleR = (drySampleR * (1.0-wet))+(inputSampleR * wet);
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