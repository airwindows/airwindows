/* ========================================
 *  Flipity - Flipity.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Flipity_H
#include "Flipity.h"
#endif

void Flipity::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    int flipity = (int)( A * 7.999 );
	static const int kDry = 0;
	static const int kFlipL = 1;
	static const int kFlipR = 2;
	static const int kFlipLR = 3;
	static const int kSwap = 4;
	static const int kSwipL = 5;
	static const int kSwipR = 6;
	static const int kSwipLR = 7;	
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		double temp;

		switch (flipity)
		{
			case kDry:
				break;
			case kFlipL:
				inputSampleL = -inputSampleL;
				break;
			case kFlipR:
				inputSampleR = -inputSampleR;
				break;
			case kFlipLR:
				inputSampleL = -inputSampleL;
				inputSampleR = -inputSampleR;
				break;
			case kSwap:
				temp = inputSampleL;
				inputSampleL = inputSampleR;
				inputSampleR = temp;
				break;
			case kSwipL:
				temp = inputSampleL;
				inputSampleL = -inputSampleR;
				inputSampleR = temp;
				break;
			case kSwipR:
				temp = inputSampleL;
				inputSampleL = inputSampleR;
				inputSampleR = -temp;
				break;
			case kSwipLR:
				temp = inputSampleL;
				inputSampleL = -inputSampleR;
				inputSampleR = -temp;
				break;
		}
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Flipity::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
    int flipity = (int)( A * 7.999 );
	static const int kDry = 0;
	static const int kFlipL = 1;
	static const int kFlipR = 2;
	static const int kFlipLR = 3;
	static const int kSwap = 4;
	static const int kSwipL = 5;
	static const int kSwipR = 6;
	static const int kSwipLR = 7;		
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		double temp;
		
		switch (flipity)
		{
			case kDry:
				break;
			case kFlipL:
				inputSampleL = -inputSampleL;
				break;
			case kFlipR:
				inputSampleR = -inputSampleR;
				break;
			case kFlipLR:
				inputSampleL = -inputSampleL;
				inputSampleR = -inputSampleR;
				break;
			case kSwap:
				temp = inputSampleL;
				inputSampleL = inputSampleR;
				inputSampleR = temp;
				break;
			case kSwipL:
				temp = inputSampleL;
				inputSampleL = -inputSampleR;
				inputSampleR = temp;
				break;
			case kSwipR:
				temp = inputSampleL;
				inputSampleL = inputSampleR;
				inputSampleR = -temp;
				break;
			case kSwipLR:
				temp = inputSampleL;
				inputSampleL = -inputSampleR;
				inputSampleR = -temp;
				break;
		}
				
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
