/* ========================================
 *  Facet - Facet.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Facet_H
#include "Facet.h"
#endif

void Facet::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double pos = A;
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		
		
		
		if (inputSampleL > pos) {
			inputSampleL = ((inputSampleL-pos)*pos)+pos;
		} //facet algorithm: put a corner distortion in producing an artifact
		if (inputSampleL < -pos) {
			inputSampleL = ((inputSampleL+pos)*pos)-pos;
		} //we increasingly produce a sharp 'angle' in the transfer function
		
		if (inputSampleR > pos) {
			inputSampleR = ((inputSampleR-pos)*pos)+pos;
		} //facet algorithm: put a corner distortion in producing an artifact
		if (inputSampleR < -pos) {
			inputSampleR = ((inputSampleR+pos)*pos)-pos;
		} //we increasingly produce a sharp 'angle' in the transfer function
		
		
		
		
		
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

void Facet::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double pos = A;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		
		
		
		if (inputSampleL > pos) {
			inputSampleL = ((inputSampleL-pos)*pos)+pos;
		} //facet algorithm: put a corner distortion in producing an artifact
		if (inputSampleL < -pos) {
			inputSampleL = ((inputSampleL+pos)*pos)-pos;
		} //we increasingly produce a sharp 'angle' in the transfer function
		
		if (inputSampleR > pos) {
			inputSampleR = ((inputSampleR-pos)*pos)+pos;
		} //facet algorithm: put a corner distortion in producing an artifact
		if (inputSampleR < -pos) {
			inputSampleR = ((inputSampleR+pos)*pos)-pos;
		} //we increasingly produce a sharp 'angle' in the transfer function
		
		
		
		
		
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
