/* ========================================
 *  DeRez2 - DeRez2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeRez2_H
#include "DeRez2.h"
#endif

void DeRez2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double targetA = pow(A,3)+0.0005;
	if (targetA > 1.0) targetA = 1.0;
	double soften = (1.0 + targetA)/2;
	double targetB = pow(1.0-B,3) / 3;
	double hard = C;
	double wet = D;	

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	targetA /= overallscale;	
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		
		incrementA = ((incrementA*999.0)+targetA)/1000.0;
		incrementB = ((incrementB*999.0)+targetB)/1000.0;
		//incrementA is the frequency derez
		//incrementB is the bit depth derez
		position += incrementA;
		
		double outputSampleL = heldSampleL;
		double outputSampleR = heldSampleR;
		if (position > 1.0)
		{
			position -= 1.0;
			heldSampleL = (lastSampleL * position) + (inputSampleL * (1.0-position));
			outputSampleL = (outputSampleL * (1.0-soften)) + (heldSampleL * soften);
			//softens the edge of the derez
			heldSampleR = (lastSampleR * position) + (inputSampleR * (1.0-position));
			outputSampleR = (outputSampleR * (1.0-soften)) + (heldSampleR * soften);
			//softens the edge of the derez
		}
		inputSampleL = outputSampleL;
		inputSampleR = outputSampleR;
		
		double tempL = inputSampleL;
		double tempR = inputSampleR;
		
		if (inputSampleL != lastOutputSampleL) {
			tempL = inputSampleL;
			inputSampleL = (inputSampleL * hard) + (lastDrySampleL * (1.0-hard));
			//transitions get an intermediate dry sample
			lastOutputSampleL = tempL; //only one intermediate sample
		} else {
			lastOutputSampleL = inputSampleL;
		}

		if (inputSampleR != lastOutputSampleR) {
			tempR = inputSampleR;
			inputSampleR = (inputSampleR * hard) + (lastDrySampleR * (1.0-hard));
			//transitions get an intermediate dry sample
			lastOutputSampleR = tempR; //only one intermediate sample
		} else {
			lastOutputSampleR = inputSampleR;
		}
		
		lastDrySampleL = drySampleL;
		lastDrySampleR = drySampleR;
		//freq section of soft/hard interpolates dry samples
		
		tempL = inputSampleL;
		tempR = inputSampleR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		if (inputSampleL > 0) inputSampleL = log(1.0+(255*fabs(inputSampleL))) / log(256);
		if (inputSampleL < 0) inputSampleL = -log(1.0+(255*fabs(inputSampleL))) / log(256);
		
		if (inputSampleR > 0) inputSampleR = log(1.0+(255*fabs(inputSampleR))) / log(256);
		if (inputSampleR < 0) inputSampleR = -log(1.0+(255*fabs(inputSampleR))) / log(256);
		
		inputSampleL = (tempL * hard) + (inputSampleL * (1.0-hard));
		inputSampleR = (tempR * hard) + (inputSampleR * (1.0-hard)); //uLaw encode as part of soft/hard
		
		tempL = inputSampleL;
		tempR = inputSampleR;
		
		if (incrementB > 0.0005)
		{
			if (inputSampleL > 0)
			{
				tempL = inputSampleL;
				while (tempL > 0) {tempL -= incrementB;}
				inputSampleL -= tempL;
				//it's below 0 so subtracting adds the remainder
			}
			if (inputSampleR > 0)
			{
				tempR = inputSampleR;
				while (tempR > 0) {tempR -= incrementB;}
				inputSampleR -= tempR;
				//it's below 0 so subtracting adds the remainder
			}
			
			if (inputSampleL < 0)
			{
				tempL = inputSampleL;
				while (tempL < 0) {tempL += incrementB;}
				inputSampleL -= tempL;
				//it's above 0 so subtracting subtracts the remainder
			}
			if (inputSampleR < 0)
			{
				tempR = inputSampleR;
				while (tempR < 0) {tempR += incrementB;}
				inputSampleR -= tempR;
				//it's above 0 so subtracting subtracts the remainder
			}
			
			inputSampleL *= (1.0 - incrementB);
			inputSampleR *= (1.0 - incrementB);
		}
		
		tempL = inputSampleL;
		tempR = inputSampleR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		if (inputSampleL > 0) inputSampleL = (pow(256,fabs(inputSampleL))-1.0) / 255;
		if (inputSampleL < 0) inputSampleL = -(pow(256,fabs(inputSampleL))-1.0) / 255;
		
		if (inputSampleR > 0) inputSampleR = (pow(256,fabs(inputSampleR))-1.0) / 255;
		if (inputSampleR < 0) inputSampleR = -(pow(256,fabs(inputSampleR))-1.0) / 255;
		
		inputSampleL = (tempL * hard) + (inputSampleL * (1.0-hard));
		inputSampleR = (tempR * hard) + (inputSampleR * (1.0-hard)); //uLaw decode as part of soft/hard
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
		lastSampleL = drySampleL;
		lastSampleR = drySampleR;
		
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

void DeRez2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double targetA = pow(A,3)+0.0005;
	if (targetA > 1.0) targetA = 1.0;
	double soften = (1.0 + targetA)/2;
	double targetB = pow(1.0-B,3) / 3;
	double hard = C;
	double wet = D;	
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	targetA /= overallscale;	
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		
		incrementA = ((incrementA*999.0)+targetA)/1000.0;
		incrementB = ((incrementB*999.0)+targetB)/1000.0;
		//incrementA is the frequency derez
		//incrementB is the bit depth derez
		position += incrementA;
		
		double outputSampleL = heldSampleL;
		double outputSampleR = heldSampleR;
		if (position > 1.0)
		{
			position -= 1.0;
			heldSampleL = (lastSampleL * position) + (inputSampleL * (1.0-position));
			outputSampleL = (outputSampleL * (1.0-soften)) + (heldSampleL * soften);
			//softens the edge of the derez
			heldSampleR = (lastSampleR * position) + (inputSampleR * (1.0-position));
			outputSampleR = (outputSampleR * (1.0-soften)) + (heldSampleR * soften);
			//softens the edge of the derez
		}
		inputSampleL = outputSampleL;
		inputSampleR = outputSampleR;
		
		double tempL = inputSampleL;
		double tempR = inputSampleR;
		
		if (inputSampleL != lastOutputSampleL) {
			tempL = inputSampleL;
			inputSampleL = (inputSampleL * hard) + (lastDrySampleL * (1.0-hard));
			//transitions get an intermediate dry sample
			lastOutputSampleL = tempL; //only one intermediate sample
		} else {
			lastOutputSampleL = inputSampleL;
		}
		
		if (inputSampleR != lastOutputSampleR) {
			tempR = inputSampleR;
			inputSampleR = (inputSampleR * hard) + (lastDrySampleR * (1.0-hard));
			//transitions get an intermediate dry sample
			lastOutputSampleR = tempR; //only one intermediate sample
		} else {
			lastOutputSampleR = inputSampleR;
		}
		
		lastDrySampleL = drySampleL;
		lastDrySampleR = drySampleR;
		//freq section of soft/hard interpolates dry samples
		
		tempL = inputSampleL;
		tempR = inputSampleR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		if (inputSampleL > 0) inputSampleL = log(1.0+(255*fabs(inputSampleL))) / log(256);
		if (inputSampleL < 0) inputSampleL = -log(1.0+(255*fabs(inputSampleL))) / log(256);
		
		if (inputSampleR > 0) inputSampleR = log(1.0+(255*fabs(inputSampleR))) / log(256);
		if (inputSampleR < 0) inputSampleR = -log(1.0+(255*fabs(inputSampleR))) / log(256);
		
		inputSampleL = (tempL * hard) + (inputSampleL * (1.0-hard));
		inputSampleR = (tempR * hard) + (inputSampleR * (1.0-hard)); //uLaw encode as part of soft/hard
		
		tempL = inputSampleL;
		tempR = inputSampleR;
		
		if (incrementB > 0.0005)
		{
			if (inputSampleL > 0)
			{
				tempL = inputSampleL;
				while (tempL > 0) {tempL -= incrementB;}
				inputSampleL -= tempL;
				//it's below 0 so subtracting adds the remainder
			}
			if (inputSampleR > 0)
			{
				tempR = inputSampleR;
				while (tempR > 0) {tempR -= incrementB;}
				inputSampleR -= tempR;
				//it's below 0 so subtracting adds the remainder
			}
			
			if (inputSampleL < 0)
			{
				tempL = inputSampleL;
				while (tempL < 0) {tempL += incrementB;}
				inputSampleL -= tempL;
				//it's above 0 so subtracting subtracts the remainder
			}
			if (inputSampleR < 0)
			{
				tempR = inputSampleR;
				while (tempR < 0) {tempR += incrementB;}
				inputSampleR -= tempR;
				//it's above 0 so subtracting subtracts the remainder
			}
			
			inputSampleL *= (1.0 - incrementB);
			inputSampleR *= (1.0 - incrementB);
		}
		
		tempL = inputSampleL;
		tempR = inputSampleR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		if (inputSampleL > 0) inputSampleL = (pow(256,fabs(inputSampleL))-1.0) / 255;
		if (inputSampleL < 0) inputSampleL = -(pow(256,fabs(inputSampleL))-1.0) / 255;
		
		if (inputSampleR > 0) inputSampleR = (pow(256,fabs(inputSampleR))-1.0) / 255;
		if (inputSampleR < 0) inputSampleR = -(pow(256,fabs(inputSampleR))-1.0) / 255;
		
		inputSampleL = (tempL * hard) + (inputSampleL * (1.0-hard));
		inputSampleR = (tempR * hard) + (inputSampleR * (1.0-hard)); //uLaw decode as part of soft/hard
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
		lastSampleL = drySampleL;
		lastSampleR = drySampleR;
		
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
