/* ========================================
 *  Ultralight - Ultralight.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Ultralight_H
#include "Ultralight.h"
#endif

void Ultralight::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 48000.0;
	overallscale *= getSampleRate();
	float pointDerez = 1.0f / overallscale; //forcing it to be simple
	
	//these become presets and are built into the reverb when used
	float rtSixty = 0.0625f-(((int)(A*9.99))*0.003125f);
	int furnish = 4-(int)(B*4.99); //0 through 4 are valid
	int wallDns = (int)(C*4.99); //0 through 4 are valid
	int reflect = (int)(D*4.99); //0 through 4 are valid
	int preDlay = (int)((E*64.0)*(E*64.0)); //0 through 4096 are valid
	//making five additional const ints adjusting the node in question
	
	float wet = F;
	float distance = pow(G*7.28f,2.0f); //0 to 52.9984
	int distanceSteps = (int)distance; //52 maximum
	distance -= (float)distanceSteps; //0.9984
	
    while (--sampleFrames >= 0)
    {
		float inputSampleL = *in1;
		float inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		pointCycle += pointDerez;
		if (pointCycle > 0.9999999) {
			//this is sample rate handling for the plugin, so that
			//a more primitive reverb node can run at any rate.
			
			float drySampleL = inputSampleL;
			float drySampleR = inputSampleR;
			//predelay for plugin: not suitable for game engine,
			//as it doesn't handle changing delays gracefully.
			//However, it might be worth designing a FIXED predelay for
			//each verb node, on the grounds that if you're right next to it,
			//you'll hear maximum delay before the sound bounces off the first wall.
			//As such it'd always be the same delay, and so it'd not pose a problem,
			//even for sound events that are moving around in the verb area.
			//it'd also mean you could set zero for other purposes.
			lZ[cZ] = inputSampleL;
			rZ[cZ] = inputSampleR;
			cZ++; if (cZ < 0 || cZ > preDlay) cZ = 0;
			inputSampleL = lZ[cZ-((cZ > preDlay)?preDlay+1:0)];
			inputSampleR = rZ[cZ-((cZ > preDlay)?preDlay+1:0)];
			//end predelay
			
			//begin just what's in reverb node
			
			//allpass/comb section
			if (furnish < 4) {
				hAL = inputSampleL - (lA[(cA+1)-((cA+1 > d4A)?d4A+1:0)]*0.5);
				hAL += (fAL * rtSixty); lA[cA] = hAL; hAL *= 0.5;
				hAR = inputSampleR - (rA[(cA+1)-((cA+1 > d4A)?d4A+1:0)]*0.5);
				hAR += (fAR * rtSixty); rA[cA] = hAR; hAR *= 0.5;
				cA++;
				if (cA > d4A) cA = 0;
				hAL += lA[cA];
				hAR += rA[cA];
			} else {
				lA[cA] = inputSampleL + (fAL * rtSixty);
				rA[cA] = inputSampleR + (fAR * rtSixty);
				cA++;
				if (cA > d4A) cA = 0;
				hAL = lA[cA];
				hAR = rA[cA];
			}
			if (furnish < 3) {
				hBL = inputSampleL - (lB[(cB+1)-((cB+1 > d4B)?d4B+1:0)]*0.5);
				hBL += (fBL * rtSixty); lB[cB] = hBL; hBL *= 0.5;
				hBR = inputSampleR - (rB[(cB+1)-((cB+1 > d4B)?d4B+1:0)]*0.5);
				hBR += (fBR * rtSixty); rB[cB] = hBR; hBR *= 0.5;
				cB++;
				if (cB > d4B) cB = 0;
				hBL += lB[cB];
				hBR += rB[cB];
			} else {
				lB[cB] = inputSampleL + (fBL * rtSixty);
				rB[cB] = inputSampleR + (fBR * rtSixty);
				cB++;
				if (cB > d4B) cB = 0;
				hBL = lB[cB];
				hBR = rB[cB];
			}
			if (furnish < 2) {
				hCL = inputSampleL - (lC[(cC+1)-((cC+1 > d4C)?d4C+1:0)]*0.5);
				hCL += (fCL * rtSixty); lC[cC] = hCL; hCL *= 0.5;
				hCR = inputSampleR - (rC[(cC+1)-((cC+1 > d4C)?d4C+1:0)]*0.5);
				hCR += (fCR * rtSixty); rC[cC] = hCR; hCR *= 0.5;
				cC++;
				if (cC > d4C) cC = 0;
				hCL += lC[cC];
				hCR += rC[cC];
			} else {
				lC[cC] = inputSampleL + (fCL * rtSixty);
				rC[cC] = inputSampleR + (fCR * rtSixty);
				cC++;
				if (cC > d4C) cC = 0;
				hCL = lC[cC];
				hCR = rC[cC];
			}
			if (furnish < 1) {
				hDL = inputSampleL - (lD[(cD+1)-((cD+1 > d4D)?d4D+1:0)]*0.5);
				hDL += (fDL * rtSixty); lD[cD] = hDL; hDL *= 0.5;
				hDR = inputSampleR - (rD[(cD+1)-((cD+1 > d4D)?d4D+1:0)]*0.5);
				hDR += (fDR * rtSixty); rD[cD] = hDR; hDR *= 0.5;
				cD++;
				if (cD > d4D) cD = 0;
				hDL += lD[cD];
				hDR += rD[cD];
			} else {
				lD[cD] = inputSampleL + (fDL * rtSixty);
				rD[cD] = inputSampleR + (fDR * rtSixty);
				cD++;
				if (cD > d4D) cD = 0;
				hDL = lD[cD];
				hDR = rD[cD];
			}
			hEL = -(hAL + hBL + hCL + hDL);
			lE[cE] = fma(hAL,2.0f,hEL);
			lF[cF] = fma(hBL,2.0f,hEL);
			lG[cG] = fma(hCL,2.0f,hEL);
			lH[cH] = fma(hDL,2.0f,hEL);
			hER = -(hAR + hBR + hCR + hDR);
			rE[cE] = fma(hAR,2.0f,hER);
			rF[cF] = fma(hBR,2.0f,hER);
			rG[cG] = fma(hCR,2.0f,hER);
			rH[cH] = fma(hDR,2.0f,hER);
			//allpass/comb section
			
			cE++; cF++; cG++; cH++;
			if (cE > d4E) cE = 0;
			if (cF > d4F) cF = 0;
			if (cG > d4G) cG = 0;
			if (cH > d4H) cH = 0;
			
			hAL = lE[cE]; hBL = lF[cF]; hCL = lG[cG]; hDL = lH[cH];
			hEL = -(hAL + hBL + hCL + hDL);
			lI[cI] = fma(hAL,2.0f,hEL);
			lJ[cJ] = fma(hBL,2.0f,hEL);
			lK[cK] = fma(hCL,2.0f,hEL);
			lL[cL] = fma(hDL,2.0f,hEL);
			hAR = rE[cE]; hBR = rF[cF]; hCR = rG[cG]; hDR = rH[cH];
			hER = -(hAR + hBR + hCR + hDR);
			rI[cI] = fma(hAR,2.0f,hER);
			rJ[cJ] = fma(hBR,2.0f,hER);
			rK[cK] = fma(hCR,2.0f,hER);
			rL[cL] = fma(hDR,2.0f,hER);
			
			cI++; cJ++; cK++; cL++;
			if (cI > d4I) cI = 0;
			if (cJ > d4J) cJ = 0;
			if (cK > d4K) cK = 0;
			if (cL > d4L) cL = 0;
			
			hAL = lI[cI]; hBL = lJ[cJ]; hCL = lK[cK]; hDL = lL[cL];
			hEL = -(hAL + hBL + hCL + hDL);
			lM[cM] = fma(hAL,2.0f,hEL);
			lN[cN] = fma(hBL,2.0f,hEL);
			lO[cO] = fma(hCL,2.0f,hEL);
			lP[cP] = fma(hDL,2.0f,hEL);
			hAR = rI[cI]; hBR = rJ[cJ]; hCR = rK[cK]; hDR = rL[cL];
			hER = -(hAR + hBR + hCR + hDR);
			rM[cM] = fma(hAR,2.0f,hER);
			rN[cN] = fma(hBR,2.0f,hER);
			rO[cO] = fma(hCR,2.0f,hER);
			rP[cP] = fma(hDR,2.0f,hER);

			cM++; cN++; cO++; cP++;
			if (cM > d4M) cM = 0;
			if (cN > d4N) cN = 0;
			if (cO > d4O) cO = 0;
			if (cP > d4P) cP = 0;
			
			hAL = lM[cM]; hBL = lN[cN]; hCL = lO[cO]; hDL = lP[cP];
			hEL = -(hAL + hBL + hCL + hDL);
			fAL = fma(hAL,2.0f,hEL);				
			fBL = fma(hBL,2.0f,hEL);
			fCL = fma(hCL,2.0f,hEL);
			fDL = fma(hDL,2.0f,hEL);
			hAR = rM[cM]; hBR = rN[cN]; hCR = rO[cO]; hDR = rP[cP];
			hER = -(hAR + hBR + hCR + hDR);
			fAR = fma(hAR,2.0f,hER);				
			fBR = fma(hBR,2.0f,hER);
			fCR = fma(hCR,2.0f,hER);
			fDR = fma(hDR,2.0f,hER);

			switch (reflect)
			{
				case 0:
					fAL = (fAL+flAL)*0.5f; flAL = fAL;
					fAR = (fAR+flAR)*0.5f; flAR = fAR;
				case 1:
					fBL = (fBL+flBL)*0.5f; flBL = fBL;
					fBR = (fBR+flBR)*0.5f; flBR = fBR;
				case 2:
					fCL = (fCL+flCL)*0.5f; flCL = fCL;
					fCR = (fCR+flCR)*0.5f; flCR = fCR;
				case 3:
					fDL = (fDL+flDL)*0.5f; flDL = fDL;
					fDR = (fDR+flDR)*0.5f; flDR = fDR;
				case 4:
					break; //This applies ALL processing
					//after the starting point, meaning the
					//cases intentionally fall through
			}
			switch (wallDns)
			{
				case 0:
					fhAL *= 0.5f; fAL -= fhAL*0.0625f; fhAL += fAL;
					fhAR *= 0.5f; fAR -= fhAR*0.0625f; fhAR += fAR;
				case 1:
					fhBL *= 0.5f; fBL -= fhBL*0.0625f; fhBL += fBL;
					fhBR *= 0.5f; fBR -= fhBR*0.0625f; fhBR += fBR;
				case 2:
					fhCL *= 0.5f; fCL -= fhCL*0.0625f; fhCL += fCL;
					fhCR *= 0.5f; fCR -= fhCR*0.0625f; fhCR += fCR;
				case 3:
					fhDL *= 0.5f; fDL -= fhDL*0.0625f; fhDL += fDL;
					fhDR *= 0.5f; fDR -= fhDR*0.0625f; fhDR += fDR;
				case 4:
					break; //This applies ALL processing
					//after the starting point, meaning the
					//cases intentionally fall through
			}
			inputSampleL = (hAL + hBL + hCL + hDL)*0.0625;
			inputSampleR = (hAR + hBR + hCR + hDR)*0.0625;
			//end of just the reverb node part
			
			//dry/wet is not part of the game engine reverb node
			inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
			//plugin only: game engine one would have just the reverb,
			//and every sound source could have the following Distance filter,
			//including the localized reverb nodes. The reason it's here
			//is so the plugin can preview how dry+reverb, both distant, will sound
			
			//begin just the distance filter L
			firstDryL = inputSampleL; //start by doing the interpolation
			inputSampleL += firstAvgL; inputSampleL *= 0.5f; firstAvgL = inputSampleL;
			inputSampleL = (firstDryL*(1.0f-distance)) + (inputSampleL*distance);
			//having done at least one interpolation we can now do the integer number of stages
			if (prevDistance < distanceSteps) {
				switch (prevDistance)
				{
					case  0: lsA = inputSampleL; //if we've changed the setting
					case  1: lsB = inputSampleL; //reset the ones being put back into play
					case  2: lsC = inputSampleL; //this won't be happening constantly,
					case  3: lsD = inputSampleL; //but since we don't have an array
					case  4: lsE = inputSampleL; //this is the structure by which we
					case  5: lsF = inputSampleL; //get the new averagings going
					case  6: lsG = inputSampleL; //without pops or clicks
					case  7: lsH = inputSampleL;
					case  8: lsI = inputSampleL;
					case  9: lsJ = inputSampleL;
					case 10: lsK = inputSampleL;
					case 11: lsL = inputSampleL;
					case 12: lsM = inputSampleL;
					case 13: lsN = inputSampleL;
					case 14: lsO = inputSampleL;
					case 15: lsP = inputSampleL;
					case 16: lsQ = inputSampleL;
					case 17: lsR = inputSampleL;
					case 18: lsS = inputSampleL;
					case 19: lsT = inputSampleL;
					case 20: lsU = inputSampleL;
					case 21: lsV = inputSampleL;
					case 22: lsW = inputSampleL;
					case 23: lsX = inputSampleL;
					case 24: lsY = inputSampleL;
					case 25: lsZ = inputSampleL;
					case 26: lsa = inputSampleL;
					case 27: lsb = inputSampleL;
					case 28: lsc = inputSampleL;
					case 29: lsd = inputSampleL;
					case 30: lse = inputSampleL;
					case 31: lsf = inputSampleL;
					case 32: lsg = inputSampleL;
					case 33: lsh = inputSampleL;
					case 34: lsi = inputSampleL;
					case 35: lsj = inputSampleL;
					case 36: lsk = inputSampleL;
					case 37: lsl = inputSampleL;
					case 38: lsm = inputSampleL;
					case 39: lsn = inputSampleL;
					case 40: lso = inputSampleL;
					case 41: lsp = inputSampleL;
					case 42: lsq = inputSampleL;
					case 43: lsr = inputSampleL;
					case 44: lss = inputSampleL;
					case 45: lst = inputSampleL;
					case 46: lsu = inputSampleL;
					case 47: lsv = inputSampleL;
					case 48: lsw = inputSampleL;
					case 49: lsx = inputSampleL;
					case 50: lsy = inputSampleL;
					case 51: lsz = inputSampleL;
					case 52: break; //reset prevDistance after R channel
				}
			}
			switch (52-distanceSteps)
			{ //apply the stack of filter steps to produce the distance filter
				case  0: inputSampleL += lsz; inputSampleL *= 0.5; lsz = inputSampleL;
				case  1: inputSampleL += lsy; inputSampleL *= 0.5; lsy = inputSampleL;
				case  2: inputSampleL += lsx; inputSampleL *= 0.5; lsx = inputSampleL;
				case  3: inputSampleL += lsw; inputSampleL *= 0.5; lsw = inputSampleL;
				case  4: inputSampleL += lsv; inputSampleL *= 0.5; lsv = inputSampleL;
				case  5: inputSampleL += lsu; inputSampleL *= 0.5; lsu = inputSampleL;
				case  6: inputSampleL += lst; inputSampleL *= 0.5; lst = inputSampleL;
				case  7: inputSampleL += lss; inputSampleL *= 0.5; lss = inputSampleL;
				case  8: inputSampleL += lsr; inputSampleL *= 0.5; lsr = inputSampleL;
				case  9: inputSampleL += lsq; inputSampleL *= 0.5; lsq = inputSampleL;
				case 10: inputSampleL += lsp; inputSampleL *= 0.5; lsp = inputSampleL;
				case 11: inputSampleL += lso; inputSampleL *= 0.5; lso = inputSampleL;
				case 12: inputSampleL += lsn; inputSampleL *= 0.5; lsn = inputSampleL;
				case 13: inputSampleL += lsm; inputSampleL *= 0.5; lsm = inputSampleL;
				case 14: inputSampleL += lsl; inputSampleL *= 0.5; lsl = inputSampleL;
				case 15: inputSampleL += lsk; inputSampleL *= 0.5; lsk = inputSampleL;
				case 16: inputSampleL += lsj; inputSampleL *= 0.5; lsj = inputSampleL;
				case 17: inputSampleL += lsi; inputSampleL *= 0.5; lsi = inputSampleL;
				case 18: inputSampleL += lsh; inputSampleL *= 0.5; lsh = inputSampleL;
				case 19: inputSampleL += lsg; inputSampleL *= 0.5; lsg = inputSampleL;
				case 20: inputSampleL += lsf; inputSampleL *= 0.5; lsf = inputSampleL;
				case 21: inputSampleL += lse; inputSampleL *= 0.5; lse = inputSampleL;
				case 22: inputSampleL += lsd; inputSampleL *= 0.5; lsd = inputSampleL;
				case 23: inputSampleL += lsc; inputSampleL *= 0.5; lsc = inputSampleL;
				case 24: inputSampleL += lsb; inputSampleL *= 0.5; lsb = inputSampleL;
				case 25: inputSampleL += lsa; inputSampleL *= 0.5; lsa = inputSampleL;
				case 26: inputSampleL += lsZ; inputSampleL *= 0.5; lsZ = inputSampleL;
				case 27: inputSampleL += lsY; inputSampleL *= 0.5; lsY = inputSampleL;
				case 28: inputSampleL += lsX; inputSampleL *= 0.5; lsX = inputSampleL;
				case 29: inputSampleL += lsW; inputSampleL *= 0.5; lsW = inputSampleL;
				case 30: inputSampleL += lsV; inputSampleL *= 0.5; lsV = inputSampleL;
				case 31: inputSampleL += lsU; inputSampleL *= 0.5; lsU = inputSampleL;
				case 32: inputSampleL += lsT; inputSampleL *= 0.5; lsT = inputSampleL;
				case 33: inputSampleL += lsS; inputSampleL *= 0.5; lsS = inputSampleL;
				case 34: inputSampleL += lsR; inputSampleL *= 0.5; lsR = inputSampleL;
				case 35: inputSampleL += lsQ; inputSampleL *= 0.5; lsQ = inputSampleL;
				case 36: inputSampleL += lsP; inputSampleL *= 0.5; lsP = inputSampleL;
				case 37: inputSampleL += lsO; inputSampleL *= 0.5; lsO = inputSampleL;
				case 38: inputSampleL += lsN; inputSampleL *= 0.5; lsN = inputSampleL;
				case 39: inputSampleL += lsM; inputSampleL *= 0.5; lsM = inputSampleL;
				case 40: inputSampleL += lsL; inputSampleL *= 0.5; lsL = inputSampleL;
				case 41: inputSampleL += lsK; inputSampleL *= 0.5; lsK = inputSampleL;
				case 42: inputSampleL += lsJ; inputSampleL *= 0.5; lsJ = inputSampleL;
				case 43: inputSampleL += lsI; inputSampleL *= 0.5; lsI = inputSampleL;
				case 44: inputSampleL += lsH; inputSampleL *= 0.5; lsH = inputSampleL;
				case 45: inputSampleL += lsG; inputSampleL *= 0.5; lsG = inputSampleL;
				case 46: inputSampleL += lsF; inputSampleL *= 0.5; lsF = inputSampleL;
				case 47: inputSampleL += lsE; inputSampleL *= 0.5; lsE = inputSampleL;
				case 48: inputSampleL += lsD; inputSampleL *= 0.5; lsD = inputSampleL;
				case 49: inputSampleL += lsC; inputSampleL *= 0.5; lsC = inputSampleL;
				case 50: inputSampleL += lsB; inputSampleL *= 0.5; lsB = inputSampleL;
				case 51: inputSampleL += lsA; inputSampleL *= 0.5; lsA = inputSampleL;
				case 52: break;
			}
			//end distance filter L
			
			//begin just the distance filter R
			firstDryR = inputSampleR; //start by doing the interpolation
			inputSampleR += firstAvgR; inputSampleR *= 0.5f; firstAvgR = inputSampleR;
			inputSampleR = (firstDryR*(1.0f-distance)) + (inputSampleR*distance);
			//having done at least one interpolation we can now do the integer number of stages
			if (prevDistance < distanceSteps) {
				switch (prevDistance)
				{
					case  0: rsA = inputSampleR; //if we've changed the setting
					case  1: rsB = inputSampleR; //reset the ones being put back into play
					case  2: rsC = inputSampleR; //this won't be happening constantly,
					case  3: rsD = inputSampleR; //but since we don't have an array
					case  4: rsE = inputSampleR; //this is the structure by which we
					case  5: rsF = inputSampleR; //get the new averagings going
					case  6: rsG = inputSampleR; //without pops or clicks
					case  7: rsH = inputSampleR;
					case  8: rsI = inputSampleR;
					case  9: rsJ = inputSampleR;
					case 10: rsK = inputSampleR;
					case 11: rsL = inputSampleR;
					case 12: rsM = inputSampleR;
					case 13: rsN = inputSampleR;
					case 14: rsO = inputSampleR;
					case 15: rsP = inputSampleR;
					case 16: rsQ = inputSampleR;
					case 17: rsR = inputSampleR;
					case 18: rsS = inputSampleR;
					case 19: rsT = inputSampleR;
					case 20: rsU = inputSampleR;
					case 21: rsV = inputSampleR;
					case 22: rsW = inputSampleR;
					case 23: rsX = inputSampleR;
					case 24: rsY = inputSampleR;
					case 25: rsZ = inputSampleR;
					case 26: rsa = inputSampleR;
					case 27: rsb = inputSampleR;
					case 28: rsc = inputSampleR;
					case 29: rsd = inputSampleR;
					case 30: rse = inputSampleR;
					case 31: rsf = inputSampleR;
					case 32: rsg = inputSampleR;
					case 33: rsh = inputSampleR;
					case 34: rsi = inputSampleR;
					case 35: rsj = inputSampleR;
					case 36: rsk = inputSampleR;
					case 37: rsl = inputSampleR;
					case 38: rsm = inputSampleR;
					case 39: rsn = inputSampleR;
					case 40: rso = inputSampleR;
					case 41: rsp = inputSampleR;
					case 42: rsq = inputSampleR;
					case 43: rsr = inputSampleR;
					case 44: rss = inputSampleR;
					case 45: rst = inputSampleR;
					case 46: rsu = inputSampleR;
					case 47: rsv = inputSampleR;
					case 48: rsw = inputSampleR;
					case 49: rsx = inputSampleR;
					case 50: rsy = inputSampleR;
					case 51: rsz = inputSampleR;
					case 52: prevDistance = distanceSteps;
				}
			}
			switch (52-distanceSteps)
			{ //apply the stack of filter steps to produce the distance filter
				case  0: inputSampleR += rsz; inputSampleR *= 0.5; rsz = inputSampleR;
				case  1: inputSampleR += rsy; inputSampleR *= 0.5; rsy = inputSampleR;
				case  2: inputSampleR += rsx; inputSampleR *= 0.5; rsx = inputSampleR;
				case  3: inputSampleR += rsw; inputSampleR *= 0.5; rsw = inputSampleR;
				case  4: inputSampleR += rsv; inputSampleR *= 0.5; rsv = inputSampleR;
				case  5: inputSampleR += rsu; inputSampleR *= 0.5; rsu = inputSampleR;
				case  6: inputSampleR += rst; inputSampleR *= 0.5; rst = inputSampleR;
				case  7: inputSampleR += rss; inputSampleR *= 0.5; rss = inputSampleR;
				case  8: inputSampleR += rsr; inputSampleR *= 0.5; rsr = inputSampleR;
				case  9: inputSampleR += rsq; inputSampleR *= 0.5; rsq = inputSampleR;
				case 10: inputSampleR += rsp; inputSampleR *= 0.5; rsp = inputSampleR;
				case 11: inputSampleR += rso; inputSampleR *= 0.5; rso = inputSampleR;
				case 12: inputSampleR += rsn; inputSampleR *= 0.5; rsn = inputSampleR;
				case 13: inputSampleR += rsm; inputSampleR *= 0.5; rsm = inputSampleR;
				case 14: inputSampleR += rsl; inputSampleR *= 0.5; rsl = inputSampleR;
				case 15: inputSampleR += rsk; inputSampleR *= 0.5; rsk = inputSampleR;
				case 16: inputSampleR += rsj; inputSampleR *= 0.5; rsj = inputSampleR;
				case 17: inputSampleR += rsi; inputSampleR *= 0.5; rsi = inputSampleR;
				case 18: inputSampleR += rsh; inputSampleR *= 0.5; rsh = inputSampleR;
				case 19: inputSampleR += rsg; inputSampleR *= 0.5; rsg = inputSampleR;
				case 20: inputSampleR += rsf; inputSampleR *= 0.5; rsf = inputSampleR;
				case 21: inputSampleR += rse; inputSampleR *= 0.5; rse = inputSampleR;
				case 22: inputSampleR += rsd; inputSampleR *= 0.5; rsd = inputSampleR;
				case 23: inputSampleR += rsc; inputSampleR *= 0.5; rsc = inputSampleR;
				case 24: inputSampleR += rsb; inputSampleR *= 0.5; rsb = inputSampleR;
				case 25: inputSampleR += rsa; inputSampleR *= 0.5; rsa = inputSampleR;
				case 26: inputSampleR += rsZ; inputSampleR *= 0.5; rsZ = inputSampleR;
				case 27: inputSampleR += rsY; inputSampleR *= 0.5; rsY = inputSampleR;
				case 28: inputSampleR += rsX; inputSampleR *= 0.5; rsX = inputSampleR;
				case 29: inputSampleR += rsW; inputSampleR *= 0.5; rsW = inputSampleR;
				case 30: inputSampleR += rsV; inputSampleR *= 0.5; rsV = inputSampleR;
				case 31: inputSampleR += rsU; inputSampleR *= 0.5; rsU = inputSampleR;
				case 32: inputSampleR += rsT; inputSampleR *= 0.5; rsT = inputSampleR;
				case 33: inputSampleR += rsS; inputSampleR *= 0.5; rsS = inputSampleR;
				case 34: inputSampleR += rsR; inputSampleR *= 0.5; rsR = inputSampleR;
				case 35: inputSampleR += rsQ; inputSampleR *= 0.5; rsQ = inputSampleR;
				case 36: inputSampleR += rsP; inputSampleR *= 0.5; rsP = inputSampleR;
				case 37: inputSampleR += rsO; inputSampleR *= 0.5; rsO = inputSampleR;
				case 38: inputSampleR += rsN; inputSampleR *= 0.5; rsN = inputSampleR;
				case 39: inputSampleR += rsM; inputSampleR *= 0.5; rsM = inputSampleR;
				case 40: inputSampleR += rsL; inputSampleR *= 0.5; rsL = inputSampleR;
				case 41: inputSampleR += rsK; inputSampleR *= 0.5; rsK = inputSampleR;
				case 42: inputSampleR += rsJ; inputSampleR *= 0.5; rsJ = inputSampleR;
				case 43: inputSampleR += rsI; inputSampleR *= 0.5; rsI = inputSampleR;
				case 44: inputSampleR += rsH; inputSampleR *= 0.5; rsH = inputSampleR;
				case 45: inputSampleR += rsG; inputSampleR *= 0.5; rsG = inputSampleR;
				case 46: inputSampleR += rsF; inputSampleR *= 0.5; rsF = inputSampleR;
				case 47: inputSampleR += rsE; inputSampleR *= 0.5; rsE = inputSampleR;
				case 48: inputSampleR += rsD; inputSampleR *= 0.5; rsD = inputSampleR;
				case 49: inputSampleR += rsC; inputSampleR *= 0.5; rsC = inputSampleR;
				case 50: inputSampleR += rsB; inputSampleR *= 0.5; rsB = inputSampleR;
				case 51: inputSampleR += rsA; inputSampleR *= 0.5; rsA = inputSampleR;
				case 52: break;
			}
			//end distance filter R
			
			pointAL = pointBL;
			pointBL = inputSampleL;
			pointAR = pointBR;
			pointBR = inputSampleR;
			pointCycle = 0.0; //interpolation point values
			//this is the sample rate adjustment for the plugin version
			//game engine may well always run at 44.1k or whatever			
		}
		inputSampleL = pointAL+((pointBL-pointAL)*pointCycle);
		inputSampleR = pointAR+((pointBR-pointAR)*pointCycle);
		//this is the linear interpolation between reverb points
		//governed by sample rate, to work as a plugin
		
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

void Ultralight::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 48000.0;
	overallscale *= getSampleRate();
	float pointDerez = 1.0f / overallscale; //forcing it to be simple
	
	//these become presets and are built into the reverb when used
	float rtSixty = 0.0625f-(((int)(A*9.99))*0.003125f);
	int furnish = 4-(int)(B*4.99); //0 through 4 are valid
	int wallDns = (int)(C*4.99); //0 through 4 are valid
	int reflect = (int)(D*4.99); //0 through 4 are valid
	int preDlay = (int)((E*64.0)*(E*64.0)); //0 through 4096 are valid
	//making five additional const ints adjusting the node in question
	
	double wet = F;
	float distance = pow(G*7.28f,2.0f); //0 to 52.9984
	int distanceSteps = (int)distance; //52 maximum
	distance -= (float)distanceSteps; //0.9984
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		pointCycle += pointDerez;
		if (pointCycle > 0.9999999) {
			//this is sample rate handling for the plugin, so that
			//a more primitive reverb node can run at any rate.
			
			double drySampleL = inputSampleL;
			double drySampleR = inputSampleR;
			//predelay for plugin: not suitable for game engine,
			//as it doesn't handle changing delays gracefully.
			//However, it might be worth designing a FIXED predelay for
			//each verb node, on the grounds that if you're right next to it,
			//you'll hear maximum delay before the sound bounces off the first wall.
			//As such it'd always be the same delay, and so it'd not pose a problem,
			//even for sound events that are moving around in the verb area.
			//it'd also mean you could set zero for other purposes.
			lZ[cZ] = inputSampleL;
			rZ[cZ] = inputSampleR;
			cZ++; if (cZ < 0 || cZ > preDlay) cZ = 0;
			inputSampleL = lZ[cZ-((cZ > preDlay)?preDlay+1:0)];
			inputSampleR = rZ[cZ-((cZ > preDlay)?preDlay+1:0)];
			//end predelay
			
			//begin just what's in reverb node
			
			//allpass/comb section
			if (furnish < 4) {
				hAL = inputSampleL - (lA[(cA+1)-((cA+1 > d4A)?d4A+1:0)]*0.5);
				hAL += (fAL * rtSixty); lA[cA] = hAL; hAL *= 0.5;
				hAR = inputSampleR - (rA[(cA+1)-((cA+1 > d4A)?d4A+1:0)]*0.5);
				hAR += (fAR * rtSixty); rA[cA] = hAR; hAR *= 0.5;
				cA++;
				if (cA > d4A) cA = 0;
				hAL += lA[cA];
				hAR += rA[cA];
			} else {
				lA[cA] = inputSampleL + (fAL * rtSixty);
				rA[cA] = inputSampleR + (fAR * rtSixty);
				cA++;
				if (cA > d4A) cA = 0;
				hAL = lA[cA];
				hAR = rA[cA];
			}
			if (furnish < 3) {
				hBL = inputSampleL - (lB[(cB+1)-((cB+1 > d4B)?d4B+1:0)]*0.5);
				hBL += (fBL * rtSixty); lB[cB] = hBL; hBL *= 0.5;
				hBR = inputSampleR - (rB[(cB+1)-((cB+1 > d4B)?d4B+1:0)]*0.5);
				hBR += (fBR * rtSixty); rB[cB] = hBR; hBR *= 0.5;
				cB++;
				if (cB > d4B) cB = 0;
				hBL += lB[cB];
				hBR += rB[cB];
			} else {
				lB[cB] = inputSampleL + (fBL * rtSixty);
				rB[cB] = inputSampleR + (fBR * rtSixty);
				cB++;
				if (cB > d4B) cB = 0;
				hBL = lB[cB];
				hBR = rB[cB];
			}
			if (furnish < 2) {
				hCL = inputSampleL - (lC[(cC+1)-((cC+1 > d4C)?d4C+1:0)]*0.5);
				hCL += (fCL * rtSixty); lC[cC] = hCL; hCL *= 0.5;
				hCR = inputSampleR - (rC[(cC+1)-((cC+1 > d4C)?d4C+1:0)]*0.5);
				hCR += (fCR * rtSixty); rC[cC] = hCR; hCR *= 0.5;
				cC++;
				if (cC > d4C) cC = 0;
				hCL += lC[cC];
				hCR += rC[cC];
			} else {
				lC[cC] = inputSampleL + (fCL * rtSixty);
				rC[cC] = inputSampleR + (fCR * rtSixty);
				cC++;
				if (cC > d4C) cC = 0;
				hCL = lC[cC];
				hCR = rC[cC];
			}
			if (furnish < 1) {
				hDL = inputSampleL - (lD[(cD+1)-((cD+1 > d4D)?d4D+1:0)]*0.5);
				hDL += (fDL * rtSixty); lD[cD] = hDL; hDL *= 0.5;
				hDR = inputSampleR - (rD[(cD+1)-((cD+1 > d4D)?d4D+1:0)]*0.5);
				hDR += (fDR * rtSixty); rD[cD] = hDR; hDR *= 0.5;
				cD++;
				if (cD > d4D) cD = 0;
				hDL += lD[cD];
				hDR += rD[cD];
			} else {
				lD[cD] = inputSampleL + (fDL * rtSixty);
				rD[cD] = inputSampleR + (fDR * rtSixty);
				cD++;
				if (cD > d4D) cD = 0;
				hDL = lD[cD];
				hDR = rD[cD];
			}
			hEL = -(hAL + hBL + hCL + hDL);
			lE[cE] = fma(hAL,2.0f,hEL);
			lF[cF] = fma(hBL,2.0f,hEL);
			lG[cG] = fma(hCL,2.0f,hEL);
			lH[cH] = fma(hDL,2.0f,hEL);
			hER = -(hAR + hBR + hCR + hDR);
			rE[cE] = fma(hAR,2.0f,hER);
			rF[cF] = fma(hBR,2.0f,hER);
			rG[cG] = fma(hCR,2.0f,hER);
			rH[cH] = fma(hDR,2.0f,hER);
			//allpass/comb section
			
			cE++; cF++; cG++; cH++;
			if (cE > d4E) cE = 0;
			if (cF > d4F) cF = 0;
			if (cG > d4G) cG = 0;
			if (cH > d4H) cH = 0;
			
			hAL = lE[cE]; hBL = lF[cF]; hCL = lG[cG]; hDL = lH[cH];
			hEL = -(hAL + hBL + hCL + hDL);
			lI[cI] = fma(hAL,2.0f,hEL);
			lJ[cJ] = fma(hBL,2.0f,hEL);
			lK[cK] = fma(hCL,2.0f,hEL);
			lL[cL] = fma(hDL,2.0f,hEL);
			hAR = rE[cE]; hBR = rF[cF]; hCR = rG[cG]; hDR = rH[cH];
			hER = -(hAR + hBR + hCR + hDR);
			rI[cI] = fma(hAR,2.0f,hER);
			rJ[cJ] = fma(hBR,2.0f,hER);
			rK[cK] = fma(hCR,2.0f,hER);
			rL[cL] = fma(hDR,2.0f,hER);
			
			cI++; cJ++; cK++; cL++;
			if (cI > d4I) cI = 0;
			if (cJ > d4J) cJ = 0;
			if (cK > d4K) cK = 0;
			if (cL > d4L) cL = 0;
			
			hAL = lI[cI]; hBL = lJ[cJ]; hCL = lK[cK]; hDL = lL[cL];
			hEL = -(hAL + hBL + hCL + hDL);
			lM[cM] = fma(hAL,2.0f,hEL);
			lN[cN] = fma(hBL,2.0f,hEL);
			lO[cO] = fma(hCL,2.0f,hEL);
			lP[cP] = fma(hDL,2.0f,hEL);
			hAR = rI[cI]; hBR = rJ[cJ]; hCR = rK[cK]; hDR = rL[cL];
			hER = -(hAR + hBR + hCR + hDR);
			rM[cM] = fma(hAR,2.0f,hER);
			rN[cN] = fma(hBR,2.0f,hER);
			rO[cO] = fma(hCR,2.0f,hER);
			rP[cP] = fma(hDR,2.0f,hER);
			
			cM++; cN++; cO++; cP++;
			if (cM > d4M) cM = 0;
			if (cN > d4N) cN = 0;
			if (cO > d4O) cO = 0;
			if (cP > d4P) cP = 0;
			
			hAL = lM[cM]; hBL = lN[cN]; hCL = lO[cO]; hDL = lP[cP];
			hEL = -(hAL + hBL + hCL + hDL);
			fAL = fma(hAL,2.0f,hEL);				
			fBL = fma(hBL,2.0f,hEL);
			fCL = fma(hCL,2.0f,hEL);
			fDL = fma(hDL,2.0f,hEL);
			hAR = rM[cM]; hBR = rN[cN]; hCR = rO[cO]; hDR = rP[cP];
			hER = -(hAR + hBR + hCR + hDR);
			fAR = fma(hAR,2.0f,hER);				
			fBR = fma(hBR,2.0f,hER);
			fCR = fma(hCR,2.0f,hER);
			fDR = fma(hDR,2.0f,hER);
			
			switch (reflect)
			{
				case 0:
					fAL = (fAL+flAL)*0.5f; flAL = fAL;
					fAR = (fAR+flAR)*0.5f; flAR = fAR;
				case 1:
					fBL = (fBL+flBL)*0.5f; flBL = fBL;
					fBR = (fBR+flBR)*0.5f; flBR = fBR;
				case 2:
					fCL = (fCL+flCL)*0.5f; flCL = fCL;
					fCR = (fCR+flCR)*0.5f; flCR = fCR;
				case 3:
					fDL = (fDL+flDL)*0.5f; flDL = fDL;
					fDR = (fDR+flDR)*0.5f; flDR = fDR;
				case 4:
					break; //This applies ALL processing
					//after the starting point, meaning the
					//cases intentionally fall through
			}
			switch (wallDns)
			{
				case 0:
					fhAL *= 0.5f; fAL -= fhAL*0.0625f; fhAL += fAL;
					fhAR *= 0.5f; fAR -= fhAR*0.0625f; fhAR += fAR;
				case 1:
					fhBL *= 0.5f; fBL -= fhBL*0.0625f; fhBL += fBL;
					fhBR *= 0.5f; fBR -= fhBR*0.0625f; fhBR += fBR;
				case 2:
					fhCL *= 0.5f; fCL -= fhCL*0.0625f; fhCL += fCL;
					fhCR *= 0.5f; fCR -= fhCR*0.0625f; fhCR += fCR;
				case 3:
					fhDL *= 0.5f; fDL -= fhDL*0.0625f; fhDL += fDL;
					fhDR *= 0.5f; fDR -= fhDR*0.0625f; fhDR += fDR;
				case 4:
					break; //This applies ALL processing
					//after the starting point, meaning the
					//cases intentionally fall through
			}
			inputSampleL = (hAL + hBL + hCL + hDL)*0.0625;
			inputSampleR = (hAR + hBR + hCR + hDR)*0.0625;
			//end of just the reverb node part
			
			//dry/wet is not part of the game engine reverb node
			inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
			//plugin only: game engine one would have just the reverb,
			//and every sound source could have the following Distance filter,
			//including the localized reverb nodes. The reason it's here
			//is so the plugin can preview how dry+reverb, both distant, will sound
			
			//begin just the distance filter L
			firstDryL = inputSampleL; //start by doing the interpolation
			inputSampleL += firstAvgL; inputSampleL *= 0.5f; firstAvgL = inputSampleL;
			inputSampleL = (firstDryL*(1.0f-distance)) + (inputSampleL*distance);
			//having done at least one interpolation we can now do the integer number of stages
			if (prevDistance < distanceSteps) {
				switch (prevDistance)
				{
					case  0: lsA = inputSampleL; //if we've changed the setting
					case  1: lsB = inputSampleL; //reset the ones being put back into play
					case  2: lsC = inputSampleL; //this won't be happening constantly,
					case  3: lsD = inputSampleL; //but since we don't have an array
					case  4: lsE = inputSampleL; //this is the structure by which we
					case  5: lsF = inputSampleL; //get the new averagings going
					case  6: lsG = inputSampleL; //without pops or clicks
					case  7: lsH = inputSampleL;
					case  8: lsI = inputSampleL;
					case  9: lsJ = inputSampleL;
					case 10: lsK = inputSampleL;
					case 11: lsL = inputSampleL;
					case 12: lsM = inputSampleL;
					case 13: lsN = inputSampleL;
					case 14: lsO = inputSampleL;
					case 15: lsP = inputSampleL;
					case 16: lsQ = inputSampleL;
					case 17: lsR = inputSampleL;
					case 18: lsS = inputSampleL;
					case 19: lsT = inputSampleL;
					case 20: lsU = inputSampleL;
					case 21: lsV = inputSampleL;
					case 22: lsW = inputSampleL;
					case 23: lsX = inputSampleL;
					case 24: lsY = inputSampleL;
					case 25: lsZ = inputSampleL;
					case 26: lsa = inputSampleL;
					case 27: lsb = inputSampleL;
					case 28: lsc = inputSampleL;
					case 29: lsd = inputSampleL;
					case 30: lse = inputSampleL;
					case 31: lsf = inputSampleL;
					case 32: lsg = inputSampleL;
					case 33: lsh = inputSampleL;
					case 34: lsi = inputSampleL;
					case 35: lsj = inputSampleL;
					case 36: lsk = inputSampleL;
					case 37: lsl = inputSampleL;
					case 38: lsm = inputSampleL;
					case 39: lsn = inputSampleL;
					case 40: lso = inputSampleL;
					case 41: lsp = inputSampleL;
					case 42: lsq = inputSampleL;
					case 43: lsr = inputSampleL;
					case 44: lss = inputSampleL;
					case 45: lst = inputSampleL;
					case 46: lsu = inputSampleL;
					case 47: lsv = inputSampleL;
					case 48: lsw = inputSampleL;
					case 49: lsx = inputSampleL;
					case 50: lsy = inputSampleL;
					case 51: lsz = inputSampleL;
					case 52: break; //reset prevDistance after R channel
				}
			}
			switch (52-distanceSteps)
			{ //apply the stack of filter steps to produce the distance filter
				case  0: inputSampleL += lsz; inputSampleL *= 0.5; lsz = inputSampleL;
				case  1: inputSampleL += lsy; inputSampleL *= 0.5; lsy = inputSampleL;
				case  2: inputSampleL += lsx; inputSampleL *= 0.5; lsx = inputSampleL;
				case  3: inputSampleL += lsw; inputSampleL *= 0.5; lsw = inputSampleL;
				case  4: inputSampleL += lsv; inputSampleL *= 0.5; lsv = inputSampleL;
				case  5: inputSampleL += lsu; inputSampleL *= 0.5; lsu = inputSampleL;
				case  6: inputSampleL += lst; inputSampleL *= 0.5; lst = inputSampleL;
				case  7: inputSampleL += lss; inputSampleL *= 0.5; lss = inputSampleL;
				case  8: inputSampleL += lsr; inputSampleL *= 0.5; lsr = inputSampleL;
				case  9: inputSampleL += lsq; inputSampleL *= 0.5; lsq = inputSampleL;
				case 10: inputSampleL += lsp; inputSampleL *= 0.5; lsp = inputSampleL;
				case 11: inputSampleL += lso; inputSampleL *= 0.5; lso = inputSampleL;
				case 12: inputSampleL += lsn; inputSampleL *= 0.5; lsn = inputSampleL;
				case 13: inputSampleL += lsm; inputSampleL *= 0.5; lsm = inputSampleL;
				case 14: inputSampleL += lsl; inputSampleL *= 0.5; lsl = inputSampleL;
				case 15: inputSampleL += lsk; inputSampleL *= 0.5; lsk = inputSampleL;
				case 16: inputSampleL += lsj; inputSampleL *= 0.5; lsj = inputSampleL;
				case 17: inputSampleL += lsi; inputSampleL *= 0.5; lsi = inputSampleL;
				case 18: inputSampleL += lsh; inputSampleL *= 0.5; lsh = inputSampleL;
				case 19: inputSampleL += lsg; inputSampleL *= 0.5; lsg = inputSampleL;
				case 20: inputSampleL += lsf; inputSampleL *= 0.5; lsf = inputSampleL;
				case 21: inputSampleL += lse; inputSampleL *= 0.5; lse = inputSampleL;
				case 22: inputSampleL += lsd; inputSampleL *= 0.5; lsd = inputSampleL;
				case 23: inputSampleL += lsc; inputSampleL *= 0.5; lsc = inputSampleL;
				case 24: inputSampleL += lsb; inputSampleL *= 0.5; lsb = inputSampleL;
				case 25: inputSampleL += lsa; inputSampleL *= 0.5; lsa = inputSampleL;
				case 26: inputSampleL += lsZ; inputSampleL *= 0.5; lsZ = inputSampleL;
				case 27: inputSampleL += lsY; inputSampleL *= 0.5; lsY = inputSampleL;
				case 28: inputSampleL += lsX; inputSampleL *= 0.5; lsX = inputSampleL;
				case 29: inputSampleL += lsW; inputSampleL *= 0.5; lsW = inputSampleL;
				case 30: inputSampleL += lsV; inputSampleL *= 0.5; lsV = inputSampleL;
				case 31: inputSampleL += lsU; inputSampleL *= 0.5; lsU = inputSampleL;
				case 32: inputSampleL += lsT; inputSampleL *= 0.5; lsT = inputSampleL;
				case 33: inputSampleL += lsS; inputSampleL *= 0.5; lsS = inputSampleL;
				case 34: inputSampleL += lsR; inputSampleL *= 0.5; lsR = inputSampleL;
				case 35: inputSampleL += lsQ; inputSampleL *= 0.5; lsQ = inputSampleL;
				case 36: inputSampleL += lsP; inputSampleL *= 0.5; lsP = inputSampleL;
				case 37: inputSampleL += lsO; inputSampleL *= 0.5; lsO = inputSampleL;
				case 38: inputSampleL += lsN; inputSampleL *= 0.5; lsN = inputSampleL;
				case 39: inputSampleL += lsM; inputSampleL *= 0.5; lsM = inputSampleL;
				case 40: inputSampleL += lsL; inputSampleL *= 0.5; lsL = inputSampleL;
				case 41: inputSampleL += lsK; inputSampleL *= 0.5; lsK = inputSampleL;
				case 42: inputSampleL += lsJ; inputSampleL *= 0.5; lsJ = inputSampleL;
				case 43: inputSampleL += lsI; inputSampleL *= 0.5; lsI = inputSampleL;
				case 44: inputSampleL += lsH; inputSampleL *= 0.5; lsH = inputSampleL;
				case 45: inputSampleL += lsG; inputSampleL *= 0.5; lsG = inputSampleL;
				case 46: inputSampleL += lsF; inputSampleL *= 0.5; lsF = inputSampleL;
				case 47: inputSampleL += lsE; inputSampleL *= 0.5; lsE = inputSampleL;
				case 48: inputSampleL += lsD; inputSampleL *= 0.5; lsD = inputSampleL;
				case 49: inputSampleL += lsC; inputSampleL *= 0.5; lsC = inputSampleL;
				case 50: inputSampleL += lsB; inputSampleL *= 0.5; lsB = inputSampleL;
				case 51: inputSampleL += lsA; inputSampleL *= 0.5; lsA = inputSampleL;
				case 52: break;
			}
			//end distance filter L
			
			//begin just the distance filter R
			firstDryR = inputSampleR; //start by doing the interpolation
			inputSampleR += firstAvgR; inputSampleR *= 0.5f; firstAvgR = inputSampleR;
			inputSampleR = (firstDryR*(1.0f-distance)) + (inputSampleR*distance);
			//having done at least one interpolation we can now do the integer number of stages
			if (prevDistance < distanceSteps) {
				switch (prevDistance)
				{
					case  0: rsA = inputSampleR; //if we've changed the setting
					case  1: rsB = inputSampleR; //reset the ones being put back into play
					case  2: rsC = inputSampleR; //this won't be happening constantly,
					case  3: rsD = inputSampleR; //but since we don't have an array
					case  4: rsE = inputSampleR; //this is the structure by which we
					case  5: rsF = inputSampleR; //get the new averagings going
					case  6: rsG = inputSampleR; //without pops or clicks
					case  7: rsH = inputSampleR;
					case  8: rsI = inputSampleR;
					case  9: rsJ = inputSampleR;
					case 10: rsK = inputSampleR;
					case 11: rsL = inputSampleR;
					case 12: rsM = inputSampleR;
					case 13: rsN = inputSampleR;
					case 14: rsO = inputSampleR;
					case 15: rsP = inputSampleR;
					case 16: rsQ = inputSampleR;
					case 17: rsR = inputSampleR;
					case 18: rsS = inputSampleR;
					case 19: rsT = inputSampleR;
					case 20: rsU = inputSampleR;
					case 21: rsV = inputSampleR;
					case 22: rsW = inputSampleR;
					case 23: rsX = inputSampleR;
					case 24: rsY = inputSampleR;
					case 25: rsZ = inputSampleR;
					case 26: rsa = inputSampleR;
					case 27: rsb = inputSampleR;
					case 28: rsc = inputSampleR;
					case 29: rsd = inputSampleR;
					case 30: rse = inputSampleR;
					case 31: rsf = inputSampleR;
					case 32: rsg = inputSampleR;
					case 33: rsh = inputSampleR;
					case 34: rsi = inputSampleR;
					case 35: rsj = inputSampleR;
					case 36: rsk = inputSampleR;
					case 37: rsl = inputSampleR;
					case 38: rsm = inputSampleR;
					case 39: rsn = inputSampleR;
					case 40: rso = inputSampleR;
					case 41: rsp = inputSampleR;
					case 42: rsq = inputSampleR;
					case 43: rsr = inputSampleR;
					case 44: rss = inputSampleR;
					case 45: rst = inputSampleR;
					case 46: rsu = inputSampleR;
					case 47: rsv = inputSampleR;
					case 48: rsw = inputSampleR;
					case 49: rsx = inputSampleR;
					case 50: rsy = inputSampleR;
					case 51: rsz = inputSampleR;
					case 52: prevDistance = distanceSteps;
				}
			}
			switch (52-distanceSteps)
			{ //apply the stack of filter steps to produce the distance filter
				case  0: inputSampleR += rsz; inputSampleR *= 0.5; rsz = inputSampleR;
				case  1: inputSampleR += rsy; inputSampleR *= 0.5; rsy = inputSampleR;
				case  2: inputSampleR += rsx; inputSampleR *= 0.5; rsx = inputSampleR;
				case  3: inputSampleR += rsw; inputSampleR *= 0.5; rsw = inputSampleR;
				case  4: inputSampleR += rsv; inputSampleR *= 0.5; rsv = inputSampleR;
				case  5: inputSampleR += rsu; inputSampleR *= 0.5; rsu = inputSampleR;
				case  6: inputSampleR += rst; inputSampleR *= 0.5; rst = inputSampleR;
				case  7: inputSampleR += rss; inputSampleR *= 0.5; rss = inputSampleR;
				case  8: inputSampleR += rsr; inputSampleR *= 0.5; rsr = inputSampleR;
				case  9: inputSampleR += rsq; inputSampleR *= 0.5; rsq = inputSampleR;
				case 10: inputSampleR += rsp; inputSampleR *= 0.5; rsp = inputSampleR;
				case 11: inputSampleR += rso; inputSampleR *= 0.5; rso = inputSampleR;
				case 12: inputSampleR += rsn; inputSampleR *= 0.5; rsn = inputSampleR;
				case 13: inputSampleR += rsm; inputSampleR *= 0.5; rsm = inputSampleR;
				case 14: inputSampleR += rsl; inputSampleR *= 0.5; rsl = inputSampleR;
				case 15: inputSampleR += rsk; inputSampleR *= 0.5; rsk = inputSampleR;
				case 16: inputSampleR += rsj; inputSampleR *= 0.5; rsj = inputSampleR;
				case 17: inputSampleR += rsi; inputSampleR *= 0.5; rsi = inputSampleR;
				case 18: inputSampleR += rsh; inputSampleR *= 0.5; rsh = inputSampleR;
				case 19: inputSampleR += rsg; inputSampleR *= 0.5; rsg = inputSampleR;
				case 20: inputSampleR += rsf; inputSampleR *= 0.5; rsf = inputSampleR;
				case 21: inputSampleR += rse; inputSampleR *= 0.5; rse = inputSampleR;
				case 22: inputSampleR += rsd; inputSampleR *= 0.5; rsd = inputSampleR;
				case 23: inputSampleR += rsc; inputSampleR *= 0.5; rsc = inputSampleR;
				case 24: inputSampleR += rsb; inputSampleR *= 0.5; rsb = inputSampleR;
				case 25: inputSampleR += rsa; inputSampleR *= 0.5; rsa = inputSampleR;
				case 26: inputSampleR += rsZ; inputSampleR *= 0.5; rsZ = inputSampleR;
				case 27: inputSampleR += rsY; inputSampleR *= 0.5; rsY = inputSampleR;
				case 28: inputSampleR += rsX; inputSampleR *= 0.5; rsX = inputSampleR;
				case 29: inputSampleR += rsW; inputSampleR *= 0.5; rsW = inputSampleR;
				case 30: inputSampleR += rsV; inputSampleR *= 0.5; rsV = inputSampleR;
				case 31: inputSampleR += rsU; inputSampleR *= 0.5; rsU = inputSampleR;
				case 32: inputSampleR += rsT; inputSampleR *= 0.5; rsT = inputSampleR;
				case 33: inputSampleR += rsS; inputSampleR *= 0.5; rsS = inputSampleR;
				case 34: inputSampleR += rsR; inputSampleR *= 0.5; rsR = inputSampleR;
				case 35: inputSampleR += rsQ; inputSampleR *= 0.5; rsQ = inputSampleR;
				case 36: inputSampleR += rsP; inputSampleR *= 0.5; rsP = inputSampleR;
				case 37: inputSampleR += rsO; inputSampleR *= 0.5; rsO = inputSampleR;
				case 38: inputSampleR += rsN; inputSampleR *= 0.5; rsN = inputSampleR;
				case 39: inputSampleR += rsM; inputSampleR *= 0.5; rsM = inputSampleR;
				case 40: inputSampleR += rsL; inputSampleR *= 0.5; rsL = inputSampleR;
				case 41: inputSampleR += rsK; inputSampleR *= 0.5; rsK = inputSampleR;
				case 42: inputSampleR += rsJ; inputSampleR *= 0.5; rsJ = inputSampleR;
				case 43: inputSampleR += rsI; inputSampleR *= 0.5; rsI = inputSampleR;
				case 44: inputSampleR += rsH; inputSampleR *= 0.5; rsH = inputSampleR;
				case 45: inputSampleR += rsG; inputSampleR *= 0.5; rsG = inputSampleR;
				case 46: inputSampleR += rsF; inputSampleR *= 0.5; rsF = inputSampleR;
				case 47: inputSampleR += rsE; inputSampleR *= 0.5; rsE = inputSampleR;
				case 48: inputSampleR += rsD; inputSampleR *= 0.5; rsD = inputSampleR;
				case 49: inputSampleR += rsC; inputSampleR *= 0.5; rsC = inputSampleR;
				case 50: inputSampleR += rsB; inputSampleR *= 0.5; rsB = inputSampleR;
				case 51: inputSampleR += rsA; inputSampleR *= 0.5; rsA = inputSampleR;
				case 52: break;
			}
			//end distance filter R
			
			pointAL = pointBL;
			pointBL = inputSampleL;
			pointAR = pointBR;
			pointBR = inputSampleR;
			pointCycle = 0.0; //interpolation point values
			//this is the sample rate adjustment for the plugin version
			//game engine may well always run at 44.1k or whatever			
		}
		inputSampleL = pointAL+((pointBL-pointAL)*pointCycle);
		inputSampleR = pointAR+((pointBR-pointAR)*pointCycle);
		//this is the linear interpolation between reverb points
		//governed by sample rate, to work as a plugin
		
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
