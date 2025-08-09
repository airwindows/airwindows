/* ========================================
 *  TakeCare - TakeCare.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TakeCare_H
#include "TakeCare.h"
#endif

void TakeCare::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	double vibSpeed = pow(A,5.0) * 0.1;
	double vibRandom = pow(B,3.0) * 0.99;
	double targetDepth = pow(C,2.0) * 0.5;
	double reg3n = D*0.0625;
	
	derezA = derezB; derezB = E*2.0;
	bool stepped = true; // Revised Bezier Undersampling
	if (derezB > 1.0) {  // has full rez at center, stepped
		stepped = false; // to left, continuous to right
		derezB = 1.0-(derezB-1.0);
	} //if it's set up like that it's the revised algorithm
	derezB = fmin(fmax(derezB/overallscale,0.0005),1.0);
	int bezFraction = (int)(1.0/derezB);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	if (stepped) { //this hard-locks derez to exact subdivisions of 1.0
		derezB = 1.0 / bezFraction;
		bezTrim = 1.0-(derezB*bezTrim);
	} else { //this makes it match the 1.0 case using stepped
		derezB /= (2.0/pow(overallscale,0.5-((overallscale-1.0)*0.0375)));
		bezTrim = 1.0-pow(derezB*0.5,1.0/(derezB*0.5));
	} //the revision more accurately connects the bezier curves
	
	int targetBuf = (pow(F,3.0)*32510.0)+256;
	outA = outB; outB = G;
	wetA = wetB; wetB = 1.0-pow(1.0-H,2.0);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double temp = (double)sampleFrames/inFramesToProcess;
		double derez = (derezA*temp)+(derezB*(1.0-temp));
		double out = (outA*temp)+(outB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			if (stepped) bez[bez_cycle] = 0.0;
			else bez[bez_cycle] -= 1.0;
			
			inputSampleL = (bez[bez_SampL]+bez[bez_AvgInSampL])*0.5;
			bez[bez_AvgInSampL] = bez[bez_SampL];
			inputSampleR = (bez[bez_SampR]+bez[bez_AvgInSampR])*0.5;
			bez[bez_AvgInSampR] = bez[bez_SampR];
			
			rotate += (oldfpd*vibSpeed);
			if (rotate > (M_PI*2.0)) {
				rotate -= (M_PI*2.0);
				oldfpd = (1.0-vibRandom) + ((fpdL*0.000000000618)*vibRandom);
			}
			vibDepth = (vibDepth*0.99)+(targetDepth*0.01);
			if (buf < targetBuf) buf++;
			if (buf > targetBuf) buf--;
			
			double mA = (sin(rotate)+1.0)*vibDepth*buf;
			double mB = (sin(rotate+(M_PI/9.0))+1.0)*vibDepth*buf;
			double mC = (sin(rotate+((M_PI/9.0)*2.0))+1.0)*vibDepth*buf;
			double mD = (sin(rotate+((M_PI/9.0)*3.0))+1.0)*vibDepth*buf;
			double mE = (sin(rotate+((M_PI/9.0)*4.0))+1.0)*vibDepth*buf;
			double mF = (sin(rotate+((M_PI/9.0)*5.0))+1.0)*vibDepth*buf;
			double mG = (sin(rotate+((M_PI/9.0)*6.0))+1.0)*vibDepth*buf;
			double mH = (sin(rotate+((M_PI/9.0)*7.0))+1.0)*vibDepth*buf;
			double mI = (sin(rotate+((M_PI/9.0)*8.0))+1.0)*vibDepth*buf;		
			
			inputSampleL = sin(fmin(fmax(inputSampleL*0.5,-M_PI_2),M_PI_2));
			inputSampleR = sin(fmin(fmax(inputSampleR*0.5,-M_PI_2),M_PI_2));
			
			a3AL[c3AL] = inputSampleL + fmin(fmax(f3AL*reg3n,-M_PI),M_PI);
			a3BL[c3BL] = inputSampleL + fmin(fmax(f3BL*reg3n,-M_PI),M_PI);
			a3CL[c3CL] = inputSampleL + fmin(fmax(f3CL*reg3n,-M_PI),M_PI);
			
			a3CR[c3CR] = inputSampleR + fmin(fmax(f3CR*reg3n,-M_PI),M_PI);
			a3FR[c3FR] = inputSampleR + fmin(fmax(f3FR*reg3n,-M_PI),M_PI);
			a3IR[c3IR] = inputSampleR + fmin(fmax(f3IR*reg3n,-M_PI),M_PI);
			
			c3AL++; if (c3AL < 0 || c3AL > buf) c3AL = 0;
			c3BL++; if (c3BL < 0 || c3BL > buf) c3BL = 0;
			c3CL++; if (c3CL < 0 || c3CL > buf) c3CL = 0;
			c3CR++; if (c3CR < 0 || c3CR > buf) c3CR = 0;
			c3FR++; if (c3FR < 0 || c3FR > buf) c3FR = 0;
			c3IR++; if (c3IR < 0 || c3IR > buf) c3IR = 0;
			
			double o3AL = a3AL[(int)(c3AL+mA-((c3AL+mA>buf)?buf+1:0))] *(1.0-(mA-floor(mA)));
			o3AL += (a3AL[(int)(c3AL+mA+1-((c3AL+mA+1>buf)?buf+1:0))] *((mA-floor(mA))));
			double o3BL = a3BL[(int)(c3BL+mB-((c3BL+mB>buf)?buf+1:0))] *(1.0-(mB-floor(mB)));
			o3BL += (a3BL[(int)(c3BL+mB+1-((c3BL+mB+1>buf)?buf+1:0))] *((mB-floor(mB))));
			double o3CL = a3CL[(int)(c3CL+mC-((c3CL+mC>buf)?buf+1:0))] *(1.0-(mC-floor(mC)));
			o3CL += (a3CL[(int)(c3CL+mC+1-((c3CL+mC+1>buf)?buf+1:0))] *((mC-floor(mC))));
			double o3CR = a3CR[(int)(c3CR+mC-((c3CR+mC>buf)?buf+1:0))] *(1.0-(mC-floor(mC)));
			o3CR += (a3CR[(int)(c3CR+mC+1-((c3CR+mC+1>buf)?buf+1:0))] *((mC-floor(mC))));
			double o3FR = a3FR[(int)(c3FR+mF-((c3FR+mF>buf)?buf+1:0))] *(1.0-(mF-floor(mF)));
			o3FR += (a3FR[(int)(c3FR+mF+1-((c3FR+mF+1>buf)?buf+1:0))] *((mF-floor(mF))));
			double o3IR = a3IR[(int)(c3IR+mI-((c3IR+mI>buf)?buf+1:0))] *(1.0-(mI-floor(mI)));
			o3IR += (a3IR[(int)(c3IR+mI+1-((c3IR+mI+1>buf)?buf+1:0))] *((mI-floor(mI))));
			
			a3DL[c3DL] = (((o3BL + o3CL) * -2.0) + o3AL);
			a3EL[c3EL] = (((o3AL + o3CL) * -2.0) + o3BL);
			a3FL[c3FL] = (((o3AL + o3BL) * -2.0) + o3CL);
			a3BR[c3BR] = (((o3FR + o3IR) * -2.0) + o3CR);
			a3ER[c3ER] = (((o3CR + o3IR) * -2.0) + o3FR);
			a3HR[c3HR] = (((o3CR + o3FR) * -2.0) + o3IR);
			
			c3DL++; if (c3DL < 0 || c3DL > buf) c3DL = 0;
			c3EL++; if (c3EL < 0 || c3EL > buf) c3EL = 0;
			c3FL++; if (c3FL < 0 || c3FL > buf) c3FL = 0;
			c3BR++; if (c3BR < 0 || c3BR > buf) c3BR = 0;
			c3ER++; if (c3ER < 0 || c3ER > buf) c3ER = 0;
			c3HR++; if (c3HR < 0 || c3HR > buf) c3HR = 0;
			
			double o3DL = a3DL[(int)(c3DL+mD-((c3DL+mD>buf)?buf+1:0))] *(1.0-(mD-floor(mD)));
			o3DL += (a3DL[(int)(c3DL+mD+1-((c3DL+mD+1>buf)?buf+1:0))] *((mD-floor(mD))));
			double o3EL = a3EL[(int)(c3EL+mE-((c3EL+mE>buf)?buf+1:0))] *(1.0-(mE-floor(mE)));
			o3EL += (a3EL[(int)(c3EL+mE+1-((c3EL+mE+1>buf)?buf+1:0))] *((mE-floor(mE))));
			double o3FL = a3FL[(int)(c3FL+mF-((c3FL+mF>buf)?buf+1:0))] *(1.0-(mF-floor(mF)));
			o3FL += (a3FL[(int)(c3FL+mF+1-((c3FL+mF+1>buf)?buf+1:0))] *((mF-floor(mF))));
			double o3BR = a3BR[(int)(c3BR+mB-((c3BR+mB>buf)?buf+1:0))] *(1.0-(mB-floor(mB)));
			o3BR += (a3BR[(int)(c3BR+mB+1-((c3BR+mB+1>buf)?buf+1:0))] *((mB-floor(mB))));
			double o3ER = a3ER[(int)(c3ER+mE-((c3ER+mE>buf)?buf+1:0))] *(1.0-(mE-floor(mE)));
			o3ER += (a3ER[(int)(c3ER+mE+1-((c3ER+mE+1>buf)?buf+1:0))] *((mE-floor(mE))));
			double o3HR = a3HR[(int)(c3HR+mH-((c3HR+mH>buf)?buf+1:0))] *(1.0-(mH-floor(mH)));
			o3HR += (a3HR[(int)(c3HR+mH+1-((c3HR+mH+1>buf)?buf+1:0))] *((mH-floor(mH))));
			
			a3GL[c3GL] = (((o3EL + o3FL) * -2.0) + o3DL);
			a3HL[c3HL] = (((o3DL + o3FL) * -2.0) + o3EL);
			a3IL[c3IL] = (((o3DL + o3EL) * -2.0) + o3FL);
			a3AR[c3AR] = (((o3ER + o3HR) * -2.0) + o3BR);
			a3DR[c3DR] = (((o3BR + o3HR) * -2.0) + o3ER);
			a3GR[c3GR] = (((o3BR + o3ER) * -2.0) + o3HR);
			
			
			c3GL++; if (c3GL < 0 || c3GL > buf) c3GL = 0;
			c3HL++; if (c3HL < 0 || c3HL > buf) c3HL = 0;
			c3IL++; if (c3IL < 0 || c3IL > buf) c3IL = 0;
			c3AR++; if (c3AR < 0 || c3AR > buf) c3AR = 0;
			c3DR++; if (c3DR < 0 || c3DR > buf) c3DR = 0;
			c3GR++; if (c3GR < 0 || c3GR > buf) c3GR = 0;
			
			double o3GL = a3GL[(int)(c3GL+mG-((c3GL+mG>buf)?buf+1:0))] *(1.0-(mG-floor(mG)));
			o3GL += (a3GL[(int)(c3GL+mG+1-((c3GL+mG+1>buf)?buf+1:0))] *((mG-floor(mG))));
			double o3HL = a3HL[(int)(c3HL+mH-((c3HL+mH>buf)?buf+1:0))] *(1.0-(mH-floor(mH)));
			o3HL += (a3HL[(int)(c3HL+mH+1-((c3HL+mH+1>buf)?buf+1:0))] *((mH-floor(mH))));
			double o3IL = a3IL[(int)(c3IL+mI-((c3IL+mI>buf)?buf+1:0))] *(1.0-(mI-floor(mI)));
			o3IL += (a3IL[(int)(c3IL+mI+1-((c3IL+mI+1>buf)?buf+1:0))] *((mI-floor(mI))));
			double o3AR = a3AR[(int)(c3AR+mA-((c3AR+mA>buf)?buf+1:0))] *(1.0-(mA-floor(mA)));
			o3AR += (a3AR[(int)(c3AR+mA+1-((c3AR+mA+1>buf)?buf+1:0))] *((mA-floor(mA))));
			double o3DR = a3DR[(int)(c3DR+mD-((c3DR+mD>buf)?buf+1:0))] *(1.0-(mD-floor(mD)));
			o3DR += (a3DR[(int)(c3DR+mD+1-((c3DR+mD+1>buf)?buf+1:0))] *((mD-floor(mD))));
			double o3GR = a3GR[(int)(c3GR+mG-((c3GR+mG>buf)?buf+1:0))] *(1.0-(mG-floor(mG)));
			o3GR += (a3GR[(int)(c3GR+mG+1-((c3GR+mG+1>buf)?buf+1:0))] *((mG-floor(mG))));
			
			f3AL = (((o3GR + o3HR) * -2.0) + o3IR);				
			f3BL = (((o3GR + o3HR) * -2.0) + o3IR);
			f3CL = (((o3GR + o3HR) * -2.0) + o3IR);
			
			f3CR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3FR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3IR = (((o3AL + o3DL) * -2.0) + o3GL);
			
			double inputSampleL = (o3GL + o3HL + o3IL)*0.03125;
			double inputSampleR = (o3AR + o3DR + o3GR)*0.03125;
			
			f3AL = (f3AL+f3AL+f3AL+fabs(avg3L))*0.25; avg3L = f3AL;
			f3CR = (f3CR+f3CR+f3CR+fabs(avg3R))*0.25; avg3R = f3CR;
			//manipulating deep reverb tail for realism	
			
			//begin ClipOnly stereo as a little, compressed chunk that can be dropped into undersampled code
			inputSampleL = fmin(fmax(inputSampleL,-4.0),4.0);
			if (wasPosClipL == true) { //current will be over
				if (inputSampleL<lastSampleL) lastSampleL=0.79+(inputSampleL*0.2);
				else lastSampleL = 0.2+(lastSampleL*0.79);
			} wasPosClipL = false;
			if (inputSampleL>0.99) {wasPosClipL=true;inputSampleL=0.79+(lastSampleL*0.2);}
			if (wasNegClipL == true) { //current will be -over
				if (inputSampleL > lastSampleL) lastSampleL=-0.79+(inputSampleL*0.2);
				else lastSampleL=-0.2+(lastSampleL*0.79);
			} wasNegClipL = false;
			if (inputSampleL<-0.99) {wasNegClipL=true;inputSampleL=-0.79+(lastSampleL*0.2);}
			lastSampleL = inputSampleL;
			inputSampleR = fmin(fmax(inputSampleR,-4.0),4.0);
			if (wasPosClipR == true) { //current will be over
				if (inputSampleR<lastSampleR) lastSampleR=0.79+(inputSampleR*0.2);
				else lastSampleR = 0.2+(lastSampleR*0.79);
			} wasPosClipR = false;
			if (inputSampleR>0.99) {wasPosClipR=true;inputSampleR=0.79+(lastSampleR*0.2);}
			if (wasNegClipR == true) { //current will be -over
				if (inputSampleR > lastSampleR) lastSampleR=-0.79+(inputSampleR*0.2);
				else lastSampleR=-0.2+(lastSampleR*0.79);
			} wasNegClipR = false;
			if (inputSampleR<-0.99) {wasNegClipR=true;inputSampleR=-0.79+(lastSampleR*0.2);}
			lastSampleR = inputSampleR;
			//end ClipOnly stereo as a little, compressed chunk that can be dropped into undersampled code							
			
			inputSampleL = asin(inputSampleL*0.7);
			inputSampleR = asin(inputSampleR*0.7);
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		
		double X = bez[bez_cycle] * bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*-0.25;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*-0.25;
		inputSampleL = CBAL+bez[bez_AvgOutSampL]; bez[bez_AvgOutSampL] = CBAL;
		inputSampleR = CBAR+bez[bez_AvgOutSampR]; bez[bez_AvgOutSampR] = CBAR;
		
		if (out != 1.0) {
			inputSampleL *= out;
			inputSampleR *= out;
		}
		
		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
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

void TakeCare::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	double vibSpeed = pow(A,5.0) * 0.1;
	double vibRandom = pow(B,3.0) * 0.99;
	double targetDepth = pow(C,2.0) * 0.5;
	double reg3n = D*0.0625;
	
	derezA = derezB; derezB = E*2.0;
	bool stepped = true; // Revised Bezier Undersampling
	if (derezB > 1.0) {  // has full rez at center, stepped
		stepped = false; // to left, continuous to right
		derezB = 1.0-(derezB-1.0);
	} //if it's set up like that it's the revised algorithm
	derezB = fmin(fmax(derezB/overallscale,0.0005),1.0);
	int bezFraction = (int)(1.0/derezB);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	if (stepped) { //this hard-locks derez to exact subdivisions of 1.0
		derezB = 1.0 / bezFraction;
		bezTrim = 1.0-(derezB*bezTrim);
	} else { //this makes it match the 1.0 case using stepped
		derezB /= (2.0/pow(overallscale,0.5-((overallscale-1.0)*0.0375)));
		bezTrim = 1.0-pow(derezB*0.5,1.0/(derezB*0.5));
	} //the revision more accurately connects the bezier curves
	
	int targetBuf = (pow(F,3.0)*32510.0)+256;
	outA = outB; outB = G;
	wetA = wetB; wetB = 1.0-pow(1.0-H,2.0);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double temp = (double)sampleFrames/inFramesToProcess;
		double derez = (derezA*temp)+(derezB*(1.0-temp));
		double out = (outA*temp)+(outB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			if (stepped) bez[bez_cycle] = 0.0;
			else bez[bez_cycle] -= 1.0;
			
			inputSampleL = (bez[bez_SampL]+bez[bez_AvgInSampL])*0.5;
			bez[bez_AvgInSampL] = bez[bez_SampL];
			inputSampleR = (bez[bez_SampR]+bez[bez_AvgInSampR])*0.5;
			bez[bez_AvgInSampR] = bez[bez_SampR];
			
			rotate += (oldfpd*vibSpeed);
			if (rotate > (M_PI*2.0)) {
				rotate -= (M_PI*2.0);
				oldfpd = (1.0-vibRandom) + ((fpdL*0.000000000618)*vibRandom);
			}
			vibDepth = (vibDepth*0.99)+(targetDepth*0.01);
			if (buf < targetBuf) buf++;
			if (buf > targetBuf) buf--;
			
			double mA = (sin(rotate)+1.0)*vibDepth*buf;
			double mB = (sin(rotate+(M_PI/9.0))+1.0)*vibDepth*buf;
			double mC = (sin(rotate+((M_PI/9.0)*2.0))+1.0)*vibDepth*buf;
			double mD = (sin(rotate+((M_PI/9.0)*3.0))+1.0)*vibDepth*buf;
			double mE = (sin(rotate+((M_PI/9.0)*4.0))+1.0)*vibDepth*buf;
			double mF = (sin(rotate+((M_PI/9.0)*5.0))+1.0)*vibDepth*buf;
			double mG = (sin(rotate+((M_PI/9.0)*6.0))+1.0)*vibDepth*buf;
			double mH = (sin(rotate+((M_PI/9.0)*7.0))+1.0)*vibDepth*buf;
			double mI = (sin(rotate+((M_PI/9.0)*8.0))+1.0)*vibDepth*buf;		
			
			inputSampleL = sin(fmin(fmax(inputSampleL*0.5,-M_PI_2),M_PI_2));
			inputSampleR = sin(fmin(fmax(inputSampleR*0.5,-M_PI_2),M_PI_2));
			
			a3AL[c3AL] = inputSampleL + fmin(fmax(f3AL*reg3n,-M_PI),M_PI);
			a3BL[c3BL] = inputSampleL + fmin(fmax(f3BL*reg3n,-M_PI),M_PI);
			a3CL[c3CL] = inputSampleL + fmin(fmax(f3CL*reg3n,-M_PI),M_PI);
			
			a3CR[c3CR] = inputSampleR + fmin(fmax(f3CR*reg3n,-M_PI),M_PI);
			a3FR[c3FR] = inputSampleR + fmin(fmax(f3FR*reg3n,-M_PI),M_PI);
			a3IR[c3IR] = inputSampleR + fmin(fmax(f3IR*reg3n,-M_PI),M_PI);
			
			c3AL++; if (c3AL < 0 || c3AL > buf) c3AL = 0;
			c3BL++; if (c3BL < 0 || c3BL > buf) c3BL = 0;
			c3CL++; if (c3CL < 0 || c3CL > buf) c3CL = 0;
			c3CR++; if (c3CR < 0 || c3CR > buf) c3CR = 0;
			c3FR++; if (c3FR < 0 || c3FR > buf) c3FR = 0;
			c3IR++; if (c3IR < 0 || c3IR > buf) c3IR = 0;
			
			double o3AL = a3AL[(int)(c3AL+mA-((c3AL+mA>buf)?buf+1:0))] *(1.0-(mA-floor(mA)));
			o3AL += (a3AL[(int)(c3AL+mA+1-((c3AL+mA+1>buf)?buf+1:0))] *((mA-floor(mA))));
			double o3BL = a3BL[(int)(c3BL+mB-((c3BL+mB>buf)?buf+1:0))] *(1.0-(mB-floor(mB)));
			o3BL += (a3BL[(int)(c3BL+mB+1-((c3BL+mB+1>buf)?buf+1:0))] *((mB-floor(mB))));
			double o3CL = a3CL[(int)(c3CL+mC-((c3CL+mC>buf)?buf+1:0))] *(1.0-(mC-floor(mC)));
			o3CL += (a3CL[(int)(c3CL+mC+1-((c3CL+mC+1>buf)?buf+1:0))] *((mC-floor(mC))));
			double o3CR = a3CR[(int)(c3CR+mC-((c3CR+mC>buf)?buf+1:0))] *(1.0-(mC-floor(mC)));
			o3CR += (a3CR[(int)(c3CR+mC+1-((c3CR+mC+1>buf)?buf+1:0))] *((mC-floor(mC))));
			double o3FR = a3FR[(int)(c3FR+mF-((c3FR+mF>buf)?buf+1:0))] *(1.0-(mF-floor(mF)));
			o3FR += (a3FR[(int)(c3FR+mF+1-((c3FR+mF+1>buf)?buf+1:0))] *((mF-floor(mF))));
			double o3IR = a3IR[(int)(c3IR+mI-((c3IR+mI>buf)?buf+1:0))] *(1.0-(mI-floor(mI)));
			o3IR += (a3IR[(int)(c3IR+mI+1-((c3IR+mI+1>buf)?buf+1:0))] *((mI-floor(mI))));
			
			a3DL[c3DL] = (((o3BL + o3CL) * -2.0) + o3AL);
			a3EL[c3EL] = (((o3AL + o3CL) * -2.0) + o3BL);
			a3FL[c3FL] = (((o3AL + o3BL) * -2.0) + o3CL);
			a3BR[c3BR] = (((o3FR + o3IR) * -2.0) + o3CR);
			a3ER[c3ER] = (((o3CR + o3IR) * -2.0) + o3FR);
			a3HR[c3HR] = (((o3CR + o3FR) * -2.0) + o3IR);
			
			c3DL++; if (c3DL < 0 || c3DL > buf) c3DL = 0;
			c3EL++; if (c3EL < 0 || c3EL > buf) c3EL = 0;
			c3FL++; if (c3FL < 0 || c3FL > buf) c3FL = 0;
			c3BR++; if (c3BR < 0 || c3BR > buf) c3BR = 0;
			c3ER++; if (c3ER < 0 || c3ER > buf) c3ER = 0;
			c3HR++; if (c3HR < 0 || c3HR > buf) c3HR = 0;
			
			double o3DL = a3DL[(int)(c3DL+mD-((c3DL+mD>buf)?buf+1:0))] *(1.0-(mD-floor(mD)));
			o3DL += (a3DL[(int)(c3DL+mD+1-((c3DL+mD+1>buf)?buf+1:0))] *((mD-floor(mD))));
			double o3EL = a3EL[(int)(c3EL+mE-((c3EL+mE>buf)?buf+1:0))] *(1.0-(mE-floor(mE)));
			o3EL += (a3EL[(int)(c3EL+mE+1-((c3EL+mE+1>buf)?buf+1:0))] *((mE-floor(mE))));
			double o3FL = a3FL[(int)(c3FL+mF-((c3FL+mF>buf)?buf+1:0))] *(1.0-(mF-floor(mF)));
			o3FL += (a3FL[(int)(c3FL+mF+1-((c3FL+mF+1>buf)?buf+1:0))] *((mF-floor(mF))));
			double o3BR = a3BR[(int)(c3BR+mB-((c3BR+mB>buf)?buf+1:0))] *(1.0-(mB-floor(mB)));
			o3BR += (a3BR[(int)(c3BR+mB+1-((c3BR+mB+1>buf)?buf+1:0))] *((mB-floor(mB))));
			double o3ER = a3ER[(int)(c3ER+mE-((c3ER+mE>buf)?buf+1:0))] *(1.0-(mE-floor(mE)));
			o3ER += (a3ER[(int)(c3ER+mE+1-((c3ER+mE+1>buf)?buf+1:0))] *((mE-floor(mE))));
			double o3HR = a3HR[(int)(c3HR+mH-((c3HR+mH>buf)?buf+1:0))] *(1.0-(mH-floor(mH)));
			o3HR += (a3HR[(int)(c3HR+mH+1-((c3HR+mH+1>buf)?buf+1:0))] *((mH-floor(mH))));
			
			a3GL[c3GL] = (((o3EL + o3FL) * -2.0) + o3DL);
			a3HL[c3HL] = (((o3DL + o3FL) * -2.0) + o3EL);
			a3IL[c3IL] = (((o3DL + o3EL) * -2.0) + o3FL);
			a3AR[c3AR] = (((o3ER + o3HR) * -2.0) + o3BR);
			a3DR[c3DR] = (((o3BR + o3HR) * -2.0) + o3ER);
			a3GR[c3GR] = (((o3BR + o3ER) * -2.0) + o3HR);
			
			
			c3GL++; if (c3GL < 0 || c3GL > buf) c3GL = 0;
			c3HL++; if (c3HL < 0 || c3HL > buf) c3HL = 0;
			c3IL++; if (c3IL < 0 || c3IL > buf) c3IL = 0;
			c3AR++; if (c3AR < 0 || c3AR > buf) c3AR = 0;
			c3DR++; if (c3DR < 0 || c3DR > buf) c3DR = 0;
			c3GR++; if (c3GR < 0 || c3GR > buf) c3GR = 0;
			
			double o3GL = a3GL[(int)(c3GL+mG-((c3GL+mG>buf)?buf+1:0))] *(1.0-(mG-floor(mG)));
			o3GL += (a3GL[(int)(c3GL+mG+1-((c3GL+mG+1>buf)?buf+1:0))] *((mG-floor(mG))));
			double o3HL = a3HL[(int)(c3HL+mH-((c3HL+mH>buf)?buf+1:0))] *(1.0-(mH-floor(mH)));
			o3HL += (a3HL[(int)(c3HL+mH+1-((c3HL+mH+1>buf)?buf+1:0))] *((mH-floor(mH))));
			double o3IL = a3IL[(int)(c3IL+mI-((c3IL+mI>buf)?buf+1:0))] *(1.0-(mI-floor(mI)));
			o3IL += (a3IL[(int)(c3IL+mI+1-((c3IL+mI+1>buf)?buf+1:0))] *((mI-floor(mI))));
			double o3AR = a3AR[(int)(c3AR+mA-((c3AR+mA>buf)?buf+1:0))] *(1.0-(mA-floor(mA)));
			o3AR += (a3AR[(int)(c3AR+mA+1-((c3AR+mA+1>buf)?buf+1:0))] *((mA-floor(mA))));
			double o3DR = a3DR[(int)(c3DR+mD-((c3DR+mD>buf)?buf+1:0))] *(1.0-(mD-floor(mD)));
			o3DR += (a3DR[(int)(c3DR+mD+1-((c3DR+mD+1>buf)?buf+1:0))] *((mD-floor(mD))));
			double o3GR = a3GR[(int)(c3GR+mG-((c3GR+mG>buf)?buf+1:0))] *(1.0-(mG-floor(mG)));
			o3GR += (a3GR[(int)(c3GR+mG+1-((c3GR+mG+1>buf)?buf+1:0))] *((mG-floor(mG))));
			
			f3AL = (((o3GR + o3HR) * -2.0) + o3IR);				
			f3BL = (((o3GR + o3HR) * -2.0) + o3IR);
			f3CL = (((o3GR + o3HR) * -2.0) + o3IR);
			
			f3CR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3FR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3IR = (((o3AL + o3DL) * -2.0) + o3GL);
			
			double inputSampleL = (o3GL + o3HL + o3IL)*0.03125;
			double inputSampleR = (o3AR + o3DR + o3GR)*0.03125;
			
			f3AL = (f3AL+f3AL+f3AL+fabs(avg3L))*0.25; avg3L = f3AL;
			f3CR = (f3CR+f3CR+f3CR+fabs(avg3R))*0.25; avg3R = f3CR;
			//manipulating deep reverb tail for realism	
			
			//begin ClipOnly stereo as a little, compressed chunk that can be dropped into undersampled code
			inputSampleL = fmin(fmax(inputSampleL,-4.0),4.0);
			if (wasPosClipL == true) { //current will be over
				if (inputSampleL<lastSampleL) lastSampleL=0.79+(inputSampleL*0.2);
				else lastSampleL = 0.2+(lastSampleL*0.79);
			} wasPosClipL = false;
			if (inputSampleL>0.99) {wasPosClipL=true;inputSampleL=0.79+(lastSampleL*0.2);}
			if (wasNegClipL == true) { //current will be -over
				if (inputSampleL > lastSampleL) lastSampleL=-0.79+(inputSampleL*0.2);
				else lastSampleL=-0.2+(lastSampleL*0.79);
			} wasNegClipL = false;
			if (inputSampleL<-0.99) {wasNegClipL=true;inputSampleL=-0.79+(lastSampleL*0.2);}
			lastSampleL = inputSampleL;
			inputSampleR = fmin(fmax(inputSampleR,-4.0),4.0);
			if (wasPosClipR == true) { //current will be over
				if (inputSampleR<lastSampleR) lastSampleR=0.79+(inputSampleR*0.2);
				else lastSampleR = 0.2+(lastSampleR*0.79);
			} wasPosClipR = false;
			if (inputSampleR>0.99) {wasPosClipR=true;inputSampleR=0.79+(lastSampleR*0.2);}
			if (wasNegClipR == true) { //current will be -over
				if (inputSampleR > lastSampleR) lastSampleR=-0.79+(inputSampleR*0.2);
				else lastSampleR=-0.2+(lastSampleR*0.79);
			} wasNegClipR = false;
			if (inputSampleR<-0.99) {wasNegClipR=true;inputSampleR=-0.79+(lastSampleR*0.2);}
			lastSampleR = inputSampleR;
			//end ClipOnly stereo as a little, compressed chunk that can be dropped into undersampled code							
			
			inputSampleL = asin(inputSampleL*0.7);
			inputSampleR = asin(inputSampleR*0.7);
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		
		double X = bez[bez_cycle] * bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*-0.25;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*-0.25;
		inputSampleL = CBAL+bez[bez_AvgOutSampL]; bez[bez_AvgOutSampL] = CBAL;
		inputSampleR = CBAR+bez[bez_AvgOutSampR]; bez[bez_AvgOutSampR] = CBAR;
		
		if (out != 1.0) {
			inputSampleL *= out;
			inputSampleR *= out;
		}
		
		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
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
