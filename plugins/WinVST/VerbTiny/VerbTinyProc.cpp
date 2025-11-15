/* ========================================
 *  VerbTiny - VerbTiny.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VerbTiny_H
#include "VerbTiny.h"
#endif

void VerbTiny::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double reg4n = 0.03125+((1.0-pow(1.0-A,2.0))*0.03125);
	double attenuate = 1.0 - (1.0-pow(1.0-A,2.0));
	double derez = pow(B,2.0);
	derez = fmin(fmax(derez/overallscale,0.0001),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	//the revision more accurately connects the bezier curves
	double derezFreq = pow(C,2.0);
	derezFreq = fmin(fmax(derezFreq/overallscale,0.0001),1.0);
	int bezFreqFraction = (int)(1.0/derezFreq);
	double bezFreqTrim = (double)bezFreqFraction/(bezFreqFraction+1.0);
	derezFreq = 1.0 / bezFreqFraction;
	bezFreqTrim = 1.0-(derezFreq*bezFreqTrim);
	//the revision more accurately connects the bezier curves
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleL*attenuate*derez);
		bez[bez_SampR] += (inputSampleR*attenuate*derez);
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
			a4AL[c4AL] = inputSampleL + (f4AL * reg4n);
			a4BL[c4BL] = inputSampleL + (f4BL * reg4n);
			a4CL[c4CL] = inputSampleL + (f4CL * reg4n);
			a4DL[c4DL] = inputSampleL + (f4DL * reg4n);
			
			c4AL++; if (c4AL < 0 || c4AL > d4A) c4AL = 0;
			c4BL++; if (c4BL < 0 || c4BL > d4B) c4BL = 0;
			c4CL++; if (c4CL < 0 || c4CL > d4C) c4CL = 0;
			c4DL++; if (c4DL < 0 || c4DL > d4D) c4DL = 0;
			
			double o4AL = a4AL[c4AL-((c4AL > d4A)?d4A+1:0)];
			double o4BL = a4BL[c4BL-((c4BL > d4B)?d4B+1:0)];
			double o4CL = a4CL[c4CL-((c4CL > d4C)?d4C+1:0)];
			double o4DL = a4DL[c4DL-((c4DL > d4D)?d4D+1:0)];
			
			a4DR[c4DR] = inputSampleR + (f4DR * reg4n);
			a4HR[c4HR] = inputSampleR + (f4HR * reg4n);
			a4LR[c4LR] = inputSampleR + (f4LR * reg4n);
			a4PR[c4PR] = inputSampleR + (f4PR * reg4n);
			
			c4DR++; if (c4DR < 0 || c4DR > d4D) c4DR = 0;
			c4HR++; if (c4HR < 0 || c4HR > d4H) c4HR = 0;
			c4LR++; if (c4LR < 0 || c4LR > d4L) c4LR = 0;
			c4PR++; if (c4PR < 0 || c4PR > d4P) c4PR = 0;
			
			double o4DR = a4DR[c4DR-((c4DR > d4D)?d4D+1:0)];
			double o4HR = a4HR[c4HR-((c4HR > d4H)?d4H+1:0)];
			double o4LR = a4LR[c4LR-((c4LR > d4L)?d4L+1:0)];
			double o4PR = a4PR[c4PR-((c4PR > d4P)?d4P+1:0)];
			
			//-------- one
			
			a4EL[c4EL] = o4AL - (o4BL + o4CL + o4DL);
			a4FL[c4FL] = o4BL - (o4AL + o4CL + o4DL);
			a4GL[c4GL] = o4CL - (o4AL + o4BL + o4DL);
			a4HL[c4HL] = o4DL - (o4AL + o4BL + o4CL);
			
			c4EL++; if (c4EL < 0 || c4EL > d4E) c4EL = 0;
			c4FL++; if (c4FL < 0 || c4FL > d4F) c4FL = 0;
			c4GL++; if (c4GL < 0 || c4GL > d4G) c4GL = 0;
			c4HL++; if (c4HL < 0 || c4HL > d4H) c4HL = 0;
			
			double o4EL = a4EL[c4EL-((c4EL > d4E)?d4E+1:0)];
			double o4FL = a4FL[c4FL-((c4FL > d4F)?d4F+1:0)];
			double o4GL = a4GL[c4GL-((c4GL > d4G)?d4G+1:0)];
			double o4HL = a4HL[c4HL-((c4HL > d4H)?d4H+1:0)];
			
			a4CR[c4CR] = o4DR - (o4HR + o4LR + o4PR);
			a4GR[c4GR] = o4HR - (o4DR + o4LR + o4PR);
			a4KR[c4KR] = o4LR - (o4DR + o4HR + o4PR);
			a4OR[c4OR] = o4PR - (o4DR + o4HR + o4LR);
			
			c4CR++; if (c4CR < 0 || c4CR > d4C) c4CR = 0;
			c4GR++; if (c4GR < 0 || c4GR > d4G) c4GR = 0;
			c4KR++; if (c4KR < 0 || c4KR > d4K) c4KR = 0;
			c4OR++; if (c4OR < 0 || c4OR > d4O) c4OR = 0;
			
			double o4CR = a4CR[c4CR-((c4CR > d4C)?d4C+1:0)];
			double o4GR = a4GR[c4GR-((c4GR > d4G)?d4G+1:0)];
			double o4KR = a4KR[c4KR-((c4KR > d4K)?d4K+1:0)];
			double o4OR = a4OR[c4OR-((c4OR > d4O)?d4O+1:0)];
			
			//-------- two
			
			a4IL[c4IL] = o4EL - (o4FL + o4GL + o4HL);
			a4JL[c4JL] = o4FL - (o4EL + o4GL + o4HL);
			a4KL[c4KL] = o4GL - (o4EL + o4FL + o4HL);
			a4LL[c4LL] = o4HL - (o4EL + o4FL + o4GL);
			
			c4IL++; if (c4IL < 0 || c4IL > d4I) c4IL = 0;
			c4JL++; if (c4JL < 0 || c4JL > d4J) c4JL = 0;
			c4KL++; if (c4KL < 0 || c4KL > d4K) c4KL = 0;
			c4LL++; if (c4LL < 0 || c4LL > d4L) c4LL = 0;
			
			double o4IL = a4IL[c4IL-((c4IL > d4I)?d4I+1:0)];
			double o4JL = a4JL[c4JL-((c4JL > d4J)?d4J+1:0)];
			double o4KL = a4KL[c4KL-((c4KL > d4K)?d4K+1:0)];
			double o4LL = a4LL[c4LL-((c4LL > d4L)?d4L+1:0)];
			
			a4BR[c4BR] = o4CR - (o4GR + o4KR + o4OR);
			a4FR[c4FR] = o4GR - (o4CR + o4KR + o4OR);
			a4JR[c4JR] = o4KR - (o4CR + o4GR + o4OR);
			a4NR[c4NR] = o4OR - (o4CR + o4GR + o4KR);
			
			c4BR++; if (c4BR < 0 || c4BR > d4B) c4BR = 0;
			c4FR++; if (c4FR < 0 || c4FR > d4F) c4FR = 0;
			c4JR++; if (c4JR < 0 || c4JR > d4J) c4JR = 0;
			c4NR++; if (c4NR < 0 || c4NR > d4N) c4NR = 0;
			
			double o4BR = a4BR[c4BR-((c4BR > d4B)?d4B+1:0)];
			double o4FR = a4FR[c4FR-((c4FR > d4F)?d4F+1:0)];
			double o4JR = a4JR[c4JR-((c4JR > d4J)?d4J+1:0)];
			double o4NR = a4NR[c4NR-((c4NR > d4N)?d4N+1:0)];
			
			//-------- three
			
			a4ML[c4ML] = o4IL - (o4JL + o4KL + o4LL);
			a4NL[c4NL] = o4JL - (o4IL + o4KL + o4LL);
			a4OL[c4OL] = o4KL - (o4IL + o4JL + o4LL);
			a4PL[c4PL] = o4LL - (o4IL + o4JL + o4KL);
			
			c4ML++; if (c4ML < 0 || c4ML > d4M) c4ML = 0;
			c4NL++; if (c4NL < 0 || c4NL > d4N) c4NL = 0;
			c4OL++; if (c4OL < 0 || c4OL > d4O) c4OL = 0;
			c4PL++; if (c4PL < 0 || c4PL > d4P) c4PL = 0;
			
			double o4ML = a4ML[c4ML-((c4ML > d4M)?d4M+1:0)];
			double o4NL = a4NL[c4NL-((c4NL > d4N)?d4N+1:0)];
			double o4OL = a4OL[c4OL-((c4OL > d4O)?d4O+1:0)];
			double o4PL = a4PL[c4PL-((c4PL > d4P)?d4P+1:0)];
			
			a4AR[c4AR] = o4BR - (o4FR + o4JR + o4NR);
			a4ER[c4ER] = o4FR - (o4BR + o4JR + o4NR);
			a4IR[c4IR] = o4JR - (o4BR + o4FR + o4NR);
			a4MR[c4MR] = o4NR - (o4BR + o4FR + o4JR);
			
			c4AR++; if (c4AR < 0 || c4AR > d4A) c4AR = 0;
			c4ER++; if (c4ER < 0 || c4ER > d4E) c4ER = 0;
			c4IR++; if (c4IR < 0 || c4IR > d4I) c4IR = 0;
			c4MR++; if (c4MR < 0 || c4MR > d4M) c4MR = 0;
			
			double o4AR = a4AR[c4AR-((c4AR > d4A)?d4A+1:0)];
			double o4ER = a4ER[c4ER-((c4ER > d4E)?d4E+1:0)];
			double o4IR = a4IR[c4IR-((c4IR > d4I)?d4I+1:0)];
			double o4MR = a4MR[c4MR-((c4MR > d4M)?d4M+1:0)];
			
			//-------- four
			
			f4AL = o4AR - (o4ER + o4IR + o4MR);				
			f4BL = o4ER - (o4AR + o4IR + o4MR);
			f4CL = o4IR - (o4AR + o4ER + o4MR);
			f4DL = o4MR - (o4AR + o4ER + o4IR);
			
			f4DR = o4ML - (o4NL + o4OL + o4PL);
			f4HR = o4NL - (o4ML + o4OL + o4PL);
			f4LR = o4OL - (o4ML + o4NL + o4PL);
			f4PR = o4PL - (o4ML + o4NL + o4OL);
			
			inputSampleL = (o4ML + o4NL + o4OL + o4PL)*0.125;
			inputSampleR = (o4AR + o4ER + o4IR + o4MR)*0.125;
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle]*bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		inputSampleL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*-0.25;
		inputSampleR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*-0.25;
		
		bezF[bez_cycle] += derezFreq;
		bezF[bez_SampL] += ((inputSampleL+bezF[bez_InL]) * derezFreq);
		bezF[bez_SampR] += ((inputSampleL+bezF[bez_InR]) * derezFreq);
		bezF[bez_InL] = inputSampleL; bezF[bez_InR] = inputSampleR;
		if (bezF[bez_cycle] > 1.0) { //hit the end point and we do a filter sample
			bezF[bez_cycle] = 0.0;
			bezF[bez_CL] = bezF[bez_BL];
			bezF[bez_BL] = bezF[bez_AL];
			bezF[bez_AL] = bezF[bez_SampL];
			bezF[bez_SampL] = 0.0;
			bezF[bez_CR] = bezF[bez_BR];
			bezF[bez_BR] = bezF[bez_AR];
			bezF[bez_AR] = bezF[bez_SampR];
			bezF[bez_SampR] = 0.0;
		}
		X = bezF[bez_cycle]*bezFreqTrim;
		double CBLfreq = (bezF[bez_CL]*(1.0-X))+(bezF[bez_BL]*X);
		double BALfreq = (bezF[bez_BL]*(1.0-X))+(bezF[bez_AL]*X);
		inputSampleL = (bezF[bez_BL]+(CBLfreq*(1.0-X))+(BALfreq*X))*0.25;
		double CBRfreq = (bezF[bez_CR]*(1.0-X))+(bezF[bez_BR]*X);
		double BARfreq = (bezF[bez_BR]*(1.0-X))+(bezF[bez_AR]*X);
		inputSampleR = (bezF[bez_BR]+(CBRfreq*(1.0-X))+(BARfreq*X))*0.25;
		//filtering the reverb separately, after making it
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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

void VerbTiny::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double reg4n = 0.03125+((1.0-pow(1.0-A,2.0))*0.03125);
	double attenuate = 1.0 - (1.0-pow(1.0-A,2.0));
	double derez = pow(B,2.0);
	derez = fmin(fmax(derez/overallscale,0.0001),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	//the revision more accurately connects the bezier curves
	double derezFreq = pow(C,2.0);
	derezFreq = fmin(fmax(derezFreq/overallscale,0.0001),1.0);
	int bezFreqFraction = (int)(1.0/derezFreq);
	double bezFreqTrim = (double)bezFreqFraction/(bezFreqFraction+1.0);
	derezFreq = 1.0 / bezFreqFraction;
	bezFreqTrim = 1.0-(derezFreq*bezFreqTrim);
	//the revision more accurately connects the bezier curves
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleL*attenuate*derez);
		bez[bez_SampR] += (inputSampleR*attenuate*derez);
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
			a4AL[c4AL] = inputSampleL + (f4AL * reg4n);
			a4BL[c4BL] = inputSampleL + (f4BL * reg4n);
			a4CL[c4CL] = inputSampleL + (f4CL * reg4n);
			a4DL[c4DL] = inputSampleL + (f4DL * reg4n);
			
			c4AL++; if (c4AL < 0 || c4AL > d4A) c4AL = 0;
			c4BL++; if (c4BL < 0 || c4BL > d4B) c4BL = 0;
			c4CL++; if (c4CL < 0 || c4CL > d4C) c4CL = 0;
			c4DL++; if (c4DL < 0 || c4DL > d4D) c4DL = 0;
			
			double o4AL = a4AL[c4AL-((c4AL > d4A)?d4A+1:0)];
			double o4BL = a4BL[c4BL-((c4BL > d4B)?d4B+1:0)];
			double o4CL = a4CL[c4CL-((c4CL > d4C)?d4C+1:0)];
			double o4DL = a4DL[c4DL-((c4DL > d4D)?d4D+1:0)];
			
			a4DR[c4DR] = inputSampleR + (f4DR * reg4n);
			a4HR[c4HR] = inputSampleR + (f4HR * reg4n);
			a4LR[c4LR] = inputSampleR + (f4LR * reg4n);
			a4PR[c4PR] = inputSampleR + (f4PR * reg4n);
			
			c4DR++; if (c4DR < 0 || c4DR > d4D) c4DR = 0;
			c4HR++; if (c4HR < 0 || c4HR > d4H) c4HR = 0;
			c4LR++; if (c4LR < 0 || c4LR > d4L) c4LR = 0;
			c4PR++; if (c4PR < 0 || c4PR > d4P) c4PR = 0;
			
			double o4DR = a4DR[c4DR-((c4DR > d4D)?d4D+1:0)];
			double o4HR = a4HR[c4HR-((c4HR > d4H)?d4H+1:0)];
			double o4LR = a4LR[c4LR-((c4LR > d4L)?d4L+1:0)];
			double o4PR = a4PR[c4PR-((c4PR > d4P)?d4P+1:0)];
			
			//-------- one
			
			a4EL[c4EL] = o4AL - (o4BL + o4CL + o4DL);
			a4FL[c4FL] = o4BL - (o4AL + o4CL + o4DL);
			a4GL[c4GL] = o4CL - (o4AL + o4BL + o4DL);
			a4HL[c4HL] = o4DL - (o4AL + o4BL + o4CL);
			
			c4EL++; if (c4EL < 0 || c4EL > d4E) c4EL = 0;
			c4FL++; if (c4FL < 0 || c4FL > d4F) c4FL = 0;
			c4GL++; if (c4GL < 0 || c4GL > d4G) c4GL = 0;
			c4HL++; if (c4HL < 0 || c4HL > d4H) c4HL = 0;
			
			double o4EL = a4EL[c4EL-((c4EL > d4E)?d4E+1:0)];
			double o4FL = a4FL[c4FL-((c4FL > d4F)?d4F+1:0)];
			double o4GL = a4GL[c4GL-((c4GL > d4G)?d4G+1:0)];
			double o4HL = a4HL[c4HL-((c4HL > d4H)?d4H+1:0)];
			
			a4CR[c4CR] = o4DR - (o4HR + o4LR + o4PR);
			a4GR[c4GR] = o4HR - (o4DR + o4LR + o4PR);
			a4KR[c4KR] = o4LR - (o4DR + o4HR + o4PR);
			a4OR[c4OR] = o4PR - (o4DR + o4HR + o4LR);
			
			c4CR++; if (c4CR < 0 || c4CR > d4C) c4CR = 0;
			c4GR++; if (c4GR < 0 || c4GR > d4G) c4GR = 0;
			c4KR++; if (c4KR < 0 || c4KR > d4K) c4KR = 0;
			c4OR++; if (c4OR < 0 || c4OR > d4O) c4OR = 0;
			
			double o4CR = a4CR[c4CR-((c4CR > d4C)?d4C+1:0)];
			double o4GR = a4GR[c4GR-((c4GR > d4G)?d4G+1:0)];
			double o4KR = a4KR[c4KR-((c4KR > d4K)?d4K+1:0)];
			double o4OR = a4OR[c4OR-((c4OR > d4O)?d4O+1:0)];
			
			//-------- two
			
			a4IL[c4IL] = o4EL - (o4FL + o4GL + o4HL);
			a4JL[c4JL] = o4FL - (o4EL + o4GL + o4HL);
			a4KL[c4KL] = o4GL - (o4EL + o4FL + o4HL);
			a4LL[c4LL] = o4HL - (o4EL + o4FL + o4GL);
			
			c4IL++; if (c4IL < 0 || c4IL > d4I) c4IL = 0;
			c4JL++; if (c4JL < 0 || c4JL > d4J) c4JL = 0;
			c4KL++; if (c4KL < 0 || c4KL > d4K) c4KL = 0;
			c4LL++; if (c4LL < 0 || c4LL > d4L) c4LL = 0;
			
			double o4IL = a4IL[c4IL-((c4IL > d4I)?d4I+1:0)];
			double o4JL = a4JL[c4JL-((c4JL > d4J)?d4J+1:0)];
			double o4KL = a4KL[c4KL-((c4KL > d4K)?d4K+1:0)];
			double o4LL = a4LL[c4LL-((c4LL > d4L)?d4L+1:0)];
			
			a4BR[c4BR] = o4CR - (o4GR + o4KR + o4OR);
			a4FR[c4FR] = o4GR - (o4CR + o4KR + o4OR);
			a4JR[c4JR] = o4KR - (o4CR + o4GR + o4OR);
			a4NR[c4NR] = o4OR - (o4CR + o4GR + o4KR);
			
			c4BR++; if (c4BR < 0 || c4BR > d4B) c4BR = 0;
			c4FR++; if (c4FR < 0 || c4FR > d4F) c4FR = 0;
			c4JR++; if (c4JR < 0 || c4JR > d4J) c4JR = 0;
			c4NR++; if (c4NR < 0 || c4NR > d4N) c4NR = 0;
			
			double o4BR = a4BR[c4BR-((c4BR > d4B)?d4B+1:0)];
			double o4FR = a4FR[c4FR-((c4FR > d4F)?d4F+1:0)];
			double o4JR = a4JR[c4JR-((c4JR > d4J)?d4J+1:0)];
			double o4NR = a4NR[c4NR-((c4NR > d4N)?d4N+1:0)];
			
			//-------- three
			
			a4ML[c4ML] = o4IL - (o4JL + o4KL + o4LL);
			a4NL[c4NL] = o4JL - (o4IL + o4KL + o4LL);
			a4OL[c4OL] = o4KL - (o4IL + o4JL + o4LL);
			a4PL[c4PL] = o4LL - (o4IL + o4JL + o4KL);
			
			c4ML++; if (c4ML < 0 || c4ML > d4M) c4ML = 0;
			c4NL++; if (c4NL < 0 || c4NL > d4N) c4NL = 0;
			c4OL++; if (c4OL < 0 || c4OL > d4O) c4OL = 0;
			c4PL++; if (c4PL < 0 || c4PL > d4P) c4PL = 0;
			
			double o4ML = a4ML[c4ML-((c4ML > d4M)?d4M+1:0)];
			double o4NL = a4NL[c4NL-((c4NL > d4N)?d4N+1:0)];
			double o4OL = a4OL[c4OL-((c4OL > d4O)?d4O+1:0)];
			double o4PL = a4PL[c4PL-((c4PL > d4P)?d4P+1:0)];
			
			a4AR[c4AR] = o4BR - (o4FR + o4JR + o4NR);
			a4ER[c4ER] = o4FR - (o4BR + o4JR + o4NR);
			a4IR[c4IR] = o4JR - (o4BR + o4FR + o4NR);
			a4MR[c4MR] = o4NR - (o4BR + o4FR + o4JR);
			
			c4AR++; if (c4AR < 0 || c4AR > d4A) c4AR = 0;
			c4ER++; if (c4ER < 0 || c4ER > d4E) c4ER = 0;
			c4IR++; if (c4IR < 0 || c4IR > d4I) c4IR = 0;
			c4MR++; if (c4MR < 0 || c4MR > d4M) c4MR = 0;
			
			double o4AR = a4AR[c4AR-((c4AR > d4A)?d4A+1:0)];
			double o4ER = a4ER[c4ER-((c4ER > d4E)?d4E+1:0)];
			double o4IR = a4IR[c4IR-((c4IR > d4I)?d4I+1:0)];
			double o4MR = a4MR[c4MR-((c4MR > d4M)?d4M+1:0)];
			
			//-------- four
			
			f4AL = o4AR - (o4ER + o4IR + o4MR);				
			f4BL = o4ER - (o4AR + o4IR + o4MR);
			f4CL = o4IR - (o4AR + o4ER + o4MR);
			f4DL = o4MR - (o4AR + o4ER + o4IR);
			
			f4DR = o4ML - (o4NL + o4OL + o4PL);
			f4HR = o4NL - (o4ML + o4OL + o4PL);
			f4LR = o4OL - (o4ML + o4NL + o4PL);
			f4PR = o4PL - (o4ML + o4NL + o4OL);
			
			inputSampleL = (o4ML + o4NL + o4OL + o4PL)*0.125;
			inputSampleR = (o4AR + o4ER + o4IR + o4MR)*0.125;
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle]*bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		inputSampleL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*-0.25;
		inputSampleR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*-0.25;
		
		bezF[bez_cycle] += derezFreq;
		bezF[bez_SampL] += ((inputSampleL+bezF[bez_InL]) * derezFreq);
		bezF[bez_SampR] += ((inputSampleL+bezF[bez_InR]) * derezFreq);
		bezF[bez_InL] = inputSampleL; bezF[bez_InR] = inputSampleR;
		if (bezF[bez_cycle] > 1.0) { //hit the end point and we do a filter sample
			bezF[bez_cycle] = 0.0;
			bezF[bez_CL] = bezF[bez_BL];
			bezF[bez_BL] = bezF[bez_AL];
			bezF[bez_AL] = bezF[bez_SampL];
			bezF[bez_SampL] = 0.0;
			bezF[bez_CR] = bezF[bez_BR];
			bezF[bez_BR] = bezF[bez_AR];
			bezF[bez_AR] = bezF[bez_SampR];
			bezF[bez_SampR] = 0.0;
		}
		X = bezF[bez_cycle]*bezFreqTrim;
		double CBLfreq = (bezF[bez_CL]*(1.0-X))+(bezF[bez_BL]*X);
		double BALfreq = (bezF[bez_BL]*(1.0-X))+(bezF[bez_AL]*X);
		inputSampleL = (bezF[bez_BL]+(CBLfreq*(1.0-X))+(BALfreq*X))*0.25;
		double CBRfreq = (bezF[bez_CR]*(1.0-X))+(bezF[bez_BR]*X);
		double BARfreq = (bezF[bez_BR]*(1.0-X))+(bezF[bez_AR]*X);
		inputSampleR = (bezF[bez_BR]+(CBRfreq*(1.0-X))+(BARfreq*X))*0.25;
		//filtering the reverb separately, after making it
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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
