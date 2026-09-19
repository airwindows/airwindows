/* ========================================
 *  Ultralight2 - Ultralight2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Ultralight2_H
#include "Ultralight2.h"
#endif

void Ultralight2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	float overallscale = 1.0f;
	overallscale /= 48000.0f;
	overallscale *= getSampleRate();
	float pointDerez = 1.0f / overallscale; //forcing it to be simple
	
	bool bypassPre = (((int)(D*998.0f)) < 1); //plugin only
	//the hardcoded verb objects use set matrix delay sizes, then the following four extras:
	float X = D*998.0f;
	int dpT = prime[(int)X];//only a delay, not an allpass
	X = sqrt(X*81.408f);
	int dpQ = prime[(int)X];//final allpass
	X = sqrt(X*81.408f);
	int dpR = prime[(int)X];//second allpass
	X = sqrt(X*81.408f);
	int dpS = prime[(int)X];//first allpass
	//this produces four ints that declare arrays like the matrix delays,
	//which replaces the plugin versions set up to scale up to the maximum predelay
	//so, here's how we gnerate const int d4Q-R-S-T with T first, then QRS
	
	//X = the number in Param D; d4T = prime[(int)X];
	//X = sqrt(X*81.408); d4Q = prime[(int)X];
	//X = sqrt(X*81.408); d4R = prime[(int)X];
	//X = sqrt(X*81.408); d4S = prime[(int)X];
	
	bool bypassVerb = (((int)(A*9.99f)) < 1); //plugin only
	//and then the following three adjustments for RT60 and tone shaping.
	float damping = 0.0625f-(((int)(A*9.99f))*0.003125f); //0 bypass, 1-9 valid
	int unSolid = 4-(int)(B*4.99f); //0 through 4 are valid
	int unReflect = 4-(int)(C*4.99f); //0 through 4 are valid
	//lastly, the verb object gets three more ints, d4U-V-W, damping-unsolid-unreflect.
	//it must generate its damping value from infinite sustain minus the damping int,
	//then unsolid/unreflect just selects which path to highpass/lowpass, or not.
	
	//damping = 0.0625f-(d4U*0.003125f); //will be 1-9
	//use d4V for unSolid, use d4W for unReflect
	
	float wet = E;
	//this plugin applies distance filter to dry AND wet, so that when testing
	//out possible spaces for the game engine, you can hear what it'll sound like
	//in practice when both the source sound and the verb are distant.
	float distance = pow(F*7.28f,2.0f); //0 to 52.9984
	int distanceSteps = (int)distance; //52 maximum
	distance -= (float)distanceSteps; //0.9984
	//distance is not a property of the reverb node, it's a property of
	//the game audio engine, where every source can be localized
	
    while (--sampleFrames >= 0)
    {
		float inputSampleL = *in1;
		float inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		pointCycle += pointDerez;
		if (pointCycle > 0.9999999f) {
			//this is sample rate handling for the plugin, so that
			//a more primitive reverb node can run at any rate.
			
			float drySampleL = inputSampleL;
			float drySampleR = inputSampleR;
			//predelay for plugin: not suitable for game engine,
			//as it doesn't handle changing delays gracefully.
			//However, we are going to run a FIXED predelay for
			//each verb node, on the grounds that if you're right next to it,
			//you'll hear maximum delay before the sound bounces off the first wall.
			//As such it'd always be the same delay, and so it'd not pose a problem,
			//even for sound events that are moving around in the verb area.
			//This is it, combined with three inline allpasses that are tuned
			//independently to the predelay, set up so the compiler can
			//vectorize the lot of them as SIMD operations.
			//Note that some old AMD processors may perform worse here.
			
			oQL = oRL - (lQ[(cQ+1)-((cQ+1 > dpQ)?dpQ+1:0)]*0.5f);
			oRL = oSL - (lR[(cR+1)-((cR+1 > dpR)?dpR+1:0)]*0.5f);
			oSL = oTL - (lS[(cS+1)-((cS+1 > dpS)?dpS+1:0)]*0.5f);
			oTL = inputSampleL; //SIMDable, but lT[] is just a simple delay
			oQR = oRR - (rQ[(cQ+1)-((cQ+1 > dpQ)?dpQ+1:0)]*0.5f);
			oRR = oSR - (rR[(cR+1)-((cR+1 > dpR)?dpR+1:0)]*0.5f);
			oSR = oTR - (rS[(cS+1)-((cS+1 > dpS)?dpS+1:0)]*0.5f);
			oTR = inputSampleR; //SIMDable, but rT[] is just a simple delay
			lQ[cQ] = oQL; lR[cR] = oRL; lS[cS] = oSL; lT[cT] = oTL;
			rQ[cQ] = oQR; rR[cR] = oRR; rS[cS] = oSR; rT[cT] = oTR;
			cQ++; cR++; cS++; cT++;
			if (cQ > dpQ) cQ = 0;
			if (cR > dpR) cR = 0;
			if (cS > dpS) cS = 0;
			if (cT > dpT) cT = 0;
			oQL = fma(oQL,0.5f,lQ[cQ]);
			oRL = fma(oRL,0.5f,lR[cR]);
			oSL = fma(oSL,0.5f,lS[cS]);
			oTL = fma(oTL,0.0f,lT[cT]); //not allpass but can use vector fma()
			oQR = fma(oQR,0.5f,rQ[cQ]);
			oRR = fma(oRR,0.5f,rR[cR]);
			oSR = fma(oSR,0.5f,rS[cS]);
			oTR = fma(oTR,0.0f,rT[cT]); //not allpass but can use vector fma()
			
			if (bypassPre) {oQL = inputSampleL; oQR = inputSampleR;} //plugin only
			
			lA[cA] = fma(fAL,damping,oQL);
			lB[cB] = fma(fBL,damping,oQL);
			lC[cC] = fma(fCL,damping,oQL);
			lD[cD] = fma(fDL,damping,oQL);
			rA[cA] = fma(fAR,damping,oQR);
			rB[cB] = fma(fBR,damping,oQR);
			rC[cC] = fma(fCR,damping,oQR);
			rD[cD] = fma(fDR,damping,oQR);
			cA++; cB++; cC++; cD++;
			if (cA > d4A) cA = 0;
			if (cB > d4B) cB = 0;
			if (cC > d4C) cC = 0;
			if (cD > d4D) cD = 0;
			hAL = lA[cA]; hBL = lB[cB]; hCL = lC[cC]; hDL = lD[cD];
			hAR = rA[cA]; hBR = rB[cB]; hCR = rC[cC]; hDR = rD[cD];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lE[cE] = fma(hAL,2.0f,hEL);
			lF[cF] = fma(hBL,2.0f,hEL);
			lG[cG] = fma(hCL,2.0f,hEL);
			lH[cH] = fma(hDL,2.0f,hEL);
			rE[cE] = fma(hAR,2.0f,hER);
			rF[cF] = fma(hBR,2.0f,hER);
			rG[cG] = fma(hCR,2.0f,hER);
			rH[cH] = fma(hDR,2.0f,hER);
			cE++; cF++; cG++; cH++;
			if (cE > d4E) cE = 0;
			if (cF > d4F) cF = 0;
			if (cG > d4G) cG = 0;
			if (cH > d4H) cH = 0;
			hAL = lE[cE]; hBL = lF[cF]; hCL = lG[cG]; hDL = lH[cH];
			hAR = rE[cE]; hBR = rF[cF]; hCR = rG[cG]; hDR = rH[cH];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lI[cI] = fma(hAL,2.0f,hEL);
			lJ[cJ] = fma(hBL,2.0f,hEL);
			lK[cK] = fma(hCL,2.0f,hEL);
			lL[cL] = fma(hDL,2.0f,hEL);
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
			hAR = rI[cI]; hBR = rJ[cJ]; hCR = rK[cK]; hDR = rL[cL];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lM[cM] = fma(hAL,2.0f,hEL);
			lN[cN] = fma(hBL,2.0f,hEL);
			lO[cO] = fma(hCL,2.0f,hEL);
			lP[cP] = fma(hDL,2.0f,hEL);
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
			hAR = rM[cM]; hBR = rN[cN]; hCR = rO[cO]; hDR = rP[cP];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			fAL = fma(hAL,2.0f,hEL);				
			fBL = fma(hBL,2.0f,hEL);
			fCL = fma(hCL,2.0f,hEL);
			fDL = fma(hDL,2.0f,hEL);
			fAR = fma(hAR,2.0f,hER);				
			fBR = fma(hBR,2.0f,hER);
			fCR = fma(hCR,2.0f,hER);
			fDR = fma(hDR,2.0f,hER);
			switch (unSolid)
			{
				case 0:
					fhAL *= 0.25f; fAL -= fhAL*0.125f; fhAL += fAL;
					fhAR *= 0.25f; fAR -= fhAR*0.125f; fhAR += fAR;
				case 1:
					fhBL *= 0.25f; fBL -= fhBL*0.125f; fhBL += fBL;
					fhBR *= 0.25f; fBR -= fhBR*0.125f; fhBR += fBR;
				case 2:
					fhCL *= 0.25f; fCL -= fhCL*0.125f; fhCL += fCL;
					fhCR *= 0.25f; fCR -= fhCR*0.125f; fhCR += fCR;
				case 3:
					fhDL *= 0.25f; fDL -= fhDL*0.125f; fhDL += fDL;
					fhDR *= 0.25f; fDR -= fhDR*0.125f; fhDR += fDR;
				case 4:
					break;
			}
			switch (unReflect)
			{
				case 0:
					fDL = (fDL+flDL)*0.5f; flDL = fDL;
					fDR = (fDR+flDR)*0.5f; flDR = fDR;
				case 1:
					fCL = (fCL+flCL)*0.5f; flCL = fCL;
					fCR = (fCR+flCR)*0.5f; flCR = fCR;
				case 2:
					fBL = (fBL+flBL)*0.5f; flBL = fBL;
					fBR = (fBR+flBR)*0.5f; flBR = fBR;
				case 3:
					fAL = (fAL+flAL)*0.5f; flAL = fAL;
					fAR = (fAR+flAR)*0.5f; flAR = fAR;
				case 4:
					break;
			}
			inputSampleL = (hAL + hBL + hCL + hDL)*0.0625f;
			inputSampleR = (hAR + hBR + hCR + hDR)*0.0625f;
			//end of just the reverb node part
			
			if (bypassVerb) {inputSampleL = oQL; inputSampleR = oQR;} //plugin only
			//dry/wet is not part of the game engine reverb node
			inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0f-wet));
			inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0f-wet));
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
				case  0: inputSampleL += lsz; inputSampleL *= 0.5f; lsz = inputSampleL;
				case  1: inputSampleL += lsy; inputSampleL *= 0.5f; lsy = inputSampleL;
				case  2: inputSampleL += lsx; inputSampleL *= 0.5f; lsx = inputSampleL;
				case  3: inputSampleL += lsw; inputSampleL *= 0.5f; lsw = inputSampleL;
				case  4: inputSampleL += lsv; inputSampleL *= 0.5f; lsv = inputSampleL;
				case  5: inputSampleL += lsu; inputSampleL *= 0.5f; lsu = inputSampleL;
				case  6: inputSampleL += lst; inputSampleL *= 0.5f; lst = inputSampleL;
				case  7: inputSampleL += lss; inputSampleL *= 0.5f; lss = inputSampleL;
				case  8: inputSampleL += lsr; inputSampleL *= 0.5f; lsr = inputSampleL;
				case  9: inputSampleL += lsq; inputSampleL *= 0.5f; lsq = inputSampleL;
				case 10: inputSampleL += lsp; inputSampleL *= 0.5f; lsp = inputSampleL;
				case 11: inputSampleL += lso; inputSampleL *= 0.5f; lso = inputSampleL;
				case 12: inputSampleL += lsn; inputSampleL *= 0.5f; lsn = inputSampleL;
				case 13: inputSampleL += lsm; inputSampleL *= 0.5f; lsm = inputSampleL;
				case 14: inputSampleL += lsl; inputSampleL *= 0.5f; lsl = inputSampleL;
				case 15: inputSampleL += lsk; inputSampleL *= 0.5f; lsk = inputSampleL;
				case 16: inputSampleL += lsj; inputSampleL *= 0.5f; lsj = inputSampleL;
				case 17: inputSampleL += lsi; inputSampleL *= 0.5f; lsi = inputSampleL;
				case 18: inputSampleL += lsh; inputSampleL *= 0.5f; lsh = inputSampleL;
				case 19: inputSampleL += lsg; inputSampleL *= 0.5f; lsg = inputSampleL;
				case 20: inputSampleL += lsf; inputSampleL *= 0.5f; lsf = inputSampleL;
				case 21: inputSampleL += lse; inputSampleL *= 0.5f; lse = inputSampleL;
				case 22: inputSampleL += lsd; inputSampleL *= 0.5f; lsd = inputSampleL;
				case 23: inputSampleL += lsc; inputSampleL *= 0.5f; lsc = inputSampleL;
				case 24: inputSampleL += lsb; inputSampleL *= 0.5f; lsb = inputSampleL;
				case 25: inputSampleL += lsa; inputSampleL *= 0.5f; lsa = inputSampleL;
				case 26: inputSampleL += lsZ; inputSampleL *= 0.5f; lsZ = inputSampleL;
				case 27: inputSampleL += lsY; inputSampleL *= 0.5f; lsY = inputSampleL;
				case 28: inputSampleL += lsX; inputSampleL *= 0.5f; lsX = inputSampleL;
				case 29: inputSampleL += lsW; inputSampleL *= 0.5f; lsW = inputSampleL;
				case 30: inputSampleL += lsV; inputSampleL *= 0.5f; lsV = inputSampleL;
				case 31: inputSampleL += lsU; inputSampleL *= 0.5f; lsU = inputSampleL;
				case 32: inputSampleL += lsT; inputSampleL *= 0.5f; lsT = inputSampleL;
				case 33: inputSampleL += lsS; inputSampleL *= 0.5f; lsS = inputSampleL;
				case 34: inputSampleL += lsR; inputSampleL *= 0.5f; lsR = inputSampleL;
				case 35: inputSampleL += lsQ; inputSampleL *= 0.5f; lsQ = inputSampleL;
				case 36: inputSampleL += lsP; inputSampleL *= 0.5f; lsP = inputSampleL;
				case 37: inputSampleL += lsO; inputSampleL *= 0.5f; lsO = inputSampleL;
				case 38: inputSampleL += lsN; inputSampleL *= 0.5f; lsN = inputSampleL;
				case 39: inputSampleL += lsM; inputSampleL *= 0.5f; lsM = inputSampleL;
				case 40: inputSampleL += lsL; inputSampleL *= 0.5f; lsL = inputSampleL;
				case 41: inputSampleL += lsK; inputSampleL *= 0.5f; lsK = inputSampleL;
				case 42: inputSampleL += lsJ; inputSampleL *= 0.5f; lsJ = inputSampleL;
				case 43: inputSampleL += lsI; inputSampleL *= 0.5f; lsI = inputSampleL;
				case 44: inputSampleL += lsH; inputSampleL *= 0.5f; lsH = inputSampleL;
				case 45: inputSampleL += lsG; inputSampleL *= 0.5f; lsG = inputSampleL;
				case 46: inputSampleL += lsF; inputSampleL *= 0.5f; lsF = inputSampleL;
				case 47: inputSampleL += lsE; inputSampleL *= 0.5f; lsE = inputSampleL;
				case 48: inputSampleL += lsD; inputSampleL *= 0.5f; lsD = inputSampleL;
				case 49: inputSampleL += lsC; inputSampleL *= 0.5f; lsC = inputSampleL;
				case 50: inputSampleL += lsB; inputSampleL *= 0.5f; lsB = inputSampleL;
				case 51: inputSampleL += lsA; inputSampleL *= 0.5f; lsA = inputSampleL;
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
				case  0: inputSampleR += rsz; inputSampleR *= 0.5f; rsz = inputSampleR;
				case  1: inputSampleR += rsy; inputSampleR *= 0.5f; rsy = inputSampleR;
				case  2: inputSampleR += rsx; inputSampleR *= 0.5f; rsx = inputSampleR;
				case  3: inputSampleR += rsw; inputSampleR *= 0.5f; rsw = inputSampleR;
				case  4: inputSampleR += rsv; inputSampleR *= 0.5f; rsv = inputSampleR;
				case  5: inputSampleR += rsu; inputSampleR *= 0.5f; rsu = inputSampleR;
				case  6: inputSampleR += rst; inputSampleR *= 0.5f; rst = inputSampleR;
				case  7: inputSampleR += rss; inputSampleR *= 0.5f; rss = inputSampleR;
				case  8: inputSampleR += rsr; inputSampleR *= 0.5f; rsr = inputSampleR;
				case  9: inputSampleR += rsq; inputSampleR *= 0.5f; rsq = inputSampleR;
				case 10: inputSampleR += rsp; inputSampleR *= 0.5f; rsp = inputSampleR;
				case 11: inputSampleR += rso; inputSampleR *= 0.5f; rso = inputSampleR;
				case 12: inputSampleR += rsn; inputSampleR *= 0.5f; rsn = inputSampleR;
				case 13: inputSampleR += rsm; inputSampleR *= 0.5f; rsm = inputSampleR;
				case 14: inputSampleR += rsl; inputSampleR *= 0.5f; rsl = inputSampleR;
				case 15: inputSampleR += rsk; inputSampleR *= 0.5f; rsk = inputSampleR;
				case 16: inputSampleR += rsj; inputSampleR *= 0.5f; rsj = inputSampleR;
				case 17: inputSampleR += rsi; inputSampleR *= 0.5f; rsi = inputSampleR;
				case 18: inputSampleR += rsh; inputSampleR *= 0.5f; rsh = inputSampleR;
				case 19: inputSampleR += rsg; inputSampleR *= 0.5f; rsg = inputSampleR;
				case 20: inputSampleR += rsf; inputSampleR *= 0.5f; rsf = inputSampleR;
				case 21: inputSampleR += rse; inputSampleR *= 0.5f; rse = inputSampleR;
				case 22: inputSampleR += rsd; inputSampleR *= 0.5f; rsd = inputSampleR;
				case 23: inputSampleR += rsc; inputSampleR *= 0.5f; rsc = inputSampleR;
				case 24: inputSampleR += rsb; inputSampleR *= 0.5f; rsb = inputSampleR;
				case 25: inputSampleR += rsa; inputSampleR *= 0.5f; rsa = inputSampleR;
				case 26: inputSampleR += rsZ; inputSampleR *= 0.5f; rsZ = inputSampleR;
				case 27: inputSampleR += rsY; inputSampleR *= 0.5f; rsY = inputSampleR;
				case 28: inputSampleR += rsX; inputSampleR *= 0.5f; rsX = inputSampleR;
				case 29: inputSampleR += rsW; inputSampleR *= 0.5f; rsW = inputSampleR;
				case 30: inputSampleR += rsV; inputSampleR *= 0.5f; rsV = inputSampleR;
				case 31: inputSampleR += rsU; inputSampleR *= 0.5f; rsU = inputSampleR;
				case 32: inputSampleR += rsT; inputSampleR *= 0.5f; rsT = inputSampleR;
				case 33: inputSampleR += rsS; inputSampleR *= 0.5f; rsS = inputSampleR;
				case 34: inputSampleR += rsR; inputSampleR *= 0.5f; rsR = inputSampleR;
				case 35: inputSampleR += rsQ; inputSampleR *= 0.5f; rsQ = inputSampleR;
				case 36: inputSampleR += rsP; inputSampleR *= 0.5f; rsP = inputSampleR;
				case 37: inputSampleR += rsO; inputSampleR *= 0.5f; rsO = inputSampleR;
				case 38: inputSampleR += rsN; inputSampleR *= 0.5f; rsN = inputSampleR;
				case 39: inputSampleR += rsM; inputSampleR *= 0.5f; rsM = inputSampleR;
				case 40: inputSampleR += rsL; inputSampleR *= 0.5f; rsL = inputSampleR;
				case 41: inputSampleR += rsK; inputSampleR *= 0.5f; rsK = inputSampleR;
				case 42: inputSampleR += rsJ; inputSampleR *= 0.5f; rsJ = inputSampleR;
				case 43: inputSampleR += rsI; inputSampleR *= 0.5f; rsI = inputSampleR;
				case 44: inputSampleR += rsH; inputSampleR *= 0.5f; rsH = inputSampleR;
				case 45: inputSampleR += rsG; inputSampleR *= 0.5f; rsG = inputSampleR;
				case 46: inputSampleR += rsF; inputSampleR *= 0.5f; rsF = inputSampleR;
				case 47: inputSampleR += rsE; inputSampleR *= 0.5f; rsE = inputSampleR;
				case 48: inputSampleR += rsD; inputSampleR *= 0.5f; rsD = inputSampleR;
				case 49: inputSampleR += rsC; inputSampleR *= 0.5f; rsC = inputSampleR;
				case 50: inputSampleR += rsB; inputSampleR *= 0.5f; rsB = inputSampleR;
				case 51: inputSampleR += rsA; inputSampleR *= 0.5f; rsA = inputSampleR;
				case 52: break;
			}
			//end distance filter R
			
			pointAL = pointBL;
			pointBL = inputSampleL;
			pointAR = pointBR;
			pointBR = inputSampleR;
			pointCycle = 0.0f; //interpolation point values
			//this is the sample rate adjustment for the plugin version
			//game engine may well always run at 44.1k or whatever			
		}
		inputSampleL = pointAL+((pointBL-pointAL)*pointCycle);
		inputSampleR = pointAR+((pointBR-pointAR)*pointCycle);
		//this is the linear interpolation between reverb points
		//governed by sample rate, to work as a plugin
		
		//begin 32 bit stereo floating point dither
		//int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		//frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		///inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Ultralight2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	float overallscale = 1.0f;
	overallscale /= 48000.0f;
	overallscale *= getSampleRate();
	float pointDerez = 1.0f / overallscale; //forcing it to be simple
	
	bool bypassPre = (((int)(D*998.0)) < 1); //plugin only
	//the hardcoded verb objects use set matrix delay sizes, then the following four extras:
	float X = D*998.0f;
	int dpT = prime[(int)X];//only a delay, not an allpass
	X = sqrt(X*81.408f);
	int dpQ = prime[(int)X];//final allpass
	X = sqrt(X*81.408f);
	int dpR = prime[(int)X];//second allpass
	X = sqrt(X*81.408f);
	int dpS = prime[(int)X];//first allpass
	//this produces four ints that declare arrays like the matrix delays,
	//which replaces the plugin versions set up to scale up to the maximum predelay
	//so, here's how we gnerate const int d4Q-R-S-T with T first, then QRS
	
	//X = the number in Param D; d4T = prime[(int)X];
	//X = sqrt(X*81.408); d4Q = prime[(int)X];
	//X = sqrt(X*81.408); d4R = prime[(int)X];
	//X = sqrt(X*81.408); d4S = prime[(int)X];
	
	bool bypassVerb = (((int)(A*9.99f)) < 1); //plugin only
	//and then the following three adjustments for RT60 and tone shaping.
	float damping = 0.0625f-(((int)(A*9.99f))*0.003125f); //0 bypass, 1-9 valid
	int unSolid = 4-(int)(B*4.99f); //0 through 4 are valid
	int unReflect = 4-(int)(C*4.99f); //0 through 4 are valid
	//lastly, the verb object gets three more ints, d4U-V-W, damping-unsolid-unreflect.
	//it must generate its damping value from infinite sustain minus the damping int,
	//then unsolid/unreflect just selects which path to highpass/lowpass, or not.
	
	//damping = 0.0625f-(d4U*0.003125f); //will be 1-9
	//use d4V for unSolid, use d4W for unReflect
	
	float wet = E;
	//this plugin applies distance filter to dry AND wet, so that when testing
	//out possible spaces for the game engine, you can hear what it'll sound like
	//in practice when both the source sound and the verb are distant.
	float distance = pow(F*7.28f,2.0f); //0 to 52.9984
	int distanceSteps = (int)distance; //52 maximum
	distance -= (float)distanceSteps; //0.9984
	//distance is not a property of the reverb node, it's a property of
	//the game audio engine, where every source can be localized
	
    while (--sampleFrames >= 0)
    {
		float inputSampleL = *in1;
		float inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		pointCycle += pointDerez;
		if (pointCycle > 0.9999999f) {
			//this is sample rate handling for the plugin, so that
			//a more primitive reverb node can run at any rate.
			
			float drySampleL = inputSampleL;
			float drySampleR = inputSampleR;
			//predelay for plugin: not suitable for game engine,
			//as it doesn't handle changing delays gracefully.
			//However, we are going to run a FIXED predelay for
			//each verb node, on the grounds that if you're right next to it,
			//you'll hear maximum delay before the sound bounces off the first wall.
			//As such it'd always be the same delay, and so it'd not pose a problem,
			//even for sound events that are moving around in the verb area.
			//This is it, combined with three inline allpasses that are tuned
			//independently to the predelay, set up so the compiler can
			//vectorize the lot of them as SIMD operations.
			//Note that some old AMD processors may perform worse here.
			
			oQL = oRL - (lQ[(cQ+1)-((cQ+1 > dpQ)?dpQ+1:0)]*0.5f);
			oRL = oSL - (lR[(cR+1)-((cR+1 > dpR)?dpR+1:0)]*0.5f);
			oSL = oTL - (lS[(cS+1)-((cS+1 > dpS)?dpS+1:0)]*0.5f);
			oTL = inputSampleL; //SIMDable, but lT[] is just a simple delay
			oQR = oRR - (rQ[(cQ+1)-((cQ+1 > dpQ)?dpQ+1:0)]*0.5f);
			oRR = oSR - (rR[(cR+1)-((cR+1 > dpR)?dpR+1:0)]*0.5f);
			oSR = oTR - (rS[(cS+1)-((cS+1 > dpS)?dpS+1:0)]*0.5f);
			oTR = inputSampleR; //SIMDable, but rT[] is just a simple delay
			lQ[cQ] = oQL; lR[cR] = oRL; lS[cS] = oSL; lT[cT] = oTL;
			rQ[cQ] = oQR; rR[cR] = oRR; rS[cS] = oSR; rT[cT] = oTR;
			cQ++; cR++; cS++; cT++;
			if (cQ > dpQ) cQ = 0;
			if (cR > dpR) cR = 0;
			if (cS > dpS) cS = 0;
			if (cT > dpT) cT = 0;
			oQL = fma(oQL,0.5f,lQ[cQ]);
			oRL = fma(oRL,0.5f,lR[cR]);
			oSL = fma(oSL,0.5f,lS[cS]);
			oTL = fma(oTL,0.0f,lT[cT]); //not allpass but can use vector fma()
			oQR = fma(oQR,0.5f,rQ[cQ]);
			oRR = fma(oRR,0.5f,rR[cR]);
			oSR = fma(oSR,0.5f,rS[cS]);
			oTR = fma(oTR,0.0f,rT[cT]); //not allpass but can use vector fma()
			
			if (bypassPre) {oQL = inputSampleL; oQR = inputSampleR;} //plugin only
			
			lA[cA] = fma(fAL,damping,oQL);
			lB[cB] = fma(fBL,damping,oQL);
			lC[cC] = fma(fCL,damping,oQL);
			lD[cD] = fma(fDL,damping,oQL);
			rA[cA] = fma(fAR,damping,oQR);
			rB[cB] = fma(fBR,damping,oQR);
			rC[cC] = fma(fCR,damping,oQR);
			rD[cD] = fma(fDR,damping,oQR);
			cA++; cB++; cC++; cD++;
			if (cA > d4A) cA = 0;
			if (cB > d4B) cB = 0;
			if (cC > d4C) cC = 0;
			if (cD > d4D) cD = 0;
			hAL = lA[cA]; hBL = lB[cB]; hCL = lC[cC]; hDL = lD[cD];
			hAR = rA[cA]; hBR = rB[cB]; hCR = rC[cC]; hDR = rD[cD];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lE[cE] = fma(hAL,2.0f,hEL);
			lF[cF] = fma(hBL,2.0f,hEL);
			lG[cG] = fma(hCL,2.0f,hEL);
			lH[cH] = fma(hDL,2.0f,hEL);
			rE[cE] = fma(hAR,2.0f,hER);
			rF[cF] = fma(hBR,2.0f,hER);
			rG[cG] = fma(hCR,2.0f,hER);
			rH[cH] = fma(hDR,2.0f,hER);
			cE++; cF++; cG++; cH++;
			if (cE > d4E) cE = 0;
			if (cF > d4F) cF = 0;
			if (cG > d4G) cG = 0;
			if (cH > d4H) cH = 0;
			hAL = lE[cE]; hBL = lF[cF]; hCL = lG[cG]; hDL = lH[cH];
			hAR = rE[cE]; hBR = rF[cF]; hCR = rG[cG]; hDR = rH[cH];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lI[cI] = fma(hAL,2.0f,hEL);
			lJ[cJ] = fma(hBL,2.0f,hEL);
			lK[cK] = fma(hCL,2.0f,hEL);
			lL[cL] = fma(hDL,2.0f,hEL);
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
			hAR = rI[cI]; hBR = rJ[cJ]; hCR = rK[cK]; hDR = rL[cL];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lM[cM] = fma(hAL,2.0f,hEL);
			lN[cN] = fma(hBL,2.0f,hEL);
			lO[cO] = fma(hCL,2.0f,hEL);
			lP[cP] = fma(hDL,2.0f,hEL);
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
			hAR = rM[cM]; hBR = rN[cN]; hCR = rO[cO]; hDR = rP[cP];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			fAL = fma(hAL,2.0f,hEL);				
			fBL = fma(hBL,2.0f,hEL);
			fCL = fma(hCL,2.0f,hEL);
			fDL = fma(hDL,2.0f,hEL);
			fAR = fma(hAR,2.0f,hER);				
			fBR = fma(hBR,2.0f,hER);
			fCR = fma(hCR,2.0f,hER);
			fDR = fma(hDR,2.0f,hER);
			switch (unSolid)
			{
				case 0:
					fhAL *= 0.25f; fAL -= fhAL*0.125f; fhAL += fAL;
					fhAR *= 0.25f; fAR -= fhAR*0.125f; fhAR += fAR;
				case 1:
					fhBL *= 0.25f; fBL -= fhBL*0.125f; fhBL += fBL;
					fhBR *= 0.25f; fBR -= fhBR*0.125f; fhBR += fBR;
				case 2:
					fhCL *= 0.25f; fCL -= fhCL*0.125f; fhCL += fCL;
					fhCR *= 0.25f; fCR -= fhCR*0.125f; fhCR += fCR;
				case 3:
					fhDL *= 0.25f; fDL -= fhDL*0.125f; fhDL += fDL;
					fhDR *= 0.25f; fDR -= fhDR*0.125f; fhDR += fDR;
				case 4:
					break;
			}
			switch (unReflect)
			{
				case 0:
					fDL = (fDL+flDL)*0.5f; flDL = fDL;
					fDR = (fDR+flDR)*0.5f; flDR = fDR;
				case 1:
					fCL = (fCL+flCL)*0.5f; flCL = fCL;
					fCR = (fCR+flCR)*0.5f; flCR = fCR;
				case 2:
					fBL = (fBL+flBL)*0.5f; flBL = fBL;
					fBR = (fBR+flBR)*0.5f; flBR = fBR;
				case 3:
					fAL = (fAL+flAL)*0.5f; flAL = fAL;
					fAR = (fAR+flAR)*0.5f; flAR = fAR;
				case 4:
					break;
			}
			inputSampleL = (hAL + hBL + hCL + hDL)*0.0625f;
			inputSampleR = (hAR + hBR + hCR + hDR)*0.0625f;
			//end of just the reverb node part
			
			if (bypassVerb) {inputSampleL = oQL; inputSampleR = oQR;} //plugin only
			//dry/wet is not part of the game engine reverb node
			inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0f-wet));
			inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0f-wet));
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
				case  0: inputSampleL += lsz; inputSampleL *= 0.5f; lsz = inputSampleL;
				case  1: inputSampleL += lsy; inputSampleL *= 0.5f; lsy = inputSampleL;
				case  2: inputSampleL += lsx; inputSampleL *= 0.5f; lsx = inputSampleL;
				case  3: inputSampleL += lsw; inputSampleL *= 0.5f; lsw = inputSampleL;
				case  4: inputSampleL += lsv; inputSampleL *= 0.5f; lsv = inputSampleL;
				case  5: inputSampleL += lsu; inputSampleL *= 0.5f; lsu = inputSampleL;
				case  6: inputSampleL += lst; inputSampleL *= 0.5f; lst = inputSampleL;
				case  7: inputSampleL += lss; inputSampleL *= 0.5f; lss = inputSampleL;
				case  8: inputSampleL += lsr; inputSampleL *= 0.5f; lsr = inputSampleL;
				case  9: inputSampleL += lsq; inputSampleL *= 0.5f; lsq = inputSampleL;
				case 10: inputSampleL += lsp; inputSampleL *= 0.5f; lsp = inputSampleL;
				case 11: inputSampleL += lso; inputSampleL *= 0.5f; lso = inputSampleL;
				case 12: inputSampleL += lsn; inputSampleL *= 0.5f; lsn = inputSampleL;
				case 13: inputSampleL += lsm; inputSampleL *= 0.5f; lsm = inputSampleL;
				case 14: inputSampleL += lsl; inputSampleL *= 0.5f; lsl = inputSampleL;
				case 15: inputSampleL += lsk; inputSampleL *= 0.5f; lsk = inputSampleL;
				case 16: inputSampleL += lsj; inputSampleL *= 0.5f; lsj = inputSampleL;
				case 17: inputSampleL += lsi; inputSampleL *= 0.5f; lsi = inputSampleL;
				case 18: inputSampleL += lsh; inputSampleL *= 0.5f; lsh = inputSampleL;
				case 19: inputSampleL += lsg; inputSampleL *= 0.5f; lsg = inputSampleL;
				case 20: inputSampleL += lsf; inputSampleL *= 0.5f; lsf = inputSampleL;
				case 21: inputSampleL += lse; inputSampleL *= 0.5f; lse = inputSampleL;
				case 22: inputSampleL += lsd; inputSampleL *= 0.5f; lsd = inputSampleL;
				case 23: inputSampleL += lsc; inputSampleL *= 0.5f; lsc = inputSampleL;
				case 24: inputSampleL += lsb; inputSampleL *= 0.5f; lsb = inputSampleL;
				case 25: inputSampleL += lsa; inputSampleL *= 0.5f; lsa = inputSampleL;
				case 26: inputSampleL += lsZ; inputSampleL *= 0.5f; lsZ = inputSampleL;
				case 27: inputSampleL += lsY; inputSampleL *= 0.5f; lsY = inputSampleL;
				case 28: inputSampleL += lsX; inputSampleL *= 0.5f; lsX = inputSampleL;
				case 29: inputSampleL += lsW; inputSampleL *= 0.5f; lsW = inputSampleL;
				case 30: inputSampleL += lsV; inputSampleL *= 0.5f; lsV = inputSampleL;
				case 31: inputSampleL += lsU; inputSampleL *= 0.5f; lsU = inputSampleL;
				case 32: inputSampleL += lsT; inputSampleL *= 0.5f; lsT = inputSampleL;
				case 33: inputSampleL += lsS; inputSampleL *= 0.5f; lsS = inputSampleL;
				case 34: inputSampleL += lsR; inputSampleL *= 0.5f; lsR = inputSampleL;
				case 35: inputSampleL += lsQ; inputSampleL *= 0.5f; lsQ = inputSampleL;
				case 36: inputSampleL += lsP; inputSampleL *= 0.5f; lsP = inputSampleL;
				case 37: inputSampleL += lsO; inputSampleL *= 0.5f; lsO = inputSampleL;
				case 38: inputSampleL += lsN; inputSampleL *= 0.5f; lsN = inputSampleL;
				case 39: inputSampleL += lsM; inputSampleL *= 0.5f; lsM = inputSampleL;
				case 40: inputSampleL += lsL; inputSampleL *= 0.5f; lsL = inputSampleL;
				case 41: inputSampleL += lsK; inputSampleL *= 0.5f; lsK = inputSampleL;
				case 42: inputSampleL += lsJ; inputSampleL *= 0.5f; lsJ = inputSampleL;
				case 43: inputSampleL += lsI; inputSampleL *= 0.5f; lsI = inputSampleL;
				case 44: inputSampleL += lsH; inputSampleL *= 0.5f; lsH = inputSampleL;
				case 45: inputSampleL += lsG; inputSampleL *= 0.5f; lsG = inputSampleL;
				case 46: inputSampleL += lsF; inputSampleL *= 0.5f; lsF = inputSampleL;
				case 47: inputSampleL += lsE; inputSampleL *= 0.5f; lsE = inputSampleL;
				case 48: inputSampleL += lsD; inputSampleL *= 0.5f; lsD = inputSampleL;
				case 49: inputSampleL += lsC; inputSampleL *= 0.5f; lsC = inputSampleL;
				case 50: inputSampleL += lsB; inputSampleL *= 0.5f; lsB = inputSampleL;
				case 51: inputSampleL += lsA; inputSampleL *= 0.5f; lsA = inputSampleL;
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
				case  0: inputSampleR += rsz; inputSampleR *= 0.5f; rsz = inputSampleR;
				case  1: inputSampleR += rsy; inputSampleR *= 0.5f; rsy = inputSampleR;
				case  2: inputSampleR += rsx; inputSampleR *= 0.5f; rsx = inputSampleR;
				case  3: inputSampleR += rsw; inputSampleR *= 0.5f; rsw = inputSampleR;
				case  4: inputSampleR += rsv; inputSampleR *= 0.5f; rsv = inputSampleR;
				case  5: inputSampleR += rsu; inputSampleR *= 0.5f; rsu = inputSampleR;
				case  6: inputSampleR += rst; inputSampleR *= 0.5f; rst = inputSampleR;
				case  7: inputSampleR += rss; inputSampleR *= 0.5f; rss = inputSampleR;
				case  8: inputSampleR += rsr; inputSampleR *= 0.5f; rsr = inputSampleR;
				case  9: inputSampleR += rsq; inputSampleR *= 0.5f; rsq = inputSampleR;
				case 10: inputSampleR += rsp; inputSampleR *= 0.5f; rsp = inputSampleR;
				case 11: inputSampleR += rso; inputSampleR *= 0.5f; rso = inputSampleR;
				case 12: inputSampleR += rsn; inputSampleR *= 0.5f; rsn = inputSampleR;
				case 13: inputSampleR += rsm; inputSampleR *= 0.5f; rsm = inputSampleR;
				case 14: inputSampleR += rsl; inputSampleR *= 0.5f; rsl = inputSampleR;
				case 15: inputSampleR += rsk; inputSampleR *= 0.5f; rsk = inputSampleR;
				case 16: inputSampleR += rsj; inputSampleR *= 0.5f; rsj = inputSampleR;
				case 17: inputSampleR += rsi; inputSampleR *= 0.5f; rsi = inputSampleR;
				case 18: inputSampleR += rsh; inputSampleR *= 0.5f; rsh = inputSampleR;
				case 19: inputSampleR += rsg; inputSampleR *= 0.5f; rsg = inputSampleR;
				case 20: inputSampleR += rsf; inputSampleR *= 0.5f; rsf = inputSampleR;
				case 21: inputSampleR += rse; inputSampleR *= 0.5f; rse = inputSampleR;
				case 22: inputSampleR += rsd; inputSampleR *= 0.5f; rsd = inputSampleR;
				case 23: inputSampleR += rsc; inputSampleR *= 0.5f; rsc = inputSampleR;
				case 24: inputSampleR += rsb; inputSampleR *= 0.5f; rsb = inputSampleR;
				case 25: inputSampleR += rsa; inputSampleR *= 0.5f; rsa = inputSampleR;
				case 26: inputSampleR += rsZ; inputSampleR *= 0.5f; rsZ = inputSampleR;
				case 27: inputSampleR += rsY; inputSampleR *= 0.5f; rsY = inputSampleR;
				case 28: inputSampleR += rsX; inputSampleR *= 0.5f; rsX = inputSampleR;
				case 29: inputSampleR += rsW; inputSampleR *= 0.5f; rsW = inputSampleR;
				case 30: inputSampleR += rsV; inputSampleR *= 0.5f; rsV = inputSampleR;
				case 31: inputSampleR += rsU; inputSampleR *= 0.5f; rsU = inputSampleR;
				case 32: inputSampleR += rsT; inputSampleR *= 0.5f; rsT = inputSampleR;
				case 33: inputSampleR += rsS; inputSampleR *= 0.5f; rsS = inputSampleR;
				case 34: inputSampleR += rsR; inputSampleR *= 0.5f; rsR = inputSampleR;
				case 35: inputSampleR += rsQ; inputSampleR *= 0.5f; rsQ = inputSampleR;
				case 36: inputSampleR += rsP; inputSampleR *= 0.5f; rsP = inputSampleR;
				case 37: inputSampleR += rsO; inputSampleR *= 0.5f; rsO = inputSampleR;
				case 38: inputSampleR += rsN; inputSampleR *= 0.5f; rsN = inputSampleR;
				case 39: inputSampleR += rsM; inputSampleR *= 0.5f; rsM = inputSampleR;
				case 40: inputSampleR += rsL; inputSampleR *= 0.5f; rsL = inputSampleR;
				case 41: inputSampleR += rsK; inputSampleR *= 0.5f; rsK = inputSampleR;
				case 42: inputSampleR += rsJ; inputSampleR *= 0.5f; rsJ = inputSampleR;
				case 43: inputSampleR += rsI; inputSampleR *= 0.5f; rsI = inputSampleR;
				case 44: inputSampleR += rsH; inputSampleR *= 0.5f; rsH = inputSampleR;
				case 45: inputSampleR += rsG; inputSampleR *= 0.5f; rsG = inputSampleR;
				case 46: inputSampleR += rsF; inputSampleR *= 0.5f; rsF = inputSampleR;
				case 47: inputSampleR += rsE; inputSampleR *= 0.5f; rsE = inputSampleR;
				case 48: inputSampleR += rsD; inputSampleR *= 0.5f; rsD = inputSampleR;
				case 49: inputSampleR += rsC; inputSampleR *= 0.5f; rsC = inputSampleR;
				case 50: inputSampleR += rsB; inputSampleR *= 0.5f; rsB = inputSampleR;
				case 51: inputSampleR += rsA; inputSampleR *= 0.5f; rsA = inputSampleR;
				case 52: break;
			}
			//end distance filter R
			
			pointAL = pointBL;
			pointBL = inputSampleL;
			pointAR = pointBR;
			pointBR = inputSampleR;
			pointCycle = 0.0f; //interpolation point values
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
