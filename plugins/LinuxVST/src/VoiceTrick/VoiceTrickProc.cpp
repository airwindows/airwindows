/* ========================================
 *  VoiceTrick - VoiceTrick.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VoiceTrick_H
#include "VoiceTrick.h"
#endif

void VoiceTrick::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	lowpassChase = pow(A,2);
	//should not scale with sample rate, because values reaching 1 are important
	//to its ability to bypass when set to max
	double lowpassSpeed = 300 / (fabs( lastLowpass - lowpassChase)+1.0);
	lastLowpass = lowpassChase;	
	double invLowpass;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		lowpassAmount = (((lowpassAmount*lowpassSpeed)+lowpassChase)/(lowpassSpeed + 1.0)); invLowpass = 1.0 - lowpassAmount;
		//setting chase functionality of Capacitor Lowpass. I could just use this value directly from the control,
		//but if I say it's the lowpass out of Capacitor it should literally be that in every behavior.
		
		double inputSample = (inputSampleL + inputSampleR) * 0.5;
		//this is now our mono audio
		
		count++; if (count > 5) count = 0; switch (count)
		{
			case 0:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirLowpassD = (iirLowpassD * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassD;
				break;
			case 1:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirLowpassE = (iirLowpassE * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassE;
				break;
			case 2:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirLowpassF = (iirLowpassF * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassF;
				break;
			case 3:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirLowpassD = (iirLowpassD * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassD;
				break;
			case 4:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirLowpassE = (iirLowpassE * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassE;
				break;
			case 5:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirLowpassF = (iirLowpassF * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassF;
				break;
		}
		//Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
		//steepens the filter after minimizing artifacts.
		
		
		inputSampleL = -inputSample;
		inputSampleR = inputSample;
		
		//and now the output is mono, maybe filtered, and phase flipped to cancel at the microphone.
		//The purpose of all this is to allow for recording of lead vocals without use of headphones:
		//or at least sealed headphones. You should be able to use this to record vocals with either
		//open-back headphones, or literally speakers in the room so long as the mic is exactly
		//equidistant from each speaker/headphone side.
		
		//You'll probably want to not use voice monitoring: just mute the track being recorded, or monitor
		//only reverb and echo for vibe. Direct sound is the singer's direct sound.
		
		//The filtering is because, if you use open-back headphones and move your head, highs will
		//bleed through first like a through-zero flange coming out of cancellation (which it is).
		//Therefore, you can filter off highs until the bleed isn't annoying.
		//Or just run with it, it shouldn't be that loud.
		
		//Thanks to Peter Gabriel for many great examples of hit vocals recorded just like this :)
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void VoiceTrick::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	lowpassChase = pow(A,2);
	//should not scale with sample rate, because values reaching 1 are important
	//to its ability to bypass when set to max
	double lowpassSpeed = 300 / (fabs( lastLowpass - lowpassChase)+1.0);
	lastLowpass = lowpassChase;	
	double invLowpass;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		lowpassAmount = (((lowpassAmount*lowpassSpeed)+lowpassChase)/(lowpassSpeed + 1.0)); invLowpass = 1.0 - lowpassAmount;
		//setting chase functionality of Capacitor Lowpass. I could just use this value directly from the control,
		//but if I say it's the lowpass out of Capacitor it should literally be that in every behavior.
		
		double inputSample = (inputSampleL + inputSampleR) * 0.5;
		//this is now our mono audio
		
		count++; if (count > 5) count = 0; switch (count)
		{
			case 0:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirLowpassD = (iirLowpassD * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassD;
				break;
			case 1:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirLowpassE = (iirLowpassE * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassE;
				break;
			case 2:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirLowpassF = (iirLowpassF * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassF;
				break;
			case 3:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirLowpassD = (iirLowpassD * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassD;
				break;
			case 4:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirLowpassE = (iirLowpassE * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassE;
				break;
			case 5:
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirLowpassF = (iirLowpassF * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassF;
				break;
		}
		//Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
		//steepens the filter after minimizing artifacts.
		
		
		inputSampleL = -inputSample;
		inputSampleR = inputSample;
		
		//and now the output is mono, maybe filtered, and phase flipped to cancel at the microphone.
		//The purpose of all this is to allow for recording of lead vocals without use of headphones:
		//or at least sealed headphones. You should be able to use this to record vocals with either
		//open-back headphones, or literally speakers in the room so long as the mic is exactly
		//equidistant from each speaker/headphone side.
		
		//You'll probably want to not use voice monitoring: just mute the track being recorded, or monitor
		//only reverb and echo for vibe. Direct sound is the singer's direct sound.
		
		//The filtering is because, if you use open-back headphones and move your head, highs will
		//bleed through first like a through-zero flange coming out of cancellation (which it is).
		//Therefore, you can filter off highs until the bleed isn't annoying.
		//Or just run with it, it shouldn't be that loud.
		
		//Thanks to Peter Gabriel for many great examples of hit vocals recorded just like this :)
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
