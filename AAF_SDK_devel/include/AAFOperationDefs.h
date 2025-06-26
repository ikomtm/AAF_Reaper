#ifndef __OperationDefinition_h__
#define __OperationDefinition_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"

// AAF well-known OperationDefinition instances
//

//{0c3bea40-fc05-11d2-8a29-0050040ef7d2}
//8a.29.00.50.04.0e.f7.d2.0c.3b.ea.40.fc.05.11.d2
const aafUID_t kAAFOperationDef_VideoDissolve =
{0x0c3bea40, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0c3bea44-fc05-11d2-8a29-0050040ef7d2}
//8a.29.00.50.04.0e.f7.d2.0c.3b.ea.44.fc.05.11.d2
const aafUID_t kAAFOperationDef_SMPTEVideoWipe =
{0x0c3bea44, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea890-0968-11d3-8a38-0050040ef7d2}
//8a.38.00.50.04.0e.f7.d2.9d.2e.a8.90.09.68.11.d3
const aafUID_t kAAFOperationDef_VideoSpeedControl =
{0x9d2ea890, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea891-0968-11d3-8a38-0050040ef7d2}
//8a.38.00.50.04.0e.f7.d2.9d.2e.a8.91.09.68.11.d3
const aafUID_t kAAFOperationDef_VideoRepeat =
{0x9d2ea891, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{f1db0f32-8d64-11d3-80df-006008143e6f}
//80.df.00.60.08.14.3e.6f.f1.db.0f.32.8d.64.11.d3
const aafUID_t kAAFOperationDef_Flip =
{0xf1db0f32, 0x8d64, 0x11d3, {0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{f1db0f34-8d64-11d3-80df-006008143e6f}
//80.df.00.60.08.14.3e.6f.f1.db.0f.34.8d.64.11.d3
const aafUID_t kAAFOperationDef_Flop =
{0xf1db0f34, 0x8d64, 0x11d3, {0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{f1db0f33-8d64-11d3-80df-006008143e6f}
//80.df.00.60.08.14.3e.6f.f1.db.0f.33.8d.64.11.d3
const aafUID_t kAAFOperationDef_FlipFlop =
{0xf1db0f33, 0x8d64, 0x11d3, {0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{86f5711e-ee72-450c-a118-17cf3b175dff}
//a1.18.17.cf.3b.17.5d.ff.86.f5.71.1e.ee.72.45.0c
const aafUID_t kAAFOperationDef_VideoPosition =
{0x86f5711e, 0xee72, 0x450c, {0xa1, 0x18, 0x17, 0xcf, 0x3b, 0x17, 0x5d, 0xff}};


//{f5826680-26c5-4149-8554-43d3c7a3bc09}
//85.54.43.d3.c7.a3.bc.09.f5.82.66.80.26.c5.41.49
const aafUID_t kAAFOperationDef_VideoCrop =
{0xf5826680, 0x26c5, 0x4149, {0x85, 0x54, 0x43, 0xd3, 0xc7, 0xa3, 0xbc, 0x09}};


//{2e0a119d-e6f7-4bee-b5dc-6dd42988687e}
//b5.dc.6d.d4.29.88.68.7e.2e.0a.11.9d.e6.f7.4b.ee
const aafUID_t kAAFOperationDef_VideoScale =
{0x2e0a119d, 0xe6f7, 0x4bee, {0xb5, 0xdc, 0x6d, 0xd4, 0x29, 0x88, 0x68, 0x7e}};


//{f2ca330d-8d45-4db4-b1b5-136ab055586f}
//b1.b5.13.6a.b0.55.58.6f.f2.ca.33.0d.8d.45.4d.b4
const aafUID_t kAAFOperationDef_VideoRotate =
{0xf2ca330d, 0x8d45, 0x4db4, {0xb1, 0xb5, 0x13, 0x6a, 0xb0, 0x55, 0x58, 0x6f}};


//{21d5c51a-8acb-46d5-9392-5cae640c8836}
//93.92.5c.ae.64.0c.88.36.21.d5.c5.1a.8a.cb.46.d5
const aafUID_t kAAFOperationDef_VideoCornerPinning =
{0x21d5c51a, 0x8acb, 0x46d5, {0x93, 0x92, 0x5c, 0xae, 0x64, 0x0c, 0x88, 0x36}};


//{14db900e-d537-49f6-889b-012568fcc234}
//88.9b.01.25.68.fc.c2.34.14.db.90.0e.d5.37.49.f6
const aafUID_t kAAFOperationDef_VideoAlphaWithinVideoKey =
{0x14db900e, 0xd537, 0x49f6, {0x88, 0x9b, 0x01, 0x25, 0x68, 0xfc, 0xc2, 0x34}};


//{e599cb0f-ba5f-4192-9356-51eb19c08589}
//93.56.51.eb.19.c0.85.89.e5.99.cb.0f.ba.5f.41.92
const aafUID_t kAAFOperationDef_VideoSeparateAlphaKey =
{0xe599cb0f, 0xba5f, 0x4192, {0x93, 0x56, 0x51, 0xeb, 0x19, 0xc0, 0x85, 0x89}};


//{38ff7903-69e5-476b-be5a-eafc2000f011}
//be.5a.ea.fc.20.00.f0.11.38.ff.79.03.69.e5.47.6b
const aafUID_t kAAFOperationDef_VideoLuminanceKey =
{0x38ff7903, 0x69e5, 0x476b, {0xbe, 0x5a, 0xea, 0xfc, 0x20, 0x00, 0xf0, 0x11}};


//{30a315c2-71e5-4e82-a4ef-0513ee056b65}
//a4.ef.05.13.ee.05.6b.65.30.a3.15.c2.71.e5.4e.82
const aafUID_t kAAFOperationDef_VideoChromaKey =
{0x30a315c2, 0x71e5, 0x4e82, {0xa4, 0xef, 0x05, 0x13, 0xee, 0x05, 0x6b, 0x65}};


//{9d2ea894-0968-11d3-8a38-0050040ef7d2}
//8a.38.00.50.04.0e.f7.d2.9d.2e.a8.94.09.68.11.d3
const aafUID_t kAAFOperationDef_MonoAudioGain =
{0x9d2ea894, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea893-0968-11d3-8a38-0050040ef7d2}
//8a.38.00.50.04.0e.f7.d2.9d.2e.a8.93.09.68.11.d3
const aafUID_t kAAFOperationDef_MonoAudioPan =
{0x9d2ea893, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0c3bea41-fc05-11d2-8a29-0050040ef7d2}
//8a.29.00.50.04.0e.f7.d2.0c.3b.ea.41.fc.05.11.d2
const aafUID_t kAAFOperationDef_MonoAudioDissolve =
{0x0c3bea41, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{2311bd90-b5da-4285-aa3a-8552848779b3}
//aa.3a.85.52.84.87.79.b3.23.11.bd.90.b5.da.42.85
const aafUID_t kAAFOperationDef_TwoParameterMonoAudioDissolve =
{0x2311bd90, 0xb5da, 0x4285, {0xaa, 0x3a, 0x85, 0x52, 0x84, 0x87, 0x79, 0xb3}};


//{9bb90dfd-2aad-49af-b09c-8ba6cd5281d1}
//b0.9c.8b.a6.cd.52.81.d1.9b.b9.0d.fd.2a.ad.49.af
const aafUID_t kAAFOperationDef_VideoOpacity =
{0x9bb90dfd, 0x2aad, 0x49af, {0xb0, 0x9c, 0x8b, 0xa6, 0xcd, 0x52, 0x81, 0xd1}};


//{2c50831c-572e-4042-b1dd-55ed0b7c49df}
//b1.dd.55.ed.0b.7c.49.df.2c.50.83.1c.57.2e.40.42
const aafUID_t kAAFOperationDef_VideoTitle =
{0x2c50831c, 0x572e, 0x4042, {0xb1, 0xdd, 0x55, 0xed, 0x0b, 0x7c, 0x49, 0xdf}};


//{5aba98f8-f389-471f-8fee-dfde7ec7f9bb}
//8f.ee.df.de.7e.c7.f9.bb.5a.ba.98.f8.f3.89.47.1f
const aafUID_t kAAFOperationDef_VideoColor =
{0x5aba98f8, 0xf389, 0x471f, {0x8f, 0xee, 0xdf, 0xde, 0x7e, 0xc7, 0xf9, 0xbb}};


//{1575e350-fca3-11d2-8a2a-0050040ef7d2}
//8a.2a.00.50.04.0e.f7.d2.15.75.e3.50.fc.a3.11.d2
const aafUID_t kAAFOperationDef_Unknown =
{0x1575e350, 0xfca3, 0x11d2, {0x8a, 0x2a, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0c3bea43-fc05-11d2-8a29-0050040ef7d2}
//8a.29.00.50.04.0e.f7.d2.0c.3b.ea.43.fc.05.11.d2
const aafUID_t kAAFOperationDef_VideoFadeToBlack =
{0x0c3bea43, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0a3c75e0-fd82-11d2-8a2b-0050040ef7d2}
//8a.2b.00.50.04.0e.f7.d2.0a.3c.75.e0.fd.82.11.d2
const aafUID_t kAAFOperationDef_PictureWithMate =
{0x0a3c75e0, 0xfd82, 0x11d2, {0x8a, 0x2b, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea892-0968-11d3-8a38-0050040ef7d2}
//8a.38.00.50.04.0e.f7.d2.9d.2e.a8.92.09.68.11.d3
const aafUID_t kAAFOperationDef_VideoFrameToMask =
{0x9d2ea892, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0c3bea42-fc05-11d2-8a29-0050040ef7d2}
//8a.29.00.50.04.0e.f7.d2.0c.3b.ea.42.fc.05.11.d2
const aafUID_t kAAFOperationDef_StereoAudioDissolve =
{0x0c3bea42, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea895-0968-11d3-8a38-0050040ef7d2}
//8a.38.00.50.04.0e.f7.d2.9d.2e.a8.95.09.68.11.d3
const aafUID_t kAAFOperationDef_StereoAudioGain =
{0x9d2ea895, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{8d896ad0-2261-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.8d.89.6a.d0.22.61.11.d3
const aafUID_t kAAFOperationDef_MonoAudioMixdown =
{0x8d896ad0, 0x2261, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


// AAF OperationDefinition legacy aliases
//

const aafUID_t kAAFEffectVideoDissolve = kAAFOperationDef_VideoDissolve;
const aafUID_t kAAFEffectSMPTEVideoWipe = kAAFOperationDef_SMPTEVideoWipe;
const aafUID_t kAAFEffectVideoSpeedControl = kAAFOperationDef_VideoSpeedControl;
const aafUID_t kAAFEffectVideoRepeat = kAAFOperationDef_VideoRepeat;
const aafUID_t kAAFEffectMonoAudioGain = kAAFOperationDef_MonoAudioGain;
const aafUID_t kAAFEffectMonoAudioPan = kAAFOperationDef_MonoAudioPan;
const aafUID_t kAAFEffectMonoAudioDissolve = kAAFOperationDef_MonoAudioDissolve;
const aafUID_t kAAFOperationUnknown = kAAFOperationDef_Unknown;
const aafUID_t kAAFEffectVideoFadeToBlack = kAAFOperationDef_VideoFadeToBlack;
const aafUID_t kAAFEffectPictureWithMate = kAAFOperationDef_PictureWithMate;
const aafUID_t kAAFEffectVideoFrameToMask = kAAFOperationDef_VideoFrameToMask;
const aafUID_t kAAFEffectStereoAudioDissolve = kAAFOperationDef_StereoAudioDissolve;
const aafUID_t kAAFEffectStereoAudioGain = kAAFOperationDef_StereoAudioGain;
const aafUID_t kAAFEffectMonoAudioMixdown = kAAFOperationDef_MonoAudioMixdown;

#endif // ! __OperationDefinition_h__
