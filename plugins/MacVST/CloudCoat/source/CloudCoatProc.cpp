/* ========================================
 *  CloudCoat - CloudCoat.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __CloudCoat_H
#include "CloudCoat.h"
#endif

void CloudCoat::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	int cloudcoat = (int)(A*16.999);
	
	if (cloudcoat != prevcloudcoat) {
		for(int count = 0; count < kshortA+2; count++) {aAL[count] = 0.0; aAR[count] = 0.0;}
		for(int count = 0; count < kshortB+2; count++) {aBL[count] = 0.0; aBR[count] = 0.0;}
		for(int count = 0; count < kshortC+2; count++) {aCL[count] = 0.0; aCR[count] = 0.0;}
		for(int count = 0; count < kshortD+2; count++) {aDL[count] = 0.0; aDR[count] = 0.0;}
		for(int count = 0; count < kshortE+2; count++) {aEL[count] = 0.0; aER[count] = 0.0;}
		for(int count = 0; count < kshortF+2; count++) {aFL[count] = 0.0; aFR[count] = 0.0;}
		for(int count = 0; count < kshortG+2; count++) {aGL[count] = 0.0; aGR[count] = 0.0;}
		for(int count = 0; count < kshortH+2; count++) {aHL[count] = 0.0; aHR[count] = 0.0;}
		for(int count = 0; count < kshortI+2; count++) {aIL[count] = 0.0; aIR[count] = 0.0;}
		for(int count = 0; count < kshortJ+2; count++) {aJL[count] = 0.0; aJR[count] = 0.0;}
		for(int count = 0; count < kshortK+2; count++) {aKL[count] = 0.0; aKR[count] = 0.0;}
		for(int count = 0; count < kshortL+2; count++) {aLL[count] = 0.0; aLR[count] = 0.0;}
		for(int count = 0; count < kshortM+2; count++) {aML[count] = 0.0; aMR[count] = 0.0;}
		for(int count = 0; count < kshortN+2; count++) {aNL[count] = 0.0; aNR[count] = 0.0;}
		for(int count = 0; count < kshortO+2; count++) {aOL[count] = 0.0; aOR[count] = 0.0;}
		for(int count = 0; count < kshortP+2; count++) {aPL[count] = 0.0; aPR[count] = 0.0;}		
		countAL = 1;
		countBL = 1;
		countCL = 1;
		countDL = 1;	
		countEL = 1;
		countFL = 1;
		countGL = 1;
		countHL = 1;
		countIL = 1;
		countJL = 1;
		countKL = 1;
		countLL = 1;
		countML = 1;
		countNL = 1;
		countOL = 1;
		countPL = 1;
		
		countAR = 1;
		countBR = 1;
		countCR = 1;
		countDR = 1;	
		countER = 1;
		countFR = 1;
		countGR = 1;
		countHR = 1;
		countIR = 1;
		countJR = 1;
		countKR = 1;
		countLR = 1;
		countMR = 1;
		countNR = 1;
		countOR = 1;
		countPR = 1;
		switch (cloudcoat)
		{
			case 0:
				shortA = 65; shortB = 124; shortC = 83; shortD = 180; shortE = 200; shortF = 291; shortG = 108; shortH = 189; shortI = 73; shortJ = 410; shortK = 479; shortL = 310; shortM = 11; shortN = 928; shortO = 23; shortP = 654; break; //5 to 51 ms, 96 seat room. Scarcity, 1 in 125324
				//Short96
			case 1:
				shortA = 114; shortB = 205; shortC = 498; shortD = 195; shortE = 205; shortF = 318; shortG = 143; shortH = 254; shortI = 64; shortJ = 721; shortK = 512; shortL = 324; shortM = 11; shortN = 782; shortO = 26; shortP = 394; break; //7 to 52 ms, 107 seat club. Scarcity, 1 in 65537
				//Short107
			case 2:
				shortA = 118; shortB = 272; shortC = 292; shortD = 145; shortE = 200; shortF = 241; shortG = 204; shortH = 504; shortI = 50; shortJ = 678; shortK = 424; shortL = 412; shortM = 11; shortN = 1124; shortO = 47; shortP = 766; break; //8 to 58 ms, 135 seat club. Scarcity, 1 in 196272
				//Short135
			case 3:
				shortA = 19; shortB = 474; shortC = 301; shortD = 275; shortE = 260; shortF = 321; shortG = 371; shortH = 571; shortI = 50; shortJ = 410; shortK = 697; shortL = 414; shortM = 11; shortN = 986; shortO = 47; shortP = 522; break; //7 to 61 ms, 143 seat club. Scarcity, 1 in 165738
				//Short143
			case 4:
				shortA = 112; shortB = 387; shortC = 452; shortD = 289; shortE = 173; shortF = 476; shortG = 321; shortH = 593; shortI = 73; shortJ = 343; shortK = 829; shortL = 91; shortM = 11; shortN = 1055; shortO = 43; shortP = 862; break; //8 to 66 ms, 166 seat club. Scarcity, 1 in 158437
				//Short166
			case 5:
				shortA = 60; shortB = 368; shortC = 295; shortD = 272; shortE = 210; shortF = 284; shortG = 326; shortH = 830; shortI = 125; shortJ = 236; shortK = 737; shortL = 486; shortM = 11; shortN = 1178; shortO = 75; shortP = 902; break; //9 to 70 ms, 189 seat club. Scarcity, 1 in 94790
				//Short189
			case 6:
				shortA = 73; shortB = 311; shortC = 472; shortD = 251; shortE = 134; shortF = 509; shortG = 393; shortH = 591; shortI = 124; shortJ = 1070; shortK = 340; shortL = 525; shortM = 11; shortN = 1367; shortO = 75; shortP = 816; break; //7 to 79 ms, 225 seat club. Scarcity, 1 in 257803
				//Short225
			case 7:
				shortA = 159; shortB = 518; shortC = 514; shortD = 165; shortE = 275; shortF = 494; shortG = 296; shortH = 667; shortI = 75; shortJ = 1101; shortK = 116; shortL = 414; shortM = 11; shortN = 1261; shortO = 79; shortP = 998; break; //11 to 80 ms, 252 seat club. Scarcity, 1 in 175192
				//Short252
			case 8:
				shortA = 41; shortB = 741; shortC = 274; shortD = 59; shortE = 306; shortF = 332; shortG = 291; shortH = 767; shortI = 42; shortJ = 881; shortK = 959; shortL = 422; shortM = 11; shortN = 1237; shortO = 45; shortP = 958; break; //8 to 83 ms, 255 seat club. Scarcity, 1 in 185708
				//Short255
			case 9:
				shortA = 251; shortB = 437; shortC = 783; shortD = 189; shortE = 130; shortF = 272; shortG = 244; shortH = 761; shortI = 128; shortJ = 1190; shortK = 320; shortL = 491; shortM = 11; shortN = 1409; shortO = 58; shortP = 455; break; //10 to 93 ms, 323 seat club. Scarcity, 1 in 334044
				//Short323
			case 10:
				shortA = 316; shortB = 510; shortC = 1087; shortD = 349; shortE = 359; shortF = 74; shortG = 79; shortH = 1269; shortI = 34; shortJ = 693; shortK = 749; shortL = 511; shortM = 11; shortN = 1751; shortO = 93; shortP = 403; break; //9 to 110 ms, 427 seat theater. Scarcity, 1 in 200715
				//Short427
			case 11:
				shortA = 254; shortB = 651; shortC = 845; shortD = 316; shortE = 373; shortF = 267; shortG = 182; shortH = 857; shortI = 215; shortJ = 1535; shortK = 1127; shortL = 315; shortM = 11; shortN = 1649; shortO = 97; shortP = 829; break; //15 to 110 ms, 470 seat theater. Scarcity, 1 in 362673
				//Short470
			case 12:
				shortA = 113; shortB = 101; shortC = 673; shortD = 357; shortE = 340; shortF = 229; shortG = 278; shortH = 1008; shortI = 265; shortJ = 1890; shortK = 155; shortL = 267; shortM = 11; shortN = 2233; shortO = 116; shortP = 600; break; //11 to 131 ms, 606 seat theater. Scarcity, 1 in 238058
				//Short606
			case 13:
				shortA = 218; shortB = 1058; shortC = 862; shortD = 505; shortE = 297; shortF = 580; shortG = 532; shortH = 1387; shortI = 120; shortJ = 576; shortK = 1409; shortL = 473; shortM = 11; shortN = 1991; shortO = 76; shortP = 685; break; //14 to 132 ms, 643 seat theater. Scarcity, 1 in 193432
				//Short643
			case 14:
				shortA = 78; shortB = 760; shortC = 982; shortD = 528; shortE = 445; shortF = 1128; shortG = 130; shortH = 708; shortI = 22; shortJ = 2144; shortK = 354; shortL = 1169; shortM = 11; shortN = 2782; shortO = 58; shortP = 1515; break; //5 to 159 ms, 809 seat hall. Scarcity, 1 in 212274
				//Short809
			case 15:
				shortA = 330; shortB = 107; shortC = 1110; shortD = 371; shortE = 620; shortF = 143; shortG = 1014; shortH = 1763; shortI = 184; shortJ = 2068; shortK = 1406; shortL = 595; shortM = 11; shortN = 2639; shortO = 33; shortP = 1594; break; //10 to 171 ms, 984 seat hall. Scarcity, 1 in 226499
				//Short984
			case 16:
			default:
				shortA = 336; shortB = 1660; shortC = 386; shortD = 623; shortE = 693; shortF = 1079; shortG = 891; shortH = 1574; shortI = 24; shortJ = 2641; shortK = 1239; shortL = 775; shortM = 11; shortN = 3104; shortO = 55; shortP = 2366; break; //24 to 203 ms, 1541 seat hall. Scarcity, 1 in 275025
				//Short1541
		}
		prevcloudcoat = cloudcoat;
	}
	
	double gaintrim = 1.0-pow(1.0-B,2);
	double sustain = gaintrim*0.086;
	gaintrim = 1.1-gaintrim;
	double wet = C*2.0;
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL /= gaintrim;
		inputSampleR /= gaintrim;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a reverb sample			
			double outAL = inputSampleL - (aAL[(countAL+1)-((countAL+1 > shortA)?shortA+1:0)]*0.618033988749894848204586);
			double outBL = inputSampleL - (aBL[(countBL+1)-((countBL+1 > shortB)?shortB+1:0)]*0.618033988749894848204586);
			double outCL = inputSampleL - (aCL[(countCL+1)-((countCL+1 > shortC)?shortC+1:0)]*0.618033988749894848204586);
			double outDL = inputSampleL - (aDL[(countDL+1)-((countDL+1 > shortD)?shortD+1:0)]*0.618033988749894848204586);
			double outDR = inputSampleR - (aDR[(countDR+1)-((countDR+1 > shortD)?shortD+1:0)]*0.618033988749894848204586);
			double outHR = inputSampleR - (aHR[(countHR+1)-((countHR+1 > shortH)?shortH+1:0)]*0.618033988749894848204586);
			double outLR = inputSampleR - (aLR[(countLR+1)-((countLR+1 > shortL)?shortL+1:0)]*0.618033988749894848204586);
			double outPR = inputSampleR - (aPR[(countPR+1)-((countPR+1 > shortP)?shortP+1:0)]*0.618033988749894848204586);
			
			outAL += (feedbackAL * (sustain*fabs(sin(feedbackBL))));
			outBL += (feedbackBL * (sustain*fabs(sin(feedbackCL))));
			outCL += (feedbackCL * (sustain*fabs(sin(feedbackDL))));
			outDL += (feedbackDL * (sustain*fabs(sin(feedbackAL))));
			
			outDR += (feedbackDR * (sustain*fabs(sin(feedbackHR))));
			outHR += (feedbackHR * (sustain*fabs(sin(feedbackLR))));
			outLR += (feedbackLR * (sustain*fabs(sin(feedbackPR))));
			outPR += (feedbackPR * (sustain*fabs(sin(feedbackDR))));
			
			aAL[countAL] = outAL; outAL *= 0.618033988749894848204586;
			aBL[countBL] = outBL; outBL *= 0.618033988749894848204586;
			aCL[countCL] = outCL; outCL *= 0.618033988749894848204586;
			aDL[countDL] = outDL; outDL *= 0.618033988749894848204586;
			aDR[countDR] = outDR; outDR *= 0.618033988749894848204586;
			aHR[countHR] = outHR; outHR *= 0.618033988749894848204586;
			aLR[countLR] = outLR; outLR *= 0.618033988749894848204586;
			aPR[countPR] = outPR; outPR *= 0.618033988749894848204586;
			
			countAL++; if (countAL < 0 || countAL > shortA) countAL = 0;
			countBL++; if (countBL < 0 || countBL > shortB) countBL = 0;
			countCL++; if (countCL < 0 || countCL > shortC) countCL = 0;
			countDL++; if (countDL < 0 || countDL > shortD) countDL = 0;
			countDR++; if (countDR < 0 || countDR > shortD) countDR = 0;
			countHR++; if (countHR < 0 || countHR > shortH) countHR = 0;
			countLR++; if (countLR < 0 || countLR > shortL) countLR = 0;
			countPR++; if (countPR < 0 || countPR > shortP) countPR = 0;
			
			outAL += aAL[countAL-((countAL > shortA)?shortA+1:0)];
			outBL += aBL[countBL-((countBL > shortB)?shortB+1:0)];
			outCL += aCL[countCL-((countCL > shortC)?shortC+1:0)];
			outDL += aDL[countDL-((countDL > shortD)?shortD+1:0)];
			outDR += aDR[countDR-((countDR > shortD)?shortD+1:0)];
			outHR += aHR[countHR-((countHR > shortH)?shortH+1:0)];
			outLR += aLR[countLR-((countLR > shortL)?shortL+1:0)];
			outPR += aPR[countPR-((countPR > shortP)?shortP+1:0)];
			
			double outEL = (outAL - (outBL + outCL + outDL)) - (aEL[(countEL+1)-((countEL+1 > shortE)?shortE+1:0)]*0.618033988749894848204586);
			double outFL = (outBL - (outAL + outCL + outDL)) - (aFL[(countFL+1)-((countFL+1 > shortF)?shortF+1:0)]*0.618033988749894848204586);
			double outGL = (outCL - (outAL + outBL + outDL)) - (aGL[(countGL+1)-((countGL+1 > shortG)?shortG+1:0)]*0.618033988749894848204586);
			double outHL = (outDL - (outAL + outBL + outCL)) - (aHL[(countHL+1)-((countHL+1 > shortH)?shortH+1:0)]*0.618033988749894848204586);
			double outCR = (outDR - (outHR + outLR + outPR)) - (aCR[(countCR+1)-((countCR+1 > shortC)?shortC+1:0)]*0.618033988749894848204586);
			double outGR = (outHR - (outDR + outLR + outPR)) - (aGR[(countGR+1)-((countGR+1 > shortG)?shortG+1:0)]*0.618033988749894848204586);
			double outKR = (outLR - (outDR + outHR + outPR)) - (aKR[(countKR+1)-((countKR+1 > shortK)?shortK+1:0)]*0.618033988749894848204586);
			double outOR = (outPR - (outDR + outHR + outLR)) - (aOR[(countOR+1)-((countOR+1 > shortO)?shortO+1:0)]*0.618033988749894848204586);
			
			aEL[countEL] = outEL; outEL *= 0.618033988749894848204586;
			aFL[countFL] = outFL; outFL *= 0.618033988749894848204586;
			aGL[countGL] = outGL; outGL *= 0.618033988749894848204586;
			aHL[countHL] = outHL; outHL *= 0.618033988749894848204586;
			aCR[countCR] = outCR; outCR *= 0.618033988749894848204586;
			aGR[countGR] = outGR; outGR *= 0.618033988749894848204586;
			aKR[countKR] = outKR; outKR *= 0.618033988749894848204586;
			aOR[countOR] = outOR; outOR *= 0.618033988749894848204586;
			
			countEL++; if (countEL < 0 || countEL > shortE) countEL = 0;
			countFL++; if (countFL < 0 || countFL > shortF) countFL = 0;
			countGL++; if (countGL < 0 || countGL > shortG) countGL = 0;
			countHL++; if (countHL < 0 || countHL > shortH) countHL = 0;
			countCR++; if (countCR < 0 || countCR > shortC) countCR = 0;
			countGR++; if (countGR < 0 || countGR > shortG) countGR = 0;
			countKR++; if (countKR < 0 || countKR > shortK) countKR = 0;
			countOR++; if (countOR < 0 || countOR > shortO) countOR = 0;
			
			outEL += aEL[countEL-((countEL > shortE)?shortE+1:0)];
			outFL += aFL[countFL-((countFL > shortF)?shortF+1:0)];
			outGL += aGL[countGL-((countGL > shortG)?shortG+1:0)];
			outHL += aHL[countHL-((countHL > shortH)?shortH+1:0)];
			outCR += aCR[countCR-((countCR > shortC)?shortC+1:0)];
			outGR += aGR[countGR-((countGR > shortG)?shortG+1:0)];
			outKR += aKR[countKR-((countKR > shortK)?shortK+1:0)];
			outOR += aOR[countOR-((countOR > shortO)?shortO+1:0)];
			
			double outIL = (outEL - (outFL + outGL + outHL)) - (aIL[(countIL+1)-((countIL+1 > shortI)?shortI+1:0)]*0.618033988749894848204586);
			double outJL = (outFL - (outEL + outGL + outHL)) - (aJL[(countJL+1)-((countJL+1 > shortJ)?shortJ+1:0)]*0.618033988749894848204586);
			double outKL = (outGL - (outEL + outFL + outHL)) - (aKL[(countKL+1)-((countKL+1 > shortK)?shortK+1:0)]*0.618033988749894848204586);
			double outLL = (outHL - (outEL + outFL + outGL)) - (aLL[(countLL+1)-((countLL+1 > shortL)?shortL+1:0)]*0.618033988749894848204586);
			double outBR = (outCR - (outGR + outKR + outOR)) - (aBR[(countBR+1)-((countBR+1 > shortB)?shortB+1:0)]*0.618033988749894848204586);
			double outFR = (outGR - (outCR + outKR + outOR)) - (aFR[(countFR+1)-((countFR+1 > shortF)?shortF+1:0)]*0.618033988749894848204586);
			double outJR = (outKR - (outCR + outGR + outOR)) - (aJR[(countJR+1)-((countJR+1 > shortJ)?shortJ+1:0)]*0.618033988749894848204586);
			double outNR = (outOR - (outCR + outGR + outKR)) - (aNR[(countNR+1)-((countNR+1 > shortN)?shortN+1:0)]*0.618033988749894848204586);
			
			aIL[countIL] = outIL; outIL *= 0.618033988749894848204586;
			aJL[countJL] = outJL; outJL *= 0.618033988749894848204586;
			aKL[countKL] = outKL; outKL *= 0.618033988749894848204586;
			aLL[countLL] = outLL; outLL *= 0.618033988749894848204586;
			aBR[countBR] = outBR; outBR *= 0.618033988749894848204586;
			aFR[countFR] = outFR; outFR *= 0.618033988749894848204586;
			aJR[countJR] = outJR; outJR *= 0.618033988749894848204586;
			aNR[countNR] = outNR; outNR *= 0.618033988749894848204586;
			
			countIL++; if (countIL < 0 || countIL > shortI) countIL = 0;
			countJL++; if (countJL < 0 || countJL > shortJ) countJL = 0;
			countKL++; if (countKL < 0 || countKL > shortK) countKL = 0;
			countLL++; if (countLL < 0 || countLL > shortL) countLL = 0;
			countBR++; if (countBR < 0 || countBR > shortB) countBR = 0;
			countFR++; if (countFR < 0 || countFR > shortF) countFR = 0;
			countJR++; if (countJR < 0 || countJR > shortJ) countJR = 0;
			countNR++; if (countNR < 0 || countNR > shortN) countNR = 0;
			
			outIL += aIL[countIL-((countIL > shortI)?shortI+1:0)];
			outJL += aJL[countJL-((countJL > shortJ)?shortJ+1:0)];
			outKL += aKL[countKL-((countKL > shortK)?shortK+1:0)];
			outLL += aLL[countLL-((countLL > shortL)?shortL+1:0)];
			outBR += aBR[countBR-((countBR > shortB)?shortB+1:0)];
			outFR += aFR[countFR-((countFR > shortF)?shortF+1:0)];
			outJR += aJR[countJR-((countJR > shortJ)?shortJ+1:0)];
			outNR += aNR[countNR-((countNR > shortN)?shortN+1:0)];
			
			double outML = (outIL - (outJL + outKL + outLL)) - (aML[(countML+1)-((countML+1 > shortM)?shortM+1:0)]*0.618033988749894848204586);
			double outNL = (outJL - (outIL + outKL + outLL)) - (aNL[(countNL+1)-((countNL+1 > shortN)?shortN+1:0)]*0.618033988749894848204586);
			double outOL = (outKL - (outIL + outJL + outLL)) - (aOL[(countOL+1)-((countOL+1 > shortO)?shortO+1:0)]*0.618033988749894848204586);
			double outPL = (outLL - (outIL + outJL + outKL)) - (aPL[(countPL+1)-((countPL+1 > shortP)?shortP+1:0)]*0.618033988749894848204586);
			double outAR = (outBR - (outFR + outJR + outNR)) - (aAR[(countAR+1)-((countAR+1 > shortA)?shortA+1:0)]*0.618033988749894848204586);
			double outER = (outFR - (outBR + outJR + outNR)) - (aER[(countER+1)-((countER+1 > shortE)?shortE+1:0)]*0.618033988749894848204586);
			double outIR = (outJR - (outBR + outFR + outNR)) - (aIR[(countIR+1)-((countIR+1 > shortI)?shortI+1:0)]*0.618033988749894848204586);
			double outMR = (outNR - (outBR + outFR + outJR)) - (aMR[(countMR+1)-((countMR+1 > shortM)?shortM+1:0)]*0.618033988749894848204586);
			
			aML[countML] = outML; outML *= 0.618033988749894848204586;
			aNL[countNL] = outNL; outNL *= 0.618033988749894848204586;
			aOL[countOL] = outOL; outOL *= 0.618033988749894848204586;
			aPL[countPL] = outPL; outPL *= 0.618033988749894848204586;
			aAR[countAR] = outAR; outAR *= 0.618033988749894848204586;
			aER[countER] = outER; outER *= 0.618033988749894848204586;
			aIR[countIR] = outIR; outIR *= 0.618033988749894848204586;
			aMR[countMR] = outMR; outMR *= 0.618033988749894848204586;
			
			countML++; if (countML < 0 || countML > shortM) countML = 0;
			countNL++; if (countNL < 0 || countNL > shortN) countNL = 0;
			countOL++; if (countOL < 0 || countOL > shortO) countOL = 0;
			countPL++; if (countPL < 0 || countPL > shortP) countPL = 0;
			countAR++; if (countAR < 0 || countAR > shortA) countAR = 0;
			countER++; if (countER < 0 || countER > shortE) countER = 0;
			countIR++; if (countIR < 0 || countIR > shortI) countIR = 0;
			countMR++; if (countMR < 0 || countMR > shortM) countMR = 0;
			
			outML += aML[countML-((countML > shortM)?shortM+1:0)];
			outNL += aNL[countNL-((countNL > shortN)?shortN+1:0)];
			outOL += aOL[countOL-((countOL > shortO)?shortO+1:0)];
			outPL += aPL[countPL-((countPL > shortP)?shortP+1:0)];
			outAR += aAR[countAR-((countAR > shortA)?shortA+1:0)];
			outER += aER[countER-((countER > shortE)?shortE+1:0)];
			outIR += aIR[countIR-((countIR > shortI)?shortI+1:0)];
			outMR += aMR[countMR-((countMR > shortM)?shortM+1:0)];
			
			double outSample = (outML + outML + outML + prevMulchAL)*0.25;
			prevMulchAL = outML; outML = outSample;
			outSample = (outAR + outAR + outAR + prevMulchAR)*0.25;
			prevMulchAR = outAR; outAR = outSample;
			
			feedbackAL = outML - (outNL + outOL + outPL);
			feedbackDR = outAR - (outER + outIR + outMR);
			feedbackBL = outNL - (outML + outOL + outPL);
			feedbackHR = outER - (outAR + outIR + outMR);
			feedbackCL = outOL - (outML + outNL + outPL);
			feedbackLR = outIR - (outAR + outER + outMR);
			feedbackDL = outPL - (outML + outNL + outOL);
			feedbackPR = outMR - (outAR + outER + outIR);
			//which we need to feed back into the input again, a bit
			
			outSample = (feedbackAL + avgAL)*0.5; avgAL = feedbackAL; feedbackAL = outSample;
			outSample = (feedbackBL + avgBL)*0.5; avgBL = feedbackBL; feedbackBL = outSample;
			outSample = (feedbackCL + avgCL)*0.5; avgCL = feedbackCL; feedbackCL = outSample;
			outSample = (feedbackDL + avgDL)*0.5; avgDL = feedbackDL; feedbackDL = outSample;
			outSample = (feedbackDR + avgDR)*0.5; avgDR = feedbackDR; feedbackDR = outSample;
			outSample = (feedbackHR + avgHR)*0.5; avgHR = feedbackHR; feedbackHR = outSample;
			outSample = (feedbackLR + avgLR)*0.5; avgLR = feedbackLR; feedbackLR = outSample;
			outSample = (feedbackPR + avgPR)*0.5; avgPR = feedbackPR; feedbackPR = outSample;
			//average all our reverb feedbacks
			
			inputSampleL = (outML + outNL + outOL + outPL)/8.0;
			inputSampleR = (outAR + outER + outIR + outMR)/8.0;
			//and take the final combined sum of outputs, corrected for Householder gain
			
			if (inputSampleL > 1.0) inputSampleL = 1.0;
			if (inputSampleL < -1.0) inputSampleL = -1.0;
			if (inputSampleR > 1.0) inputSampleR = 1.0;
			if (inputSampleR < -1.0) inputSampleR = -1.0;
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		
		if (cycleEnd > 1) {
			double outSample = (inputSampleL + tailL)*0.5;
			tailL = inputSampleL; inputSampleL = outSample;
			outSample = (inputSampleR + tailR)*0.5;
			tailR = inputSampleR; inputSampleR = outSample;
		} //let's average twice only at elevated sample rates
		
		inputSampleL *= sqrt(gaintrim);
		inputSampleR *= sqrt(gaintrim);
		
		if (wet < 1.0) {inputSampleL *= wet; inputSampleR *= wet;}
		if (dry < 1.0) {drySampleL *= dry; drySampleR *= dry;}
		inputSampleL += drySampleL; inputSampleR += drySampleR;
		//this is our submix verb dry/wet: 0.5 is BOTH at FULL VOLUME
		//purpose is that, if you're adding verb, you're not altering other balances
		
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

void CloudCoat::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	int cloudcoat = (int)(A*16.999);
	
	if (cloudcoat != prevcloudcoat) {
		for(int count = 0; count < kshortA+2; count++) {aAL[count] = 0.0; aAR[count] = 0.0;}
		for(int count = 0; count < kshortB+2; count++) {aBL[count] = 0.0; aBR[count] = 0.0;}
		for(int count = 0; count < kshortC+2; count++) {aCL[count] = 0.0; aCR[count] = 0.0;}
		for(int count = 0; count < kshortD+2; count++) {aDL[count] = 0.0; aDR[count] = 0.0;}
		for(int count = 0; count < kshortE+2; count++) {aEL[count] = 0.0; aER[count] = 0.0;}
		for(int count = 0; count < kshortF+2; count++) {aFL[count] = 0.0; aFR[count] = 0.0;}
		for(int count = 0; count < kshortG+2; count++) {aGL[count] = 0.0; aGR[count] = 0.0;}
		for(int count = 0; count < kshortH+2; count++) {aHL[count] = 0.0; aHR[count] = 0.0;}
		for(int count = 0; count < kshortI+2; count++) {aIL[count] = 0.0; aIR[count] = 0.0;}
		for(int count = 0; count < kshortJ+2; count++) {aJL[count] = 0.0; aJR[count] = 0.0;}
		for(int count = 0; count < kshortK+2; count++) {aKL[count] = 0.0; aKR[count] = 0.0;}
		for(int count = 0; count < kshortL+2; count++) {aLL[count] = 0.0; aLR[count] = 0.0;}
		for(int count = 0; count < kshortM+2; count++) {aML[count] = 0.0; aMR[count] = 0.0;}
		for(int count = 0; count < kshortN+2; count++) {aNL[count] = 0.0; aNR[count] = 0.0;}
		for(int count = 0; count < kshortO+2; count++) {aOL[count] = 0.0; aOR[count] = 0.0;}
		for(int count = 0; count < kshortP+2; count++) {aPL[count] = 0.0; aPR[count] = 0.0;}		
		countAL = 1;
		countBL = 1;
		countCL = 1;
		countDL = 1;	
		countEL = 1;
		countFL = 1;
		countGL = 1;
		countHL = 1;
		countIL = 1;
		countJL = 1;
		countKL = 1;
		countLL = 1;
		countML = 1;
		countNL = 1;
		countOL = 1;
		countPL = 1;
		
		countAR = 1;
		countBR = 1;
		countCR = 1;
		countDR = 1;	
		countER = 1;
		countFR = 1;
		countGR = 1;
		countHR = 1;
		countIR = 1;
		countJR = 1;
		countKR = 1;
		countLR = 1;
		countMR = 1;
		countNR = 1;
		countOR = 1;
		countPR = 1;
		switch (cloudcoat)
		{
			case 0:
				shortA = 65; shortB = 124; shortC = 83; shortD = 180; shortE = 200; shortF = 291; shortG = 108; shortH = 189; shortI = 73; shortJ = 410; shortK = 479; shortL = 310; shortM = 11; shortN = 928; shortO = 23; shortP = 654; break; //5 to 51 ms, 96 seat room. Scarcity, 1 in 125324
				//Short96
			case 1:
				shortA = 114; shortB = 205; shortC = 498; shortD = 195; shortE = 205; shortF = 318; shortG = 143; shortH = 254; shortI = 64; shortJ = 721; shortK = 512; shortL = 324; shortM = 11; shortN = 782; shortO = 26; shortP = 394; break; //7 to 52 ms, 107 seat club. Scarcity, 1 in 65537
				//Short107
			case 2:
				shortA = 118; shortB = 272; shortC = 292; shortD = 145; shortE = 200; shortF = 241; shortG = 204; shortH = 504; shortI = 50; shortJ = 678; shortK = 424; shortL = 412; shortM = 11; shortN = 1124; shortO = 47; shortP = 766; break; //8 to 58 ms, 135 seat club. Scarcity, 1 in 196272
				//Short135
			case 3:
				shortA = 19; shortB = 474; shortC = 301; shortD = 275; shortE = 260; shortF = 321; shortG = 371; shortH = 571; shortI = 50; shortJ = 410; shortK = 697; shortL = 414; shortM = 11; shortN = 986; shortO = 47; shortP = 522; break; //7 to 61 ms, 143 seat club. Scarcity, 1 in 165738
				//Short143
			case 4:
				shortA = 112; shortB = 387; shortC = 452; shortD = 289; shortE = 173; shortF = 476; shortG = 321; shortH = 593; shortI = 73; shortJ = 343; shortK = 829; shortL = 91; shortM = 11; shortN = 1055; shortO = 43; shortP = 862; break; //8 to 66 ms, 166 seat club. Scarcity, 1 in 158437
				//Short166
			case 5:
				shortA = 60; shortB = 368; shortC = 295; shortD = 272; shortE = 210; shortF = 284; shortG = 326; shortH = 830; shortI = 125; shortJ = 236; shortK = 737; shortL = 486; shortM = 11; shortN = 1178; shortO = 75; shortP = 902; break; //9 to 70 ms, 189 seat club. Scarcity, 1 in 94790
				//Short189
			case 6:
				shortA = 73; shortB = 311; shortC = 472; shortD = 251; shortE = 134; shortF = 509; shortG = 393; shortH = 591; shortI = 124; shortJ = 1070; shortK = 340; shortL = 525; shortM = 11; shortN = 1367; shortO = 75; shortP = 816; break; //7 to 79 ms, 225 seat club. Scarcity, 1 in 257803
				//Short225
			case 7:
				shortA = 159; shortB = 518; shortC = 514; shortD = 165; shortE = 275; shortF = 494; shortG = 296; shortH = 667; shortI = 75; shortJ = 1101; shortK = 116; shortL = 414; shortM = 11; shortN = 1261; shortO = 79; shortP = 998; break; //11 to 80 ms, 252 seat club. Scarcity, 1 in 175192
				//Short252
			case 8:
				shortA = 41; shortB = 741; shortC = 274; shortD = 59; shortE = 306; shortF = 332; shortG = 291; shortH = 767; shortI = 42; shortJ = 881; shortK = 959; shortL = 422; shortM = 11; shortN = 1237; shortO = 45; shortP = 958; break; //8 to 83 ms, 255 seat club. Scarcity, 1 in 185708
				//Short255
			case 9:
				shortA = 251; shortB = 437; shortC = 783; shortD = 189; shortE = 130; shortF = 272; shortG = 244; shortH = 761; shortI = 128; shortJ = 1190; shortK = 320; shortL = 491; shortM = 11; shortN = 1409; shortO = 58; shortP = 455; break; //10 to 93 ms, 323 seat club. Scarcity, 1 in 334044
				//Short323
			case 10:
				shortA = 316; shortB = 510; shortC = 1087; shortD = 349; shortE = 359; shortF = 74; shortG = 79; shortH = 1269; shortI = 34; shortJ = 693; shortK = 749; shortL = 511; shortM = 11; shortN = 1751; shortO = 93; shortP = 403; break; //9 to 110 ms, 427 seat theater. Scarcity, 1 in 200715
				//Short427
			case 11:
				shortA = 254; shortB = 651; shortC = 845; shortD = 316; shortE = 373; shortF = 267; shortG = 182; shortH = 857; shortI = 215; shortJ = 1535; shortK = 1127; shortL = 315; shortM = 11; shortN = 1649; shortO = 97; shortP = 829; break; //15 to 110 ms, 470 seat theater. Scarcity, 1 in 362673
				//Short470
			case 12:
				shortA = 113; shortB = 101; shortC = 673; shortD = 357; shortE = 340; shortF = 229; shortG = 278; shortH = 1008; shortI = 265; shortJ = 1890; shortK = 155; shortL = 267; shortM = 11; shortN = 2233; shortO = 116; shortP = 600; break; //11 to 131 ms, 606 seat theater. Scarcity, 1 in 238058
				//Short606
			case 13:
				shortA = 218; shortB = 1058; shortC = 862; shortD = 505; shortE = 297; shortF = 580; shortG = 532; shortH = 1387; shortI = 120; shortJ = 576; shortK = 1409; shortL = 473; shortM = 11; shortN = 1991; shortO = 76; shortP = 685; break; //14 to 132 ms, 643 seat theater. Scarcity, 1 in 193432
				//Short643
			case 14:
				shortA = 78; shortB = 760; shortC = 982; shortD = 528; shortE = 445; shortF = 1128; shortG = 130; shortH = 708; shortI = 22; shortJ = 2144; shortK = 354; shortL = 1169; shortM = 11; shortN = 2782; shortO = 58; shortP = 1515; break; //5 to 159 ms, 809 seat hall. Scarcity, 1 in 212274
				//Short809
			case 15:
				shortA = 330; shortB = 107; shortC = 1110; shortD = 371; shortE = 620; shortF = 143; shortG = 1014; shortH = 1763; shortI = 184; shortJ = 2068; shortK = 1406; shortL = 595; shortM = 11; shortN = 2639; shortO = 33; shortP = 1594; break; //10 to 171 ms, 984 seat hall. Scarcity, 1 in 226499
				//Short984
			case 16:
			default:
				shortA = 336; shortB = 1660; shortC = 386; shortD = 623; shortE = 693; shortF = 1079; shortG = 891; shortH = 1574; shortI = 24; shortJ = 2641; shortK = 1239; shortL = 775; shortM = 11; shortN = 3104; shortO = 55; shortP = 2366; break; //24 to 203 ms, 1541 seat hall. Scarcity, 1 in 275025
				//Short1541
		}
		prevcloudcoat = cloudcoat;
	}
	
	double gaintrim = 1.0-pow(1.0-B,2);
	double sustain = gaintrim*0.086;
	gaintrim = 1.1-gaintrim;
	double wet = C*2.0;
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL /= gaintrim;
		inputSampleR /= gaintrim;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a reverb sample			
			double outAL = inputSampleL - (aAL[(countAL+1)-((countAL+1 > shortA)?shortA+1:0)]*0.618033988749894848204586);
			double outBL = inputSampleL - (aBL[(countBL+1)-((countBL+1 > shortB)?shortB+1:0)]*0.618033988749894848204586);
			double outCL = inputSampleL - (aCL[(countCL+1)-((countCL+1 > shortC)?shortC+1:0)]*0.618033988749894848204586);
			double outDL = inputSampleL - (aDL[(countDL+1)-((countDL+1 > shortD)?shortD+1:0)]*0.618033988749894848204586);
			double outDR = inputSampleR - (aDR[(countDR+1)-((countDR+1 > shortD)?shortD+1:0)]*0.618033988749894848204586);
			double outHR = inputSampleR - (aHR[(countHR+1)-((countHR+1 > shortH)?shortH+1:0)]*0.618033988749894848204586);
			double outLR = inputSampleR - (aLR[(countLR+1)-((countLR+1 > shortL)?shortL+1:0)]*0.618033988749894848204586);
			double outPR = inputSampleR - (aPR[(countPR+1)-((countPR+1 > shortP)?shortP+1:0)]*0.618033988749894848204586);
			
			outAL += (feedbackAL * (sustain*fabs(sin(feedbackBL))));
			outBL += (feedbackBL * (sustain*fabs(sin(feedbackCL))));
			outCL += (feedbackCL * (sustain*fabs(sin(feedbackDL))));
			outDL += (feedbackDL * (sustain*fabs(sin(feedbackAL))));
			
			outDR += (feedbackDR * (sustain*fabs(sin(feedbackHR))));
			outHR += (feedbackHR * (sustain*fabs(sin(feedbackLR))));
			outLR += (feedbackLR * (sustain*fabs(sin(feedbackPR))));
			outPR += (feedbackPR * (sustain*fabs(sin(feedbackDR))));
			
			aAL[countAL] = outAL; outAL *= 0.618033988749894848204586;
			aBL[countBL] = outBL; outBL *= 0.618033988749894848204586;
			aCL[countCL] = outCL; outCL *= 0.618033988749894848204586;
			aDL[countDL] = outDL; outDL *= 0.618033988749894848204586;
			aDR[countDR] = outDR; outDR *= 0.618033988749894848204586;
			aHR[countHR] = outHR; outHR *= 0.618033988749894848204586;
			aLR[countLR] = outLR; outLR *= 0.618033988749894848204586;
			aPR[countPR] = outPR; outPR *= 0.618033988749894848204586;
			
			countAL++; if (countAL < 0 || countAL > shortA) countAL = 0;
			countBL++; if (countBL < 0 || countBL > shortB) countBL = 0;
			countCL++; if (countCL < 0 || countCL > shortC) countCL = 0;
			countDL++; if (countDL < 0 || countDL > shortD) countDL = 0;
			countDR++; if (countDR < 0 || countDR > shortD) countDR = 0;
			countHR++; if (countHR < 0 || countHR > shortH) countHR = 0;
			countLR++; if (countLR < 0 || countLR > shortL) countLR = 0;
			countPR++; if (countPR < 0 || countPR > shortP) countPR = 0;
			
			outAL += aAL[countAL-((countAL > shortA)?shortA+1:0)];
			outBL += aBL[countBL-((countBL > shortB)?shortB+1:0)];
			outCL += aCL[countCL-((countCL > shortC)?shortC+1:0)];
			outDL += aDL[countDL-((countDL > shortD)?shortD+1:0)];
			outDR += aDR[countDR-((countDR > shortD)?shortD+1:0)];
			outHR += aHR[countHR-((countHR > shortH)?shortH+1:0)];
			outLR += aLR[countLR-((countLR > shortL)?shortL+1:0)];
			outPR += aPR[countPR-((countPR > shortP)?shortP+1:0)];
			
			double outEL = (outAL - (outBL + outCL + outDL)) - (aEL[(countEL+1)-((countEL+1 > shortE)?shortE+1:0)]*0.618033988749894848204586);
			double outFL = (outBL - (outAL + outCL + outDL)) - (aFL[(countFL+1)-((countFL+1 > shortF)?shortF+1:0)]*0.618033988749894848204586);
			double outGL = (outCL - (outAL + outBL + outDL)) - (aGL[(countGL+1)-((countGL+1 > shortG)?shortG+1:0)]*0.618033988749894848204586);
			double outHL = (outDL - (outAL + outBL + outCL)) - (aHL[(countHL+1)-((countHL+1 > shortH)?shortH+1:0)]*0.618033988749894848204586);
			double outCR = (outDR - (outHR + outLR + outPR)) - (aCR[(countCR+1)-((countCR+1 > shortC)?shortC+1:0)]*0.618033988749894848204586);
			double outGR = (outHR - (outDR + outLR + outPR)) - (aGR[(countGR+1)-((countGR+1 > shortG)?shortG+1:0)]*0.618033988749894848204586);
			double outKR = (outLR - (outDR + outHR + outPR)) - (aKR[(countKR+1)-((countKR+1 > shortK)?shortK+1:0)]*0.618033988749894848204586);
			double outOR = (outPR - (outDR + outHR + outLR)) - (aOR[(countOR+1)-((countOR+1 > shortO)?shortO+1:0)]*0.618033988749894848204586);
			
			aEL[countEL] = outEL; outEL *= 0.618033988749894848204586;
			aFL[countFL] = outFL; outFL *= 0.618033988749894848204586;
			aGL[countGL] = outGL; outGL *= 0.618033988749894848204586;
			aHL[countHL] = outHL; outHL *= 0.618033988749894848204586;
			aCR[countCR] = outCR; outCR *= 0.618033988749894848204586;
			aGR[countGR] = outGR; outGR *= 0.618033988749894848204586;
			aKR[countKR] = outKR; outKR *= 0.618033988749894848204586;
			aOR[countOR] = outOR; outOR *= 0.618033988749894848204586;
			
			countEL++; if (countEL < 0 || countEL > shortE) countEL = 0;
			countFL++; if (countFL < 0 || countFL > shortF) countFL = 0;
			countGL++; if (countGL < 0 || countGL > shortG) countGL = 0;
			countHL++; if (countHL < 0 || countHL > shortH) countHL = 0;
			countCR++; if (countCR < 0 || countCR > shortC) countCR = 0;
			countGR++; if (countGR < 0 || countGR > shortG) countGR = 0;
			countKR++; if (countKR < 0 || countKR > shortK) countKR = 0;
			countOR++; if (countOR < 0 || countOR > shortO) countOR = 0;
			
			outEL += aEL[countEL-((countEL > shortE)?shortE+1:0)];
			outFL += aFL[countFL-((countFL > shortF)?shortF+1:0)];
			outGL += aGL[countGL-((countGL > shortG)?shortG+1:0)];
			outHL += aHL[countHL-((countHL > shortH)?shortH+1:0)];
			outCR += aCR[countCR-((countCR > shortC)?shortC+1:0)];
			outGR += aGR[countGR-((countGR > shortG)?shortG+1:0)];
			outKR += aKR[countKR-((countKR > shortK)?shortK+1:0)];
			outOR += aOR[countOR-((countOR > shortO)?shortO+1:0)];
			
			double outIL = (outEL - (outFL + outGL + outHL)) - (aIL[(countIL+1)-((countIL+1 > shortI)?shortI+1:0)]*0.618033988749894848204586);
			double outJL = (outFL - (outEL + outGL + outHL)) - (aJL[(countJL+1)-((countJL+1 > shortJ)?shortJ+1:0)]*0.618033988749894848204586);
			double outKL = (outGL - (outEL + outFL + outHL)) - (aKL[(countKL+1)-((countKL+1 > shortK)?shortK+1:0)]*0.618033988749894848204586);
			double outLL = (outHL - (outEL + outFL + outGL)) - (aLL[(countLL+1)-((countLL+1 > shortL)?shortL+1:0)]*0.618033988749894848204586);
			double outBR = (outCR - (outGR + outKR + outOR)) - (aBR[(countBR+1)-((countBR+1 > shortB)?shortB+1:0)]*0.618033988749894848204586);
			double outFR = (outGR - (outCR + outKR + outOR)) - (aFR[(countFR+1)-((countFR+1 > shortF)?shortF+1:0)]*0.618033988749894848204586);
			double outJR = (outKR - (outCR + outGR + outOR)) - (aJR[(countJR+1)-((countJR+1 > shortJ)?shortJ+1:0)]*0.618033988749894848204586);
			double outNR = (outOR - (outCR + outGR + outKR)) - (aNR[(countNR+1)-((countNR+1 > shortN)?shortN+1:0)]*0.618033988749894848204586);
			
			aIL[countIL] = outIL; outIL *= 0.618033988749894848204586;
			aJL[countJL] = outJL; outJL *= 0.618033988749894848204586;
			aKL[countKL] = outKL; outKL *= 0.618033988749894848204586;
			aLL[countLL] = outLL; outLL *= 0.618033988749894848204586;
			aBR[countBR] = outBR; outBR *= 0.618033988749894848204586;
			aFR[countFR] = outFR; outFR *= 0.618033988749894848204586;
			aJR[countJR] = outJR; outJR *= 0.618033988749894848204586;
			aNR[countNR] = outNR; outNR *= 0.618033988749894848204586;
			
			countIL++; if (countIL < 0 || countIL > shortI) countIL = 0;
			countJL++; if (countJL < 0 || countJL > shortJ) countJL = 0;
			countKL++; if (countKL < 0 || countKL > shortK) countKL = 0;
			countLL++; if (countLL < 0 || countLL > shortL) countLL = 0;
			countBR++; if (countBR < 0 || countBR > shortB) countBR = 0;
			countFR++; if (countFR < 0 || countFR > shortF) countFR = 0;
			countJR++; if (countJR < 0 || countJR > shortJ) countJR = 0;
			countNR++; if (countNR < 0 || countNR > shortN) countNR = 0;
			
			outIL += aIL[countIL-((countIL > shortI)?shortI+1:0)];
			outJL += aJL[countJL-((countJL > shortJ)?shortJ+1:0)];
			outKL += aKL[countKL-((countKL > shortK)?shortK+1:0)];
			outLL += aLL[countLL-((countLL > shortL)?shortL+1:0)];
			outBR += aBR[countBR-((countBR > shortB)?shortB+1:0)];
			outFR += aFR[countFR-((countFR > shortF)?shortF+1:0)];
			outJR += aJR[countJR-((countJR > shortJ)?shortJ+1:0)];
			outNR += aNR[countNR-((countNR > shortN)?shortN+1:0)];
			
			double outML = (outIL - (outJL + outKL + outLL)) - (aML[(countML+1)-((countML+1 > shortM)?shortM+1:0)]*0.618033988749894848204586);
			double outNL = (outJL - (outIL + outKL + outLL)) - (aNL[(countNL+1)-((countNL+1 > shortN)?shortN+1:0)]*0.618033988749894848204586);
			double outOL = (outKL - (outIL + outJL + outLL)) - (aOL[(countOL+1)-((countOL+1 > shortO)?shortO+1:0)]*0.618033988749894848204586);
			double outPL = (outLL - (outIL + outJL + outKL)) - (aPL[(countPL+1)-((countPL+1 > shortP)?shortP+1:0)]*0.618033988749894848204586);
			double outAR = (outBR - (outFR + outJR + outNR)) - (aAR[(countAR+1)-((countAR+1 > shortA)?shortA+1:0)]*0.618033988749894848204586);
			double outER = (outFR - (outBR + outJR + outNR)) - (aER[(countER+1)-((countER+1 > shortE)?shortE+1:0)]*0.618033988749894848204586);
			double outIR = (outJR - (outBR + outFR + outNR)) - (aIR[(countIR+1)-((countIR+1 > shortI)?shortI+1:0)]*0.618033988749894848204586);
			double outMR = (outNR - (outBR + outFR + outJR)) - (aMR[(countMR+1)-((countMR+1 > shortM)?shortM+1:0)]*0.618033988749894848204586);
			
			aML[countML] = outML; outML *= 0.618033988749894848204586;
			aNL[countNL] = outNL; outNL *= 0.618033988749894848204586;
			aOL[countOL] = outOL; outOL *= 0.618033988749894848204586;
			aPL[countPL] = outPL; outPL *= 0.618033988749894848204586;
			aAR[countAR] = outAR; outAR *= 0.618033988749894848204586;
			aER[countER] = outER; outER *= 0.618033988749894848204586;
			aIR[countIR] = outIR; outIR *= 0.618033988749894848204586;
			aMR[countMR] = outMR; outMR *= 0.618033988749894848204586;
			
			countML++; if (countML < 0 || countML > shortM) countML = 0;
			countNL++; if (countNL < 0 || countNL > shortN) countNL = 0;
			countOL++; if (countOL < 0 || countOL > shortO) countOL = 0;
			countPL++; if (countPL < 0 || countPL > shortP) countPL = 0;
			countAR++; if (countAR < 0 || countAR > shortA) countAR = 0;
			countER++; if (countER < 0 || countER > shortE) countER = 0;
			countIR++; if (countIR < 0 || countIR > shortI) countIR = 0;
			countMR++; if (countMR < 0 || countMR > shortM) countMR = 0;
			
			outML += aML[countML-((countML > shortM)?shortM+1:0)];
			outNL += aNL[countNL-((countNL > shortN)?shortN+1:0)];
			outOL += aOL[countOL-((countOL > shortO)?shortO+1:0)];
			outPL += aPL[countPL-((countPL > shortP)?shortP+1:0)];
			outAR += aAR[countAR-((countAR > shortA)?shortA+1:0)];
			outER += aER[countER-((countER > shortE)?shortE+1:0)];
			outIR += aIR[countIR-((countIR > shortI)?shortI+1:0)];
			outMR += aMR[countMR-((countMR > shortM)?shortM+1:0)];
			
			double outSample = (outML + outML + outML + prevMulchAL)*0.25;
			prevMulchAL = outML; outML = outSample;
			outSample = (outAR + outAR + outAR + prevMulchAR)*0.25;
			prevMulchAR = outAR; outAR = outSample;
			
			feedbackAL = outML - (outNL + outOL + outPL);
			feedbackDR = outAR - (outER + outIR + outMR);
			feedbackBL = outNL - (outML + outOL + outPL);
			feedbackHR = outER - (outAR + outIR + outMR);
			feedbackCL = outOL - (outML + outNL + outPL);
			feedbackLR = outIR - (outAR + outER + outMR);
			feedbackDL = outPL - (outML + outNL + outOL);
			feedbackPR = outMR - (outAR + outER + outIR);
			//which we need to feed back into the input again, a bit
			
			outSample = (feedbackAL + avgAL)*0.5; avgAL = feedbackAL; feedbackAL = outSample;
			outSample = (feedbackBL + avgBL)*0.5; avgBL = feedbackBL; feedbackBL = outSample;
			outSample = (feedbackCL + avgCL)*0.5; avgCL = feedbackCL; feedbackCL = outSample;
			outSample = (feedbackDL + avgDL)*0.5; avgDL = feedbackDL; feedbackDL = outSample;
			outSample = (feedbackDR + avgDR)*0.5; avgDR = feedbackDR; feedbackDR = outSample;
			outSample = (feedbackHR + avgHR)*0.5; avgHR = feedbackHR; feedbackHR = outSample;
			outSample = (feedbackLR + avgLR)*0.5; avgLR = feedbackLR; feedbackLR = outSample;
			outSample = (feedbackPR + avgPR)*0.5; avgPR = feedbackPR; feedbackPR = outSample;
			//average all our reverb feedbacks
			
			inputSampleL = (outML + outNL + outOL + outPL)/8.0;
			inputSampleR = (outAR + outER + outIR + outMR)/8.0;
			//and take the final combined sum of outputs, corrected for Householder gain
			
			if (inputSampleL > 1.0) inputSampleL = 1.0;
			if (inputSampleL < -1.0) inputSampleL = -1.0;
			if (inputSampleR > 1.0) inputSampleR = 1.0;
			if (inputSampleR < -1.0) inputSampleR = -1.0;
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		
		if (cycleEnd > 1) {
			double outSample = (inputSampleL + tailL)*0.5;
			tailL = inputSampleL; inputSampleL = outSample;
			outSample = (inputSampleR + tailR)*0.5;
			tailR = inputSampleR; inputSampleR = outSample;
		} //let's average twice only at elevated sample rates
		
		inputSampleL *= sqrt(gaintrim);
		inputSampleR *= sqrt(gaintrim);
		
		if (wet < 1.0) {inputSampleL *= wet; inputSampleR *= wet;}
		if (dry < 1.0) {drySampleL *= dry; drySampleR *= dry;}
		inputSampleL += drySampleL; inputSampleR += drySampleR;
		//this is our submix verb dry/wet: 0.5 is BOTH at FULL VOLUME
		//purpose is that, if you're adding verb, you're not altering other balances
		
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
