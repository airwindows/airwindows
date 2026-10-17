/* ========================================
 *  WoodenChest - WoodenChest.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __WoodenChest_H
#include "WoodenChest.h"
#endif

void WoodenChest::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int clearcoat = (int)(A*16.999);
	if (clearcoat != prevclearcoat) {
		for(int x = 0; x < d4A+2; x++) {lA[x] = 0.0f; rA[x] = 0.0f;}
		for(int x = 0; x < d4B+2; x++) {lB[x] = 0.0f; rB[x] = 0.0f;}
		for(int x = 0; x < d4C+2; x++) {lC[x] = 0.0f; rC[x] = 0.0f;}
		for(int x = 0; x < d4D+2; x++) {lD[x] = 0.0f; rD[x] = 0.0f;}
		for(int x = 0; x < d4E+2; x++) {lE[x] = 0.0f; rE[x] = 0.0f;}
		for(int x = 0; x < d4F+2; x++) {lF[x] = 0.0f; rF[x] = 0.0f;}
		for(int x = 0; x < d4G+2; x++) {lG[x] = 0.0f; rG[x] = 0.0f;}
		for(int x = 0; x < d4H+2; x++) {lH[x] = 0.0f; rH[x] = 0.0f;}
		for(int x = 0; x < d4I+2; x++) {lI[x] = 0.0f; rI[x] = 0.0f;}
		for(int x = 0; x < d4J+2; x++) {lJ[x] = 0.0f; rJ[x] = 0.0f;}
		for(int x = 0; x < d4K+2; x++) {lK[x] = 0.0f; rK[x] = 0.0f;}
		for(int x = 0; x < d4L+2; x++) {lL[x] = 0.0f; rL[x] = 0.0f;}
		for(int x = 0; x < d4M+2; x++) {lM[x] = 0.0f; rM[x] = 0.0f;}
		for(int x = 0; x < d4N+2; x++) {lN[x] = 0.0f; rN[x] = 0.0f;}
		for(int x = 0; x < d4O+2; x++) {lO[x] = 0.0f; rO[x] = 0.0f;}
		for(int x = 0; x < d4P+2; x++) {lP[x] = 0.0f; rP[x] = 0.0f;}
		cLA = cLB = cLC = cLD = cLE = cLF = cLG = cLH = 1;
		cLI = cLJ = cLK = cLL = cLM = cLN = cLO = cLP = 1;
		cRA = cRB = cRC = cRD = cRE = cRF = cRG = cRH = 1;
		cRI = cRJ = cRK = cRL = cRM = cRN = cRO = cRP = 1;
		
		//WoodenChest
		switch (clearcoat)
		{
			case 0:
				shortA = 350; shortB = 10; shortC = 21; shortD = 76; shortE = 80; shortF = 72; shortG = 40; shortH = 108; shortI = 83; shortJ = 7; shortK = 47; shortL = 33; shortM = 38; shortN = 65; shortO = 29; shortP = 5; break; //1 to 13 ms, 6 seat room  
			case 1:
				shortA = 4; shortB = 134; shortC = 90; shortD = 29; shortE = 70; shortF = 71; shortG = 23; shortH = 22; shortI = 96; shortJ = 135; shortK = 115; shortL = 23; shortM = 390; shortN = 59; shortO = 9; shortP = 26; break; //1 to 16 ms, 9 seat room  
			case 2:
				shortA = 51; shortB = 131; shortC = 80; shortD = 6; shortE = 95; shortF = 414; shortG = 88; shortH = 40; shortI = 113; shortJ = 19; shortK = 65; shortL = 47; shortM = 15; shortN = 131; shortO = 33; shortP = 7; break; //1 to 17 ms, 11 seat room  
			case 3:
				shortA = 75; shortB = 44; shortC = 164; shortD = 95; shortE = 26; shortF = 123; shortG = 115; shortH = 69; shortI = 386; shortJ = 57; shortK = 8; shortL = 90; shortM = 28; shortN = 32; shortO = 4; shortP = 37; break; //1 to 17 ms, 11 seat room  
			case 4:
				shortA = 17; shortB = 269; shortC = 291; shortD = 29; shortE = 31; shortF = 37; shortG = 41; shortH = 47; shortI = 19; shortJ = 28; shortK = 12; shortL = 5; shortM = 16; shortN = 33; shortO = 5; shortP = 244; break; //1 to 18 ms, 12 seat room  
			case 5:
				shortA = 17; shortB = 19; shortC = 16; shortD = 29; shortE = 31; shortF = 37; shortG = 281; shortH = 3; shortI = 24; shortJ = 53; shortK = 300; shortL = 5; shortM = 67; shortN = 49; shortO = 3; shortP = 265; break; //0 to 19 ms, 12 seat room  
			case 6:
				shortA = 17; shortB = 19; shortC = 285; shortD = 16; shortE = 30; shortF = 37; shortG = 41; shortH = 5; shortI = 3; shortJ = 53; shortK = 59; shortL = 11; shortM = 281; shortN = 9; shortO = 5; shortP = 254; break; //0 to 19 ms, 12 seat room  
			case 7:
				shortA = 39; shortB = 30; shortC = 41; shortD = 256; shortE = 50; shortF = 15; shortG = 3; shortH = 19; shortI = 41; shortJ = 64; shortK = 272; shortL = 300; shortM = 252; shortN = 27; shortO = 68; shortP = 297; break; //1 to 20 ms, 14 seat room  
			case 8:
				shortA = 255; shortB = 235; shortC = 271; shortD = 94; shortE = 58; shortF = 21; shortG = 18; shortH = 36; shortI = 29; shortJ = 251; shortK = 25; shortL = 32; shortM = 273; shortN = 19; shortO = 62; shortP = 47; break; //2 to 20 ms, 14 seat room  
			case 9:
				shortA = 320; shortB = 39; shortC = 342; shortD = 29; shortE = 292; shortF = 306; shortG = 53; shortH = 13; shortI = 349; shortJ = 354; shortK = 46; shortL = 8; shortM = 72; shortN = 78; shortO = 52; shortP = 41; break; //2 to 24 ms, 21 seat room  
			case 10:
				shortA = 48; shortB = 323; shortC = 49; shortD = 358; shortE = 93; shortF = 39; shortG = 15; shortH = 7; shortI = 19; shortJ = 341; shortK = 23; shortL = 59; shortM = 357; shortN = 353; shortO = 12; shortP = 345; break; //1 to 26 ms, 22 seat room  
			case 11:
				shortA = 350; shortB = 343; shortC = 22; shortD = 384; shortE = 29; shortF = 42; shortG = 5; shortH = 44; shortI = 394; shortJ = 400; shortK = 4; shortL = 380; shortM = 115; shortN = 30; shortO = 11; shortP = 172; break; //0 to 27 ms, 23 seat room  
			case 12:
				shortA = 380; shortB = 15; shortC = 370; shortD = 349; shortE = 363; shortF = 61; shortG = 343; shortH = 26; shortI = 24; shortJ = 35; shortK = 47; shortL = 38; shortM = 99; shortN = 101; shortO = 382; shortP = 19; break; //1 to 27 ms, 25 seat room  
			case 13:
				shortA = 27; shortB = 339; shortC = 17; shortD = 44; shortE = 27; shortF = 332; shortG = 32; shortH = 29; shortI = 379; shortJ = 356; shortK = 342; shortL = 63; shortM = 29; shortN = 111; shortO = 62; shortP = 28; break; //3 to 26 ms, 25 seat room  
			case 14:
				shortA = 35; shortB = 34; shortC = 287; shortD = 65; shortE = 141; shortF = 368; shortG = 366; shortH = 318; shortI = 69; shortJ = 356; shortK = 360; shortL = 48; shortM = 80; shortN = 64; shortO = 18; shortP = 13; break; //2 to 27 ms, 25 seat room  
			case 15:
				shortA = 44; shortB = 41; shortC = 35; shortD = 52; shortE = 352; shortF = 53; shortG = 328; shortH = 378; shortI = 88; shortJ = 15; shortK = 71; shortL = 384; shortM = 380; shortN = 27; shortO = 376; shortP = 87; break; //2 to 27 ms, 26 seat room  
			case 16:
			default:
				shortA = 81; shortB = 34; shortC = 38; shortD = 69; shortE = 10; shortF = 35; shortG = 19; shortH = 22; shortI = 401; shortJ = 372; shortK = 153; shortL = 382; shortM = 391; shortN = 352; shortO = 26; shortP = 55; break; //1 to 29 ms, 29 seat room  
		} //WoodenChest
		prevclearcoat = clearcoat;
	}
	double regen = (1.0-pow(1.0-B,2.0))*0.04;
	double derez = 1.0;
	derez = fmin(fmax(derez/overallscale,0.0001),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	//the revision more accurately connects the bezier curves
	double wet = 1.0-pow(1.0-C,2.0);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleL*derez);
		bez[bez_SampR] += (inputSampleR*derez);
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			//left verbs
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
			lA[cLA] = fma(fAL,regen,inputSampleL);
			lB[cLB] = fma(fBL,regen,inputSampleL);
			lC[cLC] = fma(fCL,regen,inputSampleL);
			lD[cLD] = fma(fDL,regen,inputSampleL);
			rD[cRD] = fma(fDR,regen,inputSampleR);
			rH[cRH] = fma(fHR,regen,inputSampleR);
			rL[cRL] = fma(fLR,regen,inputSampleR);
			rP[cRP] = fma(fPR,regen,inputSampleR);
			cLA++; cLB++; cLC++; cLD++;
			cRD++; cRH++; cRL++; cRP++;
			if (cLA > shortA) cLA = 0;
			if (cLB > shortB) cLB = 0;
			if (cLC > shortC) cLC = 0;
			if (cLD > shortD) cLD = 0;
			if (cRD > shortD) cRD = 0;
			if (cRH > shortH) cRH = 0;
			if (cRL > shortL) cRL = 0;
			if (cRP > shortP) cRP = 0;
			hAL = lA[cLA]; hBL = lB[cLB]; hCL = lC[cLC]; hDL = lD[cLD];
			hAR = rD[cRD]; hBR = rH[cRH]; hCR = rL[cRL]; hDR = rP[cRP];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lE[cLE] = fma(hAL,2.0f,hEL);
			lF[cLF] = fma(hBL,2.0f,hEL);
			lG[cLG] = fma(hCL,2.0f,hEL);
			lH[cLH] = fma(hDL,2.0f,hEL);
			rC[cRC] = fma(hAR,2.0f,hER);
			rG[cRG] = fma(hBR,2.0f,hER);
			rK[cRK] = fma(hCR,2.0f,hER);
			rO[cRO] = fma(hDR,2.0f,hER);
			cLE++; cLF++; cLG++; cLH++;
			cRC++; cRG++; cRK++; cRO++;
			if (cLE > shortE) cLE = 0;
			if (cLF > shortF) cLF = 0;
			if (cLG > shortG) cLG = 0;
			if (cLH > shortH) cLH = 0;
			if (cRC > shortC) cRC = 0;
			if (cRG > shortG) cRG = 0;
			if (cRK > shortK) cRK = 0;
			if (cRO > shortO) cRO = 0;
			hAL = lE[cLE]; hBL = lF[cLF]; hCL = lG[cLG]; hDL = lH[cLH];
			hAR = rC[cRC]; hBR = rG[cRG]; hCR = rK[cRK]; hDR = rO[cRO];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lI[cLI] = fma(hAL,2.0f,hEL);
			lJ[cLJ] = fma(hBL,2.0f,hEL);
			lK[cLK] = fma(hCL,2.0f,hEL);
			lL[cLL] = fma(hDL,2.0f,hEL);
			rB[cRB] = fma(hAR,2.0f,hER);
			rF[cRF] = fma(hBR,2.0f,hER);
			rJ[cRJ] = fma(hCR,2.0f,hER);
			rN[cRN] = fma(hDR,2.0f,hER);
			cLI++; cLJ++; cLK++; cLL++;
			cRB++; cRF++; cRJ++; cRN++;
			if (cLI > shortI) cLI = 0;
			if (cLJ > shortJ) cLJ = 0;
			if (cLK > shortK) cLK = 0;
			if (cLL > shortL) cLL = 0;
			if (cRB > shortB) cRB = 0;
			if (cRF > shortF) cRF = 0;
			if (cRJ > shortJ) cRJ = 0;
			if (cRN > shortN) cRN = 0;
			hAL = lI[cLI]; hBL = lJ[cLJ]; hCL = lK[cLK]; hDL = lL[cLL];
			hAR = rB[cRB]; hBR = rF[cRF]; hCR = rJ[cRJ]; hDR = rN[cRN];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lM[cLM] = fma(hAL,2.0f,hEL);
			lN[cLN] = fma(hBL,2.0f,hEL);
			lO[cLO] = fma(hCL,2.0f,hEL);
			lP[cLP] = fma(hDL,2.0f,hEL);
			rA[cRA] = fma(hAR,2.0f,hER);
			rE[cRE] = fma(hBR,2.0f,hER);
			rI[cRI] = fma(hCR,2.0f,hER);
			rM[cRM] = fma(hDR,2.0f,hER);
			cLM++; cLN++; cLO++; cLP++;
			cRA++; cRE++; cRI++; cRM++;
			if (cLM > shortM) cLM = 0;
			if (cLN > shortN) cLN = 0;
			if (cLO > shortO) cLO = 0;
			if (cLP > shortP) cLP = 0;
			if (cRA > shortA) cRA = 0;
			if (cRE > shortE) cRE = 0;
			if (cRI > shortI) cRI = 0;
			if (cRM > shortM) cRM = 0;
			hAL = lM[cLM]; hBL = lN[cLN]; hCL = lO[cLO]; hDL = lP[cLP];
			hAR = rA[cRA]; hBR = rE[cRE]; hCR = rI[cRI]; hDR = rM[cRM];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			fAL = fma(hAR,2.0f,hER);				
			fBL = fma(hBR,2.0f,hER);
			fCL = fma(hCR,2.0f,hER);
			fDL = fma(hDR,2.0f,hER);
			fDR = fma(hAL,2.0f,hEL);				
			fHR = fma(hBL,2.0f,hEL);
			fLR = fma(hCL,2.0f,hEL);
			fPR = fma(hDL,2.0f,hEL);
			inputSampleL = (hAL + hBL + hCL + hDL)*0.0625f;
			inputSampleR = (hAR + hBR + hCR + hDR)*0.0625f;
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle]*bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		inputSampleL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.25;
		inputSampleR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.25;
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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

void WoodenChest::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int clearcoat = (int)(A*16.999);
	if (clearcoat != prevclearcoat) {
		for(int x = 0; x < d4A+2; x++) {lA[x] = 0.0f; rA[x] = 0.0f;}
		for(int x = 0; x < d4B+2; x++) {lB[x] = 0.0f; rB[x] = 0.0f;}
		for(int x = 0; x < d4C+2; x++) {lC[x] = 0.0f; rC[x] = 0.0f;}
		for(int x = 0; x < d4D+2; x++) {lD[x] = 0.0f; rD[x] = 0.0f;}
		for(int x = 0; x < d4E+2; x++) {lE[x] = 0.0f; rE[x] = 0.0f;}
		for(int x = 0; x < d4F+2; x++) {lF[x] = 0.0f; rF[x] = 0.0f;}
		for(int x = 0; x < d4G+2; x++) {lG[x] = 0.0f; rG[x] = 0.0f;}
		for(int x = 0; x < d4H+2; x++) {lH[x] = 0.0f; rH[x] = 0.0f;}
		for(int x = 0; x < d4I+2; x++) {lI[x] = 0.0f; rI[x] = 0.0f;}
		for(int x = 0; x < d4J+2; x++) {lJ[x] = 0.0f; rJ[x] = 0.0f;}
		for(int x = 0; x < d4K+2; x++) {lK[x] = 0.0f; rK[x] = 0.0f;}
		for(int x = 0; x < d4L+2; x++) {lL[x] = 0.0f; rL[x] = 0.0f;}
		for(int x = 0; x < d4M+2; x++) {lM[x] = 0.0f; rM[x] = 0.0f;}
		for(int x = 0; x < d4N+2; x++) {lN[x] = 0.0f; rN[x] = 0.0f;}
		for(int x = 0; x < d4O+2; x++) {lO[x] = 0.0f; rO[x] = 0.0f;}
		for(int x = 0; x < d4P+2; x++) {lP[x] = 0.0f; rP[x] = 0.0f;}
		cLA = cLB = cLC = cLD = cLE = cLF = cLG = cLH = 1;
		cLI = cLJ = cLK = cLL = cLM = cLN = cLO = cLP = 1;
		cRA = cRB = cRC = cRD = cRE = cRF = cRG = cRH = 1;
		cRI = cRJ = cRK = cRL = cRM = cRN = cRO = cRP = 1;
		
		//WoodenChest
		switch (clearcoat)
		{
			case 0:
				shortA = 350; shortB = 10; shortC = 21; shortD = 76; shortE = 80; shortF = 72; shortG = 40; shortH = 108; shortI = 83; shortJ = 7; shortK = 47; shortL = 33; shortM = 38; shortN = 65; shortO = 29; shortP = 5; break; //1 to 13 ms, 6 seat room  
			case 1:
				shortA = 4; shortB = 134; shortC = 90; shortD = 29; shortE = 70; shortF = 71; shortG = 23; shortH = 22; shortI = 96; shortJ = 135; shortK = 115; shortL = 23; shortM = 390; shortN = 59; shortO = 9; shortP = 26; break; //1 to 16 ms, 9 seat room  
			case 2:
				shortA = 51; shortB = 131; shortC = 80; shortD = 6; shortE = 95; shortF = 414; shortG = 88; shortH = 40; shortI = 113; shortJ = 19; shortK = 65; shortL = 47; shortM = 15; shortN = 131; shortO = 33; shortP = 7; break; //1 to 17 ms, 11 seat room  
			case 3:
				shortA = 75; shortB = 44; shortC = 164; shortD = 95; shortE = 26; shortF = 123; shortG = 115; shortH = 69; shortI = 386; shortJ = 57; shortK = 8; shortL = 90; shortM = 28; shortN = 32; shortO = 4; shortP = 37; break; //1 to 17 ms, 11 seat room  
			case 4:
				shortA = 17; shortB = 269; shortC = 291; shortD = 29; shortE = 31; shortF = 37; shortG = 41; shortH = 47; shortI = 19; shortJ = 28; shortK = 12; shortL = 5; shortM = 16; shortN = 33; shortO = 5; shortP = 244; break; //1 to 18 ms, 12 seat room  
			case 5:
				shortA = 17; shortB = 19; shortC = 16; shortD = 29; shortE = 31; shortF = 37; shortG = 281; shortH = 3; shortI = 24; shortJ = 53; shortK = 300; shortL = 5; shortM = 67; shortN = 49; shortO = 3; shortP = 265; break; //0 to 19 ms, 12 seat room  
			case 6:
				shortA = 17; shortB = 19; shortC = 285; shortD = 16; shortE = 30; shortF = 37; shortG = 41; shortH = 5; shortI = 3; shortJ = 53; shortK = 59; shortL = 11; shortM = 281; shortN = 9; shortO = 5; shortP = 254; break; //0 to 19 ms, 12 seat room  
			case 7:
				shortA = 39; shortB = 30; shortC = 41; shortD = 256; shortE = 50; shortF = 15; shortG = 3; shortH = 19; shortI = 41; shortJ = 64; shortK = 272; shortL = 300; shortM = 252; shortN = 27; shortO = 68; shortP = 297; break; //1 to 20 ms, 14 seat room  
			case 8:
				shortA = 255; shortB = 235; shortC = 271; shortD = 94; shortE = 58; shortF = 21; shortG = 18; shortH = 36; shortI = 29; shortJ = 251; shortK = 25; shortL = 32; shortM = 273; shortN = 19; shortO = 62; shortP = 47; break; //2 to 20 ms, 14 seat room  
			case 9:
				shortA = 320; shortB = 39; shortC = 342; shortD = 29; shortE = 292; shortF = 306; shortG = 53; shortH = 13; shortI = 349; shortJ = 354; shortK = 46; shortL = 8; shortM = 72; shortN = 78; shortO = 52; shortP = 41; break; //2 to 24 ms, 21 seat room  
			case 10:
				shortA = 48; shortB = 323; shortC = 49; shortD = 358; shortE = 93; shortF = 39; shortG = 15; shortH = 7; shortI = 19; shortJ = 341; shortK = 23; shortL = 59; shortM = 357; shortN = 353; shortO = 12; shortP = 345; break; //1 to 26 ms, 22 seat room  
			case 11:
				shortA = 350; shortB = 343; shortC = 22; shortD = 384; shortE = 29; shortF = 42; shortG = 5; shortH = 44; shortI = 394; shortJ = 400; shortK = 4; shortL = 380; shortM = 115; shortN = 30; shortO = 11; shortP = 172; break; //0 to 27 ms, 23 seat room  
			case 12:
				shortA = 380; shortB = 15; shortC = 370; shortD = 349; shortE = 363; shortF = 61; shortG = 343; shortH = 26; shortI = 24; shortJ = 35; shortK = 47; shortL = 38; shortM = 99; shortN = 101; shortO = 382; shortP = 19; break; //1 to 27 ms, 25 seat room  
			case 13:
				shortA = 27; shortB = 339; shortC = 17; shortD = 44; shortE = 27; shortF = 332; shortG = 32; shortH = 29; shortI = 379; shortJ = 356; shortK = 342; shortL = 63; shortM = 29; shortN = 111; shortO = 62; shortP = 28; break; //3 to 26 ms, 25 seat room  
			case 14:
				shortA = 35; shortB = 34; shortC = 287; shortD = 65; shortE = 141; shortF = 368; shortG = 366; shortH = 318; shortI = 69; shortJ = 356; shortK = 360; shortL = 48; shortM = 80; shortN = 64; shortO = 18; shortP = 13; break; //2 to 27 ms, 25 seat room  
			case 15:
				shortA = 44; shortB = 41; shortC = 35; shortD = 52; shortE = 352; shortF = 53; shortG = 328; shortH = 378; shortI = 88; shortJ = 15; shortK = 71; shortL = 384; shortM = 380; shortN = 27; shortO = 376; shortP = 87; break; //2 to 27 ms, 26 seat room  
			case 16:
			default:
				shortA = 81; shortB = 34; shortC = 38; shortD = 69; shortE = 10; shortF = 35; shortG = 19; shortH = 22; shortI = 401; shortJ = 372; shortK = 153; shortL = 382; shortM = 391; shortN = 352; shortO = 26; shortP = 55; break; //1 to 29 ms, 29 seat room  
		} //WoodenChest
		prevclearcoat = clearcoat;
	}
	double regen = (1.0-pow(1.0-B,2.0))*0.04;
	double derez = 1.0;
	derez = fmin(fmax(derez/overallscale,0.0001),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	//the revision more accurately connects the bezier curves
	double wet = 1.0-pow(1.0-C,2.0);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleL*derez);
		bez[bez_SampR] += (inputSampleR*derez);
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			//left verbs
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
			lA[cLA] = fma(fAL,regen,inputSampleL);
			lB[cLB] = fma(fBL,regen,inputSampleL);
			lC[cLC] = fma(fCL,regen,inputSampleL);
			lD[cLD] = fma(fDL,regen,inputSampleL);
			rD[cRD] = fma(fDR,regen,inputSampleR);
			rH[cRH] = fma(fHR,regen,inputSampleR);
			rL[cRL] = fma(fLR,regen,inputSampleR);
			rP[cRP] = fma(fPR,regen,inputSampleR);
			cLA++; cLB++; cLC++; cLD++;
			cRD++; cRH++; cRL++; cRP++;
			if (cLA > shortA) cLA = 0;
			if (cLB > shortB) cLB = 0;
			if (cLC > shortC) cLC = 0;
			if (cLD > shortD) cLD = 0;
			if (cRD > shortD) cRD = 0;
			if (cRH > shortH) cRH = 0;
			if (cRL > shortL) cRL = 0;
			if (cRP > shortP) cRP = 0;
			hAL = lA[cLA]; hBL = lB[cLB]; hCL = lC[cLC]; hDL = lD[cLD];
			hAR = rD[cRD]; hBR = rH[cRH]; hCR = rL[cRL]; hDR = rP[cRP];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lE[cLE] = fma(hAL,2.0f,hEL);
			lF[cLF] = fma(hBL,2.0f,hEL);
			lG[cLG] = fma(hCL,2.0f,hEL);
			lH[cLH] = fma(hDL,2.0f,hEL);
			rC[cRC] = fma(hAR,2.0f,hER);
			rG[cRG] = fma(hBR,2.0f,hER);
			rK[cRK] = fma(hCR,2.0f,hER);
			rO[cRO] = fma(hDR,2.0f,hER);
			cLE++; cLF++; cLG++; cLH++;
			cRC++; cRG++; cRK++; cRO++;
			if (cLE > shortE) cLE = 0;
			if (cLF > shortF) cLF = 0;
			if (cLG > shortG) cLG = 0;
			if (cLH > shortH) cLH = 0;
			if (cRC > shortC) cRC = 0;
			if (cRG > shortG) cRG = 0;
			if (cRK > shortK) cRK = 0;
			if (cRO > shortO) cRO = 0;
			hAL = lE[cLE]; hBL = lF[cLF]; hCL = lG[cLG]; hDL = lH[cLH];
			hAR = rC[cRC]; hBR = rG[cRG]; hCR = rK[cRK]; hDR = rO[cRO];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lI[cLI] = fma(hAL,2.0f,hEL);
			lJ[cLJ] = fma(hBL,2.0f,hEL);
			lK[cLK] = fma(hCL,2.0f,hEL);
			lL[cLL] = fma(hDL,2.0f,hEL);
			rB[cRB] = fma(hAR,2.0f,hER);
			rF[cRF] = fma(hBR,2.0f,hER);
			rJ[cRJ] = fma(hCR,2.0f,hER);
			rN[cRN] = fma(hDR,2.0f,hER);
			cLI++; cLJ++; cLK++; cLL++;
			cRB++; cRF++; cRJ++; cRN++;
			if (cLI > shortI) cLI = 0;
			if (cLJ > shortJ) cLJ = 0;
			if (cLK > shortK) cLK = 0;
			if (cLL > shortL) cLL = 0;
			if (cRB > shortB) cRB = 0;
			if (cRF > shortF) cRF = 0;
			if (cRJ > shortJ) cRJ = 0;
			if (cRN > shortN) cRN = 0;
			hAL = lI[cLI]; hBL = lJ[cLJ]; hCL = lK[cLK]; hDL = lL[cLL];
			hAR = rB[cRB]; hBR = rF[cRF]; hCR = rJ[cRJ]; hDR = rN[cRN];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lM[cLM] = fma(hAL,2.0f,hEL);
			lN[cLN] = fma(hBL,2.0f,hEL);
			lO[cLO] = fma(hCL,2.0f,hEL);
			lP[cLP] = fma(hDL,2.0f,hEL);
			rA[cRA] = fma(hAR,2.0f,hER);
			rE[cRE] = fma(hBR,2.0f,hER);
			rI[cRI] = fma(hCR,2.0f,hER);
			rM[cRM] = fma(hDR,2.0f,hER);
			cLM++; cLN++; cLO++; cLP++;
			cRA++; cRE++; cRI++; cRM++;
			if (cLM > shortM) cLM = 0;
			if (cLN > shortN) cLN = 0;
			if (cLO > shortO) cLO = 0;
			if (cLP > shortP) cLP = 0;
			if (cRA > shortA) cRA = 0;
			if (cRE > shortE) cRE = 0;
			if (cRI > shortI) cRI = 0;
			if (cRM > shortM) cRM = 0;
			hAL = lM[cLM]; hBL = lN[cLN]; hCL = lO[cLO]; hDL = lP[cLP];
			hAR = rA[cRA]; hBR = rE[cRE]; hCR = rI[cRI]; hDR = rM[cRM];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			fAL = fma(hAR,2.0f,hER);				
			fBL = fma(hBR,2.0f,hER);
			fCL = fma(hCR,2.0f,hER);
			fDL = fma(hDR,2.0f,hER);
			fDR = fma(hAL,2.0f,hEL);				
			fHR = fma(hBL,2.0f,hEL);
			fLR = fma(hCL,2.0f,hEL);
			fPR = fma(hDL,2.0f,hEL);
			inputSampleL = (hAL + hBL + hCL + hDL)*0.0625f;
			inputSampleR = (hAR + hBR + hCR + hDR)*0.0625f;
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle]*bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		inputSampleL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.25;
		inputSampleR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.25;
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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
