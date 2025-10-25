/* ========================================
 *  SoftClock2 - SoftClock2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SoftClock2_H
#include "SoftClock2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new SoftClock2(audioMaster);}

SoftClock2::SoftClock2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.2;
	B = 0.2;
	C = 0.2;
	D = 0.0;
	E = 0.0;
	F = 0.0;
	G = 0.5;
	H = 0.5;
	I = 0.5;
	
	sinePos = 0.0;
	barPos = 0.0;
	beatPos = 0;
	for (int x = 0; x < 34; x++) {beatAccent[x] = 0.0; beatSwing[x] = 0.0;}
	inc = 0.0;
	beatTable[0]=0;
	beatTable[1]=1;
	beatTable[2]=2;
	beatTable[3]=3;
	beatTable[4]=4;
	beatTable[5]=5;
	beatTable[6]=6;
	beatTable[7]=7;
	beatTable[8]=8;
	beatTable[9]=9;
	beatTable[10]=10;
	beatTable[11]=11;
	beatTable[12]=11;
	beatTable[13]=11;
	beatTable[14]=11;
	beatTable[15]=13;
	beatTable[16]=16;
	beatTable[17]=13;
	beatTable[18]=13;
	beatTable[19]=17;
	beatTable[20]=17;
	beatTable[21]=17;
	beatTable[22]=17;
	beatTable[23]=19;
	beatTable[24]=24;
	beatTable[25]=19;
	beatTable[26]=19;
	beatTable[27]=19;
	beatTable[28]=23;
	beatTable[29]=23;
	beatTable[30]=23;
	beatTable[31]=23;
	beatTable[32]=32;
	beatTable[33]=32;
	beatTable[34]=32;
	
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

SoftClock2::~SoftClock2() {}
VstInt32 SoftClock2::getVendorVersion () {return 1000;}
void SoftClock2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void SoftClock2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 SoftClock2::getChunk (void** data, bool isPreset)
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

VstInt32 SoftClock2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
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

void SoftClock2::setParameter(VstInt32 index, float value) {
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

float SoftClock2::getParameter(VstInt32 index) {
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

void SoftClock2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Tempo", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Count", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Tuple", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Swing", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Peak", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Valley", kVstMaxParamStrLen); break;
		case kParamG: vst_strncpy (text, "Accents", kVstMaxParamStrLen); break;
		case kParamH: vst_strncpy (text, "Boost", kVstMaxParamStrLen); break;
		case kParamI: vst_strncpy (text, "Speed", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void SoftClock2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((int)(A*200.99)+40, text, kVstMaxParamStrLen); break;
        case kParamB: int2string ((int)(B*32.99), text, kVstMaxParamStrLen); break;
        case kParamC: int2string ((int)(C*16.99), text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        case kParamG: float2string (G, text, kVstMaxParamStrLen); break;
        case kParamH: float2string (H, text, kVstMaxParamStrLen); break;
        case kParamI: float2string (I, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void SoftClock2::getParameterLabel(VstInt32 index, char *text) {
	int beatCode = (int)(B*32.99);
    switch (index) {
        case kParamA: vst_strncpy (text, "bpm", kVstMaxParamStrLen); break;
        case kParamB:
			switch (beatCode){
			case 0:	vst_strncpy (text, "0", kVstMaxParamStrLen); break;
			case 1:	vst_strncpy (text, "1", kVstMaxParamStrLen); break;
			case 2:	vst_strncpy (text, "2", kVstMaxParamStrLen); break;
			case 3:	vst_strncpy (text, "3", kVstMaxParamStrLen); break;
			case 4:	vst_strncpy (text, "4", kVstMaxParamStrLen); break;
			case 5:	vst_strncpy (text, "5", kVstMaxParamStrLen); break;
			case 6:	vst_strncpy (text, "6", kVstMaxParamStrLen); break;
			case 7:	vst_strncpy (text, "7", kVstMaxParamStrLen); break;
			case 8:	vst_strncpy (text, "8", kVstMaxParamStrLen); break;
			case 9:	vst_strncpy (text, "9", kVstMaxParamStrLen); break;
			case 10:vst_strncpy (text, "10", kVstMaxParamStrLen); break;
			case 11:vst_strncpy (text, "11", kVstMaxParamStrLen); break;
			case 12:vst_strncpy (text, "11", kVstMaxParamStrLen); break;
			case 13:vst_strncpy (text, "11", kVstMaxParamStrLen); break;
			case 14:vst_strncpy (text, "11", kVstMaxParamStrLen); break;
			case 15:vst_strncpy (text, "13", kVstMaxParamStrLen); break;
			case 16:vst_strncpy (text, "16", kVstMaxParamStrLen); break;
			case 17:vst_strncpy (text, "13", kVstMaxParamStrLen); break;
			case 18:vst_strncpy (text, "13", kVstMaxParamStrLen); break;
			case 19:vst_strncpy (text, "17", kVstMaxParamStrLen); break;
			case 20:vst_strncpy (text, "17", kVstMaxParamStrLen); break;
			case 21:vst_strncpy (text, "17", kVstMaxParamStrLen); break;
			case 22:vst_strncpy (text, "17", kVstMaxParamStrLen); break;
			case 23:vst_strncpy (text, "19", kVstMaxParamStrLen); break;
			case 24:vst_strncpy (text, "24", kVstMaxParamStrLen); break;
			case 25:vst_strncpy (text, "19", kVstMaxParamStrLen); break;
			case 26:vst_strncpy (text, "19", kVstMaxParamStrLen); break;
			case 27:vst_strncpy (text, "19", kVstMaxParamStrLen); break;
			case 28:vst_strncpy (text, "23", kVstMaxParamStrLen); break;
			case 29:vst_strncpy (text, "23", kVstMaxParamStrLen); break;
			case 30:vst_strncpy (text, "23", kVstMaxParamStrLen); break;
			case 31:vst_strncpy (text, "23", kVstMaxParamStrLen); break;
			case 32:vst_strncpy (text, "32", kVstMaxParamStrLen); break;
			default: break;			
			}			
			break;
        case kParamC: vst_strncpy (text, "th note", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamH: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamI: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 SoftClock2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool SoftClock2::getEffectName(char* name) {
    vst_strncpy(name, "SoftClock2", kVstMaxProductStrLen); return true;
}

VstPlugCategory SoftClock2::getPlugCategory() {return kPlugCategEffect;}

bool SoftClock2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows SoftClock2", kVstMaxProductStrLen); return true;
}

bool SoftClock2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
