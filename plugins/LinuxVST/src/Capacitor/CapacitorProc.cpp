/* ========================================
 *  Capacitor - Capacitor.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Capacitor_H
#include "Capacitor.h"
#endif

void Capacitor::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	lowpassChase = pow(A,2);
	highpassChase = pow(B,2);
	wetChase = C;
	//should not scale with sample rate, because values reaching 1 are important
	//to its ability to bypass when set to max
	double lowpassSpeed = 300 / (fabs( lastLowpass - lowpassChase)+1.0);
	double highpassSpeed = 300 / (fabs( lastHighpass - highpassChase)+1.0);
	double wetSpeed = 300 / (fabs( lastWet - wetChase)+1.0);
	lastLowpass = lowpassChase;
	lastHighpass = highpassChase;
	lastWet = wetChase;
	
	double invLowpass;
	double invHighpass;
	double dry;
	

	double inputSampleL;
	double inputSampleR;
	float drySampleL;
	float drySampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;


		lowpassAmount = (((lowpassAmount*lowpassSpeed)+lowpassChase)/(lowpassSpeed + 1.0)); invLowpass = 1.0 - lowpassAmount;
		highpassAmount = (((highpassAmount*highpassSpeed)+highpassChase)/(highpassSpeed + 1.0)); invHighpass = 1.0 - highpassAmount;
		wet = (((wet*wetSpeed)+wetChase)/(wetSpeed+1.0)); dry = 1.0 - wet;
		
		count++; if (count > 5) count = 0; switch (count)
		{
			case 0:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassBL = (iirHighpassBL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
				iirLowpassBL = (iirLowpassBL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
				iirHighpassDL = (iirHighpassDL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassDL;
				iirLowpassDL = (iirLowpassDL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassDL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassBR = (iirHighpassBR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
				iirLowpassBR = (iirLowpassBR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
				iirHighpassDR = (iirHighpassDR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassDR;
				iirLowpassDR = (iirLowpassDR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassDR;
				break;
			case 1:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassCL = (iirHighpassCL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
				iirLowpassCL = (iirLowpassCL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
				iirHighpassEL = (iirHighpassEL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassEL;
				iirLowpassEL = (iirLowpassEL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassEL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassCR = (iirHighpassCR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
				iirLowpassCR = (iirLowpassCR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
				iirHighpassER = (iirHighpassER * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassER;
				iirLowpassER = (iirLowpassER * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassER;
				break;
			case 2:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassBL = (iirHighpassBL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
				iirLowpassBL = (iirLowpassBL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
				iirHighpassFL = (iirHighpassFL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassFL;
				iirLowpassFL = (iirLowpassFL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassFL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassBR = (iirHighpassBR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
				iirLowpassBR = (iirLowpassBR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
				iirHighpassFR = (iirHighpassFR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassFR;
				iirLowpassFR = (iirLowpassFR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassFR;
				break;
			case 3:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassCL = (iirHighpassCL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
				iirLowpassCL = (iirLowpassCL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
				iirHighpassDL = (iirHighpassDL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassDL;
				iirLowpassDL = (iirLowpassDL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassDL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassCR = (iirHighpassCR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
				iirLowpassCR = (iirLowpassCR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
				iirHighpassDR = (iirHighpassDR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassDR;
				iirLowpassDR = (iirLowpassDR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassDR;
				break;
			case 4:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassBL = (iirHighpassBL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
				iirLowpassBL = (iirLowpassBL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
				iirHighpassEL = (iirHighpassEL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassEL;
				iirLowpassEL = (iirLowpassEL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassEL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassBR = (iirHighpassBR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
				iirLowpassBR = (iirLowpassBR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
				iirHighpassER = (iirHighpassER * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassER;
				iirLowpassER = (iirLowpassER * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassER;
				break;
			case 5:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassCL = (iirHighpassCL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
				iirLowpassCL = (iirLowpassCL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
				iirHighpassFL = (iirHighpassFL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassFL;
				iirLowpassFL = (iirLowpassFL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassFL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassCR = (iirHighpassCR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
				iirLowpassCR = (iirLowpassCR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
				iirHighpassFR = (iirHighpassFR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassFR;
				iirLowpassFR = (iirLowpassFR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassFR;
				break;
		}
		//Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
		//steepens the filter after minimizing artifacts.
		
		inputSampleL = (drySampleL * dry) + (inputSampleL * wet);
		inputSampleR = (drySampleR * dry) + (inputSampleR * wet);

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

void Capacitor::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	lowpassChase = pow(A,2);
	highpassChase = pow(B,2);
	wetChase = C;
	//should not scale with sample rate, because values reaching 1 are important
	//to its ability to bypass when set to max
	double lowpassSpeed = 300 / (fabs( lastLowpass - lowpassChase)+1.0);
	double highpassSpeed = 300 / (fabs( lastHighpass - highpassChase)+1.0);
	double wetSpeed = 300 / (fabs( lastWet - wetChase)+1.0);
	lastLowpass = lowpassChase;
	lastHighpass = highpassChase;
	lastWet = wetChase;
	
	double invLowpass;
	double invHighpass;
	double dry;
	
	
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

		lowpassAmount = (((lowpassAmount*lowpassSpeed)+lowpassChase)/(lowpassSpeed + 1.0)); invLowpass = 1.0 - lowpassAmount;
		highpassAmount = (((highpassAmount*highpassSpeed)+highpassChase)/(highpassSpeed + 1.0)); invHighpass = 1.0 - highpassAmount;
		wet = (((wet*wetSpeed)+wetChase)/(wetSpeed+1.0)); dry = 1.0 - wet;
		
		count++; if (count > 5) count = 0; switch (count)
		{
			case 0:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassBL = (iirHighpassBL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
				iirLowpassBL = (iirLowpassBL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
				iirHighpassDL = (iirHighpassDL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassDL;
				iirLowpassDL = (iirLowpassDL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassDL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassBR = (iirHighpassBR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
				iirLowpassBR = (iirLowpassBR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
				iirHighpassDR = (iirHighpassDR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassDR;
				iirLowpassDR = (iirLowpassDR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassDR;
				break;
			case 1:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassCL = (iirHighpassCL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
				iirLowpassCL = (iirLowpassCL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
				iirHighpassEL = (iirHighpassEL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassEL;
				iirLowpassEL = (iirLowpassEL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassEL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassCR = (iirHighpassCR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
				iirLowpassCR = (iirLowpassCR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
				iirHighpassER = (iirHighpassER * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassER;
				iirLowpassER = (iirLowpassER * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassER;
				break;
			case 2:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassBL = (iirHighpassBL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
				iirLowpassBL = (iirLowpassBL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
				iirHighpassFL = (iirHighpassFL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassFL;
				iirLowpassFL = (iirLowpassFL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassFL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassBR = (iirHighpassBR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
				iirLowpassBR = (iirLowpassBR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
				iirHighpassFR = (iirHighpassFR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassFR;
				iirLowpassFR = (iirLowpassFR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassFR;
				break;
			case 3:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassCL = (iirHighpassCL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
				iirLowpassCL = (iirLowpassCL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
				iirHighpassDL = (iirHighpassDL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassDL;
				iirLowpassDL = (iirLowpassDL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassDL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassCR = (iirHighpassCR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
				iirLowpassCR = (iirLowpassCR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
				iirHighpassDR = (iirHighpassDR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassDR;
				iirLowpassDR = (iirLowpassDR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassDR;
				break;
			case 4:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassBL = (iirHighpassBL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassBL;
				iirLowpassBL = (iirLowpassBL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassBL;
				iirHighpassEL = (iirHighpassEL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassEL;
				iirLowpassEL = (iirLowpassEL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassEL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassBR = (iirHighpassBR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassBR;
				iirLowpassBR = (iirLowpassBR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassBR;
				iirHighpassER = (iirHighpassER * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassER;
				iirLowpassER = (iirLowpassER * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassER;
				break;
			case 5:
				iirHighpassAL = (iirHighpassAL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassAL;
				iirLowpassAL = (iirLowpassAL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassAL;
				iirHighpassCL = (iirHighpassCL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassCL;
				iirLowpassCL = (iirLowpassCL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassCL;
				iirHighpassFL = (iirHighpassFL * invHighpass) + (inputSampleL * highpassAmount); inputSampleL -= iirHighpassFL;
				iirLowpassFL = (iirLowpassFL * invLowpass) + (inputSampleL * lowpassAmount); inputSampleL = iirLowpassFL;
				iirHighpassAR = (iirHighpassAR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassAR;
				iirLowpassAR = (iirLowpassAR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassAR;
				iirHighpassCR = (iirHighpassCR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassCR;
				iirLowpassCR = (iirLowpassCR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassCR;
				iirHighpassFR = (iirHighpassFR * invHighpass) + (inputSampleR * highpassAmount); inputSampleR -= iirHighpassFR;
				iirLowpassFR = (iirLowpassFR * invLowpass) + (inputSampleR * lowpassAmount); inputSampleR = iirLowpassFR;
				break;
		}
		//Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
		//steepens the filter after minimizing artifacts.
		
		inputSampleL = (drySampleL * dry) + (inputSampleL * wet);
		inputSampleR = (drySampleR * dry) + (inputSampleR * wet);
		
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