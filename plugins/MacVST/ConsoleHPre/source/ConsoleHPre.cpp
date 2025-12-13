/* ========================================
 *  ConsoleHPre - ConsoleHPre.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleHPre_H
#include "ConsoleHPre.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleHPre(audioMaster);}

ConsoleHPre::ConsoleHPre(audioMasterCallback audioMaster) :
AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	TRM = 0.25;
	MOR = 0.0;
	
	HIG = 0.5;
	MID = 0.5;
	LOW = 0.5;
	CRS = 0.0;
	
	TRF = 0.5;
	TRG = 0.0;
	TRB = 0.5;
	HMF = 0.5;
	HMG = 0.0;
	HMB = 0.5;
	LMF = 0.5;
	LMG = 0.0;
	LMB = 0.5;
	BSF = 0.5;
	BSG = 0.0;
	BSB = 0.5;
	
	THR = 1.0;
	ATK = 0.5;
	RLS = 0.5;
	GAT = 0.0;
	
	LOP = 1.0;
	HIP = 0.0;
	FAD = 0.5;
	
	for (int x = 0; x < biq_total; x++) {
		highFast[x] = 0.0;
		lowFast[x] = 0.0;
	}
	highFastLIIR = 0.0;
	highFastRIIR = 0.0;
	lowFastLIIR = 0.0;
	lowFastRIIR = 0.0;
	//SmoothEQ3
	
	for (int x = 0; x < biqs_total; x++) {
		high[x] = 0.0;
		hmid[x] = 0.0;
		lmid[x] = 0.0;
		bass[x] = 0.0;
	}
	//HipCrush with four bands
	
	for (int x = 0; x < bez_total; x++) bezComp[x] = 0.0;
	bezComp[bez_cycle] = 1.0; 
	bezMaxL = 0.0; bezMinL = 0.0; bezGateL = 2.0;
	bezMaxR = 0.0; bezMinR = 0.0; bezGateR = 2.0; //Dual mono version
	//Dynamics3
	
	for(int count = 0; count < 22; count++) {
		iirHPositionL[count] = 0.0;
		iirHAngleL[count] = 0.0;
		iirHPositionR[count] = 0.0;
		iirHAngleR[count] = 0.0;
	}
	hBypass = false;
	
	for(int count = 0; count < 14; count++) {
		iirLPositionL[count] = 0.0;
		iirLAngleL[count] = 0.0;
		iirLPositionR[count] = 0.0;
		iirLAngleR[count] = 0.0;
	}
	lBypass = false;	
	//Cabs2
	
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
		dBaR[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBaPosR = 0.0;
	dBaXL = 1;
	dBaXR = 1;
	//Discontapeity
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; avg32R[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; avg16R[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; avg8R[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; avg4R[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; avg2R[x] = 0.0;}
	avgPos = 0;
	lastSlewL = 0.0; lastSlewR = 0.0;
	lastSlewpleL = 0.0; lastSlewpleR = 0.0;
	//preTapeHack
	
	lFreqA = 1.0; lFreqB = 1.0;
	hFreqA = 0.0; hFreqB = 0.0;
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

ConsoleHPre::~ConsoleHPre() {}
VstInt32 ConsoleHPre::getVendorVersion () {return 1000;}
void ConsoleHPre::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleHPre::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 ConsoleHPre::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = TRM;
	chunkData[1] = MOR;
	chunkData[2] = HIG;
	chunkData[3] = MID;
	chunkData[4] = LOW;
	chunkData[5] = CRS;
	chunkData[6] = TRF;
	chunkData[7] = TRG;
	chunkData[8] = TRB;
	chunkData[9] = HMF;
	chunkData[10] = HMG;
	chunkData[11] = HMB;
	chunkData[12] = LMF;
	chunkData[13] = LMG;
	chunkData[14] = LMB;
	chunkData[15] = BSF;
	chunkData[16] = BSG;
	chunkData[17] = BSB;
	chunkData[18] = THR;
	chunkData[19] = ATK;
	chunkData[20] = RLS;
	chunkData[21] = GAT;
	chunkData[22] = LOP;
	chunkData[23] = HIP;
	chunkData[24] = FAD;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 ConsoleHPre::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	TRM = pinParameter(chunkData[0]);
	MOR = pinParameter(chunkData[1]);
	HIG = pinParameter(chunkData[2]);
	MID = pinParameter(chunkData[3]);
	LOW = pinParameter(chunkData[4]);
	CRS = pinParameter(chunkData[5]);
	TRF = pinParameter(chunkData[6]);
	TRG = pinParameter(chunkData[7]);
	TRB = pinParameter(chunkData[8]);
	HMF = pinParameter(chunkData[9]);
	HMG = pinParameter(chunkData[10]);
	HMB = pinParameter(chunkData[11]);
	LMF = pinParameter(chunkData[12]);
	LMG = pinParameter(chunkData[13]);
	LMB = pinParameter(chunkData[14]);
	BSF = pinParameter(chunkData[15]);
	BSG = pinParameter(chunkData[16]);
	BSB = pinParameter(chunkData[17]);
	THR = pinParameter(chunkData[18]);
	ATK = pinParameter(chunkData[19]);
	RLS = pinParameter(chunkData[20]);
	GAT = pinParameter(chunkData[21]);
	LOP = pinParameter(chunkData[22]);
	HIP = pinParameter(chunkData[23]);
	FAD = pinParameter(chunkData[24]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void ConsoleHPre::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParam_TRM: TRM = value; break;
        case kParam_MOR: MOR = value; break;
        case kParam_HIG: HIG = value; break;
        case kParam_MID: MID = value; break;
        case kParam_LOW: LOW = value; break;
        case kParam_CRS: CRS = value; break;
        case kParam_TRF: TRF = value; break;
        case kParam_TRG: TRG = value; break;
        case kParam_TRB: TRB = value; break;
        case kParam_HMF: HMF = value; break;
        case kParam_HMG: HMG = value; break;
        case kParam_HMB: HMB = value; break;
        case kParam_LMF: LMF = value; break;
        case kParam_LMG: LMG = value; break;
        case kParam_LMB: LMB = value; break;
        case kParam_BSF: BSF = value; break;
        case kParam_BSG: BSG = value; break;
        case kParam_BSB: BSB = value; break;
        case kParam_THR: THR = value; break;
        case kParam_ATK: ATK = value; break;
        case kParam_RLS: RLS = value; break;
        case kParam_GAT: GAT = value; break;
        case kParam_LOP: LOP = value; break;
        case kParam_HIP: HIP = value; break;
        case kParam_FAD: FAD = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float ConsoleHPre::getParameter(VstInt32 index) {
    switch (index) {
        case kParam_TRM: return TRM; break;
        case kParam_MOR: return MOR; break;
        case kParam_HIG: return HIG; break;
        case kParam_MID: return MID; break;
        case kParam_LOW: return LOW; break;
        case kParam_CRS: return CRS; break;
        case kParam_TRF: return TRF; break;
        case kParam_TRG: return TRG; break;
        case kParam_TRB: return TRB; break;
        case kParam_HMF: return HMF; break;
        case kParam_HMG: return HMG; break;
        case kParam_HMB: return HMB; break;
        case kParam_LMF: return LMF; break;
        case kParam_LMG: return LMG; break;
        case kParam_LMB: return LMB; break;
        case kParam_BSF: return BSF; break;
        case kParam_BSG: return BSG; break;
        case kParam_BSB: return BSB; break;
        case kParam_THR: return THR; break;
        case kParam_ATK: return ATK; break;
        case kParam_RLS: return RLS; break;
        case kParam_GAT: return GAT; break;
        case kParam_LOP: return LOP; break;
        case kParam_HIP: return HIP; break;
        case kParam_FAD: return FAD; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleHPre::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParam_TRM: vst_strncpy (text, "Trim", kVstMaxParamStrLen); break;
		case kParam_MOR: vst_strncpy (text, "More", kVstMaxParamStrLen); break;
		case kParam_HIG: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParam_MID: vst_strncpy (text, "Mid", kVstMaxParamStrLen); break;
		case kParam_LOW: vst_strncpy (text, "Low", kVstMaxParamStrLen); break;
		case kParam_CRS: vst_strncpy (text, "CrossFd", kVstMaxParamStrLen); break;
		case kParam_TRF: vst_strncpy (text, "Tr Freq", kVstMaxParamStrLen); break;
		case kParam_TRG: vst_strncpy (text, "Treble", kVstMaxParamStrLen); break;
		case kParam_TRB: vst_strncpy (text, "TrCrush", kVstMaxParamStrLen); break;
		case kParam_HMF: vst_strncpy (text, "HM Freq", kVstMaxParamStrLen); break;
		case kParam_HMG: vst_strncpy (text, "HighMid", kVstMaxParamStrLen); break;
		case kParam_HMB: vst_strncpy (text, "HMCrush", kVstMaxParamStrLen); break;
		case kParam_LMF: vst_strncpy (text, "LM Freq", kVstMaxParamStrLen); break;
		case kParam_LMG: vst_strncpy (text, "LowMid", kVstMaxParamStrLen); break;
		case kParam_LMB: vst_strncpy (text, "LMCrush", kVstMaxParamStrLen); break;
		case kParam_BSF: vst_strncpy (text, "Bs Freq", kVstMaxParamStrLen); break;
		case kParam_BSG: vst_strncpy (text, "Bass", kVstMaxParamStrLen); break;
		case kParam_BSB: vst_strncpy (text, "BsCrush", kVstMaxParamStrLen); break;
		case kParam_THR: vst_strncpy (text, "Thresh", kVstMaxParamStrLen); break;
		case kParam_ATK: vst_strncpy (text, "Attack", kVstMaxParamStrLen); break;
		case kParam_RLS: vst_strncpy (text, "Release", kVstMaxParamStrLen); break;
		case kParam_GAT: vst_strncpy (text, "Gate", kVstMaxParamStrLen); break;
		case kParam_LOP: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
		case kParam_HIP: vst_strncpy (text, "Hipass", kVstMaxParamStrLen); break;
		case kParam_FAD: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ConsoleHPre::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParam_TRM: int2string ((int)(TRM*4.0), text, kVstMaxParamStrLen); break;
        case kParam_MOR: float2string (MOR, text, kVstMaxParamStrLen); break;
        case kParam_HIG: float2string (HIG, text, kVstMaxParamStrLen); break;
        case kParam_MID: float2string (MID, text, kVstMaxParamStrLen); break;
        case kParam_LOW: float2string (LOW, text, kVstMaxParamStrLen); break;
        case kParam_CRS: float2string (CRS, text, kVstMaxParamStrLen); break;
        case kParam_TRF: float2string (TRF, text, kVstMaxParamStrLen); break;
        case kParam_TRG: float2string (TRG, text, kVstMaxParamStrLen); break;
        case kParam_TRB: float2string (TRB, text, kVstMaxParamStrLen); break;
        case kParam_HMF: float2string (HMF, text, kVstMaxParamStrLen); break;
        case kParam_HMG: float2string (HMG, text, kVstMaxParamStrLen); break;
        case kParam_HMB: float2string (HMB, text, kVstMaxParamStrLen); break;
        case kParam_LMF: float2string (LMF, text, kVstMaxParamStrLen); break;
        case kParam_LMG: float2string (LMG, text, kVstMaxParamStrLen); break;
        case kParam_LMB: float2string (LMB, text, kVstMaxParamStrLen); break;
        case kParam_BSF: float2string (BSF, text, kVstMaxParamStrLen); break;
        case kParam_BSG: float2string (BSG, text, kVstMaxParamStrLen); break;
        case kParam_BSB: float2string (BSB, text, kVstMaxParamStrLen); break;
        case kParam_THR: float2string (THR, text, kVstMaxParamStrLen); break;
        case kParam_ATK: float2string (ATK, text, kVstMaxParamStrLen); break;
        case kParam_RLS: float2string (RLS, text, kVstMaxParamStrLen); break;
        case kParam_GAT: float2string (GAT, text, kVstMaxParamStrLen); break;
        case kParam_LOP: float2string (LOP, text, kVstMaxParamStrLen); break;
        case kParam_HIP: float2string (HIP, text, kVstMaxParamStrLen); break;
        case kParam_FAD: float2string (FAD, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ConsoleHPre::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParam_TRM: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_MOR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_HIG: vst_strncpy (text, "eq", kVstMaxParamStrLen); break;
        case kParam_MID: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_LOW: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_CRS: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_TRF: vst_strncpy (text, "crush", kVstMaxParamStrLen); break;
        case kParam_TRG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_TRB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_HMF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_HMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_HMB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_LMF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_LMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_LMB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_BSF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_BSG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_BSB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_THR: vst_strncpy (text, "dyn", kVstMaxParamStrLen); break;
        case kParam_ATK: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_RLS: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_GAT: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_LOP: vst_strncpy (text, "fltr", kVstMaxParamStrLen); break;
        case kParam_HIP: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParam_FAD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ConsoleHPre::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleHPre::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleHPre", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleHPre::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleHPre::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleHPre", kVstMaxProductStrLen); return true;
}

bool ConsoleHPre::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
