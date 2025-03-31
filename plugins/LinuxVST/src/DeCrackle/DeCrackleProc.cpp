/* ========================================
 *  DeCrackle - DeCrackle.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeCrackle_H
#include "DeCrackle.h"
#endif

void DeCrackle::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int offset = (int)(overallscale*1.1);
	double maxHeight = 1.0*overallscale;
	double filterOut = pow((A*0.618)+0.1,2.0)/overallscale;
	double filterRef = pow(((1.0-B)*0.618)+0.1,2.0)/overallscale;
	double iirCut = (pow(1.0-B,2.0)*0.2)/overallscale;
	int adjDelay = (int)(B*((double)(kshort/8)*overallscale))-2.0;
	if (adjDelay > kshort - 1) adjDelay = kshort - 1; //don't break on silly samplerates
	int halfTrig = fmin(0.5+pow(B,3.0),0.999)*adjDelay;
	int halfRaw = 0.5*adjDelay;
	int halfBez = 0.5*adjDelay;
	double threshold = pow(C*0.618,2.0)-0.1;
	double surface = (1.0-pow(1.0-D,3.0))*0.9;
	double wet = E;
	int options = 0; if (wet < 0.001) options = 1;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL -= prevSampleL; prevSampleL = drySampleL;
		inputSampleR -= prevSampleR; prevSampleR = drySampleR;
		inputSampleL *= 16.0; inputSampleR *= 16.0;
		
		double bezL = drySampleL;
		for(int x = 0; x < 6; x++) {
			iirLAngle[x] = (iirLAngle[x]*(1.0-filterOut))+((bezL-iirLSample[x])*filterOut);
			bezL = ((iirLSample[x]+(iirLAngle[x]*filterOut))*(1.0-filterOut))+(bezL*filterOut);
			iirLSample[x] = ((iirLSample[x]+(iirLAngle[x]*filterOut))*(1.0-filterOut))+(bezL*filterOut);
		} //lowpass audio band
		double bezR = drySampleR;
		for(int x = 0; x < 6; x++) {
			iirRAngle[x] = (iirRAngle[x]*(1.0-filterOut))+((bezR-iirRSample[x])*filterOut);
			bezR = ((iirRSample[x]+(iirRAngle[x]*filterOut))*(1.0-filterOut))+(bezR*filterOut);
			iirRSample[x] = ((iirRSample[x]+(iirRAngle[x]*filterOut))*(1.0-filterOut))+(bezR*filterOut);
		} //lowpass audio band
		
		double rect = fabs(drySampleL*drySampleR*64.0);
		for(int x = 0; x < 6; x++) {
			rect = fabs((iirCSample[x]*(1.0-filterRef))+(rect*filterRef));
			iirCSample[x] = (iirCSample[x]*(1.0-filterRef))+(rect*filterRef);
		} //lowpass rectified CV band
		
		aAL[count] = drySampleL;
		aAR[count] = drySampleR;
		aBL[count] = bezL;
		aBR[count] = bezR;
		aCL[count] = rect;
		
		count++; if (count < 0 || count > adjDelay) count = 0;
		double near = rect;
		double far = aCL[count-((count > adjDelay)?adjDelay+1:0)];
		//near and far are the smoothed rectified version
		
		double prevL = aAL[(count+halfRaw+offset)-(((count+halfRaw+offset) > adjDelay)?adjDelay+1:0)];
		double prevR = aAR[(count+halfRaw+offset)-(((count+halfRaw+offset) > adjDelay)?adjDelay+1:0)];
		double outL = aAL[(count+halfRaw)-(((count+halfRaw) > adjDelay)?adjDelay+1:0)];
		double outR = aAR[(count+halfRaw)-(((count+halfRaw) > adjDelay)?adjDelay+1:0)];
		double outBezL = aBL[(count+halfBez)-(((count+halfBez) > adjDelay)?adjDelay+1:0)];
		double outBezR = aBR[(count+halfBez)-(((count+halfBez) > adjDelay)?adjDelay+1:0)];
		//these are the various outputs to fade between
		double trigL = aAL[(count+halfTrig)-(((count+halfTrig) > adjDelay)?adjDelay+1:0)];
		double trigR = aAR[(count+halfTrig)-(((count+halfTrig) > adjDelay)?adjDelay+1:0)];
		
		double deClickL = pow(fmax((fabs(trigL)-threshold)-fmax(near,far),0.0)*16.0,3.0);
		double deClickR = pow(fmax((fabs(trigR)-threshold)-fmax(near,far),0.0)*16.0,3.0);
		iirTargetL = fmax(iirTargetL-iirCut,0.0);
		iirTargetR = fmax(iirTargetR-iirCut,0.0); //taper down at iirCut speed
		if (deClickL > iirTargetL) iirTargetL = fmin(deClickL,maxHeight);
		if (deClickR > iirTargetR) iirTargetR = fmin(deClickR,maxHeight); //beyond 1.0 to stretch window
		if (deClickR*0.618 > iirTargetL) iirTargetL = fmin(deClickR*0.618,maxHeight);
		if (deClickL*0.618 > iirTargetR) iirTargetR = fmin(deClickL*0.618,maxHeight); //opposite channel
		iirClickL = fmin(iirClickL+iirCut,iirTargetL);
		iirClickR = fmin(iirClickR+iirCut,iirTargetR); //taper up on attack
		inputSampleL = (outBezL*fmin(iirClickL, 1.0)) + (outL*(1.0-fmin(iirClickL, 1.0)));
		inputSampleR = (outBezR*fmin(iirClickR, 1.0)) + (outR*(1.0-fmin(iirClickR, 1.0)));
		//declicking is applied: crossfade between darkened and normal
		
		if (wet < 1.0 && wet > 0.0) {
			inputSampleL = (inputSampleL*wet)+(outL*(1.0-wet));
			inputSampleR = (inputSampleR*wet)+(outR*(1.0-wet));
		} //neither full wet, nor the 'clicks only' special case below
		
		if (wet == 0.0) {
			inputSampleL = outL - inputSampleL;
			inputSampleR = outR - inputSampleR;
		} //clicks only at full dry
		
		double recordVolume = fmax(fmax(near,far),fmax(prevL,prevR))+0.001; //engage only at quietest
		double surfaceL = sin(fmin((fabs(outL-prevL)/recordVolume)*surface, 3.14159265358979))*0.5;
		double surfaceR = sin(fmin((fabs(outR-prevR)/recordVolume)*surface, 3.14159265358979))*0.5;
		double gateOnAudio = fmax(surface-(recordVolume*surface*4.0),0.0);
		if (surface > 0.0 && wet > 0.0) {
			inputSampleL = (prevOutL*surfaceL)+(inputSampleL*(1.0-surfaceL));
			inputSampleR = (prevOutR*surfaceR)+(inputSampleR*(1.0-surfaceR));
			inputSampleL = (prevOutL*gateOnAudio)+(inputSampleL*(1.0-gateOnAudio));
			inputSampleR = (prevOutR*gateOnAudio)+(inputSampleR*(1.0-gateOnAudio));
			prevOutL = (prevOutL*gateOnAudio)+(inputSampleL*(1.0-gateOnAudio));
			prevOutR = (prevOutR*gateOnAudio)+(inputSampleR*(1.0-gateOnAudio));
		} //if we're also adding surface noise reduction that goes here
		
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

void DeCrackle::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int offset = (int)(overallscale*1.1);
	double maxHeight = 1.0*overallscale;
	double filterOut = pow((A*0.618)+0.1,2.0)/overallscale;
	double filterRef = pow(((1.0-B)*0.618)+0.1,2.0)/overallscale;
	double iirCut = (pow(1.0-B,2.0)*0.2)/overallscale;
	int adjDelay = (int)(B*((double)(kshort/8)*overallscale))-2.0;
	if (adjDelay > kshort - 1) adjDelay = kshort - 1; //don't break on silly samplerates
	int halfTrig = fmin(0.5+pow(B,3.0),0.999)*adjDelay;
	int halfRaw = 0.5*adjDelay;
	int halfBez = 0.5*adjDelay;
	double threshold = pow(C*0.618,2.0)-0.1;
	double surface = (1.0-pow(1.0-D,3.0))*0.9;
	double wet = E;
	int options = 0; if (wet < 0.001) options = 1;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL -= prevSampleL; prevSampleL = drySampleL;
		inputSampleR -= prevSampleR; prevSampleR = drySampleR;
		inputSampleL *= 16.0; inputSampleR *= 16.0;
		
		double bezL = drySampleL;
		for(int x = 0; x < 6; x++) {
			iirLAngle[x] = (iirLAngle[x]*(1.0-filterOut))+((bezL-iirLSample[x])*filterOut);
			bezL = ((iirLSample[x]+(iirLAngle[x]*filterOut))*(1.0-filterOut))+(bezL*filterOut);
			iirLSample[x] = ((iirLSample[x]+(iirLAngle[x]*filterOut))*(1.0-filterOut))+(bezL*filterOut);
		} //lowpass audio band
		double bezR = drySampleR;
		for(int x = 0; x < 6; x++) {
			iirRAngle[x] = (iirRAngle[x]*(1.0-filterOut))+((bezR-iirRSample[x])*filterOut);
			bezR = ((iirRSample[x]+(iirRAngle[x]*filterOut))*(1.0-filterOut))+(bezR*filterOut);
			iirRSample[x] = ((iirRSample[x]+(iirRAngle[x]*filterOut))*(1.0-filterOut))+(bezR*filterOut);
		} //lowpass audio band
		
		double rect = fabs(drySampleL*drySampleR*64.0);
		for(int x = 0; x < 6; x++) {
			rect = fabs((iirCSample[x]*(1.0-filterRef))+(rect*filterRef));
			iirCSample[x] = (iirCSample[x]*(1.0-filterRef))+(rect*filterRef);
		} //lowpass rectified CV band
		
		aAL[count] = drySampleL;
		aAR[count] = drySampleR;
		aBL[count] = bezL;
		aBR[count] = bezR;
		aCL[count] = rect;
		
		count++; if (count < 0 || count > adjDelay) count = 0;
		double near = rect;
		double far = aCL[count-((count > adjDelay)?adjDelay+1:0)];
		//near and far are the smoothed rectified version
		
		double prevL = aAL[(count+halfRaw+offset)-(((count+halfRaw+offset) > adjDelay)?adjDelay+1:0)];
		double prevR = aAR[(count+halfRaw+offset)-(((count+halfRaw+offset) > adjDelay)?adjDelay+1:0)];
		double outL = aAL[(count+halfRaw)-(((count+halfRaw) > adjDelay)?adjDelay+1:0)];
		double outR = aAR[(count+halfRaw)-(((count+halfRaw) > adjDelay)?adjDelay+1:0)];
		double outBezL = aBL[(count+halfBez)-(((count+halfBez) > adjDelay)?adjDelay+1:0)];
		double outBezR = aBR[(count+halfBez)-(((count+halfBez) > adjDelay)?adjDelay+1:0)];
		//these are the various outputs to fade between
		double trigL = aAL[(count+halfTrig)-(((count+halfTrig) > adjDelay)?adjDelay+1:0)];
		double trigR = aAR[(count+halfTrig)-(((count+halfTrig) > adjDelay)?adjDelay+1:0)];
		
		double deClickL = pow(fmax((fabs(trigL)-threshold)-fmax(near,far),0.0)*16.0,3.0);
		double deClickR = pow(fmax((fabs(trigR)-threshold)-fmax(near,far),0.0)*16.0,3.0);
		iirTargetL = fmax(iirTargetL-iirCut,0.0);
		iirTargetR = fmax(iirTargetR-iirCut,0.0); //taper down at iirCut speed
		if (deClickL > iirTargetL) iirTargetL = fmin(deClickL,maxHeight);
		if (deClickR > iirTargetR) iirTargetR = fmin(deClickR,maxHeight); //beyond 1.0 to stretch window
		if (deClickR*0.618 > iirTargetL) iirTargetL = fmin(deClickR*0.618,maxHeight);
		if (deClickL*0.618 > iirTargetR) iirTargetR = fmin(deClickL*0.618,maxHeight); //opposite channel
		iirClickL = fmin(iirClickL+iirCut,iirTargetL);
		iirClickR = fmin(iirClickR+iirCut,iirTargetR); //taper up on attack
		inputSampleL = (outBezL*fmin(iirClickL, 1.0)) + (outL*(1.0-fmin(iirClickL, 1.0)));
		inputSampleR = (outBezR*fmin(iirClickR, 1.0)) + (outR*(1.0-fmin(iirClickR, 1.0)));
		//declicking is applied: crossfade between darkened and normal
		
		if (wet < 1.0 && wet > 0.0) {
			inputSampleL = (inputSampleL*wet)+(outL*(1.0-wet));
			inputSampleR = (inputSampleR*wet)+(outR*(1.0-wet));
		} //neither full wet, nor the 'clicks only' special case below
		
		if (wet == 0.0) {
			inputSampleL = outL - inputSampleL;
			inputSampleR = outR - inputSampleR;
		} //clicks only at full dry
		
		double recordVolume = fmax(fmax(near,far),fmax(prevL,prevR))+0.001; //engage only at quietest
		double surfaceL = sin(fmin((fabs(outL-prevL)/recordVolume)*surface, 3.14159265358979))*0.5;
		double surfaceR = sin(fmin((fabs(outR-prevR)/recordVolume)*surface, 3.14159265358979))*0.5;
		double gateOnAudio = fmax(surface-(recordVolume*surface*4.0),0.0);
		if (surface > 0.0 && wet > 0.0) {
			inputSampleL = (prevOutL*surfaceL)+(inputSampleL*(1.0-surfaceL));
			inputSampleR = (prevOutR*surfaceR)+(inputSampleR*(1.0-surfaceR));
			inputSampleL = (prevOutL*gateOnAudio)+(inputSampleL*(1.0-gateOnAudio));
			inputSampleR = (prevOutR*gateOnAudio)+(inputSampleR*(1.0-gateOnAudio));
			prevOutL = (prevOutL*gateOnAudio)+(inputSampleL*(1.0-gateOnAudio));
			prevOutR = (prevOutR*gateOnAudio)+(inputSampleR*(1.0-gateOnAudio));
		} //if we're also adding surface noise reduction that goes here
		
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
