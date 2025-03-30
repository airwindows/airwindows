/* ========================================
 *  AngleEQ - AngleEQ.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AngleEQ_H
#include "AngleEQ.h"
#endif

void AngleEQ::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double highVol = pow(A*2.0,2.0);
	double midVol = pow(B*2.0,2.0);
	double lowVol = pow(C*2.0,2.0);
	
	double hFreq = pow(D,overallscale);
	double mhFreq = pow(E,overallscale);
	double mlFreq = pow(F,overallscale+3.0);
	double lFreq = pow(G,overallscale+3.0);
	
	double dry = 2.0-(H*2.0);
	if (dry > 1.0) dry = 1.0; //full dry for use with inv, to 0.0 at full wet
	double wet = (H*2.0)-1.0; //inv-dry-wet for highpass
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double midSampleL = inputSampleL;
		double midSampleR = inputSampleR;
		
		double highSampleL = inputSampleL;
		double highSampleR = inputSampleR;
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleL[count] = (iirHAngleL[count]*(1.0-hFreq))+((inputSampleL-iirHPositionL[count])*hFreq);
			inputSampleL = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
			iirHPositionL[count] = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
			highSampleL -= (inputSampleL * (1.0/(3.0+(hFreq*32.0))) );
			iirHAngleR[count] = (iirHAngleR[count]*(1.0-hFreq))+((inputSampleR-iirHPositionR[count])*hFreq);
			inputSampleR = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
			iirHPositionR[count] = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
			highSampleR -= (inputSampleR * (1.0/(3.0+(hFreq*32.0))) );
		} //highpass point of treble band
		
		inputSampleL = midSampleL; //restore for second highpass
		inputSampleR = midSampleR; //restore for second highpass
		for(int count = 0; count < (3.0+(mlFreq*32.0)); count++) {
			iirMHAngleL[count] = (iirMHAngleL[count]*(1.0-mlFreq))+((inputSampleL-iirMHPositionL[count])*mlFreq);
			inputSampleL = ((iirMHPositionL[count]+(iirMHAngleL[count]*mlFreq))*(1.0-mlFreq))+(inputSampleL*mlFreq);
			iirMHPositionL[count] = ((iirMHPositionL[count]+(iirMHAngleL[count]*mlFreq))*(1.0-mlFreq))+(inputSampleL*mlFreq);
			midSampleL -= (inputSampleL * (1.0/(3.0+(mlFreq*32.0))) );
			iirMHAngleR[count] = (iirMHAngleR[count]*(1.0-mlFreq))+((inputSampleR-iirMHPositionR[count])*mlFreq);
			inputSampleR = ((iirMHPositionR[count]+(iirMHAngleR[count]*mlFreq))*(1.0-mlFreq))+(inputSampleR*mlFreq);
			iirMHPositionR[count] = ((iirMHPositionR[count]+(iirMHAngleR[count]*mlFreq))*(1.0-mlFreq))+(inputSampleR*mlFreq);
			midSampleR -= (inputSampleR * (1.0/(3.0+(mlFreq*32.0))) );
		} //highpass point of mid-to-low band
		
		for(int count = 0; count < (3.0+(mhFreq*32.0)); count++) {
			iirMLAngleL[count] = (iirMLAngleL[count]*(1.0-mhFreq))+((midSampleL-iirMLPositionL[count])*mhFreq);
			midSampleL = ((iirMLPositionL[count]+(iirMLAngleL[count]*mhFreq))*(1.0-mhFreq))+(midSampleL*mhFreq);
			iirMLPositionL[count] = ((iirMLPositionL[count]+(iirMLAngleL[count]*mhFreq))*(1.0-mhFreq))+(midSampleL*mhFreq);
			iirMLAngleR[count] = (iirMLAngleR[count]*(1.0-mhFreq))+((midSampleR-iirMLPositionR[count])*mhFreq);
			midSampleR = ((iirMLPositionR[count]+(iirMLAngleR[count]*mhFreq))*(1.0-mhFreq))+(midSampleR*mhFreq);
			iirMLPositionR[count] = ((iirMLPositionR[count]+(iirMLAngleR[count]*mhFreq))*(1.0-mhFreq))+(midSampleR*mhFreq);
		} //lowpass point of mid-to-high band
		
		double lowSampleL = inputSampleL;
		double lowSampleR = inputSampleR;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirLAngleL[count] = (iirLAngleL[count]*(1.0-lFreq))+((lowSampleL-iirLPositionL[count])*lFreq);
			lowSampleL = ((iirLPositionL[count]+(iirLAngleL[count]*lFreq))*(1.0-lFreq))+(lowSampleL*lFreq);
			iirLPositionL[count] = ((iirLPositionL[count]+(iirLAngleL[count]*lFreq))*(1.0-lFreq))+(lowSampleL*lFreq);
			iirLAngleR[count] = (iirLAngleR[count]*(1.0-lFreq))+((lowSampleR-iirLPositionR[count])*lFreq);
			lowSampleR = ((iirLPositionR[count]+(iirLAngleR[count]*lFreq))*(1.0-lFreq))+(lowSampleR*lFreq);
			iirLPositionR[count] = ((iirLPositionR[count]+(iirLAngleR[count]*lFreq))*(1.0-lFreq))+(lowSampleR*lFreq);
		} //lowpass point of low band
		
		inputSampleL = (drySampleL*dry)+(((highSampleL*highVol)+(midSampleL*midVol)+(lowSampleL*lowVol))*wet);
		inputSampleR = (drySampleR*dry)+(((highSampleR*highVol)+(midSampleR*midVol)+(lowSampleR*lowVol))*wet);
		
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

void AngleEQ::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double highVol = pow(A*2.0,2.0);
	double midVol = pow(B*2.0,2.0);
	double lowVol = pow(C*2.0,2.0);
	
	double hFreq = pow(D,overallscale);
	double mhFreq = pow(E,overallscale);
	double mlFreq = pow(F,overallscale+3.0);
	double lFreq = pow(G,overallscale+3.0);
	
	double dry = 2.0-(H*2.0);
	if (dry > 1.0) dry = 1.0; //full dry for use with inv, to 0.0 at full wet
	double wet = (H*2.0)-1.0; //inv-dry-wet for highpass
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double midSampleL = inputSampleL;
		double midSampleR = inputSampleR;
		
		double highSampleL = inputSampleL;
		double highSampleR = inputSampleR;
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleL[count] = (iirHAngleL[count]*(1.0-hFreq))+((inputSampleL-iirHPositionL[count])*hFreq);
			inputSampleL = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
			iirHPositionL[count] = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
			highSampleL -= (inputSampleL * (1.0/(3.0+(hFreq*32.0))) );
			iirHAngleR[count] = (iirHAngleR[count]*(1.0-hFreq))+((inputSampleR-iirHPositionR[count])*hFreq);
			inputSampleR = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
			iirHPositionR[count] = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
			highSampleR -= (inputSampleR * (1.0/(3.0+(hFreq*32.0))) );
		} //highpass point of treble band
		
		inputSampleL = midSampleL; //restore for second highpass
		inputSampleR = midSampleR; //restore for second highpass
		for(int count = 0; count < (3.0+(mlFreq*32.0)); count++) {
			iirMHAngleL[count] = (iirMHAngleL[count]*(1.0-mlFreq))+((inputSampleL-iirMHPositionL[count])*mlFreq);
			inputSampleL = ((iirMHPositionL[count]+(iirMHAngleL[count]*mlFreq))*(1.0-mlFreq))+(inputSampleL*mlFreq);
			iirMHPositionL[count] = ((iirMHPositionL[count]+(iirMHAngleL[count]*mlFreq))*(1.0-mlFreq))+(inputSampleL*mlFreq);
			midSampleL -= (inputSampleL * (1.0/(3.0+(mlFreq*32.0))) );
			iirMHAngleR[count] = (iirMHAngleR[count]*(1.0-mlFreq))+((inputSampleR-iirMHPositionR[count])*mlFreq);
			inputSampleR = ((iirMHPositionR[count]+(iirMHAngleR[count]*mlFreq))*(1.0-mlFreq))+(inputSampleR*mlFreq);
			iirMHPositionR[count] = ((iirMHPositionR[count]+(iirMHAngleR[count]*mlFreq))*(1.0-mlFreq))+(inputSampleR*mlFreq);
			midSampleR -= (inputSampleR * (1.0/(3.0+(mlFreq*32.0))) );
		} //highpass point of mid-to-low band
		
		for(int count = 0; count < (3.0+(mhFreq*32.0)); count++) {
			iirMLAngleL[count] = (iirMLAngleL[count]*(1.0-mhFreq))+((midSampleL-iirMLPositionL[count])*mhFreq);
			midSampleL = ((iirMLPositionL[count]+(iirMLAngleL[count]*mhFreq))*(1.0-mhFreq))+(midSampleL*mhFreq);
			iirMLPositionL[count] = ((iirMLPositionL[count]+(iirMLAngleL[count]*mhFreq))*(1.0-mhFreq))+(midSampleL*mhFreq);
			iirMLAngleR[count] = (iirMLAngleR[count]*(1.0-mhFreq))+((midSampleR-iirMLPositionR[count])*mhFreq);
			midSampleR = ((iirMLPositionR[count]+(iirMLAngleR[count]*mhFreq))*(1.0-mhFreq))+(midSampleR*mhFreq);
			iirMLPositionR[count] = ((iirMLPositionR[count]+(iirMLAngleR[count]*mhFreq))*(1.0-mhFreq))+(midSampleR*mhFreq);
		} //lowpass point of mid-to-high band
		
		double lowSampleL = inputSampleL;
		double lowSampleR = inputSampleR;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirLAngleL[count] = (iirLAngleL[count]*(1.0-lFreq))+((lowSampleL-iirLPositionL[count])*lFreq);
			lowSampleL = ((iirLPositionL[count]+(iirLAngleL[count]*lFreq))*(1.0-lFreq))+(lowSampleL*lFreq);
			iirLPositionL[count] = ((iirLPositionL[count]+(iirLAngleL[count]*lFreq))*(1.0-lFreq))+(lowSampleL*lFreq);
			iirLAngleR[count] = (iirLAngleR[count]*(1.0-lFreq))+((lowSampleR-iirLPositionR[count])*lFreq);
			lowSampleR = ((iirLPositionR[count]+(iirLAngleR[count]*lFreq))*(1.0-lFreq))+(lowSampleR*lFreq);
			iirLPositionR[count] = ((iirLPositionR[count]+(iirLAngleR[count]*lFreq))*(1.0-lFreq))+(lowSampleR*lFreq);
		} //lowpass point of low band
		
		inputSampleL = (drySampleL*dry)+(((highSampleL*highVol)+(midSampleL*midVol)+(lowSampleL*lowVol))*wet);
		inputSampleR = (drySampleR*dry)+(((highSampleR*highVol)+(midSampleR*midVol)+(lowSampleR*lowVol))*wet);
		
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
