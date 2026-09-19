/* ========================================
 *  Ultralight2 - Ultralight2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Ultralight2_H
#include "Ultralight2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Ultralight2(audioMaster);}

Ultralight2::Ultralight2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.65;
	B = 0.75;
	C = 0.75;
	D = 0.54;
	E = 0.5;
	F = 0.0;
		
	for(int x = 0; x < d4A+2; x++) {lA[x] = 0.0f; rA[x] = 0.0f;}
	for(int x = 0; x < d4B+2; x++) {lB[x] = 0.0f; rB[x] = 0.0f;}
	for(int x = 0; x < d4C+2; x++) {lC[x] = 0.0f; rC[x] = 0.0f;}
	for(int x = 0; x < d4D+2; x++) {lD[x] = 0.0f; rD[x] = 0.0f;}
	for(int x = 0; x < d4E+2; x++) {lE[x] = 0.0f; rE[x] = 0.0f;}
	for(int x = 0; x < d4F+2; x++) {lF[x] = 0.0f; rF[x] = 0.0f;}
	for(int x = 0; x < d4G+2; x++) {lG[x] = 0.0f; rG[x] = 0.0f;}
	for(int x = 0; x < d4H+2; x++) {lH[x] = 0.0f; rH[x] = 0.0f;}
	for(int x = 0; x < d4I+2; x++) {lI[x] = 0.0f; rI[x] = 0.0f;}
	for(int x = 0; x < d4J+2; x++) {lJ[x] = 0.0f; rJ[x] = 0.0f;}
	for(int x = 0; x < d4K+2; x++) {lK[x] = 0.0f; rK[x] = 0.0f;}
	for(int x = 0; x < d4L+2; x++) {lL[x] = 0.0f; rL[x] = 0.0f;}
	for(int x = 0; x < d4M+2; x++) {lM[x] = 0.0f; rM[x] = 0.0f;}
	for(int x = 0; x < d4N+2; x++) {lN[x] = 0.0f; rN[x] = 0.0f;}
	for(int x = 0; x < d4O+2; x++) {lO[x] = 0.0f; rO[x] = 0.0f;}
	for(int x = 0; x < d4P+2; x++) {lP[x] = 0.0f; rP[x] = 0.0f;}
	for(int x = 0; x < d4Q+2; x++) {lQ[x] = 0.0f; rQ[x] = 0.0f;}
	for(int x = 0; x < d4R+2; x++) {lR[x] = 0.0f; rR[x] = 0.0f;}
	for(int x = 0; x < d4S+2; x++) {lS[x] = 0.0f; rS[x] = 0.0f;}
	for(int x = 0; x < d4T+2; x++) {lT[x] = 0.0f; rT[x] = 0.0f;}
	
	cA = cB = cC = cD = cE = cF = cG = cH = 1;
	cI = cJ = cK = cL = cM = cN = cO = cP = 1;
	cQ = cR = cS = cT = 1;
	
	oQL = oRL = oSL = oTL = 0.0f;
	fAL = fBL = fCL = fDL = 0.0f;
	fhAL = fhBL = fhCL = fhDL = 0.0f;
	flAL = flBL = flCL = flDL = 0.0f;
	hAL = hBL = hCL = hDL = hEL = 0.0f;
	
	oQR = oRR = oSR = oTR = 0.0f;
	fAR = fBR = fCR = fDR = 0.0f;
	fhAR = fhBR = fhCR = fhDR = 0.0f;
	flAR = flBR = flCR = flDR = 0.0f;
	hAR = hBR = hCR = hDR = hER = 0.0f;
	
	firstDryL = firstAvgL = 0.0f;
	lsA = lsB = lsC = lsD = lsE = lsF = lsG = lsH = lsI = lsJ = lsK = lsL = lsM = 0.0f;
	lsN = lsO = lsP = lsQ = lsR = lsS = lsT = lsU = lsV = lsW = lsX = lsY = lsZ = 0.0f;
	lsa = lsb = lsc = lsd = lse = lsf = lsg = lsh = lsi = lsj = lsk = lsl = lsm = 0.0f;
	lsn = lso = lsp = lsq = lsr = lss = lst = lsu = lsv = lsw = lsx = lsy = lsz = 0.0f;	
	firstDryR = firstAvgR = 0.0f;
	rsA = rsB = rsC = rsD = rsE = rsF = rsG = rsH = rsI = rsJ = rsK = rsL = rsM = 0.0f;
	rsN = rsO = rsP = rsQ = rsR = rsS = rsT = rsU = rsV = rsW = rsX = rsY = rsZ = 0.0f;
	rsa = rsb = rsc = rsd = rse = rsf = rsg = rsh = rsi = rsj = rsk = rsl = rsm = 0.0f;
	rsn = rso = rsp = rsq = rsr = rss = rst = rsu = rsv = rsw = rsx = rsy = rsz = 0.0f;	
	
	prevDistance = 0;
	pointCycle = pointAL = pointBL = pointAR = pointBR = 0.0f;	
	
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

Ultralight2::~Ultralight2() {}
VstInt32 Ultralight2::getVendorVersion () {return 1000;}
void Ultralight2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Ultralight2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Ultralight2::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	chunkData[5] = F;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Ultralight2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	F = pinParameter(chunkData[5]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Ultralight2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Ultralight2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Ultralight2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Damping", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "UnSolid", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "UnReflc", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Predlay", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Distnce", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Ultralight2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((int)(A*9.99), text, kVstMaxParamStrLen); break;
        case kParamB: int2string ((int)(B*4.99), text, kVstMaxParamStrLen); break;
        case kParamC: int2string ((int)(C*4.99), text, kVstMaxParamStrLen); break;
        case kParamD: int2string ((int)(D*998.0), text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Ultralight2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Ultralight2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Ultralight2::getEffectName(char* name) {
    vst_strncpy(name, "Ultralight2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Ultralight2::getPlugCategory() {return kPlugCategEffect;}

bool Ultralight2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Ultralight2", kVstMaxProductStrLen); return true;
}

bool Ultralight2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
