/* ========================================
 *  Dubly3 - Dubly3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Dubly3_H
#include "Dubly3.h"
#endif

void Dubly3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double inputGain = pow(A*2.0,2.0);
	double dublyAmount = B*2.0;
	double outlyAmount = (1.0-B)*-2.0;
	if (outlyAmount < -1.0) outlyAmount = -1.0;
	double iirEncFreq = (1.0-C)/overallscale;
	double iirDecFreq = C/overallscale;
	double outputGain = D*2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		if (inputGain != 1.0) {
			inputSampleL *= inputGain;
			inputSampleR *= inputGain;
		}
		
		//Dubly encode
		iirEncL = (iirEncL * (1.0 - iirEncFreq)) + (inputSampleL * iirEncFreq);
		double highPart = ((inputSampleL-iirEncL)*2.848);
		highPart += avgEncL; avgEncL = (inputSampleL-iirEncL)*1.152;
		if (highPart > 1.0) highPart = 1.0; if (highPart < -1.0) highPart = -1.0;
		double dubly = fabs(highPart);
		if (dubly > 0.0) {
			double adjust = log(1.0+(255.0*dubly))/2.40823996531;
			if (adjust > 0.0) dubly /= adjust;
			compEncL = (compEncL*(1.0-iirEncFreq))+(dubly*iirEncFreq);
			inputSampleL += ((highPart*compEncL)*dublyAmount);
		} //end Dubly encode L
		iirEncR = (iirEncR * (1.0 - iirEncFreq)) + (inputSampleR * iirEncFreq);
		highPart = ((inputSampleR-iirEncR)*2.848);
		highPart += avgEncR; avgEncR = (inputSampleR-iirEncR)*1.152;
		if (highPart > 1.0) highPart = 1.0; if (highPart < -1.0) highPart = -1.0;
		dubly = fabs(highPart);
		if (dubly > 0.0) {
			double adjust = log(1.0+(255.0*dubly))/2.40823996531;
			if (adjust > 0.0) dubly /= adjust;
			compEncR = (compEncR*(1.0-iirEncFreq))+(dubly*iirEncFreq);
			inputSampleR += ((highPart*compEncR)*dublyAmount);
		} //end Dubly encode R
		
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		
		//Dubly decode
		iirDecL = (iirDecL * (1.0 - iirDecFreq)) + (inputSampleL * iirDecFreq);
		highPart = ((inputSampleL-iirDecL)*2.628);
		highPart += avgDecL; avgDecL = (inputSampleL-iirDecL)*1.372;
		if (highPart > 1.0) highPart = 1.0; if (highPart < -1.0) highPart = -1.0;
		dubly = fabs(highPart);
		if (dubly > 0.0) {
			double adjust = log(1.0+(255.0*dubly))/2.40823996531;
			if (adjust > 0.0) dubly /= adjust;
			compDecL = (compDecL*(1.0-iirDecFreq))+(dubly*iirDecFreq);
			inputSampleL += ((highPart*compDecL)*outlyAmount);
		} //end Dubly decode L
		iirDecR = (iirDecR * (1.0 - iirDecFreq)) + (inputSampleR * iirDecFreq);
		highPart = ((inputSampleR-iirDecR)*2.628);
		highPart += avgDecR; avgDecR = (inputSampleR-iirDecR)*1.372;
		if (highPart > 1.0) highPart = 1.0; if (highPart < -1.0) highPart = -1.0;
		dubly = fabs(highPart);
		if (dubly > 0.0) {
			double adjust = log(1.0+(255.0*dubly))/2.40823996531;
			if (adjust > 0.0) dubly /= adjust;
			compDecR = (compDecR*(1.0-iirDecFreq))+(dubly*iirDecFreq);
			inputSampleR += ((highPart*compDecR)*outlyAmount);
		} //end Dubly decode R
		
		if (outputGain != 1.0) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
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

void Dubly3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputGain = pow(A*2.0,2.0);
	double dublyAmount = B*2.0;
	double outlyAmount = (1.0-B)*-2.0;
	if (outlyAmount < -1.0) outlyAmount = -1.0;
	double iirEncFreq = (1.0-C)/overallscale;
	double iirDecFreq = C/overallscale;
	double outputGain = D*2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (inputGain != 1.0) {
			inputSampleL *= inputGain;
			inputSampleR *= inputGain;
		}
		
		//Dubly encode
		iirEncL = (iirEncL * (1.0 - iirEncFreq)) + (inputSampleL * iirEncFreq);
		double highPart = ((inputSampleL-iirEncL)*2.848);
		highPart += avgEncL; avgEncL = (inputSampleL-iirEncL)*1.152;
		if (highPart > 1.0) highPart = 1.0; if (highPart < -1.0) highPart = -1.0;
		double dubly = fabs(highPart);
		if (dubly > 0.0) {
			double adjust = log(1.0+(255.0*dubly))/2.40823996531;
			if (adjust > 0.0) dubly /= adjust;
			compEncL = (compEncL*(1.0-iirEncFreq))+(dubly*iirEncFreq);
			inputSampleL += ((highPart*compEncL)*dublyAmount);
		} //end Dubly encode L
		iirEncR = (iirEncR * (1.0 - iirEncFreq)) + (inputSampleR * iirEncFreq);
		highPart = ((inputSampleR-iirEncR)*2.848);
		highPart += avgEncR; avgEncR = (inputSampleR-iirEncR)*1.152;
		if (highPart > 1.0) highPart = 1.0; if (highPart < -1.0) highPart = -1.0;
		dubly = fabs(highPart);
		if (dubly > 0.0) {
			double adjust = log(1.0+(255.0*dubly))/2.40823996531;
			if (adjust > 0.0) dubly /= adjust;
			compEncR = (compEncR*(1.0-iirEncFreq))+(dubly*iirEncFreq);
			inputSampleR += ((highPart*compEncR)*dublyAmount);
		} //end Dubly encode R
		
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		
		//Dubly decode
		iirDecL = (iirDecL * (1.0 - iirDecFreq)) + (inputSampleL * iirDecFreq);
		highPart = ((inputSampleL-iirDecL)*2.628);
		highPart += avgDecL; avgDecL = (inputSampleL-iirDecL)*1.372;
		if (highPart > 1.0) highPart = 1.0; if (highPart < -1.0) highPart = -1.0;
		dubly = fabs(highPart);
		if (dubly > 0.0) {
			double adjust = log(1.0+(255.0*dubly))/2.40823996531;
			if (adjust > 0.0) dubly /= adjust;
			compDecL = (compDecL*(1.0-iirDecFreq))+(dubly*iirDecFreq);
			inputSampleL += ((highPart*compDecL)*outlyAmount);
		} //end Dubly decode L
		iirDecR = (iirDecR * (1.0 - iirDecFreq)) + (inputSampleR * iirDecFreq);
		highPart = ((inputSampleR-iirDecR)*2.628);
		highPart += avgDecR; avgDecR = (inputSampleR-iirDecR)*1.372;
		if (highPart > 1.0) highPart = 1.0; if (highPart < -1.0) highPart = -1.0;
		dubly = fabs(highPart);
		if (dubly > 0.0) {
			double adjust = log(1.0+(255.0*dubly))/2.40823996531;
			if (adjust > 0.0) dubly /= adjust;
			compDecR = (compDecR*(1.0-iirDecFreq))+(dubly*iirDecFreq);
			inputSampleR += ((highPart*compDecR)*outlyAmount);
		} //end Dubly decode R
		
		if (outputGain != 1.0) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
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
