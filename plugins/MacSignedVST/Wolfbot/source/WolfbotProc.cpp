/* ========================================
 *  Wolfbot - Wolfbot.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Wolfbot_H
#include "Wolfbot.h"
#endif

void Wolfbot::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double kalHP = 1.0-(0.004225/overallscale);
	double kalLP = 1.0-(0.954529/overallscale);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Kalman Filter L
		double dryKal = inputSampleL = inputSampleL*(1.0-kalHP)*0.777;
		inputSampleL *= (1.0-kalHP);
		//set up gain levels to control the beast
		kHP[prevSlewL3] += kHP[prevSampL3] - kHP[prevSampL2]; kHP[prevSlewL3] *= 0.5;
		kHP[prevSlewL2] += kHP[prevSampL2] - kHP[prevSampL1]; kHP[prevSlewL2] *= 0.5;
		kHP[prevSlewL1] += kHP[prevSampL1] - inputSampleL; kHP[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kHP[accSlewL2] += kHP[prevSlewL3] - kHP[prevSlewL2]; kHP[accSlewL2] *= 0.5;
		kHP[accSlewL1] += kHP[prevSlewL2] - kHP[prevSlewL1]; kHP[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kHP[accSlewL3] += (kHP[accSlewL2] - kHP[accSlewL1]); kHP[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kHP[kalOutL] += kHP[prevSampL1] + kHP[prevSlewL2] + kHP[accSlewL3]; kHP[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kHP[kalGainL] += fabs(dryKal-kHP[kalOutL])*kalHP*8.0; kHP[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kHP[kalGainL] > kalHP*0.5) kHP[kalGainL] = kalHP*0.5;
		//attempts to avoid explosions
		kHP[kalOutL] += (dryKal*(1.0-(0.68+(kalHP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kHP[prevSampL3] = kHP[prevSampL2];
		kHP[prevSampL2] = kHP[prevSampL1];
		kHP[prevSampL1] = (kHP[kalGainL] * kHP[kalOutL]) + ((1.0-kHP[kalGainL])*dryKal);
		//feed the chain of previous samples
		if (kHP[prevSampL1] > 1.0) kHP[prevSampL1] = 1.0;
		if (kHP[prevSampL1] < -1.0) kHP[prevSampL1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSampleL = drySampleL+(kHP[kalOutL]*-0.777); //highpass
		
		//begin Kalman Filter L
		dryKal = inputSampleL = inputSampleL*(1.0-kalLP)*0.777;
		inputSampleL *= (1.0-kalLP);
		//set up gain levels to control the beast
		kLP[prevSlewL3] += kLP[prevSampL3] - kLP[prevSampL2]; kLP[prevSlewL3] *= 0.5;
		kLP[prevSlewL2] += kLP[prevSampL2] - kLP[prevSampL1]; kLP[prevSlewL2] *= 0.5;
		kLP[prevSlewL1] += kLP[prevSampL1] - inputSampleL; kLP[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kLP[accSlewL2] += kLP[prevSlewL3] - kLP[prevSlewL2]; kLP[accSlewL2] *= 0.5;
		kLP[accSlewL1] += kLP[prevSlewL2] - kLP[prevSlewL1]; kLP[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kLP[accSlewL3] += (kLP[accSlewL2] - kLP[accSlewL1]); kLP[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kLP[kalOutL] += kLP[prevSampL1] + kLP[prevSlewL2] + kLP[accSlewL3]; kLP[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kLP[kalGainL] += fabs(dryKal-kLP[kalOutL])*kalLP*8.0; kLP[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kLP[kalGainL] > kalLP*0.5) kLP[kalGainL] = kalLP*0.5;
		//attempts to avoid explosions
		kLP[kalOutL] += (dryKal*(1.0-(0.68+(kalLP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kLP[prevSampL3] = kLP[prevSampL2];
		kLP[prevSampL2] = kLP[prevSampL1];
		kLP[prevSampL1] = (kLP[kalGainL] * kLP[kalOutL]) + ((1.0-kLP[kalGainL])*dryKal);
		//feed the chain of previous samples
		if (kLP[prevSampL1] > 1.0) kLP[prevSampL1] = 1.0;
		if (kLP[prevSampL1] < -1.0) kLP[prevSampL1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSampleL = sin(kLP[kalOutL]*0.7943)*1.2589; //lowpass
		
		
		//begin Kalman Filter R
		dryKal = inputSampleR = inputSampleR*(1.0-kalHP)*0.777;
		inputSampleR *= (1.0-kalHP);
		//set up gain levels to control the beast
		kHP[prevSlewR3] += kHP[prevSampR3] - kHP[prevSampR2]; kHP[prevSlewR3] *= 0.5;
		kHP[prevSlewR2] += kHP[prevSampR2] - kHP[prevSampR1]; kHP[prevSlewR2] *= 0.5;
		kHP[prevSlewR1] += kHP[prevSampR1] - inputSampleR; kHP[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kHP[accSlewR2] += kHP[prevSlewR3] - kHP[prevSlewR2]; kHP[accSlewR2] *= 0.5;
		kHP[accSlewR1] += kHP[prevSlewR2] - kHP[prevSlewR1]; kHP[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kHP[accSlewR3] += (kHP[accSlewR2] - kHP[accSlewR1]); kHP[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kHP[kalOutR] += kHP[prevSampR1] + kHP[prevSlewR2] + kHP[accSlewR3]; kHP[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kHP[kalGainR] += fabs(dryKal-kHP[kalOutR])*kalHP*8.0; kHP[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kHP[kalGainR] > kalHP*0.5) kHP[kalGainR] = kalHP*0.5;
		//attempts to avoid explosions
		kHP[kalOutR] += (dryKal*(1.0-(0.68+(kalHP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kHP[prevSampR3] = kHP[prevSampR2];
		kHP[prevSampR2] = kHP[prevSampR1];
		kHP[prevSampR1] = (kHP[kalGainR] * kHP[kalOutR]) + ((1.0-kHP[kalGainR])*dryKal);
		//feed the chain of previous samples
		if (kHP[prevSampR1] > 1.0) kHP[prevSampR1] = 1.0;
		if (kHP[prevSampR1] < -1.0) kHP[prevSampR1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSampleR = drySampleR+(kHP[kalOutR]*-0.777); //highpass
		
		//begin Kalman Filter R
		dryKal = inputSampleR = inputSampleR*(1.0-kalLP)*0.777;
		inputSampleR *= (1.0-kalLP);
		//set up gain levels to control the beast
		kLP[prevSlewR3] += kLP[prevSampR3] - kLP[prevSampR2]; kLP[prevSlewR3] *= 0.5;
		kLP[prevSlewR2] += kLP[prevSampR2] - kLP[prevSampR1]; kLP[prevSlewR2] *= 0.5;
		kLP[prevSlewR1] += kLP[prevSampR1] - inputSampleR; kLP[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kLP[accSlewR2] += kLP[prevSlewR3] - kLP[prevSlewR2]; kLP[accSlewR2] *= 0.5;
		kLP[accSlewR1] += kLP[prevSlewR2] - kLP[prevSlewR1]; kLP[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kLP[accSlewR3] += (kLP[accSlewR2] - kLP[accSlewR1]); kLP[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kLP[kalOutR] += kLP[prevSampR1] + kLP[prevSlewR2] + kLP[accSlewR3]; kLP[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kLP[kalGainR] += fabs(dryKal-kLP[kalOutR])*kalLP*8.0; kLP[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kLP[kalGainR] > kalLP*0.5) kLP[kalGainR] = kalLP*0.5;
		//attempts to avoid explosions
		kLP[kalOutR] += (dryKal*(1.0-(0.68+(kalLP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kLP[prevSampR3] = kLP[prevSampR2];
		kLP[prevSampR2] = kLP[prevSampR1];
		kLP[prevSampR1] = (kLP[kalGainR] * kLP[kalOutR]) + ((1.0-kLP[kalGainR])*dryKal);
		//feed the chain of previous samples
		if (kLP[prevSampR1] > 1.0) kLP[prevSampR1] = 1.0;
		if (kLP[prevSampR1] < -1.0) kLP[prevSampR1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSampleR = sin(kLP[kalOutR]*0.7943)*1.2589; //lowpass
		
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

void Wolfbot::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double kalHP = 1.0-(0.004225/overallscale);
	double kalLP = 1.0-(0.954529/overallscale);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Kalman Filter L
		double dryKal = inputSampleL = inputSampleL*(1.0-kalHP)*0.777;
		inputSampleL *= (1.0-kalHP);
		//set up gain levels to control the beast
		kHP[prevSlewL3] += kHP[prevSampL3] - kHP[prevSampL2]; kHP[prevSlewL3] *= 0.5;
		kHP[prevSlewL2] += kHP[prevSampL2] - kHP[prevSampL1]; kHP[prevSlewL2] *= 0.5;
		kHP[prevSlewL1] += kHP[prevSampL1] - inputSampleL; kHP[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kHP[accSlewL2] += kHP[prevSlewL3] - kHP[prevSlewL2]; kHP[accSlewL2] *= 0.5;
		kHP[accSlewL1] += kHP[prevSlewL2] - kHP[prevSlewL1]; kHP[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kHP[accSlewL3] += (kHP[accSlewL2] - kHP[accSlewL1]); kHP[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kHP[kalOutL] += kHP[prevSampL1] + kHP[prevSlewL2] + kHP[accSlewL3]; kHP[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kHP[kalGainL] += fabs(dryKal-kHP[kalOutL])*kalHP*8.0; kHP[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kHP[kalGainL] > kalHP*0.5) kHP[kalGainL] = kalHP*0.5;
		//attempts to avoid explosions
		kHP[kalOutL] += (dryKal*(1.0-(0.68+(kalHP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kHP[prevSampL3] = kHP[prevSampL2];
		kHP[prevSampL2] = kHP[prevSampL1];
		kHP[prevSampL1] = (kHP[kalGainL] * kHP[kalOutL]) + ((1.0-kHP[kalGainL])*dryKal);
		//feed the chain of previous samples
		if (kHP[prevSampL1] > 1.0) kHP[prevSampL1] = 1.0;
		if (kHP[prevSampL1] < -1.0) kHP[prevSampL1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSampleL = drySampleL+(kHP[kalOutL]*-0.777); //highpass
		
		//begin Kalman Filter L
		dryKal = inputSampleL = inputSampleL*(1.0-kalLP)*0.777;
		inputSampleL *= (1.0-kalLP);
		//set up gain levels to control the beast
		kLP[prevSlewL3] += kLP[prevSampL3] - kLP[prevSampL2]; kLP[prevSlewL3] *= 0.5;
		kLP[prevSlewL2] += kLP[prevSampL2] - kLP[prevSampL1]; kLP[prevSlewL2] *= 0.5;
		kLP[prevSlewL1] += kLP[prevSampL1] - inputSampleL; kLP[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kLP[accSlewL2] += kLP[prevSlewL3] - kLP[prevSlewL2]; kLP[accSlewL2] *= 0.5;
		kLP[accSlewL1] += kLP[prevSlewL2] - kLP[prevSlewL1]; kLP[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kLP[accSlewL3] += (kLP[accSlewL2] - kLP[accSlewL1]); kLP[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kLP[kalOutL] += kLP[prevSampL1] + kLP[prevSlewL2] + kLP[accSlewL3]; kLP[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kLP[kalGainL] += fabs(dryKal-kLP[kalOutL])*kalLP*8.0; kLP[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kLP[kalGainL] > kalLP*0.5) kLP[kalGainL] = kalLP*0.5;
		//attempts to avoid explosions
		kLP[kalOutL] += (dryKal*(1.0-(0.68+(kalLP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kLP[prevSampL3] = kLP[prevSampL2];
		kLP[prevSampL2] = kLP[prevSampL1];
		kLP[prevSampL1] = (kLP[kalGainL] * kLP[kalOutL]) + ((1.0-kLP[kalGainL])*dryKal);
		//feed the chain of previous samples
		if (kLP[prevSampL1] > 1.0) kLP[prevSampL1] = 1.0;
		if (kLP[prevSampL1] < -1.0) kLP[prevSampL1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSampleL = sin(kLP[kalOutL]*0.7943)*1.2589; //lowpass
		
		
		//begin Kalman Filter R
		dryKal = inputSampleR = inputSampleR*(1.0-kalHP)*0.777;
		inputSampleR *= (1.0-kalHP);
		//set up gain levels to control the beast
		kHP[prevSlewR3] += kHP[prevSampR3] - kHP[prevSampR2]; kHP[prevSlewR3] *= 0.5;
		kHP[prevSlewR2] += kHP[prevSampR2] - kHP[prevSampR1]; kHP[prevSlewR2] *= 0.5;
		kHP[prevSlewR1] += kHP[prevSampR1] - inputSampleR; kHP[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kHP[accSlewR2] += kHP[prevSlewR3] - kHP[prevSlewR2]; kHP[accSlewR2] *= 0.5;
		kHP[accSlewR1] += kHP[prevSlewR2] - kHP[prevSlewR1]; kHP[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kHP[accSlewR3] += (kHP[accSlewR2] - kHP[accSlewR1]); kHP[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kHP[kalOutR] += kHP[prevSampR1] + kHP[prevSlewR2] + kHP[accSlewR3]; kHP[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kHP[kalGainR] += fabs(dryKal-kHP[kalOutR])*kalHP*8.0; kHP[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kHP[kalGainR] > kalHP*0.5) kHP[kalGainR] = kalHP*0.5;
		//attempts to avoid explosions
		kHP[kalOutR] += (dryKal*(1.0-(0.68+(kalHP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kHP[prevSampR3] = kHP[prevSampR2];
		kHP[prevSampR2] = kHP[prevSampR1];
		kHP[prevSampR1] = (kHP[kalGainR] * kHP[kalOutR]) + ((1.0-kHP[kalGainR])*dryKal);
		//feed the chain of previous samples
		if (kHP[prevSampR1] > 1.0) kHP[prevSampR1] = 1.0;
		if (kHP[prevSampR1] < -1.0) kHP[prevSampR1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSampleR = drySampleR+(kHP[kalOutR]*-0.777); //highpass
		
		//begin Kalman Filter R
		dryKal = inputSampleR = inputSampleR*(1.0-kalLP)*0.777;
		inputSampleR *= (1.0-kalLP);
		//set up gain levels to control the beast
		kLP[prevSlewR3] += kLP[prevSampR3] - kLP[prevSampR2]; kLP[prevSlewR3] *= 0.5;
		kLP[prevSlewR2] += kLP[prevSampR2] - kLP[prevSampR1]; kLP[prevSlewR2] *= 0.5;
		kLP[prevSlewR1] += kLP[prevSampR1] - inputSampleR; kLP[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kLP[accSlewR2] += kLP[prevSlewR3] - kLP[prevSlewR2]; kLP[accSlewR2] *= 0.5;
		kLP[accSlewR1] += kLP[prevSlewR2] - kLP[prevSlewR1]; kLP[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kLP[accSlewR3] += (kLP[accSlewR2] - kLP[accSlewR1]); kLP[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kLP[kalOutR] += kLP[prevSampR1] + kLP[prevSlewR2] + kLP[accSlewR3]; kLP[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kLP[kalGainR] += fabs(dryKal-kLP[kalOutR])*kalLP*8.0; kLP[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kLP[kalGainR] > kalLP*0.5) kLP[kalGainR] = kalLP*0.5;
		//attempts to avoid explosions
		kLP[kalOutR] += (dryKal*(1.0-(0.68+(kalLP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kLP[prevSampR3] = kLP[prevSampR2];
		kLP[prevSampR2] = kLP[prevSampR1];
		kLP[prevSampR1] = (kLP[kalGainR] * kLP[kalOutR]) + ((1.0-kLP[kalGainR])*dryKal);
		//feed the chain of previous samples
		if (kLP[prevSampR1] > 1.0) kLP[prevSampR1] = 1.0;
		if (kLP[prevSampR1] < -1.0) kLP[prevSampR1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSampleR = sin(kLP[kalOutR]*0.7943)*1.2589; //lowpass		
				
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
