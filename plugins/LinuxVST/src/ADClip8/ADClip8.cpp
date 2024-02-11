/* ========================================
 *  ADClip8 - ADClip8.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ADClip8_H
#include "ADClip8.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ADClip8(audioMaster);}

ADClip8::ADClip8(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.883;
	C = 0.0;
	for (int stage = 0; stage < 8; stage++) {
		lastSampleL[stage] = 0.0;
		lastSampleR[stage] = 0.0;
		wasPosClipL[stage] = false;
		wasPosClipR[stage] = false;
		wasNegClipL[stage] = false;
		wasNegClipR[stage] = false;
		for (int x = 0; x < 16; x++) {intermediateL[x][stage] = 0.0;intermediateR[x][stage] = 0.0;}
	}
	
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

ADClip8::~ADClip8() {}
VstInt32 ADClip8::getVendorVersion () {return 1000;}
void ADClip8::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ADClip8::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 ADClip8::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 ADClip8::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void ADClip8::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float ADClip8::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
		default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ADClip8::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Boost", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Ceiling", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Mode", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ADClip8::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A*18.0, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: switch((VstInt32)( C * 7.999 )) //0 to almost edge of # of params
		{case 0: vst_strncpy (text, "Normal", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "Atten", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "Clips", kVstMaxParamStrLen); break;
			case 3: vst_strncpy (text, "Afterbr", kVstMaxParamStrLen); break;
			case 4: vst_strncpy (text, "Explode", kVstMaxParamStrLen); break;
			case 5: vst_strncpy (text, "Nuke", kVstMaxParamStrLen); break;
			case 6: vst_strncpy (text, "Apocaly", kVstMaxParamStrLen); break;
			case 7: vst_strncpy (text, "Apothes", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ADClip8::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ADClip8::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ADClip8::getEffectName(char* name) {
    vst_strncpy(name, "ADClip8", kVstMaxProductStrLen); return true;
}

VstPlugCategory ADClip8::getPlugCategory() {return kPlugCategEffect;}

bool ADClip8::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ADClip8", kVstMaxProductStrLen); return true;
}

bool ADClip8::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
