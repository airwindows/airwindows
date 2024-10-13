/* ========================================
 *  Mastering - Mastering.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Mastering_H
#include "Mastering.h"
#endif

void Mastering::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	long double trebleGain = A+0.5;
	if (trebleGain > 1.0) trebleGain = pow(trebleGain,3.0+sqrt(overallscale));
	//this boost is necessary to adapt to higher sample rates
	long double midGain = B+0.5;
	long double bassGain = (1.0-C)+0.5;
	long double subGain = D+0.5;
	//simple four band to adjust
	double kalMid = pow(1.0-E,3);
	//crossover frequency between mid/bass
	double kalSub = (1.0-(pow(F,3)));
	//crossover frequency between bass/sub
	double zoom = (G*2.0)-1.0;
	double zoomStages = (fabs(zoom)*4.0)+1.0;
	for (int count = 0; count < sqrt(zoomStages); count++) zoom *= fabs(zoom);
	double threshSinew = pow(H,2)/overallscale;
	double depthSinew = I;
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
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
		long double midL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		long double temp = (midL + air[gndavgL])*0.5; air[gndavgL] = midL; midL = temp;
		long double trebleL = drySampleL-midL;
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
		long double midR = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
		temp = (midR + air[gndavgR])*0.5; air[gndavgR] = midR; midR = temp;
		long double trebleR = drySampleR-midR;
		//end Air3R
		
		//begin KalmanML
		temp = midL;
		kalM[prevSlewL3] += kalM[prevSampL3] - kalM[prevSampL2]; kalM[prevSlewL3] *= 0.5;
		kalM[prevSlewL2] += kalM[prevSampL2] - kalM[prevSampL1]; kalM[prevSlewL2] *= 0.5;
		kalM[prevSlewL1] += kalM[prevSampL1] - midL; kalM[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kalM[accSlewL2] += kalM[prevSlewL3] - kalM[prevSlewL2]; kalM[accSlewL2] *= 0.5;
		kalM[accSlewL1] += kalM[prevSlewL2] - kalM[prevSlewL1]; kalM[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalM[accSlewL3] += (kalM[accSlewL2] - kalM[accSlewL1]); kalM[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kalM[kalOutL] += kalM[prevSampL1] + kalM[prevSlewL2] + kalM[accSlewL3]; kalM[kalOutL] *= 0.5;
		
		//resynthesizing predicted result (all iir smoothed)
		kalM[kalGainL] += fabs(temp-kalM[kalOutL])*kalMid*8.0; kalM[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalM[kalGainL] > kalMid*0.5) kalM[kalGainL] = kalMid*0.5;
		//attempts to avoid explosions
		kalM[kalOutL] += (temp*(1.0-(0.68+(kalMid*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalM[prevSampL3] = kalM[prevSampL2]; kalM[prevSampL2] = kalM[prevSampL1];
		kalM[prevSampL1] = (kalM[kalGainL] * kalM[kalOutL]) + ((1.0-kalM[kalGainL])*temp);
		//feed the chain of previous samples
		long double bassL = (kalM[kalOutL]+kalM[kalAvgL])*0.5;
		kalM[kalAvgL] = kalM[kalOutL];
		midL -= bassL;
		//end KalmanML
		
		//begin KalmanMR
		temp = midR;
		kalM[prevSlewR3] += kalM[prevSampR3] - kalM[prevSampR2]; kalM[prevSlewR3] *= 0.5;
		kalM[prevSlewR2] += kalM[prevSampR2] - kalM[prevSampR1]; kalM[prevSlewR2] *= 0.5;
		kalM[prevSlewR1] += kalM[prevSampR1] - midR; kalM[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kalM[accSlewR2] += kalM[prevSlewR3] - kalM[prevSlewR2]; kalM[accSlewR2] *= 0.5;
		kalM[accSlewR1] += kalM[prevSlewR2] - kalM[prevSlewR1]; kalM[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalM[accSlewR3] += (kalM[accSlewR2] - kalM[accSlewR1]); kalM[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kalM[kalOutR] += kalM[prevSampR1] + kalM[prevSlewR2] + kalM[accSlewR3]; kalM[kalOutR] *= 0.5;
		
		//resynthesizing predicted result (all iir smoothed)
		kalM[kalGainR] += fabs(temp-kalM[kalOutR])*kalMid*8.0; kalM[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalM[kalGainR] > kalMid*0.5) kalM[kalGainR] = kalMid*0.5;
		//attempts to avoid explosions
		kalM[kalOutR] += (temp*(1.0-(0.68+(kalMid*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalM[prevSampR3] = kalM[prevSampR2]; kalM[prevSampR2] = kalM[prevSampR1];
		kalM[prevSampR1] = (kalM[kalGainR] * kalM[kalOutR]) + ((1.0-kalM[kalGainR])*temp);
		//feed the chain of previous samples
		long double bassR = (kalM[kalOutR]+kalM[kalAvgR])*0.5;
		kalM[kalAvgR] = kalM[kalOutR];
		midR -= bassR;
		//end KalmanMR
		
		//begin KalmanSL
		temp = bassL;
		kalS[prevSlewL3] += kalS[prevSampL3] - kalS[prevSampL2]; kalS[prevSlewL3] *= 0.5;
		kalS[prevSlewL2] += kalS[prevSampL2] - kalS[prevSampL1]; kalS[prevSlewL2] *= 0.5;
		kalS[prevSlewL1] += kalS[prevSampL1] - bassL; kalS[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kalS[accSlewL2] += kalS[prevSlewL3] - kalS[prevSlewL2]; kalS[accSlewL2] *= 0.5;
		kalS[accSlewL1] += kalS[prevSlewL2] - kalS[prevSlewL1]; kalS[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalS[accSlewL3] += (kalS[accSlewL2] - kalS[accSlewL1]); kalS[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kalS[kalOutL] += kalS[prevSampL1] + kalS[prevSlewL2] + kalS[accSlewL3]; kalS[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kalS[kalGainL] += fabs(temp-kalS[kalOutL])*kalSub*8.0; kalS[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalS[kalGainL] > kalSub*0.5) kalS[kalGainL] = kalSub*0.5;
		//attempts to avoid explosions
		kalS[kalOutL] += (temp*(1.0-(0.68+(kalSub*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalS[prevSampL3] = kalS[prevSampL2]; kalS[prevSampL2] = kalS[prevSampL1];
		kalS[prevSampL1] = (kalS[kalGainL] * kalS[kalOutL]) + ((1.0-kalS[kalGainL])*temp);
		//feed the chain of previous samples
		long double subL = (kalS[kalOutL]+kalS[kalAvgL])*0.5;
		kalS[kalAvgL] = kalS[kalOutL];
		bassL -= subL;
		//end KalmanSL

		//begin KalmanSR
		temp = bassR;
		kalS[prevSlewR3] += kalS[prevSampR3] - kalS[prevSampR2]; kalS[prevSlewR3] *= 0.5;
		kalS[prevSlewR2] += kalS[prevSampR2] - kalS[prevSampR1]; kalS[prevSlewR2] *= 0.5;
		kalS[prevSlewR1] += kalS[prevSampR1] - bassR; kalS[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kalS[accSlewR2] += kalS[prevSlewR3] - kalS[prevSlewR2]; kalS[accSlewR2] *= 0.5;
		kalS[accSlewR1] += kalS[prevSlewR2] - kalS[prevSlewR1]; kalS[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalS[accSlewR3] += (kalS[accSlewR2] - kalS[accSlewR1]); kalS[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kalS[kalOutR] += kalS[prevSampR1] + kalS[prevSlewR2] + kalS[accSlewR3]; kalS[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kalS[kalGainR] += fabs(temp-kalS[kalOutR])*kalSub*8.0; kalS[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalS[kalGainR] > kalSub*0.5) kalS[kalGainR] = kalSub*0.5;
		//attempts to avoid explosions
		kalS[kalOutR] += (temp*(1.0-(0.68+(kalSub*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalS[prevSampR3] = kalS[prevSampR2]; kalS[prevSampR2] = kalS[prevSampR1];
		kalS[prevSampR1] = (kalS[kalGainR] * kalS[kalOutR]) + ((1.0-kalS[kalGainR])*temp);
		//feed the chain of previous samples
		long double subR = (kalS[kalOutR]+kalS[kalAvgR])*0.5;
		kalS[kalAvgR] = kalS[kalOutR];
		bassR -= subR;
		//end KalmanSR
		
		inputSampleL = (subL*subGain);
		inputSampleL += (bassL*bassGain);
		inputSampleL += (midL*midGain);
		inputSampleL += (trebleL*trebleGain);
		inputSampleR = (subR*subGain);
		inputSampleR += (bassR*bassGain);
		inputSampleR += (midR*midGain);
		inputSampleR += (trebleR*trebleGain);
		
		for (int count = 0; count < zoomStages; count++) {
			if (zoom > 0.0) {
				double closer = inputSampleL * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleL = (inputSampleL*(1.0-zoom))+(sin(closer)*zoom);
				closer = inputSampleR * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleR = (inputSampleR*(1.0-zoom))+(sin(closer)*zoom);
			} //zooming in will make the body of the sound louder: it's just Density
			if (zoom < 0.0) {
				double farther = fabs(inputSampleL) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleL < 0.0) inputSampleL = (inputSampleL*(1.0+zoom))+(farther*zoom*1.57079633);			
				farther = fabs(inputSampleR) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleR < 0.0) inputSampleR = (inputSampleR*(1.0+zoom))+(farther*zoom*1.57079633);			
			} //zooming out boosts the hottest peaks but cuts back softer stuff
		}
		
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
		
		temp = inputSampleL;
		long double sinew = threshSinew * cos(lastSinewL*lastSinewL);
		if (inputSampleL - lastSinewL > sinew) temp = lastSinewL + sinew;
		if (-(inputSampleL - lastSinewL) > sinew) temp = lastSinewL - sinew;
		lastSinewL = temp;
		inputSampleL = (inputSampleL * (1.0-depthSinew))+(lastSinewL*depthSinew);
		temp = inputSampleR;
		sinew = threshSinew * cos(lastSinewR*lastSinewR);
		if (inputSampleR - lastSinewR > sinew) temp = lastSinewR + sinew;
		if (-(inputSampleR - lastSinewR) > sinew) temp = lastSinewR - sinew;
		lastSinewR = temp;
		inputSampleR = (inputSampleR * (1.0-depthSinew))+(lastSinewR*depthSinew);
		//run Sinew to stop excess slews, but run a dry/wet to allow a range of brights
		
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

void Mastering::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	long double trebleGain = A+0.5;
	if (trebleGain > 1.0) trebleGain = pow(trebleGain,3.0+sqrt(overallscale));
	//this boost is necessary to adapt to higher sample rates
	long double midGain = B+0.5;
	long double bassGain = (1.0-C)+0.5;
	long double subGain = D+0.5;
	//simple four band to adjust
	double kalMid = pow(1.0-E,3);
	//crossover frequency between mid/bass
	double kalSub = (1.0-(pow(F,3)));
	//crossover frequency between bass/sub
	double zoom = (G*2.0)-1.0;
	double zoomStages = (fabs(zoom)*4.0)+1.0;
	for (int count = 0; count < sqrt(zoomStages); count++) zoom *= fabs(zoom);
	double threshSinew = pow(H,2)/overallscale;
	double depthSinew = I;
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
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
		long double midL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		long double temp = (midL + air[gndavgL])*0.5; air[gndavgL] = midL; midL = temp;
		long double trebleL = drySampleL-midL;
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
		long double midR = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
		temp = (midR + air[gndavgR])*0.5; air[gndavgR] = midR; midR = temp;
		long double trebleR = drySampleR-midR;
		//end Air3R
		
		//begin KalmanML
		temp = midL;
		kalM[prevSlewL3] += kalM[prevSampL3] - kalM[prevSampL2]; kalM[prevSlewL3] *= 0.5;
		kalM[prevSlewL2] += kalM[prevSampL2] - kalM[prevSampL1]; kalM[prevSlewL2] *= 0.5;
		kalM[prevSlewL1] += kalM[prevSampL1] - midL; kalM[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kalM[accSlewL2] += kalM[prevSlewL3] - kalM[prevSlewL2]; kalM[accSlewL2] *= 0.5;
		kalM[accSlewL1] += kalM[prevSlewL2] - kalM[prevSlewL1]; kalM[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalM[accSlewL3] += (kalM[accSlewL2] - kalM[accSlewL1]); kalM[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kalM[kalOutL] += kalM[prevSampL1] + kalM[prevSlewL2] + kalM[accSlewL3]; kalM[kalOutL] *= 0.5;
		
		//resynthesizing predicted result (all iir smoothed)
		kalM[kalGainL] += fabs(temp-kalM[kalOutL])*kalMid*8.0; kalM[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalM[kalGainL] > kalMid*0.5) kalM[kalGainL] = kalMid*0.5;
		//attempts to avoid explosions
		kalM[kalOutL] += (temp*(1.0-(0.68+(kalMid*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalM[prevSampL3] = kalM[prevSampL2]; kalM[prevSampL2] = kalM[prevSampL1];
		kalM[prevSampL1] = (kalM[kalGainL] * kalM[kalOutL]) + ((1.0-kalM[kalGainL])*temp);
		//feed the chain of previous samples
		long double bassL = (kalM[kalOutL]+kalM[kalAvgL])*0.5;
		kalM[kalAvgL] = kalM[kalOutL];
		midL -= bassL;
		//end KalmanML
		
		//begin KalmanMR
		temp = midR;
		kalM[prevSlewR3] += kalM[prevSampR3] - kalM[prevSampR2]; kalM[prevSlewR3] *= 0.5;
		kalM[prevSlewR2] += kalM[prevSampR2] - kalM[prevSampR1]; kalM[prevSlewR2] *= 0.5;
		kalM[prevSlewR1] += kalM[prevSampR1] - midR; kalM[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kalM[accSlewR2] += kalM[prevSlewR3] - kalM[prevSlewR2]; kalM[accSlewR2] *= 0.5;
		kalM[accSlewR1] += kalM[prevSlewR2] - kalM[prevSlewR1]; kalM[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalM[accSlewR3] += (kalM[accSlewR2] - kalM[accSlewR1]); kalM[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kalM[kalOutR] += kalM[prevSampR1] + kalM[prevSlewR2] + kalM[accSlewR3]; kalM[kalOutR] *= 0.5;
		
		//resynthesizing predicted result (all iir smoothed)
		kalM[kalGainR] += fabs(temp-kalM[kalOutR])*kalMid*8.0; kalM[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalM[kalGainR] > kalMid*0.5) kalM[kalGainR] = kalMid*0.5;
		//attempts to avoid explosions
		kalM[kalOutR] += (temp*(1.0-(0.68+(kalMid*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalM[prevSampR3] = kalM[prevSampR2]; kalM[prevSampR2] = kalM[prevSampR1];
		kalM[prevSampR1] = (kalM[kalGainR] * kalM[kalOutR]) + ((1.0-kalM[kalGainR])*temp);
		//feed the chain of previous samples
		long double bassR = (kalM[kalOutR]+kalM[kalAvgR])*0.5;
		kalM[kalAvgR] = kalM[kalOutR];
		midR -= bassR;
		//end KalmanMR
		
		//begin KalmanSL
		temp = bassL;
		kalS[prevSlewL3] += kalS[prevSampL3] - kalS[prevSampL2]; kalS[prevSlewL3] *= 0.5;
		kalS[prevSlewL2] += kalS[prevSampL2] - kalS[prevSampL1]; kalS[prevSlewL2] *= 0.5;
		kalS[prevSlewL1] += kalS[prevSampL1] - bassL; kalS[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kalS[accSlewL2] += kalS[prevSlewL3] - kalS[prevSlewL2]; kalS[accSlewL2] *= 0.5;
		kalS[accSlewL1] += kalS[prevSlewL2] - kalS[prevSlewL1]; kalS[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalS[accSlewL3] += (kalS[accSlewL2] - kalS[accSlewL1]); kalS[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kalS[kalOutL] += kalS[prevSampL1] + kalS[prevSlewL2] + kalS[accSlewL3]; kalS[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kalS[kalGainL] += fabs(temp-kalS[kalOutL])*kalSub*8.0; kalS[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalS[kalGainL] > kalSub*0.5) kalS[kalGainL] = kalSub*0.5;
		//attempts to avoid explosions
		kalS[kalOutL] += (temp*(1.0-(0.68+(kalSub*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalS[prevSampL3] = kalS[prevSampL2]; kalS[prevSampL2] = kalS[prevSampL1];
		kalS[prevSampL1] = (kalS[kalGainL] * kalS[kalOutL]) + ((1.0-kalS[kalGainL])*temp);
		//feed the chain of previous samples
		long double subL = (kalS[kalOutL]+kalS[kalAvgL])*0.5;
		kalS[kalAvgL] = kalS[kalOutL];
		bassL -= subL;
		//end KalmanSL
		
		//begin KalmanSR
		temp = bassR;
		kalS[prevSlewR3] += kalS[prevSampR3] - kalS[prevSampR2]; kalS[prevSlewR3] *= 0.5;
		kalS[prevSlewR2] += kalS[prevSampR2] - kalS[prevSampR1]; kalS[prevSlewR2] *= 0.5;
		kalS[prevSlewR1] += kalS[prevSampR1] - bassR; kalS[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kalS[accSlewR2] += kalS[prevSlewR3] - kalS[prevSlewR2]; kalS[accSlewR2] *= 0.5;
		kalS[accSlewR1] += kalS[prevSlewR2] - kalS[prevSlewR1]; kalS[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalS[accSlewR3] += (kalS[accSlewR2] - kalS[accSlewR1]); kalS[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kalS[kalOutR] += kalS[prevSampR1] + kalS[prevSlewR2] + kalS[accSlewR3]; kalS[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kalS[kalGainR] += fabs(temp-kalS[kalOutR])*kalSub*8.0; kalS[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalS[kalGainR] > kalSub*0.5) kalS[kalGainR] = kalSub*0.5;
		//attempts to avoid explosions
		kalS[kalOutR] += (temp*(1.0-(0.68+(kalSub*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalS[prevSampR3] = kalS[prevSampR2]; kalS[prevSampR2] = kalS[prevSampR1];
		kalS[prevSampR1] = (kalS[kalGainR] * kalS[kalOutR]) + ((1.0-kalS[kalGainR])*temp);
		//feed the chain of previous samples
		long double subR = (kalS[kalOutR]+kalS[kalAvgR])*0.5;
		kalS[kalAvgR] = kalS[kalOutR];
		bassR -= subR;
		//end KalmanSR
		
		inputSampleL = (subL*subGain);
		inputSampleL += (bassL*bassGain);
		inputSampleL += (midL*midGain);
		inputSampleL += (trebleL*trebleGain);
		inputSampleR = (subR*subGain);
		inputSampleR += (bassR*bassGain);
		inputSampleR += (midR*midGain);
		inputSampleR += (trebleR*trebleGain);
		
		for (int count = 0; count < zoomStages; count++) {
			if (zoom > 0.0) {
				double closer = inputSampleL * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleL = (inputSampleL*(1.0-zoom))+(sin(closer)*zoom);
				closer = inputSampleR * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleR = (inputSampleR*(1.0-zoom))+(sin(closer)*zoom);
			} //zooming in will make the body of the sound louder: it's just Density
			if (zoom < 0.0) {
				double farther = fabs(inputSampleL) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleL < 0.0) inputSampleL = (inputSampleL*(1.0+zoom))+(farther*zoom*1.57079633);			
				farther = fabs(inputSampleR) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleR < 0.0) inputSampleR = (inputSampleR*(1.0+zoom))+(farther*zoom*1.57079633);			
			} //zooming out boosts the hottest peaks but cuts back softer stuff
		}
		
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
		
		temp = inputSampleL;
		long double sinew = threshSinew * cos(lastSinewL*lastSinewL);
		if (inputSampleL - lastSinewL > sinew) temp = lastSinewL + sinew;
		if (-(inputSampleL - lastSinewL) > sinew) temp = lastSinewL - sinew;
		lastSinewL = temp;
		inputSampleL = (inputSampleL * (1.0-depthSinew))+(lastSinewL*depthSinew);
		temp = inputSampleR;
		sinew = threshSinew * cos(lastSinewR*lastSinewR);
		if (inputSampleR - lastSinewR > sinew) temp = lastSinewR + sinew;
		if (-(inputSampleR - lastSinewR) > sinew) temp = lastSinewR - sinew;
		lastSinewR = temp;
		inputSampleR = (inputSampleR * (1.0-depthSinew))+(lastSinewR*depthSinew);
		//run Sinew to stop excess slews, but run a dry/wet to allow a range of brights
		
		//begin 64 bit stereo floating point dither
		int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
