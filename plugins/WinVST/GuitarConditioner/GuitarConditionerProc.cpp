/* ========================================
 *  GuitarConditioner - GuitarConditioner.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __GuitarConditioner_H
#include "GuitarConditioner.h"
#endif

void GuitarConditioner::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double inputSampleL;
	double inputSampleR;
	double trebleL;
	double bassL;
	double trebleR;
	double bassR;
	
	double iirTreble = 0.287496/overallscale; //tight is -1
	double iirBass = 0.085184/overallscale; //tight is 1
	iirTreble += iirTreble;
	iirBass += iirBass; //simple double when tight is -1 or 1
	double tightBass = 0.6666666666;
	double tightTreble = -0.3333333333;
	double offset;
	double clamp;
	double threshTreble = 0.0081/overallscale;
	double threshBass = 0.0256/overallscale;
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		trebleL = bassL = inputSampleL;
		trebleR = bassR = inputSampleR;
		trebleL += trebleL; //+3dB on treble as the highpass is higher
		trebleR += trebleR; //+3dB on treble as the highpass is higher
		
		offset = (1 + tightTreble) + ((1-fabs(trebleL))*tightTreble); //treble HP
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1; //made offset for HP
		if (fpFlip) {
			iirSampleTAL = (iirSampleTAL * (1 - (offset * iirTreble))) + (trebleL * (offset * iirTreble));
			trebleL = trebleL - iirSampleTAL;
		} else {
			iirSampleTBL = (iirSampleTBL * (1 - (offset * iirTreble))) + (trebleL * (offset * iirTreble));
			trebleL = trebleL - iirSampleTBL;
		} //done trebleL HP

		offset = (1 + tightTreble) + ((1-fabs(trebleR))*tightTreble); //treble HP
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1; //made offset for HP
		if (fpFlip) {
			iirSampleTAR = (iirSampleTAR * (1 - (offset * iirTreble))) + (trebleR * (offset * iirTreble));
			trebleR = trebleR - iirSampleTAR;
		} else {
			iirSampleTBR = (iirSampleTBR * (1 - (offset * iirTreble))) + (trebleR * (offset * iirTreble));
			trebleR = trebleR - iirSampleTBR;
		} //done trebleR HP
		
		offset = (1 - tightBass) + (fabs(bassL)*tightBass); //bass HP
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		if (fpFlip) {
			iirSampleBAL = (iirSampleBAL * (1 - (offset * iirBass))) + (bassL * (offset * iirBass));
			bassL = bassL - iirSampleBAL;
		} else {
			iirSampleBBL = (iirSampleBBL * (1 - (offset * iirBass))) + (bassL * (offset * iirBass));
			bassL = bassL - iirSampleBBL;
		} //done bassL HP
		
		offset = (1 - tightBass) + (fabs(bassR)*tightBass); //bass HP
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		if (fpFlip) {
			iirSampleBAR = (iirSampleBAR * (1 - (offset * iirBass))) + (bassR * (offset * iirBass));
			bassR = bassR - iirSampleBAR;
		} else {
			iirSampleBBR = (iirSampleBBR * (1 - (offset * iirBass))) + (bassR * (offset * iirBass));
			bassR = bassR - iirSampleBBR;
		} //done bassR HP
		
		inputSampleL = trebleL;
		clamp = inputSampleL - lastSampleTL;
		if (clamp > threshTreble)
			trebleL = lastSampleTL + threshTreble;
		if (-clamp > threshTreble)
			trebleL = lastSampleTL - threshTreble;
		lastSampleTL = trebleL; //trebleL slew
		
		inputSampleR = trebleR;
		clamp = inputSampleR - lastSampleTR;
		if (clamp > threshTreble)
			trebleR = lastSampleTR + threshTreble;
		if (-clamp > threshTreble)
			trebleR = lastSampleTR - threshTreble;
		lastSampleTR = trebleR; //trebleR slew
		
		inputSampleL = bassL;
		clamp = inputSampleL - lastSampleBL;
		if (clamp > threshBass)
			bassL = lastSampleBL + threshBass;
		if (-clamp > threshBass)
			bassL = lastSampleBL - threshBass;
		lastSampleBL = bassL; //bassL slew
		
		inputSampleR = bassR;
		clamp = inputSampleR - lastSampleBR;
		if (clamp > threshBass)
			bassR = lastSampleBR + threshBass;
		if (-clamp > threshBass)
			bassR = lastSampleBR - threshBass;
		lastSampleBR = bassR; //bassR slew
		
		inputSampleL = trebleL + bassL; //final merge
		inputSampleR = trebleR + bassR; //final merge	
		fpFlip = !fpFlip;

	
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

void GuitarConditioner::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double inputSampleL;
	double inputSampleR;
	double trebleL;
	double bassL;
	double trebleR;
	double bassR;
	
	double iirTreble = 0.287496/overallscale; //tight is -1
	double iirBass = 0.085184/overallscale; //tight is 1
	iirTreble += iirTreble;
	iirBass += iirBass; //simple double when tight is -1 or 1
	double tightBass = 0.6666666666;
	double tightTreble = -0.3333333333;
	double offset;
	double clamp;
	double threshTreble = 0.0081/overallscale;
	double threshBass = 0.0256/overallscale;
	

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		trebleL = bassL = inputSampleL;
		trebleR = bassR = inputSampleR;
		trebleL += trebleL; //+3dB on treble as the highpass is higher
		trebleR += trebleR; //+3dB on treble as the highpass is higher		
		
		offset = (1 + tightTreble) + ((1-fabs(trebleL))*tightTreble); //treble HP
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1; //made offset for HP
		if (fpFlip) {
			iirSampleTAL = (iirSampleTAL * (1 - (offset * iirTreble))) + (trebleL * (offset * iirTreble));
			trebleL = trebleL - iirSampleTAL;
		} else {
			iirSampleTBL = (iirSampleTBL * (1 - (offset * iirTreble))) + (trebleL * (offset * iirTreble));
			trebleL = trebleL - iirSampleTBL;
		} //done trebleL HP
		
		offset = (1 + tightTreble) + ((1-fabs(trebleR))*tightTreble); //treble HP
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1; //made offset for HP
		if (fpFlip) {
			iirSampleTAR = (iirSampleTAR * (1 - (offset * iirTreble))) + (trebleR * (offset * iirTreble));
			trebleR = trebleR - iirSampleTAR;
		} else {
			iirSampleTBR = (iirSampleTBR * (1 - (offset * iirTreble))) + (trebleR * (offset * iirTreble));
			trebleR = trebleR - iirSampleTBR;
		} //done trebleR HP
		
		offset = (1 - tightBass) + (fabs(bassL)*tightBass); //bass HP
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		if (fpFlip) {
			iirSampleBAL = (iirSampleBAL * (1 - (offset * iirBass))) + (bassL * (offset * iirBass));
			bassL = bassL - iirSampleBAL;
		} else {
			iirSampleBBL = (iirSampleBBL * (1 - (offset * iirBass))) + (bassL * (offset * iirBass));
			bassL = bassL - iirSampleBBL;
		} //done bassL HP
		
		offset = (1 - tightBass) + (fabs(bassR)*tightBass); //bass HP
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		if (fpFlip) {
			iirSampleBAR = (iirSampleBAR * (1 - (offset * iirBass))) + (bassR * (offset * iirBass));
			bassR = bassR - iirSampleBAR;
		} else {
			iirSampleBBR = (iirSampleBBR * (1 - (offset * iirBass))) + (bassR * (offset * iirBass));
			bassR = bassR - iirSampleBBR;
		} //done bassR HP
		
		inputSampleL = trebleL;
		clamp = inputSampleL - lastSampleTL;
		if (clamp > threshTreble)
			trebleL = lastSampleTL + threshTreble;
		if (-clamp > threshTreble)
			trebleL = lastSampleTL - threshTreble;
		lastSampleTL = trebleL; //trebleL slew
		
		inputSampleR = trebleR;
		clamp = inputSampleR - lastSampleTR;
		if (clamp > threshTreble)
			trebleR = lastSampleTR + threshTreble;
		if (-clamp > threshTreble)
			trebleR = lastSampleTR - threshTreble;
		lastSampleTR = trebleR; //trebleR slew
		
		inputSampleL = bassL;
		clamp = inputSampleL - lastSampleBL;
		if (clamp > threshBass)
			bassL = lastSampleBL + threshBass;
		if (-clamp > threshBass)
			bassL = lastSampleBL - threshBass;
		lastSampleBL = bassL; //bassL slew
		
		inputSampleR = bassR;
		clamp = inputSampleR - lastSampleBR;
		if (clamp > threshBass)
			bassR = lastSampleBR + threshBass;
		if (-clamp > threshBass)
			bassR = lastSampleBR - threshBass;
		lastSampleBR = bassR; //bassR slew
				
		inputSampleL = trebleL + bassL; //final merge
		inputSampleR = trebleR + bassR; //final merge	
		fpFlip = !fpFlip;

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