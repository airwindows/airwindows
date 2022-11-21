/* ========================================
 *  Pockey - Pockey.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Pockey_H
#include "Pockey.h"
#endif

void Pockey::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double freqMin = 0.08 / overallscale;
	freqA = freqB; freqB = (pow(1.0-A,3)*(0.618033988749894848204586-freqMin))+freqMin;
	//freq is always engaged at least a little
	rezA = rezB; rezB = pow(B*0.618033988749894848204586,3)+0.000244140625;
	//rez is always at least 12 bit truncation
	double wet = C;
	
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
		double rez = (rezA*temp)+(rezB*(1.0-temp));
		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleL > 0) inputSampleL = log(1.0+(255*fabs(inputSampleL)))/log(255);
		if (inputSampleL < 0) inputSampleL = -log(1.0+(255*fabs(inputSampleL)))/log(255);

		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		if (inputSampleR > 0) inputSampleR = log(1.0+(255*fabs(inputSampleR)))/log(255);
		if (inputSampleR < 0) inputSampleR = -log(1.0+(255*fabs(inputSampleR)))/log(255);
		//end uLaw encode		
		
		double offset = inputSampleL;
		if (inputSampleL > 0)
		{
			while (offset > 0) {offset -= rez;}
			inputSampleL -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (inputSampleL < 0)
		{
			while (offset < 0) {offset += rez;}
			inputSampleL -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		inputSampleL *= (1.0 - rez);
		
		offset = inputSampleR;
		if (inputSampleR > 0)
		{
			while (offset > 0) {offset -= rez;}
			inputSampleR -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (inputSampleR < 0)
		{
			while (offset < 0) {offset += rez;}
			inputSampleR -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		inputSampleR *= (1.0 - rez);
		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleL > 0) inputSampleL = (pow(256,fabs(inputSampleL))-1.0) / 255;
		if (inputSampleL < 0) inputSampleL = -(pow(256,fabs(inputSampleL))-1.0) / 255;

		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		if (inputSampleR > 0) inputSampleR = (pow(256,fabs(inputSampleR))-1.0) / 255;
		if (inputSampleR < 0) inputSampleR = -(pow(256,fabs(inputSampleR))-1.0) / 255;
		//end uLaw decode
		
		positionL += freq;
		double outputSample = heldSampleL;
		if (positionL > 1.0)
		{
			positionL -= 1.0;
			heldSampleL = (lastSampleL * positionL) + (inputSampleL * (1-positionL));
			outputSample = (heldSampleL * (1-positionL)) + (outputSample * positionL);
		}
		inputSampleL = outputSample;
		double slew = fabs(inputSampleL - lastSoftenL)*freq;
		if (slew > 0.5) slew = 0.5;
		inputSampleL = (inputSampleL * slew) + (lastSoftenL * (1.0-slew));
		//conditional average: only if we actually have brightness		
		lastSampleL = drySampleL;
		lastSoftenL = outputSample;
		
		
		positionR += freq;
		outputSample = heldSampleR;
		if (positionR > 1.0)
		{
			positionR -= 1.0;
			heldSampleR = (lastSampleR * positionR) + (inputSampleR * (1-positionR));
			outputSample = (heldSampleR * (1-positionR)) + (outputSample * positionR);
		}
		inputSampleR = outputSample;
		slew = fabs(inputSampleR - lastSoftenR)*freq;
		if (slew > 0.5) slew = 0.5;
		inputSampleR = (inputSampleR * slew) + (lastSoftenR * (1.0-slew));
		//conditional average: only if we actually have brightness
		lastSampleR = drySampleR;
		lastSoftenR = outputSample;
		//end Frequency Derez
		
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

void Pockey::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double freqMin = 0.08 / overallscale;
	freqA = freqB; freqB = (pow(1.0-A,3)*(0.618033988749894848204586-freqMin))+freqMin;
	//freq is always engaged at least a little
	rezA = rezB; rezB = pow(B*0.618033988749894848204586,3)+0.000244140625;
	//rez is always at least 12 bit truncation
	double wet = C;
	
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
		double rez = (rezA*temp)+(rezB*(1.0-temp));
		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleL > 0) inputSampleL = log(1.0+(255*fabs(inputSampleL)))/log(255);
		if (inputSampleL < 0) inputSampleL = -log(1.0+(255*fabs(inputSampleL)))/log(255);
		
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		if (inputSampleR > 0) inputSampleR = log(1.0+(255*fabs(inputSampleR)))/log(255);
		if (inputSampleR < 0) inputSampleR = -log(1.0+(255*fabs(inputSampleR)))/log(255);
		//end uLaw encode		
		
		double offset = inputSampleL;
		if (inputSampleL > 0)
		{
			while (offset > 0) {offset -= rez;}
			inputSampleL -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (inputSampleL < 0)
		{
			while (offset < 0) {offset += rez;}
			inputSampleL -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		inputSampleL *= (1.0 - rez);
		
		offset = inputSampleR;
		if (inputSampleR > 0)
		{
			while (offset > 0) {offset -= rez;}
			inputSampleR -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (inputSampleR < 0)
		{
			while (offset < 0) {offset += rez;}
			inputSampleR -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		inputSampleR *= (1.0 - rez);
		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleL > 0) inputSampleL = (pow(256,fabs(inputSampleL))-1.0) / 255;
		if (inputSampleL < 0) inputSampleL = -(pow(256,fabs(inputSampleL))-1.0) / 255;
		
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		if (inputSampleR > 0) inputSampleR = (pow(256,fabs(inputSampleR))-1.0) / 255;
		if (inputSampleR < 0) inputSampleR = -(pow(256,fabs(inputSampleR))-1.0) / 255;
		//end uLaw decode
		
		positionL += freq;
		double outputSample = heldSampleL;
		if (positionL > 1.0)
		{
			positionL -= 1.0;
			heldSampleL = (lastSampleL * positionL) + (inputSampleL * (1-positionL));
			outputSample = (heldSampleL * (1-positionL)) + (outputSample * positionL);
		}
		inputSampleL = outputSample;
		double slew = fabs(inputSampleL - lastSoftenL)*freq;
		if (slew > 0.5) slew = 0.5;
		inputSampleL = (inputSampleL * slew) + (lastSoftenL * (1.0-slew));
		//conditional average: only if we actually have brightness		
		lastSampleL = drySampleL;
		lastSoftenL = outputSample;
		
		
		positionR += freq;
		outputSample = heldSampleR;
		if (positionR > 1.0)
		{
			positionR -= 1.0;
			heldSampleR = (lastSampleR * positionR) + (inputSampleR * (1-positionR));
			outputSample = (heldSampleR * (1-positionR)) + (outputSample * positionR);
		}
		inputSampleR = outputSample;
		slew = fabs(inputSampleR - lastSoftenR)*freq;
		if (slew > 0.5) slew = 0.5;
		inputSampleR = (inputSampleR * slew) + (lastSoftenR * (1.0-slew));
		//conditional average: only if we actually have brightness
		lastSampleR = drySampleR;
		lastSoftenR = outputSample;
		//end Frequency Derez
		
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
