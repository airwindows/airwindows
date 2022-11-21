/* ========================================
 *  MultiBandDistortion - MultiBandDistortion.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __MultiBandDistortion_H
#include "MultiBandDistortion.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new MultiBandDistortion(audioMaster);}

MultiBandDistortion::MultiBandDistortion(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.0;
	C = 0.0;
	D = 0.0;
	E = 0.0;
	F = 1.0;
	G = 1.0;
	H = 1.0;
	I = 0.75;
	
	ataLast3SampleL = ataLast2SampleL = ataLast1SampleL = 0.0;
	ataHalfwaySampleL = ataHalfDrySampleL = ataHalfDiffSampleL = 0.0;
	ataAL = ataBL = ataCL = ataDrySampleL = ataDiffSampleL = ataPrevDiffSampleL = 0.0;
	ataUpsampleHighTweakL = 0.0414213562373095048801688; //more adds treble to upsampling
	ataDecayL = 0.915965594177219015; //Catalan's constant, more adds focus and clarity
	iirSampleAL = 0; iirSampleBL = 0;

	ataFlip = false; //end reset of antialias parameters

	ataLast3SampleR = ataLast2SampleR = ataLast1SampleR = 0.0;
	ataHalfwaySampleR = ataHalfDrySampleR = ataHalfDiffSampleR = 0.0;
	ataAR = ataBR = ataCR = ataDrySampleR = ataDiffSampleR = ataPrevDiffSampleR = 0.0;
	ataUpsampleHighTweakR = 0.0414213562373095048801688; //more adds treble to upsampling
	ataDecayR = 0.915965594177219015; //Catalan's constant, more adds focus and clarity
	iirSampleAR = 0; iirSampleBR = 0;
	
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

MultiBandDistortion::~MultiBandDistortion() {}
VstInt32 MultiBandDistortion::getVendorVersion () {return 1000;}
void MultiBandDistortion::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void MultiBandDistortion::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 MultiBandDistortion::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	chunkData[5] = F;
	chunkData[6] = G;
	chunkData[7] = H;
	chunkData[8] = I;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 MultiBandDistortion::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	F = pinParameter(chunkData[5]);
	G = pinParameter(chunkData[6]);
	H = pinParameter(chunkData[7]);
	I = pinParameter(chunkData[8]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void MultiBandDistortion::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        case kParamG: G = value; break;
        case kParamH: H = value; break;
        case kParamI: I = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float MultiBandDistortion::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        case kParamG: return G; break;
        case kParamH: return H; break;
        case kParamI: return I; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void MultiBandDistortion::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Freqcy", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "H Gain", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "L Gain", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "H Hard", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "L Hard", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "H Mtrx", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "L Mtrx", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Stable", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void MultiBandDistortion::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B*48.0, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C*48.0, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        case kParamI: float2string ((I-1.0)*48.0, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void MultiBandDistortion::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, "dB", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 MultiBandDistortion::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool MultiBandDistortion::getEffectName(char* name) {
    vst_strncpy(name, "MultiBandDistortion", kVstMaxProductStrLen); return true;
}

VstPlugCategory MultiBandDistortion::getPlugCategory() {return kPlugCategEffect;}

bool MultiBandDistortion::getProductString(char* text) {
  	vst_strncpy (text, "airwindows MultiBandDistortion", kVstMaxProductStrLen); return true;
}

bool MultiBandDistortion::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
