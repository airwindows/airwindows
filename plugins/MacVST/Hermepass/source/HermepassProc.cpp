/* ========================================
 *  Hermepass - Hermepass.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Hermepass_H
#include "Hermepass.h"
#endif

void Hermepass::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double fpOld = 0.618033988749894848204586; //golden ratio!
	double fpNew = 1.0 - fpOld;	
	double rangescale = 0.1 / overallscale;
	
	double cutoff = pow(A,3);
	double slope = pow(B,3) * 6.0;
	
	double newA = cutoff * rangescale;
	double newB = newA; //other part of interleaved IIR is the same
	
	double newC = cutoff * rangescale; //first extra pole is the same
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newD = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newE = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newF = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newG = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newH = cutoff * rangescale;
	//converge toward the unvarying fixed cutoff value
	
	double oldA = 1.0 - newA;
	double oldB = 1.0 - newB;
	double oldC = 1.0 - newC;
	double oldD = 1.0 - newD;
	double oldE = 1.0 - newE;
	double oldF = 1.0 - newF;
	double oldG = 1.0 - newG;
	double oldH = 1.0 - newH;
	
	double polesC;
	double polesD;
	double polesE;
	double polesF;
	double polesG;
	double polesH;
	
	polesC = slope; if (slope > 1.0) polesC = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesD = slope; if (slope > 1.0) polesD = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesE = slope; if (slope > 1.0) polesE = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesF = slope; if (slope > 1.0) polesF = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesG = slope; if (slope > 1.0) polesG = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesH = slope; if (slope > 1.0) polesH = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	//each one will either be 0.0, the fractional slope value, or 1
	
	double inputSampleL;
	double inputSampleR;
	double tempSampleL;
	double tempSampleR;
	double correction;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		tempSampleL = inputSampleL;
		tempSampleR = inputSampleR;

		//begin L channel
		if (fpFlip) {
			iirAL = (iirAL * oldA) + (tempSampleL * newA); tempSampleL -= iirAL; correction = iirAL;
		} else {
			iirBL = (iirBL * oldB) + (tempSampleL * newB); tempSampleL -= iirBL; correction = iirBL;
		}
		iirCL = (iirCL * oldC) + (tempSampleL * newC); tempSampleL -= iirCL;
		iirDL = (iirDL * oldD) + (tempSampleL * newD); tempSampleL -= iirDL;
		iirEL = (iirEL * oldE) + (tempSampleL * newE); tempSampleL -= iirEL;
		iirFL = (iirFL * oldF) + (tempSampleL * newF); tempSampleL -= iirFL;
		iirGL = (iirGL * oldG) + (tempSampleL * newG); tempSampleL -= iirGL;
		iirHL = (iirHL * oldH) + (tempSampleL * newH); tempSampleL -= iirHL;
		//set up all the iir filters in case they are used
		
		if (polesC == 1.0) correction += iirCL; if (polesC > 0.0 && polesC < 1.0) correction += (iirCL * polesC);
		if (polesD == 1.0) correction += iirDL; if (polesD > 0.0 && polesD < 1.0) correction += (iirDL * polesD);
		if (polesE == 1.0) correction += iirEL; if (polesE > 0.0 && polesE < 1.0) correction += (iirEL * polesE);
		if (polesF == 1.0) correction += iirFL; if (polesF > 0.0 && polesF < 1.0) correction += (iirFL * polesF);
		if (polesG == 1.0) correction += iirGL; if (polesG > 0.0 && polesG < 1.0) correction += (iirGL * polesG);
		if (polesH == 1.0) correction += iirHL; if (polesH > 0.0 && polesH < 1.0) correction += (iirHL * polesH);
		//each of these are added directly if they're fully engaged,
		//multiplied by 0-1 if they are the interpolated one, or skipped if they are beyond the interpolated one.
		//the purpose is to do all the math at the floating point exponent nearest to the tiny value in use.
		//also, it's formatted that way to easily substitute the next variable: this could be written as a loop
		//with everything an array value. However, this makes just as much sense for this few poles.
		
		inputSampleL -= correction;
		//end L channel
		
		//begin R channel
		if (fpFlip) {
			iirAR = (iirAR * oldA) + (tempSampleR * newA); tempSampleR -= iirAR; correction = iirAR;
		} else {
			iirBR = (iirBR * oldB) + (tempSampleR * newB); tempSampleR -= iirBR; correction = iirBR;
		}
		iirCR = (iirCR * oldC) + (tempSampleR * newC); tempSampleR -= iirCR;
		iirDR = (iirDR * oldD) + (tempSampleR * newD); tempSampleR -= iirDR;
		iirER = (iirER * oldE) + (tempSampleR * newE); tempSampleR -= iirER;
		iirFR = (iirFR * oldF) + (tempSampleR * newF); tempSampleR -= iirFR;
		iirGR = (iirGR * oldG) + (tempSampleR * newG); tempSampleR -= iirGR;
		iirHR = (iirHR * oldH) + (tempSampleR * newH); tempSampleR -= iirHR;
		//set up all the iir filters in case they are used
		
		if (polesC == 1.0) correction += iirCR; if (polesC > 0.0 && polesC < 1.0) correction += (iirCR * polesC);
		if (polesD == 1.0) correction += iirDR; if (polesD > 0.0 && polesD < 1.0) correction += (iirDR * polesD);
		if (polesE == 1.0) correction += iirER; if (polesE > 0.0 && polesE < 1.0) correction += (iirER * polesE);
		if (polesF == 1.0) correction += iirFR; if (polesF > 0.0 && polesF < 1.0) correction += (iirFR * polesF);
		if (polesG == 1.0) correction += iirGR; if (polesG > 0.0 && polesG < 1.0) correction += (iirGR * polesG);
		if (polesH == 1.0) correction += iirHR; if (polesH > 0.0 && polesH < 1.0) correction += (iirHR * polesH);
		//each of these are added directly if they're fully engaged,
		//multiplied by 0-1 if they are the interpolated one, or skipped if they are beyond the interpolated one.
		//the purpose is to do all the math at the floating point exponent nearest to the tiny value in use.
		//also, it's formatted that way to easily substitute the next variable: this could be written as a loop
		//with everything an array value. However, this makes just as much sense for this few poles.
		
		inputSampleR -= correction;
		//end R channel
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

void Hermepass::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double fpOld = 0.618033988749894848204586; //golden ratio!
	double fpNew = 1.0 - fpOld;	
	double rangescale = 0.1 / overallscale;
	
	double cutoff = pow(A,3);
	double slope = pow(B,3) * 6.0;
	
	double newA = cutoff * rangescale;
	double newB = newA; //other part of interleaved IIR is the same
	
	double newC = cutoff * rangescale; //first extra pole is the same
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newD = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newE = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newF = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newG = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	double newH = cutoff * rangescale;
	//converge toward the unvarying fixed cutoff value
	
	double oldA = 1.0 - newA;
	double oldB = 1.0 - newB;
	double oldC = 1.0 - newC;
	double oldD = 1.0 - newD;
	double oldE = 1.0 - newE;
	double oldF = 1.0 - newF;
	double oldG = 1.0 - newG;
	double oldH = 1.0 - newH;
	
	double polesC;
	double polesD;
	double polesE;
	double polesF;
	double polesG;
	double polesH;
	
	polesC = slope; if (slope > 1.0) polesC = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesD = slope; if (slope > 1.0) polesD = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesE = slope; if (slope > 1.0) polesE = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesF = slope; if (slope > 1.0) polesF = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesG = slope; if (slope > 1.0) polesG = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesH = slope; if (slope > 1.0) polesH = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	//each one will either be 0.0, the fractional slope value, or 1
	
	double inputSampleL;
	double inputSampleR;
	double tempSampleL;
	double tempSampleR;
	double correction;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		tempSampleL = inputSampleL;
		tempSampleR = inputSampleR;
		
		//begin L channel
		if (fpFlip) {
			iirAL = (iirAL * oldA) + (tempSampleL * newA); tempSampleL -= iirAL; correction = iirAL;
		} else {
			iirBL = (iirBL * oldB) + (tempSampleL * newB); tempSampleL -= iirBL; correction = iirBL;
		}
		iirCL = (iirCL * oldC) + (tempSampleL * newC); tempSampleL -= iirCL;
		iirDL = (iirDL * oldD) + (tempSampleL * newD); tempSampleL -= iirDL;
		iirEL = (iirEL * oldE) + (tempSampleL * newE); tempSampleL -= iirEL;
		iirFL = (iirFL * oldF) + (tempSampleL * newF); tempSampleL -= iirFL;
		iirGL = (iirGL * oldG) + (tempSampleL * newG); tempSampleL -= iirGL;
		iirHL = (iirHL * oldH) + (tempSampleL * newH); tempSampleL -= iirHL;
		//set up all the iir filters in case they are used
		
		if (polesC == 1.0) correction += iirCL; if (polesC > 0.0 && polesC < 1.0) correction += (iirCL * polesC);
		if (polesD == 1.0) correction += iirDL; if (polesD > 0.0 && polesD < 1.0) correction += (iirDL * polesD);
		if (polesE == 1.0) correction += iirEL; if (polesE > 0.0 && polesE < 1.0) correction += (iirEL * polesE);
		if (polesF == 1.0) correction += iirFL; if (polesF > 0.0 && polesF < 1.0) correction += (iirFL * polesF);
		if (polesG == 1.0) correction += iirGL; if (polesG > 0.0 && polesG < 1.0) correction += (iirGL * polesG);
		if (polesH == 1.0) correction += iirHL; if (polesH > 0.0 && polesH < 1.0) correction += (iirHL * polesH);
		//each of these are added directly if they're fully engaged,
		//multiplied by 0-1 if they are the interpolated one, or skipped if they are beyond the interpolated one.
		//the purpose is to do all the math at the floating point exponent nearest to the tiny value in use.
		//also, it's formatted that way to easily substitute the next variable: this could be written as a loop
		//with everything an array value. However, this makes just as much sense for this few poles.
		
		inputSampleL -= correction;
		//end L channel
		
		//begin R channel
		if (fpFlip) {
			iirAR = (iirAR * oldA) + (tempSampleR * newA); tempSampleR -= iirAR; correction = iirAR;
		} else {
			iirBR = (iirBR * oldB) + (tempSampleR * newB); tempSampleR -= iirBR; correction = iirBR;
		}
		iirCR = (iirCR * oldC) + (tempSampleR * newC); tempSampleR -= iirCR;
		iirDR = (iirDR * oldD) + (tempSampleR * newD); tempSampleR -= iirDR;
		iirER = (iirER * oldE) + (tempSampleR * newE); tempSampleR -= iirER;
		iirFR = (iirFR * oldF) + (tempSampleR * newF); tempSampleR -= iirFR;
		iirGR = (iirGR * oldG) + (tempSampleR * newG); tempSampleR -= iirGR;
		iirHR = (iirHR * oldH) + (tempSampleR * newH); tempSampleR -= iirHR;
		//set up all the iir filters in case they are used
		
		if (polesC == 1.0) correction += iirCR; if (polesC > 0.0 && polesC < 1.0) correction += (iirCR * polesC);
		if (polesD == 1.0) correction += iirDR; if (polesD > 0.0 && polesD < 1.0) correction += (iirDR * polesD);
		if (polesE == 1.0) correction += iirER; if (polesE > 0.0 && polesE < 1.0) correction += (iirER * polesE);
		if (polesF == 1.0) correction += iirFR; if (polesF > 0.0 && polesF < 1.0) correction += (iirFR * polesF);
		if (polesG == 1.0) correction += iirGR; if (polesG > 0.0 && polesG < 1.0) correction += (iirGR * polesG);
		if (polesH == 1.0) correction += iirHR; if (polesH > 0.0 && polesH < 1.0) correction += (iirHR * polesH);
		//each of these are added directly if they're fully engaged,
		//multiplied by 0-1 if they are the interpolated one, or skipped if they are beyond the interpolated one.
		//the purpose is to do all the math at the floating point exponent nearest to the tiny value in use.
		//also, it's formatted that way to easily substitute the next variable: this could be written as a loop
		//with everything an array value. However, this makes just as much sense for this few poles.
		
		inputSampleR -= correction;
		//end R channel
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