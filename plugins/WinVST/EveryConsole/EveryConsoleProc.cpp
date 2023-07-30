/* ========================================
 *  EveryConsole - EveryConsole.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __EveryConsole_H
#include "EveryConsole.h"
#endif

void EveryConsole::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double half = 0.0;
	double falf = 0.0;
	int console = (int) A*11.999;
	double inTrim = B*2.0; //0-2
	double outTrim = C*2.0;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (inTrim != 1.0) {
			inputSampleL *= inTrim;
			inputSampleR *= inTrim;
		}
		
		switch (console)
		{
			case 0:
				half = inputSampleL * 0.83;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSampleL -= half;
				half = inputSampleR * 0.83;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSampleR -= half;
				break; //original ConsoleChannel, before sin/asin
				
			case 1:
				half = inputSampleL * 0.885;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSampleL += half;
				half = inputSampleR * 0.885;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSampleR += half;
				break; //original ConsoleBuss, before sin/asin
				
			case 2:
				if (inputSampleL > M_PI_2) inputSampleL = M_PI_2;
				if (inputSampleL < -M_PI_2) inputSampleL = -M_PI_2;
				//clip to max sine without any wavefolding
				inputSampleL = sin(inputSampleL);
				if (inputSampleR > M_PI_2) inputSampleR = M_PI_2;
				if (inputSampleR < -M_PI_2) inputSampleR = -M_PI_2;
				//clip to max sine without any wavefolding
				inputSampleR = sin(inputSampleR);
				break; //sin() function ConsoleChannel
				
			case 3:
				if (inputSampleL > 1.0) inputSampleL = 1.0;
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				//without this, you can get a NaN condition where it spits out DC offset at full blast!
				inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0;
				if (inputSampleR < -1.0) inputSampleR = -1.0;
				//without this, you can get a NaN condition where it spits out DC offset at full blast!
				inputSampleR = asin(inputSampleR);
				break; //sin() function ConsoleBuss
				
			case 4:
				//encode/decode courtesy of torridgristle under the MIT license
				if (inputSampleL > 1.0) inputSampleL= 1.0;
				else if (inputSampleL > 0.0) inputSampleL = 1.0 - pow(1.0-inputSampleL,2.0);
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				else if (inputSampleL < 0.0) inputSampleL = -1.0 + pow(1.0+inputSampleL,2.0);
				if (inputSampleR > 1.0) inputSampleR = 1.0;
				else if (inputSampleR > 0.0) inputSampleR = 1.0 - pow(1.0-inputSampleR,2.0);
				if (inputSampleR < -1.0) inputSampleR = -1.0;
				else if (inputSampleR < 0.0) inputSampleR = -1.0 + pow(1.0+inputSampleR,2.0);
				//Inverse Square 1-(1-x)^2 and 1-(1-x)^0.5 for Console6Channel
				break; //crude sine. Note that because modern processors love math more than extra variables, this is optimized
				
			case 5:
				//encode/decode courtesy of torridgristle under the MIT license
				if (inputSampleL > 1.0) inputSampleL= 1.0;
				else if (inputSampleL > 0.0) inputSampleL = 1.0 - pow(1.0-inputSampleL,0.5);
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				else if (inputSampleL < 0.0) inputSampleL = -1.0 + pow(1.0+inputSampleL,0.5);
				if (inputSampleR > 1.0) inputSampleR = 1.0;
				else if (inputSampleR > 0.0) inputSampleR = 1.0 - pow(1.0-inputSampleR,0.5);
				if (inputSampleR < -1.0) inputSampleR = -1.0;
				else if (inputSampleR < 0.0) inputSampleR = -1.0 + pow(1.0+inputSampleR,0.5);
				//Inverse Square 1-(1-x)^2 and 1-(1-x)^0.5 for Console6Buss
				break; //crude arcsine. Note that because modern processors love math more than extra variables, this is optimized
				
			case 6:
				if (inputSampleL > 1.097) inputSampleL = 1.097;
				if (inputSampleL < -1.097) inputSampleL = -1.097;
				inputSampleL = ((sin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.8)+(sin(inputSampleL)*0.2);
				if (inputSampleR > 1.097) inputSampleR = 1.097;
				if (inputSampleR < -1.097) inputSampleR = -1.097;
				inputSampleR = ((sin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.8)+(sin(inputSampleR)*0.2);
				//this is a version of Spiral blended 80/20 with regular Density.
				//It's blending between two different harmonics in the overtones of the algorithm
				break; //Console7Channel
				
			case 7:
				if (inputSampleL > 1.0) inputSampleL = 1.0;
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				inputSampleL = ((asin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.618033988749894848204586)+(asin(inputSampleL)*0.381966011250105);
				if (inputSampleR > 1.0) inputSampleR = 1.0;
				if (inputSampleR < -1.0) inputSampleR = -1.0;
				inputSampleR = ((asin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.618033988749894848204586)+(asin(inputSampleR)*0.381966011250105);
				//this is an asin version of Spiral blended with regular asin ConsoleBuss.
				//It's blending between two different harmonics in the overtones of the algorithm.
				break; //Console7Buss
				
			case 8:
				inputSampleL += ((pow(inputSampleL,5)/128.0) + (pow(inputSampleL,9)/262144.0)) - ((pow(inputSampleL,3)/8.0) + (pow(inputSampleL,7)/4096.0));
				inputSampleR += ((pow(inputSampleR,5)/128.0) + (pow(inputSampleR,9)/262144.0)) - ((pow(inputSampleR,3)/8.0) + (pow(inputSampleR,7)/4096.0));
				break; //crude sine. Note that because modern processors love math more than extra variables, this is optimized
				
			case 9:
				inputSampleL += (pow(inputSampleL,3)/4.0)+(pow(inputSampleL,5)/8.0)+(pow(inputSampleL,7)/16.0)+(pow(inputSampleL,9)/32.0);
				inputSampleR += (pow(inputSampleR,3)/4.0)+(pow(inputSampleR,5)/8.0)+(pow(inputSampleR,7)/16.0)+(pow(inputSampleR,9)/32.0);
				break; //crude arcsine. Note that because modern processors love math more than extra variables, this is optimized
				
			case 10:
				if (inputSampleL > 1.4137166941154) inputSampleL = 1.4137166941154;
				if (inputSampleL < -1.4137166941154) inputSampleL = -1.4137166941154;
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL/2.0)*(2.8274333882308-inputSampleL);
				else inputSampleL = -(inputSampleL/-2.0)*(2.8274333882308+inputSampleL);
				if (inputSampleR > 1.4137166941154) inputSampleR = 1.4137166941154;
				if (inputSampleR < -1.4137166941154) inputSampleR = -1.4137166941154;
				if (inputSampleR > 0.0) inputSampleR = (inputSampleR/2.0)*(2.8274333882308-inputSampleR);
				else inputSampleR = -(inputSampleR/-2.0)*(2.8274333882308+inputSampleR);
				break; //ConsoleZeroChannel
				
			case 11:
				if (inputSampleL > 2.8) inputSampleL = 2.8;
				if (inputSampleL < -2.8) inputSampleL = -2.8;
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL*2.0)/(3.0-inputSampleL);
				else inputSampleL = -(inputSampleL*-2.0)/(3.0+inputSampleL);
				if (inputSampleR > 2.8) inputSampleR = 2.8;
				if (inputSampleR < -2.8) inputSampleR = -2.8;
				if (inputSampleR > 0.0) inputSampleR = (inputSampleR*2.0)/(3.0-inputSampleR);
				else inputSampleR = -(inputSampleR*-2.0)/(3.0+inputSampleR);
				break; //ConsoleZeroBuss
		}
		
		if (outTrim != 1.0) {
			inputSampleL *= outTrim;
			inputSampleR *= outTrim;
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

void EveryConsole::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double half = 0.0;
	double falf = 0.0;
	int console = (int) A*11.999;
	double inTrim = B*2.0; //0-2
	double outTrim = C*2.0;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (inTrim != 1.0) {
			inputSampleL *= inTrim;
			inputSampleR *= inTrim;
		}
		
		switch (console)
		{
			case 0:
				half = inputSampleL * 0.83;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSampleL -= half;
				half = inputSampleR * 0.83;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSampleR -= half;
				break; //original ConsoleChannel, before sin/asin
				
			case 1:
				half = inputSampleL * 0.885;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSampleL += half;
				half = inputSampleR * 0.885;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSampleR += half;
				break; //original ConsoleBuss, before sin/asin
				
			case 2:
				if (inputSampleL > M_PI_2) inputSampleL = M_PI_2;
				if (inputSampleL < -M_PI_2) inputSampleL = -M_PI_2;
				//clip to max sine without any wavefolding
				inputSampleL = sin(inputSampleL);
				if (inputSampleR > M_PI_2) inputSampleR = M_PI_2;
				if (inputSampleR < -M_PI_2) inputSampleR = -M_PI_2;
				//clip to max sine without any wavefolding
				inputSampleR = sin(inputSampleR);
				break; //sin() function ConsoleChannel
				
			case 3:
				if (inputSampleL > 1.0) inputSampleL = 1.0;
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				//without this, you can get a NaN condition where it spits out DC offset at full blast!
				inputSampleL = asin(inputSampleL);
				if (inputSampleR > 1.0) inputSampleR = 1.0;
				if (inputSampleR < -1.0) inputSampleR = -1.0;
				//without this, you can get a NaN condition where it spits out DC offset at full blast!
				inputSampleR = asin(inputSampleR);
				break; //sin() function ConsoleBuss
				
			case 4:
				//encode/decode courtesy of torridgristle under the MIT license
				if (inputSampleL > 1.0) inputSampleL= 1.0;
				else if (inputSampleL > 0.0) inputSampleL = 1.0 - pow(1.0-inputSampleL,2.0);
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				else if (inputSampleL < 0.0) inputSampleL = -1.0 + pow(1.0+inputSampleL,2.0);
				if (inputSampleR > 1.0) inputSampleR = 1.0;
				else if (inputSampleR > 0.0) inputSampleR = 1.0 - pow(1.0-inputSampleR,2.0);
				if (inputSampleR < -1.0) inputSampleR = -1.0;
				else if (inputSampleR < 0.0) inputSampleR = -1.0 + pow(1.0+inputSampleR,2.0);
				//Inverse Square 1-(1-x)^2 and 1-(1-x)^0.5 for Console6Channel
				break; //crude sine. Note that because modern processors love math more than extra variables, this is optimized
				
			case 5:
				//encode/decode courtesy of torridgristle under the MIT license
				if (inputSampleL > 1.0) inputSampleL= 1.0;
				else if (inputSampleL > 0.0) inputSampleL = 1.0 - pow(1.0-inputSampleL,0.5);
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				else if (inputSampleL < 0.0) inputSampleL = -1.0 + pow(1.0+inputSampleL,0.5);
				if (inputSampleR > 1.0) inputSampleR = 1.0;
				else if (inputSampleR > 0.0) inputSampleR = 1.0 - pow(1.0-inputSampleR,0.5);
				if (inputSampleR < -1.0) inputSampleR = -1.0;
				else if (inputSampleR < 0.0) inputSampleR = -1.0 + pow(1.0+inputSampleR,0.5);
				//Inverse Square 1-(1-x)^2 and 1-(1-x)^0.5 for Console6Buss
				break; //crude arcsine. Note that because modern processors love math more than extra variables, this is optimized
				
			case 6:
				if (inputSampleL > 1.097) inputSampleL = 1.097;
				if (inputSampleL < -1.097) inputSampleL = -1.097;
				inputSampleL = ((sin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.8)+(sin(inputSampleL)*0.2);
				if (inputSampleR > 1.097) inputSampleR = 1.097;
				if (inputSampleR < -1.097) inputSampleR = -1.097;
				inputSampleR = ((sin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.8)+(sin(inputSampleR)*0.2);
				//this is a version of Spiral blended 80/20 with regular Density.
				//It's blending between two different harmonics in the overtones of the algorithm
				break; //Console7Channel
				
			case 7:
				if (inputSampleL > 1.0) inputSampleL = 1.0;
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				inputSampleL = ((asin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.618033988749894848204586)+(asin(inputSampleL)*0.381966011250105);
				if (inputSampleR > 1.0) inputSampleR = 1.0;
				if (inputSampleR < -1.0) inputSampleR = -1.0;
				inputSampleR = ((asin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.618033988749894848204586)+(asin(inputSampleR)*0.381966011250105);
				//this is an asin version of Spiral blended with regular asin ConsoleBuss.
				//It's blending between two different harmonics in the overtones of the algorithm.
				break; //Console7Buss
				
			case 8:
				inputSampleL += ((pow(inputSampleL,5)/128.0) + (pow(inputSampleL,9)/262144.0)) - ((pow(inputSampleL,3)/8.0) + (pow(inputSampleL,7)/4096.0));
				inputSampleR += ((pow(inputSampleR,5)/128.0) + (pow(inputSampleR,9)/262144.0)) - ((pow(inputSampleR,3)/8.0) + (pow(inputSampleR,7)/4096.0));
				break; //crude sine. Note that because modern processors love math more than extra variables, this is optimized
				
			case 9:
				inputSampleL += (pow(inputSampleL,3)/4.0)+(pow(inputSampleL,5)/8.0)+(pow(inputSampleL,7)/16.0)+(pow(inputSampleL,9)/32.0);
				inputSampleR += (pow(inputSampleR,3)/4.0)+(pow(inputSampleR,5)/8.0)+(pow(inputSampleR,7)/16.0)+(pow(inputSampleR,9)/32.0);
				break; //crude arcsine. Note that because modern processors love math more than extra variables, this is optimized
				
			case 10:
				if (inputSampleL > 1.4137166941154) inputSampleL = 1.4137166941154;
				if (inputSampleL < -1.4137166941154) inputSampleL = -1.4137166941154;
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL/2.0)*(2.8274333882308-inputSampleL);
				else inputSampleL = -(inputSampleL/-2.0)*(2.8274333882308+inputSampleL);
				if (inputSampleR > 1.4137166941154) inputSampleR = 1.4137166941154;
				if (inputSampleR < -1.4137166941154) inputSampleR = -1.4137166941154;
				if (inputSampleR > 0.0) inputSampleR = (inputSampleR/2.0)*(2.8274333882308-inputSampleR);
				else inputSampleR = -(inputSampleR/-2.0)*(2.8274333882308+inputSampleR);
				break; //ConsoleZeroChannel
				
			case 11:
				if (inputSampleL > 2.8) inputSampleL = 2.8;
				if (inputSampleL < -2.8) inputSampleL = -2.8;
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL*2.0)/(3.0-inputSampleL);
				else inputSampleL = -(inputSampleL*-2.0)/(3.0+inputSampleL);
				if (inputSampleR > 2.8) inputSampleR = 2.8;
				if (inputSampleR < -2.8) inputSampleR = -2.8;
				if (inputSampleR > 0.0) inputSampleR = (inputSampleR*2.0)/(3.0-inputSampleR);
				else inputSampleR = -(inputSampleR*-2.0)/(3.0+inputSampleR);
				break; //ConsoleZeroBuss
		}
		
		if (outTrim != 1.0) {
			inputSampleL *= outTrim;
			inputSampleR *= outTrim;
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
