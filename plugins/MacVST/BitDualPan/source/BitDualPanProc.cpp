/* ========================================
 *  BitDualPan - BitDualPan.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BitDualPan_H
#include "BitDualPan.h"
#endif

void BitDualPan::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double gainControl = (A*0.5)+0.05; //0.0 to 1.0
	int gainBits = 20; //start beyond maximum attenuation
	if (gainControl > 0.0) gainBits = floor(1.0 / gainControl);
	int bitshiftL = gainBits - 3;
	int bitshiftR = gainBits - 3;
	double panControl = (B*2.0)-1.0; //-1.0 to 1.0
	double panAttenuation = (1.0-fabs(panControl));
	int panBits = 20; //start centered
	if (panAttenuation > 0.0) panBits = floor(1.0 / panAttenuation);
	if (panControl > 0.25) bitshiftL += panBits;
	if (panControl < -0.25) bitshiftR += panBits;
	if (bitshiftL < -2) bitshiftL = -2; if (bitshiftL > 17) bitshiftL = 17;
	if (bitshiftR < -2) bitshiftR = -2; if (bitshiftR > 17) bitshiftR = 17;
	double LgainL = 1.0;
	double LgainR = 1.0;
	switch (bitshiftL)
	{
		case 17: LgainL = 0.0; break;
		case 16: LgainL = 0.0000152587890625; break;
		case 15: LgainL = 0.000030517578125; break;
		case 14: LgainL = 0.00006103515625; break;
		case 13: LgainL = 0.0001220703125; break;
		case 12: LgainL = 0.000244140625; break;
		case 11: LgainL = 0.00048828125; break;
		case 10: LgainL = 0.0009765625; break;
		case 9: LgainL = 0.001953125; break;
		case 8: LgainL = 0.00390625; break;
		case 7: LgainL = 0.0078125; break;
		case 6: LgainL = 0.015625; break;
		case 5: LgainL = 0.03125; break;
		case 4: LgainL = 0.0625; break;
		case 3: LgainL = 0.125; break;
		case 2: LgainL = 0.25; break;
		case 1: LgainL = 0.5; break;
		case 0: LgainL = 1.0; break;
		case -1: LgainL = 2.0; break;
		case -2: LgainL = 4.0; break;
	}
	switch (bitshiftR)
	{
		case 17: LgainR = 0.0; break;
		case 16: LgainR = 0.0000152587890625; break;
		case 15: LgainR = 0.000030517578125; break;
		case 14: LgainR = 0.00006103515625; break;
		case 13: LgainR = 0.0001220703125; break;
		case 12: LgainR = 0.000244140625; break;
		case 11: LgainR = 0.00048828125; break;
		case 10: LgainR = 0.0009765625; break;
		case 9: LgainR = 0.001953125; break;
		case 8: LgainR = 0.00390625; break;
		case 7: LgainR = 0.0078125; break;
		case 6: LgainR = 0.015625; break;
		case 5: LgainR = 0.03125; break;
		case 4: LgainR = 0.0625; break;
		case 3: LgainR = 0.125; break;
		case 2: LgainR = 0.25; break;
		case 1: LgainR = 0.5; break;
		case 0: LgainR = 1.0; break;
		case -1: LgainR = 2.0; break;
		case -2: LgainR = 4.0; break;
	}
	
	gainControl = (C*0.5)+0.05; //0.0 to 1.0
	gainBits = 20; //start beyond maximum attenuation
	if (gainControl > 0.0) gainBits = floor(1.0 / gainControl);
	bitshiftL = gainBits - 3;
	bitshiftR = gainBits - 3;
	panControl = (D*2.0)-1.0; //-1.0 to 1.0
	panAttenuation = (1.0-fabs(panControl));
	panBits = 20; //start centered
	if (panAttenuation > 0.0) panBits = floor(1.0 / panAttenuation);
	if (panControl > 0.25) bitshiftL += panBits;
	if (panControl < -0.25) bitshiftR += panBits;
	if (bitshiftL < -2) bitshiftL = -2; if (bitshiftL > 17) bitshiftL = 17;
	if (bitshiftR < -2) bitshiftR = -2; if (bitshiftR > 17) bitshiftR = 17;
	double RgainL = 1.0;
	double RgainR = 1.0;
	switch (bitshiftL)
	{
		case 17: RgainL = 0.0; break;
		case 16: RgainL = 0.0000152587890625; break;
		case 15: RgainL = 0.000030517578125; break;
		case 14: RgainL = 0.00006103515625; break;
		case 13: RgainL = 0.0001220703125; break;
		case 12: RgainL = 0.000244140625; break;
		case 11: RgainL = 0.00048828125; break;
		case 10: RgainL = 0.0009765625; break;
		case 9: RgainL = 0.001953125; break;
		case 8: RgainL = 0.00390625; break;
		case 7: RgainL = 0.0078125; break;
		case 6: RgainL = 0.015625; break;
		case 5: RgainL = 0.03125; break;
		case 4: RgainL = 0.0625; break;
		case 3: RgainL = 0.125; break;
		case 2: RgainL = 0.25; break;
		case 1: RgainL = 0.5; break;
		case 0: RgainL = 1.0; break;
		case -1: RgainL = 2.0; break;
		case -2: RgainL = 4.0; break;
	}
	switch (bitshiftR)
	{
		case 17: RgainR = 0.0; break;
		case 16: RgainR = 0.0000152587890625; break;
		case 15: RgainR = 0.000030517578125; break;
		case 14: RgainR = 0.00006103515625; break;
		case 13: RgainR = 0.0001220703125; break;
		case 12: RgainR = 0.000244140625; break;
		case 11: RgainR = 0.00048828125; break;
		case 10: RgainR = 0.0009765625; break;
		case 9: RgainR = 0.001953125; break;
		case 8: RgainR = 0.00390625; break;
		case 7: RgainR = 0.0078125; break;
		case 6: RgainR = 0.015625; break;
		case 5: RgainR = 0.03125; break;
		case 4: RgainR = 0.0625; break;
		case 3: RgainR = 0.125; break;
		case 2: RgainR = 0.25; break;
		case 1: RgainR = 0.5; break;
		case 0: RgainR = 1.0; break;
		case -1: RgainR = 2.0; break;
		case -2: RgainR = 4.0; break;
	}
	
    while (--sampleFrames >= 0)
    {
		double monoL = *in1;
		double monoR = *in2;
				
		*out1 = (monoL*LgainL)+(monoR*RgainL);
		*out2 = (monoL*LgainR)+(monoR*RgainR);

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void BitDualPan::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double gainControl = (A*0.5)+0.05; //0.0 to 1.0
	int gainBits = 20; //start beyond maximum attenuation
	if (gainControl > 0.0) gainBits = floor(1.0 / gainControl);
	int bitshiftL = gainBits - 3;
	int bitshiftR = gainBits - 3;
	double panControl = (B*2.0)-1.0; //-1.0 to 1.0
	double panAttenuation = (1.0-fabs(panControl));
	int panBits = 20; //start centered
	if (panAttenuation > 0.0) panBits = floor(1.0 / panAttenuation);
	if (panControl > 0.25) bitshiftL += panBits;
	if (panControl < -0.25) bitshiftR += panBits;
	if (bitshiftL < -2) bitshiftL = -2; if (bitshiftL > 17) bitshiftL = 17;
	if (bitshiftR < -2) bitshiftR = -2; if (bitshiftR > 17) bitshiftR = 17;
	double LgainL = 1.0;
	double LgainR = 1.0;
	switch (bitshiftL)
	{
		case 17: LgainL = 0.0; break;
		case 16: LgainL = 0.0000152587890625; break;
		case 15: LgainL = 0.000030517578125; break;
		case 14: LgainL = 0.00006103515625; break;
		case 13: LgainL = 0.0001220703125; break;
		case 12: LgainL = 0.000244140625; break;
		case 11: LgainL = 0.00048828125; break;
		case 10: LgainL = 0.0009765625; break;
		case 9: LgainL = 0.001953125; break;
		case 8: LgainL = 0.00390625; break;
		case 7: LgainL = 0.0078125; break;
		case 6: LgainL = 0.015625; break;
		case 5: LgainL = 0.03125; break;
		case 4: LgainL = 0.0625; break;
		case 3: LgainL = 0.125; break;
		case 2: LgainL = 0.25; break;
		case 1: LgainL = 0.5; break;
		case 0: LgainL = 1.0; break;
		case -1: LgainL = 2.0; break;
		case -2: LgainL = 4.0; break;
	}
	switch (bitshiftR)
	{
		case 17: LgainR = 0.0; break;
		case 16: LgainR = 0.0000152587890625; break;
		case 15: LgainR = 0.000030517578125; break;
		case 14: LgainR = 0.00006103515625; break;
		case 13: LgainR = 0.0001220703125; break;
		case 12: LgainR = 0.000244140625; break;
		case 11: LgainR = 0.00048828125; break;
		case 10: LgainR = 0.0009765625; break;
		case 9: LgainR = 0.001953125; break;
		case 8: LgainR = 0.00390625; break;
		case 7: LgainR = 0.0078125; break;
		case 6: LgainR = 0.015625; break;
		case 5: LgainR = 0.03125; break;
		case 4: LgainR = 0.0625; break;
		case 3: LgainR = 0.125; break;
		case 2: LgainR = 0.25; break;
		case 1: LgainR = 0.5; break;
		case 0: LgainR = 1.0; break;
		case -1: LgainR = 2.0; break;
		case -2: LgainR = 4.0; break;
	}
	
	gainControl = (C*0.5)+0.05; //0.0 to 1.0
	gainBits = 20; //start beyond maximum attenuation
	if (gainControl > 0.0) gainBits = floor(1.0 / gainControl);
	bitshiftL = gainBits - 3;
	bitshiftR = gainBits - 3;
	panControl = (D*2.0)-1.0; //-1.0 to 1.0
	panAttenuation = (1.0-fabs(panControl));
	panBits = 20; //start centered
	if (panAttenuation > 0.0) panBits = floor(1.0 / panAttenuation);
	if (panControl > 0.25) bitshiftL += panBits;
	if (panControl < -0.25) bitshiftR += panBits;
	if (bitshiftL < -2) bitshiftL = -2; if (bitshiftL > 17) bitshiftL = 17;
	if (bitshiftR < -2) bitshiftR = -2; if (bitshiftR > 17) bitshiftR = 17;
	double RgainL = 1.0;
	double RgainR = 1.0;
	switch (bitshiftL)
	{
		case 17: RgainL = 0.0; break;
		case 16: RgainL = 0.0000152587890625; break;
		case 15: RgainL = 0.000030517578125; break;
		case 14: RgainL = 0.00006103515625; break;
		case 13: RgainL = 0.0001220703125; break;
		case 12: RgainL = 0.000244140625; break;
		case 11: RgainL = 0.00048828125; break;
		case 10: RgainL = 0.0009765625; break;
		case 9: RgainL = 0.001953125; break;
		case 8: RgainL = 0.00390625; break;
		case 7: RgainL = 0.0078125; break;
		case 6: RgainL = 0.015625; break;
		case 5: RgainL = 0.03125; break;
		case 4: RgainL = 0.0625; break;
		case 3: RgainL = 0.125; break;
		case 2: RgainL = 0.25; break;
		case 1: RgainL = 0.5; break;
		case 0: RgainL = 1.0; break;
		case -1: RgainL = 2.0; break;
		case -2: RgainL = 4.0; break;
	}
	switch (bitshiftR)
	{
		case 17: RgainR = 0.0; break;
		case 16: RgainR = 0.0000152587890625; break;
		case 15: RgainR = 0.000030517578125; break;
		case 14: RgainR = 0.00006103515625; break;
		case 13: RgainR = 0.0001220703125; break;
		case 12: RgainR = 0.000244140625; break;
		case 11: RgainR = 0.00048828125; break;
		case 10: RgainR = 0.0009765625; break;
		case 9: RgainR = 0.001953125; break;
		case 8: RgainR = 0.00390625; break;
		case 7: RgainR = 0.0078125; break;
		case 6: RgainR = 0.015625; break;
		case 5: RgainR = 0.03125; break;
		case 4: RgainR = 0.0625; break;
		case 3: RgainR = 0.125; break;
		case 2: RgainR = 0.25; break;
		case 1: RgainR = 0.5; break;
		case 0: RgainR = 1.0; break;
		case -1: RgainR = 2.0; break;
		case -2: RgainR = 4.0; break;
	}
	
    while (--sampleFrames >= 0)
    {
		double monoL = *in1;
		double monoR = *in2;
		
		*out1 = (monoL*LgainL)+(monoR*RgainL);
		*out2 = (monoL*LgainR)+(monoR*RgainR);
		
		in1++;
		in2++;
		out1++;
		out2++;
    }
}
