/* ========================================
 *  ADT - ADT.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __ADT_H
#include "ADT.h"
#endif

void ADT::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double gain = A * 1.272;
	double targetA = pow(B,4) * 4790.0;
	double fractionA;
	double minusA;
	double intensityA = C-0.5;
	//first delay
	double targetB = (pow(D,4) * 4790.0);
	double fractionB;
	double minusB;
	double intensityB = E-0.5;
	//second delay
	double output = F*2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-37) inputSampleL = fpd * 1.18e-37;
		if (fabs(inputSampleR)<1.18e-37) inputSampleR = fpd * 1.18e-37;

				
		if (fabs(offsetA - targetA) > 1000) offsetA = targetA;
		offsetA = ((offsetA*999.0)+targetA)/1000.0;
		fractionA = offsetA - floor(offsetA);
		minusA = 1.0 - fractionA;
		
		if (fabs(offsetB - targetB) > 1000) offsetB = targetB;
		offsetB = ((offsetB*999.0)+targetB)/1000.0;
		fractionB = offsetB - floor(offsetB);
		minusB = 1.0 - fractionB;
		//chase delay taps for smooth action
		
		if (gain > 0) {inputSampleL /= gain; inputSampleR /= gain;}
		
		if (inputSampleL > 1.2533141373155) inputSampleL = 1.2533141373155;
		if (inputSampleL < -1.2533141373155) inputSampleL = -1.2533141373155;
		if (inputSampleR > 1.2533141373155) inputSampleR = 1.2533141373155;
		if (inputSampleR < -1.2533141373155) inputSampleR = -1.2533141373155;

		inputSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		inputSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		//Spiral: lean out the sound a little when decoded by ConsoleBuss
		
		if (gcount < 1 || gcount > 4800) {gcount = 4800;}
		int count = gcount;
		double totalL = 0.0;
		double totalR = 0.0;
		double tempL;
		double tempR;
		pL[count+4800] = pL[count] = inputSampleL;
		pR[count+4800] = pR[count] = inputSampleR;
		//double buffer
		
		if (intensityA != 0.0)
		{
			count = (int)(gcount+floor(offsetA));

			tempL = (pL[count] * minusA); //less as value moves away from .0
			tempL += pL[count+1]; //we can assume always using this in one way or another?
			tempL += (pL[count+2] * fractionA); //greater as value moves away from .0
			tempL -= (((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50); //interpolation hacks 'r us
			totalL += (tempL * intensityA);

			tempR = (pR[count] * minusA); //less as value moves away from .0
			tempR += pR[count+1]; //we can assume always using this in one way or another?
			tempR += (pR[count+2] * fractionA); //greater as value moves away from .0
			tempR -= (((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50); //interpolation hacks 'r us
			totalR += (tempR * intensityA);
		}
		
		if (intensityB != 0.0)
		{
			count = (int)(gcount+floor(offsetB));

			tempL = (pL[count] * minusB); //less as value moves away from .0
			tempL += pL[count+1]; //we can assume always using this in one way or another?
			tempL += (pL[count+2] * fractionB); //greater as value moves away from .0
			tempL -= (((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50); //interpolation hacks 'r us
			totalL += (tempL * intensityB);

			tempR = (pR[count] * minusB); //less as value moves away from .0
			tempR += pR[count+1]; //we can assume always using this in one way or another?
			tempR += (pR[count+2] * fractionB); //greater as value moves away from .0
			tempR -= (((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50); //interpolation hacks 'r us
			totalR += (tempR * intensityB);
		}
		
		gcount--;
		//still scrolling through the samples, remember
		
		inputSampleL += totalL;
		inputSampleR += totalR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		//without this, you can get a NaN condition where it spits out DC offset at full blast!
		
		inputSampleL = asin(inputSampleL);
		inputSampleR = asin(inputSampleR);
		//amplitude aspect
		
		inputSampleL *= gain;
		inputSampleR *= gain;
		
		if (output < 1.0) {inputSampleL *= output; inputSampleR *= output;}
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		frexpf((float)inputSampleR, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleR += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void ADT::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double gain = A * 1.272;
	double targetA = pow(B,4) * 4790.0;
	double fractionA;
	double minusA;
	double intensityA = C-0.5;
	//first delay
	double targetB = (pow(D,4) * 4790.0);
	double fractionB;
	double minusB;
	double intensityB = E-0.5;
	//second delay
	double output = F*2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-43) inputSampleL = fpd * 1.18e-43;
		if (fabs(inputSampleR)<1.18e-43) inputSampleR = fpd * 1.18e-43;

		
		if (fabs(offsetA - targetA) > 1000) offsetA = targetA;
		offsetA = ((offsetA*999.0)+targetA)/1000.0;
		fractionA = offsetA - floor(offsetA);
		minusA = 1.0 - fractionA;
		
		if (fabs(offsetB - targetB) > 1000) offsetB = targetB;
		offsetB = ((offsetB*999.0)+targetB)/1000.0;
		fractionB = offsetB - floor(offsetB);
		minusB = 1.0 - fractionB;
		//chase delay taps for smooth action
		
		if (gain > 0) {inputSampleL /= gain; inputSampleR /= gain;}
		
		if (inputSampleL > 1.2533141373155) inputSampleL = 1.2533141373155;
		if (inputSampleL < -1.2533141373155) inputSampleL = -1.2533141373155;
		if (inputSampleR > 1.2533141373155) inputSampleR = 1.2533141373155;
		if (inputSampleR < -1.2533141373155) inputSampleR = -1.2533141373155;
		
		inputSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		inputSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		//Spiral: lean out the sound a little when decoded by ConsoleBuss
		
		if (gcount < 1 || gcount > 4800) {gcount = 4800;}
		int count = gcount;
		double totalL = 0.0;
		double totalR = 0.0;
		double tempL;
		double tempR;
		pL[count+4800] = pL[count] = inputSampleL;
		pR[count+4800] = pR[count] = inputSampleR;
		//double buffer
		
		if (intensityA != 0.0)
		{
			count = (int)(gcount+floor(offsetA));
			
			tempL = (pL[count] * minusA); //less as value moves away from .0
			tempL += pL[count+1]; //we can assume always using this in one way or another?
			tempL += (pL[count+2] * fractionA); //greater as value moves away from .0
			tempL -= (((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50); //interpolation hacks 'r us
			totalL += (tempL * intensityA);
			
			tempR = (pR[count] * minusA); //less as value moves away from .0
			tempR += pR[count+1]; //we can assume always using this in one way or another?
			tempR += (pR[count+2] * fractionA); //greater as value moves away from .0
			tempR -= (((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50); //interpolation hacks 'r us
			totalR += (tempR * intensityA);
		}
		
		if (intensityB != 0.0)
		{
			count = (int)(gcount+floor(offsetB));
			
			tempL = (pL[count] * minusB); //less as value moves away from .0
			tempL += pL[count+1]; //we can assume always using this in one way or another?
			tempL += (pL[count+2] * fractionB); //greater as value moves away from .0
			tempL -= (((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50); //interpolation hacks 'r us
			totalL += (tempL * intensityB);
			
			tempR = (pR[count] * minusB); //less as value moves away from .0
			tempR += pR[count+1]; //we can assume always using this in one way or another?
			tempR += (pR[count+2] * fractionB); //greater as value moves away from .0
			tempR -= (((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50); //interpolation hacks 'r us
			totalR += (tempR * intensityB);
		}
		
		gcount--;
		//still scrolling through the samples, remember
		
		inputSampleL += totalL;
		inputSampleR += totalR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		//without this, you can get a NaN condition where it spits out DC offset at full blast!
		
		inputSampleL = asin(inputSampleL);
		inputSampleR = asin(inputSampleR);
		//amplitude aspect
		
		inputSampleL *= gain;
		inputSampleR *= gain;
		
		if (output < 1.0) {inputSampleL *= output; inputSampleR *= output;}
		
		//begin 64 bit stereo floating point dither
		int expon; frexp((double)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += static_cast<int32_t>(fpd) * 1.110223024625156e-44L * pow(2,expon+62);
		frexp((double)inputSampleR, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleR += static_cast<int32_t>(fpd) * 1.110223024625156e-44L * pow(2,expon+62);
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
