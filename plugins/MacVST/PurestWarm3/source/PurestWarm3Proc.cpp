/* ========================================
 *  PurestWarm3 - PurestWarm3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestWarm3_H
#include "PurestWarm3.h"
#endif

void PurestWarm3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double asym = (A-0.5)*8.0;
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (asym != 0.0) {
			double bip_delta = inputSampleL; //delta can be just local and re-used
			inputSampleL *= asym;
			inputSampleL = ((inputSampleL*inputSampleL) * -0.166666666666666)*(inputSampleL*inputSampleL) + inputSampleL;
			inputSampleL /= asym;
			bip[bip_dvLA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
			bip[bip_dvLB] = bip[bip_pvLA]-bip[bip_dvLA]; bip[bip_pvLA] = bip[bip_dvLA];//velocity
			bip[bip_dvLC] = bip[bip_pvLB]-bip[bip_dvLB]; bip[bip_pvLB] = bip[bip_dvLB];//acceleration
			bip[bip_dvLD] = bip[bip_pvLC]-bip[bip_dvLC]; bip[bip_pvLC] = bip[bip_dvLC];//jerk
			double bip_dvE = bip[bip_pvLD]-bip[bip_dvLD]; bip[bip_pvLD] = bip[bip_dvLD];//snap
			inputSampleL *= (1.0+(fabs(bip[bip_dvLC])*0.0618)+(fabs(bip[bip_dvLD])*-0.05982)+(fabs(bip_dvE)*0.0206));
			
			bip_delta = inputSampleR; //delta can be just local and re-used
			inputSampleR *= asym;
			inputSampleR = ((inputSampleR*inputSampleR) * -0.166666666666666)*(inputSampleR*inputSampleR) + inputSampleR;
			inputSampleR /= asym;
			bip[bip_dvRA] = bip_delta - inputSampleR; // these are derivatives: raw clip is position
			bip[bip_dvRB] = bip[bip_pvRA]-bip[bip_dvRA]; bip[bip_pvRA] = bip[bip_dvRA];//velocity
			bip[bip_dvRC] = bip[bip_pvRB]-bip[bip_dvRB]; bip[bip_pvRB] = bip[bip_dvRB];//acceleration
			bip[bip_dvRD] = bip[bip_pvRC]-bip[bip_dvRC]; bip[bip_pvRC] = bip[bip_dvRC];//jerk
			bip_dvE = bip[bip_pvRD]-bip[bip_dvRD]; bip[bip_pvRD] = bip[bip_dvRD];//snap
			inputSampleR *= (1.0+(fabs(bip[bip_dvRC])*0.0618)+(fabs(bip[bip_dvRD])*-0.05982)+(fabs(bip_dvE)*0.0206));
		}

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

void PurestWarm3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double asym = (A-0.5)*8.0;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (asym != 0.0) {
			double bip_delta = inputSampleL; //delta can be just local and re-used
			inputSampleL *= asym;
			inputSampleL = ((inputSampleL*inputSampleL) * -0.166666666666666)*(inputSampleL*inputSampleL) + inputSampleL;
			inputSampleL /= asym;
			bip[bip_dvLA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
			bip[bip_dvLB] = bip[bip_pvLA]-bip[bip_dvLA]; bip[bip_pvLA] = bip[bip_dvLA];//velocity
			bip[bip_dvLC] = bip[bip_pvLB]-bip[bip_dvLB]; bip[bip_pvLB] = bip[bip_dvLB];//acceleration
			bip[bip_dvLD] = bip[bip_pvLC]-bip[bip_dvLC]; bip[bip_pvLC] = bip[bip_dvLC];//jerk
			double bip_dvE = bip[bip_pvLD]-bip[bip_dvLD]; bip[bip_pvLD] = bip[bip_dvLD];//snap
			inputSampleL *= (1.0+(fabs(bip[bip_dvLC])*0.0618)+(fabs(bip[bip_dvLD])*-0.05982)+(fabs(bip_dvE)*0.0206));
			
			bip_delta = inputSampleR; //delta can be just local and re-used
			inputSampleR *= asym;
			inputSampleR = ((inputSampleR*inputSampleR) * -0.166666666666666)*(inputSampleR*inputSampleR) + inputSampleR;
			inputSampleR /= asym;
			bip[bip_dvRA] = bip_delta - inputSampleR; // these are derivatives: raw clip is position
			bip[bip_dvRB] = bip[bip_pvRA]-bip[bip_dvRA]; bip[bip_pvRA] = bip[bip_dvRA];//velocity
			bip[bip_dvRC] = bip[bip_pvRB]-bip[bip_dvRB]; bip[bip_pvRB] = bip[bip_dvRB];//acceleration
			bip[bip_dvRD] = bip[bip_pvRC]-bip[bip_dvRC]; bip[bip_pvRC] = bip[bip_dvRC];//jerk
			bip_dvE = bip[bip_pvRD]-bip[bip_dvRD]; bip[bip_pvRD] = bip[bip_dvRD];//snap
			inputSampleR *= (1.0+(fabs(bip[bip_dvRC])*0.0618)+(fabs(bip[bip_dvRD])*-0.05982)+(fabs(bip_dvE)*0.0206));
		}
		
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
