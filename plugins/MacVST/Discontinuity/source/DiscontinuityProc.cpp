/* ========================================
 *  Discontinuity - Discontinuity.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Discontinuity_H
#include "Discontinuity.h"
#endif

void Discontinuity::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double refdB = (A*70.0)+70.0;
	double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= topdB;
		if (inputSampleL < -0.222) inputSampleL = -0.222; if (inputSampleL > 0.222) inputSampleL = 0.222;
		//Air Discontinuity A begin
		dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		int dBdly = floor(dBaPosL*dscBuf);
		double dBi = (dBaPosL*dscBuf)-dBdly;
		inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
		//Air Discontinuity A end
		//Air Discontinuity B begin
		dBbL[dBbXL] = inputSampleL;  dBbPosL *= 0.5; dBbPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		dBdly = floor(dBbPosL*dscBuf); dBi = (dBbPosL*dscBuf)-dBdly;
		inputSampleL = dBbL[dBbXL-dBdly +((dBbXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBbL[dBbXL-dBdly +((dBbXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBbXL++; if (dBbXL < 0 || dBbXL >= dscBuf) dBbXL = 0;
		//Air Discontinuity B end
		//Air Discontinuity C begin
		dBcL[dBcXL] = inputSampleL;  dBcPosL *= 0.5; dBcPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		dBdly = floor(dBcPosL*dscBuf); dBi = (dBcPosL*dscBuf)-dBdly;
		inputSampleL = dBcL[dBcXL-dBdly +((dBcXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBcL[dBcXL-dBdly +((dBcXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBcXL++; if (dBcXL < 0 || dBcXL >= dscBuf) dBcXL = 0;
		//Air Discontinuity C end
		inputSampleL /= topdB;
		
		inputSampleR *= topdB;
		if (inputSampleR < -0.222) inputSampleR = -0.222; if (inputSampleR > 0.222) inputSampleR = 0.222;
		//Air Discontinuity A begin
		dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBaPosR*dscBuf);
		dBi = (dBaPosR*dscBuf)-dBdly;
		inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
		//Air Discontinuity A end
		//Air Discontinuity B begin
		dBbR[dBbXR] = inputSampleR;  dBbPosR *= 0.5; dBbPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBbPosR*dscBuf); dBi = (dBbPosR*dscBuf)-dBdly;
		inputSampleR = dBbR[dBbXR-dBdly +((dBbXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBbR[dBbXR-dBdly +((dBbXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBbXR++; if (dBbXR < 0 || dBbXR >= dscBuf) dBbXR = 0;
		//Air Discontinuity B end
		//Air Discontinuity C begin
		dBcR[dBcXR] = inputSampleR;  dBcPosR *= 0.5; dBcPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBcPosR*dscBuf); dBi = (dBcPosR*dscBuf)-dBdly;
		inputSampleR = dBcR[dBcXR-dBdly +((dBcXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBcR[dBcXR-dBdly +((dBcXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBcXR++; if (dBcXR < 0 || dBcXR >= dscBuf) dBcXR = 0;
		//Air Discontinuity C end
		inputSampleR /= topdB;
		
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

void Discontinuity::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double refdB = (A*70.0)+70.0;
	double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= topdB;
		if (inputSampleL < -0.222) inputSampleL = -0.222; if (inputSampleL > 0.222) inputSampleL = 0.222;
		//Air Discontinuity A begin
		dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		int dBdly = floor(dBaPosL*dscBuf);
		double dBi = (dBaPosL*dscBuf)-dBdly;
		inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
		//Air Discontinuity A end
		//Air Discontinuity B begin
		dBbL[dBbXL] = inputSampleL;  dBbPosL *= 0.5; dBbPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		dBdly = floor(dBbPosL*dscBuf); dBi = (dBbPosL*dscBuf)-dBdly;
		inputSampleL = dBbL[dBbXL-dBdly +((dBbXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBbL[dBbXL-dBdly +((dBbXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBbXL++; if (dBbXL < 0 || dBbXL >= dscBuf) dBbXL = 0;
		//Air Discontinuity B end
		//Air Discontinuity C begin
		dBcL[dBcXL] = inputSampleL;  dBcPosL *= 0.5; dBcPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		dBdly = floor(dBcPosL*dscBuf); dBi = (dBcPosL*dscBuf)-dBdly;
		inputSampleL = dBcL[dBcXL-dBdly +((dBcXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBcL[dBcXL-dBdly +((dBcXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBcXL++; if (dBcXL < 0 || dBcXL >= dscBuf) dBcXL = 0;
		//Air Discontinuity C end
		inputSampleL /= topdB;
		
		inputSampleR *= topdB;
		if (inputSampleR < -0.222) inputSampleR = -0.222; if (inputSampleR > 0.222) inputSampleR = 0.222;
		//Air Discontinuity A begin
		dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBaPosR*dscBuf);
		dBi = (dBaPosR*dscBuf)-dBdly;
		inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
		//Air Discontinuity A end
		//Air Discontinuity B begin
		dBbR[dBbXR] = inputSampleR;  dBbPosR *= 0.5; dBbPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBbPosR*dscBuf); dBi = (dBbPosR*dscBuf)-dBdly;
		inputSampleR = dBbR[dBbXR-dBdly +((dBbXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBbR[dBbXR-dBdly +((dBbXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBbXR++; if (dBbXR < 0 || dBbXR >= dscBuf) dBbXR = 0;
		//Air Discontinuity B end
		//Air Discontinuity C begin
		dBcR[dBcXR] = inputSampleR;  dBcPosR *= 0.5; dBcPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBcPosR*dscBuf); dBi = (dBcPosR*dscBuf)-dBdly;
		inputSampleR = dBcR[dBcXR-dBdly +((dBcXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBcR[dBcXR-dBdly +((dBcXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBcXR++; if (dBcXR < 0 || dBcXR >= dscBuf) dBcXR = 0;
		//Air Discontinuity C end
		inputSampleR /= topdB;
		
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
