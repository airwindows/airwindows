/* ========================================
 *  BrightAmbience - BrightAmbience.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BrightAmbience_H
#include "BrightAmbience.h"
#endif

void BrightAmbience::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double wet = A;
	int cpu = (int)(B * 32);
	double decay = 1.0+(pow(C,2)/2.0);
	double scale = 4;
	int count;
	int32_t temp;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		if (gcount < 0 || gcount > 12680) {gcount = 12680;}
		count = gcount;
		
		pL[count+12680] = pL[count] = (int32_t)(inputSampleL*8388352.0);
		pR[count+12680] = pR[count] = (int32_t)(inputSampleR*8388352.0);
		//double buffer
		//-8388352 to 8388352 is equal to 24 bit linear space
		
		//begin L
		temp = 0;
		switch (cpu)
		{
			case 32:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+12679]; temp += pL[count+12671]; temp += pL[count+12661]; temp += pL[count+12613]; temp += pL[count+12587]; temp += pL[count+12577]; temp += pL[count+12563]; temp += pL[count+12557]; temp += pL[count+12521]; temp += pL[count+12499]; 
				temp += pL[count+12493]; temp += pL[count+12457]; temp += pL[count+12427]; temp += pL[count+12401]; temp += pL[count+12373]; temp += pL[count+12367]; temp += pL[count+12349]; temp += pL[count+12337]; temp += pL[count+12323]; temp += pL[count+12301]; 
			case 31:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+12281]; temp += pL[count+12247]; temp += pL[count+12239]; temp += pL[count+12203]; temp += pL[count+12197]; temp += pL[count+12191]; temp += pL[count+12163]; temp += pL[count+12143]; temp += pL[count+12127]; temp += pL[count+12109]; 
				temp += pL[count+12083]; temp += pL[count+12077]; temp += pL[count+12059]; temp += pL[count+12037]; temp += pL[count+11989]; temp += pL[count+11969]; temp += pL[count+11951]; temp += pL[count+11933]; temp += pL[count+11927]; temp += pL[count+11911]; 
			case 30:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+11897]; temp += pL[count+11873]; temp += pL[count+11843]; temp += pL[count+11819]; temp += pL[count+11791]; temp += pL[count+11773]; temp += pL[count+11731]; temp += pL[count+11723]; temp += pL[count+11701]; temp += pL[count+11687]; 
				temp += pL[count+11659]; temp += pL[count+11623]; temp += pL[count+11611]; temp += pL[count+11587]; temp += pL[count+11573]; temp += pL[count+11549]; temp += pL[count+11537]; temp += pL[count+11527]; temp += pL[count+11513]; temp += pL[count+11503]; 
			case 29:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+11467]; temp += pL[count+11441]; temp += pL[count+11413]; temp += pL[count+11399]; temp += pL[count+11363]; temp += pL[count+11353]; temp += pL[count+11329]; temp += pL[count+11323]; temp += pL[count+11303]; temp += pL[count+11293]; 
				temp += pL[count+11281]; temp += pL[count+11251]; temp += pL[count+11231]; temp += pL[count+11209]; temp += pL[count+11201]; temp += pL[count+11177]; temp += pL[count+11171]; temp += pL[count+11153]; temp += pL[count+11141]; temp += pL[count+11117]; 
			case 28:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+11099]; temp += pL[count+11071]; temp += pL[count+11047]; temp += pL[count+11033]; temp += pL[count+11021]; temp += pL[count+10993]; temp += pL[count+10973]; temp += pL[count+10943]; temp += pL[count+10933]; temp += pL[count+10919]; 
				temp += pL[count+10897]; temp += pL[count+10879]; temp += pL[count+10867]; temp += pL[count+10841]; temp += pL[count+10783]; temp += pL[count+10763]; temp += pL[count+10751]; temp += pL[count+10733]; temp += pL[count+10721]; temp += pL[count+10687]; 
			case 27:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+10669]; temp += pL[count+10657]; temp += pL[count+10649]; temp += pL[count+10637]; temp += pL[count+10621]; temp += pL[count+10609]; temp += pL[count+10603]; temp += pL[count+10597]; temp += pL[count+10571]; temp += pL[count+10561]; 
				temp += pL[count+10553]; temp += pL[count+10499]; temp += pL[count+10481]; temp += pL[count+10453]; temp += pL[count+10439]; temp += pL[count+10417]; temp += pL[count+10399]; temp += pL[count+10391]; temp += pL[count+10363]; temp += pL[count+10343]; 
			case 26:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+10321]; temp += pL[count+10301]; temp += pL[count+10261]; temp += pL[count+10253]; temp += pL[count+10231]; temp += pL[count+10211]; temp += pL[count+10189]; temp += pL[count+10159]; temp += pL[count+10139]; temp += pL[count+10099]; 
				temp += pL[count+10093]; temp += pL[count+10079]; temp += pL[count+10039]; temp += pL[count+9983]; temp += pL[count+9973]; temp += pL[count+9953]; temp += pL[count+9929]; temp += pL[count+9911]; temp += pL[count+9893]; temp += pL[count+9871]; 
			case 25:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+9851]; temp += pL[count+9817]; temp += pL[count+9773]; temp += pL[count+9763]; temp += pL[count+9721]; temp += pL[count+9703]; temp += pL[count+9697]; temp += pL[count+9673]; temp += pL[count+9641]; temp += pL[count+9631]; 
				temp += pL[count+9619]; temp += pL[count+9607]; temp += pL[count+9581]; temp += pL[count+9559]; temp += pL[count+9547]; temp += pL[count+9521]; temp += pL[count+9493]; temp += pL[count+9487]; temp += pL[count+9469]; temp += pL[count+9449]; 
			case 24:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+9433]; temp += pL[count+9413]; temp += pL[count+9403]; temp += pL[count+9391]; temp += pL[count+9371]; temp += pL[count+9349]; temp += pL[count+9323]; temp += pL[count+9311]; temp += pL[count+9301]; temp += pL[count+9277]; 
				temp += pL[count+9257]; temp += pL[count+9239]; temp += pL[count+9227]; temp += pL[count+9221]; temp += pL[count+9169]; temp += pL[count+9161]; temp += pL[count+9151]; temp += pL[count+9137]; temp += pL[count+9109]; temp += pL[count+9101]; 
			case 23:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+9089]; temp += pL[count+9071]; temp += pL[count+9043]; temp += pL[count+9019]; temp += pL[count+9007]; temp += pL[count+8971]; temp += pL[count+8963]; temp += pL[count+8929]; temp += pL[count+8917]; temp += pL[count+8893]; 
				temp += pL[count+8863]; temp += pL[count+8849]; temp += pL[count+8837]; temp += pL[count+8803]; temp += pL[count+8779]; temp += pL[count+8761]; temp += pL[count+8753]; temp += pL[count+8731]; temp += pL[count+8699]; temp += pL[count+8677]; 
			case 22:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+8669]; temp += pL[count+8657]; temp += pL[count+8647]; temp += pL[count+8623]; temp += pL[count+8599]; temp += pL[count+8581]; temp += pL[count+8569]; temp += pL[count+8563]; temp += pL[count+8539]; temp += pL[count+8521]; 
				temp += pL[count+8507]; temp += pL[count+8497]; temp += pL[count+8473]; temp += pL[count+8437]; temp += pL[count+8411]; temp += pL[count+8387]; temp += pL[count+8371]; temp += pL[count+8363]; temp += pL[count+8339]; temp += pL[count+8303]; 
			case 21:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+8287]; temp += pL[count+8273]; temp += pL[count+8233]; temp += pL[count+8221]; temp += pL[count+8209]; temp += pL[count+8201]; temp += pL[count+8173]; temp += pL[count+8161]; temp += pL[count+8143]; temp += pL[count+8117]; 
				temp += pL[count+8107]; temp += pL[count+8083]; temp += pL[count+8069]; temp += pL[count+8059]; temp += pL[count+8023]; temp += pL[count+8009]; temp += pL[count+7991]; temp += pL[count+7943]; temp += pL[count+7927]; temp += pL[count+7909]; 
			case 20:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+7879]; temp += pL[count+7871]; temp += pL[count+7841]; temp += pL[count+7831]; temp += pL[count+7813]; temp += pL[count+7799]; temp += pL[count+7783]; temp += pL[count+7747]; temp += pL[count+7733]; temp += pL[count+7709]; 
				temp += pL[count+7673]; temp += pL[count+7661]; temp += pL[count+7633]; temp += pL[count+7627]; temp += pL[count+7607]; temp += pL[count+7597]; temp += pL[count+7561]; temp += pL[count+7543]; temp += pL[count+7529]; temp += pL[count+7513]; 
			case 19:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+7481]; temp += pL[count+7447]; temp += pL[count+7429]; temp += pL[count+7409]; temp += pL[count+7397]; temp += pL[count+7369]; temp += pL[count+7363]; temp += pL[count+7319]; temp += pL[count+7291]; temp += pL[count+7277]; 
				temp += pL[count+7267]; temp += pL[count+7249]; temp += pL[count+7243]; temp += pL[count+7237]; temp += pL[count+7187]; temp += pL[count+7171]; temp += pL[count+7151]; temp += pL[count+7117]; temp += pL[count+7099]; temp += pL[count+7087]; 
			case 18:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+7069]; temp += pL[count+7061]; temp += pL[count+7039]; temp += pL[count+6997]; temp += pL[count+6983]; temp += pL[count+6971]; temp += pL[count+6953]; temp += pL[count+6941]; temp += pL[count+6919]; temp += pL[count+6913]; 
				temp += pL[count+6907]; temp += pL[count+6859]; temp += pL[count+6847]; temp += pL[count+6829]; temp += pL[count+6823]; temp += pL[count+6809]; temp += pL[count+6787]; temp += pL[count+6763]; temp += pL[count+6749]; temp += pL[count+6731]; 
			case 17:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+6707]; temp += pL[count+6677]; temp += pL[count+6653]; temp += pL[count+6641]; temp += pL[count+6623]; temp += pL[count+6607]; temp += pL[count+6581]; temp += pL[count+6563]; temp += pL[count+6553]; temp += pL[count+6539]; 
				temp += pL[count+6511]; temp += pL[count+6499]; temp += pL[count+6487]; temp += pL[count+6469]; temp += pL[count+6457]; temp += pL[count+6449]; temp += pL[count+6439]; temp += pL[count+6413]; temp += pL[count+6397]; temp += pL[count+6373]; 
			case 16:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+6353]; temp += pL[count+6329]; temp += pL[count+6317]; temp += pL[count+6301]; temp += pL[count+6281]; temp += pL[count+6263]; temp += pL[count+6253]; temp += pL[count+6233]; temp += pL[count+6197]; temp += pL[count+6163]; 
				temp += pL[count+6137]; temp += pL[count+6127]; temp += pL[count+6109]; temp += pL[count+6077]; temp += pL[count+6067]; temp += pL[count+6049]; temp += pL[count+6031]; temp += pL[count+6011]; temp += pL[count+6001]; temp += pL[count+5977]; 
			case 15:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+5959]; temp += pL[count+5947]; temp += pL[count+5933]; temp += pL[count+5921]; temp += pL[count+5911]; temp += pL[count+5897]; temp += pL[count+5879]; temp += pL[count+5849]; temp += pL[count+5833]; temp += pL[count+5813]; 
				temp += pL[count+5807]; temp += pL[count+5773]; temp += pL[count+5743]; temp += pL[count+5737]; temp += pL[count+5729]; temp += pL[count+5717]; temp += pL[count+5699]; temp += pL[count+5669]; temp += pL[count+5641]; temp += pL[count+5627]; 
			case 14:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+5611]; temp += pL[count+5599]; temp += pL[count+5587]; temp += pL[count+5569]; temp += pL[count+5549]; temp += pL[count+5521]; temp += pL[count+5503]; temp += pL[count+5483]; temp += pL[count+5461]; temp += pL[count+5429]; 
				temp += pL[count+5401]; temp += pL[count+5377]; temp += pL[count+5353]; temp += pL[count+5333]; temp += pL[count+5323]; temp += pL[count+5267]; temp += pL[count+5261]; temp += pL[count+5237]; temp += pL[count+5213]; temp += pL[count+5191]; 
			case 13:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+5179]; temp += pL[count+5147]; temp += pL[count+5113]; temp += pL[count+5101]; temp += pL[count+5087]; temp += pL[count+5081]; temp += pL[count+5059]; temp += pL[count+5053]; temp += pL[count+5021]; temp += pL[count+5009]; 
				temp += pL[count+4979]; temp += pL[count+4961]; temp += pL[count+4939]; temp += pL[count+4931]; temp += pL[count+4903]; temp += pL[count+4891]; temp += pL[count+4877]; temp += pL[count+4871]; temp += pL[count+4861]; temp += pL[count+4849]; 
			case 12:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+4813]; temp += pL[count+4799]; temp += pL[count+4777]; temp += pL[count+4757]; temp += pL[count+4733]; temp += pL[count+4703]; temp += pL[count+4679]; temp += pL[count+4657]; temp += pL[count+4643]; temp += pL[count+4631]; 
				temp += pL[count+4601]; temp += pL[count+4591]; temp += pL[count+4577]; temp += pL[count+4547]; temp += pL[count+4517]; temp += pL[count+4507]; temp += pL[count+4489]; temp += pL[count+4471]; temp += pL[count+4439]; temp += pL[count+4421]; 
			case 11:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+4399]; temp += pL[count+4367]; temp += pL[count+4357]; temp += pL[count+4343]; temp += pL[count+4331]; temp += pL[count+4307]; temp += pL[count+4289]; temp += pL[count+4279]; temp += pL[count+4243]; temp += pL[count+4223]; 
				temp += pL[count+4213]; temp += pL[count+4187]; temp += pL[count+4181]; temp += pL[count+4159]; temp += pL[count+4147]; temp += pL[count+4133]; temp += pL[count+4103]; temp += pL[count+4097]; temp += pL[count+4087]; temp += pL[count+4073]; 
			case 10:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+4033]; temp += pL[count+4009]; temp += pL[count+4001]; temp += pL[count+3979]; temp += pL[count+3961]; temp += pL[count+3937]; temp += pL[count+3919]; temp += pL[count+3893]; temp += pL[count+3877]; temp += pL[count+3851]; 
				temp += pL[count+3833]; temp += pL[count+3821]; temp += pL[count+3799]; temp += pL[count+3791]; temp += pL[count+3769]; temp += pL[count+3757]; temp += pL[count+3733]; temp += pL[count+3721]; temp += pL[count+3707]; temp += pL[count+3691]; 
			case 9:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+3677]; temp += pL[count+3667]; temp += pL[count+3653]; temp += pL[count+3631]; temp += pL[count+3613]; temp += pL[count+3593]; temp += pL[count+3581]; temp += pL[count+3559]; temp += pL[count+3553]; temp += pL[count+3523]; 
				temp += pL[count+3503]; temp += pL[count+3469]; temp += pL[count+3457]; temp += pL[count+3443]; temp += pL[count+3431]; temp += pL[count+3419]; temp += pL[count+3391]; temp += pL[count+3377]; temp += pL[count+3341]; temp += pL[count+3329]; 
			case 8:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+3299]; temp += pL[count+3289]; temp += pL[count+3281]; temp += pL[count+3253]; temp += pL[count+3229]; temp += pL[count+3221]; temp += pL[count+3191]; temp += pL[count+3167]; temp += pL[count+3151]; temp += pL[count+3139]; 
				temp += pL[count+3121]; temp += pL[count+3113]; temp += pL[count+3103]; temp += pL[count+3083]; temp += pL[count+3067]; temp += pL[count+3043]; temp += pL[count+3013]; temp += pL[count+3007]; temp += pL[count+2993]; temp += pL[count+2981]; 
			case 7:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+2963]; temp += pL[count+2957]; temp += pL[count+2911]; temp += pL[count+2897]; temp += pL[count+2879]; temp += pL[count+2861]; temp += pL[count+2837]; temp += pL[count+2801]; temp += pL[count+2777]; temp += pL[count+2767]; 
				temp += pL[count+2731]; temp += pL[count+2707]; temp += pL[count+2683]; temp += pL[count+2671]; temp += pL[count+2663]; temp += pL[count+2647]; temp += pL[count+2623]; temp += pL[count+2603]; temp += pL[count+2579]; temp += pL[count+2563]; 
			case 6:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+2533]; temp += pL[count+2509]; temp += pL[count+2497]; temp += pL[count+2479]; temp += pL[count+2459]; temp += pL[count+2441]; temp += pL[count+2431]; temp += pL[count+2419]; temp += pL[count+2377]; temp += pL[count+2353]; 
				temp += pL[count+2341]; temp += pL[count+2329]; temp += pL[count+2299]; temp += pL[count+2293]; temp += pL[count+2267]; temp += pL[count+2251]; temp += pL[count+2239]; temp += pL[count+2227]; temp += pL[count+2183]; temp += pL[count+2167]; 
			case 5:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+2153]; temp += pL[count+2129]; temp += pL[count+2117]; temp += pL[count+2083]; temp += pL[count+2069]; temp += pL[count+2047]; temp += pL[count+2039]; temp += pL[count+2027]; temp += pL[count+2011]; temp += pL[count+1969]; 
				temp += pL[count+1951]; temp += pL[count+1919]; temp += pL[count+1909]; temp += pL[count+1891]; temp += pL[count+1871]; temp += pL[count+1847]; temp += pL[count+1837]; temp += pL[count+1819]; temp += pL[count+1807]; temp += pL[count+1787]; 
			case 4:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+1777]; temp += pL[count+1747]; temp += pL[count+1711]; temp += pL[count+1693]; temp += pL[count+1679]; temp += pL[count+1657]; temp += pL[count+1639]; temp += pL[count+1633]; temp += pL[count+1609]; temp += pL[count+1579]; 
				temp += pL[count+1549]; temp += pL[count+1523]; temp += pL[count+1513]; temp += pL[count+1501]; temp += pL[count+1483]; temp += pL[count+1469]; temp += pL[count+1451]; temp += pL[count+1439]; temp += pL[count+1427]; temp += pL[count+1403]; 
			case 3:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+1373]; temp += pL[count+1357]; temp += pL[count+1343]; temp += pL[count+1331]; temp += pL[count+1319]; temp += pL[count+1291]; temp += pL[count+1279]; temp += pL[count+1271]; temp += pL[count+1249]; temp += pL[count+1241]; 
				temp += pL[count+1217]; temp += pL[count+1187]; temp += pL[count+1147]; temp += pL[count+1121]; temp += pL[count+1109]; temp += pL[count+1087]; temp += pL[count+1067]; temp += pL[count+1051]; temp += pL[count+1037]; temp += pL[count+1019]; 
			case 2:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+1009]; temp += pL[count+991]; temp += pL[count+971]; temp += pL[count+961]; temp += pL[count+947]; temp += pL[count+923]; temp += pL[count+913]; temp += pL[count+899]; temp += pL[count+869]; temp += pL[count+857]; 
				temp += pL[count+829]; temp += pL[count+811]; temp += pL[count+803]; temp += pL[count+793]; temp += pL[count+767]; temp += pL[count+727]; temp += pL[count+713]; temp += pL[count+691]; temp += pL[count+671]; temp += pL[count+653]; 
			case 1:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+643]; temp += pL[count+619]; temp += pL[count+613]; temp += pL[count+601]; temp += pL[count+593]; temp += pL[count+577]; temp += pL[count+557]; temp += pL[count+529]; temp += pL[count+521]; temp += pL[count+503]; 
				temp += pL[count+491]; temp += pL[count+463]; temp += pL[count+457]; temp += pL[count+421]; temp += pL[count+407]; temp += pL[count+389]; temp += pL[count+379]; temp += pL[count+361]; temp += pL[count+341]; temp += pL[count+331]; 
			case 0:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+313]; temp += pL[count+289]; temp += pL[count+271]; temp += pL[count+239]; temp += pL[count+223]; temp += pL[count+209]; temp += pL[count+191]; temp += pL[count+181]; temp += pL[count+151]; temp += pL[count+143]; 
				temp += pL[count+121]; temp += pL[count+109]; temp += pL[count+73]; temp += pL[count+47]; temp += pL[count+41]; temp += pL[count+19]; temp += pL[count+11]; 
				//Highest Offset = 12679: gcount limit is 12680, Reset() goes to 25360, definition in .h must be p[25361]
		}
		inputSampleL = (inputSampleL*(1-wet))+((double)(temp/(8388352.0*scale))*wet);
		//end L
		
		//begin R
		temp = 0;
		switch (cpu)
		{
			case 32:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+12679]; temp += pR[count+12671]; temp += pR[count+12661]; temp += pR[count+12613]; temp += pR[count+12587]; temp += pR[count+12577]; temp += pR[count+12563]; temp += pR[count+12557]; temp += pR[count+12521]; temp += pR[count+12499]; 
				temp += pR[count+12493]; temp += pR[count+12457]; temp += pR[count+12427]; temp += pR[count+12401]; temp += pR[count+12373]; temp += pR[count+12367]; temp += pR[count+12349]; temp += pR[count+12337]; temp += pR[count+12323]; temp += pR[count+12301]; 
			case 31:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+12281]; temp += pR[count+12247]; temp += pR[count+12239]; temp += pR[count+12203]; temp += pR[count+12197]; temp += pR[count+12191]; temp += pR[count+12163]; temp += pR[count+12143]; temp += pR[count+12127]; temp += pR[count+12109]; 
				temp += pR[count+12083]; temp += pR[count+12077]; temp += pR[count+12059]; temp += pR[count+12037]; temp += pR[count+11989]; temp += pR[count+11969]; temp += pR[count+11951]; temp += pR[count+11933]; temp += pR[count+11927]; temp += pR[count+11911]; 
			case 30:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+11897]; temp += pR[count+11873]; temp += pR[count+11843]; temp += pR[count+11819]; temp += pR[count+11791]; temp += pR[count+11773]; temp += pR[count+11731]; temp += pR[count+11723]; temp += pR[count+11701]; temp += pR[count+11687]; 
				temp += pR[count+11659]; temp += pR[count+11623]; temp += pR[count+11611]; temp += pR[count+11587]; temp += pR[count+11573]; temp += pR[count+11549]; temp += pR[count+11537]; temp += pR[count+11527]; temp += pR[count+11513]; temp += pR[count+11503]; 
			case 29:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+11467]; temp += pR[count+11441]; temp += pR[count+11413]; temp += pR[count+11399]; temp += pR[count+11363]; temp += pR[count+11353]; temp += pR[count+11329]; temp += pR[count+11323]; temp += pR[count+11303]; temp += pR[count+11293]; 
				temp += pR[count+11281]; temp += pR[count+11251]; temp += pR[count+11231]; temp += pR[count+11209]; temp += pR[count+11201]; temp += pR[count+11177]; temp += pR[count+11171]; temp += pR[count+11153]; temp += pR[count+11141]; temp += pR[count+11117]; 
			case 28:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+11099]; temp += pR[count+11071]; temp += pR[count+11047]; temp += pR[count+11033]; temp += pR[count+11021]; temp += pR[count+10993]; temp += pR[count+10973]; temp += pR[count+10943]; temp += pR[count+10933]; temp += pR[count+10919]; 
				temp += pR[count+10897]; temp += pR[count+10879]; temp += pR[count+10867]; temp += pR[count+10841]; temp += pR[count+10783]; temp += pR[count+10763]; temp += pR[count+10751]; temp += pR[count+10733]; temp += pR[count+10721]; temp += pR[count+10687]; 
			case 27:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+10669]; temp += pR[count+10657]; temp += pR[count+10649]; temp += pR[count+10637]; temp += pR[count+10621]; temp += pR[count+10609]; temp += pR[count+10603]; temp += pR[count+10597]; temp += pR[count+10571]; temp += pR[count+10561]; 
				temp += pR[count+10553]; temp += pR[count+10499]; temp += pR[count+10481]; temp += pR[count+10453]; temp += pR[count+10439]; temp += pR[count+10417]; temp += pR[count+10399]; temp += pR[count+10391]; temp += pR[count+10363]; temp += pR[count+10343]; 
			case 26:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+10321]; temp += pR[count+10301]; temp += pR[count+10261]; temp += pR[count+10253]; temp += pR[count+10231]; temp += pR[count+10211]; temp += pR[count+10189]; temp += pR[count+10159]; temp += pR[count+10139]; temp += pR[count+10099]; 
				temp += pR[count+10093]; temp += pR[count+10079]; temp += pR[count+10039]; temp += pR[count+9983]; temp += pR[count+9973]; temp += pR[count+9953]; temp += pR[count+9929]; temp += pR[count+9911]; temp += pR[count+9893]; temp += pR[count+9871]; 
			case 25:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+9851]; temp += pR[count+9817]; temp += pR[count+9773]; temp += pR[count+9763]; temp += pR[count+9721]; temp += pR[count+9703]; temp += pR[count+9697]; temp += pR[count+9673]; temp += pR[count+9641]; temp += pR[count+9631]; 
				temp += pR[count+9619]; temp += pR[count+9607]; temp += pR[count+9581]; temp += pR[count+9559]; temp += pR[count+9547]; temp += pR[count+9521]; temp += pR[count+9493]; temp += pR[count+9487]; temp += pR[count+9469]; temp += pR[count+9449]; 
			case 24:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+9433]; temp += pR[count+9413]; temp += pR[count+9403]; temp += pR[count+9391]; temp += pR[count+9371]; temp += pR[count+9349]; temp += pR[count+9323]; temp += pR[count+9311]; temp += pR[count+9301]; temp += pR[count+9277]; 
				temp += pR[count+9257]; temp += pR[count+9239]; temp += pR[count+9227]; temp += pR[count+9221]; temp += pR[count+9169]; temp += pR[count+9161]; temp += pR[count+9151]; temp += pR[count+9137]; temp += pR[count+9109]; temp += pR[count+9101]; 
			case 23:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+9089]; temp += pR[count+9071]; temp += pR[count+9043]; temp += pR[count+9019]; temp += pR[count+9007]; temp += pR[count+8971]; temp += pR[count+8963]; temp += pR[count+8929]; temp += pR[count+8917]; temp += pR[count+8893]; 
				temp += pR[count+8863]; temp += pR[count+8849]; temp += pR[count+8837]; temp += pR[count+8803]; temp += pR[count+8779]; temp += pR[count+8761]; temp += pR[count+8753]; temp += pR[count+8731]; temp += pR[count+8699]; temp += pR[count+8677]; 
			case 22:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+8669]; temp += pR[count+8657]; temp += pR[count+8647]; temp += pR[count+8623]; temp += pR[count+8599]; temp += pR[count+8581]; temp += pR[count+8569]; temp += pR[count+8563]; temp += pR[count+8539]; temp += pR[count+8521]; 
				temp += pR[count+8507]; temp += pR[count+8497]; temp += pR[count+8473]; temp += pR[count+8437]; temp += pR[count+8411]; temp += pR[count+8387]; temp += pR[count+8371]; temp += pR[count+8363]; temp += pR[count+8339]; temp += pR[count+8303]; 
			case 21:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+8287]; temp += pR[count+8273]; temp += pR[count+8233]; temp += pR[count+8221]; temp += pR[count+8209]; temp += pR[count+8201]; temp += pR[count+8173]; temp += pR[count+8161]; temp += pR[count+8143]; temp += pR[count+8117]; 
				temp += pR[count+8107]; temp += pR[count+8083]; temp += pR[count+8069]; temp += pR[count+8059]; temp += pR[count+8023]; temp += pR[count+8009]; temp += pR[count+7991]; temp += pR[count+7943]; temp += pR[count+7927]; temp += pR[count+7909]; 
			case 20:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+7879]; temp += pR[count+7871]; temp += pR[count+7841]; temp += pR[count+7831]; temp += pR[count+7813]; temp += pR[count+7799]; temp += pR[count+7783]; temp += pR[count+7747]; temp += pR[count+7733]; temp += pR[count+7709]; 
				temp += pR[count+7673]; temp += pR[count+7661]; temp += pR[count+7633]; temp += pR[count+7627]; temp += pR[count+7607]; temp += pR[count+7597]; temp += pR[count+7561]; temp += pR[count+7543]; temp += pR[count+7529]; temp += pR[count+7513]; 
			case 19:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+7481]; temp += pR[count+7447]; temp += pR[count+7429]; temp += pR[count+7409]; temp += pR[count+7397]; temp += pR[count+7369]; temp += pR[count+7363]; temp += pR[count+7319]; temp += pR[count+7291]; temp += pR[count+7277]; 
				temp += pR[count+7267]; temp += pR[count+7249]; temp += pR[count+7243]; temp += pR[count+7237]; temp += pR[count+7187]; temp += pR[count+7171]; temp += pR[count+7151]; temp += pR[count+7117]; temp += pR[count+7099]; temp += pR[count+7087]; 
			case 18:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+7069]; temp += pR[count+7061]; temp += pR[count+7039]; temp += pR[count+6997]; temp += pR[count+6983]; temp += pR[count+6971]; temp += pR[count+6953]; temp += pR[count+6941]; temp += pR[count+6919]; temp += pR[count+6913]; 
				temp += pR[count+6907]; temp += pR[count+6859]; temp += pR[count+6847]; temp += pR[count+6829]; temp += pR[count+6823]; temp += pR[count+6809]; temp += pR[count+6787]; temp += pR[count+6763]; temp += pR[count+6749]; temp += pR[count+6731]; 
			case 17:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+6707]; temp += pR[count+6677]; temp += pR[count+6653]; temp += pR[count+6641]; temp += pR[count+6623]; temp += pR[count+6607]; temp += pR[count+6581]; temp += pR[count+6563]; temp += pR[count+6553]; temp += pR[count+6539]; 
				temp += pR[count+6511]; temp += pR[count+6499]; temp += pR[count+6487]; temp += pR[count+6469]; temp += pR[count+6457]; temp += pR[count+6449]; temp += pR[count+6439]; temp += pR[count+6413]; temp += pR[count+6397]; temp += pR[count+6373]; 
			case 16:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+6353]; temp += pR[count+6329]; temp += pR[count+6317]; temp += pR[count+6301]; temp += pR[count+6281]; temp += pR[count+6263]; temp += pR[count+6253]; temp += pR[count+6233]; temp += pR[count+6197]; temp += pR[count+6163]; 
				temp += pR[count+6137]; temp += pR[count+6127]; temp += pR[count+6109]; temp += pR[count+6077]; temp += pR[count+6067]; temp += pR[count+6049]; temp += pR[count+6031]; temp += pR[count+6011]; temp += pR[count+6001]; temp += pR[count+5977]; 
			case 15:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+5959]; temp += pR[count+5947]; temp += pR[count+5933]; temp += pR[count+5921]; temp += pR[count+5911]; temp += pR[count+5897]; temp += pR[count+5879]; temp += pR[count+5849]; temp += pR[count+5833]; temp += pR[count+5813]; 
				temp += pR[count+5807]; temp += pR[count+5773]; temp += pR[count+5743]; temp += pR[count+5737]; temp += pR[count+5729]; temp += pR[count+5717]; temp += pR[count+5699]; temp += pR[count+5669]; temp += pR[count+5641]; temp += pR[count+5627]; 
			case 14:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+5611]; temp += pR[count+5599]; temp += pR[count+5587]; temp += pR[count+5569]; temp += pR[count+5549]; temp += pR[count+5521]; temp += pR[count+5503]; temp += pR[count+5483]; temp += pR[count+5461]; temp += pR[count+5429]; 
				temp += pR[count+5401]; temp += pR[count+5377]; temp += pR[count+5353]; temp += pR[count+5333]; temp += pR[count+5323]; temp += pR[count+5267]; temp += pR[count+5261]; temp += pR[count+5237]; temp += pR[count+5213]; temp += pR[count+5191]; 
			case 13:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+5179]; temp += pR[count+5147]; temp += pR[count+5113]; temp += pR[count+5101]; temp += pR[count+5087]; temp += pR[count+5081]; temp += pR[count+5059]; temp += pR[count+5053]; temp += pR[count+5021]; temp += pR[count+5009]; 
				temp += pR[count+4979]; temp += pR[count+4961]; temp += pR[count+4939]; temp += pR[count+4931]; temp += pR[count+4903]; temp += pR[count+4891]; temp += pR[count+4877]; temp += pR[count+4871]; temp += pR[count+4861]; temp += pR[count+4849]; 
			case 12:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+4813]; temp += pR[count+4799]; temp += pR[count+4777]; temp += pR[count+4757]; temp += pR[count+4733]; temp += pR[count+4703]; temp += pR[count+4679]; temp += pR[count+4657]; temp += pR[count+4643]; temp += pR[count+4631]; 
				temp += pR[count+4601]; temp += pR[count+4591]; temp += pR[count+4577]; temp += pR[count+4547]; temp += pR[count+4517]; temp += pR[count+4507]; temp += pR[count+4489]; temp += pR[count+4471]; temp += pR[count+4439]; temp += pR[count+4421]; 
			case 11:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+4399]; temp += pR[count+4367]; temp += pR[count+4357]; temp += pR[count+4343]; temp += pR[count+4331]; temp += pR[count+4307]; temp += pR[count+4289]; temp += pR[count+4279]; temp += pR[count+4243]; temp += pR[count+4223]; 
				temp += pR[count+4213]; temp += pR[count+4187]; temp += pR[count+4181]; temp += pR[count+4159]; temp += pR[count+4147]; temp += pR[count+4133]; temp += pR[count+4103]; temp += pR[count+4097]; temp += pR[count+4087]; temp += pR[count+4073]; 
			case 10:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+4033]; temp += pR[count+4009]; temp += pR[count+4001]; temp += pR[count+3979]; temp += pR[count+3961]; temp += pR[count+3937]; temp += pR[count+3919]; temp += pR[count+3893]; temp += pR[count+3877]; temp += pR[count+3851]; 
				temp += pR[count+3833]; temp += pR[count+3821]; temp += pR[count+3799]; temp += pR[count+3791]; temp += pR[count+3769]; temp += pR[count+3757]; temp += pR[count+3733]; temp += pR[count+3721]; temp += pR[count+3707]; temp += pR[count+3691]; 
			case 9:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+3677]; temp += pR[count+3667]; temp += pR[count+3653]; temp += pR[count+3631]; temp += pR[count+3613]; temp += pR[count+3593]; temp += pR[count+3581]; temp += pR[count+3559]; temp += pR[count+3553]; temp += pR[count+3523]; 
				temp += pR[count+3503]; temp += pR[count+3469]; temp += pR[count+3457]; temp += pR[count+3443]; temp += pR[count+3431]; temp += pR[count+3419]; temp += pR[count+3391]; temp += pR[count+3377]; temp += pR[count+3341]; temp += pR[count+3329]; 
			case 8:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+3299]; temp += pR[count+3289]; temp += pR[count+3281]; temp += pR[count+3253]; temp += pR[count+3229]; temp += pR[count+3221]; temp += pR[count+3191]; temp += pR[count+3167]; temp += pR[count+3151]; temp += pR[count+3139]; 
				temp += pR[count+3121]; temp += pR[count+3113]; temp += pR[count+3103]; temp += pR[count+3083]; temp += pR[count+3067]; temp += pR[count+3043]; temp += pR[count+3013]; temp += pR[count+3007]; temp += pR[count+2993]; temp += pR[count+2981]; 
			case 7:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+2963]; temp += pR[count+2957]; temp += pR[count+2911]; temp += pR[count+2897]; temp += pR[count+2879]; temp += pR[count+2861]; temp += pR[count+2837]; temp += pR[count+2801]; temp += pR[count+2777]; temp += pR[count+2767]; 
				temp += pR[count+2731]; temp += pR[count+2707]; temp += pR[count+2683]; temp += pR[count+2671]; temp += pR[count+2663]; temp += pR[count+2647]; temp += pR[count+2623]; temp += pR[count+2603]; temp += pR[count+2579]; temp += pR[count+2563]; 
			case 6:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+2533]; temp += pR[count+2509]; temp += pR[count+2497]; temp += pR[count+2479]; temp += pR[count+2459]; temp += pR[count+2441]; temp += pR[count+2431]; temp += pR[count+2419]; temp += pR[count+2377]; temp += pR[count+2353]; 
				temp += pR[count+2341]; temp += pR[count+2329]; temp += pR[count+2299]; temp += pR[count+2293]; temp += pR[count+2267]; temp += pR[count+2251]; temp += pR[count+2239]; temp += pR[count+2227]; temp += pR[count+2183]; temp += pR[count+2167]; 
			case 5:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+2153]; temp += pR[count+2129]; temp += pR[count+2117]; temp += pR[count+2083]; temp += pR[count+2069]; temp += pR[count+2047]; temp += pR[count+2039]; temp += pR[count+2027]; temp += pR[count+2011]; temp += pR[count+1969]; 
				temp += pR[count+1951]; temp += pR[count+1919]; temp += pR[count+1909]; temp += pR[count+1891]; temp += pR[count+1871]; temp += pR[count+1847]; temp += pR[count+1837]; temp += pR[count+1819]; temp += pR[count+1807]; temp += pR[count+1787]; 
			case 4:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+1777]; temp += pR[count+1747]; temp += pR[count+1711]; temp += pR[count+1693]; temp += pR[count+1679]; temp += pR[count+1657]; temp += pR[count+1639]; temp += pR[count+1633]; temp += pR[count+1609]; temp += pR[count+1579]; 
				temp += pR[count+1549]; temp += pR[count+1523]; temp += pR[count+1513]; temp += pR[count+1501]; temp += pR[count+1483]; temp += pR[count+1469]; temp += pR[count+1451]; temp += pR[count+1439]; temp += pR[count+1427]; temp += pR[count+1403]; 
			case 3:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+1373]; temp += pR[count+1357]; temp += pR[count+1343]; temp += pR[count+1331]; temp += pR[count+1319]; temp += pR[count+1291]; temp += pR[count+1279]; temp += pR[count+1271]; temp += pR[count+1249]; temp += pR[count+1241]; 
				temp += pR[count+1217]; temp += pR[count+1187]; temp += pR[count+1147]; temp += pR[count+1121]; temp += pR[count+1109]; temp += pR[count+1087]; temp += pR[count+1067]; temp += pR[count+1051]; temp += pR[count+1037]; temp += pR[count+1019]; 
			case 2:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+1009]; temp += pR[count+991]; temp += pR[count+971]; temp += pR[count+961]; temp += pR[count+947]; temp += pR[count+923]; temp += pR[count+913]; temp += pR[count+899]; temp += pR[count+869]; temp += pR[count+857]; 
				temp += pR[count+829]; temp += pR[count+811]; temp += pR[count+803]; temp += pR[count+793]; temp += pR[count+767]; temp += pR[count+727]; temp += pR[count+713]; temp += pR[count+691]; temp += pR[count+671]; temp += pR[count+653]; 
			case 1:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+643]; temp += pR[count+619]; temp += pR[count+613]; temp += pR[count+601]; temp += pR[count+593]; temp += pR[count+577]; temp += pR[count+557]; temp += pR[count+529]; temp += pR[count+521]; temp += pR[count+503]; 
				temp += pR[count+491]; temp += pR[count+463]; temp += pR[count+457]; temp += pR[count+421]; temp += pR[count+407]; temp += pR[count+389]; temp += pR[count+379]; temp += pR[count+361]; temp += pR[count+341]; temp += pR[count+331]; 
			case 0:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+313]; temp += pR[count+289]; temp += pR[count+271]; temp += pR[count+239]; temp += pR[count+223]; temp += pR[count+209]; temp += pR[count+191]; temp += pR[count+181]; temp += pR[count+151]; temp += pR[count+143]; 
				temp += pR[count+121]; temp += pR[count+109]; temp += pR[count+73]; temp += pR[count+47]; temp += pR[count+41]; temp += pR[count+19]; temp += pR[count+11]; 
				//Highest Offset = 12679: gcount limit is 12680, Reset() goes to 25360, definition in .h must be p[25361]
		}
		inputSampleR = (inputSampleR*(1-wet))+((double)(temp/(8388352.0*scale))*wet);
		//end R
		
		gcount--;
		
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

void BrightAmbience::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double wet = A;
	int cpu = (int)(B * 32);
	double decay = 1.0+(pow(C,2)/2.0);
	double scale = 4;
	int count;
	int32_t temp;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		if (gcount < 0 || gcount > 12680) {gcount = 12680;}
		count = gcount;
		
		pL[count+12680] = pL[count] = (int32_t)(inputSampleL*8388352.0);
		pR[count+12680] = pR[count] = (int32_t)(inputSampleR*8388352.0);
		//double buffer
		//-8388352 to 8388352 is equal to 24 bit linear space
		
		//begin L
		temp = 0;
		switch (cpu)
		{
			case 32:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+12679]; temp += pL[count+12671]; temp += pL[count+12661]; temp += pL[count+12613]; temp += pL[count+12587]; temp += pL[count+12577]; temp += pL[count+12563]; temp += pL[count+12557]; temp += pL[count+12521]; temp += pL[count+12499]; 
				temp += pL[count+12493]; temp += pL[count+12457]; temp += pL[count+12427]; temp += pL[count+12401]; temp += pL[count+12373]; temp += pL[count+12367]; temp += pL[count+12349]; temp += pL[count+12337]; temp += pL[count+12323]; temp += pL[count+12301]; 
			case 31:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+12281]; temp += pL[count+12247]; temp += pL[count+12239]; temp += pL[count+12203]; temp += pL[count+12197]; temp += pL[count+12191]; temp += pL[count+12163]; temp += pL[count+12143]; temp += pL[count+12127]; temp += pL[count+12109]; 
				temp += pL[count+12083]; temp += pL[count+12077]; temp += pL[count+12059]; temp += pL[count+12037]; temp += pL[count+11989]; temp += pL[count+11969]; temp += pL[count+11951]; temp += pL[count+11933]; temp += pL[count+11927]; temp += pL[count+11911]; 
			case 30:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+11897]; temp += pL[count+11873]; temp += pL[count+11843]; temp += pL[count+11819]; temp += pL[count+11791]; temp += pL[count+11773]; temp += pL[count+11731]; temp += pL[count+11723]; temp += pL[count+11701]; temp += pL[count+11687]; 
				temp += pL[count+11659]; temp += pL[count+11623]; temp += pL[count+11611]; temp += pL[count+11587]; temp += pL[count+11573]; temp += pL[count+11549]; temp += pL[count+11537]; temp += pL[count+11527]; temp += pL[count+11513]; temp += pL[count+11503]; 
			case 29:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+11467]; temp += pL[count+11441]; temp += pL[count+11413]; temp += pL[count+11399]; temp += pL[count+11363]; temp += pL[count+11353]; temp += pL[count+11329]; temp += pL[count+11323]; temp += pL[count+11303]; temp += pL[count+11293]; 
				temp += pL[count+11281]; temp += pL[count+11251]; temp += pL[count+11231]; temp += pL[count+11209]; temp += pL[count+11201]; temp += pL[count+11177]; temp += pL[count+11171]; temp += pL[count+11153]; temp += pL[count+11141]; temp += pL[count+11117]; 
			case 28:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+11099]; temp += pL[count+11071]; temp += pL[count+11047]; temp += pL[count+11033]; temp += pL[count+11021]; temp += pL[count+10993]; temp += pL[count+10973]; temp += pL[count+10943]; temp += pL[count+10933]; temp += pL[count+10919]; 
				temp += pL[count+10897]; temp += pL[count+10879]; temp += pL[count+10867]; temp += pL[count+10841]; temp += pL[count+10783]; temp += pL[count+10763]; temp += pL[count+10751]; temp += pL[count+10733]; temp += pL[count+10721]; temp += pL[count+10687]; 
			case 27:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+10669]; temp += pL[count+10657]; temp += pL[count+10649]; temp += pL[count+10637]; temp += pL[count+10621]; temp += pL[count+10609]; temp += pL[count+10603]; temp += pL[count+10597]; temp += pL[count+10571]; temp += pL[count+10561]; 
				temp += pL[count+10553]; temp += pL[count+10499]; temp += pL[count+10481]; temp += pL[count+10453]; temp += pL[count+10439]; temp += pL[count+10417]; temp += pL[count+10399]; temp += pL[count+10391]; temp += pL[count+10363]; temp += pL[count+10343]; 
			case 26:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+10321]; temp += pL[count+10301]; temp += pL[count+10261]; temp += pL[count+10253]; temp += pL[count+10231]; temp += pL[count+10211]; temp += pL[count+10189]; temp += pL[count+10159]; temp += pL[count+10139]; temp += pL[count+10099]; 
				temp += pL[count+10093]; temp += pL[count+10079]; temp += pL[count+10039]; temp += pL[count+9983]; temp += pL[count+9973]; temp += pL[count+9953]; temp += pL[count+9929]; temp += pL[count+9911]; temp += pL[count+9893]; temp += pL[count+9871]; 
			case 25:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+9851]; temp += pL[count+9817]; temp += pL[count+9773]; temp += pL[count+9763]; temp += pL[count+9721]; temp += pL[count+9703]; temp += pL[count+9697]; temp += pL[count+9673]; temp += pL[count+9641]; temp += pL[count+9631]; 
				temp += pL[count+9619]; temp += pL[count+9607]; temp += pL[count+9581]; temp += pL[count+9559]; temp += pL[count+9547]; temp += pL[count+9521]; temp += pL[count+9493]; temp += pL[count+9487]; temp += pL[count+9469]; temp += pL[count+9449]; 
			case 24:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+9433]; temp += pL[count+9413]; temp += pL[count+9403]; temp += pL[count+9391]; temp += pL[count+9371]; temp += pL[count+9349]; temp += pL[count+9323]; temp += pL[count+9311]; temp += pL[count+9301]; temp += pL[count+9277]; 
				temp += pL[count+9257]; temp += pL[count+9239]; temp += pL[count+9227]; temp += pL[count+9221]; temp += pL[count+9169]; temp += pL[count+9161]; temp += pL[count+9151]; temp += pL[count+9137]; temp += pL[count+9109]; temp += pL[count+9101]; 
			case 23:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+9089]; temp += pL[count+9071]; temp += pL[count+9043]; temp += pL[count+9019]; temp += pL[count+9007]; temp += pL[count+8971]; temp += pL[count+8963]; temp += pL[count+8929]; temp += pL[count+8917]; temp += pL[count+8893]; 
				temp += pL[count+8863]; temp += pL[count+8849]; temp += pL[count+8837]; temp += pL[count+8803]; temp += pL[count+8779]; temp += pL[count+8761]; temp += pL[count+8753]; temp += pL[count+8731]; temp += pL[count+8699]; temp += pL[count+8677]; 
			case 22:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+8669]; temp += pL[count+8657]; temp += pL[count+8647]; temp += pL[count+8623]; temp += pL[count+8599]; temp += pL[count+8581]; temp += pL[count+8569]; temp += pL[count+8563]; temp += pL[count+8539]; temp += pL[count+8521]; 
				temp += pL[count+8507]; temp += pL[count+8497]; temp += pL[count+8473]; temp += pL[count+8437]; temp += pL[count+8411]; temp += pL[count+8387]; temp += pL[count+8371]; temp += pL[count+8363]; temp += pL[count+8339]; temp += pL[count+8303]; 
			case 21:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+8287]; temp += pL[count+8273]; temp += pL[count+8233]; temp += pL[count+8221]; temp += pL[count+8209]; temp += pL[count+8201]; temp += pL[count+8173]; temp += pL[count+8161]; temp += pL[count+8143]; temp += pL[count+8117]; 
				temp += pL[count+8107]; temp += pL[count+8083]; temp += pL[count+8069]; temp += pL[count+8059]; temp += pL[count+8023]; temp += pL[count+8009]; temp += pL[count+7991]; temp += pL[count+7943]; temp += pL[count+7927]; temp += pL[count+7909]; 
			case 20:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+7879]; temp += pL[count+7871]; temp += pL[count+7841]; temp += pL[count+7831]; temp += pL[count+7813]; temp += pL[count+7799]; temp += pL[count+7783]; temp += pL[count+7747]; temp += pL[count+7733]; temp += pL[count+7709]; 
				temp += pL[count+7673]; temp += pL[count+7661]; temp += pL[count+7633]; temp += pL[count+7627]; temp += pL[count+7607]; temp += pL[count+7597]; temp += pL[count+7561]; temp += pL[count+7543]; temp += pL[count+7529]; temp += pL[count+7513]; 
			case 19:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+7481]; temp += pL[count+7447]; temp += pL[count+7429]; temp += pL[count+7409]; temp += pL[count+7397]; temp += pL[count+7369]; temp += pL[count+7363]; temp += pL[count+7319]; temp += pL[count+7291]; temp += pL[count+7277]; 
				temp += pL[count+7267]; temp += pL[count+7249]; temp += pL[count+7243]; temp += pL[count+7237]; temp += pL[count+7187]; temp += pL[count+7171]; temp += pL[count+7151]; temp += pL[count+7117]; temp += pL[count+7099]; temp += pL[count+7087]; 
			case 18:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+7069]; temp += pL[count+7061]; temp += pL[count+7039]; temp += pL[count+6997]; temp += pL[count+6983]; temp += pL[count+6971]; temp += pL[count+6953]; temp += pL[count+6941]; temp += pL[count+6919]; temp += pL[count+6913]; 
				temp += pL[count+6907]; temp += pL[count+6859]; temp += pL[count+6847]; temp += pL[count+6829]; temp += pL[count+6823]; temp += pL[count+6809]; temp += pL[count+6787]; temp += pL[count+6763]; temp += pL[count+6749]; temp += pL[count+6731]; 
			case 17:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+6707]; temp += pL[count+6677]; temp += pL[count+6653]; temp += pL[count+6641]; temp += pL[count+6623]; temp += pL[count+6607]; temp += pL[count+6581]; temp += pL[count+6563]; temp += pL[count+6553]; temp += pL[count+6539]; 
				temp += pL[count+6511]; temp += pL[count+6499]; temp += pL[count+6487]; temp += pL[count+6469]; temp += pL[count+6457]; temp += pL[count+6449]; temp += pL[count+6439]; temp += pL[count+6413]; temp += pL[count+6397]; temp += pL[count+6373]; 
			case 16:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+6353]; temp += pL[count+6329]; temp += pL[count+6317]; temp += pL[count+6301]; temp += pL[count+6281]; temp += pL[count+6263]; temp += pL[count+6253]; temp += pL[count+6233]; temp += pL[count+6197]; temp += pL[count+6163]; 
				temp += pL[count+6137]; temp += pL[count+6127]; temp += pL[count+6109]; temp += pL[count+6077]; temp += pL[count+6067]; temp += pL[count+6049]; temp += pL[count+6031]; temp += pL[count+6011]; temp += pL[count+6001]; temp += pL[count+5977]; 
			case 15:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+5959]; temp += pL[count+5947]; temp += pL[count+5933]; temp += pL[count+5921]; temp += pL[count+5911]; temp += pL[count+5897]; temp += pL[count+5879]; temp += pL[count+5849]; temp += pL[count+5833]; temp += pL[count+5813]; 
				temp += pL[count+5807]; temp += pL[count+5773]; temp += pL[count+5743]; temp += pL[count+5737]; temp += pL[count+5729]; temp += pL[count+5717]; temp += pL[count+5699]; temp += pL[count+5669]; temp += pL[count+5641]; temp += pL[count+5627]; 
			case 14:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+5611]; temp += pL[count+5599]; temp += pL[count+5587]; temp += pL[count+5569]; temp += pL[count+5549]; temp += pL[count+5521]; temp += pL[count+5503]; temp += pL[count+5483]; temp += pL[count+5461]; temp += pL[count+5429]; 
				temp += pL[count+5401]; temp += pL[count+5377]; temp += pL[count+5353]; temp += pL[count+5333]; temp += pL[count+5323]; temp += pL[count+5267]; temp += pL[count+5261]; temp += pL[count+5237]; temp += pL[count+5213]; temp += pL[count+5191]; 
			case 13:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+5179]; temp += pL[count+5147]; temp += pL[count+5113]; temp += pL[count+5101]; temp += pL[count+5087]; temp += pL[count+5081]; temp += pL[count+5059]; temp += pL[count+5053]; temp += pL[count+5021]; temp += pL[count+5009]; 
				temp += pL[count+4979]; temp += pL[count+4961]; temp += pL[count+4939]; temp += pL[count+4931]; temp += pL[count+4903]; temp += pL[count+4891]; temp += pL[count+4877]; temp += pL[count+4871]; temp += pL[count+4861]; temp += pL[count+4849]; 
			case 12:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+4813]; temp += pL[count+4799]; temp += pL[count+4777]; temp += pL[count+4757]; temp += pL[count+4733]; temp += pL[count+4703]; temp += pL[count+4679]; temp += pL[count+4657]; temp += pL[count+4643]; temp += pL[count+4631]; 
				temp += pL[count+4601]; temp += pL[count+4591]; temp += pL[count+4577]; temp += pL[count+4547]; temp += pL[count+4517]; temp += pL[count+4507]; temp += pL[count+4489]; temp += pL[count+4471]; temp += pL[count+4439]; temp += pL[count+4421]; 
			case 11:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+4399]; temp += pL[count+4367]; temp += pL[count+4357]; temp += pL[count+4343]; temp += pL[count+4331]; temp += pL[count+4307]; temp += pL[count+4289]; temp += pL[count+4279]; temp += pL[count+4243]; temp += pL[count+4223]; 
				temp += pL[count+4213]; temp += pL[count+4187]; temp += pL[count+4181]; temp += pL[count+4159]; temp += pL[count+4147]; temp += pL[count+4133]; temp += pL[count+4103]; temp += pL[count+4097]; temp += pL[count+4087]; temp += pL[count+4073]; 
			case 10:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+4033]; temp += pL[count+4009]; temp += pL[count+4001]; temp += pL[count+3979]; temp += pL[count+3961]; temp += pL[count+3937]; temp += pL[count+3919]; temp += pL[count+3893]; temp += pL[count+3877]; temp += pL[count+3851]; 
				temp += pL[count+3833]; temp += pL[count+3821]; temp += pL[count+3799]; temp += pL[count+3791]; temp += pL[count+3769]; temp += pL[count+3757]; temp += pL[count+3733]; temp += pL[count+3721]; temp += pL[count+3707]; temp += pL[count+3691]; 
			case 9:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+3677]; temp += pL[count+3667]; temp += pL[count+3653]; temp += pL[count+3631]; temp += pL[count+3613]; temp += pL[count+3593]; temp += pL[count+3581]; temp += pL[count+3559]; temp += pL[count+3553]; temp += pL[count+3523]; 
				temp += pL[count+3503]; temp += pL[count+3469]; temp += pL[count+3457]; temp += pL[count+3443]; temp += pL[count+3431]; temp += pL[count+3419]; temp += pL[count+3391]; temp += pL[count+3377]; temp += pL[count+3341]; temp += pL[count+3329]; 
			case 8:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+3299]; temp += pL[count+3289]; temp += pL[count+3281]; temp += pL[count+3253]; temp += pL[count+3229]; temp += pL[count+3221]; temp += pL[count+3191]; temp += pL[count+3167]; temp += pL[count+3151]; temp += pL[count+3139]; 
				temp += pL[count+3121]; temp += pL[count+3113]; temp += pL[count+3103]; temp += pL[count+3083]; temp += pL[count+3067]; temp += pL[count+3043]; temp += pL[count+3013]; temp += pL[count+3007]; temp += pL[count+2993]; temp += pL[count+2981]; 
			case 7:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+2963]; temp += pL[count+2957]; temp += pL[count+2911]; temp += pL[count+2897]; temp += pL[count+2879]; temp += pL[count+2861]; temp += pL[count+2837]; temp += pL[count+2801]; temp += pL[count+2777]; temp += pL[count+2767]; 
				temp += pL[count+2731]; temp += pL[count+2707]; temp += pL[count+2683]; temp += pL[count+2671]; temp += pL[count+2663]; temp += pL[count+2647]; temp += pL[count+2623]; temp += pL[count+2603]; temp += pL[count+2579]; temp += pL[count+2563]; 
			case 6:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+2533]; temp += pL[count+2509]; temp += pL[count+2497]; temp += pL[count+2479]; temp += pL[count+2459]; temp += pL[count+2441]; temp += pL[count+2431]; temp += pL[count+2419]; temp += pL[count+2377]; temp += pL[count+2353]; 
				temp += pL[count+2341]; temp += pL[count+2329]; temp += pL[count+2299]; temp += pL[count+2293]; temp += pL[count+2267]; temp += pL[count+2251]; temp += pL[count+2239]; temp += pL[count+2227]; temp += pL[count+2183]; temp += pL[count+2167]; 
			case 5:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+2153]; temp += pL[count+2129]; temp += pL[count+2117]; temp += pL[count+2083]; temp += pL[count+2069]; temp += pL[count+2047]; temp += pL[count+2039]; temp += pL[count+2027]; temp += pL[count+2011]; temp += pL[count+1969]; 
				temp += pL[count+1951]; temp += pL[count+1919]; temp += pL[count+1909]; temp += pL[count+1891]; temp += pL[count+1871]; temp += pL[count+1847]; temp += pL[count+1837]; temp += pL[count+1819]; temp += pL[count+1807]; temp += pL[count+1787]; 
			case 4:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+1777]; temp += pL[count+1747]; temp += pL[count+1711]; temp += pL[count+1693]; temp += pL[count+1679]; temp += pL[count+1657]; temp += pL[count+1639]; temp += pL[count+1633]; temp += pL[count+1609]; temp += pL[count+1579]; 
				temp += pL[count+1549]; temp += pL[count+1523]; temp += pL[count+1513]; temp += pL[count+1501]; temp += pL[count+1483]; temp += pL[count+1469]; temp += pL[count+1451]; temp += pL[count+1439]; temp += pL[count+1427]; temp += pL[count+1403]; 
			case 3:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+1373]; temp += pL[count+1357]; temp += pL[count+1343]; temp += pL[count+1331]; temp += pL[count+1319]; temp += pL[count+1291]; temp += pL[count+1279]; temp += pL[count+1271]; temp += pL[count+1249]; temp += pL[count+1241]; 
				temp += pL[count+1217]; temp += pL[count+1187]; temp += pL[count+1147]; temp += pL[count+1121]; temp += pL[count+1109]; temp += pL[count+1087]; temp += pL[count+1067]; temp += pL[count+1051]; temp += pL[count+1037]; temp += pL[count+1019]; 
			case 2:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+1009]; temp += pL[count+991]; temp += pL[count+971]; temp += pL[count+961]; temp += pL[count+947]; temp += pL[count+923]; temp += pL[count+913]; temp += pL[count+899]; temp += pL[count+869]; temp += pL[count+857]; 
				temp += pL[count+829]; temp += pL[count+811]; temp += pL[count+803]; temp += pL[count+793]; temp += pL[count+767]; temp += pL[count+727]; temp += pL[count+713]; temp += pL[count+691]; temp += pL[count+671]; temp += pL[count+653]; 
			case 1:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+643]; temp += pL[count+619]; temp += pL[count+613]; temp += pL[count+601]; temp += pL[count+593]; temp += pL[count+577]; temp += pL[count+557]; temp += pL[count+529]; temp += pL[count+521]; temp += pL[count+503]; 
				temp += pL[count+491]; temp += pL[count+463]; temp += pL[count+457]; temp += pL[count+421]; temp += pL[count+407]; temp += pL[count+389]; temp += pL[count+379]; temp += pL[count+361]; temp += pL[count+341]; temp += pL[count+331]; 
			case 0:
				temp = (int32_t)((double)temp/decay);
				temp += pL[count+313]; temp += pL[count+289]; temp += pL[count+271]; temp += pL[count+239]; temp += pL[count+223]; temp += pL[count+209]; temp += pL[count+191]; temp += pL[count+181]; temp += pL[count+151]; temp += pL[count+143]; 
				temp += pL[count+121]; temp += pL[count+109]; temp += pL[count+73]; temp += pL[count+47]; temp += pL[count+41]; temp += pL[count+19]; temp += pL[count+11]; 
				//Highest Offset = 12679: gcount limit is 12680, Reset() goes to 25360, definition in .h must be p[25361]
		}
		inputSampleL = (inputSampleL*(1-wet))+((double)(temp/(8388352.0*scale))*wet);
		//end L
		
		//begin R
		temp = 0;
		switch (cpu)
		{
			case 32:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+12679]; temp += pR[count+12671]; temp += pR[count+12661]; temp += pR[count+12613]; temp += pR[count+12587]; temp += pR[count+12577]; temp += pR[count+12563]; temp += pR[count+12557]; temp += pR[count+12521]; temp += pR[count+12499]; 
				temp += pR[count+12493]; temp += pR[count+12457]; temp += pR[count+12427]; temp += pR[count+12401]; temp += pR[count+12373]; temp += pR[count+12367]; temp += pR[count+12349]; temp += pR[count+12337]; temp += pR[count+12323]; temp += pR[count+12301]; 
			case 31:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+12281]; temp += pR[count+12247]; temp += pR[count+12239]; temp += pR[count+12203]; temp += pR[count+12197]; temp += pR[count+12191]; temp += pR[count+12163]; temp += pR[count+12143]; temp += pR[count+12127]; temp += pR[count+12109]; 
				temp += pR[count+12083]; temp += pR[count+12077]; temp += pR[count+12059]; temp += pR[count+12037]; temp += pR[count+11989]; temp += pR[count+11969]; temp += pR[count+11951]; temp += pR[count+11933]; temp += pR[count+11927]; temp += pR[count+11911]; 
			case 30:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+11897]; temp += pR[count+11873]; temp += pR[count+11843]; temp += pR[count+11819]; temp += pR[count+11791]; temp += pR[count+11773]; temp += pR[count+11731]; temp += pR[count+11723]; temp += pR[count+11701]; temp += pR[count+11687]; 
				temp += pR[count+11659]; temp += pR[count+11623]; temp += pR[count+11611]; temp += pR[count+11587]; temp += pR[count+11573]; temp += pR[count+11549]; temp += pR[count+11537]; temp += pR[count+11527]; temp += pR[count+11513]; temp += pR[count+11503]; 
			case 29:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+11467]; temp += pR[count+11441]; temp += pR[count+11413]; temp += pR[count+11399]; temp += pR[count+11363]; temp += pR[count+11353]; temp += pR[count+11329]; temp += pR[count+11323]; temp += pR[count+11303]; temp += pR[count+11293]; 
				temp += pR[count+11281]; temp += pR[count+11251]; temp += pR[count+11231]; temp += pR[count+11209]; temp += pR[count+11201]; temp += pR[count+11177]; temp += pR[count+11171]; temp += pR[count+11153]; temp += pR[count+11141]; temp += pR[count+11117]; 
			case 28:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+11099]; temp += pR[count+11071]; temp += pR[count+11047]; temp += pR[count+11033]; temp += pR[count+11021]; temp += pR[count+10993]; temp += pR[count+10973]; temp += pR[count+10943]; temp += pR[count+10933]; temp += pR[count+10919]; 
				temp += pR[count+10897]; temp += pR[count+10879]; temp += pR[count+10867]; temp += pR[count+10841]; temp += pR[count+10783]; temp += pR[count+10763]; temp += pR[count+10751]; temp += pR[count+10733]; temp += pR[count+10721]; temp += pR[count+10687]; 
			case 27:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+10669]; temp += pR[count+10657]; temp += pR[count+10649]; temp += pR[count+10637]; temp += pR[count+10621]; temp += pR[count+10609]; temp += pR[count+10603]; temp += pR[count+10597]; temp += pR[count+10571]; temp += pR[count+10561]; 
				temp += pR[count+10553]; temp += pR[count+10499]; temp += pR[count+10481]; temp += pR[count+10453]; temp += pR[count+10439]; temp += pR[count+10417]; temp += pR[count+10399]; temp += pR[count+10391]; temp += pR[count+10363]; temp += pR[count+10343]; 
			case 26:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+10321]; temp += pR[count+10301]; temp += pR[count+10261]; temp += pR[count+10253]; temp += pR[count+10231]; temp += pR[count+10211]; temp += pR[count+10189]; temp += pR[count+10159]; temp += pR[count+10139]; temp += pR[count+10099]; 
				temp += pR[count+10093]; temp += pR[count+10079]; temp += pR[count+10039]; temp += pR[count+9983]; temp += pR[count+9973]; temp += pR[count+9953]; temp += pR[count+9929]; temp += pR[count+9911]; temp += pR[count+9893]; temp += pR[count+9871]; 
			case 25:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+9851]; temp += pR[count+9817]; temp += pR[count+9773]; temp += pR[count+9763]; temp += pR[count+9721]; temp += pR[count+9703]; temp += pR[count+9697]; temp += pR[count+9673]; temp += pR[count+9641]; temp += pR[count+9631]; 
				temp += pR[count+9619]; temp += pR[count+9607]; temp += pR[count+9581]; temp += pR[count+9559]; temp += pR[count+9547]; temp += pR[count+9521]; temp += pR[count+9493]; temp += pR[count+9487]; temp += pR[count+9469]; temp += pR[count+9449]; 
			case 24:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+9433]; temp += pR[count+9413]; temp += pR[count+9403]; temp += pR[count+9391]; temp += pR[count+9371]; temp += pR[count+9349]; temp += pR[count+9323]; temp += pR[count+9311]; temp += pR[count+9301]; temp += pR[count+9277]; 
				temp += pR[count+9257]; temp += pR[count+9239]; temp += pR[count+9227]; temp += pR[count+9221]; temp += pR[count+9169]; temp += pR[count+9161]; temp += pR[count+9151]; temp += pR[count+9137]; temp += pR[count+9109]; temp += pR[count+9101]; 
			case 23:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+9089]; temp += pR[count+9071]; temp += pR[count+9043]; temp += pR[count+9019]; temp += pR[count+9007]; temp += pR[count+8971]; temp += pR[count+8963]; temp += pR[count+8929]; temp += pR[count+8917]; temp += pR[count+8893]; 
				temp += pR[count+8863]; temp += pR[count+8849]; temp += pR[count+8837]; temp += pR[count+8803]; temp += pR[count+8779]; temp += pR[count+8761]; temp += pR[count+8753]; temp += pR[count+8731]; temp += pR[count+8699]; temp += pR[count+8677]; 
			case 22:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+8669]; temp += pR[count+8657]; temp += pR[count+8647]; temp += pR[count+8623]; temp += pR[count+8599]; temp += pR[count+8581]; temp += pR[count+8569]; temp += pR[count+8563]; temp += pR[count+8539]; temp += pR[count+8521]; 
				temp += pR[count+8507]; temp += pR[count+8497]; temp += pR[count+8473]; temp += pR[count+8437]; temp += pR[count+8411]; temp += pR[count+8387]; temp += pR[count+8371]; temp += pR[count+8363]; temp += pR[count+8339]; temp += pR[count+8303]; 
			case 21:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+8287]; temp += pR[count+8273]; temp += pR[count+8233]; temp += pR[count+8221]; temp += pR[count+8209]; temp += pR[count+8201]; temp += pR[count+8173]; temp += pR[count+8161]; temp += pR[count+8143]; temp += pR[count+8117]; 
				temp += pR[count+8107]; temp += pR[count+8083]; temp += pR[count+8069]; temp += pR[count+8059]; temp += pR[count+8023]; temp += pR[count+8009]; temp += pR[count+7991]; temp += pR[count+7943]; temp += pR[count+7927]; temp += pR[count+7909]; 
			case 20:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+7879]; temp += pR[count+7871]; temp += pR[count+7841]; temp += pR[count+7831]; temp += pR[count+7813]; temp += pR[count+7799]; temp += pR[count+7783]; temp += pR[count+7747]; temp += pR[count+7733]; temp += pR[count+7709]; 
				temp += pR[count+7673]; temp += pR[count+7661]; temp += pR[count+7633]; temp += pR[count+7627]; temp += pR[count+7607]; temp += pR[count+7597]; temp += pR[count+7561]; temp += pR[count+7543]; temp += pR[count+7529]; temp += pR[count+7513]; 
			case 19:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+7481]; temp += pR[count+7447]; temp += pR[count+7429]; temp += pR[count+7409]; temp += pR[count+7397]; temp += pR[count+7369]; temp += pR[count+7363]; temp += pR[count+7319]; temp += pR[count+7291]; temp += pR[count+7277]; 
				temp += pR[count+7267]; temp += pR[count+7249]; temp += pR[count+7243]; temp += pR[count+7237]; temp += pR[count+7187]; temp += pR[count+7171]; temp += pR[count+7151]; temp += pR[count+7117]; temp += pR[count+7099]; temp += pR[count+7087]; 
			case 18:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+7069]; temp += pR[count+7061]; temp += pR[count+7039]; temp += pR[count+6997]; temp += pR[count+6983]; temp += pR[count+6971]; temp += pR[count+6953]; temp += pR[count+6941]; temp += pR[count+6919]; temp += pR[count+6913]; 
				temp += pR[count+6907]; temp += pR[count+6859]; temp += pR[count+6847]; temp += pR[count+6829]; temp += pR[count+6823]; temp += pR[count+6809]; temp += pR[count+6787]; temp += pR[count+6763]; temp += pR[count+6749]; temp += pR[count+6731]; 
			case 17:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+6707]; temp += pR[count+6677]; temp += pR[count+6653]; temp += pR[count+6641]; temp += pR[count+6623]; temp += pR[count+6607]; temp += pR[count+6581]; temp += pR[count+6563]; temp += pR[count+6553]; temp += pR[count+6539]; 
				temp += pR[count+6511]; temp += pR[count+6499]; temp += pR[count+6487]; temp += pR[count+6469]; temp += pR[count+6457]; temp += pR[count+6449]; temp += pR[count+6439]; temp += pR[count+6413]; temp += pR[count+6397]; temp += pR[count+6373]; 
			case 16:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+6353]; temp += pR[count+6329]; temp += pR[count+6317]; temp += pR[count+6301]; temp += pR[count+6281]; temp += pR[count+6263]; temp += pR[count+6253]; temp += pR[count+6233]; temp += pR[count+6197]; temp += pR[count+6163]; 
				temp += pR[count+6137]; temp += pR[count+6127]; temp += pR[count+6109]; temp += pR[count+6077]; temp += pR[count+6067]; temp += pR[count+6049]; temp += pR[count+6031]; temp += pR[count+6011]; temp += pR[count+6001]; temp += pR[count+5977]; 
			case 15:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+5959]; temp += pR[count+5947]; temp += pR[count+5933]; temp += pR[count+5921]; temp += pR[count+5911]; temp += pR[count+5897]; temp += pR[count+5879]; temp += pR[count+5849]; temp += pR[count+5833]; temp += pR[count+5813]; 
				temp += pR[count+5807]; temp += pR[count+5773]; temp += pR[count+5743]; temp += pR[count+5737]; temp += pR[count+5729]; temp += pR[count+5717]; temp += pR[count+5699]; temp += pR[count+5669]; temp += pR[count+5641]; temp += pR[count+5627]; 
			case 14:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+5611]; temp += pR[count+5599]; temp += pR[count+5587]; temp += pR[count+5569]; temp += pR[count+5549]; temp += pR[count+5521]; temp += pR[count+5503]; temp += pR[count+5483]; temp += pR[count+5461]; temp += pR[count+5429]; 
				temp += pR[count+5401]; temp += pR[count+5377]; temp += pR[count+5353]; temp += pR[count+5333]; temp += pR[count+5323]; temp += pR[count+5267]; temp += pR[count+5261]; temp += pR[count+5237]; temp += pR[count+5213]; temp += pR[count+5191]; 
			case 13:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+5179]; temp += pR[count+5147]; temp += pR[count+5113]; temp += pR[count+5101]; temp += pR[count+5087]; temp += pR[count+5081]; temp += pR[count+5059]; temp += pR[count+5053]; temp += pR[count+5021]; temp += pR[count+5009]; 
				temp += pR[count+4979]; temp += pR[count+4961]; temp += pR[count+4939]; temp += pR[count+4931]; temp += pR[count+4903]; temp += pR[count+4891]; temp += pR[count+4877]; temp += pR[count+4871]; temp += pR[count+4861]; temp += pR[count+4849]; 
			case 12:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+4813]; temp += pR[count+4799]; temp += pR[count+4777]; temp += pR[count+4757]; temp += pR[count+4733]; temp += pR[count+4703]; temp += pR[count+4679]; temp += pR[count+4657]; temp += pR[count+4643]; temp += pR[count+4631]; 
				temp += pR[count+4601]; temp += pR[count+4591]; temp += pR[count+4577]; temp += pR[count+4547]; temp += pR[count+4517]; temp += pR[count+4507]; temp += pR[count+4489]; temp += pR[count+4471]; temp += pR[count+4439]; temp += pR[count+4421]; 
			case 11:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+4399]; temp += pR[count+4367]; temp += pR[count+4357]; temp += pR[count+4343]; temp += pR[count+4331]; temp += pR[count+4307]; temp += pR[count+4289]; temp += pR[count+4279]; temp += pR[count+4243]; temp += pR[count+4223]; 
				temp += pR[count+4213]; temp += pR[count+4187]; temp += pR[count+4181]; temp += pR[count+4159]; temp += pR[count+4147]; temp += pR[count+4133]; temp += pR[count+4103]; temp += pR[count+4097]; temp += pR[count+4087]; temp += pR[count+4073]; 
			case 10:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+4033]; temp += pR[count+4009]; temp += pR[count+4001]; temp += pR[count+3979]; temp += pR[count+3961]; temp += pR[count+3937]; temp += pR[count+3919]; temp += pR[count+3893]; temp += pR[count+3877]; temp += pR[count+3851]; 
				temp += pR[count+3833]; temp += pR[count+3821]; temp += pR[count+3799]; temp += pR[count+3791]; temp += pR[count+3769]; temp += pR[count+3757]; temp += pR[count+3733]; temp += pR[count+3721]; temp += pR[count+3707]; temp += pR[count+3691]; 
			case 9:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+3677]; temp += pR[count+3667]; temp += pR[count+3653]; temp += pR[count+3631]; temp += pR[count+3613]; temp += pR[count+3593]; temp += pR[count+3581]; temp += pR[count+3559]; temp += pR[count+3553]; temp += pR[count+3523]; 
				temp += pR[count+3503]; temp += pR[count+3469]; temp += pR[count+3457]; temp += pR[count+3443]; temp += pR[count+3431]; temp += pR[count+3419]; temp += pR[count+3391]; temp += pR[count+3377]; temp += pR[count+3341]; temp += pR[count+3329]; 
			case 8:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+3299]; temp += pR[count+3289]; temp += pR[count+3281]; temp += pR[count+3253]; temp += pR[count+3229]; temp += pR[count+3221]; temp += pR[count+3191]; temp += pR[count+3167]; temp += pR[count+3151]; temp += pR[count+3139]; 
				temp += pR[count+3121]; temp += pR[count+3113]; temp += pR[count+3103]; temp += pR[count+3083]; temp += pR[count+3067]; temp += pR[count+3043]; temp += pR[count+3013]; temp += pR[count+3007]; temp += pR[count+2993]; temp += pR[count+2981]; 
			case 7:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+2963]; temp += pR[count+2957]; temp += pR[count+2911]; temp += pR[count+2897]; temp += pR[count+2879]; temp += pR[count+2861]; temp += pR[count+2837]; temp += pR[count+2801]; temp += pR[count+2777]; temp += pR[count+2767]; 
				temp += pR[count+2731]; temp += pR[count+2707]; temp += pR[count+2683]; temp += pR[count+2671]; temp += pR[count+2663]; temp += pR[count+2647]; temp += pR[count+2623]; temp += pR[count+2603]; temp += pR[count+2579]; temp += pR[count+2563]; 
			case 6:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+2533]; temp += pR[count+2509]; temp += pR[count+2497]; temp += pR[count+2479]; temp += pR[count+2459]; temp += pR[count+2441]; temp += pR[count+2431]; temp += pR[count+2419]; temp += pR[count+2377]; temp += pR[count+2353]; 
				temp += pR[count+2341]; temp += pR[count+2329]; temp += pR[count+2299]; temp += pR[count+2293]; temp += pR[count+2267]; temp += pR[count+2251]; temp += pR[count+2239]; temp += pR[count+2227]; temp += pR[count+2183]; temp += pR[count+2167]; 
			case 5:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+2153]; temp += pR[count+2129]; temp += pR[count+2117]; temp += pR[count+2083]; temp += pR[count+2069]; temp += pR[count+2047]; temp += pR[count+2039]; temp += pR[count+2027]; temp += pR[count+2011]; temp += pR[count+1969]; 
				temp += pR[count+1951]; temp += pR[count+1919]; temp += pR[count+1909]; temp += pR[count+1891]; temp += pR[count+1871]; temp += pR[count+1847]; temp += pR[count+1837]; temp += pR[count+1819]; temp += pR[count+1807]; temp += pR[count+1787]; 
			case 4:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+1777]; temp += pR[count+1747]; temp += pR[count+1711]; temp += pR[count+1693]; temp += pR[count+1679]; temp += pR[count+1657]; temp += pR[count+1639]; temp += pR[count+1633]; temp += pR[count+1609]; temp += pR[count+1579]; 
				temp += pR[count+1549]; temp += pR[count+1523]; temp += pR[count+1513]; temp += pR[count+1501]; temp += pR[count+1483]; temp += pR[count+1469]; temp += pR[count+1451]; temp += pR[count+1439]; temp += pR[count+1427]; temp += pR[count+1403]; 
			case 3:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+1373]; temp += pR[count+1357]; temp += pR[count+1343]; temp += pR[count+1331]; temp += pR[count+1319]; temp += pR[count+1291]; temp += pR[count+1279]; temp += pR[count+1271]; temp += pR[count+1249]; temp += pR[count+1241]; 
				temp += pR[count+1217]; temp += pR[count+1187]; temp += pR[count+1147]; temp += pR[count+1121]; temp += pR[count+1109]; temp += pR[count+1087]; temp += pR[count+1067]; temp += pR[count+1051]; temp += pR[count+1037]; temp += pR[count+1019]; 
			case 2:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+1009]; temp += pR[count+991]; temp += pR[count+971]; temp += pR[count+961]; temp += pR[count+947]; temp += pR[count+923]; temp += pR[count+913]; temp += pR[count+899]; temp += pR[count+869]; temp += pR[count+857]; 
				temp += pR[count+829]; temp += pR[count+811]; temp += pR[count+803]; temp += pR[count+793]; temp += pR[count+767]; temp += pR[count+727]; temp += pR[count+713]; temp += pR[count+691]; temp += pR[count+671]; temp += pR[count+653]; 
			case 1:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+643]; temp += pR[count+619]; temp += pR[count+613]; temp += pR[count+601]; temp += pR[count+593]; temp += pR[count+577]; temp += pR[count+557]; temp += pR[count+529]; temp += pR[count+521]; temp += pR[count+503]; 
				temp += pR[count+491]; temp += pR[count+463]; temp += pR[count+457]; temp += pR[count+421]; temp += pR[count+407]; temp += pR[count+389]; temp += pR[count+379]; temp += pR[count+361]; temp += pR[count+341]; temp += pR[count+331]; 
			case 0:
				temp = (int32_t)((double)temp/decay);
				temp += pR[count+313]; temp += pR[count+289]; temp += pR[count+271]; temp += pR[count+239]; temp += pR[count+223]; temp += pR[count+209]; temp += pR[count+191]; temp += pR[count+181]; temp += pR[count+151]; temp += pR[count+143]; 
				temp += pR[count+121]; temp += pR[count+109]; temp += pR[count+73]; temp += pR[count+47]; temp += pR[count+41]; temp += pR[count+19]; temp += pR[count+11]; 
				//Highest Offset = 12679: gcount limit is 12680, Reset() goes to 25360, definition in .h must be p[25361]
		}
		inputSampleR = (inputSampleR*(1-wet))+((double)(temp/(8388352.0*scale))*wet);
		//end R
		
		gcount--;
		
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
