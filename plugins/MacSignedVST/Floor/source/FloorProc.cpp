/* ========================================
 *  Floor - Floor.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Floor_H
#include "Floor.h"
#endif

void Floor::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double setting = pow(A,2);
	double iirAmount = (setting/4.0)/overallscale;
	double tight = -1.0;
	double gaintrim = 1.0 + (setting/4.0);
	double offset;
	double lows;
	double density = B;
	double bridgerectifier;
	double temp;
	iirAmount += (iirAmount * tight * tight);
	tight /=  3.0;
	if (iirAmount <= 0.0) iirAmount = 0.0;
	if (iirAmount > 1.0) iirAmount = 1.0;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin left channel
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1AL = (iirSample1AL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1AL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
				
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1BL = (iirSample1BL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1BL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1CL = (iirSample1CL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1CL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1DL = (iirSample1DL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1DL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1EL = (iirSample1EL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1EL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		//end left channel
		
		//begin right channel
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1AR = (iirSample1AR * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1AR;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1BR = (iirSample1BR * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1BR;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1CR = (iirSample1CR * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1CR;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1DR = (iirSample1DR * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1DR;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1ER = (iirSample1ER * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1ER;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		//end right channel
				
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		bridgerectifier = fabs(inputSampleL)*1.57079633;
		bridgerectifier = sin(bridgerectifier)*1.57079633;
		bridgerectifier = (fabs(inputSampleL)*(1-density))+(bridgerectifier*density);
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-density))+(bridgerectifier*density);
		else inputSampleL = (inputSampleL*(1-density))-(bridgerectifier*density);
		//drive section, left
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		bridgerectifier = sin(bridgerectifier)*1.57079633;
		bridgerectifier = (fabs(inputSampleR)*(1-density))+(bridgerectifier*density);
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-density))+(bridgerectifier*density);
		else inputSampleR = (inputSampleR*(1-density))-(bridgerectifier*density);
		//drive section, right
		
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

void Floor::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double setting = pow(A,2);
	double iirAmount = (setting/4.0)/overallscale;
	double tight = -1.0;
	double gaintrim = 1.0 + (setting/4.0);
	double offset;
	double lows;
	double density = B;
	double bridgerectifier;
	double temp;
	iirAmount += (iirAmount * tight * tight);
	tight /=  3.0;
	if (iirAmount <= 0.0) iirAmount = 0.0;
	if (iirAmount > 1.0) iirAmount = 1.0;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin left channel
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1AL = (iirSample1AL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1AL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1BL = (iirSample1BL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1BL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1CL = (iirSample1CL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1CL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1DL = (iirSample1DL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1DL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleL)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1EL = (iirSample1EL * (1 - (offset * iirAmount))) + (inputSampleL * (offset * iirAmount));
		lows = iirSample1EL;
		inputSampleL -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleL += lows;
		inputSampleL *= gaintrim;
		//end left channel
		
		//begin right channel
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1AR = (iirSample1AR * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1AR;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1BR = (iirSample1BR * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1BR;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1CR = (iirSample1CR * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1CR;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1DR = (iirSample1DR * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1DR;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSampleR)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1ER = (iirSample1ER * (1 - (offset * iirAmount))) + (inputSampleR * (offset * iirAmount));
		lows = iirSample1ER;
		inputSampleR -= lows;
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		inputSampleR += lows;
		inputSampleR *= gaintrim;
		//end right channel
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		bridgerectifier = fabs(inputSampleL)*1.57079633;
		bridgerectifier = sin(bridgerectifier)*1.57079633;
		bridgerectifier = (fabs(inputSampleL)*(1-density))+(bridgerectifier*density);
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-density))+(bridgerectifier*density);
		else inputSampleL = (inputSampleL*(1-density))-(bridgerectifier*density);
		//drive section, left
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		bridgerectifier = sin(bridgerectifier)*1.57079633;
		bridgerectifier = (fabs(inputSampleR)*(1-density))+(bridgerectifier*density);
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-density))+(bridgerectifier*density);
		else inputSampleR = (inputSampleR*(1-density))-(bridgerectifier*density);
		//drive section, right
		
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
