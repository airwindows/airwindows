/* ========================================
 *  IronOxideClassic2 - IronOxideClassic2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __IronOxideClassic2_H
#include "IronOxideClassic2.h"
#endif

void IronOxideClassic2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
	
	double inputgain = pow(10.0,((A*36.0)-18.0)/20.0);
	double outputgain = pow(10.0,((C*36.0)-18.0)/20.0);
	double ips = (((B*B)*(B*B)*148.5)+1.5) * 1.1;
	//slight correction to dial in convincing ips settings
	if (ips < 1 || ips > 200){ips=33.0;}
	//sanity checks are always key
	double iirAmount = ips/430.0; //for low leaning
	double fastTaper = ips/15.0;
	double slowTaper = 2.0/(ips*ips);
	
	iirAmount /= overallscale;
	fastTaper /= overallscale;
	slowTaper /= overallscale;
	//now that we have this, we must multiply it back up
	fastTaper *= cycleEnd;
	slowTaper *= cycleEnd;
	//because we're only running that part one sample in two, or three, or four
	fastTaper += 1.0;
	slowTaper += 1.0;
	
	biquadA[0] = 24000.0 / getSampleRate();
    biquadA[1] = 1.618033988749894848204586;
	biquadB[0] = 24000.0 / getSampleRate();
    biquadB[1] = 0.618033988749894848204586;
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadB[0]); //lowpass
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K * K * norm;
	biquadB[3] = 2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;	

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double tempSample;

		if (flip)
		{
			if (fabs(iirSampleAL)<1.18e-37) iirSampleAL = 0.0;
			if (fabs(iirSampleAR)<1.18e-37) iirSampleAR = 0.0;
			iirSampleAL = (iirSampleAL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			iirSampleAR = (iirSampleAR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleL -= iirSampleAL;
			inputSampleR -= iirSampleAR;
		}
		else
		{
			if (fabs(iirSampleBL)<1.18e-37) iirSampleBL = 0.0;
			if (fabs(iirSampleBR)<1.18e-37) iirSampleBR = 0.0;
			iirSampleBL = (iirSampleBL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			iirSampleBR = (iirSampleBR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleL -= iirSampleBL;
			inputSampleR -= iirSampleBR;
		}
		//do IIR highpass for leaning out
		
		if (biquadA[0] < 0.49999) {
			tempSample = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; inputSampleL = tempSample;
			biquadA[10] = biquadA[9]; biquadA[9] = inputSampleL; //DF1 left
			tempSample = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
			biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; inputSampleR = tempSample;
			biquadA[14] = biquadA[13]; biquadA[13] = inputSampleR; //DF1 right
		}		
		
		if (inputgain != 1.0) {
			inputSampleL *= inputgain;
			inputSampleR *= inputgain;
		}
		
		double bridgerectifierL = fabs(inputSampleL);
		if (bridgerectifierL > 1.57079633) bridgerectifierL = 1.57079633;
		bridgerectifierL = sin(bridgerectifierL);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifierL;
		else inputSampleL = -bridgerectifierL;
		//preliminary gain stage using antialiasing
		
		double bridgerectifierR = fabs(inputSampleR);
		if (bridgerectifierR > 1.57079633) bridgerectifierR = 1.57079633;
		bridgerectifierR = sin(bridgerectifierR);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifierR;
		else inputSampleR = -bridgerectifierR;
		//preliminary gain stage using antialiasing
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a tape sample

		//over to the Iron Oxide shaping code using inputsample
		if (gcount < 0 || gcount > 131) gcount = 131;
		int count = gcount;
		//increment the counter
			
		double temp;
		dL[count+131] = dL[count] = inputSampleL;
		dR[count+131] = dR[count] = inputSampleR;
		
		if (flip)
		{
			if (fabs(fastIIRAL)<1.18e-37) fastIIRAL = 0.0;
			if (fabs(slowIIRAL)<1.18e-37) slowIIRAL = 0.0;
			fastIIRAL = fastIIRAL/fastTaper;
			slowIIRAL = slowIIRAL/slowTaper;
			fastIIRAL += dL[count];
			//scale stuff down
			
			if (fabs(fastIIRAR)<1.18e-37) fastIIRAR = 0.0;
			if (fabs(slowIIRAR)<1.18e-37) slowIIRAR = 0.0;
			fastIIRAR = fastIIRAR/fastTaper;
			slowIIRAR = slowIIRAR/slowTaper;
			fastIIRAR += dR[count];
			//scale stuff down
			count += 3;
			
			temp = dL[count+127];
			temp += dL[count+113];
			temp += dL[count+109];
			temp += dL[count+107];
			temp += dL[count+103];
			temp += dL[count+101];
			temp += dL[count+97];
			temp += dL[count+89];
			temp += dL[count+83];
			temp /= 2;
			temp += dL[count+79];
			temp += dL[count+73];
			temp += dL[count+71];
			temp += dL[count+67];
			temp += dL[count+61];
			temp += dL[count+59];
			temp += dL[count+53];
			temp += dL[count+47];
			temp += dL[count+43];
			temp += dL[count+41];
			temp += dL[count+37];
			temp += dL[count+31];
			temp += dL[count+29];
			temp /= 2;
			temp += dL[count+23];
			temp += dL[count+19];
			temp += dL[count+17];
			temp += dL[count+13];
			temp += dL[count+11];
			temp /= 2;
			temp += dL[count+7];
			temp += dL[count+5];
			temp += dL[count+3];
			temp /= 2;
			temp += dL[count+2];
			temp += dL[count+1]; //end L
			slowIIRAL += (temp/128);
			
			temp = dR[count+127];
			temp += dR[count+113];
			temp += dR[count+109];
			temp += dR[count+107];
			temp += dR[count+103];
			temp += dR[count+101];
			temp += dR[count+97];
			temp += dR[count+89];
			temp += dR[count+83];
			temp /= 2;
			temp += dR[count+79];
			temp += dR[count+73];
			temp += dR[count+71];
			temp += dR[count+67];
			temp += dR[count+61];
			temp += dR[count+59];
			temp += dR[count+53];
			temp += dR[count+47];
			temp += dR[count+43];
			temp += dR[count+41];
			temp += dR[count+37];
			temp += dR[count+31];
			temp += dR[count+29];
			temp /= 2;
			temp += dR[count+23];
			temp += dR[count+19];
			temp += dR[count+17];
			temp += dR[count+13];
			temp += dR[count+11];
			temp /= 2;
			temp += dR[count+7];
			temp += dR[count+5];
			temp += dR[count+3];
			temp /= 2;
			temp += dR[count+2];
			temp += dR[count+1]; //end R
			slowIIRAR += (temp/128);
			
			inputSampleL = fastIIRAL - (slowIIRAL / slowTaper);
			inputSampleR = fastIIRAR - (slowIIRAR / slowTaper);
		}
		else
		{
			if (fabs(fastIIRBL)<1.18e-37) fastIIRBL = 0.0;
			if (fabs(slowIIRBL)<1.18e-37) slowIIRBL = 0.0;
			fastIIRBL = fastIIRBL/fastTaper;
			slowIIRBL = slowIIRBL/slowTaper;
			fastIIRBL += dL[count];
			//scale stuff down
			
			if (fabs(fastIIRBR)<1.18e-37) fastIIRBR = 0.0;
			if (fabs(slowIIRBR)<1.18e-37) slowIIRBR = 0.0;
			fastIIRBR = fastIIRBR/fastTaper;
			slowIIRBR = slowIIRBR/slowTaper;
			fastIIRBR += dR[count];
			//scale stuff down
			count += 3;
			
			temp = dL[count+127];
			temp += dL[count+113];
			temp += dL[count+109];
			temp += dL[count+107];
			temp += dL[count+103];
			temp += dL[count+101];
			temp += dL[count+97];
			temp += dL[count+89];
			temp += dL[count+83];
			temp /= 2;
			temp += dL[count+79];
			temp += dL[count+73];
			temp += dL[count+71];
			temp += dL[count+67];
			temp += dL[count+61];
			temp += dL[count+59];
			temp += dL[count+53];
			temp += dL[count+47];
			temp += dL[count+43];
			temp += dL[count+41];
			temp += dL[count+37];
			temp += dL[count+31];
			temp += dL[count+29];
			temp /= 2;
			temp += dL[count+23];
			temp += dL[count+19];
			temp += dL[count+17];
			temp += dL[count+13];
			temp += dL[count+11];
			temp /= 2;
			temp += dL[count+7];
			temp += dL[count+5];
			temp += dL[count+3];
			temp /= 2;
			temp += dL[count+2];
			temp += dL[count+1];
			slowIIRBL += (temp/128);
			
			temp = dR[count+127];
			temp += dR[count+113];
			temp += dR[count+109];
			temp += dR[count+107];
			temp += dR[count+103];
			temp += dR[count+101];
			temp += dR[count+97];
			temp += dR[count+89];
			temp += dR[count+83];
			temp /= 2;
			temp += dR[count+79];
			temp += dR[count+73];
			temp += dR[count+71];
			temp += dR[count+67];
			temp += dR[count+61];
			temp += dR[count+59];
			temp += dR[count+53];
			temp += dR[count+47];
			temp += dR[count+43];
			temp += dR[count+41];
			temp += dR[count+37];
			temp += dR[count+31];
			temp += dR[count+29];
			temp /= 2;
			temp += dR[count+23];
			temp += dR[count+19];
			temp += dR[count+17];
			temp += dR[count+13];
			temp += dR[count+11];
			temp /= 2;
			temp += dR[count+7];
			temp += dR[count+5];
			temp += dR[count+3];
			temp /= 2;
			temp += dR[count+2];
			temp += dR[count+1];
			slowIIRBR += (temp/128);
			
			inputSampleL = fastIIRBL - (slowIIRBL / slowTaper);
			inputSampleR = fastIIRBR - (slowIIRBR / slowTaper);
		}
		
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
		
		bridgerectifierL = fabs(inputSampleL);
		if (bridgerectifierL > 1.57079633) bridgerectifierL = 1.57079633;
		bridgerectifierL = sin(bridgerectifierL);
		//can use as an output limiter
		if (inputSampleL > 0.0) inputSampleL = bridgerectifierL;
		else inputSampleL = -bridgerectifierL;
		//second stage of overdrive to prevent overs and allow bloody loud extremeness		
		
		bridgerectifierR = fabs(inputSampleR);
		if (bridgerectifierR > 1.57079633) bridgerectifierR = 1.57079633;
		bridgerectifierR = sin(bridgerectifierR);
		//can use as an output limiter
		if (inputSampleR > 0.0) inputSampleR = bridgerectifierR;
		else inputSampleR = -bridgerectifierR;
		//second stage of overdrive to prevent overs and allow bloody loud extremeness
		
		if (biquadB[0] < 0.49999) {
			tempSample = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; inputSampleL = tempSample;
			biquadB[10] = biquadB[9]; biquadB[9] = inputSampleL; //DF1 left
			tempSample = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
			biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; inputSampleR = tempSample;
			biquadB[14] = biquadB[13]; biquadB[13] = inputSampleR; //DF1 right
		}		
		
		if (outputgain != 1.0) {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
		}
		flip = !flip;
		
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

void IronOxideClassic2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
	
	double inputgain = pow(10.0,((A*36.0)-18.0)/20.0);
	double outputgain = pow(10.0,((C*36.0)-18.0)/20.0);
	double ips = (((B*B)*(B*B)*148.5)+1.5) * 1.1;
	//slight correction to dial in convincing ips settings
	if (ips < 1 || ips > 200){ips=33.0;}
	//sanity checks are always key
	double iirAmount = ips/430.0; //for low leaning
	double fastTaper = ips/15.0;
	double slowTaper = 2.0/(ips*ips);
	
	iirAmount /= overallscale;
	fastTaper /= overallscale;
	slowTaper /= overallscale;
	//now that we have this, we must multiply it back up
	fastTaper *= cycleEnd;
	slowTaper *= cycleEnd;
	//because we're only running that part one sample in two, or three, or four
	fastTaper += 1.0;
	slowTaper += 1.0;
	
	biquadA[0] = 24000.0 / getSampleRate();
    biquadA[1] = 1.618033988749894848204586;
	biquadB[0] = 24000.0 / getSampleRate();
    biquadB[1] = 0.618033988749894848204586;
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadB[0]); //lowpass
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K * K * norm;
	biquadB[3] = 2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;	
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double tempSample;
		
		if (flip)
		{
			if (fabs(iirSampleAL)<1.18e-37) iirSampleAL = 0.0;
			if (fabs(iirSampleAR)<1.18e-37) iirSampleAR = 0.0;
			iirSampleAL = (iirSampleAL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			iirSampleAR = (iirSampleAR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleL -= iirSampleAL;
			inputSampleR -= iirSampleAR;
		}
		else
		{
			if (fabs(iirSampleBL)<1.18e-37) iirSampleBL = 0.0;
			if (fabs(iirSampleBR)<1.18e-37) iirSampleBR = 0.0;
			iirSampleBL = (iirSampleBL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			iirSampleBR = (iirSampleBR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleL -= iirSampleBL;
			inputSampleR -= iirSampleBR;
		}
		//do IIR highpass for leaning out
		
		if (biquadA[0] < 0.49999) {
			tempSample = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; inputSampleL = tempSample;
			biquadA[10] = biquadA[9]; biquadA[9] = inputSampleL; //DF1 left
			tempSample = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
			biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; inputSampleR = tempSample;
			biquadA[14] = biquadA[13]; biquadA[13] = inputSampleR; //DF1 right
		}		
		
		if (inputgain != 1.0) {
			inputSampleL *= inputgain;
			inputSampleR *= inputgain;
		}
		
		double bridgerectifierL = fabs(inputSampleL);
		if (bridgerectifierL > 1.57079633) bridgerectifierL = 1.57079633;
		bridgerectifierL = sin(bridgerectifierL);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifierL;
		else inputSampleL = -bridgerectifierL;
		//preliminary gain stage using antialiasing
		
		double bridgerectifierR = fabs(inputSampleR);
		if (bridgerectifierR > 1.57079633) bridgerectifierR = 1.57079633;
		bridgerectifierR = sin(bridgerectifierR);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifierR;
		else inputSampleR = -bridgerectifierR;
		//preliminary gain stage using antialiasing
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a tape sample
			
			//over to the Iron Oxide shaping code using inputsample
			if (gcount < 0 || gcount > 131) gcount = 131;
			int count = gcount;
			//increment the counter
			
			double temp;
			dL[count+131] = dL[count] = inputSampleL;
			dR[count+131] = dR[count] = inputSampleR;
			
			if (flip)
			{
				if (fabs(fastIIRAL)<1.18e-37) fastIIRAL = 0.0;
				if (fabs(slowIIRAL)<1.18e-37) slowIIRAL = 0.0;
				fastIIRAL = fastIIRAL/fastTaper;
				slowIIRAL = slowIIRAL/slowTaper;
				fastIIRAL += dL[count];
				//scale stuff down
				
				if (fabs(fastIIRAR)<1.18e-37) fastIIRAR = 0.0;
				if (fabs(slowIIRAR)<1.18e-37) slowIIRAR = 0.0;
				fastIIRAR = fastIIRAR/fastTaper;
				slowIIRAR = slowIIRAR/slowTaper;
				fastIIRAR += dR[count];
				//scale stuff down
				count += 3;
				
				temp = dL[count+127];
				temp += dL[count+113];
				temp += dL[count+109];
				temp += dL[count+107];
				temp += dL[count+103];
				temp += dL[count+101];
				temp += dL[count+97];
				temp += dL[count+89];
				temp += dL[count+83];
				temp /= 2;
				temp += dL[count+79];
				temp += dL[count+73];
				temp += dL[count+71];
				temp += dL[count+67];
				temp += dL[count+61];
				temp += dL[count+59];
				temp += dL[count+53];
				temp += dL[count+47];
				temp += dL[count+43];
				temp += dL[count+41];
				temp += dL[count+37];
				temp += dL[count+31];
				temp += dL[count+29];
				temp /= 2;
				temp += dL[count+23];
				temp += dL[count+19];
				temp += dL[count+17];
				temp += dL[count+13];
				temp += dL[count+11];
				temp /= 2;
				temp += dL[count+7];
				temp += dL[count+5];
				temp += dL[count+3];
				temp /= 2;
				temp += dL[count+2];
				temp += dL[count+1]; //end L
				slowIIRAL += (temp/128);
				
				temp = dR[count+127];
				temp += dR[count+113];
				temp += dR[count+109];
				temp += dR[count+107];
				temp += dR[count+103];
				temp += dR[count+101];
				temp += dR[count+97];
				temp += dR[count+89];
				temp += dR[count+83];
				temp /= 2;
				temp += dR[count+79];
				temp += dR[count+73];
				temp += dR[count+71];
				temp += dR[count+67];
				temp += dR[count+61];
				temp += dR[count+59];
				temp += dR[count+53];
				temp += dR[count+47];
				temp += dR[count+43];
				temp += dR[count+41];
				temp += dR[count+37];
				temp += dR[count+31];
				temp += dR[count+29];
				temp /= 2;
				temp += dR[count+23];
				temp += dR[count+19];
				temp += dR[count+17];
				temp += dR[count+13];
				temp += dR[count+11];
				temp /= 2;
				temp += dR[count+7];
				temp += dR[count+5];
				temp += dR[count+3];
				temp /= 2;
				temp += dR[count+2];
				temp += dR[count+1]; //end R
				slowIIRAR += (temp/128);
				
				inputSampleL = fastIIRAL - (slowIIRAL / slowTaper);
				inputSampleR = fastIIRAR - (slowIIRAR / slowTaper);
			}
			else
			{
				if (fabs(fastIIRBL)<1.18e-37) fastIIRBL = 0.0;
				if (fabs(slowIIRBL)<1.18e-37) slowIIRBL = 0.0;
				fastIIRBL = fastIIRBL/fastTaper;
				slowIIRBL = slowIIRBL/slowTaper;
				fastIIRBL += dL[count];
				//scale stuff down
				
				if (fabs(fastIIRBR)<1.18e-37) fastIIRBR = 0.0;
				if (fabs(slowIIRBR)<1.18e-37) slowIIRBR = 0.0;
				fastIIRBR = fastIIRBR/fastTaper;
				slowIIRBR = slowIIRBR/slowTaper;
				fastIIRBR += dR[count];
				//scale stuff down
				count += 3;
				
				temp = dL[count+127];
				temp += dL[count+113];
				temp += dL[count+109];
				temp += dL[count+107];
				temp += dL[count+103];
				temp += dL[count+101];
				temp += dL[count+97];
				temp += dL[count+89];
				temp += dL[count+83];
				temp /= 2;
				temp += dL[count+79];
				temp += dL[count+73];
				temp += dL[count+71];
				temp += dL[count+67];
				temp += dL[count+61];
				temp += dL[count+59];
				temp += dL[count+53];
				temp += dL[count+47];
				temp += dL[count+43];
				temp += dL[count+41];
				temp += dL[count+37];
				temp += dL[count+31];
				temp += dL[count+29];
				temp /= 2;
				temp += dL[count+23];
				temp += dL[count+19];
				temp += dL[count+17];
				temp += dL[count+13];
				temp += dL[count+11];
				temp /= 2;
				temp += dL[count+7];
				temp += dL[count+5];
				temp += dL[count+3];
				temp /= 2;
				temp += dL[count+2];
				temp += dL[count+1];
				slowIIRBL += (temp/128);
				
				temp = dR[count+127];
				temp += dR[count+113];
				temp += dR[count+109];
				temp += dR[count+107];
				temp += dR[count+103];
				temp += dR[count+101];
				temp += dR[count+97];
				temp += dR[count+89];
				temp += dR[count+83];
				temp /= 2;
				temp += dR[count+79];
				temp += dR[count+73];
				temp += dR[count+71];
				temp += dR[count+67];
				temp += dR[count+61];
				temp += dR[count+59];
				temp += dR[count+53];
				temp += dR[count+47];
				temp += dR[count+43];
				temp += dR[count+41];
				temp += dR[count+37];
				temp += dR[count+31];
				temp += dR[count+29];
				temp /= 2;
				temp += dR[count+23];
				temp += dR[count+19];
				temp += dR[count+17];
				temp += dR[count+13];
				temp += dR[count+11];
				temp /= 2;
				temp += dR[count+7];
				temp += dR[count+5];
				temp += dR[count+3];
				temp /= 2;
				temp += dR[count+2];
				temp += dR[count+1];
				slowIIRBR += (temp/128);
				
				inputSampleL = fastIIRBL - (slowIIRBL / slowTaper);
				inputSampleR = fastIIRBR - (slowIIRBR / slowTaper);
			}
			
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
		
		bridgerectifierL = fabs(inputSampleL);
		if (bridgerectifierL > 1.57079633) bridgerectifierL = 1.57079633;
		bridgerectifierL = sin(bridgerectifierL);
		//can use as an output limiter
		if (inputSampleL > 0.0) inputSampleL = bridgerectifierL;
		else inputSampleL = -bridgerectifierL;
		//second stage of overdrive to prevent overs and allow bloody loud extremeness		
		
		bridgerectifierR = fabs(inputSampleR);
		if (bridgerectifierR > 1.57079633) bridgerectifierR = 1.57079633;
		bridgerectifierR = sin(bridgerectifierR);
		//can use as an output limiter
		if (inputSampleR > 0.0) inputSampleR = bridgerectifierR;
		else inputSampleR = -bridgerectifierR;
		//second stage of overdrive to prevent overs and allow bloody loud extremeness
		
		if (biquadB[0] < 0.49999) {
			tempSample = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; inputSampleL = tempSample;
			biquadB[10] = biquadB[9]; biquadB[9] = inputSampleL; //DF1 left
			tempSample = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
			biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; inputSampleR = tempSample;
			biquadB[14] = biquadB[13]; biquadB[13] = inputSampleR; //DF1 right
		}		
		
		if (outputgain != 1.0) {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
		}
		flip = !flip;
		
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
