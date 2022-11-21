/* ========================================
 *  Air2 - Air2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Air2_H
#include "Air2.h"
#endif

void Air2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
	
	double hiIntensity = (A*2.0)-1.0;
	double tripletIntensity = (B*2.0)-1.0;
	double airIntensity = (C*2.0)-1.0;
	if (hiIntensity < 0.0) hiIntensity *= 0.57525;
	if (tripletIntensity < 0.0) tripletIntensity *= 0.71325;
	if (airIntensity < 0.0) airIntensity *= 0.5712;
	hiIntensity = -pow(hiIntensity,3);
	tripletIntensity = -pow(tripletIntensity,3);
	airIntensity = -pow(airIntensity,3);
	double hiQ = 1.5+fabs(hiIntensity*0.5);
	double tripletQ = 1.5+fabs(tripletIntensity*0.5);
	double airQ = 1.5+fabs(airIntensity*0.5);
	double intensity = (pow(D,3)*4.0) + 0.0001;
	double mix = E;
	double drymix = (1.0-mix)*4.0;
	if (drymix > 1.0) drymix = 1.0;

	//all types of air band are running in parallel, not series
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do an Air sample
			double correctionL = 0.0;
			double correctionR = 0.0;
			if (fabs(hiIntensity) > 0.0001) {
				airFactorCL = airPrevCL - inputSampleL;
				airFactorCR = airPrevCR - inputSampleR;
				if (flop)
				{
					airEvenCL += airFactorCL; airOddCL -= airFactorCL;
					airFactorCL = airEvenCL * hiIntensity;
					
					airEvenCR += airFactorCR; airOddCR -= airFactorCR;
					airFactorCR = airEvenCR * hiIntensity;
				}
				else
				{
					airOddCL += airFactorCL; airEvenCL -= airFactorCL;
					airFactorCL = airOddCL * hiIntensity;
					
					airOddCR += airFactorCR; airEvenCR -= airFactorCR;
					airFactorCR = airOddCR * hiIntensity;
				}
				airOddCL = (airOddCL - ((airOddCL - airEvenCL)/256.0)) / hiQ;
				airEvenCL = (airEvenCL - ((airEvenCL - airOddCL)/256.0)) / hiQ;
				airPrevCL = inputSampleL;
				correctionL = correctionL + airFactorCL;
				
				airOddCR = (airOddCR - ((airOddCR - airEvenCR)/256.0)) / hiQ;
				airEvenCR = (airEvenCR - ((airEvenCR - airOddCR)/256.0)) / hiQ;
				airPrevCR = inputSampleR;
				correctionR = correctionR + airFactorCR;
				flop = !flop;
			}//22k
			
			if (fabs(tripletIntensity) > 0.0001) {
				tripletFactorL = tripletPrevL - inputSampleL;
				tripletFactorR = tripletPrevR - inputSampleR;
				if (count < 1 || count > 3) count = 1;
				switch (count)
				{
					case 1:
						tripletAL += tripletFactorL; tripletCL -= tripletFactorL;
						tripletFactorL = tripletAL * tripletIntensity;
						tripletPrevL = tripletMidL; tripletMidL = inputSampleL;
						
						tripletAR += tripletFactorR; tripletCR -= tripletFactorR;
						tripletFactorR = tripletAR * tripletIntensity;
						tripletPrevR = tripletMidR; tripletMidR = inputSampleR;
						break;
					case 2:
						tripletBL += tripletFactorL; tripletAL -= tripletFactorL;
						tripletFactorL = tripletBL * tripletIntensity;
						tripletPrevL = tripletMidL; tripletMidL = inputSampleL;
						
						tripletBR += tripletFactorR; tripletAR -= tripletFactorR;
						tripletFactorR = tripletBR * tripletIntensity;
						tripletPrevR = tripletMidR; tripletMidR = inputSampleR;
						break;
					case 3:
						tripletCL += tripletFactorL; tripletBL -= tripletFactorL;
						tripletFactorL = tripletCL * tripletIntensity;
						tripletPrevL = tripletMidL; tripletMidL = inputSampleL;
						
						tripletCR += tripletFactorR; tripletBR -= tripletFactorR;
						tripletFactorR = tripletCR * tripletIntensity;
						tripletPrevR = tripletMidR; tripletMidR = inputSampleR;
						break;
				}
				tripletAL /= tripletQ; tripletBL /= tripletQ; tripletCL /= tripletQ;
				tripletAR /= tripletQ; tripletBR /= tripletQ; tripletCR /= tripletQ;
				correctionL = correctionL + tripletFactorL;
				correctionR = correctionR + tripletFactorR;
				count++;
			}//15K
			
			if (fabs(airIntensity) > 0.0001) {
				if (flop)
				{
					airFactorAL = airPrevAL - inputSampleL;
					airFactorAR = airPrevAR - inputSampleR;
					if (flipA)
					{
						airEvenAL += airFactorAL; airOddAL -= airFactorAL;
						airFactorAL = airEvenAL * airIntensity;
						airEvenAR += airFactorAR; airOddAR -= airFactorAR;
						airFactorAR = airEvenAR * airIntensity;
					}
					else
					{
						airOddAL += airFactorAL; airEvenAL -= airFactorAL;
						airFactorAL = airOddAL * airIntensity;
						airOddAR += airFactorAR; airEvenAR -= airFactorAR;
						airFactorAR = airOddAR * airIntensity;
					}
					airOddAL = (airOddAL - ((airOddAL - airEvenAL)/256.0)) / airQ;
					airEvenAL = (airEvenAL - ((airEvenAL - airOddAL)/256.0)) / airQ;
					airPrevAL = inputSampleL;
					correctionL = correctionL + airFactorAL;
					
					airOddAR = (airOddAR - ((airOddAR - airEvenAR)/256.0)) / airQ;
					airEvenAR = (airEvenAR - ((airEvenAR - airOddAR)/256.0)) / airQ;
					airPrevAR = inputSampleR;
					correctionR = correctionR + airFactorAR;
					flipA = !flipA;
				}
				else
				{
					airFactorBL = airPrevBL - inputSampleL;
					airFactorBR = airPrevBR - inputSampleR;
					if (flipB)
					{
						airEvenBL += airFactorBL; airOddBL -= airFactorBL;
						airFactorBL = airEvenBL * airIntensity;
						airEvenBR += airFactorBR; airOddBR -= airFactorBR;
						airFactorBR = airEvenBR * airIntensity;
					}
					else
					{
						airOddBL += airFactorBL; airEvenBL -= airFactorBL;
						airFactorBL = airOddBL * airIntensity;
						airOddBR += airFactorBR; airEvenBR -= airFactorBR;
						airFactorBR = airOddBR * airIntensity;
					}
					airOddBL = (airOddBL - ((airOddBL - airEvenBL)/256.0)) / airQ;
					airEvenBL = (airEvenBL - ((airEvenBL - airOddBL)/256.0)) / airQ;
					airPrevBL = inputSampleL;
					correctionL = correctionL + airFactorBL;
					
					airOddBR = (airOddBR - ((airOddBR - airEvenBR)/256.0)) / airQ;
					airEvenBR = (airEvenBR - ((airEvenBR - airOddBR)/256.0)) / airQ;
					airPrevBR = inputSampleR;
					correctionR = correctionR + airFactorBR;
					flipB = !flipB;
				}
			}//11K one
			
			correctionL *= intensity;
			correctionL -= 1.0;
			double bridgerectifier = fabs(correctionL);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (correctionL > 0) correctionL = bridgerectifier;
			else correctionL = -bridgerectifier;
			correctionL += postsine;
			correctionL /= intensity;
			
			correctionR *= intensity;
			correctionR -= 1.0;
			bridgerectifier = fabs(correctionR);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (correctionR > 0) correctionR = bridgerectifier;
			else correctionR = -bridgerectifier;
			correctionR += postsine;
			correctionR /= intensity;
			
			inputSampleL = correctionL * 4.0 * mix;
			inputSampleR = correctionR * 4.0 * mix;
			
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
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[7])*0.5;
				lastRefL[7] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[7])*0.5;
				lastRefR[7] = lastRefR[8]; //continue, do not break
			case 3:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[6])*0.5;
				lastRefL[6] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[6])*0.5;
				lastRefR[6] = lastRefR[8]; //continue, do not break
			case 2:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[5])*0.5;
				lastRefL[5] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[5])*0.5;
				lastRefR[5] = lastRefR[8]; //continue, do not break
			case 1:
				break; //no further averaging
		}
		
		if (drymix < 1.0) {drySampleL *= drymix; drySampleR *= drymix;}
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

void Air2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
	
	double hiIntensity = (A*2.0)-1.0;
	double tripletIntensity = (B*2.0)-1.0;
	double airIntensity = (C*2.0)-1.0;
	if (hiIntensity < 0.0) hiIntensity *= 0.57525;
	if (tripletIntensity < 0.0) tripletIntensity *= 0.71325;
	if (airIntensity < 0.0) airIntensity *= 0.5712;
	hiIntensity = -pow(hiIntensity,3);
	tripletIntensity = -pow(tripletIntensity,3);
	airIntensity = -pow(airIntensity,3);
	double hiQ = 1.5+fabs(hiIntensity*0.5);
	double tripletQ = 1.5+fabs(tripletIntensity*0.5);
	double airQ = 1.5+fabs(airIntensity*0.5);
	double intensity = (pow(D,3)*4.0) + 0.0001;
	double mix = E;
	double drymix = (1.0-mix)*4.0;
	if (drymix > 1.0) drymix = 1.0;
	//all types of air band are running in parallel, not series
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do an Air sample
			double correctionL = 0.0;
			double correctionR = 0.0;
			if (fabs(hiIntensity) > 0.0001) {
				airFactorCL = airPrevCL - inputSampleL;
				airFactorCR = airPrevCR - inputSampleR;
				if (flop)
				{
					airEvenCL += airFactorCL; airOddCL -= airFactorCL;
					airFactorCL = airEvenCL * hiIntensity;
					
					airEvenCR += airFactorCR; airOddCR -= airFactorCR;
					airFactorCR = airEvenCR * hiIntensity;
				}
				else
				{
					airOddCL += airFactorCL; airEvenCL -= airFactorCL;
					airFactorCL = airOddCL * hiIntensity;
					
					airOddCR += airFactorCR; airEvenCR -= airFactorCR;
					airFactorCR = airOddCR * hiIntensity;
				}
				airOddCL = (airOddCL - ((airOddCL - airEvenCL)/256.0)) / hiQ;
				airEvenCL = (airEvenCL - ((airEvenCL - airOddCL)/256.0)) / hiQ;
				airPrevCL = inputSampleL;
				correctionL = correctionL + airFactorCL;
				
				airOddCR = (airOddCR - ((airOddCR - airEvenCR)/256.0)) / hiQ;
				airEvenCR = (airEvenCR - ((airEvenCR - airOddCR)/256.0)) / hiQ;
				airPrevCR = inputSampleR;
				correctionR = correctionR + airFactorCR;
				flop = !flop;
			}//22k
			
			if (fabs(tripletIntensity) > 0.0001) {
				tripletFactorL = tripletPrevL - inputSampleL;
				tripletFactorR = tripletPrevR - inputSampleR;
				if (count < 1 || count > 3) count = 1;
				switch (count)
				{
					case 1:
						tripletAL += tripletFactorL; tripletCL -= tripletFactorL;
						tripletFactorL = tripletAL * tripletIntensity;
						tripletPrevL = tripletMidL; tripletMidL = inputSampleL;
						
						tripletAR += tripletFactorR; tripletCR -= tripletFactorR;
						tripletFactorR = tripletAR * tripletIntensity;
						tripletPrevR = tripletMidR; tripletMidR = inputSampleR;
						break;
					case 2:
						tripletBL += tripletFactorL; tripletAL -= tripletFactorL;
						tripletFactorL = tripletBL * tripletIntensity;
						tripletPrevL = tripletMidL; tripletMidL = inputSampleL;
						
						tripletBR += tripletFactorR; tripletAR -= tripletFactorR;
						tripletFactorR = tripletBR * tripletIntensity;
						tripletPrevR = tripletMidR; tripletMidR = inputSampleR;
						break;
					case 3:
						tripletCL += tripletFactorL; tripletBL -= tripletFactorL;
						tripletFactorL = tripletCL * tripletIntensity;
						tripletPrevL = tripletMidL; tripletMidL = inputSampleL;
						
						tripletCR += tripletFactorR; tripletBR -= tripletFactorR;
						tripletFactorR = tripletCR * tripletIntensity;
						tripletPrevR = tripletMidR; tripletMidR = inputSampleR;
						break;
				}
				tripletAL /= tripletQ; tripletBL /= tripletQ; tripletCL /= tripletQ;
				tripletAR /= tripletQ; tripletBR /= tripletQ; tripletCR /= tripletQ;
				correctionL = correctionL + tripletFactorL;
				correctionR = correctionR + tripletFactorR;
				count++;
			}//15K
			
			if (fabs(airIntensity) > 0.0001) {
				if (flop)
				{
					airFactorAL = airPrevAL - inputSampleL;
					airFactorAR = airPrevAR - inputSampleR;
					if (flipA)
					{
						airEvenAL += airFactorAL; airOddAL -= airFactorAL;
						airFactorAL = airEvenAL * airIntensity;
						airEvenAR += airFactorAR; airOddAR -= airFactorAR;
						airFactorAR = airEvenAR * airIntensity;
					}
					else
					{
						airOddAL += airFactorAL; airEvenAL -= airFactorAL;
						airFactorAL = airOddAL * airIntensity;
						airOddAR += airFactorAR; airEvenAR -= airFactorAR;
						airFactorAR = airOddAR * airIntensity;
					}
					airOddAL = (airOddAL - ((airOddAL - airEvenAL)/256.0)) / airQ;
					airEvenAL = (airEvenAL - ((airEvenAL - airOddAL)/256.0)) / airQ;
					airPrevAL = inputSampleL;
					correctionL = correctionL + airFactorAL;
					
					airOddAR = (airOddAR - ((airOddAR - airEvenAR)/256.0)) / airQ;
					airEvenAR = (airEvenAR - ((airEvenAR - airOddAR)/256.0)) / airQ;
					airPrevAR = inputSampleR;
					correctionR = correctionR + airFactorAR;
					flipA = !flipA;
				}
				else
				{
					airFactorBL = airPrevBL - inputSampleL;
					airFactorBR = airPrevBR - inputSampleR;
					if (flipB)
					{
						airEvenBL += airFactorBL; airOddBL -= airFactorBL;
						airFactorBL = airEvenBL * airIntensity;
						airEvenBR += airFactorBR; airOddBR -= airFactorBR;
						airFactorBR = airEvenBR * airIntensity;
					}
					else
					{
						airOddBL += airFactorBL; airEvenBL -= airFactorBL;
						airFactorBL = airOddBL * airIntensity;
						airOddBR += airFactorBR; airEvenBR -= airFactorBR;
						airFactorBR = airOddBR * airIntensity;
					}
					airOddBL = (airOddBL - ((airOddBL - airEvenBL)/256.0)) / airQ;
					airEvenBL = (airEvenBL - ((airEvenBL - airOddBL)/256.0)) / airQ;
					airPrevBL = inputSampleL;
					correctionL = correctionL + airFactorBL;
					
					airOddBR = (airOddBR - ((airOddBR - airEvenBR)/256.0)) / airQ;
					airEvenBR = (airEvenBR - ((airEvenBR - airOddBR)/256.0)) / airQ;
					airPrevBR = inputSampleR;
					correctionR = correctionR + airFactorBR;
					flipB = !flipB;
				}
			}//11K one
			
			correctionL *= intensity;
			correctionL -= 1.0;
			double bridgerectifier = fabs(correctionL);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (correctionL > 0) correctionL = bridgerectifier;
			else correctionL = -bridgerectifier;
			correctionL += postsine;
			correctionL /= intensity;
			
			correctionR *= intensity;
			correctionR -= 1.0;
			bridgerectifier = fabs(correctionR);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (correctionR > 0) correctionR = bridgerectifier;
			else correctionR = -bridgerectifier;
			correctionR += postsine;
			correctionR /= intensity;
			
			inputSampleL = correctionL * 4.0 * mix;
			inputSampleR = correctionR * 4.0 * mix;
			
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
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[7])*0.5;
				lastRefL[7] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[7])*0.5;
				lastRefR[7] = lastRefR[8]; //continue, do not break
			case 3:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[6])*0.5;
				lastRefL[6] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[6])*0.5;
				lastRefR[6] = lastRefR[8]; //continue, do not break
			case 2:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[5])*0.5;
				lastRefL[5] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[5])*0.5;
				lastRefR[5] = lastRefR[8]; //continue, do not break
			case 1:
				break; //no further averaging
		}
		
		if (drymix < 1.0) {drySampleL *= drymix; drySampleR *= drymix;}
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
