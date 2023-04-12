/*
*	File:		Ditherbox.cpp
*	
*	Version:	1.0
* 
*	Created:	1/1/09
*	
*	Copyright:  Copyright © 2009 Airwindows, Airwindows uses the MIT license
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
	Ditherbox.h
	
=============================================================================*/
#include "Ditherbox.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Ditherbox)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ditherbox::Ditherbox
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Ditherbox::Ditherbox(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
        
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ditherbox::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Ditherbox::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_One)) //ID must be actual name of parameter identifier, not number
	{
			if (outStrings == NULL) return noErr;
			CFStringRef strings [] =
			{
				kMenuItem_Truncate,
				kMenuItem_Flat,
				kMenuItem_TPDF,
				kMenuItem_Paul,
				kMenuItem_DoublePaul,
				kMenuItem_Tape,
				kMenuItem_Quadratic,
				kMenuItem_TenNines,
				kMenuItem_Contingent,
				kMenuItem_Naturalize,
				kMenuItem_NJAD,
				kMenuItem_TruncateHR,
				kMenuItem_FlatHR,
				kMenuItem_TPDFHR,
				kMenuItem_PaulHR,
				kMenuItem_DoublePaulHR,
				kMenuItem_TapeHR,
				kMenuItem_QuadraticHR,
				kMenuItem_TenNinesHR,
				kMenuItem_ContingentHR,
				kMenuItem_NaturalizeHR,
				kMenuItem_NJADHR,
				kMenuItem_SlewOnly,
				kMenuItem_SubsOnly,
				kMenuItem_Silhouette,
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
//	Ditherbox::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Ditherbox::GetParameterInfo(AudioUnitScope		inScope,
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
				outParameterInfo.minValue = kTruncate;
				outParameterInfo.maxValue = kSilhouette;
				outParameterInfo.defaultValue = kDefaultValue_ParamOne;
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
//	Ditherbox::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Ditherbox::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ditherbox::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Ditherbox::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ditherbox::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Ditherbox::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____DitherboxEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ditherbox::DitherboxKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Ditherbox::DitherboxKernel::Reset()
{
	Position = 99999999;
	contingentErr = 0.0;
	byn[0] = 1000;
	byn[1] = 301;
	byn[2] = 176;
	byn[3] = 125;
	byn[4] = 97;
	byn[5] = 79;
	byn[6] = 67;
	byn[7] = 58;
	byn[8] = 51;
	byn[9] = 46;
	byn[10] = 1000;
	noiseShaping = 0.0;
	NSOdd = 0.0;
	NSEven = 0.0;
	prev = 0.0;
	ns[0] = 0;
	ns[1] = 0;
	ns[2] = 0;
	ns[3] = 0;
	ns[4] = 0;
	ns[5] = 0;
	ns[6] = 0;
	ns[7] = 0;
	ns[8] = 0;
	ns[9] = 0;
	ns[10] = 0;
	ns[11] = 0;
	ns[12] = 0;
	ns[13] = 0;
	ns[14] = 0;
	ns[15] = 0;
	lastSample = 0.0;
	outSample = 0.0;
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	iirSampleC = 0.0;
	iirSampleD = 0.0;
	iirSampleE = 0.0;
	iirSampleF = 0.0;
	iirSampleG = 0.0;
	iirSampleH = 0.0;
	iirSampleI = 0.0;
	iirSampleJ = 0.0;
	iirSampleK = 0.0;
	iirSampleL = 0.0;
	iirSampleM = 0.0;
	iirSampleN = 0.0;
	iirSampleO = 0.0;
	iirSampleP = 0.0;
	iirSampleQ = 0.0;
	iirSampleR = 0.0;
	iirSampleS = 0.0;
	iirSampleT = 0.0;
	iirSampleU = 0.0;
	iirSampleV = 0.0;
	iirSampleW = 0.0;
	iirSampleX = 0.0;
	iirSampleY = 0.0;
	iirSampleZ = 0.0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ditherbox::DitherboxKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Ditherbox::DitherboxKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, // for version 2 AudioUnits inNumChannels is always 1
                                                    bool			&ioSilence )
{

	
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	double contingentRnd;
	double absSample;
	double contingent;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	double iirAmount = 2250/44100.0;
	double gaintarget = 1.42;
	double gain;
	iirAmount /= overallscale;
	double altAmount = 1.0 - iirAmount;
	double inputSample;
	double outputSample;
	double silhouette;
	double smoother;
	double bridgerectifier;
	double benfordize;
	int hotbinA;
	int hotbinB;
	double totalA;
	double totalB;
	double randyConstant = 1.61803398874989484820458683436563811772030917980576;
	double omegaConstant = 0.56714329040978387299996866221035554975381578718651;
	double expConstant = 0.06598803584531253707679018759684642493857704825279;
	double trim = 2.302585092994045684017991; //natural logarithm of 10
	int dtype = (int) GetParameter( kParam_One ); // +1 for Reaper bug workaround
	bool highRes = false;
	bool dithering = true;
	
	Float32 drySample; //should be the same as what the native DAW buss is

	if (dtype > 11){highRes = true; dtype -= 11;}
	
	if (dtype > 11){dithering = false; highRes = false;}
	//follow up by switching high res back off for the monitoring
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		drySample = inputSample;
		
		sourceP += inNumChannels;
		
		if (dtype == 8) inputSample -= noiseShaping;
		
		if (dithering) inputSample *= 32768.0;
		//denormalizing as way of controlling insane detail boosting
		if (highRes) inputSample *= 256.0; //256 for 16/24 version
		
		switch (dtype)
		{
		case 1: 
		inputSample = floor(inputSample);
		//truncate
		break;
		
		case 2: 
		inputSample += (double(fpd)/UINT32_MAX);
		inputSample -= 0.5;
		inputSample = floor(inputSample);
		//flat dither
		break;
		
			case 3:
				inputSample += (double(fpd)/UINT32_MAX);
				inputSample += (double(fpd)/UINT32_MAX);
				inputSample -= 1.0;
				inputSample = floor(inputSample);
				//TPDF dither
				break;
				
			case 4:
				currentDither = (double(fpd)/UINT32_MAX);
				inputSample += currentDither;
				inputSample -= lastSample;
				inputSample = floor(inputSample);
				lastSample = currentDither;
				//Paul dither
				break;
				
			case 5:
				ns[9] = ns[8]; ns[8] = ns[7]; ns[7] = ns[6]; ns[6] = ns[5];
				ns[5] = ns[4]; ns[4] = ns[3]; ns[3] = ns[2]; ns[2] = ns[1];
				ns[1] = ns[0]; ns[0] = (double(fpd)/UINT32_MAX);
				
				currentDither  = (ns[0] * 0.061);
				currentDither -= (ns[1] * 0.11);
				currentDither += (ns[8] * 0.126);
				currentDither -= (ns[7] * 0.23);
				currentDither += (ns[2] * 0.25);
				currentDither -= (ns[3] * 0.43);
				currentDither += (ns[6] * 0.5);
				currentDither -= ns[5];
				currentDither += ns[4];
				//this sounds different from doing it in order of sample position
				//cumulative tiny errors seem to build up even at this buss depth
				//considerably more pronounced at 32 bit float.
				//Therefore we add the most significant components LAST.
				//trying to keep values on like exponents of the floating point value.
				inputSample += currentDither;
				
				inputSample = floor(inputSample);
				//DoublePaul dither
				break;
				
			case 6:
				currentDither = (double(fpd)/UINT32_MAX);
				inputSample += currentDither;
				inputSample -= ns[4];
				inputSample = floor(inputSample);
				ns[4] = ns[3];
				ns[3] = ns[2];
				ns[2] = ns[1];
				ns[1] = currentDither;
				//Tape dither
				break;
				
				
		case 7: 
		Position += 1;
		//Note- uses integer overflow as a 'mod' operator
		hotbinA = Position * Position;
		hotbinA = hotbinA % 170003; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 17011; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 1709; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 173; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 17;
		hotbinA *= 0.0635;
		if (flip) hotbinA = -hotbinA;
		inputSample += hotbinA;
		inputSample = floor(inputSample);
		//Quadratic dither
		break;

		case 8:
		absSample = ((double(fpd)/UINT32_MAX) - 0.5);
		ns[0] += absSample; ns[0] /= 2; absSample -= ns[0];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[1] += absSample; ns[1] /= 2; absSample -= ns[1];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[2] += absSample; ns[2] /= 2; absSample -= ns[2];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[3] += absSample; ns[3] /= 2; absSample -= ns[3];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[4] += absSample; ns[4] /= 2; absSample -= ns[4];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[5] += absSample; ns[5] /= 2; absSample -= ns[5];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[6] += absSample; ns[6] /= 2; absSample -= ns[6];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[7] += absSample; ns[7] /= 2; absSample -= ns[7];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[8] += absSample; ns[8] /= 2; absSample -= ns[8];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[9] += absSample; ns[9] /= 2; absSample -= ns[9];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[10] += absSample; ns[10] /= 2; absSample -= ns[10];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[11] += absSample; ns[11] /= 2; absSample -= ns[11];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[12] += absSample; ns[12] /= 2; absSample -= ns[12];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[13] += absSample; ns[13] /= 2; absSample -= ns[13];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[14] += absSample; ns[14] /= 2; absSample -= ns[14];
		absSample += ((double(fpd)/UINT32_MAX) - 0.5);
		ns[15] += absSample; ns[15] /= 2; absSample -= ns[15];
		//install noise and then shape it
		absSample += inputSample;
		
		//NSOdd /= 1.0001; //NSDensity
		
		if (NSOdd > 0) NSOdd -= 0.97;
		if (NSOdd < 0) NSOdd += 0.97;
		
		NSOdd -= (NSOdd * NSOdd * NSOdd * 0.475);

		NSOdd += prev;
		absSample += (NSOdd*0.475);
		prev = floor(absSample) - inputSample;
		inputSample = floor(absSample);
		//TenNines dither
		break;
		
			case 9: 
				if (inputSample > 0) inputSample += 0.383;
				if (inputSample < 0) inputSample -= 0.383;
				//adjusting to permit more information drug outta the noisefloor
				contingentRnd = (((double(fpd)/UINT32_MAX)+(double(fpd)/UINT32_MAX))-1.0) * randyConstant; //produce TPDF dist, scale
				contingentRnd -= contingentErr*omegaConstant; //include err
				absSample = fabs(inputSample);
				contingentErr = absSample - floor(absSample); //get next err
				contingent = contingentErr * 2.0; //scale of quantization levels
				if (contingent > 1.0) contingent = ((-contingent+2.0)*omegaConstant) + expConstant;
				else contingent = (contingent * omegaConstant) + expConstant;
				//zero is next to a quantization level, one is exactly between them
				if (flip) contingentRnd = (contingentRnd * (1.0-contingent)) + contingent + 0.5;
				else contingentRnd = (contingentRnd * (1.0-contingent)) - contingent + 0.5;
				inputSample += (contingentRnd * contingent);
				//Contingent Dither
				inputSample = floor(inputSample);
				//note: this does not dither for values exactly the same as 16 bit values-
				//which forces the dither to gate at 0.0. It goes to digital black,
				//and does a teeny parallel-compression thing when almost at digital black.
				break;
				
			case 10:
				if (inputSample > 0) inputSample += (0.3333333333);
				if (inputSample < 0) inputSample -= (0.3333333333);
				
				inputSample += (double(fpd)/UINT32_MAX)*0.6666666666;
				
				benfordize = floor(inputSample);
				while (benfordize >= 1.0) {benfordize /= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				hotbinA = floor(benfordize);
				//hotbin becomes the Benford bin value for this number floored
				totalA = 0;
				if ((hotbinA > 0) && (hotbinA < 10))
				{
					byn[hotbinA] += 1;
					totalA += (301-byn[1]);
					totalA += (176-byn[2]);
					totalA += (125-byn[3]);
					totalA += (97-byn[4]);
					totalA += (79-byn[5]);
					totalA += (67-byn[6]);
					totalA += (58-byn[7]);
					totalA += (51-byn[8]);
					totalA += (46-byn[9]);
					byn[hotbinA] -= 1;
				} else {hotbinA = 10;}
				//produce total number- smaller is closer to Benford real
				
				benfordize = ceil(inputSample);
				while (benfordize >= 1.0) {benfordize /= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				hotbinB = floor(benfordize);
				//hotbin becomes the Benford bin value for this number ceiled
				totalB = 0;
				if ((hotbinB > 0) && (hotbinB < 10))
				{
					byn[hotbinB] += 1;
					totalB += (301-byn[1]);
					totalB += (176-byn[2]);
					totalB += (125-byn[3]);
					totalB += (97-byn[4]);
					totalB += (79-byn[5]);
					totalB += (67-byn[6]);
					totalB += (58-byn[7]);
					totalB += (51-byn[8]);
					totalB += (46-byn[9]);
					byn[hotbinB] -= 1;
				} else {hotbinB = 10;}
				//produce total number- smaller is closer to Benford real
				
				if (totalA < totalB)
				{
					byn[hotbinA] += 1;
					inputSample = floor(inputSample);
				}
				else
				{
					byn[hotbinB] += 1;
					inputSample = ceil(inputSample);
				}
				//assign the relevant one to the delay line
				//and floor/ceil signal accordingly
				
				totalA = byn[1] + byn[2] + byn[3] + byn[4] + byn[5] + byn[6] + byn[7] + byn[8] + byn[9];
				totalA /= 1000;
				totalA = 1; // spotted by Laserbat: this 'scaling back' code doesn't. It always divides by the fallback of 1. Old NJAD doesn't scale back the things we're comparing against. Kept to retain known behavior, use the one in StudioTan and Monitoring for a tuned-as-intended NJAD.
				byn[1] /= totalA;
				byn[2] /= totalA;
				byn[3] /= totalA;
				byn[4] /= totalA;
				byn[5] /= totalA;
				byn[6] /= totalA;
				byn[7] /= totalA;
				byn[8] /= totalA;
				byn[9] /= totalA;
				byn[10] /= 2; //catchall for garbage data
				break;
				
			case 11: //this one is the Not Just Another Dither
				
				benfordize = floor(inputSample);
				while (benfordize >= 1.0) {benfordize /= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				hotbinA = floor(benfordize);
				//hotbin becomes the Benford bin value for this number floored
				totalA = 0;
				if ((hotbinA > 0) && (hotbinA < 10))
				{
					byn[hotbinA] += 1;
					totalA += (301-byn[1]);
					totalA += (176-byn[2]);
					totalA += (125-byn[3]);
					totalA += (97-byn[4]);
					totalA += (79-byn[5]);
					totalA += (67-byn[6]);
					totalA += (58-byn[7]);
					totalA += (51-byn[8]);
					totalA += (46-byn[9]);
					byn[hotbinA] -= 1;
				} else {hotbinA = 10;}
				//produce total number- smaller is closer to Benford real
				
				benfordize = ceil(inputSample);
				while (benfordize >= 1.0) {benfordize /= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				if (benfordize < 1.0) {benfordize *= 10;}
				hotbinB = floor(benfordize);
				//hotbin becomes the Benford bin value for this number ceiled
				totalB = 0;
				if ((hotbinB > 0) && (hotbinB < 10))
				{
					byn[hotbinB] += 1;
					totalB += (301-byn[1]);
					totalB += (176-byn[2]);
					totalB += (125-byn[3]);
					totalB += (97-byn[4]);
					totalB += (79-byn[5]);
					totalB += (67-byn[6]);
					totalB += (58-byn[7]);
					totalB += (51-byn[8]);
					totalB += (46-byn[9]);
					byn[hotbinB] -= 1;
				} else {hotbinB = 10;}
				//produce total number- smaller is closer to Benford real
				
				if (totalA < totalB)
				{
					byn[hotbinA] += 1;
					inputSample = floor(inputSample);
				}
				else
				{
					byn[hotbinB] += 1;
					inputSample = ceil(inputSample);
				}
				//assign the relevant one to the delay line
				//and floor/ceil signal accordingly
				
				totalA = byn[1] + byn[2] + byn[3] + byn[4] + byn[5] + byn[6] + byn[7] + byn[8] + byn[9];
				totalA /= 1000;
				totalA = 1; // spotted by Laserbat: this 'scaling back' code doesn't. It always divides by the fallback of 1. Old NJAD doesn't scale back the things we're comparing against. Kept to retain known behavior, use the one in StudioTan and Monitoring for a tuned-as-intended NJAD.
				byn[1] /= totalA;
				byn[2] /= totalA;
				byn[3] /= totalA;
				byn[4] /= totalA;
				byn[5] /= totalA;
				byn[6] /= totalA;
				byn[7] /= totalA;
				byn[8] /= totalA;
				byn[9] /= totalA;
				byn[10] /= 2; //catchall for garbage data
				break;
				
			case 12: 
			//slew only
			outputSample = (inputSample - lastSample)*trim;
			lastSample = inputSample;
			if (outputSample > 1.0) outputSample = 1.0;
			if (outputSample < -1.0) outputSample = -1.0;
			inputSample = outputSample;
		break;
				
		case 13: 
			//subs only
			gain = gaintarget;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			iirSampleA = (iirSampleA * altAmount) + (inputSample * iirAmount); inputSample = iirSampleA;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleB = (iirSampleB * altAmount) + (inputSample * iirAmount); inputSample = iirSampleB;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleC = (iirSampleC * altAmount) + (inputSample * iirAmount); inputSample = iirSampleC;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleD = (iirSampleD * altAmount) + (inputSample * iirAmount); inputSample = iirSampleD;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleE = (iirSampleE * altAmount) + (inputSample * iirAmount); inputSample = iirSampleE;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleF = (iirSampleF * altAmount) + (inputSample * iirAmount); inputSample = iirSampleF;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleG = (iirSampleG * altAmount) + (inputSample * iirAmount); inputSample = iirSampleG;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleH = (iirSampleH * altAmount) + (inputSample * iirAmount); inputSample = iirSampleH;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleI = (iirSampleI * altAmount) + (inputSample * iirAmount); inputSample = iirSampleI;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleJ = (iirSampleJ * altAmount) + (inputSample * iirAmount); inputSample = iirSampleJ;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleK = (iirSampleK * altAmount) + (inputSample * iirAmount); inputSample = iirSampleK;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleL = (iirSampleL * altAmount) + (inputSample * iirAmount); inputSample = iirSampleL;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleM = (iirSampleM * altAmount) + (inputSample * iirAmount); inputSample = iirSampleM;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleN = (iirSampleN * altAmount) + (inputSample * iirAmount); inputSample = iirSampleN;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleO = (iirSampleO * altAmount) + (inputSample * iirAmount); inputSample = iirSampleO;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleP = (iirSampleP * altAmount) + (inputSample * iirAmount); inputSample = iirSampleP;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleQ = (iirSampleQ * altAmount) + (inputSample * iirAmount); inputSample = iirSampleQ;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleR = (iirSampleR * altAmount) + (inputSample * iirAmount); inputSample = iirSampleR;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleS = (iirSampleS * altAmount) + (inputSample * iirAmount); inputSample = iirSampleS;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleT = (iirSampleT * altAmount) + (inputSample * iirAmount); inputSample = iirSampleT;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleU = (iirSampleU * altAmount) + (inputSample * iirAmount); inputSample = iirSampleU;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleV = (iirSampleV * altAmount) + (inputSample * iirAmount); inputSample = iirSampleV;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleW = (iirSampleW * altAmount) + (inputSample * iirAmount); inputSample = iirSampleW;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleX = (iirSampleX * altAmount) + (inputSample * iirAmount); inputSample = iirSampleX;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleY = (iirSampleY * altAmount) + (inputSample * iirAmount); inputSample = iirSampleY;
			inputSample *= gain; gain = ((gain-1)*0.75)+1;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			iirSampleZ = (iirSampleZ * altAmount) + (inputSample * iirAmount); inputSample = iirSampleZ;
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;				
		break;
				
		case 14: 
			//silhouette
			bridgerectifier = fabs(inputSample)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = 1.0-cos(bridgerectifier);
			if (inputSample > 0.0) inputSample = bridgerectifier;
			else inputSample = -bridgerectifier;
			
			silhouette = rand()/(double)RAND_MAX;
			silhouette -= 0.5;
			silhouette *= 2.0;
			silhouette *= fabs(inputSample);
			
			smoother = rand()/(double)RAND_MAX;
			smoother -= 0.5;
			smoother *= 2.0;
			smoother *= fabs(lastSample);
			lastSample = inputSample;
			
			silhouette += smoother;
			
			bridgerectifier = fabs(silhouette)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (silhouette > 0.0) silhouette = bridgerectifier;
			else silhouette = -bridgerectifier;
			
			inputSample = (silhouette + outSample) / 2.0;
			outSample = silhouette;
		break;
		}
		
		flip = !flip;
		//several dithers use this
		if (highRes) inputSample /= 256.0; //256 for 16/24 version
		if (dithering) inputSample /= 32768.0;
		
		if (dtype == 8) noiseShaping += inputSample - drySample;

		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		//pseudorandom number updater
		
		*destP = inputSample;
		destP += inNumChannels;
	}
}

