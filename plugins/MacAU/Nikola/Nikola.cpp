/*
*	File:		Nikola.cpp
*	
*	Version:	1.0
* 
*	Created:	1/11/20
*	
*	Copyright:  Copyright © 2020 Airwindows, Airwindows uses the MIT license
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
	Nikola.cpp
	
=============================================================================*/
#include "Nikola.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Nikola)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Nikola::Nikola
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Nikola::Nikola(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Nikola::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Nikola::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Nikola::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Nikola::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
			case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
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
//	Nikola::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Nikola::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Nikola::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Nikola::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Nikola::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Nikola::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____NikolaEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Nikola::NikolaKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Nikola::NikolaKernel::Reset()
{
	WasNegative = false;
	outlevel = 0.0;
	framenumber = 0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Nikola::NikolaKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Nikola::NikolaKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;

	Float64 decay = pow(GetParameter( kParam_One ),3)*32767.0;
	int HighsBoost = 16 - (int)(pow(GetParameter( kParam_One ),2)*16);
	Float64 wet = GetParameter( kParam_Two );
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;

		Float64 bridgerectifier = fabs(inputSample);
		if (bridgerectifier > outlevel) outlevel = bridgerectifier;
		else outlevel = ((outlevel * decay) + bridgerectifier) / (decay+1.0);
		//set up envelope for handling quieter stuff if desired
		
		framenumber += 1;
		//we're having the pulse wave start with a 0 if it's starting
		
		if (inputSample > 0) {if (WasNegative) framenumber = HighsBoost; WasNegative = false;}
		else WasNegative = true;
		//our crossover trigger. If framenumber is 0, we've just gone into the positive cycle of the wave.
		
		if ((framenumber > 0) && (framenumber < 469))
		{
			switch(framenumber)
			{
				case 0: inputSample = 0.0; break;
				case 1: inputSample = 0.043397916539811392; break;
				case 2: inputSample = 0.083011537904293956; break;
				case 3: inputSample = 0.115117486724753554; break;
				case 4: inputSample = 0.129278313547471546; break;
				case 5: inputSample = 0.126867351298867753; break;
				case 6: inputSample = 0.115941486480605482; break;
				case 7: inputSample = 0.117162226859645377; break;
				case 8: inputSample = 0.152472738388012313; break;
				case 9: inputSample = 0.226817019600817904; break;
				case 10: inputSample = 0.362687450407727319; break;
				case 11: inputSample = 0.548425960188909545; break;
				case 12: inputSample = 0.761967189741508233; break;
				case 13: inputSample = 0.963667118404492373; break;
				case 14: inputSample = 0.970747531815851383; break;
				case 15: inputSample = 0.978133130321970246; break;
				case 16: inputSample = 0.98234468462965796; break;
				case 17: inputSample = 0.779637645153965941; break;
				case 18: inputSample = 0.420765008431043475; break;
				case 19: inputSample = -0.006744709806817921; break;
				case 20: inputSample = -0.427998133602099762; break;
				case 21: inputSample = -0.776677468947111538; break;
				case 22: inputSample = -0.970656095499740634; break;
				case 23: inputSample = -0.976790435117343758; break;
				case 24: inputSample = -0.982924655522019242; break;
				case 25: inputSample = -0.913249825471968868; break;
				case 26: inputSample = -0.707521453558153102; break;
				case 27: inputSample = -0.498252815332194066; break;
				case 28: inputSample = -0.319930422566606754; break;
				case 29: inputSample = -0.204965528389538354; break;
				case 30: inputSample = -0.152533894619281685; break;
				case 31: inputSample = -0.14597229586901464; break;
				case 32: inputSample = -0.169685535370647372; break;
				case 33: inputSample = -0.198434448148747306; break;
				case 34: inputSample = -0.212503719443037281; break;
				case 35: inputSample = -0.200692817849971089; break;
				case 36: inputSample = -0.16089608542863254; break;
				case 37: inputSample = -0.098973075998413124; break;
				case 38: inputSample = -0.028993060853602797; break;
				case 39: inputSample = 0.031831282235480818; break;
				case 40: inputSample = 0.083896574679097863; break;
				case 41: inputSample = 0.115025931196325565; break;
				case 42: inputSample = 0.11221822832453382; break;
				case 43: inputSample = 0.082187538148442027; break;
				case 44: inputSample = 0.023194305627918334; break;
				case 45: inputSample = -0.057436788536942743; break;
				case 46: inputSample = -0.148383258117618433; break;
				case 47: inputSample = -0.230754026535538892; break;
				case 48: inputSample = -0.291364740058290439; break;
				case 49: inputSample = -0.32316550378399006; break;
				case 50: inputSample = -0.312972202406079369; break;
				case 51: inputSample = -0.270215055352641453; break;
				case 52: inputSample = -0.196267633975951511; break;
				case 53: inputSample = -0.109166377376323828; break;
				case 54: inputSample = -0.026582098604999018; break;
				case 55: inputSample = 0.038179132206488234; break;
				case 56: inputSample = 0.078281168935514386; break;
				case 57: inputSample = 0.090061432806176941; break;
				case 58: inputSample = 0.082523241752678; break;
				case 59: inputSample = 0.0721773478273873; break;
				case 60: inputSample = 0.060427483253273108; break;
				case 61: inputSample = 0.064822267830744337; break;
				case 62: inputSample = 0.081088871807306132; break;
				case 63: inputSample = 0.107609814180730609; break;
				case 64: inputSample = 0.136328208449354532; break;
				case 65: inputSample = 0.15158770161320842; break;
				case 66: inputSample = 0.151129923971068447; break;
				case 67: inputSample = 0.128881572924283566; break;
				case 68: inputSample = 0.094791920987884151; break;
				case 69: inputSample = 0.059725557535325172; break;
				case 70: inputSample = 0.034059133427228612; break;
				case 71: inputSample = 0.030366274567705312; break;
				case 72: inputSample = 0.044954360523087246; break;
				case 73: inputSample = 0.07425260647175512; break;
				case 74: inputSample = 0.103733963477278965; break;
				case 75: inputSample = 0.112737042985625785; break;
				case 76: inputSample = 0.086490767197485274; break;
				case 77: inputSample = 0.019135343867610705; break;
				case 78: inputSample = -0.083164249663991302; break;
				case 79: inputSample = -0.19562674527695556; break;
				case 80: inputSample = -0.283765511985839491; break;
				case 81: inputSample = -0.322799281670278104; break;
				case 82: inputSample = -0.288312769897763055; break;
				case 83: inputSample = -0.171516645939207207; break;
				case 84: inputSample = 0.015076262894375438; break;
				case 85: inputSample = 0.243449845691091654; break;
				case 86: inputSample = 0.469503902554704478; break;
				case 87: inputSample = 0.648131361202124112; break;
				case 88: inputSample = 0.74451036389538261; break;
				case 89: inputSample = 0.733553980567644315; break;
				case 90: inputSample = 0.618070271729483922; break;
				case 91: inputSample = 0.409137337107760901; break;
				case 92: inputSample = 0.135046431051362659; break;
				case 93: inputSample = -0.165931758705099741; break;
				case 94: inputSample = -0.451131521423688564; break;
				case 95: inputSample = -0.68655487990936015; break;
				case 96: inputSample = -0.842262699514450652; break;
				case 97: inputSample = -0.900889709921262383; break;
				case 98: inputSample = -0.856484563356120554; break;
				case 99: inputSample = -0.716158310952787924; break;
				case 100: inputSample = -0.500877526360057446; break;
				case 101: inputSample = -0.240825253875545597; break;
				case 102: inputSample = 0.024720350314645832; break;
				case 103: inputSample = 0.264263707579577012; break;
				case 104: inputSample = 0.44478343302743617; break;
				case 105: inputSample = 0.540826732116458708; break;
				case 106: inputSample = 0.542444213118686513; break;
				case 107: inputSample = 0.451497624325083169; break;
				case 108: inputSample = 0.278241423345439054; break;
				case 109: inputSample = 0.048921885967894527; break;
				case 110: inputSample = -0.203348047387310493; break;
				case 111: inputSample = -0.445638189718009059; break;
				case 112: inputSample = -0.64224140808557395; break;
				case 113: inputSample = -0.765202270958281305; break;
				case 114: inputSample = -0.791509702978301477; break;
				case 115: inputSample = -0.713472682118900248; break;
				case 116: inputSample = -0.537011918431348656; break;
				case 117: inputSample = -0.286664770386059253; break;
				case 118: inputSample = 0.001312295907773064; break;
				case 119: inputSample = 0.277692090174871065; break;
				case 120: inputSample = 0.500450148015076146; break;
				case 121: inputSample = 0.634519986762901778; break;
				case 122: inputSample = 0.666290231979125402; break;
				case 123: inputSample = 0.602291963904538097; break;
				case 124: inputSample = 0.46706230258369702; break;
				case 125: inputSample = 0.301069626072267837; break;
				case 126: inputSample = 0.147223435545213166; break;
				case 127: inputSample = 0.037049947355876335; break;
				case 128: inputSample = -0.013611493651844931; break;
				case 129: inputSample = -0.00317404419812625; break;
				case 130: inputSample = 0.051943218406018246; break;
				case 131: inputSample = 0.123662788590960415; break;
				case 132: inputSample = 0.190804582354503011; break;
				case 133: inputSample = 0.234874025315408796; break;
				case 134: inputSample = 0.243846586314279606; break;
				case 135: inputSample = 0.221018383587450795; break;
				case 136: inputSample = 0.177162570192876984; break;
				case 137: inputSample = 0.126470610675679801; break;
				case 138: inputSample = 0.080417464598834185; break;
				case 139: inputSample = 0.042940138897671433; break;
				case 140: inputSample = 0.016022455901058993; break;
				case 141: inputSample = -0.009033717230445352; break;
				case 142: inputSample = -0.03940011101077312; break;
				case 143: inputSample = -0.075076725439924399; break;
				case 144: inputSample = -0.117406613360698353; break;
				case 145: inputSample = -0.168007017349467547; break;
				case 146: inputSample = -0.220163984534440221; break;
				case 147: inputSample = -0.262280123675954546; break;
				case 148: inputSample = -0.287580325670339143; break;
				case 149: inputSample = -0.293714665287942212; break;
				case 150: inputSample = -0.282361660549943605; break;
				case 151: inputSample = -0.254314673489791643; break;
				case 152: inputSample = -0.219767243911252269; break;
				case 153: inputSample = -0.186287842951445126; break;
				case 154: inputSample = -0.157478012367320869; break;
				case 155: inputSample = -0.1383120307776117; break;
				case 156: inputSample = -0.129919321458784848; break;
				case 157: inputSample = -0.131048506309396739; break;
				case 158: inputSample = -0.136084179585863912; break;
				case 159: inputSample = -0.142035408146611003; break;
				case 160: inputSample = -0.148047554513382446; break;
				case 161: inputSample = -0.147711850909146486; break;
				case 162: inputSample = -0.140387289421979505; break;
				case 163: inputSample = -0.126775914982757115; break;
				case 164: inputSample = -0.108525488677327892; break;
				case 165: inputSample = -0.087436960203558539; break;
				case 166: inputSample = -0.065585349779961638; break;
				case 167: inputSample = -0.045076553773308596; break;
				case 168: inputSample = -0.026612617114475014; break;
				case 169: inputSample = -0.012787493895993006; break;
				case 170: inputSample = -0.005066191785638076; break;
				case 171: inputSample = -0.001739674252754382; break;
				case 172: inputSample = -0.001709155743278385; break;
				case 173: inputSample = -0.002258488913846334; break;
				case 174: inputSample = -0.001281896610614424; break;
				case 175: inputSample = 0.005188146611224708; break;
				case 176: inputSample = 0.018250307092806787; break;
				case 177: inputSample = 0.038148613697012237; break;
				case 178: inputSample = 0.065676786096072262; break;
				case 179: inputSample = 0.097691179388103885; break;
				case 180: inputSample = 0.128820535905331574; break;
				case 181: inputSample = 0.156012885487227998; break;
				case 182: inputSample = 0.177498273797112943; break;
				case 183: inputSample = 0.189003990295419172; break;
				case 184: inputSample = 0.18885139774803919; break;
				case 185: inputSample = 0.17816968100558489; break;
				case 186: inputSample = 0.156806485946531571; break;
				case 187: inputSample = 0.127172536393627744; break;
				case 188: inputSample = 0.09524969863002411; break;
				case 189: inputSample = 0.066470267342448194; break;
				case 190: inputSample = 0.043977768219855336; break;
				case 191: inputSample = 0.032014393292336796; break;
				case 192: inputSample = 0.033570837275612657; break;
				case 193: inputSample = 0.04724336794671468; break;
				case 194: inputSample = 0.070163006989043847; break;
				case 195: inputSample = 0.096043179876400028; break;
				case 196: inputSample = 0.117650642224188973; break;
				case 197: inputSample = 0.127172536393627744; break;
				case 198: inputSample = 0.120427826586504708; break;
				case 199: inputSample = 0.09720288323648793; break;
				case 200: inputSample = 0.059634002006897183; break;
				case 201: inputSample = 0.0144048556859035; break;
				case 202: inputSample = -0.029817060609454722; break;
				case 203: inputSample = -0.064639275986205724; break;
				case 204: inputSample = -0.083866175381939231; break;
				case 205: inputSample = -0.085514174893643075; break;
				case 206: inputSample = -0.071841763435468692; break;
				case 207: inputSample = -0.050936226805627698; break;
				case 208: inputSample = -0.028443727683034846; break;
				case 209: inputSample = -0.010468087175817219; break;
				case 210: inputSample = -0.002105896366466348; break;
				case 211: inputSample = -0.003662340349742205; break;
				case 212: inputSample = -0.01220764221594906; break;
				case 213: inputSample = -0.022248351046479778; break;
				case 214: inputSample = -0.029847579118930719; break;
				case 215: inputSample = -0.031343105296182221; break;
				case 216: inputSample = -0.024750988036439187; break;
				case 217: inputSample = -0.013550456632892937; break;
				case 218: inputSample = -0.000854637477950463; break;
				case 219: inputSample = 0.010071227340311899; break;
				case 220: inputSample = 0.016663344600054933; break;
				case 221: inputSample = 0.016846455656910918; break;
				case 222: inputSample = 0.01190233790887173; break;
				case 223: inputSample = 0.006378368480788599; break;
				case 224: inputSample = 0.004242072817468795; break;
				case 225: inputSample = 0.007934931676992095; break;
				case 226: inputSample = 0.01995934362346263; break;
				case 227: inputSample = 0.041017472800683609; break;
				case 228: inputSample = 0.067507896664632097; break;
				case 229: inputSample = 0.095280217139500106; break;
				case 230: inputSample = 0.119237604716940823; break;
				case 231: inputSample = 0.134863319994506653; break;
				case 232: inputSample = 0.139074874302194285; break;
				case 233: inputSample = 0.130285424360179453; break;
				case 234: inputSample = 0.109227295182958456; break;
				case 235: inputSample = 0.077731716552018798; break;
				case 236: inputSample = 0.038667428358104189; break;
				case 237: inputSample = -0.002929896122318273; break;
				case 238: inputSample = -0.042360406429944847; break;
				case 239: inputSample = -0.075229317987304381; break;
				case 240: inputSample = -0.097386113505661273; break;
				case 241: inputSample = -0.105595711767632161; break;
				case 242: inputSample = -0.098606853884701154; break;
				case 243: inputSample = -0.07764028023590816; break;
				case 244: inputSample = -0.046632997756584457; break;
				case 245: inputSample = -0.011505716498001124; break;
				case 246: inputSample = 0.021668380154118472; break;
				case 247: inputSample = 0.048555663854182557; break;
				case 248: inputSample = 0.061800816179692977; break;
				case 249: inputSample = 0.060244372196417123; break;
				case 250: inputSample = 0.046663397053743089; break;
				case 251: inputSample = 0.024995016899929806; break;
				case 252: inputSample = 0.000061037019257179; break;
				case 253: inputSample = -0.022461980612811756; break;
				case 254: inputSample = -0.036927992530289708; break;
				case 255: inputSample = -0.038942214155705521; break;
				case 256: inputSample = -0.026917802209234988; break;
				case 257: inputSample = -0.002624711027558301; break;
				case 258: inputSample = 0.029603311830805383; break;
				case 259: inputSample = 0.064456045717032381; break;
				case 260: inputSample = 0.096531476028015983; break;
				case 261: inputSample = 0.120824567209692674; break;
				case 262: inputSample = 0.132910135388042833; break;
				case 263: inputSample = 0.129858165227515476; break;
				case 264: inputSample = 0.111302673040253916; break;
				case 265: inputSample = 0.080173316523026214; break;
				case 266: inputSample = 0.042482361255531474; break;
				case 267: inputSample = 0.006225775933408613; break;
				case 268: inputSample = -0.020295166440015958; break;
				case 269: inputSample = -0.030457949308450662; break;
				case 270: inputSample = -0.020844499610583905; break;
				case 271: inputSample = 0.007202368236640523; break;
				case 272: inputSample = 0.048006330683614609; break;
				case 273: inputSample = 0.092655506111636698; break;
				case 274: inputSample = 0.13120097964445937; break;
				case 275: inputSample = 0.154975256165044095; break;
				case 276: inputSample = 0.15885122608142338; break;
				case 277: inputSample = 0.142798251671193588; break;
				case 278: inputSample = 0.111333191549729912; break;
				case 279: inputSample = 0.071811125713675344; break;
				case 280: inputSample = 0.032472170934476755; break;
				case 281: inputSample = 0.000732444227729118; break;
				case 282: inputSample = -0.018616648418836111; break;
				case 283: inputSample = -0.024903580583819172; break;
				case 284: inputSample = -0.020722425572679917; break;
				case 285: inputSample = -0.010681716742149201; break;
				case 286: inputSample = 0.000030518509781182; break;
				case 287: inputSample = 0.006958220160832545; break;
				case 288: inputSample = 0.007538071840876492; break;
				case 289: inputSample = 0.001831110568865017; break;
				case 290: inputSample = -0.008179198965117431; break;
				case 291: inputSample = -0.019501685193640029; break;
				case 292: inputSample = -0.028443727683034846; break;
				case 293: inputSample = -0.031678808900418187; break;
				case 294: inputSample = -0.027467135379802936; break;
				case 295: inputSample = -0.016083612132328343; break;
				case 296: inputSample = 0.000518814661397137; break;
				case 297: inputSample = 0.019593121509750663; break;
				case 298: inputSample = 0.037782391583300268; break;
				case 299: inputSample = 0.052126329462874231; break;
				case 300: inputSample = 0.060946297914365059; break;
				case 301: inputSample = 0.063601527451704457; break;
				case 302: inputSample = 0.059939187101657153; break;
				case 303: inputSample = 0.050386774422742392; break;
				case 304: inputSample = 0.036714243751640369; break;
				case 305: inputSample = 0.021729417173070469; break;
				case 306: inputSample = 0.008453746338084048; break;
				case 307: inputSample = -0.000579970892666489; break;
				case 308: inputSample = -0.003814932897122191; break;
				case 309: inputSample = -0.000793600458998469; break;
				case 310: inputSample = 0.007507553331400495; break;
				case 311: inputSample = 0.019654158528702659; break;
				case 312: inputSample = 0.033692911313516642; break;
				case 313: inputSample = 0.047579071550950647; break;
				case 314: inputSample = 0.059542446478469187; break;
				case 315: inputSample = 0.068881229591051973; break;
				case 316: inputSample = 0.075961643002410956; break;
				case 317: inputSample = 0.081821316034730057; break;
				case 318: inputSample = 0.087864100123905151; break;
				case 319: inputSample = 0.094944513535264133; break;
				case 320: inputSample = 0.10254374160771508; break;
				case 321: inputSample = 0.108342258408154549; break;
				case 322: inputSample = 0.109257813692434452; break;
				case 323: inputSample = 0.102574260117191077; break;
				case 324: inputSample = 0.086032989555345316; break;
				case 325: inputSample = 0.058748965232093261; break;
				case 326: inputSample = 0.02255341692892239; break;
				case 327: inputSample = -0.017701093134556194; break;
				case 328: inputSample = -0.0557277520062869; break;
				case 329: inputSample = -0.085025878742027119; break;
				case 330: inputSample = -0.100071742339549019; break;
				case 331: inputSample = -0.09766078009094524; break;
				case 332: inputSample = -0.07873906578997171; break;
				case 333: inputSample = -0.048220079462263948; break;
				case 334: inputSample = -0.013458901104464944; break;
				case 335: inputSample = 0.017304233299050873; break;
				case 336: inputSample = 0.036439577166356395; break;
				case 337: inputSample = 0.039033650471816152; break;
				case 338: inputSample = 0.024567757767265847; break;
				case 339: inputSample = -0.003753895878170197; break;
				case 340: inputSample = -0.040620851389813008; break;
				case 341: inputSample = -0.078555954733115718; break;
				case 342: inputSample = -0.109135858866847832; break;
				case 343: inputSample = -0.12497520371074565; break;
				case 344: inputSample = -0.121618167668385957; break;
				case 345: inputSample = -0.097599743071993247; break;
				case 346: inputSample = -0.054140670300607402; break;
				case 347: inputSample = 0.004272591326944792; break;
				case 348: inputSample = 0.069857821894283884; break;
				case 349: inputSample = 0.133306876011230813; break;
				case 350: inputSample = 0.185463723983275847; break;
				case 351: inputSample = 0.218088487464827424; break;
				case 352: inputSample = 0.225413048951994377; break;
				case 353: inputSample = 0.206582890179448836; break;
				case 354: inputSample = 0.166389417134922335; break;
				case 355: inputSample = 0.113957783364665666; break;
				case 356: inputSample = 0.05987815008270516; break;
				case 357: inputSample = 0.014435374195379498; break;
				case 358: inputSample = -0.01416082682241288; break;
				case 359: inputSample = -0.021790573404339819; break;
				case 360: inputSample = -0.00991875400524927; break;
				case 361: inputSample = 0.0144048556859035; break;
				case 362: inputSample = 0.040742806215399635; break;
				case 363: inputSample = 0.059176224364757224; break;
				case 364: inputSample = 0.063784638508560434; break;
				case 365: inputSample = 0.053255514313486123; break;
				case 366: inputSample = 0.030244200529801323; break;
				case 367: inputSample = 0.000610370189825129; break;
				case 368: inputSample = -0.017395908039796223; break;
				case 369: inputSample = -0.039918925671865071; break;
				case 370: inputSample = -0.052797855883663529; break;
				case 371: inputSample = -0.055544640949430915; break;
				case 372: inputSample = -0.043153887676320772; break;
				case 373: inputSample = -0.021210721724295872; break;
				case 374: inputSample = 0.004119998779564807; break;
				case 375: inputSample = 0.029237089717093417; break;
				case 376: inputSample = 0.048982922986846519; break;
				case 377: inputSample = 0.061739779160740985; break;
				case 378: inputSample = 0.067141674550920127; break;
				case 379: inputSample = 0.065615749077120269; break;
				case 380: inputSample = 0.060305409215369116; break;
				case 381: inputSample = 0.052034773934446235; break;
				case 382: inputSample = 0.047731664098330635; break;
				case 383: inputSample = 0.042390805727103485; break;
				case 384: inputSample = 0.035096762749412515; break;
				case 385: inputSample = 0.025819016655781731; break;
				case 386: inputSample = 0.01278737468367565; break;
				case 387: inputSample = -0.004303229048738146; break;
				case 388: inputSample = -0.022461980612811756; break;
				case 389: inputSample = -0.041200703069856952; break;
				case 390: inputSample = -0.058138714254890679; break;
				case 391: inputSample = -0.073215096361888568; break;
				case 392: inputSample = -0.077213021103244198; break;
				case 393: inputSample = -0.073001466795556594; break;
				case 394: inputSample = -0.063937350268257781; break;
				case 395: inputSample = -0.054171188810083405; break;
				case 396: inputSample = -0.040773443937192996; break;
				case 397: inputSample = -0.031739845919370187; break;
				case 398: inputSample = -0.023682840204779284; break;
				case 399: inputSample = -0.020508796006347935; break;
				case 400: inputSample = -0.025910691396527082; break;
				case 401: inputSample = -0.04406944296060069; break;
				case 402: inputSample = -0.065646386798913631; break;
				case 403: inputSample = -0.091160218359630202; break;
				case 404: inputSample = -0.117101309053010735; break;
				case 405: inputSample = -0.140478844950407522; break;
				case 406: inputSample = -0.152778042695089655; break;
				case 407: inputSample = -0.149024266029541996; break;
				case 408: inputSample = -0.126867470511185132; break;
				case 409: inputSample = -0.084934323213599131; break;
				case 410: inputSample = -0.027711283455610913; break;
				case 411: inputSample = 0.028779312074953458; break;
				case 412: inputSample = 0.082279093676870016; break;
				case 413: inputSample = 0.120580419133884703; break;
				case 414: inputSample = 0.137945689451582393; break;
				case 415: inputSample = 0.128332239753715632; break;
				case 416: inputSample = 0.098209994049195837; break;
				case 417: inputSample = 0.057345113796197389; break;
				case 418: inputSample = 0.014282781647999511; break;
				case 419: inputSample = -0.019440648174688036; break;
				case 420: inputSample = -0.040712406918240997; break;
				case 421: inputSample = -0.052675781845759537; break;
				case 422: inputSample = -0.049166153256019862; break;
				case 423: inputSample = -0.03909492591601315; break;
				case 424: inputSample = -0.02404906231849125; break;
				case 425: inputSample = -0.010071346552629257; break;
				case 426: inputSample = -0.005920710050965997; break;
				case 427: inputSample = -0.004547377124546123; break;
				case 428: inputSample = -0.014008234275032894; break;
				case 429: inputSample = -0.021515906819055845; break;
				case 430: inputSample = -0.027009357737662981; break;
				case 431: inputSample = -0.037538362719809648; break;
				case 432: inputSample = -0.043520109790032742; break;
				case 433: inputSample = -0.04736556119693603; break;
				case 434: inputSample = -0.048159042443311956; break;
				case 435: inputSample = -0.045076553773308596; break;
				case 436: inputSample = -0.039216999953917135; break;
				case 437: inputSample = -0.030946245460066617; break;
				case 438: inputSample = -0.026490543076571026; break;
				case 439: inputSample = -0.020234129421063961; break;
				case 440: inputSample = -0.012085568178045071; break;
				case 441: inputSample = -0.007660265091097837; break;
				case 442: inputSample = -0.005615524956206025; break;
				case 443: inputSample = -0.005218784333018062; break;
				case 444: inputSample = -0.008698013626209382; break;
				case 445: inputSample = -0.015931019584948358; break;
				case 446: inputSample = -0.026521061586047025; break;
				case 447: inputSample = -0.036165029793389779; break;
				case 448: inputSample = -0.043733739356364723; break;
				case 449: inputSample = -0.04843370902859593; break;
				case 450: inputSample = -0.048616820085451914; break;
				case 451: inputSample = -0.043031813638416787; break;
				case 452: inputSample = -0.032167105052034142; break;
				case 453: inputSample = -0.019288055627308048; break;
				case 454: inputSample = -0.00415063650135816; break;
				case 455: inputSample = 0.010467967963499863; break;
				case 456: inputSample = 0.020539195303506577; break;
				case 457: inputSample = 0.025696942617877742; break;
				case 458: inputSample = 0.027375460639057589; break;
				case 459: inputSample = 0.023621683973509933; break;
				case 460: inputSample = 0.016358159505294963; break;
				case 461: inputSample = 0.008911523980224005; break;
				case 462: inputSample = 0.002319406720480971; break;
				case 463: inputSample = -0.002716266555986292; break;
				case 464: inputSample = -0.00589019154149; break;
				case 465: inputSample = -0.006683672787865927; break;
				case 466: inputSample = -0.005279821351970056; break;
				case 467: inputSample = -0.003387673764458231; break;
				case 468: inputSample = -0.001281896610614424; break;
			}
		}
				
		inputSample *= outlevel;
		//if envelope is in use, reduce by amount of quietness in source
		
		if (wet !=1.0) {
			inputSample = (inputSample * wet) + (drySample * (1.0 - wet));
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

