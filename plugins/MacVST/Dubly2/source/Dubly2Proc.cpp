/* ========================================
 *  Dubly2 - Dubly2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Dubly2_H
#include "Dubly2.h"
#endif

void Dubly2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double dublyAmount = pow(A,3)*0.25;
	double iirEncFreq = B/overallscale;
	double tapeDrv = (C*2.0)+1.0;
	
	double outlyAmount = pow(D,3)*0.25;
	double iirDecFreq = E/overallscale;
	double outPad = (F*2.0)+1.0;
	if (tapeDrv > 1.0) outPad += 0.005;
	
	double wet = G;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		iirEncL = (iirEncL * (1.0 - iirEncFreq)) + (inputSampleL * iirEncFreq);
		iirEncR = (iirEncR * (1.0 - iirEncFreq)) + (inputSampleR * iirEncFreq);
		double doubly = inputSampleL - iirEncL;
		if (doubly > 1.0) doubly = 1.0; if (doubly < -1.0) doubly = -1.0;
		if (doubly > 0) doubly = log(1.0+(255*fabs(doubly)))/2.40823996531;
		if (doubly < 0) doubly = -log(1.0+(255*fabs(doubly)))/2.40823996531;
		inputSampleL += doubly*dublyAmount;
		doubly = inputSampleR - iirEncR;
		if (doubly > 1.0) doubly = 1.0; if (doubly < -1.0) doubly = -1.0;
		if (doubly > 0) doubly = log(1.0+(255*fabs(doubly)))/2.40823996531;
		if (doubly < 0) doubly = -log(1.0+(255*fabs(doubly)))/2.40823996531;
		inputSampleR += doubly*dublyAmount;
		//end Dubly encode
		
		if (tapeDrv > 1.0) {
			inputSampleL *= tapeDrv;
			inputSampleR *= tapeDrv;
		}
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		if (outPad > 1.0) {
			inputSampleL /= outPad;
			inputSampleR /= outPad;
		}
		
		iirDecL = (iirDecL * (1.0 - iirDecFreq)) + (inputSampleL * iirDecFreq);
		iirDecR = (iirDecR * (1.0 - iirDecFreq)) + (inputSampleR * iirDecFreq);
		doubly = inputSampleL - iirDecL;
		if (doubly > 1.0) doubly = 1.0; if (doubly < -1.0) doubly = -1.0;
		if (doubly > 0) doubly = log(1.0+(255*fabs(doubly)))/2.40823996531;
		if (doubly < 0) doubly = -log(1.0+(255*fabs(doubly)))/2.40823996531;
		inputSampleL -= doubly*outlyAmount;
		doubly = inputSampleR - iirDecR;
		if (doubly > 1.0) doubly = 1.0; if (doubly < -1.0) doubly = -1.0;
		if (doubly > 0) doubly = log(1.0+(255*fabs(doubly)))/2.40823996531;
		if (doubly < 0) doubly = -log(1.0+(255*fabs(doubly)))/2.40823996531;
		inputSampleR -= doubly*outlyAmount;
		//end Dubly decode
				
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Dubly2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double dublyAmount = pow(A,3)*0.25;
	double iirEncFreq = B/overallscale;
	double tapeDrv = (C*2.0)+1.0;
	
	double outlyAmount = pow(D,3)*0.25;
	double iirDecFreq = E/overallscale;
	double outPad = (F*2.0)+1.0;
	if (tapeDrv > 1.0) outPad += 0.005;
	
	double wet = G;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		iirEncL = (iirEncL * (1.0 - iirEncFreq)) + (inputSampleL * iirEncFreq);
		iirEncR = (iirEncR * (1.0 - iirEncFreq)) + (inputSampleR * iirEncFreq);
		double doubly = inputSampleL - iirEncL;
		if (doubly > 1.0) doubly = 1.0; if (doubly < -1.0) doubly = -1.0;
		if (doubly > 0) doubly = log(1.0+(255*fabs(doubly)))/2.40823996531;
		if (doubly < 0) doubly = -log(1.0+(255*fabs(doubly)))/2.40823996531;
		inputSampleL += doubly*dublyAmount;
		doubly = inputSampleR - iirEncR;
		if (doubly > 1.0) doubly = 1.0; if (doubly < -1.0) doubly = -1.0;
		if (doubly > 0) doubly = log(1.0+(255*fabs(doubly)))/2.40823996531;
		if (doubly < 0) doubly = -log(1.0+(255*fabs(doubly)))/2.40823996531;
		inputSampleR += doubly*dublyAmount;
		//end Dubly encode
		
		if (tapeDrv > 1.0) {
			inputSampleL *= tapeDrv;
			inputSampleR *= tapeDrv;
		}
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		if (outPad > 1.0) {
			inputSampleL /= outPad;
			inputSampleR /= outPad;
		}
		
		iirDecL = (iirDecL * (1.0 - iirDecFreq)) + (inputSampleL * iirDecFreq);
		iirDecR = (iirDecR * (1.0 - iirDecFreq)) + (inputSampleR * iirDecFreq);
		doubly = inputSampleL - iirDecL;
		if (doubly > 1.0) doubly = 1.0; if (doubly < -1.0) doubly = -1.0;
		if (doubly > 0) doubly = log(1.0+(255*fabs(doubly)))/2.40823996531;
		if (doubly < 0) doubly = -log(1.0+(255*fabs(doubly)))/2.40823996531;
		inputSampleL -= doubly*outlyAmount;
		doubly = inputSampleR - iirDecR;
		if (doubly > 1.0) doubly = 1.0; if (doubly < -1.0) doubly = -1.0;
		if (doubly > 0) doubly = log(1.0+(255*fabs(doubly)))/2.40823996531;
		if (doubly < 0) doubly = -log(1.0+(255*fabs(doubly)))/2.40823996531;
		inputSampleR -= doubly*outlyAmount;
		//end Dubly decode
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
