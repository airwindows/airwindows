/* ========================================
 *  ADClip9 - ADClip9.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ADClip9_H
#include "ADClip9.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ADClip9(audioMaster);}

ADClip9::ADClip9(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.0;
	C = 0.7;
	D = 0.75;
	E = 0.0;
	
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;
	for (int x = 0; x < 17; x++) {intermediateL[x] = 0.0; intermediateR[x] = 0.0;}
	for (int x = 0; x < 33; x++) {slewL[x] = 0.0; slewR[x] = 0.0;}

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

ADClip9::~ADClip9() {}
VstInt32 ADClip9::getVendorVersion () {return 1000;}
void ADClip9::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ADClip9::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 ADClip9::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 ADClip9::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void ADClip9::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float ADClip9::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ADClip9::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Boost", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Match", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Noise", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Ceiling", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Mode", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ADClip9::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A*18.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B*18.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: switch((VstInt32)( E * 2.999 )) {
			case 0: vst_strncpy (text, "Boost", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "Match", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "ClipOnly", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ADClip9::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ADClip9::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ADClip9::getEffectName(char* name) {
    vst_strncpy(name, "ADClip9", kVstMaxProductStrLen); return true;
}

VstPlugCategory ADClip9::getPlugCategory() {return kPlugCategEffect;}

bool ADClip9::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ADClip9", kVstMaxProductStrLen); return true;
}

bool ADClip9::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
