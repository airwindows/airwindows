/* ========================================
 *  Pop3 - Pop3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Pop3_H
#include "Pop3.h"
#endif

void Pop3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double compThresh = pow(A,4);
	double compRatio = 1.0-pow(1.0-B,2);
	double compAttack = 1.0/(((pow(C,3)*5000.0)+500.0)*overallscale);
	double compRelease = 1.0/(((pow(D,5)*50000.0)+500.0)*overallscale);
	
	double gateThresh = pow(E,4);
	double gateRatio = 1.0-pow(1.0-F,2);
	double gateSustain = M_PI_2 * pow(G+1.0,4.0);
	double gateRelease = 1.0/(((pow(H,5)*500000.0)+500.0)*overallscale);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (fabs(inputSampleL) > compThresh) { //compression L
			popCompL -= (popCompL * compAttack);
			popCompL += ((compThresh / fabs(inputSampleL))*compAttack);
		} else popCompL = (popCompL*(1.0-compRelease))+compRelease;
		if (fabs(inputSampleR) > compThresh) { //compression R
			popCompR -= (popCompR * compAttack);
			popCompR += ((compThresh / fabs(inputSampleR))*compAttack);
		} else popCompR = (popCompR*(1.0-compRelease))+compRelease;
		if (popCompL > popCompR) popCompL -= (popCompL * compAttack);
		if (popCompR > popCompL) popCompR -= (popCompR * compAttack);
		if (fabs(inputSampleL) > gateThresh) popGate = gateSustain;
		else if (fabs(inputSampleR) > gateThresh) popGate = gateSustain;
		else popGate *= (1.0-gateRelease);
		if (popGate < 0.0) popGate = 0.0;
		popCompL = fmax(fmin(popCompL,1.0),0.0);
		popCompR = fmax(fmin(popCompR,1.0),0.0);
		inputSampleL *= ((1.0-compRatio)+(popCompL*compRatio));
		inputSampleR *= ((1.0-compRatio)+(popCompR*compRatio));
		if (popGate < M_PI_2) {
			inputSampleL *= ((1.0-gateRatio)+(sin(popGate)*gateRatio));
			inputSampleR *= ((1.0-gateRatio)+(sin(popGate)*gateRatio));
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

void Pop3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double compThresh = pow(A,4);
	double compRatio = 1.0-pow(1.0-B,2);
	double compAttack = 1.0/(((pow(C,3)*5000.0)+500.0)*overallscale);
	double compRelease = 1.0/(((pow(D,5)*50000.0)+500.0)*overallscale);
	
	double gateThresh = pow(E,4);
	double gateRatio = 1.0-pow(1.0-F,2);
	double gateSustain = M_PI_2 * pow(G+1.0,4.0);
	double gateRelease = 1.0/(((pow(H,5)*500000.0)+500.0)*overallscale);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (fabs(inputSampleL) > compThresh) { //compression L
			popCompL -= (popCompL * compAttack);
			popCompL += ((compThresh / fabs(inputSampleL))*compAttack);
		} else popCompL = (popCompL*(1.0-compRelease))+compRelease;
		if (fabs(inputSampleR) > compThresh) { //compression R
			popCompR -= (popCompR * compAttack);
			popCompR += ((compThresh / fabs(inputSampleR))*compAttack);
		} else popCompR = (popCompR*(1.0-compRelease))+compRelease;
		if (popCompL > popCompR) popCompL -= (popCompL * compAttack);
		if (popCompR > popCompL) popCompR -= (popCompR * compAttack);
		if (fabs(inputSampleL) > gateThresh) popGate = gateSustain;
		else if (fabs(inputSampleR) > gateThresh) popGate = gateSustain;
		else popGate *= (1.0-gateRelease);
		if (popGate < 0.0) popGate = 0.0;
		popCompL = fmax(fmin(popCompL,1.0),0.0);
		popCompR = fmax(fmin(popCompR,1.0),0.0);
		inputSampleL *= ((1.0-compRatio)+(popCompL*compRatio));
		inputSampleR *= ((1.0-compRatio)+(popCompR*compRatio));
		if (popGate < M_PI_2) {
			inputSampleL *= ((1.0-gateRatio)+(sin(popGate)*gateRatio));
			inputSampleR *= ((1.0-gateRatio)+(sin(popGate)*gateRatio));
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
