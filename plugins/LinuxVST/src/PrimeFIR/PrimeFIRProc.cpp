/* ========================================
 *  PrimeFIR - PrimeFIR.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PrimeFIR_H
#include "PrimeFIR.h"
#endif

void PrimeFIR::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double freq = pow(A,2)*M_PI_2; if (freq < 0.0001) freq = 0.0001;
	double positionMiddle = sin(freq)*0.5; //shift relative to frequency, not sample-rate
	freq /= overallscale; //generating the FIR relative to sample rate
	int window = (int)(B*256.0*overallscale); //so's the window size
	if (window < 2) window = 2; if (window > 998) window = 998;
	double fir[1000];
	int middle = (int)((double)window*positionMiddle);
	bool nonPrime = (C < 0.5);
	if (nonPrime) {
		for(int fip = 0; fip < middle; fip++) {
			fir[fip] = (fip-middle)*freq;
			fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
			fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
		}
		fir[middle] = 1.0;
		for(int fip = middle+1; fip < window; fip++) {
			fir[fip] = (fip-middle)*freq;
			fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
			fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
		}
	} else {
		for(int fip = 0; fip < middle; fip++) {
			fir[fip] = (prime[middle-fip])*freq;
			fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
			fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
		}
		fir[middle] = 1.0;
		for(int fip = middle+1; fip < window; fip++) {
			fir[fip] = (prime[fip-middle])*freq;
			fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
			fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
		}
	}
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		if (firPosition < 0 || firPosition > 32767) firPosition = 32767; int firp = firPosition;		
		firBufferL[firp] = inputSampleL; inputSampleL = 0.0;
		firBufferR[firp] = inputSampleR; inputSampleR = 0.0;
		if (nonPrime) {
			if (firp + window < 32767) {
				for(int fip = 1; fip < window; fip++) {
					inputSampleL += firBufferL[firp+fip] * fir[fip];
					inputSampleR += firBufferR[firp+fip] * fir[fip];
				}
			} else {
				for(int fip = 1; fip < window; fip++) {
					inputSampleL += firBufferL[firp+fip - ((firp+fip > 32767)?32768:0)] * fir[fip];
					inputSampleR += firBufferR[firp+fip - ((firp+fip > 32767)?32768:0)] * fir[fip];
				}
			}
			inputSampleL *= 0.25;
			inputSampleR *= 0.25;
		} else {
			if (firp + prime[window] < 32767) {
				for(int fip = 1; fip < window; fip++) {
					inputSampleL += firBufferL[firp+prime[fip]] * fir[fip];
					inputSampleR += firBufferR[firp+prime[fip]] * fir[fip];
				}
			} else {
				for(int fip = 1; fip < window; fip++) {
					inputSampleL += firBufferL[firp+prime[fip] - ((firp+prime[fip] > 32767)?32768:0)] * fir[fip];
					inputSampleR += firBufferR[firp+prime[fip] - ((firp+prime[fip] > 32767)?32768:0)] * fir[fip];
				}
			}
			inputSampleL *= 0.5;
			inputSampleR *= 0.5;
		}
		inputSampleL *= sqrt(freq); //compensate for gain
		inputSampleR *= sqrt(freq); //compensate for gain
		firPosition--;
		
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

void PrimeFIR::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double freq = pow(A,2)*M_PI_2; if (freq < 0.0001) freq = 0.0001;
	double positionMiddle = sin(freq)*0.5; //shift relative to frequency, not sample-rate
	freq /= overallscale; //generating the FIR relative to sample rate
	int window = (int)(B*256.0*overallscale); //so's the window size
	if (window < 2) window = 2; if (window > 998) window = 998;
	double fir[1000];
	int middle = (int)((double)window*positionMiddle);
	bool nonPrime = (C < 0.5);
	if (nonPrime) {
		for(int fip = 0; fip < middle; fip++) {
			fir[fip] = (fip-middle)*freq;
			fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
			fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
		}
		fir[middle] = 1.0;
		for(int fip = middle+1; fip < window; fip++) {
			fir[fip] = (fip-middle)*freq;
			fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
			fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
		}
	} else {
		for(int fip = 0; fip < middle; fip++) {
			fir[fip] = (prime[middle-fip])*freq;
			fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
			fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
		}
		fir[middle] = 1.0;
		for(int fip = middle+1; fip < window; fip++) {
			fir[fip] = (prime[fip-middle])*freq;
			fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
			fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
		}
	}
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (firPosition < 0 || firPosition > 32767) firPosition = 32767; int firp = firPosition;		
		firBufferL[firp] = inputSampleL; inputSampleL = 0.0;
		firBufferR[firp] = inputSampleR; inputSampleR = 0.0;
		if (nonPrime) {
			if (firp + window < 32767) {
				for(int fip = 1; fip < window; fip++) {
					inputSampleL += firBufferL[firp+fip] * fir[fip];
					inputSampleR += firBufferR[firp+fip] * fir[fip];
				}
			} else {
				for(int fip = 1; fip < window; fip++) {
					inputSampleL += firBufferL[firp+fip - ((firp+fip > 32767)?32768:0)] * fir[fip];
					inputSampleR += firBufferR[firp+fip - ((firp+fip > 32767)?32768:0)] * fir[fip];
				}
			}
			inputSampleL *= 0.25;
			inputSampleR *= 0.25;
		} else {
			if (firp + prime[window] < 32767) {
				for(int fip = 1; fip < window; fip++) {
					inputSampleL += firBufferL[firp+prime[fip]] * fir[fip];
					inputSampleR += firBufferR[firp+prime[fip]] * fir[fip];
				}
			} else {
				for(int fip = 1; fip < window; fip++) {
					inputSampleL += firBufferL[firp+prime[fip] - ((firp+prime[fip] > 32767)?32768:0)] * fir[fip];
					inputSampleR += firBufferR[firp+prime[fip] - ((firp+prime[fip] > 32767)?32768:0)] * fir[fip];
				}
			}
			inputSampleL *= 0.5;
			inputSampleR *= 0.5;
		}
		inputSampleL *= sqrt(freq); //compensate for gain
		inputSampleR *= sqrt(freq); //compensate for gain
		firPosition--;
		
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
