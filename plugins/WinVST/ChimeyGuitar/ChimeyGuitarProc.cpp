/* ========================================
 *  ChimeyGuitar - ChimeyGuitar.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ChimeyGuitar_H
#include "ChimeyGuitar.h"
#endif

void ChimeyGuitar::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int poles = (int)(pow(A,2)*20.0);
	angG[0] = sqrt(B*2.0);
	angG[2] = sqrt(C*2.0);
	angG[4] = sqrt(D*2.0);
	angG[6] = sqrt(E*2.0);
	angG[8] = sqrt(F*2.0);
	angG[1] = (angG[0]+angG[2])*0.5;
	angG[3] = (angG[2]+angG[4])*0.5;
	angG[5] = (angG[4]+angG[6])*0.5;
	angG[7] = (angG[6]+angG[8])*0.5;
	angG[9] = angG[8];
	double hFreq = pow(G,overallscale);
	double lFreq = pow(H,overallscale+3.0);
	bezRezA = bezRezB; bezRezB = ((pow(I,4.0)*0.5)+0.0002)/overallscale;
	if (bezRezB > 1.0) bezRezB = 1.0;
	double output = J;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double bezRez = (bezRezA*temp)+(bezRezB*(1.0-temp));
		
		double CBAL = 0.0;
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSampleL; inputSampleL = 0.0;
			for (int y = 0; y < 9; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((band-angSL[x][y])*fr);
				double temp = band; band = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleL += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
				bezComp[bez_cycleL][y] += bezRez;
				bezComp[bez_SampL][y] += (fabs(inputSampleL) * bezRez);
				if (bezComp[bez_cycleL][y] > 1.0) {
					bezComp[bez_cycleL][y] -= 1.0;
					bezComp[bez_CL][y] = bezComp[bez_BL][y];
					bezComp[bez_BL][y] = bezComp[bez_AL][y];
					bezComp[bez_AL][y] = bezComp[bez_SampL][y];
					bezComp[bez_SampL][y] = 0.0;
				}
				double CBL = (bezComp[bez_CL][y]*(1.0-bezComp[bez_cycleL][y]))+(bezComp[bez_BL][y]*bezComp[bez_cycleL][y]);
				double BAL = (bezComp[bez_BL][y]*(1.0-bezComp[bez_cycleL][y]))+(bezComp[bez_AL][y]*bezComp[bez_cycleL][y]);
				CBAL += (bezComp[bez_BL][y]+(CBL*(1.0-bezComp[bez_cycleL][y]))+(BAL*bezComp[bez_cycleL][y]))*0.5;
				inputSampleL *= 1.0-(fmin(CBAL*0.01,1.0));
			}
			inputSampleL += (band*angG[9]);
		}
		inputSampleL = sin(inputSampleL);

		double lowSample = inputSampleL;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngleL[count] = (iirBAngleL[count]*(1.0-lFreq))+((lowSample-iirBPositionL[count])*lFreq);
			lowSample = ((iirBPositionL[count]+(iirBAngleL[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPositionL[count] = ((iirBPositionL[count]+(iirBAngleL[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSampleL -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleL[count] = (iirHAngleL[count]*(1.0-hFreq))+((inputSampleL-iirHPositionL[count])*hFreq);
			inputSampleL = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
			iirHPositionL[count] = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
		} //the lowpass	
		inputSampleL *= output;
		
		double CBAR = 0.0;
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 9; y++) {
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((band-angSR[x][y])*fr);
				double temp = band; band = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleR += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
				bezComp[bez_cycleR][y] += bezRez;
				bezComp[bez_SampR][y] += (fabs(inputSampleR) * bezRez);
				if (bezComp[bez_cycleR][y] > 1.0) {
					bezComp[bez_cycleR][y] -= 1.0;
					bezComp[bez_CR][y] = bezComp[bez_BR][y];
					bezComp[bez_BR][y] = bezComp[bez_AR][y];
					bezComp[bez_AR][y] = bezComp[bez_SampR][y];
					bezComp[bez_SampR][y] = 0.0;
				}
				double CBR = (bezComp[bez_CR][y]*(1.0-bezComp[bez_cycleR][y]))+(bezComp[bez_BR][y]*bezComp[bez_cycleR][y]);
				double BAR = (bezComp[bez_BR][y]*(1.0-bezComp[bez_cycleR][y]))+(bezComp[bez_AR][y]*bezComp[bez_cycleR][y]);
				CBAR += (bezComp[bez_BR][y]+(CBR*(1.0-bezComp[bez_cycleR][y]))+(BAR*bezComp[bez_cycleR][y]))*0.5;
				inputSampleR *= 1.0-(fmin(CBAR*0.01,1.0));
			}
			inputSampleR += (band*angG[9]);
		}
		inputSampleR = sin(inputSampleR);
				
		lowSample = inputSampleR;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngleR[count] = (iirBAngleR[count]*(1.0-lFreq))+((lowSample-iirBPositionR[count])*lFreq);
			lowSample = ((iirBPositionR[count]+(iirBAngleR[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPositionR[count] = ((iirBPositionR[count]+(iirBAngleR[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSampleR -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleR[count] = (iirHAngleR[count]*(1.0-hFreq))+((inputSampleR-iirHPositionR[count])*hFreq);
			inputSampleR = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
			iirHPositionR[count] = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
		} //the lowpass	
		inputSampleR *= output;
		
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

void ChimeyGuitar::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int poles = (int)(pow(A,2)*20.0);
	angG[0] = sqrt(B*2.0);
	angG[2] = sqrt(C*2.0);
	angG[4] = sqrt(D*2.0);
	angG[6] = sqrt(E*2.0);
	angG[8] = sqrt(F*2.0);
	angG[1] = (angG[0]+angG[2])*0.5;
	angG[3] = (angG[2]+angG[4])*0.5;
	angG[5] = (angG[4]+angG[6])*0.5;
	angG[7] = (angG[6]+angG[8])*0.5;
	angG[9] = angG[8];
	double hFreq = pow(G,overallscale);
	double lFreq = pow(H,overallscale+3.0);
	bezRezA = bezRezB; bezRezB = ((pow(I,4.0)*0.5)+0.0002)/overallscale;
	if (bezRezB > 1.0) bezRezB = 1.0;
	double output = J;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double bezRez = (bezRezA*temp)+(bezRezB*(1.0-temp));
		
		double CBAL = 0.0;
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSampleL; inputSampleL = 0.0;
			for (int y = 0; y < 9; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((band-angSL[x][y])*fr);
				double temp = band; band = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleL += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
				bezComp[bez_cycleL][y] += bezRez;
				bezComp[bez_SampL][y] += (fabs(inputSampleL) * bezRez);
				if (bezComp[bez_cycleL][y] > 1.0) {
					bezComp[bez_cycleL][y] -= 1.0;
					bezComp[bez_CL][y] = bezComp[bez_BL][y];
					bezComp[bez_BL][y] = bezComp[bez_AL][y];
					bezComp[bez_AL][y] = bezComp[bez_SampL][y];
					bezComp[bez_SampL][y] = 0.0;
				}
				double CBL = (bezComp[bez_CL][y]*(1.0-bezComp[bez_cycleL][y]))+(bezComp[bez_BL][y]*bezComp[bez_cycleL][y]);
				double BAL = (bezComp[bez_BL][y]*(1.0-bezComp[bez_cycleL][y]))+(bezComp[bez_AL][y]*bezComp[bez_cycleL][y]);
				CBAL += (bezComp[bez_BL][y]+(CBL*(1.0-bezComp[bez_cycleL][y]))+(BAL*bezComp[bez_cycleL][y]))*0.5;
				inputSampleL *= 1.0-(fmin(CBAL*0.01,1.0));
			}
			inputSampleL += (band*angG[9]);
		}
		inputSampleL = sin(inputSampleL);
		
		double lowSample = inputSampleL;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngleL[count] = (iirBAngleL[count]*(1.0-lFreq))+((lowSample-iirBPositionL[count])*lFreq);
			lowSample = ((iirBPositionL[count]+(iirBAngleL[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPositionL[count] = ((iirBPositionL[count]+(iirBAngleL[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSampleL -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleL[count] = (iirHAngleL[count]*(1.0-hFreq))+((inputSampleL-iirHPositionL[count])*hFreq);
			inputSampleL = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
			iirHPositionL[count] = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(inputSampleL*hFreq);
		} //the lowpass	
		inputSampleL *= output;
		
		double CBAR = 0.0;
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 9; y++) {
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((band-angSR[x][y])*fr);
				double temp = band; band = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleR += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
				bezComp[bez_cycleR][y] += bezRez;
				bezComp[bez_SampR][y] += (fabs(inputSampleR) * bezRez);
				if (bezComp[bez_cycleR][y] > 1.0) {
					bezComp[bez_cycleR][y] -= 1.0;
					bezComp[bez_CR][y] = bezComp[bez_BR][y];
					bezComp[bez_BR][y] = bezComp[bez_AR][y];
					bezComp[bez_AR][y] = bezComp[bez_SampR][y];
					bezComp[bez_SampR][y] = 0.0;
				}
				double CBR = (bezComp[bez_CR][y]*(1.0-bezComp[bez_cycleR][y]))+(bezComp[bez_BR][y]*bezComp[bez_cycleR][y]);
				double BAR = (bezComp[bez_BR][y]*(1.0-bezComp[bez_cycleR][y]))+(bezComp[bez_AR][y]*bezComp[bez_cycleR][y]);
				CBAR += (bezComp[bez_BR][y]+(CBR*(1.0-bezComp[bez_cycleR][y]))+(BAR*bezComp[bez_cycleR][y]))*0.5;
				inputSampleR *= 1.0-(fmin(CBAR*0.01,1.0));
			}
			inputSampleR += (band*angG[9]);
		}
		inputSampleR = sin(inputSampleR);
		
		lowSample = inputSampleR;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngleR[count] = (iirBAngleR[count]*(1.0-lFreq))+((lowSample-iirBPositionR[count])*lFreq);
			lowSample = ((iirBPositionR[count]+(iirBAngleR[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPositionR[count] = ((iirBPositionR[count]+(iirBAngleR[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSampleR -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngleR[count] = (iirHAngleR[count]*(1.0-hFreq))+((inputSampleR-iirHPositionR[count])*hFreq);
			inputSampleR = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
			iirHPositionR[count] = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(inputSampleR*hFreq);
		} //the lowpass	
		inputSampleR *= output;
		
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
