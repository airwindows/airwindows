/* ========================================
 *  VinylDither - VinylDither.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VinylDither_H
#include "VinylDither.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new VinylDither(audioMaster);}

VinylDither::VinylDither(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	NSOddL = 0.0;
	prevL = 0.0;
	nsL[0] = 0;
	nsL[1] = 0;
	nsL[2] = 0;
	nsL[3] = 0;
	nsL[4] = 0;
	nsL[5] = 0;
	nsL[6] = 0;
	nsL[7] = 0;
	nsL[8] = 0;
	nsL[9] = 0;
	nsL[10] = 0;
	nsL[11] = 0;
	nsL[12] = 0;
	nsL[13] = 0;
	nsL[14] = 0;
	nsL[15] = 0;
	NSOddR = 0.0;
	prevR = 0.0;
	nsR[0] = 0;
	nsR[1] = 0;
	nsR[2] = 0;
	nsR[3] = 0;
	nsR[4] = 0;
	nsR[5] = 0;
	nsR[6] = 0;
	nsR[7] = 0;
	nsR[8] = 0;
	nsR[9] = 0;
	nsR[10] = 0;
	nsR[11] = 0;
	nsR[12] = 0;
	nsR[13] = 0;
	nsR[14] = 0;
	nsR[15] = 0;
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

VinylDither::~VinylDither() {}
VstInt32 VinylDither::getVendorVersion () {return 1000;}
void VinylDither::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void VinylDither::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 VinylDither::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 VinylDither::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void VinylDither::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
		default: throw; // unknown parameter, shouldn't happen!
    }
}

float VinylDither::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void VinylDither::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Quant", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "DeRez", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void VinylDither::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 1.999 )) //0 to almost edge of # of params
		{	case 0: vst_strncpy (text, "CD 16", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "HD 24", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break; //completed consoletype 'popup' parameter, exit
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void VinylDither::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 VinylDither::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool VinylDither::getEffectName(char* name) {
    vst_strncpy(name, "VinylDither", kVstMaxProductStrLen); return true;
}

VstPlugCategory VinylDither::getPlugCategory() {return kPlugCategEffect;}

bool VinylDither::getProductString(char* text) {
  	vst_strncpy (text, "airwindows VinylDither", kVstMaxProductStrLen); return true;
}

bool VinylDither::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
