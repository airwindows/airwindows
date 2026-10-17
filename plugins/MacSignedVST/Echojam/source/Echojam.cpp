/* ========================================
 *  Echojam - Echojam.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Echojam_H
#include "Echojam.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Echojam(audioMaster);}

Echojam::Echojam(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 0.5;

	for(int x = 0; x < d4A+2; x++) {lA[x] = 0.0; rA[x] = 0.0;}
	for(int x = 0; x < d4B+2; x++) {lB[x] = 0.0; rB[x] = 0.0;}
	for(int x = 0; x < d4C+2; x++) {lC[x] = 0.0; rC[x] = 0.0;}
	for(int x = 0; x < d4D+2; x++) {lD[x] = 0.0; rD[x] = 0.0;}
	for(int x = 0; x < d4E+2; x++) {lE[x] = 0.0; rE[x] = 0.0;}
	for(int x = 0; x < d4F+2; x++) {lF[x] = 0.0; rF[x] = 0.0;}
	for(int x = 0; x < d4G+2; x++) {lG[x] = 0.0; rG[x] = 0.0;}
	for(int x = 0; x < d4H+2; x++) {lH[x] = 0.0; rH[x] = 0.0;}
	for(int x = 0; x < d4I+2; x++) {lI[x] = 0.0; rI[x] = 0.0;}
	for(int x = 0; x < d4J+2; x++) {lJ[x] = 0.0; rJ[x] = 0.0;}
	for(int x = 0; x < d4K+2; x++) {lK[x] = 0.0; rK[x] = 0.0;}
	for(int x = 0; x < d4L+2; x++) {lL[x] = 0.0; rL[x] = 0.0;}
	for(int x = 0; x < d4M+2; x++) {lM[x] = 0.0; rM[x] = 0.0;}
	for(int x = 0; x < d4N+2; x++) {lN[x] = 0.0; rN[x] = 0.0;}
	for(int x = 0; x < d4O+2; x++) {lO[x] = 0.0; rO[x] = 0.0;}
	for(int x = 0; x < d4P+2; x++) {lP[x] = 0.0; rP[x] = 0.0;}
	
	cLA = cLB = cLC = cLD = cLE = cLF = cLG = cLH = 1;
	cLI = cLJ = cLK = cLL = cLM = cLN = cLO = cLP = 1;
	
	cRA = cRB = cRC = cRD = cRE = cRF = cRG = cRH = 1;
	cRI = cRJ = cRK = cRL = cRM = cRN = cRO = cRP = 1;
	
	fAL = fBL = fCL = fDL = 0.0f;
	hAL = hBL = hCL = hDL = hEL = 0.0f;
	
	fDR = fHR = fLR = fPR = 0.0f;
	hAR = hBR = hCR = hDR = hER = 0.0f;
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	shortA = 17; shortB = 19; shortC = 23; shortD = 29; shortE = 19; shortF = 106; shortG = 41;
	shortH = 6361; shortI = 47; shortJ = 53; shortK = 59; shortL = 61; shortM = 67; shortN = 5824;
	shortO = 6257; shortP = 653; //1 to 419 ms, 5286 seat arena
	prevclearcoat = -1;
		
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

Echojam::~Echojam() {}
VstInt32 Echojam::getVendorVersion () {return 1000;}
void Echojam::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Echojam::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Echojam::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Echojam::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Echojam::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Echojam::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Echojam::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Select", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Regen", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Speed", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Echojam::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((VstInt32)( A * 16.999 ), text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Echojam::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Echojam::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Echojam::getEffectName(char* name) {
    vst_strncpy(name, "Echojam", kVstMaxProductStrLen); return true;
}

VstPlugCategory Echojam::getPlugCategory() {return kPlugCategEffect;}

bool Echojam::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Echojam", kVstMaxProductStrLen); return true;
}

bool Echojam::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
