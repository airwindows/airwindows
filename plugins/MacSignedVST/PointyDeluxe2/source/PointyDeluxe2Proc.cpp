/* ========================================
 *  PointyDeluxe2 - PointyDeluxe2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PointyDeluxe2_H
#include "PointyDeluxe2.h"
#endif

void PointyDeluxe2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	angG[0] = 1.0;
	angG[1] = A*2.0;
	angG[10] = J+0.5;
	double stepValue = (angG[10] - angG[1])*0.1;
	for(int y=1; y < 15; y++) angG[y] = fmax(angG[1]+(stepValue*y),0.0);
	double drive = angG[1];
	angG[2] *= B+0.5;
	if (drive < angG[2]) drive = angG[2];
	angG[3] *= C+0.5;
	if (drive < angG[3]) drive = angG[3];
	angG[4] *= D+0.5;
	if (drive < angG[4]) drive = angG[4];
	angG[5] *= E+0.5;
	if (drive < angG[5]) drive = angG[5];
	angG[6] *= F+0.5;
	if (drive < angG[6]) drive = angG[6];
	angG[7] *= G+0.5;
	if (drive < angG[7]) drive = angG[7];
	angG[8] *= H+0.5;
	if (drive < angG[8]) drive = angG[8];
	angG[9] *= I+0.5;
	if (drive < angG[9]) drive = angG[9];
	double tune = 0.618+(overallscale*0.0055);
	double pad = sqrt(1.0/drive);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int x = 0; x < 14; x++) {
			inputSampleL *= drive;
			double bip_delta = inputSampleL; //delta can be just local and re-used
			inputSampleL = fmin(fmax(inputSampleL,-1.0),1.0);
			inputSampleL -= inputSampleL*(inputSampleL*0.654)*(inputSampleL*0.654);
			bip[bip_dvLA][x] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
			bip[bip_dvLB][x] = bip[bip_pvLA][x]-bip[bip_dvLA][x]; bip[bip_pvLA][x] = bip[bip_dvLA][x];//velocity
			bip[bip_dvLC][x] = bip[bip_pvLB][x]-bip[bip_dvLB][x]; bip[bip_pvLB][x] = bip[bip_dvLB][x];//acceleration
			bip[bip_dvLD][x] = bip[bip_pvLC][x]-bip[bip_dvLC][x]; bip[bip_pvLC][x] = bip[bip_dvLC][x];//jerk
			double bip_dvE = bip[bip_pvLD][x]-bip[bip_dvLD][x]; bip[bip_pvLD][x] = bip[bip_dvLD][x];//snap
			inputSampleL *= (1.0+(fabs(bip[bip_dvLC][x])*0.0618)+(fabs(bip[bip_dvLD][x])*-0.05982)+(fabs(bip_dvE)*0.0206));
			double fr = (0.92/overallscale)+(overallscale*0.01);
			double band = inputSampleL; inputSampleL = 0.0;
			for (int y = 0; y < 14; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((band-angSL[x][y])*fr);
				double temp = band; band = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleL += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSampleL += band*angG[14];
			
			inputSampleR *= drive;
			bip_delta = inputSampleR; //delta can be just local and re-used
			inputSampleR = fmin(fmax(inputSampleR,-1.0),1.0);
			inputSampleR -= inputSampleR*(inputSampleR*0.654)*(inputSampleR*0.654);
			bip[bip_dvRA][x] = bip_delta - inputSampleR; // these are derivatives: raw clip is position
			bip[bip_dvRB][x] = bip[bip_pvRA][x]-bip[bip_dvRA][x]; bip[bip_pvRA][x] = bip[bip_dvRA][x];//velocity
			bip[bip_dvRC][x] = bip[bip_pvRB][x]-bip[bip_dvRB][x]; bip[bip_pvRB][x] = bip[bip_dvRB][x];//acceleration
			bip[bip_dvRD][x] = bip[bip_pvRC][x]-bip[bip_dvRC][x]; bip[bip_pvRC][x] = bip[bip_dvRC][x];//jerk
			bip_dvE = bip[bip_pvRD][x]-bip[bip_dvRD][x]; bip[bip_pvRD][x] = bip[bip_dvRD][x];//snap
			inputSampleR *= (1.0+(fabs(bip[bip_dvRC][x])*0.0618)+(fabs(bip[bip_dvRD][x])*-0.05982)+(fabs(bip_dvE)*0.0206));
			fr = (0.92/overallscale)+(overallscale*0.01);
			band = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 14; y++) {
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((band-angSR[x][y])*fr);
				double temp = band; band = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleR += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSampleR += band*angG[14];
		}
		
		if (pad < 1.0) {
			inputSampleL *= pad;
			inputSampleR *= pad;
		}
		
		double bop_delta = inputSampleL; //delta can be just local and re-used
		inputSampleL = fmin(fmax(inputSampleL,-M_PI_2),M_PI_2);
		long double X = inputSampleL; X *= X; //long double for even
		long double temp = inputSampleL * X; //the initial multiplies
		inputSampleL -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSampleL += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSampleL -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSampleL += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSampleL -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSampleL += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSampleL -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSampleL += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSampleL -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSampleL += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		bop[bip_dvLA] = bop_delta - inputSampleL; // these are derivatives: raw clip is position
		bop[bip_dvLB] = bop[bip_pvLA]-bop[bip_dvLA]; bop[bip_pvLA] = bop[bip_dvLA];//velocity
		bop[bip_dvLC] = bop[bip_pvLB]-bop[bip_dvLB]; bop[bip_pvLB] = bop[bip_dvLB];//acceleration
		bop[bip_dvLD] = bop[bip_pvLC]-bop[bip_dvLC]; bop[bip_pvLC] = bop[bip_dvLC];//jerk
		double bop_dvE = bop[bip_pvLD]-bop[bip_dvLD]; bop[bip_pvLD] = bop[bip_dvLD];//snap
		inputSampleL *= (1.0+(fabs(bop[bip_dvLC])*0.0618)+(fabs(bop[bip_dvLD])*-0.05982)+(fabs(bop_dvE)*0.0206));
		
		bop_delta = inputSampleR; //delta can be just local and re-used
		inputSampleR = fmin(fmax(inputSampleR,-M_PI_2),M_PI_2);
		X = inputSampleR; X *= X; //long double for even
		temp = inputSampleR * X; //the initial multiplies
		inputSampleR -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSampleR += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSampleR -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSampleR += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSampleR -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSampleR += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSampleR -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSampleR += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSampleR -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSampleR += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		bop[bip_dvRA] = bop_delta - inputSampleR; // these are derivatives: raw clip is position
		bop[bip_dvRB] = bop[bip_pvRA]-bop[bip_dvRA]; bop[bip_pvRA] = bop[bip_dvRA];//velocity
		bop[bip_dvRC] = bop[bip_pvRB]-bop[bip_dvRB]; bop[bip_pvRB] = bop[bip_dvRB];//acceleration
		bop[bip_dvRD] = bop[bip_pvRC]-bop[bip_dvRC]; bop[bip_pvRC] = bop[bip_dvRC];//jerk
		bop_dvE = bop[bip_pvRD]-bop[bip_dvRD]; bop[bip_pvRD] = bop[bip_dvRD];//snap
		inputSampleR *= (1.0+(fabs(bop[bip_dvRC])*0.0618)+(fabs(bop[bip_dvRD])*-0.05982)+(fabs(bop_dvE)*0.0206));
		
		if (pad < 1.0) {
			inputSampleL *= pad;
			inputSampleR *= pad;
		}
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void PointyDeluxe2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	angG[0] = 1.0;
	angG[1] = A*2.0;
	angG[10] = J+0.5;
	double stepValue = (angG[10] - angG[1])*0.1;
	for(int y=1; y < 15; y++) angG[y] = fmax(angG[1]+(stepValue*y),0.0);
	double drive = angG[1];
	angG[2] *= B+0.5;
	if (drive < angG[2]) drive = angG[2];
	angG[3] *= C+0.5;
	if (drive < angG[3]) drive = angG[3];
	angG[4] *= D+0.5;
	if (drive < angG[4]) drive = angG[4];
	angG[5] *= E+0.5;
	if (drive < angG[5]) drive = angG[5];
	angG[6] *= F+0.5;
	if (drive < angG[6]) drive = angG[6];
	angG[7] *= G+0.5;
	if (drive < angG[7]) drive = angG[7];
	angG[8] *= H+0.5;
	if (drive < angG[8]) drive = angG[8];
	angG[9] *= I+0.5;
	if (drive < angG[9]) drive = angG[9];
	double tune = 0.618+(overallscale*0.0055);
	double pad = sqrt(1.0/drive);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int x = 0; x < 14; x++) {
			inputSampleL *= drive;
			double bip_delta = inputSampleL; //delta can be just local and re-used
			inputSampleL = fmin(fmax(inputSampleL,-1.0),1.0);
			inputSampleL -= inputSampleL*(inputSampleL*0.654)*(inputSampleL*0.654);
			bip[bip_dvLA][x] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
			bip[bip_dvLB][x] = bip[bip_pvLA][x]-bip[bip_dvLA][x]; bip[bip_pvLA][x] = bip[bip_dvLA][x];//velocity
			bip[bip_dvLC][x] = bip[bip_pvLB][x]-bip[bip_dvLB][x]; bip[bip_pvLB][x] = bip[bip_dvLB][x];//acceleration
			bip[bip_dvLD][x] = bip[bip_pvLC][x]-bip[bip_dvLC][x]; bip[bip_pvLC][x] = bip[bip_dvLC][x];//jerk
			double bip_dvE = bip[bip_pvLD][x]-bip[bip_dvLD][x]; bip[bip_pvLD][x] = bip[bip_dvLD][x];//snap
			inputSampleL *= (1.0+(fabs(bip[bip_dvLC][x])*0.0618)+(fabs(bip[bip_dvLD][x])*-0.05982)+(fabs(bip_dvE)*0.0206));
			double fr = (0.92/overallscale)+(overallscale*0.01);
			double band = inputSampleL; inputSampleL = 0.0;
			for (int y = 0; y < 14; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((band-angSL[x][y])*fr);
				double temp = band; band = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleL += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSampleL += band*angG[14];
			
			inputSampleR *= drive;
			bip_delta = inputSampleR; //delta can be just local and re-used
			inputSampleR = fmin(fmax(inputSampleR,-1.0),1.0);
			inputSampleR -= inputSampleR*(inputSampleR*0.654)*(inputSampleR*0.654);
			bip[bip_dvRA][x] = bip_delta - inputSampleR; // these are derivatives: raw clip is position
			bip[bip_dvRB][x] = bip[bip_pvRA][x]-bip[bip_dvRA][x]; bip[bip_pvRA][x] = bip[bip_dvRA][x];//velocity
			bip[bip_dvRC][x] = bip[bip_pvRB][x]-bip[bip_dvRB][x]; bip[bip_pvRB][x] = bip[bip_dvRB][x];//acceleration
			bip[bip_dvRD][x] = bip[bip_pvRC][x]-bip[bip_dvRC][x]; bip[bip_pvRC][x] = bip[bip_dvRC][x];//jerk
			bip_dvE = bip[bip_pvRD][x]-bip[bip_dvRD][x]; bip[bip_pvRD][x] = bip[bip_dvRD][x];//snap
			inputSampleR *= (1.0+(fabs(bip[bip_dvRC][x])*0.0618)+(fabs(bip[bip_dvRD][x])*-0.05982)+(fabs(bip_dvE)*0.0206));
			fr = (0.92/overallscale)+(overallscale*0.01);
			band = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 14; y++) {
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((band-angSR[x][y])*fr);
				double temp = band; band = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleR += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSampleR += band*angG[14];
		}
		
		if (pad < 1.0) {
			inputSampleL *= pad;
			inputSampleR *= pad;
		}
		
		double bop_delta = inputSampleL; //delta can be just local and re-used
		inputSampleL = fmin(fmax(inputSampleL,-M_PI_2),M_PI_2);
		long double X = inputSampleL; X *= X; //long double for even
		long double temp = inputSampleL * X; //the initial multiplies
		inputSampleL -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSampleL += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSampleL -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSampleL += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSampleL -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSampleL += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSampleL -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSampleL += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSampleL -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSampleL += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		bop[bip_dvLA] = bop_delta - inputSampleL; // these are derivatives: raw clip is position
		bop[bip_dvLB] = bop[bip_pvLA]-bop[bip_dvLA]; bop[bip_pvLA] = bop[bip_dvLA];//velocity
		bop[bip_dvLC] = bop[bip_pvLB]-bop[bip_dvLB]; bop[bip_pvLB] = bop[bip_dvLB];//acceleration
		bop[bip_dvLD] = bop[bip_pvLC]-bop[bip_dvLC]; bop[bip_pvLC] = bop[bip_dvLC];//jerk
		double bop_dvE = bop[bip_pvLD]-bop[bip_dvLD]; bop[bip_pvLD] = bop[bip_dvLD];//snap
		inputSampleL *= (1.0+(fabs(bop[bip_dvLC])*0.0618)+(fabs(bop[bip_dvLD])*-0.05982)+(fabs(bop_dvE)*0.0206));
		
		bop_delta = inputSampleR; //delta can be just local and re-used
		inputSampleR = fmin(fmax(inputSampleR,-M_PI_2),M_PI_2);
		X = inputSampleR; X *= X; //long double for even
		temp = inputSampleR * X; //the initial multiplies
		inputSampleR -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSampleR += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSampleR -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSampleR += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSampleR -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSampleR += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSampleR -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSampleR += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSampleR -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSampleR += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		bop[bip_dvRA] = bop_delta - inputSampleR; // these are derivatives: raw clip is position
		bop[bip_dvRB] = bop[bip_pvRA]-bop[bip_dvRA]; bop[bip_pvRA] = bop[bip_dvRA];//velocity
		bop[bip_dvRC] = bop[bip_pvRB]-bop[bip_dvRB]; bop[bip_pvRB] = bop[bip_dvRB];//acceleration
		bop[bip_dvRD] = bop[bip_pvRC]-bop[bip_dvRC]; bop[bip_pvRC] = bop[bip_dvRC];//jerk
		bop_dvE = bop[bip_pvRD]-bop[bip_dvRD]; bop[bip_pvRD] = bop[bip_dvRD];//snap
		inputSampleR *= (1.0+(fabs(bop[bip_dvRC])*0.0618)+(fabs(bop[bip_dvRD])*-0.05982)+(fabs(bop_dvE)*0.0206));
		
		if (pad < 1.0) {
			inputSampleL *= pad;
			inputSampleR *= pad;
		}
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l  * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
