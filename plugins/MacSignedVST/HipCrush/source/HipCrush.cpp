/* ========================================
 *  HipCrush - HipCrush.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __HipCrush_H
#include "HipCrush.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new HipCrush(audioMaster);}

HipCrush::HipCrush(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	TRF = 0.5;
	TRG = 0.0;
	TRB = 0.5;
	HMF = 0.5;
	HMG = 0.0;
	HMB = 0.5;
	LMF = 0.5;
	LMG = 0.0;
	LMB = 0.5;
	DW = 1.0;
	
	for (int x = 0; x < biqs_total; x++) {
		high[x] = 0.0;
		hmid[x] = 0.0;
		lmid[x] = 0.0;
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

HipCrush::~HipCrush() {}
VstInt32 HipCrush::getVendorVersion () {return 1000;}
void HipCrush::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void HipCrush::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 HipCrush::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = TRF;
	chunkData[1] = TRG;
	chunkData[2] = TRB;
	chunkData[3] = HMF;
	chunkData[4] = HMG;
	chunkData[5] = HMB;
	chunkData[6] = LMF;
	chunkData[7] = LMG;
	chunkData[8] = LMB;
	chunkData[9] = DW;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 HipCrush::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	TRF = pinParameter(chunkData[0]);
	TRG = pinParameter(chunkData[1]);
	TRB = pinParameter(chunkData[2]);
	HMF = pinParameter(chunkData[3]);
	HMG = pinParameter(chunkData[4]);
	HMB = pinParameter(chunkData[5]);
	LMF = pinParameter(chunkData[6]);
	LMG = pinParameter(chunkData[7]);
	LMB = pinParameter(chunkData[8]);
	DW = pinParameter(chunkData[9]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void HipCrush::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamTRF: TRF = value; break;
        case kParamTRG: TRG = value; break;
        case kParamTRB: TRB = value; break;
        case kParamHMF: HMF = value; break;
        case kParamHMG: HMG = value; break;
        case kParamHMB: HMB = value; break;
        case kParamLMF: LMF = value; break;
        case kParamLMG: LMG = value; break;
        case kParamLMB: LMB = value; break;
        case kParamDW: DW = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float HipCrush::getParameter(VstInt32 index) {
    switch (index) {
        case kParamTRF: return TRF; break;
        case kParamTRG: return TRG; break;
        case kParamTRB: return TRB; break;
        case kParamHMF: return HMF; break;
        case kParamHMG: return HMG; break;
        case kParamHMB: return HMB; break;
        case kParamLMF: return LMF; break;
        case kParamLMG: return LMG; break;
        case kParamLMB: return LMB; break;
        case kParamDW: return DW; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void HipCrush::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamTRF: vst_strncpy (text, "Hi Freq", kVstMaxParamStrLen); break;
		case kParamTRG: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamTRB: vst_strncpy (text, "HiCrush", kVstMaxParamStrLen); break;
		case kParamHMF: vst_strncpy (text, "MidFreq", kVstMaxParamStrLen); break;
		case kParamHMG: vst_strncpy (text, "Mid", kVstMaxParamStrLen); break;
		case kParamHMB: vst_strncpy (text, "MdCrush", kVstMaxParamStrLen); break;
		case kParamLMF: vst_strncpy (text, "Lo Freq", kVstMaxParamStrLen); break;
		case kParamLMG: vst_strncpy (text, "Low", kVstMaxParamStrLen); break;
		case kParamLMB: vst_strncpy (text, "LoCrush", kVstMaxParamStrLen); break;
		case kParamDW: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void HipCrush::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamTRF: float2string (TRF, text, kVstMaxParamStrLen); break;
        case kParamTRG: float2string (TRG, text, kVstMaxParamStrLen); break;
        case kParamTRB: float2string (TRB, text, kVstMaxParamStrLen); break;
        case kParamHMF: float2string (HMF, text, kVstMaxParamStrLen); break;
        case kParamHMG: float2string (HMG, text, kVstMaxParamStrLen); break;
        case kParamHMB: float2string (HMB, text, kVstMaxParamStrLen); break;
        case kParamLMF: float2string (LMF, text, kVstMaxParamStrLen); break;
        case kParamLMG: float2string (LMG, text, kVstMaxParamStrLen); break;
        case kParamLMB: float2string (LMB, text, kVstMaxParamStrLen); break;
        case kParamDW: float2string (DW, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void HipCrush::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamTRF: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
        case kParamTRG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamTRB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamHMF: vst_strncpy (text, "Mid", kVstMaxParamStrLen); break;
        case kParamHMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamHMB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamLMF: vst_strncpy (text, "Low", kVstMaxParamStrLen); break;
        case kParamLMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamLMB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamDW: vst_strncpy (text, "Wet", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 HipCrush::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool HipCrush::getEffectName(char* name) {
    vst_strncpy(name, "HipCrush", kVstMaxProductStrLen); return true;
}

VstPlugCategory HipCrush::getPlugCategory() {return kPlugCategEffect;}

bool HipCrush::getProductString(char* text) {
  	vst_strncpy (text, "airwindows HipCrush", kVstMaxProductStrLen); return true;
}

bool HipCrush::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
