/* ========================================
 *  SubTight - SubTight.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SubTight_H
#include "SubTight.h"
#endif

void SubTight::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int subStages = pow(B,2)*16.0;
	if (subStages < 1) subStages = 1;
	double subTrim = pow((A*0.3)+(pow(B,2)*0.2),subStages)/overallscale;
	//to use this as an analog modeler for restricting digital lows, find set values that still show bass
	//Note that this is best used sparingly, on the 'not enough subtraction' side of the node.

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//you want subStages and subTrim to be hardcoded values when embedding this into something else
		//then it only needs the sub[] array, and to have it initialized to 0.0
		
		//begin SubTight section
		double subSampleL = inputSampleL * subTrim;
		double subSampleR = inputSampleR * subTrim;
		for (int x = 0; x < subStages; x++) {
			double scale = 0.5+fabs(subSampleL*0.5);
			subSampleL = (subL[x]+(sin(subL[x]-subSampleL)*scale));
			subL[x] = subSampleL*scale;
			scale = 0.5+fabs(subSampleR*0.5);
			subSampleR = (subR[x]+(sin(subR[x]-subSampleR)*scale));
			subR[x] = subSampleR*scale;
		}
		if (subStages % 2 > 0) {
			subSampleL = -subSampleL;
			subSampleR = -subSampleR;
		}
		if (subSampleL > 0.25) subSampleL = 0.25;
		if (subSampleL < -0.25) subSampleL = -0.25;
		if (subSampleR > 0.25) subSampleR = 0.25;
		if (subSampleR < -0.25) subSampleR = -0.25;
		inputSampleL -= (subSampleL*16.0);
		inputSampleR -= (subSampleR*16.0);
		//end SubTight section
		
		//cut the level WAY down, then the modified Creature code blows up subs.
		//the adjustment of scale destabilizes the routine to blow up more DC.
		//this is boosted by 24dB and subtracted from the dry signal
		
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

void SubTight::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int subStages = pow(B,2)*16.0;
	if (subStages < 1) subStages = 1;
	double subTrim = pow((A*0.3)+(pow(B,2)*0.2),subStages)/overallscale;
	//to use this as an analog modeler for restricting digital lows, find set values that still show bass
	//Note that this is best used sparingly, on the 'not enough subtraction' side of the node.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//you want subStages and subTrim to be hardcoded values when embedding this into something else
		//then it only needs the sub[] array, and to have it initialized to 0.0
		
		//begin SubTight section
		double subSampleL = inputSampleL * subTrim;
		double subSampleR = inputSampleR * subTrim;
		for (int x = 0; x < subStages; x++) {
			double scale = 0.5+fabs(subSampleL*0.5);
			subSampleL = (subL[x]+(sin(subL[x]-subSampleL)*scale));
			subL[x] = subSampleL*scale;
			scale = 0.5+fabs(subSampleR*0.5);
			subSampleR = (subR[x]+(sin(subR[x]-subSampleR)*scale));
			subR[x] = subSampleR*scale;
		}
		if (subStages % 2 > 0) {
			subSampleL = -subSampleL;
			subSampleR = -subSampleR;
		}
		if (subSampleL > 0.25) subSampleL = 0.25;
		if (subSampleL < -0.25) subSampleL = -0.25;
		if (subSampleR > 0.25) subSampleR = 0.25;
		if (subSampleR < -0.25) subSampleR = -0.25;
		inputSampleL -= (subSampleL*16.0);
		inputSampleR -= (subSampleR*16.0);
		//end SubTight section
		
		//cut the level WAY down, then the modified Creature code blows up subs.
		//the adjustment of scale destabilizes the routine to blow up more DC.
		//this is boosted by 24dB and subtracted from the dry signal
		
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
