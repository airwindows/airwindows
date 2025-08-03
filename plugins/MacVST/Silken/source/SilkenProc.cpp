/* ========================================
 *  Silken - Silken.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Silken_H
#include "Silken.h"
#endif

void Silken::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	double wet = A;
	double freq = pow(B,2)*M_PI_2; if (freq < 0.0001) freq = 0.0001;
	double positionMiddle = sin(freq)*0.5; //shift relative to frequency, not sample-rate
	freq /= overallscale; //generating the FIR relative to sample rate
	const int window = (int)fmin((C*C*256.0*overallscale)+2.0,998.0); //so's the window size
	const int middle = (int)((double)window*positionMiddle);
	for(int fip = 0; fip < middle; fip++) {
		fir[fip] = (unprime[middle-fip])*freq;
		fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
		fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
	}
	fir[middle] = 1.0;
	for(int fip = middle+1; fip < window; fip++) {
		fir[fip] = (unprime[fip-middle])*freq;
		fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
		fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
	}
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		if (firPosition < 0 || firPosition > 32767) firPosition = 32767;
		int firp = firPosition;		
		firBufferL[firp] = inputSampleL; inputSampleL = 0.0;
		firBufferR[firp] = inputSampleR; inputSampleR = 0.0;
		
		if (firp + unprime[window] < 32767) {
			for(int fip = 1; fip < window; fip++) {
				inputSampleL += firBufferL[firp+unprime[fip]] * fir[fip];
				inputSampleR += firBufferR[firp+unprime[fip]] * fir[fip];
			}
		} else {
			for(int fip = 1; fip < window; fip++) {
				inputSampleL += firBufferL[firp+unprime[fip] - ((firp+unprime[fip] > 32767)?32768:0)] * fir[fip];
				inputSampleR += firBufferR[firp+unprime[fip] - ((firp+unprime[fip] > 32767)?32768:0)] * fir[fip];
			}
		}
		inputSampleL *= sqrt(freq*0.618033988749894848204586); //compensate for gain
		inputSampleR *= sqrt(freq*0.618033988749894848204586); //compensate for gain
		firPosition--;
		
		double softSpeed = fabs(inputSampleL);
		if (softSpeed < 1.0) softSpeed = 1.0; else softSpeed = 1.0/softSpeed;
		inputSampleL = sin(inputSampleL)*0.9549925859; //scale to what cliponly uses
		inputSampleL = (inputSampleL*softSpeed)+(firlastSampleL*(1.0-softSpeed));
		
		softSpeed = fabs(inputSampleR);
		if (softSpeed < 1.0) softSpeed = 1.0; else softSpeed = 1.0/softSpeed;
		inputSampleR = sin(inputSampleR)*0.9549925859; //scale to what cliponly uses
		inputSampleR = (inputSampleR*softSpeed)+(firlastSampleR*(1.0-softSpeed));
		
		infirmediateL[spacing] = inputSampleL;
        inputSampleL = firlastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) infirmediateL[x-1] = infirmediateL[x];
		firlastSampleL = infirmediateL[0]; //run a little buffer to handle this
		
		infirmediateR[spacing] = inputSampleR;
        inputSampleR = firlastSampleR; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) infirmediateR[x-1] = infirmediateR[x];
		firlastSampleR = infirmediateR[0]; //run a little buffer to handle this
				
		if (firp+unprime[middle] < 32768) {
			inputSampleL = (firBufferL[firp+unprime[middle]]*(wet+1.0))-(inputSampleL*wet);
			inputSampleR = (firBufferR[firp+unprime[middle]]*(wet+1.0))-(inputSampleR*wet);
		} else {
			inputSampleL = (firBufferL[firp+unprime[middle]-32768]*(wet+1.0))-(inputSampleL*wet);
			inputSampleR = (firBufferR[firp+unprime[middle]-32768]*(wet+1.0))-(inputSampleR*wet);
		}//dry/wet must use a sample from the middle of firBuffer for dry,
		//because it's an FIR filter that is phase linear by nature
		
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

void Silken::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	double wet = A;
	double freq = pow(B,2)*M_PI_2; if (freq < 0.0001) freq = 0.0001;
	double positionMiddle = sin(freq)*0.5; //shift relative to frequency, not sample-rate
	freq /= overallscale; //generating the FIR relative to sample rate
	const int window = (int)fmin((C*C*256.0*overallscale)+2.0,998.0); //so's the window size
	const int middle = (int)((double)window*positionMiddle);
	for(int fip = 0; fip < middle; fip++) {
		fir[fip] = (unprime[middle-fip])*freq;
		fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
		fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
	}
	fir[middle] = 1.0;
	for(int fip = middle+1; fip < window; fip++) {
		fir[fip] = (unprime[fip-middle])*freq;
		fir[fip] = sin(fir[fip])/fir[fip]; //sinc function
		fir[fip] *= sin(((double)fip/(double)window)*M_PI); //windowed with sin()
	}
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (firPosition < 0 || firPosition > 32767) firPosition = 32767;
		int firp = firPosition;		
		firBufferL[firp] = inputSampleL; inputSampleL = 0.0;
		firBufferR[firp] = inputSampleR; inputSampleR = 0.0;
		
		if (firp + unprime[window] < 32767) {
			for(int fip = 1; fip < window; fip++) {
				inputSampleL += firBufferL[firp+unprime[fip]] * fir[fip];
				inputSampleR += firBufferR[firp+unprime[fip]] * fir[fip];
			}
		} else {
			for(int fip = 1; fip < window; fip++) {
				inputSampleL += firBufferL[firp+unprime[fip] - ((firp+unprime[fip] > 32767)?32768:0)] * fir[fip];
				inputSampleR += firBufferR[firp+unprime[fip] - ((firp+unprime[fip] > 32767)?32768:0)] * fir[fip];
			}
		}
		inputSampleL *= sqrt(freq*0.618033988749894848204586); //compensate for gain
		inputSampleR *= sqrt(freq*0.618033988749894848204586); //compensate for gain
		firPosition--;
		
		double softSpeed = fabs(inputSampleL);
		if (softSpeed < 1.0) softSpeed = 1.0; else softSpeed = 1.0/softSpeed;
		inputSampleL = sin(inputSampleL)*0.9549925859; //scale to what cliponly uses
		inputSampleL = (inputSampleL*softSpeed)+(firlastSampleL*(1.0-softSpeed));
		
		softSpeed = fabs(inputSampleR);
		if (softSpeed < 1.0) softSpeed = 1.0; else softSpeed = 1.0/softSpeed;
		inputSampleR = sin(inputSampleR)*0.9549925859; //scale to what cliponly uses
		inputSampleR = (inputSampleR*softSpeed)+(firlastSampleR*(1.0-softSpeed));
		
		infirmediateL[spacing] = inputSampleL;
        inputSampleL = firlastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) infirmediateL[x-1] = infirmediateL[x];
		firlastSampleL = infirmediateL[0]; //run a little buffer to handle this
		
		infirmediateR[spacing] = inputSampleR;
        inputSampleR = firlastSampleR; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) infirmediateR[x-1] = infirmediateR[x];
		firlastSampleR = infirmediateR[0]; //run a little buffer to handle this
		
		if (firp+unprime[middle] < 32768) {
			inputSampleL = (firBufferL[firp+unprime[middle]]*(wet+1.0))-(inputSampleL*wet);
			inputSampleR = (firBufferR[firp+unprime[middle]]*(wet+1.0))-(inputSampleR*wet);
		} else {
			inputSampleL = (firBufferL[firp+unprime[middle]-32768]*(wet+1.0))-(inputSampleL*wet);
			inputSampleR = (firBufferR[firp+unprime[middle]-32768]*(wet+1.0))-(inputSampleR*wet);
		}//dry/wet must use a sample from the middle of firBuffer for dry,
		//because it's an FIR filter that is phase linear by nature
		
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
