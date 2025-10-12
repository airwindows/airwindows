/* ========================================
 *  FatEQ - FatEQ.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __FatEQ_H
#include "FatEQ.h"
#endif

void FatEQ::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	double topLevl = A*2.0; if (topLevl < 1.0) topLevl = sqrt(topLevl);
	pearA[levl] = B*2.0; if (pearA[levl] < 1.0) pearA[levl] = sqrt(pearA[levl]);
	pearB[levl] = C*2.0; if (pearB[levl] < 1.0) pearB[levl] = sqrt(pearB[levl]);
	pearC[levl] = D*2.0; if (pearC[levl] < 1.0) pearC[levl] = sqrt(pearC[levl]);
	double out = E;
	
	double freqFactor = sqrt(overallscale) + (overallscale*0.5);
	pearA[freq] = pow(0.564,freqFactor+0.85);
	pearB[freq] = pow(0.564,freqFactor+4.1);
	pearC[freq] = pow(0.564,freqFactor+7.1);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int x = 0; x < pear_max; x += 4) {
			//begin Pear filter stages
			pearA[figL] = inputSampleL; pearA[figR] = inputSampleR;
			pearA[slew] = ((pearA[figL]-pearA[x])+pearA[x+1])*pearA[freq]*0.5;
			pearA[x] = pearA[figL] = (pearA[freq]*pearA[figL])+((1.0-pearA[freq])*(pearA[x]+pearA[x+1]));
			pearA[x+1] = pearA[slew];
			pearA[slew] = ((pearA[figR]-pearA[x+2])+pearA[x+3])*pearA[freq]*0.5;
			pearA[x+2] = pearA[figR] = (pearA[freq]*pearA[figR])+((1.0-pearA[freq])*(pearA[x+2]+pearA[x+3]));
			pearA[x+3] = pearA[slew];
			inputSampleL -= pearA[figL]; inputSampleR -= pearA[figR];
			
			pearB[figL] = pearA[figL]; pearB[figR] = pearA[figR];
			pearB[slew] = ((pearB[figL]-pearB[x])+pearB[x+1])*pearB[freq]*0.5;
			pearB[x] = pearB[figL] = (pearB[freq]*pearA[figL])+((1.0-pearB[freq])*(pearB[x]+pearB[x+1]));
			pearB[x+1] = pearB[slew];
			pearB[slew] = ((pearB[figR]-pearB[x+2])+pearB[x+3])*pearB[freq]*0.5;
			pearB[x+2] = pearB[figR] = (pearB[freq]*pearA[figR])+((1.0-pearB[freq])*(pearB[x+2]+pearB[x+3]));
			pearB[x+3] = pearB[slew];
			pearA[figL] -= pearB[figL]; pearA[figR] -= pearB[figR];
			
			pearC[figL] = pearB[figL]; pearC[figR] = pearB[figR];
			pearC[slew] = ((pearC[figL]-pearC[x])+pearC[x+1])*pearC[freq]*0.5;
			pearC[x] = pearC[figL] = (pearC[freq]*pearB[figL])+((1.0-pearC[freq])*(pearC[x]+pearC[x+1]));
			pearC[x+1] = pearC[slew];
			pearC[slew] = ((pearC[figR]-pearC[x+2])+pearC[x+3])*pearC[freq]*0.5;
			pearC[x+2] = pearC[figR] = (pearC[freq]*pearB[figR])+((1.0-pearC[freq])*(pearC[x+2]+pearC[x+3]));
			pearC[x+3] = pearC[slew];
			pearB[figL] -= pearC[figL]; pearB[figR] -= pearC[figR];
			
			//begin FatEQL
			double altered = inputSampleL;
			if (topLevl > 1.0) {
				altered = fmax(fmin(inputSampleL*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (topLevl < 1.0) {
				altered = fmax(fmin(inputSampleL,1.0),-1.0);
				double polarity = altered;
				double X = inputSampleL * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL = (inputSampleL*(1.0-fabs(topLevl-1.0)))+(altered*fabs(topLevl-1.0));
			
			altered = pearA[figL];
			if (pearA[levl] > 1.0) {
				altered = fmax(fmin(pearA[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearA[levl] < 1.0) {
				altered = fmax(fmin(pearA[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearA[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearA[figL]*(1.0-fabs(pearA[levl]-1.0)))+(altered*fabs(pearA[levl]-1.0));
			
			altered = pearB[figL];
			if (pearB[levl] > 1.0) {
				altered = fmax(fmin(pearB[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearB[levl] < 1.0) {
				altered = fmax(fmin(pearB[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearB[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearB[figL]*(1.0-fabs(pearB[levl]-1.0)))+(altered*fabs(pearB[levl]-1.0));
			
			altered = pearC[figL];
			if (pearC[levl] > 1.0) {
				altered = fmax(fmin(pearC[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearC[levl] < 1.0) {
				altered = fmax(fmin(pearC[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearC[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearC[figL]*(1.0-fabs(pearC[levl]-1.0)))+(altered*fabs(pearC[levl]-1.0));
			//end FatEQ L
			
			//begin FatEQ R
			altered = inputSampleR;
			if (topLevl > 1.0) {
				altered = fmax(fmin(inputSampleR*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (topLevl < 1.0) {
				altered = fmax(fmin(inputSampleR,1.0),-1.0);
				double polarity = altered;
				double X = inputSampleR * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleR = (inputSampleR*(1.0-fabs(topLevl-1.0)))+(altered*fabs(topLevl-1.0));
			
			altered = pearA[figR];
			if (pearA[levl] > 1.0) {
				altered = fmax(fmin(pearA[figR]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearA[levl] < 1.0) {
				altered = fmax(fmin(pearA[figR],1.0),-1.0);
				double polarity = altered;
				double X = pearA[figR] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleR += (pearA[figR]*(1.0-fabs(pearA[levl]-1.0)))+(altered*fabs(pearA[levl]-1.0));
			
			altered = pearB[figR];
			if (pearB[levl] > 1.0) {
				altered = fmax(fmin(pearB[figR]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearB[levl] < 1.0) {
				altered = fmax(fmin(pearB[figR],1.0),-1.0);
				double polarity = altered;
				double X = pearB[figR] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleR += (pearB[figR]*(1.0-fabs(pearB[levl]-1.0)))+(altered*fabs(pearB[levl]-1.0));
			
			altered = pearC[figR];
			if (pearC[levl] > 1.0) {
				altered = fmax(fmin(pearC[figR]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearC[levl] < 1.0) {
				altered = fmax(fmin(pearC[figR],1.0),-1.0);
				double polarity = altered;
				double X = pearC[figR] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleR += (pearC[figR]*(1.0-fabs(pearC[levl]-1.0)))+(altered*fabs(pearC[levl]-1.0));
			//end FatEQ R
		}
		
		inputSampleL *= out;
		inputSampleR *= out;

		//begin ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=0.7058208+(inputSampleL*0.2609148);
			else lastSampleL = 0.2491717+(lastSampleL*0.7390851);
		} wasPosClipL = false;
		if (inputSampleL>0.9549925859) {wasPosClipL=true;inputSampleL=0.7058208+(lastSampleL*0.2609148);}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=-0.7058208+(inputSampleL*0.2609148);
			else lastSampleL=-0.2491717+(lastSampleL*0.7390851);
		} wasNegClipL = false;
		if (inputSampleL<-0.9549925859) {wasNegClipL=true;inputSampleL=-0.7058208+(lastSampleL*0.2609148);}
		intermediateL[spacing] = inputSampleL;
        inputSampleL = lastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateL[x-1] = intermediateL[x];
		lastSampleL = intermediateL[0]; //run a little buffer to handle this
		
		if (inputSampleR > 4.0) inputSampleR = 4.0; if (inputSampleR < -4.0) inputSampleR = -4.0;
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=0.7058208+(inputSampleR*0.2609148);
			else lastSampleR = 0.2491717+(lastSampleR*0.7390851);
		} wasPosClipR = false;
		if (inputSampleR>0.9549925859) {wasPosClipR=true;inputSampleR=0.7058208+(lastSampleR*0.2609148);}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=-0.7058208+(inputSampleR*0.2609148);
			else lastSampleR=-0.2491717+(lastSampleR*0.7390851);
		} wasNegClipR = false;
		if (inputSampleR<-0.9549925859) {wasNegClipR=true;inputSampleR=-0.7058208+(lastSampleR*0.2609148);}
		intermediateR[spacing] = inputSampleR;
        inputSampleR = lastSampleR; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateR[x-1] = intermediateR[x];
		lastSampleR = intermediateR[0]; //run a little buffer to handle this
		//end ClipOnly2 stereo as a little, compressed chunk that can be dropped into code		
		
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

void FatEQ::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	double topLevl = A*2.0; if (topLevl < 1.0) topLevl = sqrt(topLevl);
	pearA[levl] = B*2.0; if (pearA[levl] < 1.0) pearA[levl] = sqrt(pearA[levl]);
	pearB[levl] = C*2.0; if (pearB[levl] < 1.0) pearB[levl] = sqrt(pearB[levl]);
	pearC[levl] = D*2.0; if (pearC[levl] < 1.0) pearC[levl] = sqrt(pearC[levl]);
	double out = E;
	
	double freqFactor = sqrt(overallscale) + (overallscale*0.5);
	pearA[freq] = pow(0.564,freqFactor+0.85);
	pearB[freq] = pow(0.564,freqFactor+4.1);
	pearC[freq] = pow(0.564,freqFactor+7.1);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int x = 0; x < pear_max; x += 4) {
			//begin Pear filter stages
			pearA[figL] = inputSampleL; pearA[figR] = inputSampleR;
			pearA[slew] = ((pearA[figL]-pearA[x])+pearA[x+1])*pearA[freq]*0.5;
			pearA[x] = pearA[figL] = (pearA[freq]*pearA[figL])+((1.0-pearA[freq])*(pearA[x]+pearA[x+1]));
			pearA[x+1] = pearA[slew];
			pearA[slew] = ((pearA[figR]-pearA[x+2])+pearA[x+3])*pearA[freq]*0.5;
			pearA[x+2] = pearA[figR] = (pearA[freq]*pearA[figR])+((1.0-pearA[freq])*(pearA[x+2]+pearA[x+3]));
			pearA[x+3] = pearA[slew];
			inputSampleL -= pearA[figL]; inputSampleR -= pearA[figR];
			
			pearB[figL] = pearA[figL]; pearB[figR] = pearA[figR];
			pearB[slew] = ((pearB[figL]-pearB[x])+pearB[x+1])*pearB[freq]*0.5;
			pearB[x] = pearB[figL] = (pearB[freq]*pearA[figL])+((1.0-pearB[freq])*(pearB[x]+pearB[x+1]));
			pearB[x+1] = pearB[slew];
			pearB[slew] = ((pearB[figR]-pearB[x+2])+pearB[x+3])*pearB[freq]*0.5;
			pearB[x+2] = pearB[figR] = (pearB[freq]*pearA[figR])+((1.0-pearB[freq])*(pearB[x+2]+pearB[x+3]));
			pearB[x+3] = pearB[slew];
			pearA[figL] -= pearB[figL]; pearA[figR] -= pearB[figR];
			
			pearC[figL] = pearB[figL]; pearC[figR] = pearB[figR];
			pearC[slew] = ((pearC[figL]-pearC[x])+pearC[x+1])*pearC[freq]*0.5;
			pearC[x] = pearC[figL] = (pearC[freq]*pearB[figL])+((1.0-pearC[freq])*(pearC[x]+pearC[x+1]));
			pearC[x+1] = pearC[slew];
			pearC[slew] = ((pearC[figR]-pearC[x+2])+pearC[x+3])*pearC[freq]*0.5;
			pearC[x+2] = pearC[figR] = (pearC[freq]*pearB[figR])+((1.0-pearC[freq])*(pearC[x+2]+pearC[x+3]));
			pearC[x+3] = pearC[slew];
			pearB[figL] -= pearC[figL]; pearB[figR] -= pearC[figR];
			
			//begin FatEQL
			double altered = inputSampleL;
			if (topLevl > 1.0) {
				altered = fmax(fmin(inputSampleL*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (topLevl < 1.0) {
				altered = fmax(fmin(inputSampleL,1.0),-1.0);
				double polarity = altered;
				double X = inputSampleL * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL = (inputSampleL*(1.0-fabs(topLevl-1.0)))+(altered*fabs(topLevl-1.0));
			
			altered = pearA[figL];
			if (pearA[levl] > 1.0) {
				altered = fmax(fmin(pearA[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearA[levl] < 1.0) {
				altered = fmax(fmin(pearA[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearA[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearA[figL]*(1.0-fabs(pearA[levl]-1.0)))+(altered*fabs(pearA[levl]-1.0));
			
			altered = pearB[figL];
			if (pearB[levl] > 1.0) {
				altered = fmax(fmin(pearB[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearB[levl] < 1.0) {
				altered = fmax(fmin(pearB[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearB[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearB[figL]*(1.0-fabs(pearB[levl]-1.0)))+(altered*fabs(pearB[levl]-1.0));
			
			altered = pearC[figL];
			if (pearC[levl] > 1.0) {
				altered = fmax(fmin(pearC[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearC[levl] < 1.0) {
				altered = fmax(fmin(pearC[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearC[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearC[figL]*(1.0-fabs(pearC[levl]-1.0)))+(altered*fabs(pearC[levl]-1.0));
			//end FatEQ L
			
			//begin FatEQ R
			altered = inputSampleR;
			if (topLevl > 1.0) {
				altered = fmax(fmin(inputSampleR*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (topLevl < 1.0) {
				altered = fmax(fmin(inputSampleR,1.0),-1.0);
				double polarity = altered;
				double X = inputSampleR * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleR = (inputSampleR*(1.0-fabs(topLevl-1.0)))+(altered*fabs(topLevl-1.0));
			
			altered = pearA[figR];
			if (pearA[levl] > 1.0) {
				altered = fmax(fmin(pearA[figR]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearA[levl] < 1.0) {
				altered = fmax(fmin(pearA[figR],1.0),-1.0);
				double polarity = altered;
				double X = pearA[figR] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleR += (pearA[figR]*(1.0-fabs(pearA[levl]-1.0)))+(altered*fabs(pearA[levl]-1.0));
			
			altered = pearB[figR];
			if (pearB[levl] > 1.0) {
				altered = fmax(fmin(pearB[figR]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearB[levl] < 1.0) {
				altered = fmax(fmin(pearB[figR],1.0),-1.0);
				double polarity = altered;
				double X = pearB[figR] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleR += (pearB[figR]*(1.0-fabs(pearB[levl]-1.0)))+(altered*fabs(pearB[levl]-1.0));
			
			altered = pearC[figR];
			if (pearC[levl] > 1.0) {
				altered = fmax(fmin(pearC[figR]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearC[levl] < 1.0) {
				altered = fmax(fmin(pearC[figR],1.0),-1.0);
				double polarity = altered;
				double X = pearC[figR] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleR += (pearC[figR]*(1.0-fabs(pearC[levl]-1.0)))+(altered*fabs(pearC[levl]-1.0));
			//end FatEQ R
		}
		
		inputSampleL *= out;
		inputSampleR *= out;
		
		//begin ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=0.7058208+(inputSampleL*0.2609148);
			else lastSampleL = 0.2491717+(lastSampleL*0.7390851);
		} wasPosClipL = false;
		if (inputSampleL>0.9549925859) {wasPosClipL=true;inputSampleL=0.7058208+(lastSampleL*0.2609148);}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=-0.7058208+(inputSampleL*0.2609148);
			else lastSampleL=-0.2491717+(lastSampleL*0.7390851);
		} wasNegClipL = false;
		if (inputSampleL<-0.9549925859) {wasNegClipL=true;inputSampleL=-0.7058208+(lastSampleL*0.2609148);}
		intermediateL[spacing] = inputSampleL;
        inputSampleL = lastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateL[x-1] = intermediateL[x];
		lastSampleL = intermediateL[0]; //run a little buffer to handle this
		
		if (inputSampleR > 4.0) inputSampleR = 4.0; if (inputSampleR < -4.0) inputSampleR = -4.0;
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=0.7058208+(inputSampleR*0.2609148);
			else lastSampleR = 0.2491717+(lastSampleR*0.7390851);
		} wasPosClipR = false;
		if (inputSampleR>0.9549925859) {wasPosClipR=true;inputSampleR=0.7058208+(lastSampleR*0.2609148);}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=-0.7058208+(inputSampleR*0.2609148);
			else lastSampleR=-0.2491717+(lastSampleR*0.7390851);
		} wasNegClipR = false;
		if (inputSampleR<-0.9549925859) {wasNegClipR=true;inputSampleR=-0.7058208+(lastSampleR*0.2609148);}
		intermediateR[spacing] = inputSampleR;
        inputSampleR = lastSampleR; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateR[x-1] = intermediateR[x];
		lastSampleR = intermediateR[0]; //run a little buffer to handle this
		//end ClipOnly2 stereo as a little, compressed chunk that can be dropped into code		
		
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
