/* ========================================
 *  BezEQ4 - BezEQ4.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BezEQ4_H
#include "BezEQ4.h"
#endif

void BezEQ4::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	//begin BezEQ4 control setup
	double highGain = (pow(A+0.5,3.0)+2.0)*0.3333333;
	double hmidGain = (pow(B+0.5,3.0)+2.0)*0.3333333;
	double lmidGain = (pow(C+0.5,3.0)+2.0)*0.3333333;
	double bassGain = (pow(D+0.5,3.0)+2.0)*0.3333333;
	double trebleRef = E-0.3;
	double highmidRef = F-0.5;
	double lowmidRef = G-0.7;
	double bassRef = H-0.9;
	double HMderez = 0.75 + (trebleRef*0.25);
	double LMderez = 0.25 + (bassRef*0.25);
	double Mderez = ((HMderez+LMderez+highmidRef+lowmidRef)*0.25);
	HMderez /= overallscale; Mderez /= overallscale; LMderez /= overallscale;
	if (HMderez > 1.0) HMderez = 1.0;
	int stepped = 999999; if (HMderez > 0.000001) stepped = (int)(1.0/HMderez);
	HMderez = 0.99999999 / stepped;
	double HMtrim = 1.0-(HMderez*((double)stepped/(stepped+1.0)));
	stepped = 999999; if (Mderez > 0.000001) stepped = (int)(1.0/Mderez);
	Mderez = 0.99999999 / stepped;
	double Mtrim = 1.0-(Mderez*((double)stepped/(stepped+1.0)));
	stepped = 999999; if (LMderez > 0.000001) stepped = (int)(1.0/LMderez);
	LMderez = 0.99999999 / stepped;
	double LMtrim = 1.0-(LMderez*((double)stepped/(stepped+1.0)));
	double highQ = 0.84+((highmidRef-trebleRef)*0.1666666);
	double midQ = 0.84+((lowmidRef-highmidRef)*0.1666666);
	double lowQ = 0.84+((bassRef-lowmidRef)*0.1666666);
	//end BezEQ4 control setup
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin BezEQ4
		for (int p = 0; p < 3; p++) {
			double highL = inputSampleL;
			double highR = inputSampleR;
			bezEQ[bez_HMcycle][p] += HMderez;
			bezEQ[bez_HMAL][p] += (highL * HMderez);
			bezEQ[bez_HMAR][p] += (highR * HMderez);
			if (bezEQ[bez_HMcycle][p] > HMtrim) {
				bezEQ[bez_HMDL][p] = bezEQ[bez_HMCL][p]; bezEQ[bez_HMCL][p] = bezEQ[bez_HMBL][p]; 
				bezEQ[bez_HMBL][p] = bezEQ[bez_HMAL][p]*(0.5-(HMderez*0.082));
				bezEQ[bez_HMAL][p] = 0.0; bezEQ[bez_HMcycle][p] = 0.0;
				bezEQ[bez_HMDR][p] = bezEQ[bez_HMCR][p]; bezEQ[bez_HMCR][p] = bezEQ[bez_HMBR][p]; 
				bezEQ[bez_HMBR][p] = bezEQ[bez_HMAR][p]*(0.5-(HMderez*0.082));
				bezEQ[bez_HMAR][p] = 0.0; bezEQ[bez_HMcycle][p] = 0.0;
			}
			double X = bezEQ[bez_HMcycle][p];
			double hmidL = bezEQ[bez_HMCL][p]+(bezEQ[bez_HMDL][p]*(1.0-X)*(1.0-X));
			hmidL += (bezEQ[bez_HMCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBL][p]*X*X);
			hmidL *= highQ;
			highL -= hmidL; //final high and hmid
			double hmidR = bezEQ[bez_HMCR][p]+(bezEQ[bez_HMDR][p]*(1.0-X)*(1.0-X));
			hmidR += (bezEQ[bez_HMCR][p]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBR][p]*X*X);
			hmidR *= highQ;
			highR -= hmidR; //final high and hmid
			bezEQ[bez_Mcycle][p] += Mderez;
			bezEQ[bez_MAL][p] += (hmidL * Mderez);
			bezEQ[bez_MAR][p] += (hmidR * Mderez);
			if (bezEQ[bez_Mcycle][p] > Mtrim) {
				bezEQ[bez_MDL][p] = bezEQ[bez_MCL][p]; bezEQ[bez_MCL][p] = bezEQ[bez_MBL][p]; 
				bezEQ[bez_MBL][p] = bezEQ[bez_MAL][p]*(0.5-(Mderez*0.082));
				bezEQ[bez_MAL][p] = 0.0; bezEQ[bez_Mcycle][p] = 0.0;
				bezEQ[bez_MDR][p] = bezEQ[bez_MCR][p]; bezEQ[bez_MCR][p] = bezEQ[bez_MBR][p]; 
				bezEQ[bez_MBR][p] = bezEQ[bez_MAR][p]*(0.5-(Mderez*0.082));
				bezEQ[bez_MAR][p] = 0.0; bezEQ[bez_Mcycle][p] = 0.0;
			}
			X = bezEQ[bez_Mcycle][p];
			double lmidL = bezEQ[bez_MCL][p]+(bezEQ[bez_MDL][p]*(1.0-X)*(1.0-X));
			lmidL += (bezEQ[bez_MCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_MBL][p]*X*X);
			lmidL *= midQ;
			hmidL -= lmidL; //final hmid and lmid
			double lmidR = bezEQ[bez_MCR][p]+(bezEQ[bez_MDR][p]*(1.0-X)*(1.0-X));
			lmidR += (bezEQ[bez_MCR][p]*2.0*(1.0-X)*X)+(bezEQ[bez_MBR][p]*X*X);
			lmidR *= midQ;
			hmidR -= lmidR; //final hmid and lmid
			bezEQ[bez_LMcycle][p] += LMderez;
			bezEQ[bez_LMAL][p] += (lmidL * LMderez);
			bezEQ[bez_LMAR][p] += (lmidR * LMderez);
			if (bezEQ[bez_LMcycle][p] > LMtrim) {
				bezEQ[bez_LMDL][p] = bezEQ[bez_LMCL][p]; bezEQ[bez_LMCL][p] = bezEQ[bez_LMBL][p]; 
				bezEQ[bez_LMBL][p] = bezEQ[bez_LMAL][p]*(0.5-(LMderez*0.082));
				bezEQ[bez_LMAL][p] = 0.0; bezEQ[bez_LMcycle][p] = 0.0;
				bezEQ[bez_LMDR][p] = bezEQ[bez_LMCR][p]; bezEQ[bez_LMCR][p] = bezEQ[bez_LMBR][p]; 
				bezEQ[bez_LMBR][p] = bezEQ[bez_LMAR][p]*(0.5-(LMderez*0.082));
				bezEQ[bez_LMAR][p] = 0.0; bezEQ[bez_LMcycle][p] = 0.0;
			}
			X = bezEQ[bez_LMcycle][p];
			double bassL = bezEQ[bez_LMCL][p]+(bezEQ[bez_LMDL][p]*(1.0-X)*(1.0-X));
			bassL += (bezEQ[bez_LMCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBL][p]*X*X);
			bassL *= lowQ;
			lmidL -= bassL; //final lmid and bass
			double bassR = bezEQ[bez_LMCR][p]+(bezEQ[bez_LMDR][p]*(1.0-X)*(1.0-X));
			bassR += (bezEQ[bez_LMCR][p]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBR][p]*X*X);
			bassR *= lowQ;
			lmidR -= bassR; //final lmid and bass
			inputSampleL = (highL*highGain)+(hmidL*hmidGain)+(lmidL*lmidGain)+(bassL*bassGain);
			inputSampleR = (highR*highGain)+(hmidR*hmidGain)+(lmidR*lmidGain)+(bassR*bassGain);
		}
		//end BezEQ4
		
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

void BezEQ4::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	//begin BezEQ4 control setup
	double highGain = (pow(A+0.5,3.0)+2.0)*0.3333333;
	double hmidGain = (pow(B+0.5,3.0)+2.0)*0.3333333;
	double lmidGain = (pow(C+0.5,3.0)+2.0)*0.3333333;
	double bassGain = (pow(D+0.5,3.0)+2.0)*0.3333333;
	double trebleRef = E-0.3;
	double highmidRef = F-0.5;
	double lowmidRef = G-0.7;
	double bassRef = H-0.9;
	double HMderez = 0.75 + (trebleRef*0.25);
	double LMderez = 0.25 + (bassRef*0.25);
	double Mderez = ((HMderez+LMderez+highmidRef+lowmidRef)*0.25);
	HMderez /= overallscale; Mderez /= overallscale; LMderez /= overallscale;
	if (HMderez > 1.0) HMderez = 1.0;
	int stepped = 999999; if (HMderez > 0.000001) stepped = (int)(1.0/HMderez);
	HMderez = 0.99999999 / stepped;
	double HMtrim = 1.0-(HMderez*((double)stepped/(stepped+1.0)));
	stepped = 999999; if (Mderez > 0.000001) stepped = (int)(1.0/Mderez);
	Mderez = 0.99999999 / stepped;
	double Mtrim = 1.0-(Mderez*((double)stepped/(stepped+1.0)));
	stepped = 999999; if (LMderez > 0.000001) stepped = (int)(1.0/LMderez);
	LMderez = 0.99999999 / stepped;
	double LMtrim = 1.0-(LMderez*((double)stepped/(stepped+1.0)));
	double highQ = 0.84+((highmidRef-trebleRef)*0.1666666);
	double midQ = 0.84+((lowmidRef-highmidRef)*0.1666666);
	double lowQ = 0.84+((bassRef-lowmidRef)*0.1666666);
	//end BezEQ4 control setup
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin BezEQ4
		for (int p = 0; p < 3; p++) {
			double highL = inputSampleL;
			double highR = inputSampleR;
			bezEQ[bez_HMcycle][p] += HMderez;
			bezEQ[bez_HMAL][p] += (highL * HMderez);
			bezEQ[bez_HMAR][p] += (highR * HMderez);
			if (bezEQ[bez_HMcycle][p] > HMtrim) {
				bezEQ[bez_HMDL][p] = bezEQ[bez_HMCL][p]; bezEQ[bez_HMCL][p] = bezEQ[bez_HMBL][p]; 
				bezEQ[bez_HMBL][p] = bezEQ[bez_HMAL][p]*(0.5-(HMderez*0.082));
				bezEQ[bez_HMAL][p] = 0.0; bezEQ[bez_HMcycle][p] = 0.0;
				bezEQ[bez_HMDR][p] = bezEQ[bez_HMCR][p]; bezEQ[bez_HMCR][p] = bezEQ[bez_HMBR][p]; 
				bezEQ[bez_HMBR][p] = bezEQ[bez_HMAR][p]*(0.5-(HMderez*0.082));
				bezEQ[bez_HMAR][p] = 0.0; bezEQ[bez_HMcycle][p] = 0.0;
			}
			double X = bezEQ[bez_HMcycle][p];
			double hmidL = bezEQ[bez_HMCL][p]+(bezEQ[bez_HMDL][p]*(1.0-X)*(1.0-X));
			hmidL += (bezEQ[bez_HMCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBL][p]*X*X);
			hmidL *= highQ;
			highL -= hmidL; //final high and hmid
			double hmidR = bezEQ[bez_HMCR][p]+(bezEQ[bez_HMDR][p]*(1.0-X)*(1.0-X));
			hmidR += (bezEQ[bez_HMCR][p]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBR][p]*X*X);
			hmidR *= highQ;
			highR -= hmidR; //final high and hmid
			bezEQ[bez_Mcycle][p] += Mderez;
			bezEQ[bez_MAL][p] += (hmidL * Mderez);
			bezEQ[bez_MAR][p] += (hmidR * Mderez);
			if (bezEQ[bez_Mcycle][p] > Mtrim) {
				bezEQ[bez_MDL][p] = bezEQ[bez_MCL][p]; bezEQ[bez_MCL][p] = bezEQ[bez_MBL][p]; 
				bezEQ[bez_MBL][p] = bezEQ[bez_MAL][p]*(0.5-(Mderez*0.082));
				bezEQ[bez_MAL][p] = 0.0; bezEQ[bez_Mcycle][p] = 0.0;
				bezEQ[bez_MDR][p] = bezEQ[bez_MCR][p]; bezEQ[bez_MCR][p] = bezEQ[bez_MBR][p]; 
				bezEQ[bez_MBR][p] = bezEQ[bez_MAR][p]*(0.5-(Mderez*0.082));
				bezEQ[bez_MAR][p] = 0.0; bezEQ[bez_Mcycle][p] = 0.0;
			}
			X = bezEQ[bez_Mcycle][p];
			double lmidL = bezEQ[bez_MCL][p]+(bezEQ[bez_MDL][p]*(1.0-X)*(1.0-X));
			lmidL += (bezEQ[bez_MCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_MBL][p]*X*X);
			lmidL *= midQ;
			hmidL -= lmidL; //final hmid and lmid
			double lmidR = bezEQ[bez_MCR][p]+(bezEQ[bez_MDR][p]*(1.0-X)*(1.0-X));
			lmidR += (bezEQ[bez_MCR][p]*2.0*(1.0-X)*X)+(bezEQ[bez_MBR][p]*X*X);
			lmidR *= midQ;
			hmidR -= lmidR; //final hmid and lmid
			bezEQ[bez_LMcycle][p] += LMderez;
			bezEQ[bez_LMAL][p] += (lmidL * LMderez);
			bezEQ[bez_LMAR][p] += (lmidR * LMderez);
			if (bezEQ[bez_LMcycle][p] > LMtrim) {
				bezEQ[bez_LMDL][p] = bezEQ[bez_LMCL][p]; bezEQ[bez_LMCL][p] = bezEQ[bez_LMBL][p]; 
				bezEQ[bez_LMBL][p] = bezEQ[bez_LMAL][p]*(0.5-(LMderez*0.082));
				bezEQ[bez_LMAL][p] = 0.0; bezEQ[bez_LMcycle][p] = 0.0;
				bezEQ[bez_LMDR][p] = bezEQ[bez_LMCR][p]; bezEQ[bez_LMCR][p] = bezEQ[bez_LMBR][p]; 
				bezEQ[bez_LMBR][p] = bezEQ[bez_LMAR][p]*(0.5-(LMderez*0.082));
				bezEQ[bez_LMAR][p] = 0.0; bezEQ[bez_LMcycle][p] = 0.0;
			}
			X = bezEQ[bez_LMcycle][p];
			double bassL = bezEQ[bez_LMCL][p]+(bezEQ[bez_LMDL][p]*(1.0-X)*(1.0-X));
			bassL += (bezEQ[bez_LMCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBL][p]*X*X);
			bassL *= lowQ;
			lmidL -= bassL; //final lmid and bass
			double bassR = bezEQ[bez_LMCR][p]+(bezEQ[bez_LMDR][p]*(1.0-X)*(1.0-X));
			bassR += (bezEQ[bez_LMCR][p]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBR][p]*X*X);
			bassR *= lowQ;
			lmidR -= bassR; //final lmid and bass
			inputSampleL = (highL*highGain)+(hmidL*hmidGain)+(lmidL*lmidGain)+(bassL*bassGain);
			inputSampleR = (highR*highGain)+(hmidR*hmidGain)+(lmidR*lmidGain)+(bassR*bassGain);
		}
		//end BezEQ4
		
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
