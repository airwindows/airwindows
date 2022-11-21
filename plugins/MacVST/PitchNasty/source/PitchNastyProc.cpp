/* ========================================
 *  PitchNasty - PitchNasty.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PitchNasty_H
#include "PitchNasty.h"
#endif

void PitchNasty::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double note = (int)(A*24)-12;// -12 to +12
	double bend = (B*24.0)-12.0;// -12 to +12
	double speed = pow(2,note/12.0)*pow(2,bend/12.0);
	
	double grindRef = (1.0/261.6)*getSampleRate(); //samples per Middle C cycle
	double grindNote = pow(2,((int)(C*72)-36)/12.0);
	double width = grindRef / grindNote;
	if (width > 9990) width = 9990; //safety check
	double feedback = D;
	double trim = E;
	double wet = F;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL += (feedbackSampleL * feedback);
		inputSampleR += (feedbackSampleR * feedback);
		if (fabs(feedbackSampleL) > 1.0) inputSampleL /= fabs(feedbackSampleL);
		if (fabs(feedbackSampleR) > 1.0) inputSampleR /= fabs(feedbackSampleR);
		
		dL[inCount] = inputSampleL;
		dR[inCount] = inputSampleR;
		
		inCount++;
		outCount += speed;
		
		if (outCount > inCount && switchTransition) {
			switchAmountL = 1.0;
			switchAmountR = 1.0;
			switchTransition = false;
		}
		
		if (inCount > width) inCount -= width;
		if (outCount > width) {
			outCount -= width;
			switchTransition = true;
			//reset the transition point
		}
		
		int count = floor(outCount);
		int arrayWidth = floor(width);
		
		inputSampleL = (dL[count-((count > arrayWidth)?arrayWidth+1:0)] * (1-(outCount-floor(outCount))));
		inputSampleL += (dL[count+1-((count+1 > arrayWidth)?arrayWidth+1:0)] * (outCount-floor(outCount)));
		
		inputSampleR = (dR[count-((count > arrayWidth)?arrayWidth+1:0)] * (1-(outCount-floor(outCount))));
		inputSampleR += (dR[count+1-((count+1 > arrayWidth)?arrayWidth+1:0)] * (outCount-floor(outCount)));
		
		if (switchAmountL > fabs(inputSampleL-dL[inCount])*2.0) {
			switchAmountL = (switchAmountL * 0.5) + (fabs(inputSampleL-dL[inCount]));
		}
		if (switchAmountR > fabs(inputSampleR-dR[inCount])*2.0) {
			switchAmountR = (switchAmountR * 0.5) + (fabs(inputSampleR-dR[inCount]));
		}
		inputSampleL = (dL[inCount] * switchAmountL) + (inputSampleL * (1.0-switchAmountL));
		inputSampleR = (dR[inCount] * switchAmountR) + (inputSampleR * (1.0-switchAmountR));
		
		feedbackSampleL = inputSampleL;
		feedbackSampleR = inputSampleR;
		
		if (trim != 1.0) {
			inputSampleL *= trim;
			inputSampleR *= trim;
		}
		
		if (wet !=1.0) {
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

void PitchNasty::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double note = (int)(A*24)-12;// -12 to +12
	double bend = (B*24.0)-12.0;// -12 to +12
	double speed = pow(2,note/12.0)*pow(2,bend/12.0);
	
	double grindRef = (1.0/261.6)*getSampleRate(); //samples per Middle C cycle
	double grindNote = pow(2,((int)(C*72)-36)/12.0);
	double width = grindRef / grindNote;
	if (width > 9990) width = 9990; //safety check
	double feedback = D;
	double trim = E;
	double wet = F;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL += (feedbackSampleL * feedback);
		inputSampleR += (feedbackSampleR * feedback);
		if (fabs(feedbackSampleL) > 1.0) inputSampleL /= fabs(feedbackSampleL);
		if (fabs(feedbackSampleR) > 1.0) inputSampleR /= fabs(feedbackSampleR);
		
		dL[inCount] = inputSampleL;
		dR[inCount] = inputSampleR;
		
		inCount++;
		outCount += speed;
		
		if (outCount > inCount && switchTransition) {
			switchAmountL = 1.0;
			switchAmountR = 1.0;
			switchTransition = false;
		}
		
		if (inCount > width) inCount -= width;
		if (outCount > width) {
			outCount -= width;
			switchTransition = true;
			//reset the transition point
		}
		
		int count = floor(outCount);
		int arrayWidth = floor(width);
		
		inputSampleL = (dL[count-((count > arrayWidth)?arrayWidth+1:0)] * (1-(outCount-floor(outCount))));
		inputSampleL += (dL[count+1-((count+1 > arrayWidth)?arrayWidth+1:0)] * (outCount-floor(outCount)));
		
		inputSampleR = (dR[count-((count > arrayWidth)?arrayWidth+1:0)] * (1-(outCount-floor(outCount))));
		inputSampleR += (dR[count+1-((count+1 > arrayWidth)?arrayWidth+1:0)] * (outCount-floor(outCount)));
		
		if (switchAmountL > fabs(inputSampleL-dL[inCount])*2.0) {
			switchAmountL = (switchAmountL * 0.5) + (fabs(inputSampleL-dL[inCount]));
		}
		if (switchAmountR > fabs(inputSampleR-dR[inCount])*2.0) {
			switchAmountR = (switchAmountR * 0.5) + (fabs(inputSampleR-dR[inCount]));
		}
		inputSampleL = (dL[inCount] * switchAmountL) + (inputSampleL * (1.0-switchAmountL));
		inputSampleR = (dR[inCount] * switchAmountR) + (inputSampleR * (1.0-switchAmountR));
		
		feedbackSampleL = inputSampleL;
		feedbackSampleR = inputSampleR;
		
		if (trim != 1.0) {
			inputSampleL *= trim;
			inputSampleR *= trim;
		}
		
		if (wet !=1.0) {
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
