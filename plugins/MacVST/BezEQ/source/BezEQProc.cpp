/* ========================================
 *  BezEQ - BezEQ.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BezEQ_H
#include "BezEQ.h"
#endif

void BezEQ::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double trebleGain = A * 2.0; trebleGain *= trebleGain;
	
	double derezA = B/overallscale;
	if (derezA < 0.01) derezA = 0.01; if (derezA > 1.0) derezA = 1.0;
	derezA = 1.0 / ((int)(1.0/derezA));
	
	double midGain = C * 2.0; midGain *= midGain;
	
	double derezB = pow(D,4.0)/overallscale;
	if (derezB < 0.0001) derezB = 0.0001; if (derezB > 1.0) derezB = 1.0;
	derezB = 1.0 / ((int)(1.0/derezB));
	
	double bassGain = E * 2.0; bassGain *= bassGain;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bezA[bez_cycle] += derezA;
		bezA[bez_SampL] += ((inputSampleL+bezA[bez_InL]) * derezA);
		bezA[bez_InL] = inputSampleL;
		bezA[bez_SampR] += ((inputSampleR+bezA[bez_InR]) * derezA);
		bezA[bez_InR] = inputSampleR;
		
		if (bezA[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bezA[bez_cycle] = 0.0;
			bezA[bez_CL] = bezA[bez_BL];
			bezA[bez_BL] = bezA[bez_AL];
			bezA[bez_AL] = inputSampleL;
			bezA[bez_SampL] = 0.0;
			bezA[bez_CR] = bezA[bez_BR];
			bezA[bez_BR] = bezA[bez_AR];
			bezA[bez_AR] = inputSampleR;
			bezA[bez_SampR] = 0.0;
		}
		double CBL = (bezA[bez_CL]*(1.0-bezA[bez_cycle]))+(bezA[bez_BL]*bezA[bez_cycle]);
		double BAL = (bezA[bez_BL]*(1.0-bezA[bez_cycle]))+(bezA[bez_AL]*bezA[bez_cycle]);
		double CBAL = (bezA[bez_BL]+(CBL*(1.0-bezA[bez_cycle]))+(BAL*bezA[bez_cycle]))*0.5;
		double midL = CBAL;
		double trebleL = inputSampleL - CBAL;
		double CBR = (bezA[bez_CR]*(1.0-bezA[bez_cycle]))+(bezA[bez_BR]*bezA[bez_cycle]);
		double BAR = (bezA[bez_BR]*(1.0-bezA[bez_cycle]))+(bezA[bez_AR]*bezA[bez_cycle]);
		double CBAR = (bezA[bez_BR]+(CBR*(1.0-bezA[bez_cycle]))+(BAR*bezA[bez_cycle]))*0.5;
		double midR = CBAR;
		double trebleR = inputSampleR - CBAR;
		
		bezB[bez_cycle] += derezB;
		bezB[bez_SampL] += ((midL+bezB[bez_InL]) * derezB);
		bezB[bez_InL] = midL;
		bezB[bez_SampR] += ((midR+bezB[bez_InR]) * derezB);
		bezB[bez_InR] = midR;
		
		if (bezB[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bezB[bez_cycle] = 0.0;
			bezB[bez_CL] = bezB[bez_BL];
			bezB[bez_BL] = bezB[bez_AL];
			bezB[bez_AL] = inputSampleL;
			bezB[bez_SampL] = 0.0;
			bezB[bez_CR] = bezB[bez_BR];
			bezB[bez_BR] = bezB[bez_AR];
			bezB[bez_AR] = inputSampleR;
			bezB[bez_SampR] = 0.0;
		}
		CBL = (bezB[bez_CL]*(1.0-bezB[bez_cycle]))+(bezB[bez_BL]*bezB[bez_cycle]);
		BAL = (bezB[bez_BL]*(1.0-bezB[bez_cycle]))+(bezB[bez_AL]*bezB[bez_cycle]);
		CBAL = (bezB[bez_BL]+(CBL*(1.0-bezB[bez_cycle]))+(BAL*bezB[bez_cycle]))*0.5;
		double bassL = CBAL;
		midL -= bassL;
		CBR = (bezB[bez_CR]*(1.0-bezB[bez_cycle]))+(bezB[bez_BR]*bezB[bez_cycle]);
		BAR = (bezB[bez_BR]*(1.0-bezB[bez_cycle]))+(bezB[bez_AR]*bezB[bez_cycle]);
		CBAR = (bezB[bez_BR]+(CBR*(1.0-bezB[bez_cycle]))+(BAR*bezB[bez_cycle]))*0.5;
		double bassR = CBAR;
		midR -= bassR;
		
		inputSampleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		inputSampleR = (bassR*bassGain) + (midR*midGain) + (trebleR*trebleGain);

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

void BezEQ::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double trebleGain = A * 2.0; trebleGain *= trebleGain;
	
	double derezA = B/overallscale;
	if (derezA < 0.01) derezA = 0.01; if (derezA > 1.0) derezA = 1.0;
	derezA = 1.0 / ((int)(1.0/derezA));
	
	double midGain = C * 2.0; midGain *= midGain;
	
	double derezB = pow(D,4.0)/overallscale;
	if (derezB < 0.0001) derezB = 0.0001; if (derezB > 1.0) derezB = 1.0;
	derezB = 1.0 / ((int)(1.0/derezB));
	
	double bassGain = E * 2.0; bassGain *= bassGain;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bezA[bez_cycle] += derezA;
		bezA[bez_SampL] += ((inputSampleL+bezA[bez_InL]) * derezA);
		bezA[bez_InL] = inputSampleL;
		bezA[bez_SampR] += ((inputSampleR+bezA[bez_InR]) * derezA);
		bezA[bez_InR] = inputSampleR;
		
		if (bezA[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bezA[bez_cycle] = 0.0;
			bezA[bez_CL] = bezA[bez_BL];
			bezA[bez_BL] = bezA[bez_AL];
			bezA[bez_AL] = inputSampleL;
			bezA[bez_SampL] = 0.0;
			bezA[bez_CR] = bezA[bez_BR];
			bezA[bez_BR] = bezA[bez_AR];
			bezA[bez_AR] = inputSampleR;
			bezA[bez_SampR] = 0.0;
		}
		double CBL = (bezA[bez_CL]*(1.0-bezA[bez_cycle]))+(bezA[bez_BL]*bezA[bez_cycle]);
		double BAL = (bezA[bez_BL]*(1.0-bezA[bez_cycle]))+(bezA[bez_AL]*bezA[bez_cycle]);
		double CBAL = (bezA[bez_BL]+(CBL*(1.0-bezA[bez_cycle]))+(BAL*bezA[bez_cycle]))*0.5;
		double midL = CBAL;
		double trebleL = inputSampleL - CBAL;
		double CBR = (bezA[bez_CR]*(1.0-bezA[bez_cycle]))+(bezA[bez_BR]*bezA[bez_cycle]);
		double BAR = (bezA[bez_BR]*(1.0-bezA[bez_cycle]))+(bezA[bez_AR]*bezA[bez_cycle]);
		double CBAR = (bezA[bez_BR]+(CBR*(1.0-bezA[bez_cycle]))+(BAR*bezA[bez_cycle]))*0.5;
		double midR = CBAR;
		double trebleR = inputSampleR - CBAR;
		
		bezB[bez_cycle] += derezB;
		bezB[bez_SampL] += ((midL+bezB[bez_InL]) * derezB);
		bezB[bez_InL] = midL;
		bezB[bez_SampR] += ((midR+bezB[bez_InR]) * derezB);
		bezB[bez_InR] = midR;
		
		if (bezB[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bezB[bez_cycle] = 0.0;
			bezB[bez_CL] = bezB[bez_BL];
			bezB[bez_BL] = bezB[bez_AL];
			bezB[bez_AL] = inputSampleL;
			bezB[bez_SampL] = 0.0;
			bezB[bez_CR] = bezB[bez_BR];
			bezB[bez_BR] = bezB[bez_AR];
			bezB[bez_AR] = inputSampleR;
			bezB[bez_SampR] = 0.0;
		}
		CBL = (bezB[bez_CL]*(1.0-bezB[bez_cycle]))+(bezB[bez_BL]*bezB[bez_cycle]);
		BAL = (bezB[bez_BL]*(1.0-bezB[bez_cycle]))+(bezB[bez_AL]*bezB[bez_cycle]);
		CBAL = (bezB[bez_BL]+(CBL*(1.0-bezB[bez_cycle]))+(BAL*bezB[bez_cycle]))*0.5;
		double bassL = CBAL;
		midL -= bassL;
		CBR = (bezB[bez_CR]*(1.0-bezB[bez_cycle]))+(bezB[bez_BR]*bezB[bez_cycle]);
		BAR = (bezB[bez_BR]*(1.0-bezB[bez_cycle]))+(bezB[bez_AR]*bezB[bez_cycle]);
		CBAR = (bezB[bez_BR]+(CBR*(1.0-bezB[bez_cycle]))+(BAR*bezB[bez_cycle]))*0.5;
		double bassR = CBAR;
		midR -= bassR;
		
		inputSampleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		inputSampleR = (bassR*bassGain) + (midR*midGain) + (trebleR*trebleGain);
		
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
