#ifndef __ParameterDefinition_h__
#define __ParameterDefinition_h__

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

// AAF well-known ParameterDefinition instances
//

//{e4962320-2267-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.e4.96.23.20.22.67.11.d3
const aafUID_t kAAFParameterDef_Level =
{0xe4962320, 0x2267, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{e4962323-2267-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.e4.96.23.23.22.67.11.d3
const aafUID_t kAAFParameterDef_SMPTEWipeNumber =
{0xe4962323, 0x2267, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba0-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a0.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTEReverse =
{0x9c894ba0, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{72559a80-24d7-11d3-8a50-0050040ef7d2}
//8a.50.00.50.04.0e.f7.d2.72.55.9a.80.24.d7.11.d3
const aafUID_t kAAFParameterDef_SpeedRatio =
{0x72559a80, 0x24d7, 0x11d3, {0x8a, 0x50, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{c573a510-071a-454f-b617-ad6ae69054c2}
//b6.17.ad.6a.e6.90.54.c2.c5.73.a5.10.07.1a.45.4f
const aafUID_t kAAFParameterDef_PositionOffsetX =
{0xc573a510, 0x071a, 0x454f, {0xb6, 0x17, 0xad, 0x6a, 0xe6, 0x90, 0x54, 0xc2}};


//{82e27478-1336-4ea3-bcb9-6b8f17864c42}
//bc.b9.6b.8f.17.86.4c.42.82.e2.74.78.13.36.4e.a3
const aafUID_t kAAFParameterDef_PositionOffsetY =
{0x82e27478, 0x1336, 0x4ea3, {0xbc, 0xb9, 0x6b, 0x8f, 0x17, 0x86, 0x4c, 0x42}};


//{d47b3377-318c-4657-a9d8-75811b6dc3d1}
//a9.d8.75.81.1b.6d.c3.d1.d4.7b.33.77.31.8c.46.57
const aafUID_t kAAFParameterDef_CropLeft =
{0xd47b3377, 0x318c, 0x4657, {0xa9, 0xd8, 0x75, 0x81, 0x1b, 0x6d, 0xc3, 0xd1}};


//{5ecc9dd5-21c1-462b-9fec-c2bd85f14033}
//9f.ec.c2.bd.85.f1.40.33.5e.cc.9d.d5.21.c1.46.2b
const aafUID_t kAAFParameterDef_CropRight =
{0x5ecc9dd5, 0x21c1, 0x462b, {0x9f, 0xec, 0xc2, 0xbd, 0x85, 0xf1, 0x40, 0x33}};


//{8170a539-9b55-4051-9d4e-46598d01b914}
//9d.4e.46.59.8d.01.b9.14.81.70.a5.39.9b.55.40.51
const aafUID_t kAAFParameterDef_CropTop =
{0x8170a539, 0x9b55, 0x4051, {0x9d, 0x4e, 0x46, 0x59, 0x8d, 0x01, 0xb9, 0x14}};


//{154ba82b-990a-4c80-9101-3037e28839a1}
//91.01.30.37.e2.88.39.a1.15.4b.a8.2b.99.0a.4c.80
const aafUID_t kAAFParameterDef_CropBottom =
{0x154ba82b, 0x990a, 0x4c80, {0x91, 0x01, 0x30, 0x37, 0xe2, 0x88, 0x39, 0xa1}};


//{8d568129-847e-11d5-935a-50f857c10000}
//93.5a.50.f8.57.c1.00.00.8d.56.81.29.84.7e.11.d5
const aafUID_t kAAFParameterDef_ScaleX =
{0x8d568129, 0x847e, 0x11d5, {0x93, 0x5a, 0x50, 0xf8, 0x57, 0xc1, 0x00, 0x00}};


//{8d56812a-847e-11d5-935a-50f857c10000}
//93.5a.50.f8.57.c1.00.00.8d.56.81.2a.84.7e.11.d5
const aafUID_t kAAFParameterDef_ScaleY =
{0x8d56812a, 0x847e, 0x11d5, {0x93, 0x5a, 0x50, 0xf8, 0x57, 0xc1, 0x00, 0x00}};


//{062cfbd8-f4b1-4a50-b944-f39e2fc73c17}
//b9.44.f3.9e.2f.c7.3c.17.06.2c.fb.d8.f4.b1.4a.50
const aafUID_t kAAFParameterDef_Rotation =
{0x062cfbd8, 0xf4b1, 0x4a50, {0xb9, 0x44, 0xf3, 0x9e, 0x2f, 0xc7, 0x3c, 0x17}};


//{72a3b4a2-873d-4733-9052-9f83a706ca5b}
//90.52.9f.83.a7.06.ca.5b.72.a3.b4.a2.87.3d.47.33
const aafUID_t kAAFParameterDef_PinTopLeftX =
{0x72a3b4a2, 0x873d, 0x4733, {0x90, 0x52, 0x9f, 0x83, 0xa7, 0x06, 0xca, 0x5b}};


//{29e4d78f-a502-4ebb-8c07-ed5a0320c1b0}
//8c.07.ed.5a.03.20.c1.b0.29.e4.d7.8f.a5.02.4e.bb
const aafUID_t kAAFParameterDef_PinTopLeftY =
{0x29e4d78f, 0xa502, 0x4ebb, {0x8c, 0x07, 0xed, 0x5a, 0x03, 0x20, 0xc1, 0xb0}};


//{a95296c0-1ed9-4925-8481-2096c72e818d}
//84.81.20.96.c7.2e.81.8d.a9.52.96.c0.1e.d9.49.25
const aafUID_t kAAFParameterDef_PinTopRightX =
{0xa95296c0, 0x1ed9, 0x4925, {0x84, 0x81, 0x20, 0x96, 0xc7, 0x2e, 0x81, 0x8d}};


//{ce1757ae-7a0b-45d9-b3f3-3686adff1e2d}
//b3.f3.36.86.ad.ff.1e.2d.ce.17.57.ae.7a.0b.45.d9
const aafUID_t kAAFParameterDef_PinTopRightY =
{0xce1757ae, 0x7a0b, 0x45d9, {0xb3, 0xf3, 0x36, 0x86, 0xad, 0xff, 0x1e, 0x2d}};


//{08b2bc81-9b1b-4c01-ba73-bba3554ed029}
//ba.73.bb.a3.55.4e.d0.29.08.b2.bc.81.9b.1b.4c.01
const aafUID_t kAAFParameterDef_PinBottomLeftX =
{0x08b2bc81, 0x9b1b, 0x4c01, {0xba, 0x73, 0xbb, 0xa3, 0x55, 0x4e, 0xd0, 0x29}};


//{c163f2ff-cd83-4655-826e-3724ab7fa092}
//82.6e.37.24.ab.7f.a0.92.c1.63.f2.ff.cd.83.46.55
const aafUID_t kAAFParameterDef_PinBottomLeftY =
{0xc163f2ff, 0xcd83, 0x4655, {0x82, 0x6e, 0x37, 0x24, 0xab, 0x7f, 0xa0, 0x92}};


//{53bc5884-897f-479e-b833-191f8692100d}
//b8.33.19.1f.86.92.10.0d.53.bc.58.84.89.7f.47.9e
const aafUID_t kAAFParameterDef_PinBottomRightX =
{0x53bc5884, 0x897f, 0x479e, {0xb8, 0x33, 0x19, 0x1f, 0x86, 0x92, 0x10, 0x0d}};


//{812fb15b-0b95-4406-878d-efaa1cffc129}
//87.8d.ef.aa.1c.ff.c1.29.81.2f.b1.5b.0b.95.44.06
const aafUID_t kAAFParameterDef_PinBottomRightY =
{0x812fb15b, 0x0b95, 0x4406, {0x87, 0x8d, 0xef, 0xaa, 0x1c, 0xff, 0xc1, 0x29}};


//{a2667f65-65d8-4abf-a179-0b9b93413949}
//a1.79.0b.9b.93.41.39.49.a2.66.7f.65.65.d8.4a.bf
const aafUID_t kAAFParameterDef_AlphaKeyInvertAlpha =
{0xa2667f65, 0x65d8, 0x4abf, {0xa1, 0x79, 0x0b, 0x9b, 0x93, 0x41, 0x39, 0x49}};


//{21ed5b0f-b7a0-43bc-b779-c47f85bf6c4d}
//b7.79.c4.7f.85.bf.6c.4d.21.ed.5b.0f.b7.a0.43.bc
const aafUID_t kAAFParameterDef_LumKeyLevel =
{0x21ed5b0f, 0xb7a0, 0x43bc, {0xb7, 0x79, 0xc4, 0x7f, 0x85, 0xbf, 0x6c, 0x4d}};


//{cbd39b25-3ece-441e-ba2c-da473ab5cc7c}
//ba.2c.da.47.3a.b5.cc.7c.cb.d3.9b.25.3e.ce.44.1e
const aafUID_t kAAFParameterDef_LumKeyClip =
{0xcbd39b25, 0x3ece, 0x441e, {0xba, 0x2c, 0xda, 0x47, 0x3a, 0xb5, 0xcc, 0x7c}};


//{e4962321-2267-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.e4.96.23.21.22.67.11.d3
const aafUID_t kAAFParameterDef_Amplitude =
{0xe4962321, 0x2267, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{e4962322-2267-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.e4.96.23.22.22.67.11.d3
const aafUID_t kAAFParameterDef_Pan =
{0xe4962322, 0x2267, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9e610007-1be2-41e1-bb11-c95de9964d03}
//bb.11.c9.5d.e9.96.4d.03.9e.61.00.07.1b.e2.41.e1
const aafUID_t kAAFParameterDef_OutgoingLevel =
{0x9e610007, 0x1be2, 0x41e1, {0xbb, 0x11, 0xc9, 0x5d, 0xe9, 0x96, 0x4d, 0x03}};


//{48cea642-a8f9-455b-82b3-86c814b797c7}
//82.b3.86.c8.14.b7.97.c7.48.ce.a6.42.a8.f9.45.5b
const aafUID_t kAAFParameterDef_IncomingLevel =
{0x48cea642, 0xa8f9, 0x455b, {0x82, 0xb3, 0x86, 0xc8, 0x14, 0xb7, 0x97, 0xc7}};


//{cb7c0ec4-f45f-4ee6-aef0-c63ddb134924}
//ae.f0.c6.3d.db.13.49.24.cb.7c.0e.c4.f4.5f.4e.e6
const aafUID_t kAAFParameterDef_OpacityLevel =
{0xcb7c0ec4, 0xf45f, 0x4ee6, {0xae, 0xf0, 0xc6, 0x3d, 0xdb, 0x13, 0x49, 0x24}};


//{7b92827b-5ae3-465e-b5f9-5ee21b070859}
//b5.f9.5e.e2.1b.07.08.59.7b.92.82.7b.5a.e3.46.5e
const aafUID_t kAAFParameterDef_TitleText =
{0x7b92827b, 0x5ae3, 0x465e, {0xb5, 0xf9, 0x5e, 0xe2, 0x1b, 0x07, 0x08, 0x59}};


//{e8eb7f50-602f-4a2f-8fb2-86c8826ccf24}
//8f.b2.86.c8.82.6c.cf.24.e8.eb.7f.50.60.2f.4a.2f
const aafUID_t kAAFParameterDef_TitleFontName =
{0xe8eb7f50, 0x602f, 0x4a2f, {0x8f, 0xb2, 0x86, 0xc8, 0x82, 0x6c, 0xcf, 0x24}};


//{01c55287-31b3-4f8f-bb87-c92f06eb7f5a}
//bb.87.c9.2f.06.eb.7f.5a.01.c5.52.87.31.b3.4f.8f
const aafUID_t kAAFParameterDef_TitleFontSize =
{0x01c55287, 0x31b3, 0x4f8f, {0xbb, 0x87, 0xc9, 0x2f, 0x06, 0xeb, 0x7f, 0x5a}};


//{dfe86f24-8a71-4dc5-83a2-988f583af711}
//83.a2.98.8f.58.3a.f7.11.df.e8.6f.24.8a.71.4d.c5
const aafUID_t kAAFParameterDef_TitleFontColorR =
{0xdfe86f24, 0x8a71, 0x4dc5, {0x83, 0xa2, 0x98, 0x8f, 0x58, 0x3a, 0xf7, 0x11}};


//{f9f41222-36d9-4650-bd5a-a17866cf86b9}
//bd.5a.a1.78.66.cf.86.b9.f9.f4.12.22.36.d9.46.50
const aafUID_t kAAFParameterDef_TitleFontColorG =
{0xf9f41222, 0x36d9, 0x4650, {0xbd, 0x5a, 0xa1, 0x78, 0x66, 0xcf, 0x86, 0xb9}};


//{f5ba87fa-cf72-4f37-a736-d7096fcb06f1}
//a7.36.d7.09.6f.cb.06.f1.f5.ba.87.fa.cf.72.4f.37
const aafUID_t kAAFParameterDef_TitleFontColorB =
{0xf5ba87fa, 0xcf72, 0x4f37, {0xa7, 0x36, 0xd7, 0x09, 0x6f, 0xcb, 0x06, 0xf1}};


//{47c1733f-6afb-4168-9b6d-476adfbae7ab}
//9b.6d.47.6a.df.ba.e7.ab.47.c1.73.3f.6a.fb.41.68
const aafUID_t kAAFParameterDef_TitleAlignment =
{0x47c1733f, 0x6afb, 0x4168, {0x9b, 0x6d, 0x47, 0x6a, 0xdf, 0xba, 0xe7, 0xab}};


//{8b5732c0-be8e-4332-aa71-5d866add777d}
//aa.71.5d.86.6a.dd.77.7d.8b.57.32.c0.be.8e.43.32
const aafUID_t kAAFParameterDef_TitleBold =
{0x8b5732c0, 0xbe8e, 0x4332, {0xaa, 0x71, 0x5d, 0x86, 0x6a, 0xdd, 0x77, 0x7d}};


//{e4a3c91b-f96a-4dd4-91d8-1ba32000ab72}
//91.d8.1b.a3.20.00.ab.72.e4.a3.c9.1b.f9.6a.4d.d4
const aafUID_t kAAFParameterDef_TitleItalic =
{0xe4a3c91b, 0xf96a, 0x4dd4, {0x91, 0xd8, 0x1b, 0xa3, 0x20, 0x00, 0xab, 0x72}};


//{a25061da-db25-402e-89ff-a6d0efa39444}
//89.ff.a6.d0.ef.a3.94.44.a2.50.61.da.db.25.40.2e
const aafUID_t kAAFParameterDef_TitlePositionX =
{0xa25061da, 0xdb25, 0x402e, {0x89, 0xff, 0xa6, 0xd0, 0xef, 0xa3, 0x94, 0x44}};


//{6151541f-9d3f-4a0e-a3f9-24cc60eea969}
//a3.f9.24.cc.60.ee.a9.69.61.51.54.1f.9d.3f.4a.0e
const aafUID_t kAAFParameterDef_TitlePositionY =
{0x6151541f, 0x9d3f, 0x4a0e, {0xa3, 0xf9, 0x24, 0xcc, 0x60, 0xee, 0xa9, 0x69}};


//{be2033da-723b-4146-ace0-3299e0ff342e}
//ac.e0.32.99.e0.ff.34.2e.be.20.33.da.72.3b.41.46
const aafUID_t kAAFParameterDef_ColorSlopeR =
{0xbe2033da, 0x723b, 0x4146, {0xac, 0xe0, 0x32, 0x99, 0xe0, 0xff, 0x34, 0x2e}};


//{7ca8e01b-c6d8-4b3f-b251-28a53e5b958f}
//b2.51.28.a5.3e.5b.95.8f.7c.a8.e0.1b.c6.d8.4b.3f
const aafUID_t kAAFParameterDef_ColorSlopeG =
{0x7ca8e01b, 0xc6d8, 0x4b3f, {0xb2, 0x51, 0x28, 0xa5, 0x3e, 0x5b, 0x95, 0x8f}};


//{1aeb007b-3cd5-4814-87b5-cbd6a3cdfe8d}
//87.b5.cb.d6.a3.cd.fe.8d.1a.eb.00.7b.3c.d5.48.14
const aafUID_t kAAFParameterDef_ColorSlopeB =
{0x1aeb007b, 0x3cd5, 0x4814, {0x87, 0xb5, 0xcb, 0xd6, 0xa3, 0xcd, 0xfe, 0x8d}};


//{4d1e65e0-85fc-4bb9-a264-13cf320a8539}
//a2.64.13.cf.32.0a.85.39.4d.1e.65.e0.85.fc.4b.b9
const aafUID_t kAAFParameterDef_ColorOffsetR =
{0x4d1e65e0, 0x85fc, 0x4bb9, {0xa2, 0x64, 0x13, 0xcf, 0x32, 0x0a, 0x85, 0x39}};


//{76f783e4-0bbd-41d7-b01e-f418c1602a6f}
//b0.1e.f4.18.c1.60.2a.6f.76.f7.83.e4.0b.bd.41.d7
const aafUID_t kAAFParameterDef_ColorOffsetG =
{0x76f783e4, 0x0bbd, 0x41d7, {0xb0, 0x1e, 0xf4, 0x18, 0xc1, 0x60, 0x2a, 0x6f}};


//{57110628-522d-4b48-8a28-75477ced984d}
//8a.28.75.47.7c.ed.98.4d.57.11.06.28.52.2d.4b.48
const aafUID_t kAAFParameterDef_ColorOffsetB =
{0x57110628, 0x522d, 0x4b48, {0x8a, 0x28, 0x75, 0x47, 0x7c, 0xed, 0x98, 0x4d}};


//{c2d79c3a-9263-40d9-827d-953ac6b88813}
//82.7d.95.3a.c6.b8.88.13.c2.d7.9c.3a.92.63.40.d9
const aafUID_t kAAFParameterDef_ColorPowerR =
{0xc2d79c3a, 0x9263, 0x40d9, {0x82, 0x7d, 0x95, 0x3a, 0xc6, 0xb8, 0x88, 0x13}};


//{524d52e6-86a3-4f41-864b-fb53b15b1d5d}
//86.4b.fb.53.b1.5b.1d.5d.52.4d.52.e6.86.a3.4f.41
const aafUID_t kAAFParameterDef_ColorPowerG =
{0x524d52e6, 0x86a3, 0x4f41, {0x86, 0x4b, 0xfb, 0x53, 0xb1, 0x5b, 0x1d, 0x5d}};


//{5f0cc7dc-907d-4153-bf00-1f3cdf3c05bb}
//bf.00.1f.3c.df.3c.05.bb.5f.0c.c7.dc.90.7d.41.53
const aafUID_t kAAFParameterDef_ColorPowerB =
{0x5f0cc7dc, 0x907d, 0x4153, {0xbf, 0x00, 0x1f, 0x3c, 0xdf, 0x3c, 0x05, 0xbb}};


//{0b135705-3312-4d03-ba89-be9ef45e5470}
//ba.89.be.9e.f4.5e.54.70.0b.13.57.05.33.12.4d.03
const aafUID_t kAAFParameterDef_ColorSaturation =
{0x0b135705, 0x3312, 0x4d03, {0xba, 0x89, 0xbe, 0x9e, 0xf4, 0x5e, 0x54, 0x70}};


//{f3b9466a-2579-4168-beb5-66b996919a3f}
//be.b5.66.b9.96.91.9a.3f.f3.b9.46.6a.25.79.41.68
const aafUID_t kAAFParameterDef_ColorCorrectionDescription =
{0xf3b9466a, 0x2579, 0x4168, {0xbe, 0xb5, 0x66, 0xb9, 0x96, 0x91, 0x9a, 0x3f}};


//{b0124dbe-7f97-443c-ae39-c49c1c53d728}
//ae.39.c4.9c.1c.53.d7.28.b0.12.4d.be.7f.97.44.3c
const aafUID_t kAAFParameterDef_ColorInputDescription =
{0xb0124dbe, 0x7f97, 0x443c, {0xae, 0x39, 0xc4, 0x9c, 0x1c, 0x53, 0xd7, 0x28}};


//{5a9dfc6f-611f-4db8-8eff-3b9cdb6e1220}
//8e.ff.3b.9c.db.6e.12.20.5a.9d.fc.6f.61.1f.4d.b8
const aafUID_t kAAFParameterDef_ColorViewingDescription =
{0x5a9dfc6f, 0x611f, 0x4db8, {0x8e, 0xff, 0x3b, 0x9c, 0xdb, 0x6e, 0x12, 0x20}};


//{9c894ba1-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a1.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTESoft =
{0x9c894ba1, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba2-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a2.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTEBorder =
{0x9c894ba2, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba3-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a3.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTEPosition =
{0x9c894ba3, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba4-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a4.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTEModulator =
{0x9c894ba4, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba5-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a5.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTEShadow =
{0x9c894ba5, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba6-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a6.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTETumble =
{0x9c894ba6, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba7-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a7.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTESpotlight =
{0x9c894ba7, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba8-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a8.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTEReplicationH =
{0x9c894ba8, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894ba9-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.a9.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTEReplicationV =
{0x9c894ba9, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9c894baa-2277-11d3-8a4c-0050040ef7d2}
//8a.4c.00.50.04.0e.f7.d2.9c.89.4b.aa.22.77.11.d3
const aafUID_t kAAFParameterDef_SMPTECheckerboard =
{0x9c894baa, 0x2277, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{5f1c2560-2415-11d3-8a4f-0050040ef7d2}
//8a.4f.00.50.04.0e.f7.d2.5f.1c.25.60.24.15.11.d3
const aafUID_t kAAFParameterDef_PhaseOffset =
{0x5f1c2560, 0x2415, 0x11d3, {0x8a, 0x4f, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


// AAF ParameterDefinition legacy aliases
//

const aafUID_t kAAFParameterDefLevel = kAAFParameterDef_Level;
const aafUID_t kAAFParameterDefSMPTEWipeNumber = kAAFParameterDef_SMPTEWipeNumber;
const aafUID_t kAAFParameterDefSMPTEReverse = kAAFParameterDef_SMPTEReverse;
const aafUID_t kAAFParameterDefSpeedRatio = kAAFParameterDef_SpeedRatio;
const aafUID_t kAAFParameterDefAmplitude = kAAFParameterDef_Amplitude;
const aafUID_t kAAFParameterDefPan = kAAFParameterDef_Pan;
const aafUID_t kAAFParameterDefSMPTESoft = kAAFParameterDef_SMPTESoft;
const aafUID_t kAAFParameterDefSMPTEBorder = kAAFParameterDef_SMPTEBorder;
const aafUID_t kAAFParameterDefSMPTEPosition = kAAFParameterDef_SMPTEPosition;
const aafUID_t kAAFParameterDefSMPTEModulator = kAAFParameterDef_SMPTEModulator;
const aafUID_t kAAFParameterDefSMPTEShadow = kAAFParameterDef_SMPTEShadow;
const aafUID_t kAAFParameterDefSMPTETumble = kAAFParameterDef_SMPTETumble;
const aafUID_t kAAFParameterDefSMPTESpotlight = kAAFParameterDef_SMPTESpotlight;
const aafUID_t kAAFParameterDefSMPTEReplicationH = kAAFParameterDef_SMPTEReplicationH;
const aafUID_t kAAFParameterDefSMPTEReplicationV = kAAFParameterDef_SMPTEReplicationV;
const aafUID_t kAAFParameterDefSMPTECheckerboard = kAAFParameterDef_SMPTECheckerboard;
const aafUID_t kAAFParameterDefPhaseOffset = kAAFParameterDef_PhaseOffset;

#endif // ! __ParameterDefinition_h__
