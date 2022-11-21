/*
*	File:		BussColors4.cpp
*	
*	Version:	1.0
* 
*	Created:	5/26/17
*	
*	Copyright:  Copyright © 2017 Airwindows, Airwindows uses the MIT license
* 
*	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
*				consideration of your agreement to the following terms, and your use, installation, modification 
*				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
*				not agree with these terms, please do not use, install, modify or redistribute this Apple 
*				software.
*
*				In consideration of your agreement to abide by the following terms, and subject to these terms, 
*				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
*				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
*				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
*				redistribute the Apple Software in its entirety and without modifications, you must retain this 
*				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
*				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
*				endorse or promote products derived from the Apple Software without specific prior written 
*				permission from Apple.  Except as expressly stated in this notice, no other rights or 
*				licenses, express or implied, are granted by Apple herein, including but not limited to any 
*				patent rights that may be infringed by your derivative works or by other works in which the 
*				Apple Software may be incorporated.
*
*				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
*				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
*				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
*				OR IN COMBINATION WITH YOUR PRODUCTS.
*
*				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
*				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
*				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
*				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
*				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
/*=============================================================================
	BussColors4.cpp
	
=============================================================================*/
#include "BussColors4.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, BussColors4)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BussColors4::BussColors4
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BussColors4::BussColors4(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BussColors4::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BussColors4::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_One)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_Dark,
			kMenuItem_Rock,
			kMenuItem_Lush,
			kMenuItem_Vibe,
			kMenuItem_Holo,
			kMenuItem_Punch,
			kMenuItem_Steel,
			kMenuItem_Tube,
		};
		*outStrings = CFArrayCreate (
									 NULL,
									 (const void **) strings,
									 (sizeof (strings) / sizeof (strings [0])),
									 NULL
									 );
		return noErr;
	}
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BussColors4::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BussColors4::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
 			case kParam_One:
                AUBase::FillInParameterName (outParameterInfo, kParameterOneName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 1;
                outParameterInfo.maxValue = 8;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -18.0;
                outParameterInfo.maxValue = 18.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -18.0;
                outParameterInfo.maxValue = 18.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
                break;
            case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFour;
                break;
			default:
                result = kAudioUnitErr_InvalidParameter;
                break;
		}
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
    


	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BussColors4::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BussColors4::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BussColors4::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BussColors4::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BussColors4::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BussColors4::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BussColors4EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BussColors4::BussColors4Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BussColors4::BussColors4Kernel::Reset()
{
	for (int count = 0; count < 174; count++) b[count] = 0;
	for (int count = 0; count < 99; count++) d[count] = 0;
	for (int count = 0; count < 34; count++) c[count] = count; //initial setup for 44.1K
	g[1] = pow(10.0, -5.2 / 14.0); //dark
	g[2] = pow(10.0, -6.2 / 14.0); //rock
	g[3] = pow(10.0, -2.9 / 14.0); //lush
	g[4] = pow(10.0, -1.1 / 14.0); //vibe
	g[5] = pow(10.0, -5.1 / 14.0); //holo
	g[6] = pow(10.0, -3.6 / 14.0); //punch
	g[7] = pow(10.0, -2.3 / 14.0); //steel
	g[8] = pow(10.0, -2.9 / 14.0); //tube
	//preset gains for models
	outg[1] = pow(10.0, -0.3 / 14.0); //dark
	outg[2] = pow(10.0, 0.5 / 14.0); //rock
	outg[3] = pow(10.0, -0.7 / 14.0); //lush
	outg[4] = pow(10.0, -0.6 / 14.0); //vibe
	outg[5] = pow(10.0, -0.2 / 14.0); //holo
	outg[6] = pow(10.0, 0.3 / 14.0); //punch
	outg[7] = pow(10.0, 0.1 / 14.0); //steel
	outg[8] = pow(10.0, 0.9 / 14.0); //tube
	//preset gains for models
	control = 0;
	gcount = 0;
	slowdyn = 0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BussColors4::BussColors4Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BussColors4::BussColors4Kernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	if (overallscale < 1.0) overallscale = 1.0;
	if (overallscale > 4.5) overallscale = 4.5;
	
	const int maxConvolutionBufferSize = (int)(34.0 * overallscale); //we won't use more of the buffer than we have to
	for (int count = 0; count < 34; count++) c[count] = (int)(count * overallscale); //assign conv taps

	double inputSample;
	Float64 drySample;
	Float64 applyconv;
	
	int offsetA = 3;
	Float64 dynamicconv = 3.0;
	Float64 gain = 0.436;
	Float64 outgain = 1.0;
	Float64 bridgerectifier;
	
	int console = (int) GetParameter( kParam_One );
	switch (console)
	{
		case 1: offsetA = 4; gain = g[1]; outgain = outg[1]; break; //Dark (Focusrite)
		case 2: offsetA = 3; gain = g[2]; outgain = outg[2]; break; //Rock (SSL)
		case 3: offsetA = 5; gain = g[3]; outgain = outg[3]; break; //Lush (Neve)
		case 4: offsetA = 8; gain = g[4]; outgain = outg[4]; break; //Vibe (Elation)
		case 5: offsetA = 5; gain = g[5]; outgain = outg[5]; break; //Holo (Precision 8)
		case 6: offsetA = 7; gain = g[6]; outgain = outg[6]; break; //Punch (API)
		case 7: offsetA = 7; gain = g[7]; outgain = outg[7]; break; //Steel (Calibre)
		case 8: offsetA = 6; gain = g[8]; outgain = outg[8]; break; //Tube (Manley)
	}
	offsetA = (int)(offsetA * overallscale); //we extend the sag buffer too, at high sample rates
	if (offsetA < 3) offsetA = 3; //are we getting divides by zero?
	gain *= pow(10.0,GetParameter( kParam_Two )/14.0); //add adjustment factor
	outgain *= pow(10.0,(GetParameter( kParam_Three )+3.3)/14.0); //add adjustment factor
	Float64 wet = GetParameter( kParam_Four );
	//removed unnecessary dry variable
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		drySample = inputSample;
		
		if (gain != 1.0) {
			inputSample *= gain;
		}
		
		bridgerectifier = fabs(inputSample);
		
		slowdyn *= 0.999;
		slowdyn += bridgerectifier;
		if (slowdyn > 1.5) {slowdyn = 1.5;}
		//before the iron bar- fry console for crazy behavior
		dynamicconv = 2.5 + slowdyn;
		
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		
		
		if (gcount < 0 || gcount > 44) gcount = 44;
		d[gcount+44] = d[gcount] = fabs(inputSample);
		control += (d[gcount] / offsetA);
		control -= (d[gcount+offsetA] / offsetA);
		control -= 0.000001;				
		if (control < 0) control = 0;
		if (control > 100) control = 100;
		gcount--;
		applyconv = (control / offsetA) * dynamicconv;
		//now we have a 'sag' style average to apply to the conv
		
		//now the convolution
		for (int count = maxConvolutionBufferSize; count > 0; --count) {b[count] = b[count-1];} //was 173
		//we're only doing assigns, and it saves us an add inside the convolution calculation
		//therefore, we'll just assign everything one step along and have our buffer that way.
		b[0] = inputSample;

		switch (console)
		{
			case 1:
				//begin Cider (Focusrite) MCI
				inputSample += (b[c[1]] * (0.61283288942201319  + (0.00024011410669522*applyconv)));
				inputSample -= (b[c[2]] * (0.24036380659761222  - (0.00020789518206241*applyconv)));
				inputSample += (b[c[3]] * (0.09104669761717916  + (0.00012829642741548*applyconv)));
				inputSample -= (b[c[4]] * (0.02378290768554025  - (0.00017673646470440*applyconv)));
				inputSample -= (b[c[5]] * (0.02832818490275965  - (0.00013536187747384*applyconv)));
				inputSample += (b[c[6]] * (0.03268797679215937  + (0.00015035126653359*applyconv)));
				inputSample -= (b[c[7]] * (0.04024464202655586  - (0.00015034923056735*applyconv)));
				inputSample += (b[c[8]] * (0.01864890074318696  + (0.00014513281680642*applyconv)));
				inputSample -= (b[c[9]] * (0.01632731954100322  - (0.00015509089075614*applyconv)));
				inputSample -= (b[c[10]] * (0.00318907090555589  - (0.00014784812076550*applyconv)));
				inputSample -= (b[c[11]] * (0.00208573465221869  - (0.00015350520779465*applyconv)));
				inputSample -= (b[c[12]] * (0.00907033901519614  - (0.00015442964157250*applyconv)));
				inputSample -= (b[c[13]] * (0.00199458794148013  - (0.00015595640046297*applyconv)));
				inputSample -= (b[c[14]] * (0.00705979153201755  - (0.00015730069418051*applyconv)));
				inputSample -= (b[c[15]] * (0.00429488975412722  - (0.00015743697943505*applyconv)));
				inputSample -= (b[c[16]] * (0.00497724878704936  - (0.00016014760011861*applyconv)));
				inputSample -= (b[c[17]] * (0.00506059305562353  - (0.00016194824072466*applyconv)));
				inputSample -= (b[c[18]] * (0.00483432223285621  - (0.00016329050124225*applyconv)));
				inputSample -= (b[c[19]] * (0.00495100420886005  - (0.00016297509798749*applyconv)));
				inputSample -= (b[c[20]] * (0.00489319520555115  - (0.00016472839684661*applyconv)));
				inputSample -= (b[c[21]] * (0.00489177657970308  - (0.00016791875866630*applyconv)));
				inputSample -= (b[c[22]] * (0.00487900894707044  - (0.00016755993898534*applyconv)));
				inputSample -= (b[c[23]] * (0.00486234009335561  - (0.00016968157345446*applyconv)));
				inputSample -= (b[c[24]] * (0.00485737490288736  - (0.00017180713324431*applyconv)));
				inputSample -= (b[c[25]] * (0.00484106070563455  - (0.00017251073661092*applyconv)));
				inputSample -= (b[c[26]] * (0.00483219429408410  - (0.00017321683790891*applyconv)));
				inputSample -= (b[c[27]] * (0.00482013597437550  - (0.00017392186866488*applyconv)));
				inputSample -= (b[c[28]] * (0.00480949628051497  - (0.00017569098775602*applyconv)));
				inputSample -= (b[c[29]] * (0.00479992055604049  - (0.00017746046369449*applyconv)));
				inputSample -= (b[c[30]] * (0.00478750757986987  - (0.00017745630047554*applyconv)));
				inputSample -= (b[c[31]] * (0.00477828651185740  - (0.00017958043287604*applyconv)));
				inputSample -= (b[c[32]] * (0.00476906544384494  - (0.00018170456527653*applyconv)));
				inputSample -= (b[c[33]] * (0.00475700712413634  - (0.00018099144598088*applyconv)));
				//end Cider (Focusrite) MCI
				break;
			case 2:
				//begin Rock (SSL) conv
				inputSample += (b[c[1]] * (0.67887916185274055  + (0.00068787552301086*applyconv)));
				inputSample -= (b[c[2]] * (0.25671050678827934  + (0.00017691749454490*applyconv)));
				inputSample += (b[c[3]] * (0.15135839896615280  + (0.00007481480365043*applyconv)));
				inputSample -= (b[c[4]] * (0.11813512969090802  + (0.00005191138121359*applyconv)));
				inputSample += (b[c[5]] * (0.08329104347166429  + (0.00001871054659794*applyconv)));
				inputSample -= (b[c[6]] * (0.07663817456103936  + (0.00002751359071705*applyconv)));
				inputSample += (b[c[7]] * (0.05477586152148759  + (0.00000744843212679*applyconv)));
				inputSample -= (b[c[8]] * (0.05547314737187786  + (0.00001025289931145*applyconv)));
				inputSample += (b[c[9]] * (0.03822948356540711  - (0.00000249791561457*applyconv)));
				inputSample -= (b[c[10]] * (0.04199383340841713  - (0.00000067328840674*applyconv)));
				inputSample += (b[c[11]] * (0.02695796542339694  - (0.00000796704606548*applyconv)));
				inputSample -= (b[c[12]] * (0.03228715059431878  - (0.00000579711816722*applyconv)));
				inputSample += (b[c[13]] * (0.01846929689819187  - (0.00000984017804950*applyconv)));
				inputSample -= (b[c[14]] * (0.02528050435045951  - (0.00000701189792484*applyconv)));
				inputSample += (b[c[15]] * (0.01207844846859765  - (0.00001522630289356*applyconv)));
				inputSample -= (b[c[16]] * (0.01894464378378515  - (0.00001205456372080*applyconv)));
				inputSample += (b[c[17]] * (0.00667804407593324  - (0.00001343604283817*applyconv)));
				inputSample -= (b[c[18]] * (0.01408418045473130  - (0.00001246443581504*applyconv)));
				inputSample += (b[c[19]] * (0.00228696509481569  - (0.00001506764046927*applyconv)));
				inputSample -= (b[c[20]] * (0.01006277891348454  - (0.00000970723079112*applyconv)));
				inputSample -= (b[c[21]] * (0.00132368373546377  + (0.00001188847238761*applyconv)));
				inputSample -= (b[c[22]] * (0.00676615715578373  - (0.00001209129844861*applyconv)));
				inputSample -= (b[c[23]] * (0.00426288438418556  + (0.00001286836943559*applyconv)));
				inputSample -= (b[c[24]] * (0.00408897698639688  - (0.00001102542567911*applyconv)));
				inputSample -= (b[c[25]] * (0.00662040619382751  + (0.00001206328529063*applyconv)));
				inputSample -= (b[c[26]] * (0.00196101294183599  - (0.00000950703614981*applyconv)));
				inputSample -= (b[c[27]] * (0.00845620581010342  + (0.00001279970295678*applyconv)));
				inputSample -= (b[c[28]] * (0.00032595215043616  - (0.00000920518241371*applyconv)));
				inputSample -= (b[c[29]] * (0.00982957737435458  + (0.00001177745362317*applyconv)));
				inputSample += (b[c[30]] * (0.00086920573760513  + (0.00000913758382404*applyconv)));
				inputSample -= (b[c[31]] * (0.01079020871452061  + (0.00000900750153697*applyconv)));
				inputSample += (b[c[32]] * (0.00167613606334460  + (0.00000732769151038*applyconv)));
				inputSample -= (b[c[33]] * (0.01138050011044332  + (0.00000946908207442*applyconv)));
				//end Rock (SSL) conv
				break;
			case 3:
				//begin Lush (Neve) conv
				inputSample += (b[c[1]] * (0.20641602693167951  - (0.00078952185394898*applyconv)));
				inputSample -= (b[c[2]] * (0.07601816702459827  + (0.00022786334179951*applyconv)));
				inputSample += (b[c[3]] * (0.03929765560019285  - (0.00054517993246352*applyconv)));
				inputSample += (b[c[4]] * (0.00298333157711103  - (0.00033083756545638*applyconv)));
				inputSample -= (b[c[5]] * (0.00724006282304610  + (0.00045483683460812*applyconv)));
				inputSample += (b[c[6]] * (0.03073108963506036  - (0.00038190060537423*applyconv)));
				inputSample -= (b[c[7]] * (0.02332434692533051  + (0.00040347288688932*applyconv)));
				inputSample += (b[c[8]] * (0.03792606869061214  - (0.00039673687335892*applyconv)));
				inputSample -= (b[c[9]] * (0.02437059376675688  + (0.00037221210539535*applyconv)));
				inputSample += (b[c[10]] * (0.03416764311979521  - (0.00040314850796953*applyconv)));
				inputSample -= (b[c[11]] * (0.01761669868102127  + (0.00035989484330131*applyconv)));
				inputSample += (b[c[12]] * (0.02538237753523052  - (0.00040149119125394*applyconv)));
				inputSample -= (b[c[13]] * (0.00770737340728377  + (0.00035462118723555*applyconv)));
				inputSample += (b[c[14]] * (0.01580706228482803  - (0.00037563141307594*applyconv)));
				inputSample += (b[c[15]] * (0.00055119240005586  - (0.00035409299268971*applyconv)));
				inputSample += (b[c[16]] * (0.00818552143438768  - (0.00036507661042180*applyconv)));
				inputSample += (b[c[17]] * (0.00661842703548304  - (0.00034550528559056*applyconv)));
				inputSample += (b[c[18]] * (0.00362447476272098  - (0.00035553012761240*applyconv)));
				inputSample += (b[c[19]] * (0.00957098027225745  - (0.00034091691045338*applyconv)));
				inputSample += (b[c[20]] * (0.00193621774016660  - (0.00034554529131668*applyconv)));
				inputSample += (b[c[21]] * (0.01005433027357935  - (0.00033878223153845*applyconv)));
				inputSample += (b[c[22]] * (0.00221712428802004  - (0.00033481410137711*applyconv)));
				inputSample += (b[c[23]] * (0.00911255639207995  - (0.00033263425232666*applyconv)));
				inputSample += (b[c[24]] * (0.00339667169034909  - (0.00032634428038430*applyconv)));
				inputSample += (b[c[25]] * (0.00774096948249924  - (0.00032599868802996*applyconv)));
				inputSample += (b[c[26]] * (0.00463907626773794  - (0.00032131993173361*applyconv)));
				inputSample += (b[c[27]] * (0.00658222997260378  - (0.00032014977430211*applyconv)));
				inputSample += (b[c[28]] * (0.00550347079924993  - (0.00031557153256653*applyconv)));
				inputSample += (b[c[29]] * (0.00588754981375325  - (0.00032041307242303*applyconv)));
				inputSample += (b[c[30]] * (0.00590293898419892  - (0.00030457857428714*applyconv)));
				inputSample += (b[c[31]] * (0.00558952010441800  - (0.00030448053548086*applyconv)));
				inputSample += (b[c[32]] * (0.00598183557634295  - (0.00030715064323181*applyconv)));
				inputSample += (b[c[33]] * (0.00555223929714115  - (0.00030319367948553*applyconv)));
				//end Lush (Neve) conv
				break;
			case 4:
				//begin Elation (LA2A) vibe
				inputSample -= (b[c[1]] * (0.25867935358656502  - (0.00045755657070112*applyconv)));
				inputSample += (b[c[2]] * (0.11509367290253694  - (0.00017494270657228*applyconv)));
				inputSample -= (b[c[3]] * (0.06709853575891785  - (0.00058913102597723*applyconv)));
				inputSample += (b[c[4]] * (0.01871006356851681  - (0.00003387358004645*applyconv)));
				inputSample -= (b[c[5]] * (0.00794797957360465  - (0.00044224784691203*applyconv)));
				inputSample -= (b[c[6]] * (0.01956921817394220  - (0.00006718936750076*applyconv)));
				inputSample += (b[c[7]] * (0.01682120257195205  + (0.00032857446292230*applyconv)));
				inputSample -= (b[c[8]] * (0.03401069039824205  - (0.00013634182872897*applyconv)));
				inputSample += (b[c[9]] * (0.02369950268232634  + (0.00023112685751657*applyconv)));
				inputSample -= (b[c[10]] * (0.03477071178117132  - (0.00018029792231600*applyconv)));
				inputSample += (b[c[11]] * (0.02024369717958201  + (0.00017337813374202*applyconv)));
				inputSample -= (b[c[12]] * (0.02819087729102172  - (0.00021438538665420*applyconv)));
				inputSample += (b[c[13]] * (0.01147946743141303  + (0.00014424066034649*applyconv)));
				inputSample -= (b[c[14]] * (0.01894777011468867  - (0.00021549146262408*applyconv)));
				inputSample += (b[c[15]] * (0.00301370330346873  + (0.00013527460148394*applyconv)));
				inputSample -= (b[c[16]] * (0.01067147835815486  - (0.00020960689910868*applyconv)));
				inputSample -= (b[c[17]] * (0.00402715397506384  - (0.00014421582712470*applyconv)));
				inputSample -= (b[c[18]] * (0.00502221703392005  - (0.00019805767015024*applyconv)));
				inputSample -= (b[c[19]] * (0.00808788533308497  - (0.00016095444141931*applyconv)));
				inputSample -= (b[c[20]] * (0.00232696588842683  - (0.00018384470981829*applyconv)));
				inputSample -= (b[c[21]] * (0.00943950821324531  - (0.00017098987347593*applyconv)));
				inputSample -= (b[c[22]] * (0.00193709517200834  - (0.00018151995939591*applyconv)));
				inputSample -= (b[c[23]] * (0.00899713952612659  - (0.00017385835059948*applyconv)));
				inputSample -= (b[c[24]] * (0.00280584331659089  - (0.00017742164162470*applyconv)));
				inputSample -= (b[c[25]] * (0.00780381001954970  - (0.00018002500755708*applyconv)));
				inputSample -= (b[c[26]] * (0.00400370310490333  - (0.00017471691087957*applyconv)));
				inputSample -= (b[c[27]] * (0.00661527728186928  - (0.00018137323370347*applyconv)));
				inputSample -= (b[c[28]] * (0.00496545526864518  - (0.00017681872601767*applyconv)));
				inputSample -= (b[c[29]] * (0.00580728820997532  - (0.00018186220389790*applyconv)));
				inputSample -= (b[c[30]] * (0.00549309984725666  - (0.00017722985399075*applyconv)));
				inputSample -= (b[c[31]] * (0.00542194777529239  - (0.00018486900185338*applyconv)));
				inputSample -= (b[c[32]] * (0.00565992080998939  - (0.00018005824393118*applyconv)));
				inputSample -= (b[c[33]] * (0.00532121562846656  - (0.00018643189636216*applyconv)));
				//end Elation (LA2A)
				break;
			case 5:
				//begin Precious (Precision 8) Holo
				inputSample += (b[c[1]] * (0.59188440274551890  - (0.00008361469668405*applyconv)));
				inputSample -= (b[c[2]] * (0.24439750948076133  + (0.00002651678396848*applyconv)));
				inputSample += (b[c[3]] * (0.14109876103205621  - (0.00000840487181372*applyconv)));
				inputSample -= (b[c[4]] * (0.10053507128157971  + (0.00001768100964598*applyconv)));
				inputSample += (b[c[5]] * (0.05859287880626238  - (0.00000361398065989*applyconv)));
				inputSample -= (b[c[6]] * (0.04337406889823660  + (0.00000735941182117*applyconv)));
				inputSample += (b[c[7]] * (0.01589900680531097  + (0.00000207347387987*applyconv)));
				inputSample -= (b[c[8]] * (0.01087234854973281  + (0.00000732123412029*applyconv)));
				inputSample -= (b[c[9]] * (0.00845782429679176  - (0.00000133058605071*applyconv)));
				inputSample += (b[c[10]] * (0.00662278586618295  - (0.00000424594730611*applyconv)));
				inputSample -= (b[c[11]] * (0.02000592193760155  + (0.00000632896879068*applyconv)));
				inputSample += (b[c[12]] * (0.01321157777167565  - (0.00001421171592570*applyconv)));
				inputSample -= (b[c[13]] * (0.02249955362988238  + (0.00000163937127317*applyconv)));
				inputSample += (b[c[14]] * (0.01196492077581504  - (0.00000535385220676*applyconv)));
				inputSample -= (b[c[15]] * (0.01905917427000097  + (0.00000121672882030*applyconv)));
				inputSample += (b[c[16]] * (0.00761909482108073  - (0.00000326242895115*applyconv)));
				inputSample -= (b[c[17]] * (0.01362744780256239  + (0.00000359274216003*applyconv)));
				inputSample += (b[c[18]] * (0.00200183122683721  - (0.00000089207452791*applyconv)));
				inputSample -= (b[c[19]] * (0.00833042637239315  + (0.00000946767677294*applyconv)));
				inputSample -= (b[c[20]] * (0.00258481175207224  - (0.00000087429351464*applyconv)));
				inputSample -= (b[c[21]] * (0.00459744479712244  - (0.00000049519758701*applyconv)));
				inputSample -= (b[c[22]] * (0.00534277030993820  + (0.00000397547847155*applyconv)));
				inputSample -= (b[c[23]] * (0.00272332919605675  + (0.00000040077229097*applyconv)));
				inputSample -= (b[c[24]] * (0.00637243782359372  - (0.00000139419072176*applyconv)));
				inputSample -= (b[c[25]] * (0.00233001590327504  + (0.00000420129915747*applyconv)));
				inputSample -= (b[c[26]] * (0.00623296727793041  + (0.00000019010664856*applyconv)));
				inputSample -= (b[c[27]] * (0.00276177096376805  + (0.00000580301901385*applyconv)));
				inputSample -= (b[c[28]] * (0.00559184754866264  + (0.00000080597287792*applyconv)));
				inputSample -= (b[c[29]] * (0.00343180144395919  - (0.00000243701142085*applyconv)));
				inputSample -= (b[c[30]] * (0.00493325428861701  + (0.00000300985740900*applyconv)));
				inputSample -= (b[c[31]] * (0.00396140827680823  - (0.00000051459681789*applyconv)));
				inputSample -= (b[c[32]] * (0.00448497879902493  + (0.00000744412841743*applyconv)));
				inputSample -= (b[c[33]] * (0.00425146888772076  - (0.00000082346016542*applyconv)));
				//end Precious (Precision 8) Holo
				break;
			case 6:
				//begin Punch (API) conv
				inputSample += (b[c[1]] * (0.09299870608542582  - (0.00009582362368873*applyconv)));
				inputSample -= (b[c[2]] * (0.11947847710741009  - (0.00004500891602770*applyconv)));
				inputSample += (b[c[3]] * (0.09071606264761795  + (0.00005639498984741*applyconv)));
				inputSample -= (b[c[4]] * (0.08561982770836980  - (0.00004964855606916*applyconv)));
				inputSample += (b[c[5]] * (0.06440549220820363  + (0.00002428052139507*applyconv)));
				inputSample -= (b[c[6]] * (0.05987991812840746  + (0.00000101867082290*applyconv)));
				inputSample += (b[c[7]] * (0.03980233135839382  + (0.00003312430049041*applyconv)));
				inputSample -= (b[c[8]] * (0.03648402630896925  - (0.00002116186381142*applyconv)));
				inputSample += (b[c[9]] * (0.01826860869525248  + (0.00003115110025396*applyconv)));
				inputSample -= (b[c[10]] * (0.01723968622495364  - (0.00002450634121718*applyconv)));
				inputSample += (b[c[11]] * (0.00187588812316724  + (0.00002838206198968*applyconv)));
				inputSample -= (b[c[12]] * (0.00381796423957237  - (0.00003155815499462*applyconv)));
				inputSample -= (b[c[13]] * (0.00852092214496733  - (0.00001702651162392*applyconv)));
				inputSample += (b[c[14]] * (0.00315560292270588  + (0.00002547861676047*applyconv)));
				inputSample -= (b[c[15]] * (0.01258630914496868  - (0.00004555319243213*applyconv)));
				inputSample += (b[c[16]] * (0.00536435648963575  + (0.00001812393657101*applyconv)));
				inputSample -= (b[c[17]] * (0.01272975658159178  - (0.00004103775306121*applyconv)));
				inputSample += (b[c[18]] * (0.00403818975172755  + (0.00003764615492871*applyconv)));
				inputSample -= (b[c[19]] * (0.01042617366897483  - (0.00003605210426041*applyconv)));
				inputSample += (b[c[20]] * (0.00126599583390057  + (0.00004305458668852*applyconv)));
				inputSample -= (b[c[21]] * (0.00747876207688339  - (0.00003731207018977*applyconv)));
				inputSample -= (b[c[22]] * (0.00149873689175324  - (0.00005086601800791*applyconv)));
				inputSample -= (b[c[23]] * (0.00503221309488033  - (0.00003636086782783*applyconv)));
				inputSample -= (b[c[24]] * (0.00342998224655821  - (0.00004103091180506*applyconv)));
				inputSample -= (b[c[25]] * (0.00355585977903117  - (0.00003698982145400*applyconv)));
				inputSample -= (b[c[26]] * (0.00437201792934817  - (0.00002720235666939*applyconv)));
				inputSample -= (b[c[27]] * (0.00299217874451556  - (0.00004446954727956*applyconv)));
				inputSample -= (b[c[28]] * (0.00457924652487249  - (0.00003859065778860*applyconv)));
				inputSample -= (b[c[29]] * (0.00298182934892027  - (0.00002064710931733*applyconv)));
				inputSample -= (b[c[30]] * (0.00438838441540584  - (0.00005223008424866*applyconv)));
				inputSample -= (b[c[31]] * (0.00323984218794705  - (0.00003397987535887*applyconv)));
				inputSample -= (b[c[32]] * (0.00407693981307314  - (0.00003935772436894*applyconv)));
				inputSample -= (b[c[33]] * (0.00350435348467321  - (0.00005525463935338*applyconv)));
				//end Punch (API) conv
				break;
			case 7:
				//begin Calibre (?) steel
				inputSample -= (b[c[1]] * (0.23505923670562212  - (0.00028312859289245*applyconv)));
				inputSample += (b[c[2]] * (0.08188436704577637  - (0.00008817721351341*applyconv)));
				inputSample -= (b[c[3]] * (0.05075798481700617  - (0.00018817166632483*applyconv)));
				inputSample -= (b[c[4]] * (0.00455811821873093  + (0.00001922902995296*applyconv)));
				inputSample -= (b[c[5]] * (0.00027610521433660  - (0.00013252525469291*applyconv)));
				inputSample -= (b[c[6]] * (0.03529246280346626  - (0.00002772989223299*applyconv)));
				inputSample += (b[c[7]] * (0.01784111585586136  + (0.00010230276997291*applyconv)));
				inputSample -= (b[c[8]] * (0.04394950700298298  - (0.00005910607126944*applyconv)));
				inputSample += (b[c[9]] * (0.01990770780547606  + (0.00007640328340556*applyconv)));
				inputSample -= (b[c[10]] * (0.04073629569741782  - (0.00007712327117090*applyconv)));
				inputSample += (b[c[11]] * (0.01349648572795252  + (0.00005959130575917*applyconv)));
				inputSample -= (b[c[12]] * (0.03191590248003717  - (0.00008418000575151*applyconv)));
				inputSample += (b[c[13]] * (0.00348795527924766  + (0.00005489156318238*applyconv)));
				inputSample -= (b[c[14]] * (0.02198496281481767  - (0.00008471601187581*applyconv)));
				inputSample -= (b[c[15]] * (0.00504771152505089  - (0.00005525060587917*applyconv)));
				inputSample -= (b[c[16]] * (0.01391075698598491  - (0.00007929630732607*applyconv)));
				inputSample -= (b[c[17]] * (0.01142762504081717  - (0.00005967036737742*applyconv)));
				inputSample -= (b[c[18]] * (0.00893541815021255  - (0.00007535697758141*applyconv)));
				inputSample -= (b[c[19]] * (0.01459704973464936  - (0.00005969199602841*applyconv)));
				inputSample -= (b[c[20]] * (0.00694755135226282  - (0.00006930127097865*applyconv)));
				inputSample -= (b[c[21]] * (0.01516695630808575  - (0.00006365800069826*applyconv)));
				inputSample -= (b[c[22]] * (0.00705917318113651  - (0.00006497209096539*applyconv)));
				inputSample -= (b[c[23]] * (0.01420501209177591  - (0.00006555654576113*applyconv)));
				inputSample -= (b[c[24]] * (0.00815905656808701  - (0.00006105622534761*applyconv)));
				inputSample -= (b[c[25]] * (0.01274326525552961  - (0.00006542652857017*applyconv)));
				inputSample -= (b[c[26]] * (0.00937146927845488  - (0.00006051267868722*applyconv)));
				inputSample -= (b[c[27]] * (0.01146573981165209  - (0.00006381511607749*applyconv)));
				inputSample -= (b[c[28]] * (0.01021294359409007  - (0.00005930397856398*applyconv)));
				inputSample -= (b[c[29]] * (0.01065217095323532  - (0.00006371505438319*applyconv)));
				inputSample -= (b[c[30]] * (0.01058751196699751  - (0.00006042857480233*applyconv)));
				inputSample -= (b[c[31]] * (0.01026557827762401  - (0.00006007776163871*applyconv)));
				inputSample -= (b[c[32]] * (0.01060929183604604  - (0.00006114703012726*applyconv)));
				inputSample -= (b[c[33]] * (0.01014533525058528  - (0.00005963567932887*applyconv)));
				//end Calibre (?)
				break;
			case 8:
				//begin Tube (Manley) conv
				inputSample -= (b[c[1]] * (0.20641602693167951  - (0.00078952185394898*applyconv)));
				inputSample += (b[c[2]] * (0.07601816702459827  + (0.00022786334179951*applyconv)));
				inputSample -= (b[c[3]] * (0.03929765560019285  - (0.00054517993246352*applyconv)));
				inputSample -= (b[c[4]] * (0.00298333157711103  - (0.00033083756545638*applyconv)));
				inputSample += (b[c[5]] * (0.00724006282304610  + (0.00045483683460812*applyconv)));
				inputSample -= (b[c[6]] * (0.03073108963506036  - (0.00038190060537423*applyconv)));
				inputSample += (b[c[7]] * (0.02332434692533051  + (0.00040347288688932*applyconv)));
				inputSample -= (b[c[8]] * (0.03792606869061214  - (0.00039673687335892*applyconv)));
				inputSample += (b[c[9]] * (0.02437059376675688  + (0.00037221210539535*applyconv)));
				inputSample -= (b[c[10]] * (0.03416764311979521  - (0.00040314850796953*applyconv)));
				inputSample += (b[c[11]] * (0.01761669868102127  + (0.00035989484330131*applyconv)));
				inputSample -= (b[c[12]] * (0.02538237753523052  - (0.00040149119125394*applyconv)));
				inputSample += (b[c[13]] * (0.00770737340728377  + (0.00035462118723555*applyconv)));
				inputSample -= (b[c[14]] * (0.01580706228482803  - (0.00037563141307594*applyconv)));
				inputSample -= (b[c[15]] * (0.00055119240005586  - (0.00035409299268971*applyconv)));
				inputSample -= (b[c[16]] * (0.00818552143438768  - (0.00036507661042180*applyconv)));
				inputSample -= (b[c[17]] * (0.00661842703548304  - (0.00034550528559056*applyconv)));
				inputSample -= (b[c[18]] * (0.00362447476272098  - (0.00035553012761240*applyconv)));
				inputSample -= (b[c[19]] * (0.00957098027225745  - (0.00034091691045338*applyconv)));
				inputSample -= (b[c[20]] * (0.00193621774016660  - (0.00034554529131668*applyconv)));
				inputSample -= (b[c[21]] * (0.01005433027357935  - (0.00033878223153845*applyconv)));
				inputSample -= (b[c[22]] * (0.00221712428802004  - (0.00033481410137711*applyconv)));
				inputSample -= (b[c[23]] * (0.00911255639207995  - (0.00033263425232666*applyconv)));
				inputSample -= (b[c[24]] * (0.00339667169034909  - (0.00032634428038430*applyconv)));
				inputSample -= (b[c[25]] * (0.00774096948249924  - (0.00032599868802996*applyconv)));
				inputSample -= (b[c[26]] * (0.00463907626773794  - (0.00032131993173361*applyconv)));
				inputSample -= (b[c[27]] * (0.00658222997260378  - (0.00032014977430211*applyconv)));
				inputSample -= (b[c[28]] * (0.00550347079924993  - (0.00031557153256653*applyconv)));
				inputSample -= (b[c[29]] * (0.00588754981375325  - (0.00032041307242303*applyconv)));
				inputSample -= (b[c[30]] * (0.00590293898419892  - (0.00030457857428714*applyconv)));
				inputSample -= (b[c[31]] * (0.00558952010441800  - (0.00030448053548086*applyconv)));
				inputSample -= (b[c[32]] * (0.00598183557634295  - (0.00030715064323181*applyconv)));
				inputSample -= (b[c[33]] * (0.00555223929714115  - (0.00030319367948553*applyconv)));
				//end Tube (Manley) conv
				break;
		}
				
		bridgerectifier = fabs(inputSample);
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSample > 0) inputSample -= bridgerectifier;
		else inputSample += bridgerectifier;
		
		if (outgain != 1.0) {
			inputSample *= outgain;
		}
		
		if (wet !=1.0) {
			inputSample = (inputSample * wet) + (drySample * (1.0-wet));
		}
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

