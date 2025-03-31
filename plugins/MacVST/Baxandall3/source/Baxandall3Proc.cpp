/* ========================================
 *  Baxandall3 - Baxandall3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Baxandall3_H
#include "Baxandall3.h"
#endif

void Baxandall3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputGain = pow(A*2.0,2.0);
	double trebleGain = pow(B*2.0,2.0);
	double trebleFreq = ((2000.0*trebleGain)+200.0)/getSampleRate();
	if (trebleFreq > 0.45) trebleFreq = 0.45;
	baxH[bax_freq] = trebleFreq;
    baxH[bax_reso] = 0.57735026919; //bessel second order
	double K = tan(M_PI * baxH[bax_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / baxH[bax_reso] + K * K);
	baxH[bax_a0] = K * K * norm;
	baxH[bax_a1] = 2.0 * baxH[bax_a0];
	baxH[bax_a2] = baxH[bax_a0];
	baxH[bax_b1] = 2.0 * (K * K - 1.0) * norm;
	baxH[bax_b2] = (1.0 - K / baxH[bax_reso] + K * K) * norm;
	//end bax highpass
	double bassGain = pow(C*2.0,2.0);
	double bassFreq = pow((1.0-C)*2.0,2.0);
	bassFreq = ((2000.0*bassFreq)+200.0)/getSampleRate();
	if (bassFreq > 0.45) bassFreq = 0.45;
	baxL[bax_freq] = bassFreq;
    baxL[bax_reso] = 0.57735026919; //bessel second order
	K = tan(M_PI * baxL[bax_freq]); //lowpass
	norm = 1.0 / (1.0 + K / baxL[bax_reso] + K * K);
	baxL[bax_a0] = K * K * norm;
	baxL[bax_a1] = 2.0 * baxL[bax_a0];
	baxL[bax_a2] = baxL[bax_a0];
	baxL[bax_b1] = 2.0 * (K * K - 1.0) * norm;
	baxL[bax_b2] = (1.0 - K / baxL[bax_reso] + K * K) * norm;
	//end bax lowpass
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL = sin(fmax(fmin(inputSampleL*inputGain,M_PI_2),-M_PI_2));
		inputSampleR = sin(fmax(fmin(inputSampleR*inputGain,M_PI_2),-M_PI_2));
		//encode Console5: good cleanness
		double trebleSampleL;
		double trebleSampleR;
		double bassSampleL;
		double bassSampleR;
		if (flip) {
			trebleSampleL = (inputSampleL * baxH[bax_a0]) + baxH[bax_sLA1];
			baxH[bax_sLA1] = (inputSampleL * baxH[bax_a1]) - (trebleSampleL * baxH[bax_b1]) + baxH[bax_sLA2];
			baxH[bax_sLA2] = (inputSampleL * baxH[bax_a2]) - (trebleSampleL * baxH[bax_b2]);
			trebleSampleL = inputSampleL - trebleSampleL;
			trebleSampleR = (inputSampleR * baxH[bax_a0]) + baxH[bax_sRA1];
			baxH[bax_sRA1] = (inputSampleR * baxH[bax_a1]) - (trebleSampleR * baxH[bax_b1]) + baxH[bax_sRA2];
			baxH[bax_sRA2] = (inputSampleR * baxH[bax_a2]) - (trebleSampleR * baxH[bax_b2]);
			trebleSampleR = inputSampleR - trebleSampleR;
			bassSampleL = (inputSampleL * baxL[bax_a0]) + baxL[bax_sLA1];
			baxL[bax_sLA1] = (inputSampleL * baxL[bax_a1]) - (bassSampleL * baxL[bax_b1]) + baxL[bax_sLA2];
			baxL[bax_sLA2] = (inputSampleL * baxL[bax_a2]) - (bassSampleL * baxL[bax_b2]);
			bassSampleR = (inputSampleR * baxL[bax_a0]) + baxL[bax_sRA1];
			baxL[bax_sRA1] = (inputSampleR * baxL[bax_a1]) - (bassSampleR * baxL[bax_b1]) + baxL[bax_sRA2];
			baxL[bax_sRA2] = (inputSampleR * baxL[bax_a2]) - (bassSampleR * baxL[bax_b2]);
		} else {
			trebleSampleL = (inputSampleL * baxH[bax_a0]) + baxH[bax_sLB1];
			baxH[bax_sLB1] = (inputSampleL * baxH[bax_a1]) - (trebleSampleL * baxH[bax_b1]) + baxH[bax_sLB2];
			baxH[bax_sLB2] = (inputSampleL * baxH[bax_a2]) - (trebleSampleL * baxH[bax_b2]);
			trebleSampleL = inputSampleL - trebleSampleL;
			trebleSampleR = (inputSampleR * baxH[bax_a0]) + baxH[bax_sRB1];
			baxH[bax_sRB1] = (inputSampleR * baxH[bax_a1]) - (trebleSampleR * baxH[bax_b1]) + baxH[bax_sRB2];
			baxH[bax_sRB2] = (inputSampleR * baxH[bax_a2]) - (trebleSampleR * baxH[bax_b2]);
			trebleSampleR = inputSampleR - trebleSampleR;
			bassSampleL = (inputSampleL * baxL[bax_a0]) + baxL[bax_sLB1];
			baxL[bax_sLB1] = (inputSampleL * baxL[bax_a1]) - (bassSampleL * baxL[bax_b1]) + baxL[bax_sLB2];
			baxL[bax_sLB2] = (inputSampleL * baxL[bax_a2]) - (bassSampleL * baxL[bax_b2]);
			bassSampleR = (inputSampleR * baxL[bax_a0]) + baxL[bax_sRB1];
			baxL[bax_sRB1] = (inputSampleR * baxL[bax_a1]) - (bassSampleR * baxL[bax_b1]) + baxL[bax_sRB2];
			baxL[bax_sRB2] = (inputSampleR * baxL[bax_a2]) - (bassSampleR * baxL[bax_b2]);
		}
		flip = !flip;
		
		trebleSampleL *= trebleGain;
		trebleSampleR *= trebleGain;
		bassSampleL *= bassGain;
		bassSampleR *= bassGain;
		inputSampleL = bassSampleL + trebleSampleL; //interleaved biquad
		inputSampleR = bassSampleR + trebleSampleR; //interleaved biquad
		
		inputSampleL = asin(fmax(fmin(inputSampleL,0.99999),-0.99999));
		inputSampleR = asin(fmax(fmin(inputSampleR,0.99999),-0.99999));
		//amplitude aspect
		
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

void Baxandall3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputGain = pow(A*2.0,2.0);
	double trebleGain = pow(B*2.0,2.0);
	double trebleFreq = ((2000.0*trebleGain)+200.0)/getSampleRate();
	if (trebleFreq > 0.45) trebleFreq = 0.45;
	baxH[bax_freq] = trebleFreq;
    baxH[bax_reso] = 0.57735026919; //bessel second order
	double K = tan(M_PI * baxH[bax_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / baxH[bax_reso] + K * K);
	baxH[bax_a0] = K * K * norm;
	baxH[bax_a1] = 2.0 * baxH[bax_a0];
	baxH[bax_a2] = baxH[bax_a0];
	baxH[bax_b1] = 2.0 * (K * K - 1.0) * norm;
	baxH[bax_b2] = (1.0 - K / baxH[bax_reso] + K * K) * norm;
	//end bax highpass
	double bassGain = pow(C*2.0,2.0);
	double bassFreq = pow((1.0-C)*2.0,2.0);
	bassFreq = ((2000.0*bassFreq)+200.0)/getSampleRate();
	if (bassFreq > 0.45) bassFreq = 0.45;
	baxL[bax_freq] = bassFreq;
    baxL[bax_reso] = 0.57735026919; //bessel second order
	K = tan(M_PI * baxL[bax_freq]); //lowpass
	norm = 1.0 / (1.0 + K / baxL[bax_reso] + K * K);
	baxL[bax_a0] = K * K * norm;
	baxL[bax_a1] = 2.0 * baxL[bax_a0];
	baxL[bax_a2] = baxL[bax_a0];
	baxL[bax_b1] = 2.0 * (K * K - 1.0) * norm;
	baxL[bax_b2] = (1.0 - K / baxL[bax_reso] + K * K) * norm;
	//end bax lowpass
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL = sin(fmax(fmin(inputSampleL*inputGain,M_PI_2),-M_PI_2));
		inputSampleR = sin(fmax(fmin(inputSampleR*inputGain,M_PI_2),-M_PI_2));
		//encode Console5: good cleanness
		double trebleSampleL;
		double trebleSampleR;
		double bassSampleL;
		double bassSampleR;
		if (flip) {
			trebleSampleL = (inputSampleL * baxH[bax_a0]) + baxH[bax_sLA1];
			baxH[bax_sLA1] = (inputSampleL * baxH[bax_a1]) - (trebleSampleL * baxH[bax_b1]) + baxH[bax_sLA2];
			baxH[bax_sLA2] = (inputSampleL * baxH[bax_a2]) - (trebleSampleL * baxH[bax_b2]);
			trebleSampleL = inputSampleL - trebleSampleL;
			trebleSampleR = (inputSampleR * baxH[bax_a0]) + baxH[bax_sRA1];
			baxH[bax_sRA1] = (inputSampleR * baxH[bax_a1]) - (trebleSampleR * baxH[bax_b1]) + baxH[bax_sRA2];
			baxH[bax_sRA2] = (inputSampleR * baxH[bax_a2]) - (trebleSampleR * baxH[bax_b2]);
			trebleSampleR = inputSampleR - trebleSampleR;
			bassSampleL = (inputSampleL * baxL[bax_a0]) + baxL[bax_sLA1];
			baxL[bax_sLA1] = (inputSampleL * baxL[bax_a1]) - (bassSampleL * baxL[bax_b1]) + baxL[bax_sLA2];
			baxL[bax_sLA2] = (inputSampleL * baxL[bax_a2]) - (bassSampleL * baxL[bax_b2]);
			bassSampleR = (inputSampleR * baxL[bax_a0]) + baxL[bax_sRA1];
			baxL[bax_sRA1] = (inputSampleR * baxL[bax_a1]) - (bassSampleR * baxL[bax_b1]) + baxL[bax_sRA2];
			baxL[bax_sRA2] = (inputSampleR * baxL[bax_a2]) - (bassSampleR * baxL[bax_b2]);
		} else {
			trebleSampleL = (inputSampleL * baxH[bax_a0]) + baxH[bax_sLB1];
			baxH[bax_sLB1] = (inputSampleL * baxH[bax_a1]) - (trebleSampleL * baxH[bax_b1]) + baxH[bax_sLB2];
			baxH[bax_sLB2] = (inputSampleL * baxH[bax_a2]) - (trebleSampleL * baxH[bax_b2]);
			trebleSampleL = inputSampleL - trebleSampleL;
			trebleSampleR = (inputSampleR * baxH[bax_a0]) + baxH[bax_sRB1];
			baxH[bax_sRB1] = (inputSampleR * baxH[bax_a1]) - (trebleSampleR * baxH[bax_b1]) + baxH[bax_sRB2];
			baxH[bax_sRB2] = (inputSampleR * baxH[bax_a2]) - (trebleSampleR * baxH[bax_b2]);
			trebleSampleR = inputSampleR - trebleSampleR;
			bassSampleL = (inputSampleL * baxL[bax_a0]) + baxL[bax_sLB1];
			baxL[bax_sLB1] = (inputSampleL * baxL[bax_a1]) - (bassSampleL * baxL[bax_b1]) + baxL[bax_sLB2];
			baxL[bax_sLB2] = (inputSampleL * baxL[bax_a2]) - (bassSampleL * baxL[bax_b2]);
			bassSampleR = (inputSampleR * baxL[bax_a0]) + baxL[bax_sRB1];
			baxL[bax_sRB1] = (inputSampleR * baxL[bax_a1]) - (bassSampleR * baxL[bax_b1]) + baxL[bax_sRB2];
			baxL[bax_sRB2] = (inputSampleR * baxL[bax_a2]) - (bassSampleR * baxL[bax_b2]);
		}
		flip = !flip;
		
		trebleSampleL *= trebleGain;
		trebleSampleR *= trebleGain;
		bassSampleL *= bassGain;
		bassSampleR *= bassGain;
		inputSampleL = bassSampleL + trebleSampleL; //interleaved biquad
		inputSampleR = bassSampleR + trebleSampleR; //interleaved biquad
		
		inputSampleL = asin(fmax(fmin(inputSampleL,0.99999),-0.99999));
		inputSampleR = asin(fmax(fmin(inputSampleR,0.99999),-0.99999));
		//amplitude aspect
		
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
