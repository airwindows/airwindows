/* ========================================
 *  Dynamics3 - Dynamics3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Dynamics3_H
#include "Dynamics3.h"
#endif

void Dynamics3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	//begin Dynamics3
	double bezThresh = pow(A+0.6180339887498949,2.0)*1.6180339887498949;
	double sqrThresh = sqrt(bezThresh);
	double bezRez = fmax(pow((1.0-B)*0.5,4.0)/overallscale,0.0001); 
	bezRez /= (2.0/pow(overallscale,0.5-((overallscale-1.0)*0.0375)));
	int stepped = 999999; if (bezRez > 0.000001) stepped = (int)(1.0/bezRez);
	bezRez = 1.0 / stepped;
	double bezTrim = 1.0-(bezRez*((double)stepped/(stepped+1.0)));
	double sloRez = fmax(pow((1.0-C)*0.5,4.0)/overallscale,0.00001);
	sloRez /= (2.0/pow(overallscale,0.5-((overallscale-1.0)*0.0375)));
	double bezCompDry = pow(1.0-D,1.6180339887498949);
	if (bezThresh > 4.236) bezCompDry = 0.0;
	//end Dynamics3	

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Dynamics3
		inputSampleL *= (bezComp[bez_comp]/bezThresh);
		inputSampleR *= (bezComp[bez_comp]/bezThresh);
		double ctrl = fmin(fmax(fabs(inputSampleL),fabs(inputSampleR)),sqrThresh*bezComp[bez_comp]*0.6180339887498949);
		bezComp[bez_min] = fmax(bezComp[bez_min]-sloRez,ctrl);
		bezComp[bez_Ctrl] += (bezComp[bez_min] * bezRez);
		bezComp[bez_cycle] += bezRez;
		if (bezComp[bez_cycle] > 1.0) {bezComp[bez_cycle] = 0.0;
			bezComp[bez_C] = bezComp[bez_B]; bezComp[bez_B] = bezComp[bez_A];
			bezComp[bez_A] = bezComp[bez_Ctrl]; bezComp[bez_Ctrl] = 0.0;}
		double X = bezComp[bez_cycle]*bezTrim;
		bezComp[bez_comp] = bezComp[bez_B]+(bezComp[bez_C]*(1.0-X)*(1.0-X))+(bezComp[bez_B]*2.0*(1.0-X)*X)+(bezComp[bez_A]*X*X);
		bezComp[bez_comp] = ((1.0-(fmin(bezComp[bez_comp],1.0))));
		inputSampleL = (drySampleL*bezCompDry*(1.0-(bezComp[bez_comp]*(1.0-bezCompDry))))+(inputSampleL*(1.0-bezCompDry)*bezComp[bez_comp]*bezThresh);
		inputSampleR = (drySampleR*bezCompDry*(1.0-(bezComp[bez_comp]*(1.0-bezCompDry))))+(inputSampleR*(1.0-bezCompDry)*bezComp[bez_comp]*bezThresh);
		//end Dynamics3
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Dynamics3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	//begin Dynamics3
	double bezThresh = pow(A+0.6180339887498949,2.0)*1.6180339887498949;
	double sqrThresh = sqrt(bezThresh);
	double bezRez = fmax(pow((1.0-B)*0.5,4.0)/overallscale,0.0001); 
	bezRez /= (2.0/pow(overallscale,0.5-((overallscale-1.0)*0.0375)));
	int stepped = 999999; if (bezRez > 0.000001) stepped = (int)(1.0/bezRez);
	bezRez = 1.0 / stepped;
	double bezTrim = 1.0-(bezRez*((double)stepped/(stepped+1.0)));
	double sloRez = fmax(pow((1.0-C)*0.5,4.0)/overallscale,0.00001);
	sloRez /= (2.0/pow(overallscale,0.5-((overallscale-1.0)*0.0375)));
	double bezCompDry = pow(1.0-D,1.6180339887498949);
	if (bezThresh > 4.236) bezCompDry = 0.0;
	//end Dynamics3	

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Dynamics3
		inputSampleL *= (bezComp[bez_comp]/bezThresh);
		inputSampleR *= (bezComp[bez_comp]/bezThresh);
		double ctrl = fmin(fmax(fabs(inputSampleL),fabs(inputSampleR)),sqrThresh*bezComp[bez_comp]*0.6180339887498949);
		bezComp[bez_min] = fmax(bezComp[bez_min]-sloRez,ctrl);
		bezComp[bez_Ctrl] += (bezComp[bez_min] * bezRez);
		bezComp[bez_cycle] += bezRez;
		if (bezComp[bez_cycle] > 1.0) {bezComp[bez_cycle] = 0.0;
			bezComp[bez_C] = bezComp[bez_B]; bezComp[bez_B] = bezComp[bez_A];
			bezComp[bez_A] = bezComp[bez_Ctrl]; bezComp[bez_Ctrl] = 0.0;}
		double X = bezComp[bez_cycle]*bezTrim;
		bezComp[bez_comp] = bezComp[bez_B]+(bezComp[bez_C]*(1.0-X)*(1.0-X))+(bezComp[bez_B]*2.0*(1.0-X)*X)+(bezComp[bez_A]*X*X);
		bezComp[bez_comp] = ((1.0-(fmin(bezComp[bez_comp],1.0))));
		inputSampleL = (drySampleL*bezCompDry*(1.0-(bezComp[bez_comp]*(1.0-bezCompDry))))+(inputSampleL*(1.0-bezCompDry)*bezComp[bez_comp]*bezThresh);
		inputSampleR = (drySampleR*bezCompDry*(1.0-(bezComp[bez_comp]*(1.0-bezCompDry))))+(inputSampleR*(1.0-bezCompDry)*bezComp[bez_comp]*bezThresh);
		//end Dynamics3
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
