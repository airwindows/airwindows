/* ========================================
 *  FastDistance2 - FastDistance2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __FastDistance2_H
#include "FastDistance2.h"
#endif

void FastDistance2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	float distance = pow(1.0f-A,overallscale+1.0f);
	
    while (--sampleFrames >= 0)
    {
		float inputSampleL = *in1;
		float inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		inputSampleL *= 0.5f; //gain stage as part of the design
		inputSampleR *= 0.5f; //gain stage as part of the design
		//this can be omitted in a game engine and then you scale the
		//entire output by -6dB at the end, rather than constantly
		//having to make everything *0.5f
		
		fdlE = distance*0.875f*(inputSampleL-fdlA-fdlE) + fdlE;
		fdlF = distance*(fdlA-fdlB-fdlF) + fdlF;
		fdlG = distance*(fdlB-fdlC-fdlG) + fdlG;
		fdlH = distance*(fdlC-fdlD-fdlH) + fdlH;
		
		fdlA = distance*0.875f*fdlE + fdlA;
		fdlB = distance*fdlF + fdlB;
		fdlC = distance*fdlG + fdlC;
		fdlD = distance*fdlH + fdlD;
		
		inputSampleL = fdlC+fdlD; 
		
		fdrE = distance*0.875f*(inputSampleR-fdrA-fdrE) + fdrE;
		fdrF = distance*(fdrA-fdrB-fdrF) + fdrF;
		fdrG = distance*(fdrB-fdrC-fdrG) + fdrG;
		fdrH = distance*(fdrC-fdrD-fdrH) + fdrH;
		
		fdrA = distance*0.875f*fdrE + fdrA;
		fdrB = distance*fdrF + fdrB;
		fdrC = distance*fdrG + fdrC;
		fdrD = distance*fdrH + fdrD;
		
		inputSampleR = fdrC+fdrD; 
		
		//somewhere, scale by -6dB
		
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

void FastDistance2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	float distance = pow(1.0f-A,overallscale+1.0f);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		inputSampleL *= 0.5f; //gain stage as part of the design
		inputSampleR *= 0.5f; //gain stage as part of the design
		//this can be omitted in a game engine and then you scale the
		//entire output by -6dB at the end, rather than constantly
		//having to make everything *0.5f
		
		fdlE = distance*0.875f*(inputSampleL-fdlA-fdlE) + fdlE;
		fdlF = distance*(fdlA-fdlB-fdlF) + fdlF;
		fdlG = distance*(fdlB-fdlC-fdlG) + fdlG;
		fdlH = distance*(fdlC-fdlD-fdlH) + fdlH;
		
		fdlA = distance*0.875f*fdlE + fdlA;
		fdlB = distance*fdlF + fdlB;
		fdlC = distance*fdlG + fdlC;
		fdlD = distance*fdlH + fdlD;
		
		inputSampleL = fdlC+fdlD; 
		
		fdrE = distance*0.875f*(inputSampleR-fdrA-fdrE) + fdrE;
		fdrF = distance*(fdrA-fdrB-fdrF) + fdrF;
		fdrG = distance*(fdrB-fdrC-fdrG) + fdrG;
		fdrH = distance*(fdrC-fdrD-fdrH) + fdrH;
		
		fdrA = distance*0.875f*fdrE + fdrA;
		fdrB = distance*fdrF + fdrB;
		fdrC = distance*fdrG + fdrC;
		fdrD = distance*fdrH + fdrD;
		
		inputSampleR = fdrC+fdrD; 
		
		//somewhere, scale by -6dB
		
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
