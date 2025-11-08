/* ========================================
 *  ChimeyDeluxe - ChimeyDeluxe.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ChimeyDeluxe_H
#include "ChimeyDeluxe.h"
#endif

void ChimeyDeluxe::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drive = 1.0;
	double pad = 1.0;
	angG[1] = (A+1.5)*0.5;
	if (pad > angG[1]) pad = angG[1];
	if (drive < angG[1]) drive = angG[1];
	angG[0] = (1.0+angG[1])*0.5; //if boost, boost half
	if (angG[0] > angG[1]) angG[0] = angG[1];
	angG[2] = (B+1.5)*0.5;
	if (pad > angG[2]) pad = angG[2];
	if (drive < angG[2]) drive = angG[2];
	angG[3] = (C+1.5)*0.5;
	if (pad > angG[3]) pad = angG[3];
	if (drive < angG[3]) drive = angG[3];
	angG[4] = (D+1.5)*0.5;
	if (pad > angG[4]) pad = angG[4];
	if (drive < angG[4]) drive = angG[4];
	angG[5] = (E+1.5)*0.5;
	if (pad > angG[5]) pad = angG[5];
	if (drive < angG[5]) drive = angG[5];
	angG[6] = (F+1.5)*0.5;
	if (pad > angG[6]) pad = angG[6];
	if (drive < angG[6]) drive = angG[6];
	angG[7] = (G+1.5)*0.5;
	if (pad > angG[7]) pad = angG[7];
	if (drive < angG[7]) drive = angG[7];
	angG[8] = (H+1.5)*0.5;
	if (pad > angG[8]) pad = angG[8];
	if (drive < angG[8]) drive = angG[8];
	angG[9] = (I+1.5)*0.5;
	if (pad > angG[9]) pad = angG[9];
	if (drive < angG[9]) drive = angG[9];
	angG[10] = (J+1.5)*0.5;
	if (pad > angG[10]) pad = angG[10];
	if (drive < angG[10]) drive = angG[10];
	angG[12] = angG[11] = angG[10];
	if (drive > 1.0) drive = pow(drive,drive*2.0);
	double tune = 0.618+(overallscale*0.0055);
	double bezRez = (pad * drive * 0.0005)/overallscale;
	int bezFreqFraction = (int)(1.0/bezRez);
	double bezFreqTrim = (double)bezFreqFraction/(bezFreqFraction+1.0);
	bezRez = 1.0 / bezFreqFraction;
	bezFreqTrim = 1.0-(bezRez*bezFreqTrim);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int x = 0; x < 16; x++) {
			double fr = (0.92/overallscale)+(overallscale*0.01);
			double bandL = inputSampleL; inputSampleL = 0.0;
			double bandR = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 12; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((bandL-angSL[x][y])*fr);
				double tempL = bandL; bandL = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(bandL*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(bandL*fr);
				inputSampleL += ((tempL-bandL)*angG[y]);
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((bandR-angSR[x][y])*fr);
				double tempR = bandR; bandR = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(bandR*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(bandR*fr);
				inputSampleR += ((tempR-bandR)*angG[y]);
				fr *= tune;
			}
			inputSampleL += bandL;
			inputSampleR += bandR; //end of filter part
			
			bezComp[bez_cycle][x] += bezRez;
			bezComp[bez_SampL][x] += (fabs(inputSampleL) * bezRez);
			bezComp[bez_SampR][x] += (fabs(inputSampleR) * bezRez);
			if (bezComp[bez_cycle][x] > 1.0) {
				bezComp[bez_cycle][x] = 0.0;
				bezComp[bez_CL][x] = bezComp[bez_BL][x];
				bezComp[bez_BL][x] = bezComp[bez_AL][x];
				bezComp[bez_AL][x] = bezComp[bez_SampL][x];
				bezComp[bez_SampL][x] = 0.0;
				bezComp[bez_CR][x] = bezComp[bez_BR][x];
				bezComp[bez_BR][x] = bezComp[bez_AR][x];
				bezComp[bez_AR][x] = bezComp[bez_SampR][x];
				bezComp[bez_SampR][x] = 0.0;
			}
			double z = bezComp[bez_cycle][x]*bezFreqTrim;
			double CBL = (bezComp[bez_CL][x]*(1.0-z))+(bezComp[bez_BL][x]*z);
			double BAL = (bezComp[bez_BL][x]*(1.0-z))+(bezComp[bez_AL][x]*z);
			double CBAL = (bezComp[bez_BL][x]+(CBL*(1.0-z))+(BAL*z));
			
			double CBR = (bezComp[bez_CR][x]*(1.0-z))+(bezComp[bez_BR][x]*z);
			double BAR = (bezComp[bez_BR][x]*(1.0-z))+(bezComp[bez_AR][x]*z);
			double CBAR = (bezComp[bez_BR][x]+(CBR*(1.0-z))+(BAR*z));
			
			CBAL = fmin(fmax(CBAL*drive*0.23,0.0),M_PI_2);
			CBAR = fmin(fmax(CBAR*drive*0.23,0.0),M_PI_2);
			inputSampleL *= 1.0-sin(CBAL);
			inputSampleR *= 1.0-sin(CBAR);
		}
		inputSampleL /= drive;
		inputSampleR /= drive;
		
		inputSampleL = fmin(fmax(inputSampleL*pad,-1.0),1.0);
		inputSampleR = fmin(fmax(inputSampleR*pad,-1.0),1.0);
		
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

void ChimeyDeluxe::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drive = 1.0;
	double pad = 1.0;
	angG[1] = (A+1.5)*0.5;
	if (pad > angG[1]) pad = angG[1];
	if (drive < angG[1]) drive = angG[1];
	angG[0] = (1.0+angG[1])*0.5; //if boost, boost half
	if (angG[0] > angG[1]) angG[0] = angG[1];
	angG[2] = (B+1.5)*0.5;
	if (pad > angG[2]) pad = angG[2];
	if (drive < angG[2]) drive = angG[2];
	angG[3] = (C+1.5)*0.5;
	if (pad > angG[3]) pad = angG[3];
	if (drive < angG[3]) drive = angG[3];
	angG[4] = (D+1.5)*0.5;
	if (pad > angG[4]) pad = angG[4];
	if (drive < angG[4]) drive = angG[4];
	angG[5] = (E+1.5)*0.5;
	if (pad > angG[5]) pad = angG[5];
	if (drive < angG[5]) drive = angG[5];
	angG[6] = (F+1.5)*0.5;
	if (pad > angG[6]) pad = angG[6];
	if (drive < angG[6]) drive = angG[6];
	angG[7] = (G+1.5)*0.5;
	if (pad > angG[7]) pad = angG[7];
	if (drive < angG[7]) drive = angG[7];
	angG[8] = (H+1.5)*0.5;
	if (pad > angG[8]) pad = angG[8];
	if (drive < angG[8]) drive = angG[8];
	angG[9] = (I+1.5)*0.5;
	if (pad > angG[9]) pad = angG[9];
	if (drive < angG[9]) drive = angG[9];
	angG[10] = (J+1.5)*0.5;
	if (pad > angG[10]) pad = angG[10];
	if (drive < angG[10]) drive = angG[10];
	angG[12] = angG[11] = angG[10];
	if (drive > 1.0) drive = pow(drive,drive*2.0);
	double tune = 0.618+(overallscale*0.0055);
	double bezRez = (pad * drive * 0.0005)/overallscale;
	int bezFreqFraction = (int)(1.0/bezRez);
	double bezFreqTrim = (double)bezFreqFraction/(bezFreqFraction+1.0);
	bezRez = 1.0 / bezFreqFraction;
	bezFreqTrim = 1.0-(bezRez*bezFreqTrim);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int x = 0; x < 16; x++) {
			double fr = (0.92/overallscale)+(overallscale*0.01);
			double bandL = inputSampleL; inputSampleL = 0.0;
			double bandR = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 12; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((bandL-angSL[x][y])*fr);
				double tempL = bandL; bandL = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(bandL*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(bandL*fr);
				inputSampleL += ((tempL-bandL)*angG[y]);
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((bandR-angSR[x][y])*fr);
				double tempR = bandR; bandR = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(bandR*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(bandR*fr);
				inputSampleR += ((tempR-bandR)*angG[y]);
				fr *= tune;
			}
			inputSampleL += bandL;
			inputSampleR += bandR; //end of filter part
			
			bezComp[bez_cycle][x] += bezRez;
			bezComp[bez_SampL][x] += (fabs(inputSampleL) * bezRez);
			bezComp[bez_SampR][x] += (fabs(inputSampleR) * bezRez);
			if (bezComp[bez_cycle][x] > 1.0) {
				bezComp[bez_cycle][x] = 0.0;
				bezComp[bez_CL][x] = bezComp[bez_BL][x];
				bezComp[bez_BL][x] = bezComp[bez_AL][x];
				bezComp[bez_AL][x] = bezComp[bez_SampL][x];
				bezComp[bez_SampL][x] = 0.0;
				bezComp[bez_CR][x] = bezComp[bez_BR][x];
				bezComp[bez_BR][x] = bezComp[bez_AR][x];
				bezComp[bez_AR][x] = bezComp[bez_SampR][x];
				bezComp[bez_SampR][x] = 0.0;
			}
			double z = bezComp[bez_cycle][x]*bezFreqTrim;
			double CBL = (bezComp[bez_CL][x]*(1.0-z))+(bezComp[bez_BL][x]*z);
			double BAL = (bezComp[bez_BL][x]*(1.0-z))+(bezComp[bez_AL][x]*z);
			double CBAL = (bezComp[bez_BL][x]+(CBL*(1.0-z))+(BAL*z));
			
			double CBR = (bezComp[bez_CR][x]*(1.0-z))+(bezComp[bez_BR][x]*z);
			double BAR = (bezComp[bez_BR][x]*(1.0-z))+(bezComp[bez_AR][x]*z);
			double CBAR = (bezComp[bez_BR][x]+(CBR*(1.0-z))+(BAR*z));
			
			CBAL = fmin(fmax(CBAL*drive*0.23,0.0),M_PI_2);
			CBAR = fmin(fmax(CBAR*drive*0.23,0.0),M_PI_2);
			inputSampleL *= 1.0-sin(CBAL);
			inputSampleR *= 1.0-sin(CBAR);
		}
		inputSampleL /= drive;
		inputSampleR /= drive;
		
		inputSampleL = fmin(fmax(inputSampleL*pad,-1.0),1.0);
		inputSampleR = fmin(fmax(inputSampleR*pad,-1.0),1.0);
		
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
