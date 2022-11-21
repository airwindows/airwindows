/* ========================================
 *  Tape - Tape.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Tape_H
#include "Tape.h"
#endif

void Tape::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double inputgain = pow(10.0,((A-0.5)*24.0)/20.0);
	double bumpgain = B*0.1;
	double HeadBumpFreq = 0.12/overallscale;
	double softness = 0.618033988749894848204586;
	double RollAmount = (1.0 - softness) / overallscale;
	//[0] is frequency: 0.000001 to 0.499999 is near-zero to near-Nyquist
	//[1] is resonance, 0.7071 is Butterworth. Also can't be zero
	biquadAL[0] = biquadBL[0] = biquadAR[0] = biquadBR[0] = 0.0072/overallscale;
	biquadAL[1] = biquadBL[1] = biquadAR[1] = biquadBR[1] = 0.0009;
	double K = tan(M_PI * biquadBR[0]);
	double norm = 1.0 / (1.0 + K / biquadBR[1] + K * K);
	biquadAL[2] = biquadBL[2] = biquadAR[2] = biquadBR[2] = K / biquadBR[1] * norm;
	biquadAL[4] = biquadBL[4] = biquadAR[4] = biquadBR[4] = -biquadBR[2];
	biquadAL[5] = biquadBL[5] = biquadAR[5] = biquadBR[5] = 2.0 * (K * K - 1.0) * norm;
	biquadAL[6] = biquadBL[6] = biquadAR[6] = biquadBR[6] = (1.0 - K / biquadBR[1] + K * K) * norm;
	
	biquadCL[0] = biquadDL[0] = biquadCR[0] = biquadDR[0] = 0.032/overallscale;
	biquadCL[1] = biquadDL[1] = biquadCR[1] = biquadDR[1] = 0.0007;
	K = tan(M_PI * biquadDR[0]);
	norm = 1.0 / (1.0 + K / biquadDR[1] + K * K);
	biquadCL[2] = biquadDL[2] = biquadCR[2] = biquadDR[2] = K / biquadDR[1] * norm;
	biquadCL[4] = biquadDL[4] = biquadCR[4] = biquadDR[4] = -biquadDR[2];
	biquadCL[5] = biquadDL[5] = biquadCR[5] = biquadDR[5] = 2.0 * (K * K - 1.0) * norm;
	biquadCL[6] = biquadDL[6] = biquadCR[6] = biquadDR[6] = (1.0 - K / biquadDR[1] + K * K) * norm;
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double HighsSampleL = 0.0;
		double HighsSampleR = 0.0;
		double NonHighsSampleL = 0.0;
		double NonHighsSampleR = 0.0;
		double tempSample;
		
		if (flip)
		{
			iirMidRollerAL = (iirMidRollerAL * (1.0 - RollAmount)) + (inputSampleL * RollAmount);
			iirMidRollerAR = (iirMidRollerAR * (1.0 - RollAmount)) + (inputSampleR * RollAmount);
			HighsSampleL = inputSampleL - iirMidRollerAL;
			HighsSampleR = inputSampleR - iirMidRollerAR;
			NonHighsSampleL = iirMidRollerAL;
			NonHighsSampleR = iirMidRollerAR;
			
			iirHeadBumpAL += (inputSampleL * 0.05);
			iirHeadBumpAR += (inputSampleR * 0.05);
			iirHeadBumpAL -= (iirHeadBumpAL * iirHeadBumpAL * iirHeadBumpAL * HeadBumpFreq);
			iirHeadBumpAR -= (iirHeadBumpAR * iirHeadBumpAR * iirHeadBumpAR * HeadBumpFreq);
			iirHeadBumpAL = sin(iirHeadBumpAL);
			iirHeadBumpAR = sin(iirHeadBumpAR);
			
			tempSample = (iirHeadBumpAL * biquadAL[2]) + biquadAL[7];
			biquadAL[7] = (iirHeadBumpAL * biquadAL[3]) - (tempSample * biquadAL[5]) + biquadAL[8];
			biquadAL[8] = (iirHeadBumpAL * biquadAL[4]) - (tempSample * biquadAL[6]);
			iirHeadBumpAL = tempSample; //interleaved biquad
			if (iirHeadBumpAL > 1.0) iirHeadBumpAL = 1.0;
			if (iirHeadBumpAL < -1.0) iirHeadBumpAL = -1.0;
			iirHeadBumpAL = asin(iirHeadBumpAL);
			
			tempSample = (iirHeadBumpAR * biquadAR[2]) + biquadAR[7];
			biquadAR[7] = (iirHeadBumpAR * biquadAR[3]) - (tempSample * biquadAR[5]) + biquadAR[8];
			biquadAR[8] = (iirHeadBumpAR * biquadAR[4]) - (tempSample * biquadAR[6]);
			iirHeadBumpAR = tempSample; //interleaved biquad
			if (iirHeadBumpAR > 1.0) iirHeadBumpAR = 1.0;
			if (iirHeadBumpAR < -1.0) iirHeadBumpAR = -1.0;
			iirHeadBumpAR = asin(iirHeadBumpAR);
			
			inputSampleL = sin(inputSampleL);
			tempSample = (inputSampleL * biquadCL[2]) + biquadCL[7];
			biquadCL[7] = (inputSampleL * biquadCL[3]) - (tempSample * biquadCL[5]) + biquadCL[8];
			biquadCL[8] = (inputSampleL * biquadCL[4]) - (tempSample * biquadCL[6]);
			inputSampleL = tempSample; //interleaved biquad
			if (inputSampleL > 1.0) inputSampleL = 1.0;
			if (inputSampleL < -1.0) inputSampleL = -1.0;
			inputSampleL = asin(inputSampleL);
			
			inputSampleR = sin(inputSampleR);
			tempSample = (inputSampleR * biquadCR[2]) + biquadCR[7];
			biquadCR[7] = (inputSampleR * biquadCR[3]) - (tempSample * biquadCR[5]) + biquadCR[8];
			biquadCR[8] = (inputSampleR * biquadCR[4]) - (tempSample * biquadCR[6]);
			inputSampleR = tempSample; //interleaved biquad
			if (inputSampleR > 1.0) inputSampleR = 1.0;
			if (inputSampleR < -1.0) inputSampleR = -1.0;
			inputSampleR = asin(inputSampleR);
		} else {
			iirMidRollerBL = (iirMidRollerBL * (1.0 - RollAmount)) + (inputSampleL * RollAmount);
			iirMidRollerBR = (iirMidRollerBR * (1.0 - RollAmount)) + (inputSampleR * RollAmount);
			HighsSampleL = inputSampleL - iirMidRollerBL;
			HighsSampleR = inputSampleR - iirMidRollerBR;
			NonHighsSampleL = iirMidRollerBL;
			NonHighsSampleR = iirMidRollerBR;
			
			iirHeadBumpBL += (inputSampleL * 0.05);
			iirHeadBumpBR += (inputSampleR * 0.05);
			iirHeadBumpBL -= (iirHeadBumpBL * iirHeadBumpBL * iirHeadBumpBL * HeadBumpFreq);
			iirHeadBumpBR -= (iirHeadBumpBR * iirHeadBumpBR * iirHeadBumpBR * HeadBumpFreq);
			iirHeadBumpBL = sin(iirHeadBumpBL);
			iirHeadBumpBR = sin(iirHeadBumpBR);
			
			tempSample = (iirHeadBumpBL * biquadBL[2]) + biquadBL[7];
			biquadBL[7] = (iirHeadBumpBL * biquadBL[3]) - (tempSample * biquadBL[5]) + biquadBL[8];
			biquadBL[8] = (iirHeadBumpBL * biquadBL[4]) - (tempSample * biquadBL[6]);
			iirHeadBumpBL = tempSample; //interleaved biquad
			if (iirHeadBumpBL > 1.0) iirHeadBumpBL = 1.0;
			if (iirHeadBumpBL < -1.0) iirHeadBumpBL = -1.0;
			iirHeadBumpBL = asin(iirHeadBumpBL);
			
			tempSample = (iirHeadBumpBR * biquadBR[2]) + biquadBR[7];
			biquadBR[7] = (iirHeadBumpBR * biquadBR[3]) - (tempSample * biquadBR[5]) + biquadBR[8];
			biquadBR[8] = (iirHeadBumpBR * biquadBR[4]) - (tempSample * biquadBR[6]);
			iirHeadBumpBR = tempSample; //interleaved biquad
			if (iirHeadBumpBR > 1.0) iirHeadBumpBR = 1.0;
			if (iirHeadBumpBR < -1.0) iirHeadBumpBR = -1.0;
			iirHeadBumpBR = asin(iirHeadBumpBR);
			
			inputSampleL = sin(inputSampleL);
			tempSample = (inputSampleL * biquadDL[2]) + biquadDL[7];
			biquadDL[7] = (inputSampleL * biquadDL[3]) - (tempSample * biquadDL[5]) + biquadDL[8];
			biquadDL[8] = (inputSampleL * biquadDL[4]) - (tempSample * biquadDL[6]);
			inputSampleL = tempSample; //interleaved biquad
			if (inputSampleL > 1.0) inputSampleL = 1.0;
			if (inputSampleL < -1.0) inputSampleL = -1.0;
			inputSampleL = asin(inputSampleL);
			
			inputSampleR = sin(inputSampleR);
			tempSample = (inputSampleR * biquadDR[2]) + biquadDR[7];
			biquadDR[7] = (inputSampleR * biquadDR[3]) - (tempSample * biquadDR[5]) + biquadDR[8];
			biquadDR[8] = (inputSampleR * biquadDR[4]) - (tempSample * biquadDR[6]);
			inputSampleR = tempSample; //interleaved biquad
			if (inputSampleR > 1.0) inputSampleR = 1.0;
			if (inputSampleR < -1.0) inputSampleR = -1.0;
			inputSampleR = asin(inputSampleR);
		}
		flip = !flip;
		
		double groundSampleL = drySampleL - inputSampleL; //set up UnBox
		double groundSampleR = drySampleR - inputSampleR; //set up UnBox
		
		if (inputgain != 1.0) {
			inputSampleL *= inputgain;
			inputSampleR *= inputgain;
		} //gain boost inside UnBox: do not boost fringe audio
		
		double applySoften = fabs(HighsSampleL)*1.57079633;
		if (applySoften > 1.57079633) applySoften = 1.57079633;
		applySoften = 1-cos(applySoften);
		if (HighsSampleL > 0) inputSampleL -= applySoften;
		if (HighsSampleL < 0) inputSampleL += applySoften;
		//apply Soften depending on polarity
		applySoften = fabs(HighsSampleR)*1.57079633;
		if (applySoften > 1.57079633) applySoften = 1.57079633;
		applySoften = 1-cos(applySoften);
		if (HighsSampleR > 0) inputSampleR -= applySoften;
		if (HighsSampleR < 0) inputSampleR += applySoften;
		//apply Soften depending on polarity
		
		if (inputSampleL > 1.2533141373155) inputSampleL = 1.2533141373155;
		if (inputSampleL < -1.2533141373155) inputSampleL = -1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		inputSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		//Spiral, for cleanest most optimal tape effect
		if (inputSampleR > 1.2533141373155) inputSampleR = 1.2533141373155;
		if (inputSampleR < -1.2533141373155) inputSampleR = -1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		inputSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		//Spiral, for cleanest most optimal tape effect
		
		double suppress = (1.0-fabs(inputSampleL)) * 0.00013;
		if (iirHeadBumpAL > suppress) iirHeadBumpAL -= suppress;
		if (iirHeadBumpAL < -suppress) iirHeadBumpAL += suppress;
		if (iirHeadBumpBL > suppress) iirHeadBumpBL -= suppress;
		if (iirHeadBumpBL < -suppress) iirHeadBumpBL += suppress;
		//restrain resonant quality of head bump algorithm
		suppress = (1.0-fabs(inputSampleR)) * 0.00013;
		if (iirHeadBumpAR > suppress) iirHeadBumpAR -= suppress;
		if (iirHeadBumpAR < -suppress) iirHeadBumpAR += suppress;
		if (iirHeadBumpBR > suppress) iirHeadBumpBR -= suppress;
		if (iirHeadBumpBR < -suppress) iirHeadBumpBR += suppress;
		//restrain resonant quality of head bump algorithm
		
		inputSampleL += groundSampleL; //apply UnBox processing
		inputSampleR += groundSampleR; //apply UnBox processing
		
		inputSampleL += ((iirHeadBumpAL + iirHeadBumpBL) * bumpgain);//and head bump
		inputSampleR += ((iirHeadBumpAR + iirHeadBumpBR) * bumpgain);//and head bump
		
		if (lastSampleL >= 0.99)
		{
			if (inputSampleL < 0.99) lastSampleL = ((0.99*softness) + (inputSampleL * (1.0-softness)));
			else lastSampleL = 0.99;
		}
		
		if (lastSampleL <= -0.99)
		{
			if (inputSampleL > -0.99) lastSampleL = ((-0.99*softness) + (inputSampleL * (1.0-softness)));
			else lastSampleL = -0.99;
		}
		
		if (inputSampleL > 0.99)
		{
			if (lastSampleL < 0.99) inputSampleL = ((0.99*softness) + (lastSampleL * (1.0-softness)));
			else inputSampleL = 0.99;
		}
		
		if (inputSampleL < -0.99)
		{
			if (lastSampleL > -0.99) inputSampleL = ((-0.99*softness) + (lastSampleL * (1.0-softness)));
			else inputSampleL = -0.99;
		}
		lastSampleL = inputSampleL; //end ADClip L
		
		
		if (lastSampleR >= 0.99)
		{
			if (inputSampleR < 0.99) lastSampleR = ((0.99*softness) + (inputSampleR * (1.0-softness)));
			else lastSampleR = 0.99;
		}
		
		if (lastSampleR <= -0.99)
		{
			if (inputSampleR > -0.99) lastSampleR = ((-0.99*softness) + (inputSampleR * (1.0-softness)));
			else lastSampleR = -0.99;
		}
		
		if (inputSampleR > 0.99)
		{
			if (lastSampleR < 0.99) inputSampleR = ((0.99*softness) + (lastSampleR * (1.0-softness)));
			else inputSampleR = 0.99;
		}
		
		if (inputSampleR < -0.99)
		{
			if (lastSampleR > -0.99) inputSampleR = ((-0.99*softness) + (lastSampleR * (1.0-softness)));
			else inputSampleR = -0.99;
		}
		lastSampleR = inputSampleR; //end ADClip R		
		
		if (inputSampleL > 0.99) inputSampleL = 0.99;
		if (inputSampleL < -0.99) inputSampleL = -0.99;
		//final iron bar
		if (inputSampleR > 0.99) inputSampleR = 0.99;
		if (inputSampleR < -0.99) inputSampleR = -0.99;
		//final iron bar
				
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

void Tape::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputgain = pow(10.0,((A-0.5)*24.0)/20.0);
	double bumpgain = B*0.1;
	double HeadBumpFreq = 0.12/overallscale;
	double softness = 0.618033988749894848204586;
	double RollAmount = (1.0 - softness) / overallscale;
	//[0] is frequency: 0.000001 to 0.499999 is near-zero to near-Nyquist
	//[1] is resonance, 0.7071 is Butterworth. Also can't be zero
	biquadAL[0] = biquadBL[0] = biquadAR[0] = biquadBR[0] = 0.0072/overallscale;
	biquadAL[1] = biquadBL[1] = biquadAR[1] = biquadBR[1] = 0.0009;
	double K = tan(M_PI * biquadBR[0]);
	double norm = 1.0 / (1.0 + K / biquadBR[1] + K * K);
	biquadAL[2] = biquadBL[2] = biquadAR[2] = biquadBR[2] = K / biquadBR[1] * norm;
	biquadAL[4] = biquadBL[4] = biquadAR[4] = biquadBR[4] = -biquadBR[2];
	biquadAL[5] = biquadBL[5] = biquadAR[5] = biquadBR[5] = 2.0 * (K * K - 1.0) * norm;
	biquadAL[6] = biquadBL[6] = biquadAR[6] = biquadBR[6] = (1.0 - K / biquadBR[1] + K * K) * norm;
	
	biquadCL[0] = biquadDL[0] = biquadCR[0] = biquadDR[0] = 0.032/overallscale;
	biquadCL[1] = biquadDL[1] = biquadCR[1] = biquadDR[1] = 0.0007;
	K = tan(M_PI * biquadDR[0]);
	norm = 1.0 / (1.0 + K / biquadDR[1] + K * K);
	biquadCL[2] = biquadDL[2] = biquadCR[2] = biquadDR[2] = K / biquadDR[1] * norm;
	biquadCL[4] = biquadDL[4] = biquadCR[4] = biquadDR[4] = -biquadDR[2];
	biquadCL[5] = biquadDL[5] = biquadCR[5] = biquadDR[5] = 2.0 * (K * K - 1.0) * norm;
	biquadCL[6] = biquadDL[6] = biquadCR[6] = biquadDR[6] = (1.0 - K / biquadDR[1] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (inputgain < 1.0) {
			inputSampleL *= inputgain;
			inputSampleR *= inputgain;
		} //gain cut before anything, even dry
		
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double HighsSampleL = 0.0;
		double HighsSampleR = 0.0;
		double NonHighsSampleL = 0.0;
		double NonHighsSampleR = 0.0;
		double tempSample;
		
		if (flip)
		{
			iirMidRollerAL = (iirMidRollerAL * (1.0 - RollAmount)) + (inputSampleL * RollAmount);
			iirMidRollerAR = (iirMidRollerAR * (1.0 - RollAmount)) + (inputSampleR * RollAmount);
			HighsSampleL = inputSampleL - iirMidRollerAL;
			HighsSampleR = inputSampleR - iirMidRollerAR;
			NonHighsSampleL = iirMidRollerAL;
			NonHighsSampleR = iirMidRollerAR;
			
			iirHeadBumpAL += (inputSampleL * 0.05);
			iirHeadBumpAR += (inputSampleR * 0.05);
			iirHeadBumpAL -= (iirHeadBumpAL * iirHeadBumpAL * iirHeadBumpAL * HeadBumpFreq);
			iirHeadBumpAR -= (iirHeadBumpAR * iirHeadBumpAR * iirHeadBumpAR * HeadBumpFreq);
			iirHeadBumpAL = sin(iirHeadBumpAL);
			iirHeadBumpAR = sin(iirHeadBumpAR);
			
			tempSample = (iirHeadBumpAL * biquadAL[2]) + biquadAL[7];
			biquadAL[7] = (iirHeadBumpAL * biquadAL[3]) - (tempSample * biquadAL[5]) + biquadAL[8];
			biquadAL[8] = (iirHeadBumpAL * biquadAL[4]) - (tempSample * biquadAL[6]);
			iirHeadBumpAL = tempSample; //interleaved biquad
			if (iirHeadBumpAL > 1.0) iirHeadBumpAL = 1.0;
			if (iirHeadBumpAL < -1.0) iirHeadBumpAL = -1.0;
			iirHeadBumpAL = asin(iirHeadBumpAL);
			
			tempSample = (iirHeadBumpAR * biquadAR[2]) + biquadAR[7];
			biquadAR[7] = (iirHeadBumpAR * biquadAR[3]) - (tempSample * biquadAR[5]) + biquadAR[8];
			biquadAR[8] = (iirHeadBumpAR * biquadAR[4]) - (tempSample * biquadAR[6]);
			iirHeadBumpAR = tempSample; //interleaved biquad
			if (iirHeadBumpAR > 1.0) iirHeadBumpAR = 1.0;
			if (iirHeadBumpAR < -1.0) iirHeadBumpAR = -1.0;
			iirHeadBumpAR = asin(iirHeadBumpAR);
			
			inputSampleL = sin(inputSampleL);
			tempSample = (inputSampleL * biquadCL[2]) + biquadCL[7];
			biquadCL[7] = (inputSampleL * biquadCL[3]) - (tempSample * biquadCL[5]) + biquadCL[8];
			biquadCL[8] = (inputSampleL * biquadCL[4]) - (tempSample * biquadCL[6]);
			inputSampleL = tempSample; //interleaved biquad
			if (inputSampleL > 1.0) inputSampleL = 1.0;
			if (inputSampleL < -1.0) inputSampleL = -1.0;
			inputSampleL = asin(inputSampleL);
			
			inputSampleR = sin(inputSampleR);
			tempSample = (inputSampleR * biquadCR[2]) + biquadCR[7];
			biquadCR[7] = (inputSampleR * biquadCR[3]) - (tempSample * biquadCR[5]) + biquadCR[8];
			biquadCR[8] = (inputSampleR * biquadCR[4]) - (tempSample * biquadCR[6]);
			inputSampleR = tempSample; //interleaved biquad
			if (inputSampleR > 1.0) inputSampleR = 1.0;
			if (inputSampleR < -1.0) inputSampleR = -1.0;
			inputSampleR = asin(inputSampleR);
		} else {
			iirMidRollerBL = (iirMidRollerBL * (1.0 - RollAmount)) + (inputSampleL * RollAmount);
			iirMidRollerBR = (iirMidRollerBR * (1.0 - RollAmount)) + (inputSampleR * RollAmount);
			HighsSampleL = inputSampleL - iirMidRollerBL;
			HighsSampleR = inputSampleR - iirMidRollerBR;
			NonHighsSampleL = iirMidRollerBL;
			NonHighsSampleR = iirMidRollerBR;
			
			iirHeadBumpBL += (inputSampleL * 0.05);
			iirHeadBumpBR += (inputSampleR * 0.05);
			iirHeadBumpBL -= (iirHeadBumpBL * iirHeadBumpBL * iirHeadBumpBL * HeadBumpFreq);
			iirHeadBumpBR -= (iirHeadBumpBR * iirHeadBumpBR * iirHeadBumpBR * HeadBumpFreq);
			iirHeadBumpBL = sin(iirHeadBumpBL);
			iirHeadBumpBR = sin(iirHeadBumpBR);
			
			tempSample = (iirHeadBumpBL * biquadBL[2]) + biquadBL[7];
			biquadBL[7] = (iirHeadBumpBL * biquadBL[3]) - (tempSample * biquadBL[5]) + biquadBL[8];
			biquadBL[8] = (iirHeadBumpBL * biquadBL[4]) - (tempSample * biquadBL[6]);
			iirHeadBumpBL = tempSample; //interleaved biquad
			if (iirHeadBumpBL > 1.0) iirHeadBumpBL = 1.0;
			if (iirHeadBumpBL < -1.0) iirHeadBumpBL = -1.0;
			iirHeadBumpBL = asin(iirHeadBumpBL);
			
			tempSample = (iirHeadBumpBR * biquadBR[2]) + biquadBR[7];
			biquadBR[7] = (iirHeadBumpBR * biquadBR[3]) - (tempSample * biquadBR[5]) + biquadBR[8];
			biquadBR[8] = (iirHeadBumpBR * biquadBR[4]) - (tempSample * biquadBR[6]);
			iirHeadBumpBR = tempSample; //interleaved biquad
			if (iirHeadBumpBR > 1.0) iirHeadBumpBR = 1.0;
			if (iirHeadBumpBR < -1.0) iirHeadBumpBR = -1.0;
			iirHeadBumpBR = asin(iirHeadBumpBR);
			
			inputSampleL = sin(inputSampleL);
			tempSample = (inputSampleL * biquadDL[2]) + biquadDL[7];
			biquadDL[7] = (inputSampleL * biquadDL[3]) - (tempSample * biquadDL[5]) + biquadDL[8];
			biquadDL[8] = (inputSampleL * biquadDL[4]) - (tempSample * biquadDL[6]);
			inputSampleL = tempSample; //interleaved biquad
			if (inputSampleL > 1.0) inputSampleL = 1.0;
			if (inputSampleL < -1.0) inputSampleL = -1.0;
			inputSampleL = asin(inputSampleL);
			
			inputSampleR = sin(inputSampleR);
			tempSample = (inputSampleR * biquadDR[2]) + biquadDR[7];
			biquadDR[7] = (inputSampleR * biquadDR[3]) - (tempSample * biquadDR[5]) + biquadDR[8];
			biquadDR[8] = (inputSampleR * biquadDR[4]) - (tempSample * biquadDR[6]);
			inputSampleR = tempSample; //interleaved biquad
			if (inputSampleR > 1.0) inputSampleR = 1.0;
			if (inputSampleR < -1.0) inputSampleR = -1.0;
			inputSampleR = asin(inputSampleR);
		}
		flip = !flip;
		
		double groundSampleL = drySampleL - inputSampleL; //set up UnBox
		double groundSampleR = drySampleR - inputSampleR; //set up UnBox
		
		if (inputgain > 1.0) {
			inputSampleL *= inputgain;
			inputSampleR *= inputgain;
		} //gain boost inside UnBox: do not boost fringe audio
		
		double applySoften = fabs(HighsSampleL)*1.57079633;
		if (applySoften > 1.57079633) applySoften = 1.57079633;
		applySoften = 1-cos(applySoften);
		if (HighsSampleL > 0) inputSampleL -= applySoften;
		if (HighsSampleL < 0) inputSampleL += applySoften;
		//apply Soften depending on polarity
		applySoften = fabs(HighsSampleR)*1.57079633;
		if (applySoften > 1.57079633) applySoften = 1.57079633;
		applySoften = 1-cos(applySoften);
		if (HighsSampleR > 0) inputSampleR -= applySoften;
		if (HighsSampleR < 0) inputSampleR += applySoften;
		//apply Soften depending on polarity
		
		if (inputSampleL > 1.2533141373155) inputSampleL = 1.2533141373155;
		if (inputSampleL < -1.2533141373155) inputSampleL = -1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		inputSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		//Spiral, for cleanest most optimal tape effect
		if (inputSampleR > 1.2533141373155) inputSampleR = 1.2533141373155;
		if (inputSampleR < -1.2533141373155) inputSampleR = -1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		inputSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		//Spiral, for cleanest most optimal tape effect
		
		double suppress = (1.0-fabs(inputSampleL)) * 0.00013;
		if (iirHeadBumpAL > suppress) iirHeadBumpAL -= suppress;
		if (iirHeadBumpAL < -suppress) iirHeadBumpAL += suppress;
		if (iirHeadBumpBL > suppress) iirHeadBumpBL -= suppress;
		if (iirHeadBumpBL < -suppress) iirHeadBumpBL += suppress;
		//restrain resonant quality of head bump algorithm
		suppress = (1.0-fabs(inputSampleR)) * 0.00013;
		if (iirHeadBumpAR > suppress) iirHeadBumpAR -= suppress;
		if (iirHeadBumpAR < -suppress) iirHeadBumpAR += suppress;
		if (iirHeadBumpBR > suppress) iirHeadBumpBR -= suppress;
		if (iirHeadBumpBR < -suppress) iirHeadBumpBR += suppress;
		//restrain resonant quality of head bump algorithm
		
		inputSampleL += groundSampleL; //apply UnBox processing
		inputSampleR += groundSampleR; //apply UnBox processing
		
		inputSampleL += ((iirHeadBumpAL + iirHeadBumpBL) * bumpgain);//and head bump
		inputSampleR += ((iirHeadBumpAR + iirHeadBumpBR) * bumpgain);//and head bump
		
		if (lastSampleL >= 0.99)
		{
			if (inputSampleL < 0.99) lastSampleL = ((0.99*softness) + (inputSampleL * (1.0-softness)));
			else lastSampleL = 0.99;
		}
		
		if (lastSampleL <= -0.99)
		{
			if (inputSampleL > -0.99) lastSampleL = ((-0.99*softness) + (inputSampleL * (1.0-softness)));
			else lastSampleL = -0.99;
		}
		
		if (inputSampleL > 0.99)
		{
			if (lastSampleL < 0.99) inputSampleL = ((0.99*softness) + (lastSampleL * (1.0-softness)));
			else inputSampleL = 0.99;
		}
		
		if (inputSampleL < -0.99)
		{
			if (lastSampleL > -0.99) inputSampleL = ((-0.99*softness) + (lastSampleL * (1.0-softness)));
			else inputSampleL = -0.99;
		}
		lastSampleL = inputSampleL; //end ADClip L
		
		
		if (lastSampleR >= 0.99)
		{
			if (inputSampleR < 0.99) lastSampleR = ((0.99*softness) + (inputSampleR * (1.0-softness)));
			else lastSampleR = 0.99;
		}
		
		if (lastSampleR <= -0.99)
		{
			if (inputSampleR > -0.99) lastSampleR = ((-0.99*softness) + (inputSampleR * (1.0-softness)));
			else lastSampleR = -0.99;
		}
		
		if (inputSampleR > 0.99)
		{
			if (lastSampleR < 0.99) inputSampleR = ((0.99*softness) + (lastSampleR * (1.0-softness)));
			else inputSampleR = 0.99;
		}
		
		if (inputSampleR < -0.99)
		{
			if (lastSampleR > -0.99) inputSampleR = ((-0.99*softness) + (lastSampleR * (1.0-softness)));
			else inputSampleR = -0.99;
		}
		lastSampleR = inputSampleR; //end ADClip R		
		
		if (inputSampleL > 0.99) inputSampleL = 0.99;
		if (inputSampleL < -0.99) inputSampleL = -0.99;
		//final iron bar
		if (inputSampleR > 0.99) inputSampleR = 0.99;
		if (inputSampleR < -0.99) inputSampleR = -0.99;
		//final iron bar
		
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
