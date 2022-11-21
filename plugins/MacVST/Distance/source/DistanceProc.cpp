/* ========================================
 *  Distance - Distance.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Distance_H
#include "Distance.h"
#endif

void Distance::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double softslew = (pow(A*2.0,3.0)*12.0)+0.6;
	softslew *= overallscale;
	double filtercorrect = softslew / 2.0;
	double thirdfilter = softslew / 3.0;
	double levelcorrect = 1.0 + (softslew / 6.0);
	double postfilter;
	double wet = B;
	double dry = 1.0-wet;
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
		
		inputSampleL *= softslew;
		lastclampL = clampL;
		clampL = inputSampleL - lastL;
		postfilter = changeL = fabs(clampL - lastclampL);
		postfilter += filtercorrect;
		if (changeL > 1.5707963267949) changeL = 1.5707963267949;
		bridgerectifier = (1.0-sin(changeL));
		if (bridgerectifier < 0.0) bridgerectifier = 0.0;
		inputSampleL = lastL + (clampL * bridgerectifier);
		lastL = inputSampleL;
		inputSampleL /= softslew;
		inputSampleL += (thirdresultL * thirdfilter);
		inputSampleL /= (thirdfilter + 1.0);
		inputSampleL += (prevresultL * postfilter);
		inputSampleL /= (postfilter + 1.0);
		//do an IIR like thing to further squish superdistant stuff
		thirdresultL = prevresultL;
		prevresultL = inputSampleL;
		inputSampleL *= levelcorrect;
		
		inputSampleR *= softslew;
		lastclampR = clampR;
		clampR = inputSampleR - lastR;
		postfilter = changeR = fabs(clampR - lastclampR);
		postfilter += filtercorrect;
		if (changeR > 1.5707963267949) changeR = 1.5707963267949;
		bridgerectifier = (1.0-sin(changeR));
		if (bridgerectifier < 0.0) bridgerectifier = 0.0;
		inputSampleR = lastR + (clampR * bridgerectifier);
		lastR = inputSampleR;
		inputSampleR /= softslew;
		inputSampleR += (thirdresultR * thirdfilter);
		inputSampleR /= (thirdfilter + 1.0);
		inputSampleR += (prevresultR * postfilter);
		inputSampleR /= (postfilter + 1.0);
		//do an IIR like thing to further squish superdistant stuff
		thirdresultR = prevresultR;
		prevresultR = inputSampleR;
		inputSampleR *= levelcorrect;
		
		if (wet < 1.0) {
			inputSampleL = (drySampleL * dry)+(inputSampleL*wet);
			inputSampleR = (drySampleR * dry)+(inputSampleR*wet);
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

void Distance::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double softslew = (pow(A*2.0,3.0)*12.0)+0.6;
	softslew *= overallscale;
	double filtercorrect = softslew / 2.0;
	double thirdfilter = softslew / 3.0;
	double levelcorrect = 1.0 + (softslew / 6.0);
	double postfilter;
	double wet = B;
	double dry = 1.0-wet;
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

		inputSampleL *= softslew;
		lastclampL = clampL;
		clampL = inputSampleL - lastL;
		postfilter = changeL = fabs(clampL - lastclampL);
		postfilter += filtercorrect;
		if (changeL > 1.5707963267949) changeL = 1.5707963267949;
		bridgerectifier = (1.0-sin(changeL));
		if (bridgerectifier < 0.0) bridgerectifier = 0.0;
		inputSampleL = lastL + (clampL * bridgerectifier);
		lastL = inputSampleL;
		inputSampleL /= softslew;
		inputSampleL += (thirdresultL * thirdfilter);
		inputSampleL /= (thirdfilter + 1.0);
		inputSampleL += (prevresultL * postfilter);
		inputSampleL /= (postfilter + 1.0);
		//do an IIR like thing to further squish superdistant stuff
		thirdresultL = prevresultL;
		prevresultL = inputSampleL;
		inputSampleL *= levelcorrect;
		
		inputSampleR *= softslew;
		lastclampR = clampR;
		clampR = inputSampleR - lastR;
		postfilter = changeR = fabs(clampR - lastclampR);
		postfilter += filtercorrect;
		if (changeR > 1.5707963267949) changeR = 1.5707963267949;
		bridgerectifier = (1.0-sin(changeR));
		if (bridgerectifier < 0.0) bridgerectifier = 0.0;
		inputSampleR = lastR + (clampR * bridgerectifier);
		lastR = inputSampleR;
		inputSampleR /= softslew;
		inputSampleR += (thirdresultR * thirdfilter);
		inputSampleR /= (thirdfilter + 1.0);
		inputSampleR += (prevresultR * postfilter);
		inputSampleR /= (postfilter + 1.0);
		//do an IIR like thing to further squish superdistant stuff
		thirdresultR = prevresultR;
		prevresultR = inputSampleR;
		inputSampleR *= levelcorrect;
		
		if (wet < 1.0) {
			inputSampleL = (drySampleL * dry)+(inputSampleL*wet);
			inputSampleR = (drySampleR * dry)+(inputSampleR*wet);
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