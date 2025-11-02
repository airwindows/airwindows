/* ========================================
 *  ConsoleX2Buss - ConsoleX2Buss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleX2Buss_H
#include "ConsoleX2Buss.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleX2Buss(audioMaster);}

ConsoleX2Buss::ConsoleX2Buss(audioMasterCallback audioMaster) :
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
	I = 1.0;
	J = 0.5;
	K = 0.5;
	
	for (int x = 0; x < biq_total; x++) {
		highA[x] = 0.0;
		highB[x] = 0.0;
		highC[x] = 0.0;
		midA[x] = 0.0;
		midB[x] = 0.0;
		midC[x] = 0.0;
		lowA[x] = 0.0;
		lowB[x] = 0.0;
		lowC[x] = 0.0;
	}
	highLIIR = 0.0;
	highRIIR = 0.0;
	midLIIR = 0.0;
	midRIIR = 0.0;
	lowLIIR = 0.0;
	lowRIIR = 0.0;
	//SmoothEQ2
	
	for (int x = 0; x < bez_total; x++) {bezCompF[x] = 0.0;bezCompS[x] = 0.0;}
	bezCompF[bez_cycle] = 1.0; bezMaxF = 0.0;
	bezCompS[bez_cycle] = 1.0;
	//Dynamics2
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; avg32R[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; avg16R[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; avg8R[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; avg4R[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; avg2R[x] = 0.0;}
	avgPos = 0;
	lastSlewL = 0.0; lastSlewR = 0.0;
	lastSlewpleL = 0.0; lastSlewpleR = 0.0;
	//preTapeHack
	
	panA = 0.5; panB = 0.5;
	inTrimA = 0.5; inTrimB = 0.5;
	
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

ConsoleX2Buss::~ConsoleX2Buss() {}
VstInt32 ConsoleX2Buss::getVendorVersion () {return 1000;}
void ConsoleX2Buss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleX2Buss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 ConsoleX2Buss::getChunk (void** data, bool isPreset)
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
	chunkData[9] = J;
	chunkData[10] = K;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 ConsoleX2Buss::setChunk (void* data, VstInt32 byteSize, bool isPreset)
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
	J = pinParameter(chunkData[9]);
	K = pinParameter(chunkData[10]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void ConsoleX2Buss::setParameter(VstInt32 index, float value) {
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
        case kParamJ: J = value; break;
        case kParamK: K = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float ConsoleX2Buss::getParameter(VstInt32 index) {
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
        case kParamJ: return J; break;
        case kParamK: return K; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleX2Buss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "HMid", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "LMid", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Bass", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "HighF", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "HMidF", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "LMidF", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "BassF", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "Thresh", kVstMaxParamStrLen); break;
		case kParamJ: vst_strncpy (text, "Pan", kVstMaxParamStrLen); break;
		case kParamK: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ConsoleX2Buss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        case kParamI: float2string (I, text, kVstMaxParamStrLen); break;
        case kParamJ: float2string (J, text, kVstMaxParamStrLen); break;
        case kParamK: float2string (K, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ConsoleX2Buss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "eq", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "freq", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, "dyn", kVstMaxParamStrLen); break;
        case kParamJ: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamK: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ConsoleX2Buss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleX2Buss::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleX2Buss", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleX2Buss::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleX2Buss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleX2Buss", kVstMaxProductStrLen); return true;
}

bool ConsoleX2Buss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
