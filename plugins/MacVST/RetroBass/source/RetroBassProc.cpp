/* ========================================
 *  RetroBass - RetroBass.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __RetroBass_H
#include "RetroBass.h"
#endif

void RetroBass::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double rbAmount = pow(A*0.5,2.0)/overallscale;
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		rbSampleL = (rbSampleL * (1.0-rbAmount))+(inputSampleL*rbAmount);
		if (fabs(rbSampleL)>1.18e-37) {
			double bip_delta = inputSampleL; //delta can be just local and re-used
			inputSampleL *= rbSampleL*0.96;
			inputSampleL = ((inputSampleL*inputSampleL) * -0.166666666666666)*(inputSampleL*inputSampleL) + inputSampleL;
			inputSampleL /= rbSampleL*0.96;
			bip[bip_drbLA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
			bip[bip_drbLB] = bip[bip_prbLA]-bip[bip_drbLA]; bip[bip_prbLA] = bip[bip_drbLA];//velocity
			bip[bip_drbLC] = bip[bip_prbLB]-bip[bip_drbLB]; bip[bip_prbLB] = bip[bip_drbLB];//acceleration
			bip[bip_drbLD] = bip[bip_prbLC]-bip[bip_drbLC]; bip[bip_prbLC] = bip[bip_drbLC];//jerk
			double bip_drbE = bip[bip_prbLD]-bip[bip_drbLD]; bip[bip_prbLD] = bip[bip_drbLD];//snap
			inputSampleL *= (1.0+(fabs(bip[bip_drbLC])*0.0618)+(fabs(bip[bip_drbLD])*-0.05982)+(fabs(bip_drbE)*0.0206));
		}
		inputSampleL -= rbSampleL*0.92;
		
		rbSampleR = (rbSampleR * (1.0-rbAmount))+(inputSampleR*rbAmount);
		if (fabs(rbSampleR)>1.18e-37) {
			double bip_delta = inputSampleR; //delta can be just local and re-used
			inputSampleR *= rbSampleR*0.96;
			inputSampleR = ((inputSampleR*inputSampleR) * -0.166666666666666)*(inputSampleR*inputSampleR) + inputSampleR;
			inputSampleR /= rbSampleR*0.96;
			bip[bip_drbRA] = bip_delta - inputSampleR; // these are derivatives: raw clip is position
			bip[bip_drbRB] = bip[bip_prbRA]-bip[bip_drbRA]; bip[bip_prbRA] = bip[bip_drbRA];//velocity
			bip[bip_drbRC] = bip[bip_prbRB]-bip[bip_drbRB]; bip[bip_prbRB] = bip[bip_drbRB];//acceleration
			bip[bip_drbRD] = bip[bip_prbRC]-bip[bip_drbRC]; bip[bip_prbRC] = bip[bip_drbRC];//jerk
			double bip_drbE = bip[bip_prbRD]-bip[bip_drbRD]; bip[bip_prbRD] = bip[bip_drbRD];//snap
			inputSampleR *= (1.0+(fabs(bip[bip_drbRC])*0.0618)+(fabs(bip[bip_drbRD])*-0.05982)+(fabs(bip_drbE)*0.0206));
		}
		inputSampleR -= rbSampleR*0.92;
		
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

void RetroBass::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double rbAmount = pow(A*0.5,2.0)/overallscale;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		rbSampleL = (rbSampleL * (1.0-rbAmount))+(inputSampleL*rbAmount);
		if (fabs(rbSampleL)>1.18e-37) {
			double bip_delta = inputSampleL; //delta can be just local and re-used
			inputSampleL *= rbSampleL*0.96;
			inputSampleL = ((inputSampleL*inputSampleL) * -0.166666666666666)*(inputSampleL*inputSampleL) + inputSampleL;
			inputSampleL /= rbSampleL*0.96;
			bip[bip_drbLA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
			bip[bip_drbLB] = bip[bip_prbLA]-bip[bip_drbLA]; bip[bip_prbLA] = bip[bip_drbLA];//velocity
			bip[bip_drbLC] = bip[bip_prbLB]-bip[bip_drbLB]; bip[bip_prbLB] = bip[bip_drbLB];//acceleration
			bip[bip_drbLD] = bip[bip_prbLC]-bip[bip_drbLC]; bip[bip_prbLC] = bip[bip_drbLC];//jerk
			double bip_drbE = bip[bip_prbLD]-bip[bip_drbLD]; bip[bip_prbLD] = bip[bip_drbLD];//snap
			inputSampleL *= (1.0+(fabs(bip[bip_drbLC])*0.0618)+(fabs(bip[bip_drbLD])*-0.05982)+(fabs(bip_drbE)*0.0206));
		}
		inputSampleL -= rbSampleL*0.92;
		
		rbSampleR = (rbSampleR * (1.0-rbAmount))+(inputSampleR*rbAmount);
		if (fabs(rbSampleR)>1.18e-37) {
			double bip_delta = inputSampleR; //delta can be just local and re-used
			inputSampleR *= rbSampleR*0.96;
			inputSampleR = ((inputSampleR*inputSampleR) * -0.166666666666666)*(inputSampleR*inputSampleR) + inputSampleR;
			inputSampleR /= rbSampleR*0.96;
			bip[bip_drbRA] = bip_delta - inputSampleR; // these are derivatives: raw clip is position
			bip[bip_drbRB] = bip[bip_prbRA]-bip[bip_drbRA]; bip[bip_prbRA] = bip[bip_drbRA];//velocity
			bip[bip_drbRC] = bip[bip_prbRB]-bip[bip_drbRB]; bip[bip_prbRB] = bip[bip_drbRB];//acceleration
			bip[bip_drbRD] = bip[bip_prbRC]-bip[bip_drbRC]; bip[bip_prbRC] = bip[bip_drbRC];//jerk
			double bip_drbE = bip[bip_prbRD]-bip[bip_drbRD]; bip[bip_prbRD] = bip[bip_drbRD];//snap
			inputSampleR *= (1.0+(fabs(bip[bip_drbRC])*0.0618)+(fabs(bip[bip_drbRD])*-0.05982)+(fabs(bip_drbE)*0.0206));
		}
		inputSampleR -= rbSampleR*0.92;
		
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
