/* ========================================
 *  Channel9 - Channel9.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Channel9_H
#include "Channel9.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Channel9(audioMaster);}

Channel9::Channel9(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.0;
	C = 1.0;
	for (int x = 0; x < 15; x++) {biquadA[x] = 0.0; biquadB[x] = 0.0;}
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	iirSampleLA = 0.0;
	iirSampleRA = 0.0;
	iirSampleLB = 0.0;
	iirSampleRB = 0.0;
	lastSampleAL = lastSampleBL = lastSampleCL = 0.0;
	lastSampleAR = lastSampleBR = lastSampleCR = 0.0;
	flip = false;
	iirAmount = 0.005832;
	threshold = 0.33362176; //instantiating with Neve values
	cutoff = 28811.0;
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

Channel9::~Channel9() {}
VstInt32 Channel9::getVendorVersion () {return 1000;}
void Channel9::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Channel9::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Channel9::getChunk (void** data, bool isPreset)
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

VstInt32 Channel9::setChunk (void* data, VstInt32 byteSize, bool isPreset)
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

void Channel9::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
	switch((VstInt32)( A * 4.999 ))
	{  
		case 0: iirAmount = 0.005832; threshold = 0.33362176; cutoff = 28811.0; break; //Neve
		case 1: iirAmount = 0.004096; threshold = 0.59969536; cutoff = 27216.0; break; //API
		case 2: iirAmount = 0.004913; threshold = 0.84934656; cutoff = 23011.0; break; //SSL
		case 3: iirAmount = 0.009216; threshold = 0.149; cutoff = 18544.0; break; //Teac
		case 4: iirAmount = 0.011449; threshold = 0.092; cutoff = 19748.0; break; //Mackie
		default: break; //should not happen
	}
}

float Channel9::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Channel9::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Console Type", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Drive", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Channel9::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 4.999 )) //0 to almost edge of # of params
		{	case 0: vst_strncpy (text, "Neve", kVstMaxParamStrLen); break;
			case 1: vst_strncpy (text, "API", kVstMaxParamStrLen); break;
			case 2: vst_strncpy (text, "SSL", kVstMaxParamStrLen); break;
			case 3: vst_strncpy (text, "Teac", kVstMaxParamStrLen); break;
			case 4: vst_strncpy (text, "Mackie", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break; //completed consoletype 'popup' parameter, exit
		case kParamB: int2string ((VstInt32)(B*200), text, kVstMaxParamStrLen); break;
		case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Channel9::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "%", kVstMaxParamStrLen); break; //the percent
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break; //the percent
		default: break; // unknown parameter, shouldn't happen!
    }
}


VstInt32 Channel9::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Channel9::getEffectName(char* name) {
    vst_strncpy(name, "Channel9", kVstMaxProductStrLen); return true;
}

VstPlugCategory Channel9::getPlugCategory() {return kPlugCategEffect;}

bool Channel9::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Channel9", kVstMaxProductStrLen); return true;
}

bool Channel9::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
