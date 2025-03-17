/* ========================================
 *  Mastering2 - Mastering2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Mastering2_H
#include "Mastering2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Mastering2(audioMaster);}

Mastering2::Mastering2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.0;
	C = 0.5;
	D = 0.5;
	E = 0.5;
	F = 0.5;
	G = 1.0;
	
	iirA = 0.0;
	iirB = 0.0;
	iirC = 0.0;
	fpFlip = true;
	
	for (int x = 0; x < air_total; x++) air[x] = 0.0;
	for (int x = 0; x < kal_total; x++) {kalM[x] = 0.0;kalS[x] = 0.0;}
	
	lastSinewL = 0.0;
	lastSinewR = 0.0;
	
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;
	for (int x = 0; x < 16; x++) {intermediateL[x] = 0.0; intermediateR[x] = 0.0;}
	
	quantA = 0;
	quantB = 1;
	expectedSlew = 0.0;
	testA = 0.0;
	testB = 0.0;
	correction = 0.0;
	shapedSampleL = 0.0;
	shapedSampleR = 0.0;
	currentDither = 0.0;
	ditherL = 0.0;
	ditherR = 0.0;
	cutbinsL = false;
	cutbinsR = false;
	hotbinA = 0;
	hotbinB = 0;
	benfordize = 0.0;
	totalA = 0.0;
	totalB = 0.0;
	outputSample = 0.0;
	expon = 0; //internal dither variables
	//these didn't like to be defined inside a case statement
	
	NSOddL = 0.0; NSEvenL = 0.0; prevShapeL = 0.0;
	NSOddR = 0.0; NSEvenR = 0.0; prevShapeR = 0.0;
	flip = true; //Ten Nines
	for(int count = 0; count < 99; count++) {
		darkSampleL[count] = 0;
		darkSampleR[count] = 0;
	} //Dark
	previousDitherL = 0.0;
	previousDitherR = 0.0; //PaulWide
	bynL[0] = 1000.0;
	bynL[1] = 301.0;
	bynL[2] = 176.0;
	bynL[3] = 125.0;
	bynL[4] = 97.0;
	bynL[5] = 79.0;
	bynL[6] = 67.0;
	bynL[7] = 58.0;
	bynL[8] = 51.0;
	bynL[9] = 46.0;
	bynL[10] = 1000.0;
	noiseShapingL = 0.0;
	bynR[0] = 1000.0;
	bynR[1] = 301.0;
	bynR[2] = 176.0;
	bynR[3] = 125.0;
	bynR[4] = 97.0;
	bynR[5] = 79.0;
	bynR[6] = 67.0;
	bynR[7] = 58.0;
	bynR[8] = 51.0;
	bynR[9] = 46.0;
	bynR[10] = 1000.0;
	noiseShapingR = 0.0; //NJAD
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	//this is reset: values being initialized only once. Startup values, whatever they are.
	
    _canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
    _canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
    _canDo.insert("x2in2out"); 
    setNumInputs(kNumInputs);
    setNumOutputs(kNumOutputs);
    setUniqueID(kUniqueId);
    canProcessReplacing();     // supports output replacing
    canDoubleReplacing();      // supports double precision processing
	programsAreChunks(true);
    vst_strncpy (_programName, "Default", kVstMaxProgNameLen); // default program name
}

Mastering2::~Mastering2() {}
VstInt32 Mastering2::getVendorVersion () {return 1000;}
void Mastering2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Mastering2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Mastering2::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	chunkData[5] = F;
	chunkData[6] = G;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Mastering2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	F = pinParameter(chunkData[5]);
	G = pinParameter(chunkData[6]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Mastering2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Mastering2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        case kParamG: return G; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Mastering2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Sidepas", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "Glue", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Scope", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Skronk", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Girth", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Dither", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Mastering2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: switch((VstInt32)( G * 5.999 )) //0 to almost edge of # of params
		{	case 0: vst_strncpy (text, "Dark", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "TenNines", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "TPDFWde", kVstMaxParamStrLen); break;
			case 3: vst_strncpy (text, "PaulWde", kVstMaxParamStrLen); break;
			case 4: vst_strncpy (text, "NJAD", kVstMaxParamStrLen); break;
			case 5: vst_strncpy (text, "Bypass", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break; //completed consoletype 'popup' parameter, exit
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Mastering2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Mastering2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Mastering2::getEffectName(char* name) {
    vst_strncpy(name, "Mastering2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Mastering2::getPlugCategory() {return kPlugCategEffect;}

bool Mastering2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Mastering2", kVstMaxProductStrLen); return true;
}

bool Mastering2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
