/* ========================================
 *  DeRez4 - DeRez4.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeRez4_H
#include "DeRez4.h"
#endif

void DeRez4::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double Sderez = pow(A,2.0+overallscale);
	int stepped = 999999; if (Sderez > 0.000001) stepped = (int)(1.0/Sderez);
	if (Sderez > 0.0) Sderez = 0.99999999 / stepped;
	double Strim = 1.0-(Sderez*((double)stepped/(stepped+1.0)));
	double brightDerez = (1.0-pow(B,2.0))*4.0*overallscale;
	stepped = 999999; 
	if (brightDerez > 1.0) {stepped = (int)brightDerez; brightDerez = 0.99999999 / stepped;}
	else brightDerez = 1.1;
	double brightTrim = 1.0-(brightDerez*((double)stepped/(stepped+1.0)));
	double bassRef = pow(C,4.0);
	double bassisDerez = (bassRef*0.25) / overallscale;
	stepped = 999999; if (bassisDerez > 0.000001) stepped = (int)(1.0/bassisDerez);
	if (bassisDerez > 0.0) bassisDerez = 0.99999999 / stepped;
	double bassisTrim = 1.0-(bassisDerez*((double)stepped/(stepped+1.0)));
	double output = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (brightDerez < 1.0){
			bezEQ[bez_HMcycle][0] += brightDerez;
			bezEQ[bez_HMAL][0] += (inputSampleL * 0.5 * brightDerez);
			bezEQ[bez_HMAR][0] += (inputSampleR * 0.5 * brightDerez);
			if (bezEQ[bez_HMcycle][0] > brightTrim) {
				bezEQ[bez_HMDL][0] = bezEQ[bez_HMCL][0]; bezEQ[bez_HMCL][0] = bezEQ[bez_HMBL][0]; 
				bezEQ[bez_HMBL][0] = bezEQ[bez_HMAL][0]*(0.5-(brightDerez*0.082));
				bezEQ[bez_HMAL][0] = 0.0;
				bezEQ[bez_HMDR][0] = bezEQ[bez_HMCR][0]; bezEQ[bez_HMCR][0] = bezEQ[bez_HMBR][0]; 
				bezEQ[bez_HMBR][0] = bezEQ[bez_HMAR][0]*(0.5-(brightDerez*0.082));
				bezEQ[bez_HMAR][0] = 0.0;
				bezEQ[bez_HMcycle][0] = 0.0;
			}
			double X = bezEQ[bez_HMcycle][0];
			inputSampleL = bezEQ[bez_HMCL][0]+(bezEQ[bez_HMDL][0]*(1.0-X)*(1.0-X));
			inputSampleL += (bezEQ[bez_HMCL][0]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBL][0]*X*X);
			inputSampleL /= brightTrim;
			inputSampleR = bezEQ[bez_HMCR][0]+(bezEQ[bez_HMDR][0]*(1.0-X)*(1.0-X));
			inputSampleR += (bezEQ[bez_HMCR][0]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBR][0]*X*X);
			inputSampleR /= brightTrim;
		}
		if (bassisDerez > 0.0) {
			bezEQ[bez_LMcycle][0] += bassisDerez;
			bezEQ[bez_LMAL][0] += (inputSampleL * bassisDerez);
			bezEQ[bez_LMAR][0] += (inputSampleR * bassisDerez);
			if (bezEQ[bez_LMcycle][0] > bassisTrim) {
				bezEQ[bez_LMDL][0] = bezEQ[bez_LMCL][0]; bezEQ[bez_LMCL][0] = bezEQ[bez_LMBL][0]; 
				bezEQ[bez_LMBL][0] = bezEQ[bez_LMAL][0]*(0.5-(bassisDerez*0.082));
				bezEQ[bez_LMAL][0] = 0.0;
				bezEQ[bez_LMDR][0] = bezEQ[bez_LMCR][0]; bezEQ[bez_LMCR][0] = bezEQ[bez_LMBR][0]; 
				bezEQ[bez_LMBR][0] = bezEQ[bez_LMAR][0]*(0.5-(bassisDerez*0.082));
				bezEQ[bez_LMAR][0] = 0.0;
				bezEQ[bez_LMcycle][0] = 0.0;
			}
			double X = bezEQ[bez_LMcycle][0];
			inputSampleL -= (bezEQ[bez_LMCL][0]+(bezEQ[bez_LMDL][0]*(1.0-X)*(1.0-X)))*0.5;
			inputSampleL -= ((bezEQ[bez_LMCL][0]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBL][0]*X*X))*0.5;
			inputSampleL /= bassisTrim;
			inputSampleR -= (bezEQ[bez_LMCR][0]+(bezEQ[bez_LMDR][0]*(1.0-X)*(1.0-X)))*0.5;
			inputSampleR -= ((bezEQ[bez_LMCR][0]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBR][0]*X*X))*0.5;
			inputSampleR /= bassisTrim;
		}
		if (Sderez < 1.0) {
			pointCycle += Sderez;
			if (pointCycle > 0.9999999) {
				pointAL = pointBL;
				pointBL = inputSampleL;
				pointAR = pointBR;
				pointBR = inputSampleR;
				pointCycle = 0.0;
			}
			double X = pointCycle*Strim;
			if (Sderez > 0.0) {
				inputSampleL = pointAL+((pointBL-pointAL)*X);
				inputSampleL /= Strim;
				inputSampleR = pointAR+((pointBR-pointAR)*X);
				inputSampleR /= Strim;
			} else {
				inputSampleL = 0.0;
				inputSampleR = 0.0;
			}
		}	
		if (brightDerez < 1.0) {
			bezEQ[bez_HMcycle][1] += brightDerez;
			bezEQ[bez_HMAL][1] += (inputSampleL * brightDerez);
			bezEQ[bez_HMAR][1] += (inputSampleR * brightDerez);
			if (bezEQ[bez_HMcycle][1] > brightTrim) {
				bezEQ[bez_HMDL][1] = bezEQ[bez_HMCL][1]; bezEQ[bez_HMCL][1] = bezEQ[bez_HMBL][1]; 
				bezEQ[bez_HMBL][1] = bezEQ[bez_HMAL][1]*(0.5-(brightDerez*0.082));
				bezEQ[bez_HMAL][1] = 0.0;
				bezEQ[bez_HMDR][1] = bezEQ[bez_HMCR][1]; bezEQ[bez_HMCR][1] = bezEQ[bez_HMBR][1]; 
				bezEQ[bez_HMBR][1] = bezEQ[bez_HMAR][1]*(0.5-(brightDerez*0.082));
				bezEQ[bez_HMAR][1] = 0.0; bezEQ[bez_HMcycle][1] = 0.0;
			}
			double X = bezEQ[bez_HMcycle][1];
			inputSampleL = bezEQ[bez_HMCL][1]+(bezEQ[bez_HMDL][1]*(1.0-X)*(1.0-X));
			inputSampleL += (bezEQ[bez_HMCL][1]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBL][1]*X*X);
			inputSampleL /= brightTrim;
			inputSampleR = bezEQ[bez_HMCR][1]+(bezEQ[bez_HMDR][1]*(1.0-X)*(1.0-X));
			inputSampleR += (bezEQ[bez_HMCR][1]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBR][1]*X*X);
			inputSampleR /= brightTrim;
		}
		if (bassisDerez > 0.0) {
			bezEQ[bez_LMcycle][1] += bassisDerez;
			bezEQ[bez_LMAL][1] += (inputSampleL * bassisDerez);
			bezEQ[bez_LMAR][1] += (inputSampleR * bassisDerez);
			if (bezEQ[bez_LMcycle][1] > bassisTrim) {
				bezEQ[bez_LMDL][1] = bezEQ[bez_LMCL][1]; bezEQ[bez_LMCL][1] = bezEQ[bez_LMBL][1]; 
				bezEQ[bez_LMBL][1] = bezEQ[bez_LMAL][1]*(0.5-(bassisDerez*0.082));
				bezEQ[bez_LMAL][1] = 0.0;
				bezEQ[bez_LMDR][1] = bezEQ[bez_LMCR][1]; bezEQ[bez_LMCR][1] = bezEQ[bez_LMBR][1]; 
				bezEQ[bez_LMBR][1] = bezEQ[bez_LMAR][1]*(0.5-(bassisDerez*0.082));
				bezEQ[bez_LMAR][1] = 0.0; bezEQ[bez_LMcycle][1] = 0.0;
			}
			double X = bezEQ[bez_LMcycle][1];
			inputSampleL -= (bezEQ[bez_LMCL][1]+(bezEQ[bez_LMDL][1]*(1.0-X)*(1.0-X)))*0.5;
			inputSampleL -= ((bezEQ[bez_LMCL][1]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBL][1]*X*X))*0.5;
			inputSampleL /= bassisTrim;
			inputSampleR -= (bezEQ[bez_LMCR][1]+(bezEQ[bez_LMDR][1]*(1.0-X)*(1.0-X)))*0.5;
			inputSampleR -= ((bezEQ[bez_LMCR][1]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBR][1]*X*X))*0.5;
			inputSampleR /= bassisTrim;
		}
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void DeRez4::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double Sderez = pow(A,2.0+overallscale);
	int stepped = 999999; if (Sderez > 0.000001) stepped = (int)(1.0/Sderez);
	if (Sderez > 0.0) Sderez = 0.99999999 / stepped;
	double Strim = 1.0-(Sderez*((double)stepped/(stepped+1.0)));
	double brightDerez = (1.0-pow(B,2.0))*4.0*overallscale;
	stepped = 999999; 
	if (brightDerez > 1.0) {stepped = (int)brightDerez; brightDerez = 0.99999999 / stepped;}
	else brightDerez = 1.1;
	double brightTrim = 1.0-(brightDerez*((double)stepped/(stepped+1.0)));
	double bassRef = pow(C,4.0);
	double bassisDerez = (bassRef*0.25) / overallscale;
	stepped = 999999; if (bassisDerez > 0.000001) stepped = (int)(1.0/bassisDerez);
	if (bassisDerez > 0.0) bassisDerez = 0.99999999 / stepped;
	double bassisTrim = 1.0-(bassisDerez*((double)stepped/(stepped+1.0)));
	double output = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (brightDerez < 1.0){
			bezEQ[bez_HMcycle][0] += brightDerez;
			bezEQ[bez_HMAL][0] += (inputSampleL * 0.5 * brightDerez);
			bezEQ[bez_HMAR][0] += (inputSampleR * 0.5 * brightDerez);
			if (bezEQ[bez_HMcycle][0] > brightTrim) {
				bezEQ[bez_HMDL][0] = bezEQ[bez_HMCL][0]; bezEQ[bez_HMCL][0] = bezEQ[bez_HMBL][0]; 
				bezEQ[bez_HMBL][0] = bezEQ[bez_HMAL][0]*(0.5-(brightDerez*0.082));
				bezEQ[bez_HMAL][0] = 0.0;
				bezEQ[bez_HMDR][0] = bezEQ[bez_HMCR][0]; bezEQ[bez_HMCR][0] = bezEQ[bez_HMBR][0]; 
				bezEQ[bez_HMBR][0] = bezEQ[bez_HMAR][0]*(0.5-(brightDerez*0.082));
				bezEQ[bez_HMAR][0] = 0.0;
				bezEQ[bez_HMcycle][0] = 0.0;
			}
			double X = bezEQ[bez_HMcycle][0];
			inputSampleL = bezEQ[bez_HMCL][0]+(bezEQ[bez_HMDL][0]*(1.0-X)*(1.0-X));
			inputSampleL += (bezEQ[bez_HMCL][0]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBL][0]*X*X);
			inputSampleL /= brightTrim;
			inputSampleR = bezEQ[bez_HMCR][0]+(bezEQ[bez_HMDR][0]*(1.0-X)*(1.0-X));
			inputSampleR += (bezEQ[bez_HMCR][0]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBR][0]*X*X);
			inputSampleR /= brightTrim;
		}
		if (bassisDerez > 0.0) {
			bezEQ[bez_LMcycle][0] += bassisDerez;
			bezEQ[bez_LMAL][0] += (inputSampleL * bassisDerez);
			bezEQ[bez_LMAR][0] += (inputSampleR * bassisDerez);
			if (bezEQ[bez_LMcycle][0] > bassisTrim) {
				bezEQ[bez_LMDL][0] = bezEQ[bez_LMCL][0]; bezEQ[bez_LMCL][0] = bezEQ[bez_LMBL][0]; 
				bezEQ[bez_LMBL][0] = bezEQ[bez_LMAL][0]*(0.5-(bassisDerez*0.082));
				bezEQ[bez_LMAL][0] = 0.0;
				bezEQ[bez_LMDR][0] = bezEQ[bez_LMCR][0]; bezEQ[bez_LMCR][0] = bezEQ[bez_LMBR][0]; 
				bezEQ[bez_LMBR][0] = bezEQ[bez_LMAR][0]*(0.5-(bassisDerez*0.082));
				bezEQ[bez_LMAR][0] = 0.0;
				bezEQ[bez_LMcycle][0] = 0.0;
			}
			double X = bezEQ[bez_LMcycle][0];
			inputSampleL -= (bezEQ[bez_LMCL][0]+(bezEQ[bez_LMDL][0]*(1.0-X)*(1.0-X)))*0.5;
			inputSampleL -= ((bezEQ[bez_LMCL][0]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBL][0]*X*X))*0.5;
			inputSampleL /= bassisTrim;
			inputSampleR -= (bezEQ[bez_LMCR][0]+(bezEQ[bez_LMDR][0]*(1.0-X)*(1.0-X)))*0.5;
			inputSampleR -= ((bezEQ[bez_LMCR][0]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBR][0]*X*X))*0.5;
			inputSampleR /= bassisTrim;
		}
		if (Sderez < 1.0) {
			pointCycle += Sderez;
			if (pointCycle > 0.9999999) {
				pointAL = pointBL;
				pointBL = inputSampleL;
				pointAR = pointBR;
				pointBR = inputSampleR;
				pointCycle = 0.0;
			}
			double X = pointCycle*Strim;
			if (Sderez > 0.0) {
				inputSampleL = pointAL+((pointBL-pointAL)*X);
				inputSampleL /= Strim;
				inputSampleR = pointAR+((pointBR-pointAR)*X);
				inputSampleR /= Strim;
			} else {
				inputSampleL = 0.0;
				inputSampleR = 0.0;
			}
		}	
		if (brightDerez < 1.0) {
			bezEQ[bez_HMcycle][1] += brightDerez;
			bezEQ[bez_HMAL][1] += (inputSampleL * brightDerez);
			bezEQ[bez_HMAR][1] += (inputSampleR * brightDerez);
			if (bezEQ[bez_HMcycle][1] > brightTrim) {
				bezEQ[bez_HMDL][1] = bezEQ[bez_HMCL][1]; bezEQ[bez_HMCL][1] = bezEQ[bez_HMBL][1]; 
				bezEQ[bez_HMBL][1] = bezEQ[bez_HMAL][1]*(0.5-(brightDerez*0.082));
				bezEQ[bez_HMAL][1] = 0.0;
				bezEQ[bez_HMDR][1] = bezEQ[bez_HMCR][1]; bezEQ[bez_HMCR][1] = bezEQ[bez_HMBR][1]; 
				bezEQ[bez_HMBR][1] = bezEQ[bez_HMAR][1]*(0.5-(brightDerez*0.082));
				bezEQ[bez_HMAR][1] = 0.0; bezEQ[bez_HMcycle][1] = 0.0;
			}
			double X = bezEQ[bez_HMcycle][1];
			inputSampleL = bezEQ[bez_HMCL][1]+(bezEQ[bez_HMDL][1]*(1.0-X)*(1.0-X));
			inputSampleL += (bezEQ[bez_HMCL][1]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBL][1]*X*X);
			inputSampleL /= brightTrim;
			inputSampleR = bezEQ[bez_HMCR][1]+(bezEQ[bez_HMDR][1]*(1.0-X)*(1.0-X));
			inputSampleR += (bezEQ[bez_HMCR][1]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBR][1]*X*X);
			inputSampleR /= brightTrim;
		}
		if (bassisDerez > 0.0) {
			bezEQ[bez_LMcycle][1] += bassisDerez;
			bezEQ[bez_LMAL][1] += (inputSampleL * bassisDerez);
			bezEQ[bez_LMAR][1] += (inputSampleR * bassisDerez);
			if (bezEQ[bez_LMcycle][1] > bassisTrim) {
				bezEQ[bez_LMDL][1] = bezEQ[bez_LMCL][1]; bezEQ[bez_LMCL][1] = bezEQ[bez_LMBL][1]; 
				bezEQ[bez_LMBL][1] = bezEQ[bez_LMAL][1]*(0.5-(bassisDerez*0.082));
				bezEQ[bez_LMAL][1] = 0.0;
				bezEQ[bez_LMDR][1] = bezEQ[bez_LMCR][1]; bezEQ[bez_LMCR][1] = bezEQ[bez_LMBR][1]; 
				bezEQ[bez_LMBR][1] = bezEQ[bez_LMAR][1]*(0.5-(bassisDerez*0.082));
				bezEQ[bez_LMAR][1] = 0.0; bezEQ[bez_LMcycle][1] = 0.0;
			}
			double X = bezEQ[bez_LMcycle][1];
			inputSampleL -= (bezEQ[bez_LMCL][1]+(bezEQ[bez_LMDL][1]*(1.0-X)*(1.0-X)))*0.5;
			inputSampleL -= ((bezEQ[bez_LMCL][1]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBL][1]*X*X))*0.5;
			inputSampleL /= bassisTrim;
			inputSampleR -= (bezEQ[bez_LMCR][1]+(bezEQ[bez_LMDR][1]*(1.0-X)*(1.0-X)))*0.5;
			inputSampleR -= ((bezEQ[bez_LMCR][1]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBR][1]*X*X))*0.5;
			inputSampleR /= bassisTrim;
		}
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l  * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
