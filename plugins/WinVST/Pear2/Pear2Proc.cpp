/* ========================================
 *  Pear2 - Pear2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Pear2_H
#include "Pear2.h"
#endif

void Pear2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	freqA = freqB;
	freqB = pow(A,2);
	if (freqB < 0.0001) freqB = 0.0001;
	nonLinA = nonLinB;
	nonLinB = pow(B,2);
	double maxPoles = pow(C,3)*pear_total;
	wetA = wetB;
	wetB = D; //inv-dry-wet for highpass
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double freq = (freqA*temp)+(freqB*(1.0-temp));
		double nonLin = (nonLinA*temp)+(nonLinB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		double dry = 2.0-(wet*2.0);
		if (dry > 1.0) dry = 1.0; //full dry for use with inv, to 0.0 at full wet
		wet = (wet*2.0)-1.0; //inv-dry-wet for highpass
		
		for (int x = 0; x < maxPoles; x += 4) {
			double di = fabs(freq*(1.0+(inputSampleL*nonLin))); if (di > 1.0) di = 1.0;
			double slew = ((inputSampleL - pear[x]) + pear[x+1])*di*0.5;
			pear[x] = inputSampleL = (di * inputSampleL) + ((1.0-di) * (pear[x] + pear[x+1]));
			pear[x+1] = slew;
			di = fabs(freq*(1.0+(inputSampleR*nonLin))); if (di > 1.0) di = 1.0;
			slew = ((inputSampleR - pear[x+2]) + pear[x+3])*di*0.5;
			pear[x+2] = inputSampleR = (di * inputSampleR) + ((1.0-di) * (pear[x+2] + pear[x+3]));
			pear[x+3] = slew;
		}
		
		inputSampleL *= wet;
		inputSampleR *= wet;
		drySampleL *= dry;
		drySampleR *= dry;
		inputSampleL += drySampleL;
		inputSampleR += drySampleR;
		
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

void Pear2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	freqA = freqB;
	freqB = pow(A,2);
	if (freqB < 0.0001) freqB = 0.0001;
	nonLinA = nonLinB;
	nonLinB = pow(B,2);
	double maxPoles = pow(C,3)*pear_total;
	wetA = wetB;
	wetB = D; //inv-dry-wet for highpass
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double freq = (freqA*temp)+(freqB*(1.0-temp));
		double nonLin = (nonLinA*temp)+(nonLinB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		double dry = 2.0-(wet*2.0);
		if (dry > 1.0) dry = 1.0; //full dry for use with inv, to 0.0 at full wet
		wet = (wet*2.0)-1.0; //inv-dry-wet for highpass
		
		for (int x = 0; x < maxPoles; x += 4) {
			double di = fabs(freq*(1.0+(inputSampleL*nonLin))); if (di > 1.0) di = 1.0;
			double slew = ((inputSampleL - pear[x]) + pear[x+1])*di*0.5;
			pear[x] = inputSampleL = (di * inputSampleL) + ((1.0-di) * (pear[x] + pear[x+1]));
			pear[x+1] = slew;
			di = fabs(freq*(1.0+(inputSampleR*nonLin))); if (di > 1.0) di = 1.0;
			slew = ((inputSampleR - pear[x+2]) + pear[x+3])*di*0.5;
			pear[x+2] = inputSampleR = (di * inputSampleR) + ((1.0-di) * (pear[x+2] + pear[x+3]));
			pear[x+3] = slew;
		}
		
		inputSampleL *= wet;
		inputSampleR *= wet;
		drySampleL *= dry;
		drySampleR *= dry;
		inputSampleL += drySampleL;
		inputSampleR += drySampleR;
		
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
