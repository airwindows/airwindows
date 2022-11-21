/* ========================================
 *  Preponderant - Preponderant.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Preponderant_H
#include "Preponderant.h"
#endif

void Preponderant::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double chase = 0.00005 / overallscale;
	
	biquadA[0] = pow(A,(3.0*cbrt(overallscale)))*0.42;
	if (biquadA[0] < 0.0001) biquadA[0] = 0.0001;
	
	biquadB[0] = pow(B,(3.0*cbrt(overallscale)))*0.42;
	if (biquadB[0] < 0.0001) biquadB[0] = 0.0001;
	
	biquadC[0] = pow(C,(3.0*cbrt(overallscale)))*0.42;
	if (biquadC[0] < 0.0001) biquadC[0] = 0.0001;
	
    biquadA[1] = biquadB[1] = biquadC[1] = (pow(D,3)*8.0)+0.33;
	biquadB[1] /= 2.0; biquadC[1] /= 4.0;
	
	double volumeCompensation = sqrt(biquadA[1]);
	
	double wet = E;
	
	double K = tan(M_PI * biquadA[0]);
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K / biquadA[1] * norm;
	biquadA[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
	biquadA[4] = -biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadB[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K / biquadB[1] * norm;
	biquadB[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
	biquadB[4] = -biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	K = tan(M_PI * biquadC[0]);
	norm = 1.0 / (1.0 + K / biquadC[1] + K * K);
	biquadC[2] = K / biquadC[1] * norm;
	biquadC[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
	biquadC[4] = -biquadC[2];
	biquadC[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = (1.0 - K / biquadC[1] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = *in1;
		double drySampleR = *in2;
		
		double tempSampleL = (inputSampleL * biquadA[2]) + biquadA[7];
		biquadA[7] = -(tempSampleL * biquadA[5]) + biquadA[8]; //BANDPASS form
		biquadA[8] = (inputSampleL * biquadA[4]) - (tempSampleL * biquadA[6]);
		double bassSampleL = tempSampleL; //like mono AU, 7 and 8 store L channel
		
		double tempSampleR = (inputSampleR * biquadA[2]) + biquadA[9];
		biquadA[9] = -(tempSampleR * biquadA[5]) + biquadA[10]; //BANDPASS form
		biquadA[10] = (inputSampleR * biquadA[4]) - (tempSampleR * biquadA[6]);
		double bassSampleR = tempSampleR; //note: 9 and 10 store the R channel
		
		tempSampleL = (inputSampleL * biquadB[2]) + biquadB[7];
		biquadB[7] = -(tempSampleL * biquadB[5]) + biquadB[8]; //BANDPASS form
		biquadB[8] = (inputSampleL * biquadB[4]) - (tempSampleL * biquadB[6]);
		double midSampleL = tempSampleL; //like mono AU, 7 and 8 store L channel
		
		tempSampleR = (inputSampleR * biquadB[2]) + biquadB[9];
		biquadB[9] = -(tempSampleR * biquadB[5]) + biquadB[10]; //BANDPASS form
		biquadB[10] = (inputSampleR * biquadB[4]) - (tempSampleR * biquadB[6]);
		double midSampleR = tempSampleR; //note: 9 and 10 store the R channel
		
		
		tempSampleL = (inputSampleL * biquadC[2]) + biquadC[7];
		biquadC[7] = -(tempSampleL * biquadC[5]) + biquadC[8]; //BANDPASS form
		biquadC[8] = (inputSampleL * biquadC[4]) - (tempSampleL * biquadC[6]);
		double highSampleL = tempSampleL; //like mono AU, 7 and 8 store L channel
		
		tempSampleR = (inputSampleR * biquadC[2]) + biquadC[9];
		biquadC[9] = -(tempSampleR * biquadC[5]) + biquadC[10]; //BANDPASS form
		biquadC[10] = (inputSampleR * biquadC[4]) - (tempSampleR * biquadC[6]);
		double highSampleR = tempSampleR; //note: 9 and 10 store the R channel
		
		if (bassBalanceL < 0.00125) bassBalanceL = 0.00125;
		if (bassBalanceL > 1.0) bassBalanceL = 1.0;
		
		if (bassBalanceR < 0.00125) bassBalanceR = 0.00125;
		if (bassBalanceR > 1.0) bassBalanceR = 1.0;
		
		if (midBalanceL < 0.00125) midBalanceL = 0.00125;
		if (midBalanceL > 1.0) midBalanceL = 1.0;
		
		if (midBalanceR < 0.00125) midBalanceR = 0.00125;
		if (midBalanceR > 1.0) midBalanceR = 1.0;
		
		if (highBalanceL < 0.00125) highBalanceL = 0.00125;
		if (highBalanceL > 1.0) highBalanceL = 1.0;
		
		if (highBalanceR < 0.00125) highBalanceR = 0.00125;
		if (highBalanceR > 1.0) highBalanceR = 1.0;
		
		bassSampleL *= bassBalanceL;
		midSampleL *= midBalanceL;
		highSampleL *= highBalanceL;
		if (bassTrackL < fabs(bassSampleL)) bassTrackL = fabs(bassSampleL);
		if (midTrackL < fabs(midSampleL)) midTrackL = fabs(midSampleL);
		if (highTrackL < fabs(highSampleL)) highTrackL = fabs(highSampleL);
		if (bassTrackL > quicknessL) bassTrackL -= quicknessL;
		if (midTrackL > quicknessL) midTrackL -= quicknessL;
		if (highTrackL > quicknessL) highTrackL -= quicknessL;
		
		double disparity = 0.0;
		if (disparity < fabs(bassTrackL - midTrackL)) disparity = fabs(bassTrackL - midTrackL);
		if (disparity < fabs(bassTrackL - highTrackL)) disparity = fabs(bassTrackL - highTrackL);
		if (disparity < fabs(midTrackL - highTrackL)) disparity = fabs(midTrackL - highTrackL);
		
		quicknessL *= (1.0-chase);
		quicknessL += ((1.0+disparity) * 0.0001 * chase);
		
		if (bassTrackL > midTrackL) {bassBalanceL -= quicknessL; midBalanceL += quicknessL;}
		else {bassBalanceL += quicknessL; midBalanceL -= quicknessL;}
		if (midTrackL > highTrackL) {midBalanceL -= quicknessL; highBalanceL += quicknessL;}
		else {midBalanceL += quicknessL; highBalanceL -= quicknessL;}
		if (highTrackL > bassTrackL) {highBalanceL -= quicknessL; bassBalanceL += quicknessL;}
		else {highBalanceL += quicknessL; bassBalanceL -= quicknessL;}
		
		if (highBalanceL > 0.618033988749894848204586) highBalanceL -= (quicknessL*0.001); else highBalanceL += (quicknessL*0.001);
		if (midBalanceL > 0.618033988749894848204586) midBalanceL -= (quicknessL*0.001); else midBalanceL += (quicknessL*0.001);
		if (bassBalanceL > 0.618033988749894848204586) bassBalanceL -= (quicknessL*0.001); else bassBalanceL += (quicknessL*0.001);
		
		bassSampleR *= bassBalanceR;
		midSampleR *= midBalanceR;
		highSampleR *= highBalanceR;
		if (bassTrackR < fabs(bassSampleR)) bassTrackR = fabs(bassSampleR);
		if (midTrackR < fabs(midSampleR)) midTrackR = fabs(midSampleR);
		if (highTrackR < fabs(highSampleR)) highTrackR = fabs(highSampleR);
		if (bassTrackR > quicknessR) bassTrackR -= quicknessR;
		if (midTrackR > quicknessR) midTrackR -= quicknessR;
		if (highTrackR > quicknessR) highTrackR -= quicknessR;
		
		disparity = 0.0;
		if (disparity < fabs(bassTrackR - midTrackR)) disparity = fabs(bassTrackR - midTrackR);
		if (disparity < fabs(bassTrackR - highTrackR)) disparity = fabs(bassTrackR - highTrackR);
		if (disparity < fabs(midTrackR - highTrackR)) disparity = fabs(midTrackR - highTrackR);
		
		quicknessR *= (1.0-chase);
		quicknessR += ((1.0+disparity) * 0.0001 * chase);
		
		if (bassTrackR > midTrackR) {bassBalanceR -= quicknessR; midBalanceR += quicknessR;}
		else {bassBalanceR += quicknessR; midBalanceR -= quicknessR;}
		if (midTrackR > highTrackR) {midBalanceR -= quicknessR; highBalanceR += quicknessR;}
		else {midBalanceR += quicknessR; highBalanceR -= quicknessR;}
		if (highTrackR > bassTrackR) {highBalanceR -= quicknessR; bassBalanceR += quicknessR;}
		else {highBalanceR += quicknessR; bassBalanceR -= quicknessR;}
		
		if (highBalanceR > 0.618033988749894848204586) highBalanceR -= (quicknessR*0.001); else highBalanceR += (quicknessR*0.001);
		if (midBalanceR > 0.618033988749894848204586) midBalanceR -= (quicknessR*0.001); else midBalanceR += (quicknessR*0.001);
		if (bassBalanceR > 0.618033988749894848204586) bassBalanceR -= (quicknessR*0.001); else bassBalanceR += (quicknessR*0.001);
		
		inputSampleL = (bassSampleL + midSampleL + highSampleL) * volumeCompensation;
		inputSampleR = (bassSampleR + midSampleR + highSampleR) * volumeCompensation;
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Preponderant::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double chase = 0.00005 / overallscale;
	
	biquadA[0] = pow(A,(3.0*cbrt(overallscale)))*0.42;
	if (biquadA[0] < 0.0001) biquadA[0] = 0.0001;
	
	biquadB[0] = pow(B,(3.0*cbrt(overallscale)))*0.42;
	if (biquadB[0] < 0.0001) biquadB[0] = 0.0001;
	
	biquadC[0] = pow(C,(3.0*cbrt(overallscale)))*0.42;
	if (biquadC[0] < 0.0001) biquadC[0] = 0.0001;
	
    biquadA[1] = biquadB[1] = biquadC[1] = (pow(D,3)*8.0)+0.33;
	biquadB[1] /= 2.0; biquadC[1] /= 4.0;
	
	double volumeCompensation = sqrt(biquadA[1]);
	
	double wet = E;
	
	double K = tan(M_PI * biquadA[0]);
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K / biquadA[1] * norm;
	biquadA[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
	biquadA[4] = -biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadB[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K / biquadB[1] * norm;
	biquadB[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
	biquadB[4] = -biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	K = tan(M_PI * biquadC[0]);
	norm = 1.0 / (1.0 + K / biquadC[1] + K * K);
	biquadC[2] = K / biquadC[1] * norm;
	biquadC[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
	biquadC[4] = -biquadC[2];
	biquadC[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = (1.0 - K / biquadC[1] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = *in1;
		double drySampleR = *in2;
		
		double tempSampleL = (inputSampleL * biquadA[2]) + biquadA[7];
		biquadA[7] = -(tempSampleL * biquadA[5]) + biquadA[8]; //BANDPASS form
		biquadA[8] = (inputSampleL * biquadA[4]) - (tempSampleL * biquadA[6]);
		double bassSampleL = tempSampleL; //like mono AU, 7 and 8 store L channel
		
		double tempSampleR = (inputSampleR * biquadA[2]) + biquadA[9];
		biquadA[9] = -(tempSampleR * biquadA[5]) + biquadA[10]; //BANDPASS form
		biquadA[10] = (inputSampleR * biquadA[4]) - (tempSampleR * biquadA[6]);
		double bassSampleR = tempSampleR; //note: 9 and 10 store the R channel
		
		tempSampleL = (inputSampleL * biquadB[2]) + biquadB[7];
		biquadB[7] = -(tempSampleL * biquadB[5]) + biquadB[8]; //BANDPASS form
		biquadB[8] = (inputSampleL * biquadB[4]) - (tempSampleL * biquadB[6]);
		double midSampleL = tempSampleL; //like mono AU, 7 and 8 store L channel
		
		tempSampleR = (inputSampleR * biquadB[2]) + biquadB[9];
		biquadB[9] = -(tempSampleR * biquadB[5]) + biquadB[10]; //BANDPASS form
		biquadB[10] = (inputSampleR * biquadB[4]) - (tempSampleR * biquadB[6]);
		double midSampleR = tempSampleR; //note: 9 and 10 store the R channel
		
		
		tempSampleL = (inputSampleL * biquadC[2]) + biquadC[7];
		biquadC[7] = -(tempSampleL * biquadC[5]) + biquadC[8]; //BANDPASS form
		biquadC[8] = (inputSampleL * biquadC[4]) - (tempSampleL * biquadC[6]);
		double highSampleL = tempSampleL; //like mono AU, 7 and 8 store L channel
		
		tempSampleR = (inputSampleR * biquadC[2]) + biquadC[9];
		biquadC[9] = -(tempSampleR * biquadC[5]) + biquadC[10]; //BANDPASS form
		biquadC[10] = (inputSampleR * biquadC[4]) - (tempSampleR * biquadC[6]);
		double highSampleR = tempSampleR; //note: 9 and 10 store the R channel
		
		if (bassBalanceL < 0.00125) bassBalanceL = 0.00125;
		if (bassBalanceL > 1.0) bassBalanceL = 1.0;
		
		if (bassBalanceR < 0.00125) bassBalanceR = 0.00125;
		if (bassBalanceR > 1.0) bassBalanceR = 1.0;
		
		if (midBalanceL < 0.00125) midBalanceL = 0.00125;
		if (midBalanceL > 1.0) midBalanceL = 1.0;
		
		if (midBalanceR < 0.00125) midBalanceR = 0.00125;
		if (midBalanceR > 1.0) midBalanceR = 1.0;
		
		if (highBalanceL < 0.00125) highBalanceL = 0.00125;
		if (highBalanceL > 1.0) highBalanceL = 1.0;
		
		if (highBalanceR < 0.00125) highBalanceR = 0.00125;
		if (highBalanceR > 1.0) highBalanceR = 1.0;
		
		bassSampleL *= bassBalanceL;
		midSampleL *= midBalanceL;
		highSampleL *= highBalanceL;
		if (bassTrackL < fabs(bassSampleL)) bassTrackL = fabs(bassSampleL);
		if (midTrackL < fabs(midSampleL)) midTrackL = fabs(midSampleL);
		if (highTrackL < fabs(highSampleL)) highTrackL = fabs(highSampleL);
		if (bassTrackL > quicknessL) bassTrackL -= quicknessL;
		if (midTrackL > quicknessL) midTrackL -= quicknessL;
		if (highTrackL > quicknessL) highTrackL -= quicknessL;
		
		double disparity = 0.0;
		if (disparity < fabs(bassTrackL - midTrackL)) disparity = fabs(bassTrackL - midTrackL);
		if (disparity < fabs(bassTrackL - highTrackL)) disparity = fabs(bassTrackL - highTrackL);
		if (disparity < fabs(midTrackL - highTrackL)) disparity = fabs(midTrackL - highTrackL);
		
		quicknessL *= (1.0-chase);
		quicknessL += ((1.0+disparity) * 0.0001 * chase);
		
		if (bassTrackL > midTrackL) {bassBalanceL -= quicknessL; midBalanceL += quicknessL;}
		else {bassBalanceL += quicknessL; midBalanceL -= quicknessL;}
		if (midTrackL > highTrackL) {midBalanceL -= quicknessL; highBalanceL += quicknessL;}
		else {midBalanceL += quicknessL; highBalanceL -= quicknessL;}
		if (highTrackL > bassTrackL) {highBalanceL -= quicknessL; bassBalanceL += quicknessL;}
		else {highBalanceL += quicknessL; bassBalanceL -= quicknessL;}
		
		if (highBalanceL > 0.618033988749894848204586) highBalanceL -= (quicknessL*0.001); else highBalanceL += (quicknessL*0.001);
		if (midBalanceL > 0.618033988749894848204586) midBalanceL -= (quicknessL*0.001); else midBalanceL += (quicknessL*0.001);
		if (bassBalanceL > 0.618033988749894848204586) bassBalanceL -= (quicknessL*0.001); else bassBalanceL += (quicknessL*0.001);
		
		bassSampleR *= bassBalanceR;
		midSampleR *= midBalanceR;
		highSampleR *= highBalanceR;
		if (bassTrackR < fabs(bassSampleR)) bassTrackR = fabs(bassSampleR);
		if (midTrackR < fabs(midSampleR)) midTrackR = fabs(midSampleR);
		if (highTrackR < fabs(highSampleR)) highTrackR = fabs(highSampleR);
		if (bassTrackR > quicknessR) bassTrackR -= quicknessR;
		if (midTrackR > quicknessR) midTrackR -= quicknessR;
		if (highTrackR > quicknessR) highTrackR -= quicknessR;
		
		disparity = 0.0;
		if (disparity < fabs(bassTrackR - midTrackR)) disparity = fabs(bassTrackR - midTrackR);
		if (disparity < fabs(bassTrackR - highTrackR)) disparity = fabs(bassTrackR - highTrackR);
		if (disparity < fabs(midTrackR - highTrackR)) disparity = fabs(midTrackR - highTrackR);
		
		quicknessR *= (1.0-chase);
		quicknessR += ((1.0+disparity) * 0.0001 * chase);
		
		if (bassTrackR > midTrackR) {bassBalanceR -= quicknessR; midBalanceR += quicknessR;}
		else {bassBalanceR += quicknessR; midBalanceR -= quicknessR;}
		if (midTrackR > highTrackR) {midBalanceR -= quicknessR; highBalanceR += quicknessR;}
		else {midBalanceR += quicknessR; highBalanceR -= quicknessR;}
		if (highTrackR > bassTrackR) {highBalanceR -= quicknessR; bassBalanceR += quicknessR;}
		else {highBalanceR += quicknessR; bassBalanceR -= quicknessR;}
		
		if (highBalanceR > 0.618033988749894848204586) highBalanceR -= (quicknessR*0.001); else highBalanceR += (quicknessR*0.001);
		if (midBalanceR > 0.618033988749894848204586) midBalanceR -= (quicknessR*0.001); else midBalanceR += (quicknessR*0.001);
		if (bassBalanceR > 0.618033988749894848204586) bassBalanceR -= (quicknessR*0.001); else bassBalanceR += (quicknessR*0.001);
		
		inputSampleL = (bassSampleL + midSampleL + highSampleL) * volumeCompensation;
		inputSampleR = (bassSampleR + midSampleR + highSampleR) * volumeCompensation;
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
