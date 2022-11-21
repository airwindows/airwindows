/* ========================================
 *  Flipity - Flipity.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Flipity_H
#include "Flipity.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Flipity(audioMaster);}

Flipity::Flipity(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
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

Flipity::~Flipity() {}
VstInt32 Flipity::getVendorVersion () {return 1000;}
void Flipity::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Flipity::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Flipity::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Flipity::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Flipity::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Flipity::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Flipity::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Flipity", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Flipity::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 7.999 )) //0 to almost edge of # of params
		{
			case 0: vst_strncpy (text, "Dry", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "Flip L", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "Flip R", kVstMaxParamStrLen); break;
			case 3: vst_strncpy (text, "Flip LR", kVstMaxParamStrLen); break;
			case 4: vst_strncpy (text, "Swap LR", kVstMaxParamStrLen); break;
			case 5: vst_strncpy (text, "Swip L", kVstMaxParamStrLen); break;
			case 6: vst_strncpy (text, "Swip R", kVstMaxParamStrLen); break;
			case 7: vst_strncpy (text, "Swip LR", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break; //E as example 'popup' parameter with four values  
        default: break; // unknown parameter, shouldn't happen!
	}
}

void Flipity::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Flipity::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Flipity::getEffectName(char* name) {
    vst_strncpy(name, "Flipity", kVstMaxProductStrLen); return true;
}

VstPlugCategory Flipity::getPlugCategory() {return kPlugCategEffect;}

bool Flipity::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Flipity", kVstMaxProductStrLen); return true;
}

bool Flipity::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
