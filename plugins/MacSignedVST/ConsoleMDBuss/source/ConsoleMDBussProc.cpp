/* ========================================
 *  ConsoleMDBuss - ConsoleMDBuss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleMDBuss_H
#include "ConsoleMDBuss.h"
#endif

void ConsoleMDBuss::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
		
	gainA = gainB;
	gainB = sqrt(A); //smoothed master fader from Z2 filters
	//this will be applied three times: this is to make the various tone alterations
	//hit differently at different master fader drive levels.
	//in particular, backing off the master fader tightens the super lows
	//but opens up the modified Sinew, because more of the attentuation happens before
	//you even get to slew clipping :) and if the fader is not active, it bypasses completely.
	
	double threshSinew = 0.5171104/overallscale;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double gain = (gainA*temp)+(gainB*(1.0-temp));
		//setting up smoothed master fader
				
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//subtight is always fully engaged: tighten response when restraining full console
		
		//begin Console7Buss which is the one we choose for ConsoleMC
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = ((asin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.618033988749894848204586)+(asin(inputSampleL)*0.381966011250105);
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = ((asin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.618033988749894848204586)+(asin(inputSampleR)*0.381966011250105);
		//this is an asin version of Spiral blended with regular asin ConsoleBuss.
		//It's blending between two different harmonics in the overtones of the algorithm
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//after C7Buss but before EverySlew: allow highs to come out a bit more
		//when pulling back master fader. Less drive equals more open
		
		temp = inputSampleL;
		double clamp = inputSampleL - lastSinewL;
		if (lastSinewL > 1.0) lastSinewL = 1.0;
		if (lastSinewL < -1.0) lastSinewL = -1.0;
		double sinew = threshSinew * cos(lastSinewL);
		if (clamp > sinew) temp = lastSinewL + sinew;
		if (-clamp > sinew) temp = lastSinewL - sinew;
		inputSampleL = lastSinewL = temp;
		temp = inputSampleR;
		clamp = inputSampleR - lastSinewR;
		if (lastSinewR > 1.0) lastSinewR = 1.0;
		if (lastSinewR < -1.0) lastSinewR = -1.0;
		sinew = threshSinew * cos(lastSinewR);
		if (clamp > sinew) temp = lastSinewR + sinew;
		if (-clamp > sinew) temp = lastSinewR - sinew;
		inputSampleR = lastSinewR = temp;
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//after EverySlew fades the total output sound: least change in tone here.
				
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

void ConsoleMDBuss::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
		
	gainA = gainB;
	gainB = sqrt(A); //smoothed master fader from Z2 filters
	//this will be applied three times: this is to make the various tone alterations
	//hit differently at different master fader drive levels.
	//in particular, backing off the master fader tightens the super lows
	//but opens up the modified Sinew, because more of the attentuation happens before
	//you even get to slew clipping :) and if the fader is not active, it bypasses completely.
	
	double threshSinew = 0.5171104/overallscale;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double gain = (gainA*temp)+(gainB*(1.0-temp));
		//setting up smoothed master fader
				
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//subtight is always fully engaged: tighten response when restraining full console
		
		//begin Console7Buss which is the one we choose for ConsoleMC
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = ((asin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.618033988749894848204586)+(asin(inputSampleL)*0.381966011250105);
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = ((asin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.618033988749894848204586)+(asin(inputSampleR)*0.381966011250105);
		//this is an asin version of Spiral blended with regular asin ConsoleBuss.
		//It's blending between two different harmonics in the overtones of the algorithm
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//after C7Buss but before EverySlew: allow highs to come out a bit more
		//when pulling back master fader. Less drive equals more open
		
		temp = inputSampleL;
		double clamp = inputSampleL - lastSinewL;
		if (lastSinewL > 1.0) lastSinewL = 1.0;
		if (lastSinewL < -1.0) lastSinewL = -1.0;
		double sinew = threshSinew * cos(lastSinewL);
		if (clamp > sinew) temp = lastSinewL + sinew;
		if (-clamp > sinew) temp = lastSinewL - sinew;
		inputSampleL = lastSinewL = temp;
		temp = inputSampleR;
		clamp = inputSampleR - lastSinewR;
		if (lastSinewR > 1.0) lastSinewR = 1.0;
		if (lastSinewR < -1.0) lastSinewR = -1.0;
		sinew = threshSinew * cos(lastSinewR);
		if (clamp > sinew) temp = lastSinewR + sinew;
		if (-clamp > sinew) temp = lastSinewR - sinew;
		inputSampleR = lastSinewR = temp;
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//after EverySlew fades the total output sound: least change in tone here.
		
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
