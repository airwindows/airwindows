/* ========================================
 *  StereoChorus - StereoChorus.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StereoChorus_H
#include "StereoChorus.h"
#endif

void StereoChorus::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	double speed = pow(0.32+(A/6),10);
	double depth = (B/60) / speed;
	double tupi = 3.141592653589793238 * 2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a chorus sample
			//assign working variables
			airFactorL = airPrevL - inputSampleL;
			if (flip) {airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;}
			else {airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;}
			airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
			airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
			airPrevL = inputSampleL;
			inputSampleL += airFactorL;
			//left
			airFactorR = airPrevR - inputSampleR;
			if (flip) {airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;}
			else {airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;}
			airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
			airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
			airPrevR = inputSampleR;
			inputSampleR += airFactorR;
			//right
			flip = !flip;
			//air, compensates for loss of highs in flanger's interpolation
			
			int tempL = 0;
			int tempR = 0;
			if (gcount < 1 || gcount > 32760) {gcount = 32760;}
			int count = gcount;
			pL[count+32760] = pL[count] = (int)(inputSampleL*8388352.0);
			//double buffer -8388352 to 8388352 is equal to 24 bit linear space
			double offset = depth + (depth * sin(sweepL));
			count += (int)floor(offset);
			tempL += (int)(pL[count] * (1-(offset-floor(offset)))); //less as value moves away from .0
			tempL += pL[count+1]; //we can assume always using this in one way or another?
			tempL += (int)(pL[count+2] * (offset-floor(offset))); //greater as value moves away from .0
			tempL -= (int)(((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50); //interpolation hacks 'r us
			//left
			
			count = gcount;
			pR[count+32760] = pR[count] = (int)(inputSampleR*8388352.0);
			//double buffer -8388352 to 8388352 is equal to 24 bit linear space
			offset = depth + (depth * sin(sweepR));
			count += (int)floor(offset);
			tempR += (int)(pR[count] * (1-(offset-floor(offset)))); //less as value moves away from .0
			tempR += pR[count+1]; //we can assume always using this in one way or another?
			tempR += (int)(pR[count+2] * (offset-floor(offset))); //greater as value moves away from .0
			tempR -= (int)(((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50); //interpolation hacks 'r us
			//right
			
			sweepL += speed;
			sweepR += speed;
			if (sweepL > tupi){sweepL -= tupi;}
			if (sweepR > tupi){sweepR -= tupi;}
			gcount--;
			//still scrolling through the samples, remember
			
			inputSampleL = ((double)(tempL/16776704.0));
			inputSampleR = ((double)(tempR/16776704.0));
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
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

void StereoChorus::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	double speed = pow(0.32+(A/6),10);
	double depth = (B/60) / speed;
	double tupi = 3.141592653589793238 * 2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a chorus sample
			//assign working variables
			airFactorL = airPrevL - inputSampleL;
			if (flip) {airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;}
			else {airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;}
			airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
			airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
			airPrevL = inputSampleL;
			inputSampleL += airFactorL;
			//left
			airFactorR = airPrevR - inputSampleR;
			if (flip) {airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;}
			else {airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;}
			airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
			airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
			airPrevR = inputSampleR;
			inputSampleR += airFactorR;
			//right
			flip = !flip;
			//air, compensates for loss of highs in flanger's interpolation
			
			int tempL = 0;
			int tempR = 0;
			if (gcount < 1 || gcount > 32760) {gcount = 32760;}
			int count = gcount;
			pL[count+32760] = pL[count] = (int)(inputSampleL*8388352.0);
			//double buffer -8388352 to 8388352 is equal to 24 bit linear space
			double offset = depth + (depth * sin(sweepL));
			count += (int)floor(offset);
			tempL += (int)(pL[count] * (1-(offset-floor(offset)))); //less as value moves away from .0
			tempL += pL[count+1]; //we can assume always using this in one way or another?
			tempL += (int)(pL[count+2] * (offset-floor(offset))); //greater as value moves away from .0
			tempL -= (int)(((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50); //interpolation hacks 'r us
			//left
			
			count = gcount;
			pR[count+32760] = pR[count] = (int)(inputSampleR*8388352.0);
			//double buffer -8388352 to 8388352 is equal to 24 bit linear space
			offset = depth + (depth * sin(sweepR));
			count += (int)floor(offset);
			tempR += (int)(pR[count] * (1-(offset-floor(offset)))); //less as value moves away from .0
			tempR += pR[count+1]; //we can assume always using this in one way or another?
			tempR += (int)(pR[count+2] * (offset-floor(offset))); //greater as value moves away from .0
			tempR -= (int)(((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50); //interpolation hacks 'r us
			//right
			
			sweepL += speed;
			sweepR += speed;
			if (sweepL > tupi){sweepL -= tupi;}
			if (sweepR > tupi){sweepR -= tupi;}
			gcount--;
			//still scrolling through the samples, remember
			
			inputSampleL = ((double)(tempL/16776704.0));
			inputSampleR = ((double)(tempR/16776704.0));
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
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
