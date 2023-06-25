/* ========================================
 *  Pear - Pear.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Pear_H
#include "Pear.h"
#endif

void Pear::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 3) cycleEnd = 3;
	cycleEnd--; //this is going to be 0 for 44.1 or 48k, 1 for 88.2 or 96k, 2 for 176 or 192k.
	//DIFFERENT! Offsetting the bt shift
	
	int bitshiftFreq = (10-(A*10.0))+cycleEnd;
	double freq = 1.0;
	switch (bitshiftFreq)
	{
		case 16: freq = 0.0000152587890625; break;
		case 15: freq = 0.000030517578125; break;
		case 14: freq = 0.00006103515625; break;
		case 13: freq = 0.0001220703125; break;
		case 12: freq = 0.000244140625; break;
		case 11: freq = 0.00048828125; break;
		case 10: freq = 0.0009765625; break;
		case 9: freq = 0.001953125; break;
		case 8: freq = 0.00390625; break;
		case 7: freq = 0.0078125; break;
		case 6: freq = 0.015625; break;
		case 5: freq = 0.03125; break;
		case 4: freq = 0.0625; break;
		case 3: freq = 0.125; break;
		case 2: freq = 0.25; break;
		case 1: freq = 0.5; break;
		case 0: freq = 1.0; break;
	}
	double maxPoles = B*pear_total;
	double wet = (C*2.0)-1.0; //inv-dry-wet for highpass
	double dry = 2.0-(C*2.0);
	if (dry > 1.0) dry = 1.0; //full dry for use with inv, to 0.0 at full wet

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//Pear is a variation on a Holt filter, made to act like my biquad filters:
		//its variables (L and R prevSample and prevSlew) contained in arrays for easier handling.
		//On top of that, the array is used to facilitate use of the filter in a stack which will
		//sharpen its very weak frequency response performance (3dB/oct per pole) and allow it to run
		//in a loop. Many earlier Airwindows filters did this 'unrolled', often with variables given
		//lettered suffixes like iirA through iirZ. The use of an array will make this a lot tidier.
		
		for (int x = 0; x < maxPoles; x += 4) {
			double slew = ((inputSampleL - pear[x]) + pear[x+1])*freq*0.5;
			pear[x] = inputSampleL = (freq * inputSampleL) + ((1.0-freq) * (pear[x] + pear[x+1]));
			pear[x+1] = slew;
			slew = ((inputSampleR - pear[x+2]) + pear[x+3])*freq*0.5;
			pear[x+2] = inputSampleR = (freq * inputSampleR) + ((1.0-freq) * (pear[x+2] + pear[x+3]));
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

void Pear::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 3) cycleEnd = 3;
	cycleEnd--; //this is going to be 0 for 44.1 or 48k, 1 for 88.2 or 96k, 2 for 176 or 192k.
	//DIFFERENT! Offsetting the bt shift
	
	int bitshiftFreq = (10-(A*10.0))+cycleEnd;
	double freq = 1.0;
	switch (bitshiftFreq)
	{
		case 16: freq = 0.0000152587890625; break;
		case 15: freq = 0.000030517578125; break;
		case 14: freq = 0.00006103515625; break;
		case 13: freq = 0.0001220703125; break;
		case 12: freq = 0.000244140625; break;
		case 11: freq = 0.00048828125; break;
		case 10: freq = 0.0009765625; break;
		case 9: freq = 0.001953125; break;
		case 8: freq = 0.00390625; break;
		case 7: freq = 0.0078125; break;
		case 6: freq = 0.015625; break;
		case 5: freq = 0.03125; break;
		case 4: freq = 0.0625; break;
		case 3: freq = 0.125; break;
		case 2: freq = 0.25; break;
		case 1: freq = 0.5; break;
		case 0: freq = 1.0; break;
	}
	double maxPoles = B*pear_total;
	double wet = (C*2.0)-1.0; //inv-dry-wet for highpass
	double dry = 2.0-(C*2.0);
	if (dry > 1.0) dry = 1.0; //full dry for use with inv, to 0.0 at full wet
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//Pear is a variation on a Holt filter, made to act like my biquad filters:
		//its variables (L and R prevSample and prevSlew) contained in arrays for easier handling.
		//On top of that, the array is used to facilitate use of the filter in a stack which will
		//sharpen its very weak frequency response performance (3dB/oct per pole) and allow it to run
		//in a loop. Many earlier Airwindows filters did this 'unrolled', often with variables given
		//lettered suffixes like iirA through iirZ. The use of an array will make this a lot tidier.
		
		for (int x = 0; x < maxPoles; x += 4) {
			double slew = ((inputSampleL - pear[x]) + pear[x+1])*freq*0.5;
			pear[x] = inputSampleL = (freq * inputSampleL) + ((1.0-freq) * (pear[x] + pear[x+1]));
			pear[x+1] = slew;
			slew = ((inputSampleR - pear[x+2]) + pear[x+3])*freq*0.5;
			pear[x+2] = inputSampleR = (freq * inputSampleR) + ((1.0-freq) * (pear[x+2] + pear[x+3]));
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
