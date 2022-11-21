/* ========================================
 *  Vibrato - Vibrato.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Vibrato_H
#include "Vibrato.h"
#endif

void Vibrato::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	double speed = pow(0.1+A,6);
	double depth = (pow(B,3) / sqrt(speed))*4.0;
	double speedB = pow(0.1+C,6);
	double depthB = pow(D,3) / sqrt(speedB);
	double tupi = 3.141592653589793238 * 2.0;
	double wet = (E*2.0)-1.0; //note: inv/dry/wet
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		airFactorL = airPrevL - inputSampleL;
		airFactorR = airPrevR - inputSampleR;
		
		if (flip) {
			airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;
			airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;
		} else {
			airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;
			airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;
		}
		
		airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
		airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
		airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
		airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
		airPrevL = inputSampleL;
		airPrevR = inputSampleR;
		inputSampleL += airFactorL;
		inputSampleR += airFactorR;
		
		flip = !flip;
		//air, compensates for loss of highs in the interpolation
		
		if (gcount < 1 || gcount > 8192) {gcount = 8192;}
		int count = gcount;
		pL[count+8192] = pL[count] = inputSampleL;
		pR[count+8192] = pR[count] = inputSampleR;
		
		double offset = depth + (depth * sin(sweep));
		count += (int)floor(offset);
		
		inputSampleL = pL[count] * (1.0-(offset-floor(offset))); //less as value moves away from .0
		inputSampleL += pL[count+1]; //we can assume always using this in one way or another?
		inputSampleL += pL[count+2] * (offset-floor(offset)); //greater as value moves away from .0
		inputSampleL -= ((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50.0; //interpolation hacks 'r us
		inputSampleL *= 0.5; // gain trim
		
		inputSampleR = pR[count] * (1.0-(offset-floor(offset))); //less as value moves away from .0
		inputSampleR += pR[count+1]; //we can assume always using this in one way or another?
		inputSampleR += pR[count+2] * (offset-floor(offset)); //greater as value moves away from .0
		inputSampleR -= ((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50.0; //interpolation hacks 'r us
		inputSampleR *= 0.5; // gain trim
		
		sweep += (speed + (speedB * sin(sweepB) * depthB));
		sweepB += speedB;
		if (sweep > tupi){sweep -= tupi;}
		if (sweep < 0.0){sweep += tupi;} //through zero FM
		if (sweepB > tupi){sweepB -= tupi;}
		gcount--;
		//still scrolling through the samples, remember
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-fabs(wet)));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-fabs(wet)));
		}
		//Inv/Dry/Wet control
		
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

void Vibrato::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];	
	double speed = pow(0.1+A,6);
	double depth = (pow(B,3) / sqrt(speed))*4.0;
	double speedB = pow(0.1+C,6);
	double depthB = pow(D,3) / sqrt(speedB);
	double tupi = 3.141592653589793238 * 2.0;
	double wet = (E*2.0)-1.0; //note: inv/dry/wet
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		airFactorL = airPrevL - inputSampleL;
		airFactorR = airPrevR - inputSampleR;
		
		if (flip) {
			airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;
			airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;
		} else {
			airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;
			airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;
		}
		
		airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
		airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
		airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
		airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
		airPrevL = inputSampleL;
		airPrevR = inputSampleR;
		inputSampleL += airFactorL;
		inputSampleR += airFactorR;
		
		flip = !flip;
		//air, compensates for loss of highs in the interpolation
		
		if (gcount < 1 || gcount > 8192) {gcount = 8192;}
		int count = gcount;
		pL[count+8192] = pL[count] = inputSampleL;
		pR[count+8192] = pR[count] = inputSampleR;
		
		double offset = depth + (depth * sin(sweep));
		count += (int)floor(offset);
		
		inputSampleL = pL[count] * (1.0-(offset-floor(offset))); //less as value moves away from .0
		inputSampleL += pL[count+1]; //we can assume always using this in one way or another?
		inputSampleL += pL[count+2] * (offset-floor(offset)); //greater as value moves away from .0
		inputSampleL -= ((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50.0; //interpolation hacks 'r us
		inputSampleL *= 0.5; // gain trim
		
		inputSampleR = pR[count] * (1.0-(offset-floor(offset))); //less as value moves away from .0
		inputSampleR += pR[count+1]; //we can assume always using this in one way or another?
		inputSampleR += pR[count+2] * (offset-floor(offset)); //greater as value moves away from .0
		inputSampleR -= ((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50.0; //interpolation hacks 'r us
		inputSampleR *= 0.5; // gain trim
		
		sweep += (speed + (speedB * sin(sweepB) * depthB));
		sweepB += speedB;
		if (sweep > tupi){sweep -= tupi;}
		if (sweep < 0.0){sweep += tupi;} //through zero FM
		if (sweepB > tupi){sweepB -= tupi;}
		gcount--;
		//still scrolling through the samples, remember
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-fabs(wet)));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-fabs(wet)));
		}
		//Inv/Dry/Wet control
		
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
