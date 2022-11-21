/* ========================================
 *  ButterComp - ButterComp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ButterComp_H
#include "ButterComp.h"
#endif

void ButterComp::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double inputposL;
	double inputnegL;
	double calcposL;
	double calcnegL;
	double outputposL;
	double outputnegL;
	double totalmultiplierL;
	double inputSampleL;
	double drySampleL;

	double inputposR;
	double inputnegR;
	double calcposR;
	double calcnegR;
	double outputposR;
	double outputnegR;
	double totalmultiplierR;
	double inputSampleR;
	double drySampleR;

	double inputgain = pow(10.0,(A*14.0)/20.0);
	double wet = B;
	//removed extra dry variable
	double outputgain = inputgain;
	outputgain -= 1.0;
	outputgain /= 1.5;
	outputgain += 1.0;
	double divisor = 0.012 * (A / 135.0);
	divisor /= overallscale;
	double remainder = divisor;
	divisor = 1.0 - divisor;
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL *= inputgain;
		inputSampleR *= inputgain;
		
		inputposL = inputSampleL + 1.0;
		if (inputposL < 0.0) inputposL = 0.0;
		outputposL = inputposL / 2.0;
		if (outputposL > 1.0) outputposL = 1.0;		
		inputposL *= inputposL;
		targetposL *= divisor;
		targetposL += (inputposL * remainder);
		calcposL = pow((1.0/targetposL),2);
		
		inputnegL = (-inputSampleL) + 1.0;
		if (inputnegL < 0.0) inputnegL = 0.0;
		outputnegL = inputnegL / 2.0;
		if (outputnegL > 1.0) outputnegL = 1.0;		
		inputnegL *= inputnegL;
		targetnegL *= divisor;
		targetnegL += (inputnegL * remainder);
		calcnegL = pow((1.0/targetnegL),2);
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		inputposR = inputSampleR + 1.0;
		if (inputposR < 0.0) inputposR = 0.0;
		outputposR = inputposR / 2.0;
		if (outputposR > 1.0) outputposR = 1.0;		
		inputposR *= inputposR;
		targetposR *= divisor;
		targetposR += (inputposR * remainder);
		calcposR = pow((1.0/targetposR),2);
		
		inputnegR = (-inputSampleR) + 1.0;
		if (inputnegR < 0.0) inputnegR = 0.0;
		outputnegR = inputnegR / 2.0;
		if (outputnegR > 1.0) outputnegR = 1.0;		
		inputnegR *= inputnegR;
		targetnegR *= divisor;
		targetnegR += (inputnegR * remainder);
		calcnegR = pow((1.0/targetnegR),2);
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		
		if (inputSampleL > 0)
		{ //working on pos
			controlAposL *= divisor;
			controlAposL += (calcposL*remainder);
		}
		else
		{ //working on neg
			controlAnegL *= divisor;
			controlAnegL += (calcnegL*remainder);
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		if (inputSampleR > 0)
		{ //working on pos
			controlAposR *= divisor;
			controlAposR += (calcposR*remainder);
		}
		else
		{ //working on neg
			controlAnegR *= divisor;
			controlAnegR += (calcnegR*remainder);
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		totalmultiplierL = (controlAposL * outputposL) + (controlAnegL * outputnegL);
		totalmultiplierR = (controlAposR * outputposR) + (controlAnegR * outputnegR);
		//this combines the sides according to flip, blending relative to the input value
		
		inputSampleL *= totalmultiplierL;
		inputSampleL /= outputgain;
		
		inputSampleR *= totalmultiplierR;
		inputSampleR /= outputgain;
		
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

void ButterComp::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputposL;
	double inputnegL;
	double calcposL;
	double calcnegL;
	double outputposL;
	double outputnegL;
	double totalmultiplierL;
	double inputSampleL;
	double drySampleL;
	
	double inputposR;
	double inputnegR;
	double calcposR;
	double calcnegR;
	double outputposR;
	double outputnegR;
	double totalmultiplierR;
	double inputSampleR;
	double drySampleR;
	
	double inputgain = pow(10.0,(A*14.0)/20.0);
	double wet = B;
	//removed extra dry variable
	double outputgain = inputgain;
	outputgain -= 1.0;
	outputgain /= 1.5;
	outputgain += 1.0;
	double divisor = 0.012 * (A / 135.0);
	divisor /= overallscale;
	double remainder = divisor;
	divisor = 1.0 - divisor;
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL *= inputgain;
		inputSampleR *= inputgain;
		
		inputposL = inputSampleL + 1.0;
		if (inputposL < 0.0) inputposL = 0.0;
		outputposL = inputposL / 2.0;
		if (outputposL > 1.0) outputposL = 1.0;		
		inputposL *= inputposL;
		targetposL *= divisor;
		targetposL += (inputposL * remainder);
		calcposL = pow((1.0/targetposL),2);
		
		inputnegL = (-inputSampleL) + 1.0;
		if (inputnegL < 0.0) inputnegL = 0.0;
		outputnegL = inputnegL / 2.0;
		if (outputnegL > 1.0) outputnegL = 1.0;		
		inputnegL *= inputnegL;
		targetnegL *= divisor;
		targetnegL += (inputnegL * remainder);
		calcnegL = pow((1.0/targetnegL),2);
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		inputposR = inputSampleR + 1.0;
		if (inputposR < 0.0) inputposR = 0.0;
		outputposR = inputposR / 2.0;
		if (outputposR > 1.0) outputposR = 1.0;		
		inputposR *= inputposR;
		targetposR *= divisor;
		targetposR += (inputposR * remainder);
		calcposR = pow((1.0/targetposR),2);
		
		inputnegR = (-inputSampleR) + 1.0;
		if (inputnegR < 0.0) inputnegR = 0.0;
		outputnegR = inputnegR / 2.0;
		if (outputnegR > 1.0) outputnegR = 1.0;		
		inputnegR *= inputnegR;
		targetnegR *= divisor;
		targetnegR += (inputnegR * remainder);
		calcnegR = pow((1.0/targetnegR),2);
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		
		if (inputSampleL > 0)
		{ //working on pos
			controlAposL *= divisor;
			controlAposL += (calcposL*remainder);
		}
		else
		{ //working on neg
			controlAnegL *= divisor;
			controlAnegL += (calcnegL*remainder);
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		if (inputSampleR > 0)
		{ //working on pos
			controlAposR *= divisor;
			controlAposR += (calcposR*remainder);
		}
		else
		{ //working on neg
			controlAnegR *= divisor;
			controlAnegR += (calcnegR*remainder);
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		totalmultiplierL = (controlAposL * outputposL) + (controlAnegL * outputnegL);
		totalmultiplierR = (controlAposR * outputposR) + (controlAnegR * outputnegR);
		//this combines the sides according to flip, blending relative to the input value
		
		inputSampleL *= totalmultiplierL;
		inputSampleL /= outputgain;
		
		inputSampleR *= totalmultiplierR;
		inputSampleR /= outputgain;
		
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