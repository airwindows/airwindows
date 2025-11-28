/* ========================================
 *  PurestDualPan - PurestDualPan.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestDualPan_H
#include "PurestDualPan.h"
#endif

void PurestDualPan::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	
	gainLA = gainLB; gainLB = A;
	panLA = panLB; panLB = B;
	gainRA = gainRB; gainRB = C;
	panRA = panRB; panRB = D;
	
    while (--sampleFrames >= 0)
    {
		long double inputSampleL = *in1;
		long double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		double temp = (double)sampleFrames/inFramesToProcess;
		long double LpanR = ((panLA*temp) + (panLB*(1.0-temp)))*1.57079633;
		long double LpanL = 1.57079633 - LpanR;
		LpanR = sin(LpanR); LpanL = sin(LpanL);
		long double gainL = ((gainLA*temp) + (gainLB*(1.0-temp)))*2.0;
		gainL = pow(gainL,gainL + 0.618033988749894848204586);
		long double RpanR = ((panRA*temp) + (panRB*(1.0-temp)))*1.57079633;
		long double RpanL = 1.57079633 - RpanR;
		RpanR = sin(RpanR); RpanL = sin(RpanL);
		long double gainR = ((gainRA*temp) + (gainRB*(1.0-temp)))*2.0;
		gainR = pow(gainR,gainR + 0.618033988749894848204586);
		long double LoutL = LpanL*gainL*inputSampleL;
		long double LoutR = LpanR*gainL*inputSampleL;
		long double RoutL = RpanL*gainR*inputSampleR;
		long double RoutR = RpanR*gainR*inputSampleR;
		inputSampleL = LoutL + RoutL;
		inputSampleR = LoutR + RoutR;
		
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

void PurestDualPan::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	
	gainLA = gainLB; gainLB = A;
	panLA = panLB; panLB = B;
	gainRA = gainRB; gainRB = C;
	panRA = panRB; panRB = D;
	
    while (--sampleFrames >= 0)
    {
		long double inputSampleL = *in1;
		long double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		double temp = (double)sampleFrames/inFramesToProcess;
		long double LpanR = ((panLA*temp) + (panLB*(1.0-temp)))*1.57079633;
		long double LpanL = 1.57079633 - LpanR;
		LpanR = sin(LpanR); LpanL = sin(LpanL);
		long double gainL = ((gainLA*temp) + (gainLB*(1.0-temp)))*2.0;
		gainL = pow(gainL,gainL + 0.618033988749894848204586);
		long double RpanR = ((panRA*temp) + (panRB*(1.0-temp)))*1.57079633;
		long double RpanL = 1.57079633 - RpanR;
		RpanR = sin(RpanR); RpanL = sin(RpanL);
		long double gainR = ((gainRA*temp) + (gainRB*(1.0-temp)))*2.0;
		gainR = pow(gainR,gainR + 0.618033988749894848204586);
		long double LoutL = LpanL*gainL*inputSampleL;
		long double LoutR = LpanR*gainL*inputSampleL;
		long double RoutL = RpanL*gainR*inputSampleR;
		long double RoutR = RpanR*gainR*inputSampleR;
		inputSampleL = LoutL + RoutL;
		inputSampleR = LoutR + RoutR;
		
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
