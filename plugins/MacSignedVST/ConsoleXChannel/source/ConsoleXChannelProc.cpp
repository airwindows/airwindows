/* ========================================
 *  ConsoleXChannel - ConsoleXChannel.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleXChannel_H
#include "ConsoleXChannel.h"
#endif

void ConsoleXChannel::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 3) cycleEnd = 3;
	
	biquad[biq_freq] = 25000.0/getSampleRate();	
    biquad[biq_reso] = 0.50979558;
	double K = tan(M_PI * biquad[biq_freq]);
	double norm = 1.0 / (1.0 + K / biquad[biq_reso] + K * K);
	biquad[biq_a0] = K * K * norm;
	biquad[biq_a1] = 2.0 * biquad[biq_a0];
	biquad[biq_a2] = biquad[biq_a0];
	biquad[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	biquad[biq_b2] = (1.0 - K / biquad[biq_reso] + K * K) * norm;
	//ultrasonic nonlinear filter
	
	trebleGainA = trebleGainB; trebleGainB = A*2.0;
	midGainA = midGainB; midGainB = B*2.0;
	bassGainA = bassGainB; bassGainB = C*2.0;
	//simple three band to adjust
	
	//begin ResEQ2 Mid Boost
	double freqMPeak = pow(D+0.16,3);
	mPeakA = mPeakB; mPeakB = fabs(midGainB-1.0); //amount of mid peak leak through (or boost)
	if (midGainB < 1.0) mPeakB *= 0.5;
	int maxMPeak = (24.0*(2.0-freqMPeak))+16;
	if ((freqMPeak != prevfreqMPeak)||(mPeakB != prevamountMPeak)) {
		for (int x = 0; x < maxMPeak; x++) {
			if (((double)x*freqMPeak) < M_PI_4) f[x] = sin(((double)x*freqMPeak)*4.0)*freqMPeak*sin(((double)(maxMPeak-x)/(double)maxMPeak)*M_PI_2);
			else f[x] = cos((double)x*freqMPeak)*freqMPeak*sin(((double)(maxMPeak-x)/(double)maxMPeak)*M_PI_2);
		}
		prevfreqMPeak = freqMPeak; prevamountMPeak = mPeakB;
	}//end ResEQ2 Mid Boost
	//mid peak for either retaining during mid cut, or adding during mid boost
	
	double kalman = 1.0-pow(E,2);
	//crossover frequency between mid/bass
	
	double refdB = (F*70.0)+70.0;
	double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale;
	
	panA = panB; panB = G*1.57079633;
	inTrimA = inTrimB; inTrimB = H*2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (biquad[biq_freq] < 0.5) {
			double nlBiq = fabs(biquad[biq_a0]*(1.0+(inputSampleL*0.25))); if (nlBiq > 1.0) nlBiq = 1.0;
			double tmp = (inputSampleL * nlBiq) + biquad[biq_sL1];
			biquad[biq_sL1] = (inputSampleL * biquad[biq_a1]) - (tmp * biquad[biq_b1]) + biquad[biq_sL2];
			biquad[biq_sL2] = (inputSampleL * nlBiq) - (tmp * biquad[biq_b2]);
			inputSampleL = tmp;
			nlBiq = fabs(biquad[biq_a0]*(1.0+(inputSampleR*0.25))); if (nlBiq > 1.0) nlBiq = 1.0;
			tmp = (inputSampleR * nlBiq) + biquad[biq_sR1];
			biquad[biq_sR1] = (inputSampleR * biquad[biq_a1]) - (tmp * biquad[biq_b1]) + biquad[biq_sR2];
			biquad[biq_sR2] = (inputSampleR * nlBiq) - (tmp * biquad[biq_b2]);
			inputSampleR = tmp;
			//ultrasonic filter before anything else is done
		}
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double trebleGain = (trebleGainA*temp)+(trebleGainB*(1.0-temp));
		if (trebleGain > 1.0) trebleGain = pow(trebleGain,3.0+sqrt(overallscale));
		if (trebleGain < 1.0) trebleGain = 1.0-pow(1.0-trebleGain,2);
		
		double midGain = (midGainA*temp)+(midGainB*(1.0-temp));
		if (midGain > 1.0) midGain = 1.0;
		if (midGain < 1.0) midGain = 1.0-pow(1.0-midGain,2);
		double mPeak = pow((mPeakA*temp)+(mPeakB*(1.0-temp)),2);
		
		double bassGain = (bassGainA*temp)+(bassGainB*(1.0-temp));
		if (bassGain > 1.0) bassGain *= bassGain;
		if (bassGain < 1.0) bassGain = 1.0-pow(1.0-bassGain,2);
		
		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain;
		if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
		gain *= 1.527864045000421;
		
		//begin Air3L
		air[pvSL4] = air[pvAL4] - air[pvAL3]; air[pvSL3] = air[pvAL3] - air[pvAL2];
		air[pvSL2] = air[pvAL2] - air[pvAL1]; air[pvSL1] = air[pvAL1] - inputSampleL;
		air[accSL3] = air[pvSL4] - air[pvSL3]; air[accSL2] = air[pvSL3] - air[pvSL2];
		air[accSL1] = air[pvSL2] - air[pvSL1];
		air[acc2SL2] = air[accSL3] - air[accSL2]; air[acc2SL1] = air[accSL2] - air[accSL1];		
		air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
		air[gainAL] *= 0.5; air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
		if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
		air[pvAL4] = air[pvAL3]; air[pvAL3] = air[pvAL2];
		air[pvAL2] = air[pvAL1]; air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
		double midL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		temp = (midL + air[gndavgL])*0.5; air[gndavgL] = midL; midL = temp;
		double trebleL = drySampleL-midL;
		inputSampleL = midL;
		//end Air3L
		
		//begin Air3R
		air[pvSR4] = air[pvAR4] - air[pvAR3]; air[pvSR3] = air[pvAR3] - air[pvAR2];
		air[pvSR2] = air[pvAR2] - air[pvAR1]; air[pvSR1] = air[pvAR1] - inputSampleR;
		air[accSR3] = air[pvSR4] - air[pvSR3]; air[accSR2] = air[pvSR3] - air[pvSR2];
		air[accSR1] = air[pvSR2] - air[pvSR1];
		air[acc2SR2] = air[accSR3] - air[accSR2]; air[acc2SR1] = air[accSR2] - air[accSR1];		
		air[outAR] = -(air[pvAR1] + air[pvSR3] + air[acc2SR2] - ((air[acc2SR2] + air[acc2SR1])*0.5));
		air[gainAR] *= 0.5; air[gainAR] += fabs(drySampleR-air[outAR])*0.5;
		if (air[gainAR] > 0.3*sqrt(overallscale)) air[gainAR] = 0.3*sqrt(overallscale);
		air[pvAR4] = air[pvAR3]; air[pvAR3] = air[pvAR2];
		air[pvAR2] = air[pvAR1]; air[pvAR1] = (air[gainAR] * air[outAR]) + drySampleR;
		double midR = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
		temp = (midR + air[gndavgR])*0.5; air[gndavgR] = midR; midR = temp;
		double trebleR = drySampleR-midR;
		inputSampleR = midR;
		//end Air3R
		
		//begin KalmanL
		temp = inputSampleL = inputSampleL*(1.0-kalman)*0.777;
		inputSampleL *= (1.0-kalman);
		//set up gain levels to control the beast
		kal[prevSlewL3] += kal[prevSampL3] - kal[prevSampL2]; kal[prevSlewL3] *= 0.5;
		kal[prevSlewL2] += kal[prevSampL2] - kal[prevSampL1]; kal[prevSlewL2] *= 0.5;
		kal[prevSlewL1] += kal[prevSampL1] - inputSampleL; kal[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kal[accSlewL2] += kal[prevSlewL3] - kal[prevSlewL2]; kal[accSlewL2] *= 0.5;
		kal[accSlewL1] += kal[prevSlewL2] - kal[prevSlewL1]; kal[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kal[accSlewL3] += (kal[accSlewL2] - kal[accSlewL1]); kal[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kal[kalOutL] += kal[prevSampL1] + kal[prevSlewL2] + kal[accSlewL3]; kal[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kal[kalGainL] += fabs(temp-kal[kalOutL])*kalman*8.0; kal[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kal[kalGainL] > kalman*0.5) kal[kalGainL] = kalman*0.5;
		//attempts to avoid explosions
		kal[kalOutL] += (temp*(1.0-(0.68+(kalman*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kal[prevSampL3] = kal[prevSampL2]; kal[prevSampL2] = kal[prevSampL1];
		kal[prevSampL1] = (kal[kalGainL] * kal[kalOutL]) + ((1.0-kal[kalGainL])*temp);
		//feed the chain of previous samples
		if (kal[prevSampL1] > 1.0) kal[prevSampL1] = 1.0; if (kal[prevSampL1] < -1.0) kal[prevSampL1] = -1.0;
		double bassL = kal[kalOutL]*0.777;
		midL -= bassL;
		//end KalmanL
		
		//begin KalmanR
		temp = inputSampleR = inputSampleR*(1.0-kalman)*0.777;
		inputSampleR *= (1.0-kalman);
		//set up gain levels to control the beast
		kal[prevSlewR3] += kal[prevSampR3] - kal[prevSampR2]; kal[prevSlewR3] *= 0.5;
		kal[prevSlewR2] += kal[prevSampR2] - kal[prevSampR1]; kal[prevSlewR2] *= 0.5;
		kal[prevSlewR1] += kal[prevSampR1] - inputSampleR; kal[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kal[accSlewR2] += kal[prevSlewR3] - kal[prevSlewR2]; kal[accSlewR2] *= 0.5;
		kal[accSlewR1] += kal[prevSlewR2] - kal[prevSlewR1]; kal[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kal[accSlewR3] += (kal[accSlewR2] - kal[accSlewR1]); kal[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kal[kalOutR] += kal[prevSampR1] + kal[prevSlewR2] + kal[accSlewR3]; kal[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kal[kalGainR] += fabs(temp-kal[kalOutR])*kalman*8.0; kal[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kal[kalGainR] > kalman*0.5) kal[kalGainR] = kalman*0.5;
		//attempts to avoid explosions
		kal[kalOutR] += (temp*(1.0-(0.68+(kalman*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kal[prevSampR3] = kal[prevSampR2]; kal[prevSampR2] = kal[prevSampR1];
		kal[prevSampR1] = (kal[kalGainR] * kal[kalOutR]) + ((1.0-kal[kalGainR])*temp);
		//feed the chain of previous samples
		if (kal[prevSampR1] > 1.0) kal[prevSampR1] = 1.0; if (kal[prevSampR1] < -1.0) kal[prevSampR1] = -1.0;
		double bassR = kal[kalOutR]*0.777;
		midR -= bassR;
		//end KalmanR
		
		//begin ResEQ2 Mid Boost
		mpc++; if (mpc < 1 || mpc > 2001) mpc = 1;
		mpkL[mpc] = midL;
		mpkR[mpc] = midR;
		double midPeakL = 0.0;
		double midPeakR = 0.0;
		for (int x = 0; x < maxMPeak; x++) {
			int y = x*cycleEnd;
			switch (cycleEnd)
			{
				case 1: 
					midPeakL += (mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x]);
					midPeakR += (mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x]); break;
				case 2: 
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5); break;
				case 3: 
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); break;
				case 4: 
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); //break
			}
		}//end ResEQ2 Mid Boost creating
		
		inputSampleL = ((bassL*bassGain) + (midL*midGain) + (midPeakL*mPeak) + (trebleL*trebleGain)) * gainL * gain;
		inputSampleR = ((bassR*bassGain) + (midR*midGain) + (midPeakR*mPeak) + (trebleR*trebleGain)) * gainR * gain;
		//applies pan section, and smoothed fader gain
		
		inputSampleL *= topdB;
		if (inputSampleL < -0.222) inputSampleL = -0.222; if (inputSampleL > 0.222) inputSampleL = 0.222;
		dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		int dBdly = floor(dBaPosL*dscBuf);
		double dBi = (dBaPosL*dscBuf)-dBdly;
		inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
		inputSampleL /= topdB;		
		inputSampleR *= topdB;
		if (inputSampleR < -0.222) inputSampleR = -0.222; if (inputSampleR > 0.222) inputSampleR = 0.222;
		dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBaPosR*dscBuf);
		dBi = (dBaPosR*dscBuf)-dBdly;
		inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
		inputSampleR /= topdB;		
		//top dB processing for distributed discontinuity modeling air nonlinearity
		
		inputSampleL *= 0.618033988749895;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 1.618033988749895));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 1.618033988749895));
		
		inputSampleR *= 0.618033988749895;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 1.618033988749895));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 1.618033988749895));
		
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

void ConsoleXChannel::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 3) cycleEnd = 3;
	
	biquad[biq_freq] = 25000.0/getSampleRate();	
    biquad[biq_reso] = 0.50979558;
	double K = tan(M_PI * biquad[biq_freq]);
	double norm = 1.0 / (1.0 + K / biquad[biq_reso] + K * K);
	biquad[biq_a0] = K * K * norm;
	biquad[biq_a1] = 2.0 * biquad[biq_a0];
	biquad[biq_a2] = biquad[biq_a0];
	biquad[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	biquad[biq_b2] = (1.0 - K / biquad[biq_reso] + K * K) * norm;
	//ultrasonic nonlinear filter
	
	trebleGainA = trebleGainB; trebleGainB = A*2.0;
	midGainA = midGainB; midGainB = B*2.0;
	bassGainA = bassGainB; bassGainB = C*2.0;
	//simple three band to adjust
	
	//begin ResEQ2 Mid Boost
	double freqMPeak = pow(D+0.16,3);
	mPeakA = mPeakB; mPeakB = fabs(midGainB-1.0); //amount of mid peak leak through (or boost)
	if (midGainB < 1.0) mPeakB *= 0.5;
	int maxMPeak = (24.0*(2.0-freqMPeak))+16;
	if ((freqMPeak != prevfreqMPeak)||(mPeakB != prevamountMPeak)) {
		for (int x = 0; x < maxMPeak; x++) {
			if (((double)x*freqMPeak) < M_PI_4) f[x] = sin(((double)x*freqMPeak)*4.0)*freqMPeak*sin(((double)(maxMPeak-x)/(double)maxMPeak)*M_PI_2);
			else f[x] = cos((double)x*freqMPeak)*freqMPeak*sin(((double)(maxMPeak-x)/(double)maxMPeak)*M_PI_2);
		}
		prevfreqMPeak = freqMPeak; prevamountMPeak = mPeakB;
	}//end ResEQ2 Mid Boost
	//mid peak for either retaining during mid cut, or adding during mid boost
	
	double kalman = 1.0-pow(E,2);
	//crossover frequency between mid/bass
	
	double refdB = (F*70.0)+70.0;
	double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale;
	
	panA = panB; panB = G*1.57079633;
	inTrimA = inTrimB; inTrimB = H*2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (biquad[biq_freq] < 0.5) {
			double nlBiq = fabs(biquad[biq_a0]*(1.0+(inputSampleL*0.25))); if (nlBiq > 1.0) nlBiq = 1.0;
			double tmp = (inputSampleL * nlBiq) + biquad[biq_sL1];
			biquad[biq_sL1] = (inputSampleL * biquad[biq_a1]) - (tmp * biquad[biq_b1]) + biquad[biq_sL2];
			biquad[biq_sL2] = (inputSampleL * nlBiq) - (tmp * biquad[biq_b2]);
			inputSampleL = tmp;
			nlBiq = fabs(biquad[biq_a0]*(1.0+(inputSampleR*0.25))); if (nlBiq > 1.0) nlBiq = 1.0;
			tmp = (inputSampleR * nlBiq) + biquad[biq_sR1];
			biquad[biq_sR1] = (inputSampleR * biquad[biq_a1]) - (tmp * biquad[biq_b1]) + biquad[biq_sR2];
			biquad[biq_sR2] = (inputSampleR * nlBiq) - (tmp * biquad[biq_b2]);
			inputSampleR = tmp;
			//ultrasonic filter before anything else is done
		}
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double trebleGain = (trebleGainA*temp)+(trebleGainB*(1.0-temp));
		if (trebleGain > 1.0) trebleGain = pow(trebleGain,3.0+sqrt(overallscale));
		if (trebleGain < 1.0) trebleGain = 1.0-pow(1.0-trebleGain,2);
		
		double midGain = (midGainA*temp)+(midGainB*(1.0-temp));
		if (midGain > 1.0) midGain = 1.0;
		if (midGain < 1.0) midGain = 1.0-pow(1.0-midGain,2);
		double mPeak = pow((mPeakA*temp)+(mPeakB*(1.0-temp)),2);
		
		double bassGain = (bassGainA*temp)+(bassGainB*(1.0-temp));
		if (bassGain > 1.0) bassGain *= bassGain;
		if (bassGain < 1.0) bassGain = 1.0-pow(1.0-bassGain,2);
		
		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain;
		if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
		gain *= 1.527864045000421;
		
		//begin Air3L
		air[pvSL4] = air[pvAL4] - air[pvAL3]; air[pvSL3] = air[pvAL3] - air[pvAL2];
		air[pvSL2] = air[pvAL2] - air[pvAL1]; air[pvSL1] = air[pvAL1] - inputSampleL;
		air[accSL3] = air[pvSL4] - air[pvSL3]; air[accSL2] = air[pvSL3] - air[pvSL2];
		air[accSL1] = air[pvSL2] - air[pvSL1];
		air[acc2SL2] = air[accSL3] - air[accSL2]; air[acc2SL1] = air[accSL2] - air[accSL1];		
		air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
		air[gainAL] *= 0.5; air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
		if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
		air[pvAL4] = air[pvAL3]; air[pvAL3] = air[pvAL2];
		air[pvAL2] = air[pvAL1]; air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
		double midL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		temp = (midL + air[gndavgL])*0.5; air[gndavgL] = midL; midL = temp;
		double trebleL = drySampleL-midL;
		inputSampleL = midL;
		//end Air3L
		
		//begin Air3R
		air[pvSR4] = air[pvAR4] - air[pvAR3]; air[pvSR3] = air[pvAR3] - air[pvAR2];
		air[pvSR2] = air[pvAR2] - air[pvAR1]; air[pvSR1] = air[pvAR1] - inputSampleR;
		air[accSR3] = air[pvSR4] - air[pvSR3]; air[accSR2] = air[pvSR3] - air[pvSR2];
		air[accSR1] = air[pvSR2] - air[pvSR1];
		air[acc2SR2] = air[accSR3] - air[accSR2]; air[acc2SR1] = air[accSR2] - air[accSR1];		
		air[outAR] = -(air[pvAR1] + air[pvSR3] + air[acc2SR2] - ((air[acc2SR2] + air[acc2SR1])*0.5));
		air[gainAR] *= 0.5; air[gainAR] += fabs(drySampleR-air[outAR])*0.5;
		if (air[gainAR] > 0.3*sqrt(overallscale)) air[gainAR] = 0.3*sqrt(overallscale);
		air[pvAR4] = air[pvAR3]; air[pvAR3] = air[pvAR2];
		air[pvAR2] = air[pvAR1]; air[pvAR1] = (air[gainAR] * air[outAR]) + drySampleR;
		double midR = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
		temp = (midR + air[gndavgR])*0.5; air[gndavgR] = midR; midR = temp;
		double trebleR = drySampleR-midR;
		inputSampleR = midR;
		//end Air3R
		
		//begin KalmanL
		temp = inputSampleL = inputSampleL*(1.0-kalman)*0.777;
		inputSampleL *= (1.0-kalman);
		//set up gain levels to control the beast
		kal[prevSlewL3] += kal[prevSampL3] - kal[prevSampL2]; kal[prevSlewL3] *= 0.5;
		kal[prevSlewL2] += kal[prevSampL2] - kal[prevSampL1]; kal[prevSlewL2] *= 0.5;
		kal[prevSlewL1] += kal[prevSampL1] - inputSampleL; kal[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kal[accSlewL2] += kal[prevSlewL3] - kal[prevSlewL2]; kal[accSlewL2] *= 0.5;
		kal[accSlewL1] += kal[prevSlewL2] - kal[prevSlewL1]; kal[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kal[accSlewL3] += (kal[accSlewL2] - kal[accSlewL1]); kal[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kal[kalOutL] += kal[prevSampL1] + kal[prevSlewL2] + kal[accSlewL3]; kal[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kal[kalGainL] += fabs(temp-kal[kalOutL])*kalman*8.0; kal[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kal[kalGainL] > kalman*0.5) kal[kalGainL] = kalman*0.5;
		//attempts to avoid explosions
		kal[kalOutL] += (temp*(1.0-(0.68+(kalman*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kal[prevSampL3] = kal[prevSampL2]; kal[prevSampL2] = kal[prevSampL1];
		kal[prevSampL1] = (kal[kalGainL] * kal[kalOutL]) + ((1.0-kal[kalGainL])*temp);
		//feed the chain of previous samples
		if (kal[prevSampL1] > 1.0) kal[prevSampL1] = 1.0; if (kal[prevSampL1] < -1.0) kal[prevSampL1] = -1.0;
		double bassL = kal[kalOutL]*0.777;
		midL -= bassL;
		//end KalmanL
		
		//begin KalmanR
		temp = inputSampleR = inputSampleR*(1.0-kalman)*0.777;
		inputSampleR *= (1.0-kalman);
		//set up gain levels to control the beast
		kal[prevSlewR3] += kal[prevSampR3] - kal[prevSampR2]; kal[prevSlewR3] *= 0.5;
		kal[prevSlewR2] += kal[prevSampR2] - kal[prevSampR1]; kal[prevSlewR2] *= 0.5;
		kal[prevSlewR1] += kal[prevSampR1] - inputSampleR; kal[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kal[accSlewR2] += kal[prevSlewR3] - kal[prevSlewR2]; kal[accSlewR2] *= 0.5;
		kal[accSlewR1] += kal[prevSlewR2] - kal[prevSlewR1]; kal[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kal[accSlewR3] += (kal[accSlewR2] - kal[accSlewR1]); kal[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kal[kalOutR] += kal[prevSampR1] + kal[prevSlewR2] + kal[accSlewR3]; kal[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kal[kalGainR] += fabs(temp-kal[kalOutR])*kalman*8.0; kal[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kal[kalGainR] > kalman*0.5) kal[kalGainR] = kalman*0.5;
		//attempts to avoid explosions
		kal[kalOutR] += (temp*(1.0-(0.68+(kalman*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kal[prevSampR3] = kal[prevSampR2]; kal[prevSampR2] = kal[prevSampR1];
		kal[prevSampR1] = (kal[kalGainR] * kal[kalOutR]) + ((1.0-kal[kalGainR])*temp);
		//feed the chain of previous samples
		if (kal[prevSampR1] > 1.0) kal[prevSampR1] = 1.0; if (kal[prevSampR1] < -1.0) kal[prevSampR1] = -1.0;
		double bassR = kal[kalOutR]*0.777;
		midR -= bassR;
		//end KalmanR
		
		//begin ResEQ2 Mid Boost
		mpc++; if (mpc < 1 || mpc > 2001) mpc = 1;
		mpkL[mpc] = midL;
		mpkR[mpc] = midR;
		double midPeakL = 0.0;
		double midPeakR = 0.0;
		for (int x = 0; x < maxMPeak; x++) {
			int y = x*cycleEnd;
			switch (cycleEnd)
			{
				case 1: 
					midPeakL += (mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x]);
					midPeakR += (mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x]); break;
				case 2: 
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5); break;
				case 3: 
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); break;
				case 4: 
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); //break
			}
		}//end ResEQ2 Mid Boost creating
		
		inputSampleL = ((bassL*bassGain) + (midL*midGain) + (midPeakL*mPeak) + (trebleL*trebleGain)) * gainL * gain;
		inputSampleR = ((bassR*bassGain) + (midR*midGain) + (midPeakR*mPeak) + (trebleR*trebleGain)) * gainR * gain;
		//applies pan section, and smoothed fader gain
		
		inputSampleL *= topdB;
		if (inputSampleL < -0.222) inputSampleL = -0.222; if (inputSampleL > 0.222) inputSampleL = 0.222;
		dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		int dBdly = floor(dBaPosL*dscBuf);
		double dBi = (dBaPosL*dscBuf)-dBdly;
		inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
		inputSampleL /= topdB;		
		inputSampleR *= topdB;
		if (inputSampleR < -0.222) inputSampleR = -0.222; if (inputSampleR > 0.222) inputSampleR = 0.222;
		dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBaPosR*dscBuf);
		dBi = (dBaPosR*dscBuf)-dBdly;
		inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
		inputSampleR /= topdB;		
		//top dB processing for distributed discontinuity modeling air nonlinearity
		
		inputSampleL *= 0.618033988749895;
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 1.618033988749895));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 1.618033988749895));
		
		inputSampleR *= 0.618033988749895;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 1.618033988749895));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 1.618033988749895));
		
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
