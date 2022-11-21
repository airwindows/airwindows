/* ========================================
 *  Exciter - Exciter.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Exciter_H
#include "Exciter.h"
#endif

void Exciter::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	biquad[0] = ((A*7000.0)+8000.0)/getSampleRate();
	biquad[1] = A+B+0.7071;
	//tighter resonance as frequency and boost increases
	double boost = pow(B,2.0)*16.0;
	double K = tan(M_PI * biquad[0]);
	double norm = 1.0 / (1.0 + K / biquad[1] + K * K);
	biquad[2] = K / 0.7071 * norm;
	biquad[4] = -biquad[2];
	biquad[5] = 2.0 * (K * K - 1.0) * norm;
	biquad[6] = (1.0 - K / biquad[1] + K * K) * norm;
	//bandpass to focus the intensity of the effect
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double outSampleL = (inputSampleL * biquad[2]) + biquad[7];
		biquad[7] = (inputSampleL * biquad[3]) - (outSampleL * biquad[5]) + biquad[8];
		biquad[8] = (inputSampleL * biquad[4]) - (outSampleL * biquad[6]);
		
		double outSampleR = (inputSampleR * biquad[2]) + biquad[9];
		biquad[9] = (inputSampleR * biquad[3]) - (outSampleR * biquad[5]) + biquad[10];
		biquad[10] = (inputSampleR * biquad[4]) - (outSampleR * biquad[6]);
		//so the audio we're working with is going to be a bandpassed signal: only high mids
		
		outSampleL *= boost;
		if (outSampleL > M_PI) outSampleL = M_PI;
		if (outSampleL < -M_PI) outSampleL = -M_PI;
		outSampleL -= sin(outSampleL);

		outSampleR *= boost;
		if (outSampleR > M_PI) outSampleR = M_PI;
		if (outSampleR < -M_PI) outSampleR = -M_PI;
		outSampleR -= sin(outSampleR);
		//so we're clipping to generate artifacts, but subtracting them, meaning that
		//our deviations from the unclipped waveform are now negative in form.
		
		outSampleL *= boost;
		if (outSampleL > M_PI) outSampleL = M_PI;
		if (outSampleL < -M_PI) outSampleL = -M_PI;
		outSampleL = sin(outSampleL);

		outSampleR *= boost;
		if (outSampleR > M_PI) outSampleR = M_PI;
		if (outSampleR < -M_PI) outSampleR = -M_PI;
		outSampleR = sin(outSampleR);
		//now we're clipping the result, to make the peaks less intense
		
		inputSampleL -= outSampleL;
		inputSampleR -= outSampleR;
		//and we apply only those deviations from distorted high mids, by subtracting them from
		//the original full bandwidth audio. And that's how analog aural exciters basically work.
		//The real ones used a 4049 chip sometimes to produce the soft saturation we're getting with sin()	
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		//and we'll do the harshest of hardclips to cope with how intense the new peaks can get,
		//without in any way dialing back the ruthless brightness the exciter can create.
		
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

void Exciter::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	biquad[0] = ((A*7000.0)+8000.0)/getSampleRate();
	biquad[1] = A+B+0.7071;
	//tighter resonance as frequency and boost increases
	double boost = pow(B,2.0)*16.0;
	double K = tan(M_PI * biquad[0]);
	double norm = 1.0 / (1.0 + K / biquad[1] + K * K);
	biquad[2] = K / 0.7071 * norm;
	biquad[4] = -biquad[2];
	biquad[5] = 2.0 * (K * K - 1.0) * norm;
	biquad[6] = (1.0 - K / biquad[1] + K * K) * norm;
	//bandpass to focus the intensity of the effect
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double outSampleL = (inputSampleL * biquad[2]) + biquad[7];
		biquad[7] = (inputSampleL * biquad[3]) - (outSampleL * biquad[5]) + biquad[8];
		biquad[8] = (inputSampleL * biquad[4]) - (outSampleL * biquad[6]);
		
		double outSampleR = (inputSampleR * biquad[2]) + biquad[9];
		biquad[9] = (inputSampleR * biquad[3]) - (outSampleR * biquad[5]) + biquad[10];
		biquad[10] = (inputSampleR * biquad[4]) - (outSampleR * biquad[6]);
		//so the audio we're working with is going to be a bandpassed signal: only high mids
		
		outSampleL *= boost;
		if (outSampleL > M_PI) outSampleL = M_PI;
		if (outSampleL < -M_PI) outSampleL = -M_PI;
		outSampleL -= sin(outSampleL);
		
		outSampleR *= boost;
		if (outSampleR > M_PI) outSampleR = M_PI;
		if (outSampleR < -M_PI) outSampleR = -M_PI;
		outSampleR -= sin(outSampleR);
		//so we're clipping to generate artifacts, but subtracting them, meaning that
		//our deviations from the unclipped waveform are now negative in form.
		
		outSampleL *= boost;
		if (outSampleL > M_PI) outSampleL = M_PI;
		if (outSampleL < -M_PI) outSampleL = -M_PI;
		outSampleL = sin(outSampleL);
		
		outSampleR *= boost;
		if (outSampleR > M_PI) outSampleR = M_PI;
		if (outSampleR < -M_PI) outSampleR = -M_PI;
		outSampleR = sin(outSampleR);
		//now we're clipping the result, to make the peaks less intense
		
		inputSampleL -= outSampleL;
		inputSampleR -= outSampleR;
		//and we apply only those deviations from distorted high mids, by subtracting them from
		//the original full bandwidth audio. And that's how analog aural exciters basically work.
		//The real ones used a 4049 chip sometimes to produce the soft saturation we're getting with sin()	
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		//and we'll do the harshest of hardclips to cope with how intense the new peaks can get,
		//without in any way dialing back the ruthless brightness the exciter can create.
		
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
