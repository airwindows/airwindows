/* ========================================
 *  ToTape8 - ToTape8.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ToTape8_H
#include "ToTape8.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ToTape8(audioMaster);}

ToTape8::ToTape8(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 0.5;
	F = 0.5;
	G = 0.5;
	H = 0.5;
	I = 0.5;
	
	iirEncL = 0.0; iirDecL = 0.0;
	compEncL = 1.0; compDecL = 1.0;
	avgEncL = 0.0; avgDecL = 0.0;
	
	iirEncR = 0.0; iirDecR = 0.0;
	compEncR = 1.0; compDecR = 1.0;
	avgEncR = 0.0; avgDecR = 0.0;
	
	for (int temp = 0; temp < 1001; temp++) {dL[temp] = 0.0;dR[temp] = 0.0;}
	sweepL = M_PI;
	sweepR = M_PI;
	nextmaxL = 0.5;	
	nextmaxR = 0.5;
	gcount = 0;	
	
	for (int x = 0; x < gslew_total; x++) gslew[x] = 0.0;
	
	iirMidRollerL = 0.0;
	iirLowCutoffL = 0.0;
	iirMidRollerR = 0.0;
	iirLowCutoffR = 0.0;
	
	headBumpL = 0.0;
	headBumpR = 0.0;
	for (int x = 0; x < hdb_total; x++) {hdbA[x] = 0.0;hdbB[x] = 0.0;}
	//from ZBandpass, so I can use enums with it	
	
	iirDecL = 0.0;
	iirDecR = 0.0;
	
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;
	for (int x = 0; x < 16; x++) {intermediateL[x] = 0.0; intermediateR[x] = 0.0;}
	//this is reset: values being initialized only once. Startup values, whatever they are.
	
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

ToTape8::~ToTape8() {}
VstInt32 ToTape8::getVendorVersion () {return 1000;}
void ToTape8::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ToTape8::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 ToTape8::getChunk (void** data, bool isPreset)
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

VstInt32 ToTape8::setChunk (void* data, VstInt32 byteSize, bool isPreset)
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

void ToTape8::setParameter(VstInt32 index, float value) {
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

float ToTape8::getParameter(VstInt32 index) {
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

void ToTape8::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Input", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Tilt", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Shape", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Flutter", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "FlutSpd", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Bias", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "HeadBmp", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "HeadFrq", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ToTape8::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (((H*H)*175.0)+25.0, text, kVstMaxParamStrLen); break;
        case kParamI: float2string (I, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ToTape8::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "hz", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ToTape8::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ToTape8::getEffectName(char* name) {
    vst_strncpy(name, "ToTape8", kVstMaxProductStrLen); return true;
}

VstPlugCategory ToTape8::getPlugCategory() {return kPlugCategEffect;}

bool ToTape8::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ToTape8", kVstMaxProductStrLen); return true;
}

bool ToTape8::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
