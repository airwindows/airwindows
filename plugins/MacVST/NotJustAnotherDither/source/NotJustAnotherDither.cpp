/* ========================================
 *  NotJustAnotherDither - NotJustAnotherDither.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __NotJustAnotherDither_H
#include "NotJustAnotherDither.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new NotJustAnotherDither(audioMaster);}

NotJustAnotherDither::NotJustAnotherDither(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	bynL[0] = 1000;
	bynL[1] = 301;
	bynL[2] = 176;
	bynL[3] = 125;
	bynL[4] = 97;
	bynL[5] = 79;
	bynL[6] = 67;
	bynL[7] = 58;
	bynL[8] = 51;
	bynL[9] = 46;
	bynL[10] = 1000;
	noiseShapingL = 0.0;
	
	bynR[0] = 1000;
	bynR[1] = 301;
	bynR[2] = 176;
	bynR[3] = 125;
	bynR[4] = 97;
	bynR[5] = 79;
	bynR[6] = 67;
	bynR[7] = 58;
	bynR[8] = 51;
	bynR[9] = 46;
	bynR[10] = 1000;
	noiseShapingR = 0.0;
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

NotJustAnotherDither::~NotJustAnotherDither() {}
VstInt32 NotJustAnotherDither::getVendorVersion () {return 1000;}
void NotJustAnotherDither::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void NotJustAnotherDither::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 NotJustAnotherDither::getChunk (void** data, bool isPreset)
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

VstInt32 NotJustAnotherDither::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void NotJustAnotherDither::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
		default: throw; // unknown parameter, shouldn't happen!
    }
}

float NotJustAnotherDither::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void NotJustAnotherDither::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Quant", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "DeRez", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void NotJustAnotherDither::getParameterDisplay(VstInt32 index, char *text) {
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

void NotJustAnotherDither::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 NotJustAnotherDither::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool NotJustAnotherDither::getEffectName(char* name) {
    vst_strncpy(name, "NotJustAnotherDither", kVstMaxProductStrLen); return true;
}

VstPlugCategory NotJustAnotherDither::getPlugCategory() {return kPlugCategEffect;}

bool NotJustAnotherDither::getProductString(char* text) {
  	vst_strncpy (text, "airwindows NotJustAnotherDither", kVstMaxProductStrLen); return true;
}

bool NotJustAnotherDither::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
