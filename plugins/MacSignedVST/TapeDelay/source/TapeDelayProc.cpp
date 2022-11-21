/* ========================================
 *  TapeDelay - TapeDelay.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TapeDelay_H
#include "TapeDelay.h"
#endif

void TapeDelay::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double dry = pow(A,2);
	double wet = pow(B,2);
	int targetdelay = (int)(44000*C);
	double feedback = (D*1.3);
	double leanfat = ((E*2.0)-1.0);
	double fatwet = fabs(leanfat);
	int fatness = (int)floor((F*29.0)+3.0);
	int count;
	
	double storedelayL;
	double storedelayR;
	double sumL = 0.0;
	double sumR = 0.0;
	double floattotalL = 0.0;
	double floattotalR = 0.0;
	int sumtotalL = 0;
	int sumtotalR = 0;
	
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (gcount < 0 || gcount > 128) {gcount = 128;}
		count = gcount;
		if (delay < 0 || delay > maxdelay) {delay = maxdelay;}
		
		sumL = inputSampleL + (dL[delay]*feedback);
		sumR = inputSampleR + (dR[delay]*feedback);
		pL[count+128] = pL[count] = sumtotalL = (int)(sumL*8388608.0);
		pR[count+128] = pR[count] = sumtotalR = (int)(sumR*8388608.0);
		
		switch (fatness)
		{
			case 32: sumtotalL += pL[count+127]; sumtotalR += pR[count+127]; //note NO break statement.
			case 31: sumtotalL += pL[count+113]; sumtotalR += pR[count+113]; //This jumps to the relevant tap
			case 30: sumtotalL += pL[count+109]; sumtotalR += pR[count+109]; //and then includes all smaller taps.
			case 29: sumtotalL += pL[count+107]; sumtotalR += pR[count+107];
			case 28: sumtotalL += pL[count+103]; sumtotalR += pR[count+103];
			case 27: sumtotalL += pL[count+101]; sumtotalR += pR[count+101];
			case 26: sumtotalL += pL[count+97]; sumtotalR += pR[count+97];
			case 25: sumtotalL += pL[count+89]; sumtotalR += pR[count+89];
			case 24: sumtotalL += pL[count+83]; sumtotalR += pR[count+83];
			case 23: sumtotalL += pL[count+79]; sumtotalR += pR[count+79];
			case 22: sumtotalL += pL[count+73]; sumtotalR += pR[count+73];
			case 21: sumtotalL += pL[count+71]; sumtotalR += pR[count+71];
			case 20: sumtotalL += pL[count+67]; sumtotalR += pR[count+67];
			case 19: sumtotalL += pL[count+61]; sumtotalR += pR[count+61];
			case 18: sumtotalL += pL[count+59]; sumtotalR += pR[count+59];
			case 17: sumtotalL += pL[count+53]; sumtotalR += pR[count+53];
			case 16: sumtotalL += pL[count+47]; sumtotalR += pR[count+47];
			case 15: sumtotalL += pL[count+43]; sumtotalR += pR[count+43];
			case 14: sumtotalL += pL[count+41]; sumtotalR += pR[count+41];
			case 13: sumtotalL += pL[count+37]; sumtotalR += pR[count+37];
			case 12: sumtotalL += pL[count+31]; sumtotalR += pR[count+31];
			case 11: sumtotalL += pL[count+29]; sumtotalR += pR[count+29];
			case 10: sumtotalL += pL[count+23]; sumtotalR += pR[count+23];
			case 9: sumtotalL += pL[count+19]; sumtotalR += pR[count+19];
			case 8: sumtotalL += pL[count+17]; sumtotalR += pR[count+17];
			case 7: sumtotalL += pL[count+13]; sumtotalR += pR[count+13];
			case 6: sumtotalL += pL[count+11]; sumtotalR += pR[count+11];
			case 5: sumtotalL += pL[count+7]; sumtotalR += pR[count+7];
			case 4: sumtotalL += pL[count+5]; sumtotalR += pR[count+5];
			case 3: sumtotalL += pL[count+3]; sumtotalR += pR[count+3];
			case 2: sumtotalL += pL[count+2]; sumtotalR += pR[count+2];
			case 1: sumtotalL += pL[count+1]; sumtotalR += pR[count+1];
		}
		
		floattotalL = (double)(sumtotalL/fatness+1);
		floattotalR = (double)(sumtotalR/fatness+1);
		floattotalL /= 8388608.0;
		floattotalR /= 8388608.0;
		floattotalL *= fatwet;
		floattotalR *= fatwet;
		if (leanfat < 0) {storedelayL = sumL-floattotalL; storedelayR = sumR-floattotalR;}
		else {storedelayL = (sumL * (1-fatwet))+floattotalL; storedelayR = (sumR * (1-fatwet))+floattotalR;}
		
		chase += abs(maxdelay - targetdelay);
		if (chase > 9000)
		{
			if (maxdelay > targetdelay) {
				dL[delay] = storedelayL; dR[delay] = storedelayR;
				maxdelay -= 1; delay -= 1; if (delay < 0) {delay = maxdelay;}
				dL[delay] = storedelayL; dR[delay] = storedelayR;
			}
			if (maxdelay < targetdelay) {
				maxdelay += 1; delay += 1; if (delay > maxdelay) {delay = 0;}
				dL[delay] = storedelayL; dR[delay] = storedelayR;
			}
			chase = 0;
		}
		else
		{
			dL[delay] = storedelayL; dR[delay] = storedelayR;
		}
		
		gcount--;
		delay--;
		if (delay < 0 || delay > maxdelay) {delay = maxdelay;}
		//yes this is a second bounds check. it's cheap, check EVERY time
		
		inputSampleL = (inputSampleL * dry) + (dL[delay] * wet);		
		inputSampleR = (inputSampleR * dry) + (dR[delay] * wet);				
		
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

void TapeDelay::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double dry = pow(A,2);
	double wet = pow(B,2);
	int targetdelay = (int)(44000*C);
	double feedback = (D*1.3);
	double leanfat = ((E*2.0)-1.0);
	double fatwet = fabs(leanfat);
	int fatness = (int)floor((F*29.0)+3.0);
	int count;
	
	double storedelayL;
	double storedelayR;
	double sumL = 0.0;
	double sumR = 0.0;
	double floattotalL = 0.0;
	double floattotalR = 0.0;
	int sumtotalL = 0;
	int sumtotalR = 0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (gcount < 0 || gcount > 128) {gcount = 128;}
		count = gcount;
		if (delay < 0 || delay > maxdelay) {delay = maxdelay;}
		
		sumL = inputSampleL + (dL[delay]*feedback);
		sumR = inputSampleR + (dR[delay]*feedback);
		pL[count+128] = pL[count] = sumtotalL = (int)(sumL*8388608.0);
		pR[count+128] = pR[count] = sumtotalR = (int)(sumR*8388608.0);
		
		switch (fatness)
		{
			case 32: sumtotalL += pL[count+127]; sumtotalR += pR[count+127]; //note NO break statement.
			case 31: sumtotalL += pL[count+113]; sumtotalR += pR[count+113]; //This jumps to the relevant tap
			case 30: sumtotalL += pL[count+109]; sumtotalR += pR[count+109]; //and then includes all smaller taps.
			case 29: sumtotalL += pL[count+107]; sumtotalR += pR[count+107];
			case 28: sumtotalL += pL[count+103]; sumtotalR += pR[count+103];
			case 27: sumtotalL += pL[count+101]; sumtotalR += pR[count+101];
			case 26: sumtotalL += pL[count+97]; sumtotalR += pR[count+97];
			case 25: sumtotalL += pL[count+89]; sumtotalR += pR[count+89];
			case 24: sumtotalL += pL[count+83]; sumtotalR += pR[count+83];
			case 23: sumtotalL += pL[count+79]; sumtotalR += pR[count+79];
			case 22: sumtotalL += pL[count+73]; sumtotalR += pR[count+73];
			case 21: sumtotalL += pL[count+71]; sumtotalR += pR[count+71];
			case 20: sumtotalL += pL[count+67]; sumtotalR += pR[count+67];
			case 19: sumtotalL += pL[count+61]; sumtotalR += pR[count+61];
			case 18: sumtotalL += pL[count+59]; sumtotalR += pR[count+59];
			case 17: sumtotalL += pL[count+53]; sumtotalR += pR[count+53];
			case 16: sumtotalL += pL[count+47]; sumtotalR += pR[count+47];
			case 15: sumtotalL += pL[count+43]; sumtotalR += pR[count+43];
			case 14: sumtotalL += pL[count+41]; sumtotalR += pR[count+41];
			case 13: sumtotalL += pL[count+37]; sumtotalR += pR[count+37];
			case 12: sumtotalL += pL[count+31]; sumtotalR += pR[count+31];
			case 11: sumtotalL += pL[count+29]; sumtotalR += pR[count+29];
			case 10: sumtotalL += pL[count+23]; sumtotalR += pR[count+23];
			case 9: sumtotalL += pL[count+19]; sumtotalR += pR[count+19];
			case 8: sumtotalL += pL[count+17]; sumtotalR += pR[count+17];
			case 7: sumtotalL += pL[count+13]; sumtotalR += pR[count+13];
			case 6: sumtotalL += pL[count+11]; sumtotalR += pR[count+11];
			case 5: sumtotalL += pL[count+7]; sumtotalR += pR[count+7];
			case 4: sumtotalL += pL[count+5]; sumtotalR += pR[count+5];
			case 3: sumtotalL += pL[count+3]; sumtotalR += pR[count+3];
			case 2: sumtotalL += pL[count+2]; sumtotalR += pR[count+2];
			case 1: sumtotalL += pL[count+1]; sumtotalR += pR[count+1];
		}
		
		floattotalL = (double)(sumtotalL/fatness+1);
		floattotalR = (double)(sumtotalR/fatness+1);
		floattotalL /= 8388608.0;
		floattotalR /= 8388608.0;
		floattotalL *= fatwet;
		floattotalR *= fatwet;
		if (leanfat < 0) {storedelayL = sumL-floattotalL; storedelayR = sumR-floattotalR;}
		else {storedelayL = (sumL * (1-fatwet))+floattotalL; storedelayR = (sumR * (1-fatwet))+floattotalR;}
		
		chase += abs(maxdelay - targetdelay);
		if (chase > 9000)
		{
			if (maxdelay > targetdelay) {
				dL[delay] = storedelayL; dR[delay] = storedelayR;
				maxdelay -= 1; delay -= 1; if (delay < 0) {delay = maxdelay;}
				dL[delay] = storedelayL; dR[delay] = storedelayR;
			}
			if (maxdelay < targetdelay) {
				maxdelay += 1; delay += 1; if (delay > maxdelay) {delay = 0;}
				dL[delay] = storedelayL; dR[delay] = storedelayR;
			}
			chase = 0;
		}
		else
		{
			dL[delay] = storedelayL; dR[delay] = storedelayR;
		}
		
		gcount--;
		delay--;
		if (delay < 0 || delay > maxdelay) {delay = maxdelay;}
		//yes this is a second bounds check. it's cheap, check EVERY time
		
		inputSampleL = (inputSampleL * dry) + (dL[delay] * wet);		
		inputSampleR = (inputSampleR * dry) + (dR[delay] * wet);				
		
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
