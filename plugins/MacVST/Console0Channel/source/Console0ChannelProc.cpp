/* ========================================
 *  Console0Channel - Console0Channel.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console0Channel_H
#include "Console0Channel.h"
#endif

void Console0Channel::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double gainControl = (A*0.5)+0.05; //0.0 to 1.0
	int gainBits = 20; //start beyond maximum attenuation
	if (gainControl > 0.0) gainBits = floor(1.0 / gainControl);
	int bitshiftL = gainBits - 3;
	int bitshiftR = gainBits - 3;
	double panControl = (B*2.0)-1.0; //-1.0 to 1.0
	double panAttenuation = (1.0-fabs(panControl));
	int panBits = 20; //start centered
	if (panAttenuation > 0.0) panBits = floor(1.0 / panAttenuation);
	if (panControl > 0.25) bitshiftL += panBits;
	if (panControl < -0.25) bitshiftR += panBits;
	if (bitshiftL < -2) bitshiftL = -2; if (bitshiftL > 17) bitshiftL = 17;
	if (bitshiftR < -2) bitshiftR = -2; if (bitshiftR > 17) bitshiftR = 17;
	double gainL = 1.0;
	double gainR = 1.0;
	switch (bitshiftL)
	{
		case 17: gainL = 0.0; break;
		case 16: gainL = 0.0000152587890625; break;
		case 15: gainL = 0.000030517578125; break;
		case 14: gainL = 0.00006103515625; break;
		case 13: gainL = 0.0001220703125; break;
		case 12: gainL = 0.000244140625; break;
		case 11: gainL = 0.00048828125; break;
		case 10: gainL = 0.0009765625; break;
		case 9: gainL = 0.001953125; break;
		case 8: gainL = 0.00390625; break;
		case 7: gainL = 0.0078125; break;
		case 6: gainL = 0.015625; break;
		case 5: gainL = 0.03125; break;
		case 4: gainL = 0.0625; break;
		case 3: gainL = 0.125; break;
		case 2: gainL = 0.25; break;
		case 1: gainL = 0.5; break;
		case 0: gainL = 1.0; break;
		case -1: gainL = 2.0; break;
		case -2: gainL = 4.0; break;
	}
	switch (bitshiftR)
	{
		case 17: gainR = 0.0; break;
		case 16: gainR = 0.0000152587890625; break;
		case 15: gainR = 0.000030517578125; break;
		case 14: gainR = 0.00006103515625; break;
		case 13: gainR = 0.0001220703125; break;
		case 12: gainR = 0.000244140625; break;
		case 11: gainR = 0.00048828125; break;
		case 10: gainR = 0.0009765625; break;
		case 9: gainR = 0.001953125; break;
		case 8: gainR = 0.00390625; break;
		case 7: gainR = 0.0078125; break;
		case 6: gainR = 0.015625; break;
		case 5: gainR = 0.03125; break;
		case 4: gainR = 0.0625; break;
		case 3: gainR = 0.125; break;
		case 2: gainR = 0.25; break;
		case 1: gainR = 0.5; break;
		case 0: gainR = 1.0; break;
		case -1: gainR = 2.0; break;
		case -2: gainR = 4.0; break;
	}
	double temp;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		temp = inputSampleL;
		inputSampleL = (inputSampleL + avgAL) * 0.5; avgAL = temp;
		temp = inputSampleR;
		inputSampleR = (inputSampleR + avgAR) * 0.5; avgAR = temp;
		
		inputSampleL *= gainL;
		inputSampleR *= gainR;
		
		if (inputSampleL > 1.4137166941154) inputSampleL = 1.4137166941154;
		if (inputSampleL < -1.4137166941154) inputSampleL = -1.4137166941154;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL/2.0)*(2.8274333882308-inputSampleL);
		else inputSampleL = -(inputSampleL/-2.0)*(2.8274333882308+inputSampleL);
		if (inputSampleR > 1.4137166941154) inputSampleR = 1.4137166941154;
		if (inputSampleR < -1.4137166941154) inputSampleR = -1.4137166941154;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR/2.0)*(2.8274333882308-inputSampleR);
		else inputSampleR = -(inputSampleR/-2.0)*(2.8274333882308+inputSampleR);
		//BigFastSin channel stage
		
		temp = inputSampleL;
		inputSampleL = (inputSampleL + avgBL) * 0.5; avgBL = temp;
		temp = inputSampleR;
		inputSampleR = (inputSampleR + avgBR) * 0.5; avgBR = temp;		
		
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

void Console0Channel::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double gainControl = (A*0.5)+0.05; //0.0 to 1.0
	int gainBits = 20; //start beyond maximum attenuation
	if (gainControl > 0.0) gainBits = floor(1.0 / gainControl);
	int bitshiftL = gainBits - 3;
	int bitshiftR = gainBits - 3;
	double panControl = (B*2.0)-1.0; //-1.0 to 1.0
	double panAttenuation = (1.0-fabs(panControl));
	int panBits = 20; //start centered
	if (panAttenuation > 0.0) panBits = floor(1.0 / panAttenuation);
	if (panControl > 0.25) bitshiftL += panBits;
	if (panControl < -0.25) bitshiftR += panBits;
	if (bitshiftL < -2) bitshiftL = -2; if (bitshiftL > 17) bitshiftL = 17;
	if (bitshiftR < -2) bitshiftR = -2; if (bitshiftR > 17) bitshiftR = 17;
	double gainL = 1.0;
	double gainR = 1.0;
	switch (bitshiftL)
	{
		case 17: gainL = 0.0; break;
		case 16: gainL = 0.0000152587890625; break;
		case 15: gainL = 0.000030517578125; break;
		case 14: gainL = 0.00006103515625; break;
		case 13: gainL = 0.0001220703125; break;
		case 12: gainL = 0.000244140625; break;
		case 11: gainL = 0.00048828125; break;
		case 10: gainL = 0.0009765625; break;
		case 9: gainL = 0.001953125; break;
		case 8: gainL = 0.00390625; break;
		case 7: gainL = 0.0078125; break;
		case 6: gainL = 0.015625; break;
		case 5: gainL = 0.03125; break;
		case 4: gainL = 0.0625; break;
		case 3: gainL = 0.125; break;
		case 2: gainL = 0.25; break;
		case 1: gainL = 0.5; break;
		case 0: gainL = 1.0; break;
		case -1: gainL = 2.0; break;
		case -2: gainL = 4.0; break;
	}
	switch (bitshiftR)
	{
		case 17: gainR = 0.0; break;
		case 16: gainR = 0.0000152587890625; break;
		case 15: gainR = 0.000030517578125; break;
		case 14: gainR = 0.00006103515625; break;
		case 13: gainR = 0.0001220703125; break;
		case 12: gainR = 0.000244140625; break;
		case 11: gainR = 0.00048828125; break;
		case 10: gainR = 0.0009765625; break;
		case 9: gainR = 0.001953125; break;
		case 8: gainR = 0.00390625; break;
		case 7: gainR = 0.0078125; break;
		case 6: gainR = 0.015625; break;
		case 5: gainR = 0.03125; break;
		case 4: gainR = 0.0625; break;
		case 3: gainR = 0.125; break;
		case 2: gainR = 0.25; break;
		case 1: gainR = 0.5; break;
		case 0: gainR = 1.0; break;
		case -1: gainR = 2.0; break;
		case -2: gainR = 4.0; break;
	}
	double temp;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		temp = inputSampleL;
		inputSampleL = (inputSampleL + avgAL) * 0.5; avgAL = temp;
		temp = inputSampleR;
		inputSampleR = (inputSampleR + avgAR) * 0.5; avgAR = temp;
		
		inputSampleL *= gainL;
		inputSampleR *= gainR;
		
		if (inputSampleL > 1.4137166941154) inputSampleL = 1.4137166941154;
		if (inputSampleL < -1.4137166941154) inputSampleL = -1.4137166941154;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL/2.0)*(2.8274333882308-inputSampleL);
		else inputSampleL = -(inputSampleL/-2.0)*(2.8274333882308+inputSampleL);
		if (inputSampleR > 1.4137166941154) inputSampleR = 1.4137166941154;
		if (inputSampleR < -1.4137166941154) inputSampleR = -1.4137166941154;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR/2.0)*(2.8274333882308-inputSampleR);
		else inputSampleR = -(inputSampleR/-2.0)*(2.8274333882308+inputSampleR);
		//BigFastSin channel stage
		
		temp = inputSampleL;
		inputSampleL = (inputSampleL + avgBL) * 0.5; avgBL = temp;
		temp = inputSampleR;
		inputSampleR = (inputSampleR + avgBR) * 0.5; avgBR = temp;		
		
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
