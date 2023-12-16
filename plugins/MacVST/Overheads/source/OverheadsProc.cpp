/* ========================================
 *  Overheads - Overheads.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Overheads_H
#include "Overheads.h"
#endif

void Overheads::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double ovhTrim = pow(A,3);
	ovhTrim += 1.0; ovhTrim *= ovhTrim;
	int offset = (pow(B,7) * 16.0 * overallscale)+1;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Overheads compressor
		inputSampleL *= ovhTrim; inputSampleR *= ovhTrim;
		ovhCount--; if (ovhCount < 0 || ovhCount > 128) ovhCount = 128; ovhL[ovhCount] = inputSampleL; ovhR[ovhCount] = inputSampleR;
		double ovhClamp = sin(fabs(inputSampleL - ovhL[(ovhCount+offset)-(((ovhCount+offset) > 128)?129:0)])*(ovhTrim-1.0)*64.0);
		ovhGain *= (1.0 - ovhClamp); ovhGain += ((1.0-ovhClamp) * ovhClamp);
		ovhClamp = sin(fabs(inputSampleR - ovhR[(ovhCount+offset)-(((ovhCount+offset) > 128)?129:0)])*(ovhTrim-1.0)*64.0);
		ovhGain *= (1.0 - ovhClamp); ovhGain += ((1.0-ovhClamp) * ovhClamp);
		if (ovhGain > 1.0) ovhGain = 1.0; if (ovhGain < 0.0) ovhGain = 0.0;
		inputSampleL *= ovhGain; inputSampleR *= ovhGain;
		//end Overheads compressor
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		
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

void Overheads::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double ovhTrim = pow(A,3);
	ovhTrim += 1.0; ovhTrim *= ovhTrim;
	int offset = (pow(B,7) * 16.0 * overallscale)+1;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Overheads compressor
		inputSampleL *= ovhTrim; inputSampleR *= ovhTrim;
		ovhCount--; if (ovhCount < 0 || ovhCount > 128) ovhCount = 128; ovhL[ovhCount] = inputSampleL; ovhR[ovhCount] = inputSampleR;
		double ovhClamp = sin(fabs(inputSampleL - ovhL[(ovhCount+offset)-(((ovhCount+offset) > 128)?129:0)])*(ovhTrim-1.0)*64.0);
		ovhGain *= (1.0 - ovhClamp); ovhGain += ((1.0-ovhClamp) * ovhClamp);
		ovhClamp = sin(fabs(inputSampleR - ovhR[(ovhCount+offset)-(((ovhCount+offset) > 128)?129:0)])*(ovhTrim-1.0)*64.0);
		ovhGain *= (1.0 - ovhClamp); ovhGain += ((1.0-ovhClamp) * ovhClamp);
		if (ovhGain > 1.0) ovhGain = 1.0; if (ovhGain < 0.0) ovhGain = 0.0;
		inputSampleL *= ovhGain; inputSampleR *= ovhGain;
		//end Overheads compressor
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
				
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
