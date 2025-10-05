/* ========================================
 *  Dynamics2 - Dynamics2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Dynamics2_H
#include "Dynamics2.h"
#endif

void Dynamics2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double bezCThresh = pow(1.0-A, 6.0) * 8.0;
	double bezRez = pow(1.0-B, 8.0) / overallscale; 
	double sloRez = pow(1.0-C,12.0) / overallscale;
	sloRez = fmin(fmax(sloRez-(bezRez*0.5),0.00001),1.0);
	bezRez = fmin(fmax(bezRez,0.0001),1.0);
	double gate = pow(pow(D,4.0),sqrt(bezCThresh+1.0));
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (fmax(fabs(inputSampleL),fabs(inputSampleR)) > gate+(sloRez*bezGate)) bezGate = ((bezGate*overallscale*3.0)+3.0)*(0.25/overallscale);
		else bezGate = fmax(0.0, bezGate-(sloRez*sloRez));
		
		if (bezCThresh > 0.0) {
			inputSampleL *= ((bezCThresh*0.5)+1.0);
			inputSampleR *= ((bezCThresh*0.5)+1.0);
		}
		
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezCompF[bez_SampR] += (fabs(inputSampleR) * bezRez);
		bezMaxF = fmax(bezMaxF,fmax(fabs(inputSampleL),fabs(inputSampleR)));
		
		if (bezCompF[bez_cycle] > 1.0) {
			bezCompF[bez_cycle] -= 1.0;
			
			if (bezMaxF < gate) bezCompF[bez_SampL] = bezMaxF/gate; //note: SampL is a control voltage,
			if (bezCompF[bez_SampL]<gate) bezCompF[bez_SampL] = 0.0; //not a bipolar audio signal
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			
			if (bezMaxF < gate) bezCompF[bez_SampR] = bezMaxF/gate; //note: SampR is a control voltage,
			if (bezCompF[bez_SampR]<gate) bezCompF[bez_SampR] = 0.0; //not a bipolar audio signal
			bezCompF[bez_CR] = bezCompF[bez_BR];
			bezCompF[bez_BR] = bezCompF[bez_AR];
			bezCompF[bez_AR] = bezCompF[bez_SampR];
			bezCompF[bez_SampR] = 0.0;
			
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage
		bezCompS[bez_SampR] += (fabs(inputSampleR) * sloRez); //note: SampR is a control voltage
		if (bezCompS[bez_cycle] > 1.0) {
			bezCompS[bez_cycle] -= 1.0;
			
			if (bezCompS[bez_SampL]<gate) bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			
			if (bezCompS[bez_SampR]<gate) bezCompS[bez_SampR] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
		}
		double CBFL = (bezCompF[bez_CL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BL]*bezCompF[bez_cycle]);
		double BAFL = (bezCompF[bez_BL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AL]*bezCompF[bez_cycle]);
		double CBAFL = (bezCompF[bez_BL]+(CBFL*(1.0-bezCompF[bez_cycle]))+(BAFL*bezCompF[bez_cycle]))*0.5;
		double CBSL = (bezCompS[bez_CL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BL]*bezCompS[bez_cycle]);
		double BASL = (bezCompS[bez_BL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AL]*bezCompS[bez_cycle]);
		double CBASL = (bezCompS[bez_BL]+(CBSL*(1.0-bezCompS[bez_cycle]))+(BASL*bezCompS[bez_cycle]))*0.5;
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		
		double CBFR = (bezCompF[bez_CR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BR]*bezCompF[bez_cycle]);
		double BAFR = (bezCompF[bez_BR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AR]*bezCompF[bez_cycle]);
		double CBAFR = (bezCompF[bez_BR]+(CBFR*(1.0-bezCompF[bez_cycle]))+(BAFR*bezCompF[bez_cycle]))*0.5;
		double CBSR = (bezCompS[bez_CR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BR]*bezCompS[bez_cycle]);
		double BASR = (bezCompS[bez_BR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AR]*bezCompS[bez_cycle]);
		double CBASR = (bezCompS[bez_BR]+(CBSR*(1.0-bezCompS[bez_cycle]))+(BASR*bezCompS[bez_cycle]))*0.5;
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		
		if (bezGate < 1.0 && gate > 0.0) {inputSampleL *= bezGate; inputSampleR *= bezGate;}
		
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

void Dynamics2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double bezCThresh = pow(1.0-A, 6.0) * 8.0;
	double bezRez = pow(1.0-B, 8.0) / overallscale; 
	double sloRez = pow(1.0-C,12.0) / overallscale;
	sloRez = fmin(fmax(sloRez-(bezRez*0.5),0.00001),1.0);
	bezRez = fmin(fmax(bezRez,0.0001),1.0);
	double gate = pow(pow(D,4.0),sqrt(bezCThresh+1.0));
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (fmax(fabs(inputSampleL),fabs(inputSampleR)) > gate+(sloRez*bezGate)) bezGate = ((bezGate*overallscale*3.0)+3.0)*(0.25/overallscale);
		else bezGate = fmax(0.0, bezGate-(sloRez*sloRez));
		
		if (bezCThresh > 0.0) {
			inputSampleL *= ((bezCThresh*0.5)+1.0);
			inputSampleR *= ((bezCThresh*0.5)+1.0);
		}
		
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezCompF[bez_SampR] += (fabs(inputSampleR) * bezRez);
		bezMaxF = fmax(bezMaxF,fmax(fabs(inputSampleL),fabs(inputSampleR)));
		
		if (bezCompF[bez_cycle] > 1.0) {
			bezCompF[bez_cycle] -= 1.0;
			
			if (bezMaxF < gate) bezCompF[bez_SampL] = bezMaxF/gate; //note: SampL is a control voltage,
			if (bezCompF[bez_SampL]<gate) bezCompF[bez_SampL] = 0.0; //not a bipolar audio signal
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			
			if (bezMaxF < gate) bezCompF[bez_SampR] = bezMaxF/gate; //note: SampR is a control voltage,
			if (bezCompF[bez_SampR]<gate) bezCompF[bez_SampR] = 0.0; //not a bipolar audio signal
			bezCompF[bez_CR] = bezCompF[bez_BR];
			bezCompF[bez_BR] = bezCompF[bez_AR];
			bezCompF[bez_AR] = bezCompF[bez_SampR];
			bezCompF[bez_SampR] = 0.0;
			
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage
		bezCompS[bez_SampR] += (fabs(inputSampleR) * sloRez); //note: SampR is a control voltage
		if (bezCompS[bez_cycle] > 1.0) {
			bezCompS[bez_cycle] -= 1.0;
			
			if (bezCompS[bez_SampL]<gate) bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			
			if (bezCompS[bez_SampR]<gate) bezCompS[bez_SampR] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
		}
		double CBFL = (bezCompF[bez_CL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BL]*bezCompF[bez_cycle]);
		double BAFL = (bezCompF[bez_BL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AL]*bezCompF[bez_cycle]);
		double CBAFL = (bezCompF[bez_BL]+(CBFL*(1.0-bezCompF[bez_cycle]))+(BAFL*bezCompF[bez_cycle]))*0.5;
		double CBSL = (bezCompS[bez_CL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BL]*bezCompS[bez_cycle]);
		double BASL = (bezCompS[bez_BL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AL]*bezCompS[bez_cycle]);
		double CBASL = (bezCompS[bez_BL]+(CBSL*(1.0-bezCompS[bez_cycle]))+(BASL*bezCompS[bez_cycle]))*0.5;
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		
		double CBFR = (bezCompF[bez_CR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BR]*bezCompF[bez_cycle]);
		double BAFR = (bezCompF[bez_BR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AR]*bezCompF[bez_cycle]);
		double CBAFR = (bezCompF[bez_BR]+(CBFR*(1.0-bezCompF[bez_cycle]))+(BAFR*bezCompF[bez_cycle]))*0.5;
		double CBSR = (bezCompS[bez_CR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BR]*bezCompS[bez_cycle]);
		double BASR = (bezCompS[bez_BR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AR]*bezCompS[bez_cycle]);
		double CBASR = (bezCompS[bez_BR]+(CBSR*(1.0-bezCompS[bez_cycle]))+(BASR*bezCompS[bez_cycle]))*0.5;
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		
		if (bezGate < 1.0 && gate > 0.0) {inputSampleL *= bezGate; inputSampleR *= bezGate;}
		
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
