/* ========================================
 *  Compresaturator - Compresaturator.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Compresaturator_H
#include "Compresaturator.h"
#endif

void Compresaturator::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double inputgain = pow(10.0,((A*24.0)-12.0)/20.0);
	double satComp = B*2.0;
	int widestRange = C*C*C*5000;
	if (widestRange < 50) widestRange = 50;
	satComp += (((double)widestRange/3000.0)*satComp);
	//set the max wideness of comp zone, minimum range boosted (too much?)
	double output = D;
	double wet = E;
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (dCount < 1 || dCount > 5000) {dCount = 5000;}
		
		//begin drive L
		double temp = inputSampleL;
		double variSpeed = 1.0 + ((padFactorL/lastWidthL)*satComp);
		if (variSpeed < 1.0) variSpeed = 1.0;
		double totalgain = inputgain / variSpeed;
		if (totalgain != 1.0) {
			inputSampleL *= totalgain;
			if (totalgain < 1.0) {
				temp *= totalgain;
				//no boosting beyond unity please
			}
		}
		
		double bridgerectifier = fabs(inputSampleL);
		double overspill = 0;
		int targetWidth = widestRange;
		//we now have defaults and an absolute input value to work with
		if (bridgerectifier < 0.01) padFactorL *= 0.9999;
		//in silences we bring back padFactor if it got out of hand		
		if (bridgerectifier > 1.57079633) {
			bridgerectifier = 1.57079633;
			targetWidth = 8;
		}
		//if our output's gone beyond saturating to distorting, we begin chasing the
		//buffer size smaller. Anytime we don't have that, we expand (smoothest sound, only adding to an increasingly subdivided buffer)
		
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) {
			inputSampleL = bridgerectifier;
			overspill = temp - bridgerectifier;
		}
		
		if (inputSampleL < 0) {
			inputSampleL = -bridgerectifier;
			overspill = (-temp) - bridgerectifier;
		} 
		//drive section L
		
		//begin drive R
		temp = inputSampleR;
		variSpeed = 1.0 + ((padFactorR/lastWidthR)*satComp);
		if (variSpeed < 1.0) variSpeed = 1.0;
		totalgain = inputgain / variSpeed;
		if (totalgain != 1.0) {
			inputSampleR *= totalgain;
			if (totalgain < 1.0) {
				temp *= totalgain;
				//no boosting beyond unity please
			}
		}
		
		bridgerectifier = fabs(inputSampleR);
		overspill = 0;
		targetWidth = widestRange;
		//we now have defaults and an absolute input value to work with
		if (bridgerectifier < 0.01) padFactorR *= 0.9999;
		//in silences we bring back padFactor if it got out of hand		
		if (bridgerectifier > 1.57079633) {
			bridgerectifier = 1.57079633;
			targetWidth = 8;
		}
		//if our output's gone beyond saturating to distorting, we begin chasing the
		//buffer size smaller. Anytime we don't have that, we expand (smoothest sound, only adding to an increasingly subdivided buffer)
		
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) {
			inputSampleR = bridgerectifier;
			overspill = temp - bridgerectifier;
		}
		
		if (inputSampleR < 0) {
			inputSampleR = -bridgerectifier;
			overspill = (-temp) - bridgerectifier;
		} 
		//drive section R
		
		
		dL[dCount + 5000] = dL[dCount] = overspill * satComp;
		dR[dCount + 5000] = dR[dCount] = overspill * satComp;
		dCount--;
		//we now have a big buffer to draw from, which is always positive amount of overspill
		
		//begin pad L
		padFactorL += dL[dCount];
		double randy = (double(fpdL)/UINT32_MAX);
		if ((targetWidth*randy) > lastWidthL) {
			//we are expanding the buffer so we don't remove this trailing sample
			lastWidthL += 1;
		} else {
			padFactorL -= dL[dCount+lastWidthL];
			//zero change, or target is smaller and we are shrinking
			if (targetWidth < lastWidthL) {
				lastWidthL -= 1;
				if (lastWidthL < 2) lastWidthL = 2;
				//sanity check as randy can give us target zero
				padFactorL -= dL[dCount+lastWidthL];
			}
		}
		//variable attack/release speed more rapid as comp intensity increases
		//implemented in a way where we're repeatedly not altering the buffer as it expands, which makes the comp artifacts smoother
		if (padFactorL < 0) padFactorL = 0;
		//end pad L
		
		//begin pad R
		padFactorR += dR[dCount];
		randy = (double(fpdR)/UINT32_MAX);
		if ((targetWidth*randy) > lastWidthR) {
			//we are expanding the buffer so we don't remove this trailing sample
			lastWidthR += 1;
		} else {
			padFactorR -= dR[dCount+lastWidthR];
			//zero change, or target is smaller and we are shrinking
			if (targetWidth < lastWidthR) {
				lastWidthR -= 1;
				if (lastWidthR < 2) lastWidthR = 2;
				//sanity check as randy can give us target zero
				padFactorR -= dR[dCount+lastWidthR];
			}
		}
		//variable attack/release speed more rapid as comp intensity increases
		//implemented in a way where we're repeatedly not altering the buffer as it expands, which makes the comp artifacts smoother
		if (padFactorR < 0) padFactorR = 0;
		//end pad R
				
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		
		if (wet < 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += static_cast<int32_t>(fpdL) * 5.960464655174751e-36L * pow(2,expon+62);
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += static_cast<int32_t>(fpdR) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Compresaturator::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double inputgain = pow(10.0,((A*24.0)-12.0)/20.0);
	double satComp = B*2.0;
	int widestRange = C*C*5000;
	if (widestRange < 50) widestRange = 50;
	satComp += (((double)widestRange/3000.0)*satComp);
	//set the max wideness of comp zone, minimum range boosted (too much?)
	double output = D;
	double wet = E;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (dCount < 1 || dCount > 5000) {dCount = 5000;}
		
		//begin L
		double temp = inputSampleL;
		double variSpeed = 1.0 + ((padFactorL/lastWidthL)*satComp);
		if (variSpeed < 1.0) variSpeed = 1.0;
		double totalgain = inputgain / variSpeed;
		if (totalgain != 1.0) {
			inputSampleL *= totalgain;
			if (totalgain < 1.0) {
				temp *= totalgain;
				//no boosting beyond unity please
			}
		}
		
		double bridgerectifier = fabs(inputSampleL);
		double overspill = 0;
		int targetWidth = widestRange;
		//we now have defaults and an absolute input value to work with
		if (bridgerectifier < 0.01) padFactorL *= 0.9999;
		//in silences we bring back padFactor if it got out of hand		
		if (bridgerectifier > 1.57079633) {
			bridgerectifier = 1.57079633;
			targetWidth = 8;
		}
		//if our output's gone beyond saturating to distorting, we begin chasing the
		//buffer size smaller. Anytime we don't have that, we expand (smoothest sound, only adding to an increasingly subdivided buffer)
		
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) {
			inputSampleL = bridgerectifier;
			overspill = temp - bridgerectifier;
		}
		
		if (inputSampleL < 0) {
			inputSampleL = -bridgerectifier;
			overspill = (-temp) - bridgerectifier;
		} 
		//drive section
		
		dL[dCount + 5000] = dL[dCount] = overspill * satComp;
		//we now have a big buffer to draw from, which is always positive amount of overspill
		
		padFactorL += dL[dCount];
		double randy = (double(fpdL)/UINT32_MAX);
		if ((targetWidth*randy) > lastWidthL) {
			//we are expanding the buffer so we don't remove this trailing sample
			lastWidthL += 1;
		} else {
			padFactorL -= dL[dCount+lastWidthL];
			//zero change, or target is smaller and we are shrinking
			if (targetWidth < lastWidthL) {
				lastWidthL -= 1;
				if (lastWidthL < 2) lastWidthL = 2;
				//sanity check as randy can give us target zero
				padFactorL -= dL[dCount+lastWidthL];
			}
		}
		//variable attack/release speed more rapid as comp intensity increases
		//implemented in a way where we're repeatedly not altering the buffer as it expands, which makes the comp artifacts smoother
		if (padFactorL < 0) padFactorL = 0;
		//end L
		
		//begin R
		temp = inputSampleR;
		variSpeed = 1.0 + ((padFactorR/lastWidthR)*satComp);
		if (variSpeed < 1.0) variSpeed = 1.0;
		totalgain = inputgain / variSpeed;
		if (totalgain != 1.0) {
			inputSampleR *= totalgain;
			if (totalgain < 1.0) {
				temp *= totalgain;
				//no boosting beyond unity please
			}
		}
		
		bridgerectifier = fabs(inputSampleR);
		overspill = 0;
		targetWidth = widestRange;
		//we now have defaults and an absolute input value to work with
		if (bridgerectifier < 0.01) padFactorR *= 0.9999;
		//in silences we bring back padFactor if it got out of hand		
		if (bridgerectifier > 1.57079633) {
			bridgerectifier = 1.57079633;
			targetWidth = 8;
		}
		//if our output's gone beyond saturating to distorting, we begin chasing the
		//buffer size smaller. Anytime we don't have that, we expand (smoothest sound, only adding to an increasingly subdivided buffer)
		
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) {
			inputSampleR = bridgerectifier;
			overspill = temp - bridgerectifier;
		}
		
		if (inputSampleR < 0) {
			inputSampleR = -bridgerectifier;
			overspill = (-temp) - bridgerectifier;
		} 
		//drive section
		
		dR[dCount + 5000] = dR[dCount] = overspill * satComp;
		//we now have a big buffer to draw from, which is always positive amount of overspill
		
		padFactorR += dR[dCount];
		randy = (double(fpdR)/UINT32_MAX);
		if ((targetWidth*randy) > lastWidthR) {
			//we are expanding the buffer so we don't remove this trailing sample
			lastWidthR += 1;
		} else {
			padFactorR -= dR[dCount+lastWidthR];
			//zero change, or target is smaller and we are shrinking
			if (targetWidth < lastWidthR) {
				lastWidthR -= 1;
				if (lastWidthR < 2) lastWidthR = 2;
				//sanity check as randy can give us target zero
				padFactorR -= dR[dCount+lastWidthR];
			}
		}
		//variable attack/release speed more rapid as comp intensity increases
		//implemented in a way where we're repeatedly not altering the buffer as it expands, which makes the comp artifacts smoother
		if (padFactorR < 0) padFactorR = 0;
		//end R
		
		dCount--;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		
		if (wet < 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += static_cast<int32_t>(fpd) * 1.110223024625156e-44L * pow(2,expon+62);
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//inputSampleR += static_cast<int32_t>(fpd) * 1.110223024625156e-44L * pow(2,expon+62);
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
