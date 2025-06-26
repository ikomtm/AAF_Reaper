
#ifndef __AAFAudioLabels_h__
#define __AAFAudioLabels_h__

//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2018, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"


//
// Audio Labels IDs and symobols for use with SMPTE ST 377-4 MXF Multichannel
// Audio Labeling Framework
//


//
// SMPTE Label identifiers for Audio Channel interpretive data
//

// {03020101-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.01.00.00.00.00
const aafUID_t kAAFAudioLabel_LeftAudioChannel =
{0x03020101, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_LeftAudioChannel = L"L";

// {03020102-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.02.00.00.00.00
const aafUID_t kAAFAudioLabel_RightAudioChannel =
{0x03020102, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_RightAudioChannel = L"R";

// {03020103-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.03.00.00.00.00
const aafUID_t kAAFAudioLabel_CenterAudioChannel =
{0x03020103, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_CenterAudioChannel = L"C";

// {03020104-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.04.00.00.00.00
const aafUID_t kAAFAudioLabel_LFEAudioChannel =
{0x03020104, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_LFEAudioChannel = L"LFE";

// {03020105-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.05.00.00.00.00
const aafUID_t kAAFAudioLabel_LeftSurroundAudioChannel =
{0x03020105, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_LeftSurroundAudioChannel = L"Ls";

// {03020106-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.06.00.00.00.00
const aafUID_t kAAFAudioLabel_RightSurroundAudioChannel =
{0x03020106, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_RightSurroundAudioChannel = L"Rs";

// {03020107-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.07.00.00.00.00
const aafUID_t kAAFAudioLabel_LeftSideSurroundAudioChannel =
{0x03020107, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_LeftSideSurroundAudioChannel = L"Lss";

// {03020108-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.08.00.00.00.00
const aafUID_t kAAFAudioLabel_RightSideSurroundAudioChannel =
{0x03020108, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_RightSideSurroundAudioChannel = L"Rss";

// {03020109-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.09.00.00.00.00
const aafUID_t kAAFAudioLabel_LeftRearSurroundAudioChannel =
{0x03020109, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_LeftRearSurroundAudioChannel = L"Lrs";

// {0302010a-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.0a.00.00.00.00
const aafUID_t kAAFAudioLabel_RightRearSurroundAudioChannel =
{0x0302010a, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_RightRearSurroundAudioChannel = L"Rrs";

// {0302010b-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.0b.00.00.00.00
const aafUID_t kAAFAudioLabel_LeftCenterAudioChannel =
{0x0302010b, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_LeftCenterAudioChannel = L"Lc";

// {0302010c-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.0c.00.00.00.00
const aafUID_t kAAFAudioLabel_RightCenterAudioChannel =
{0x0302010c, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_RightCenterAudioChannel = L"Rc";

// {0302010d-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.0d.00.00.00.00
const aafUID_t kAAFAudioLabel_CenterSurroundAudioChannel =
{0x0302010d, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_CenterSurroundAudioChannel = L"Cs";

// {0302010e-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.0e.00.00.00.00
const aafUID_t kAAFAudioLabel_HearingImpairedAudioChannel =
{0x0302010e, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_HearingImpairedAudioChannel = L"HI";

// {0302010f-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.0f.00.00.00.00
const aafUID_t kAAFAudioLabel_VisuallyImpairedNarrativeAudioChannel =
{0x0302010f, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_VisuallyImpairedNarrativeAudioChannel = L"VIN";

//
// SMPTE Label identifiers for SMPTE ST 2067-8 Audio Channel interpretive data
//

// {03020120-0100-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.01.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678MonoOne =
{0x03020120, 0x0100, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678MonoOne = L"M1";

// {03020120-0200-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.02.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678MonoTwo =
{0x03020120, 0x0200, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678MonoTwo = L"M2";

// {03020120-0300-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.03.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678LeftTotal =
{0x03020120, 0x0300, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678LeftTotal = L"Lt";

// {03020120-0400-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.04.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678RightTotal =
{0x03020120, 0x0400, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678RightTotal = L"Rt";

// {03020120-0500-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.05.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678LeftSurroundTotal =
{0x03020120, 0x0500, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678LeftSurroundTotal = L"Lst";

// {03020120-0600-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.06.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678RightSurroundTotal =
{0x03020120, 0x0600, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678RightSurroundTotal = L"Rst";

// {03020120-0700-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.07.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678Surround =
{0x03020120, 0x0700, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678Surround = L"S";

//
// SMPTE Label identifiers for SMPTE ST 2067-8 Audio Channels numbered between 001 and 127
//

// {03020120-0801-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.01.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel001 =
{0x03020120, 0x0801, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel001 = L"NSC001";

// {03020120-0802-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.02.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel002 =
{0x03020120, 0x0802, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel002 = L"NSC002";

// {03020120-0803-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.03.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel003 =
{0x03020120, 0x0803, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel003 = L"NSC003";

// {03020120-0804-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.04.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel004 =
{0x03020120, 0x0804, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel004 = L"NSC004";

// {03020120-0805-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.05.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel005 =
{0x03020120, 0x0805, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel005 = L"NSC005";

// {03020120-0806-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.06.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel006 =
{0x03020120, 0x0806, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel006 = L"NSC006";

// {03020120-0807-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.07.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel007 =
{0x03020120, 0x0807, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel007 = L"NSC007";

// {03020120-0808-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.08.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel008 =
{0x03020120, 0x0808, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel008 = L"NSC008";

// {03020120-0809-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.09.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel009 =
{0x03020120, 0x0809, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel009 = L"NSC009";

// {03020120-080a-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.0a.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel0A =
{0x03020120, 0x080a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel0A = L"NSC00A";

// {03020120-080b-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.0b.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel0B =
{0x03020120, 0x080b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel0B = L"NSC00B";

// {03020120-080c-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.0c.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel0C =
{0x03020120, 0x080c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel0C = L"NSC00C";

// {03020120-080d-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.0d.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel0D =
{0x03020120, 0x080d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel0D = L"NSC00D";

// {03020120-080e-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.0e.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel0E =
{0x03020120, 0x080e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel0E = L"NSC00E";

// {03020120-080f-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.0f.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel0F =
{0x03020120, 0x080f, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel0F = L"NSC00F";

// {03020120-0810-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.10.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel010 =
{0x03020120, 0x0810, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel010 = L"NSC010";

// {03020120-0811-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.11.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel011 =
{0x03020120, 0x0811, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel011 = L"NSC011";

// {03020120-0812-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.12.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel012 =
{0x03020120, 0x0812, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel012 = L"NSC012";

// {03020120-0813-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.13.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel013 =
{0x03020120, 0x0813, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel013 = L"NSC013";

// {03020120-0814-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.14.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel014 =
{0x03020120, 0x0814, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel014 = L"NSC014";

// {03020120-0815-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.15.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel015 =
{0x03020120, 0x0815, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel015 = L"NSC015";

// {03020120-0816-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.16.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel016 =
{0x03020120, 0x0816, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel016 = L"NSC016";

// {03020120-0817-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.17.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel017 =
{0x03020120, 0x0817, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel017 = L"NSC017";

// {03020120-0818-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.18.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel018 =
{0x03020120, 0x0818, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel018 = L"NSC018";

// {03020120-0819-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.19.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel019 =
{0x03020120, 0x0819, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel019 = L"NSC019";

// {03020120-081a-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.1a.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel1A =
{0x03020120, 0x081a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel1A = L"NSC01A";

// {03020120-081b-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.1b.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel1B =
{0x03020120, 0x081b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel1B = L"NSC01B";

// {03020120-081c-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.1c.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel1C =
{0x03020120, 0x081c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel1C = L"NSC01C";

// {03020120-081d-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.1d.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel1D =
{0x03020120, 0x081d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel1D = L"NSC01D";

// {03020120-081e-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.1e.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel1E =
{0x03020120, 0x081e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel1E = L"NSC01E";

// {03020120-081f-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.1f.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel1F =
{0x03020120, 0x081f, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel1F = L"NSC01F";

// {03020120-0820-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.20.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel020 =
{0x03020120, 0x0820, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel020 = L"NSC020";

// {03020120-0821-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.21.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel021 =
{0x03020120, 0x0821, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel021 = L"NSC021";

// {03020120-0822-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.22.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel022 =
{0x03020120, 0x0822, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel022 = L"NSC022";

// {03020120-0823-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.23.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel023 =
{0x03020120, 0x0823, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel023 = L"NSC023";

// {03020120-0824-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.24.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel024 =
{0x03020120, 0x0824, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel024 = L"NSC024";

// {03020120-0825-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.25.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel025 =
{0x03020120, 0x0825, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel025 = L"NSC025";

// {03020120-0826-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.26.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel026 =
{0x03020120, 0x0826, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel026 = L"NSC026";

// {03020120-0827-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.27.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel027 =
{0x03020120, 0x0827, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel027 = L"NSC027";

// {03020120-0828-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.28.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel028 =
{0x03020120, 0x0828, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel028 = L"NSC028";

// {03020120-0829-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.29.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel029 =
{0x03020120, 0x0829, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel029 = L"NSC029";

// {03020120-082a-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.2a.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel2A =
{0x03020120, 0x082a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel2A = L"NSC02A";

// {03020120-082b-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.2b.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel2B =
{0x03020120, 0x082b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel2B = L"NSC02B";

// {03020120-082c-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.2c.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel2C =
{0x03020120, 0x082c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel2C = L"NSC02C";

// {03020120-082d-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.2d.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel2D =
{0x03020120, 0x082d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel2D = L"NSC02D";

// {03020120-082e-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.2e.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel2E =
{0x03020120, 0x082e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel2E = L"NSC02E";

// {03020120-082f-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.2f.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel2F =
{0x03020120, 0x082f, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel2F = L"NSC02F";

// {03020120-0830-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.30.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel030 =
{0x03020120, 0x0830, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel030 = L"NSC030";

// {03020120-0831-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.31.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel031 =
{0x03020120, 0x0831, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel031 = L"NSC031";

// {03020120-0832-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.32.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel032 =
{0x03020120, 0x0832, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel032 = L"NSC032";

// {03020120-0833-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.33.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel033 =
{0x03020120, 0x0833, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel033 = L"NSC033";

// {03020120-0834-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.34.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel034 =
{0x03020120, 0x0834, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel034 = L"NSC034";

// {03020120-0835-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.35.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel035 =
{0x03020120, 0x0835, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel035 = L"NSC035";

// {03020120-0836-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.36.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel036 =
{0x03020120, 0x0836, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel036 = L"NSC036";

// {03020120-0837-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.37.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel037 =
{0x03020120, 0x0837, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel037 = L"NSC037";

// {03020120-0838-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.38.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel038 =
{0x03020120, 0x0838, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel038 = L"NSC038";

// {03020120-0839-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.39.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel039 =
{0x03020120, 0x0839, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel039 = L"NSC039";

// {03020120-083a-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.3a.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel3A =
{0x03020120, 0x083a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel3A = L"NSC03A";

// {03020120-083b-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.3b.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel3B =
{0x03020120, 0x083b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel3B = L"NSC03B";

// {03020120-083c-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.3c.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel3C =
{0x03020120, 0x083c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel3C = L"NSC03C";

// {03020120-083d-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.3d.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel3D =
{0x03020120, 0x083d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel3D = L"NSC03D";

// {03020120-083e-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.3e.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel3E =
{0x03020120, 0x083e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel3E = L"NSC03E";

// {03020120-083f-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.3f.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel3F =
{0x03020120, 0x083f, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel3F = L"NSC03F";

// {03020120-0840-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.40.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel040 =
{0x03020120, 0x0840, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel040 = L"NSC040";

// {03020120-0841-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.41.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel041 =
{0x03020120, 0x0841, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel041 = L"NSC041";

// {03020120-0842-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.42.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel042 =
{0x03020120, 0x0842, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel042 = L"NSC042";

// {03020120-0843-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.43.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel043 =
{0x03020120, 0x0843, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel043 = L"NSC043";

// {03020120-0844-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.44.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel044 =
{0x03020120, 0x0844, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel044 = L"NSC044";

// {03020120-0845-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.45.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel045 =
{0x03020120, 0x0845, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel045 = L"NSC045";

// {03020120-0846-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.46.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel046 =
{0x03020120, 0x0846, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel046 = L"NSC046";

// {03020120-0847-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.47.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel047 =
{0x03020120, 0x0847, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel047 = L"NSC047";

// {03020120-0848-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.48.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel048 =
{0x03020120, 0x0848, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel048 = L"NSC048";

// {03020120-0849-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.49.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel049 =
{0x03020120, 0x0849, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel049 = L"NSC049";

// {03020120-084a-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.4a.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel4A =
{0x03020120, 0x084a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel4A = L"NSC04A";

// {03020120-084b-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.4b.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel4B =
{0x03020120, 0x084b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel4B = L"NSC04B";

// {03020120-084c-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.4c.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel4C =
{0x03020120, 0x084c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel4C = L"NSC04C";

// {03020120-084d-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.4d.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel4D =
{0x03020120, 0x084d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel4D = L"NSC04D";

// {03020120-084e-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.4e.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel4E =
{0x03020120, 0x084e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel4E = L"NSC04E";

// {03020120-084f-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.4f.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel4F =
{0x03020120, 0x084f, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel4F = L"NSC04F";

// {03020120-0850-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.50.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel050 =
{0x03020120, 0x0850, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel050 = L"NSC050";

// {03020120-0851-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.51.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel051 =
{0x03020120, 0x0851, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel051 = L"NSC051";

// {03020120-0852-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.52.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel052 =
{0x03020120, 0x0852, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel052 = L"NSC052";

// {03020120-0853-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.53.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel053 =
{0x03020120, 0x0853, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel053 = L"NSC053";

// {03020120-0854-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.54.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel054 =
{0x03020120, 0x0854, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel054 = L"NSC054";

// {03020120-0855-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.55.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel055 =
{0x03020120, 0x0855, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel055 = L"NSC055";

// {03020120-0856-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.56.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel056 =
{0x03020120, 0x0856, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel056 = L"NSC056";

// {03020120-0857-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.57.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel057 =
{0x03020120, 0x0857, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel057 = L"NSC057";

// {03020120-0858-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.58.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel058 =
{0x03020120, 0x0858, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel058 = L"NSC058";

// {03020120-0859-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.59.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel059 =
{0x03020120, 0x0859, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel059 = L"NSC059";

// {03020120-085a-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.5a.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel5A =
{0x03020120, 0x085a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel5A = L"NSC05A";

// {03020120-085b-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.5b.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel5B =
{0x03020120, 0x085b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel5B = L"NSC05B";

// {03020120-085c-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.5c.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel5C =
{0x03020120, 0x085c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel5C = L"NSC05C";

// {03020120-085d-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.5d.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel5D =
{0x03020120, 0x085d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel5D = L"NSC05D";

// {03020120-085e-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.5e.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel5E =
{0x03020120, 0x085e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel5E = L"NSC05E";

// {03020120-085f-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.5f.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel5F =
{0x03020120, 0x085f, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel5F = L"NSC05F";

// {03020120-0860-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.60.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel060 =
{0x03020120, 0x0860, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel060 = L"NSC060";

// {03020120-0861-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.61.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel061 =
{0x03020120, 0x0861, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel061 = L"NSC061";

// {03020120-0862-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.62.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel062 =
{0x03020120, 0x0862, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel062 = L"NSC062";

// {03020120-0863-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.63.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel063 =
{0x03020120, 0x0863, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel063 = L"NSC063";

// {03020120-0864-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.64.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel064 =
{0x03020120, 0x0864, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel064 = L"NSC064";

// {03020120-0865-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.65.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel065 =
{0x03020120, 0x0865, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel065 = L"NSC065";

// {03020120-0866-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.66.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel066 =
{0x03020120, 0x0866, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel066 = L"NSC066";

// {03020120-0867-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.67.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel067 =
{0x03020120, 0x0867, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel067 = L"NSC067";

// {03020120-0868-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.68.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel068 =
{0x03020120, 0x0868, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel068 = L"NSC068";

// {03020120-0869-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.69.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel069 =
{0x03020120, 0x0869, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel069 = L"NSC069";

// {03020120-086a-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.6a.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel6A =
{0x03020120, 0x086a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel6A = L"NSC06A";

// {03020120-086b-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.6b.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel6B =
{0x03020120, 0x086b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel6B = L"NSC06B";

// {03020120-086c-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.6c.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel6C =
{0x03020120, 0x086c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel6C = L"NSC06C";

// {03020120-086d-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.6d.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel6D =
{0x03020120, 0x086d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel6D = L"NSC06D";

// {03020120-086e-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.6e.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel6E =
{0x03020120, 0x086e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel6E = L"NSC06E";

// {03020120-086f-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.6f.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel6F =
{0x03020120, 0x086f, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel6F = L"NSC06F";

// {03020120-0870-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.70.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel070 =
{0x03020120, 0x0870, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel070 = L"NSC070";

// {03020120-0871-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.71.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel071 =
{0x03020120, 0x0871, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel071 = L"NSC071";

// {03020120-0872-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.72.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel072 =
{0x03020120, 0x0872, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel072 = L"NSC072";

// {03020120-0873-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.73.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel073 =
{0x03020120, 0x0873, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel073 = L"NSC073";

// {03020120-0874-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.74.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel074 =
{0x03020120, 0x0874, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel074 = L"NSC074";

// {03020120-0875-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.75.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel075 =
{0x03020120, 0x0875, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel075 = L"NSC075";

// {03020120-0876-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.76.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel076 =
{0x03020120, 0x0876, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel076 = L"NSC076";

// {03020120-0877-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.77.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel077 =
{0x03020120, 0x0877, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel077 = L"NSC077";

// {03020120-0878-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.78.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel078 =
{0x03020120, 0x0878, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel078 = L"NSC078";

// {03020120-0879-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.79.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel079 =
{0x03020120, 0x0879, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel079 = L"NSC079";

// {03020120-087a-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.7a.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel7A =
{0x03020120, 0x087a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel7A = L"NSC07A";

// {03020120-087b-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.7b.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel7B =
{0x03020120, 0x087b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel7B = L"NSC07B";

// {03020120-087c-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.7c.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel7C =
{0x03020120, 0x087c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel7C = L"NSC07C";

// {03020120-087d-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.7d.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel7D =
{0x03020120, 0x087d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel7D = L"NSC07D";

// {03020120-087e-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.7e.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel7E =
{0x03020120, 0x087e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel7E = L"NSC07E";

// {03020120-087f-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.01.20.08.7f.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678NumberedSourceChannel7F =
{0x03020120, 0x087f, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678NumberedSourceChannel7F = L"NSC07F";

//
// SMPTE Label identifiers for Soundfield Groups interpretive data
//

// {03020201-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.01.00.00.00.00
const aafUID_t kAAFAudioLabel_51SoundfieldGroup =
{0x03020201, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_51SoundfieldGroup = L"51";

// {03020202-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.02.00.00.00.00
const aafUID_t kAAFAudioLabel_71DSSoundfieldGroup =
{0x03020202, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_71DSSoundfieldGroup = L"71";

// {03020203-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.03.00.00.00.00
const aafUID_t kAAFAudioLabel_71SDSSoundfieldGroup =
{0x03020203, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_71SDSSoundfieldGroup = L"SDS";

// {03020204-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.04.00.00.00.00
const aafUID_t kAAFAudioLabel_61SoundfieldGroup =
{0x03020204, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_61SoundfieldGroup = L"61";

// {03020205-0000-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.05.00.00.00.00
const aafUID_t kAAFAudioLabel_10MonauralSoundfieldGroup =
{0x03020205, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_10MonauralSoundfieldGroup = L"M";

//
// SMPTE Label identifiers for SMPTE ST 2067-8 Soundfield Groups
//

// {03020220-0100-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.01.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678StandardStereo =
{0x03020220, 0x0100, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678StandardStereo = L"ST";

// {03020220-0200-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.02.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678DualMono =
{0x03020220, 0x0200, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678DualMono = L"DM";

// {03020220-0300-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.03.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678DiscreteNumberedSources =
{0x03020220, 0x0300, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678DiscreteNumberedSources = L"DNS";

// {03020220-0400-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.04.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST2067830 =
{0x03020220, 0x0400, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST2067830 = L"30";

// {03020220-0500-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.05.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST2067840 =
{0x03020220, 0x0500, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST2067840 = L"40";

// {03020220-0600-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.06.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST2067850 =
{0x03020220, 0x0600, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST2067850 = L"50";

// {03020220-0700-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.07.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST2067860 =
{0x03020220, 0x0700, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST2067860 = L"60";

// {03020220-0800-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.08.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST2067870DS =
{0x03020220, 0x0800, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST2067870DS = L"70";

// {03020220-0900-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.09.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678LtRt =
{0x03020220, 0x0900, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678LtRt = L"LtRt";

// {03020220-0a00-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.0a.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST2067851EX =
{0x03020220, 0x0a00, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST2067851EX = L"51EX";

// {03020220-0b00-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.0b.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678HearingAccessibility =
{0x03020220, 0x0b00, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678HearingAccessibility = L"HA";

// {03020220-0c00-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.02.20.0c.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678VisualAccessibility =
{0x03020220, 0x0c00, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678VisualAccessibility = L"VA";

//
// SMPTE Label identifiers for Groups of Soundfield Groups interpretive data
//

//
// SMPTE Label identifiers for SMPTE ST 2067-8 Groups of Soundfield Groups
//

// {03020320-0100-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.03.20.01.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678MainProgram =
{0x03020320, 0x0100, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678MainProgram = L"MPg";

// {03020320-0200-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.03.20.02.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678DescriptiveVideoService =
{0x03020320, 0x0200, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678DescriptiveVideoService = L"DVS";

// {03020320-0300-0000-060e-2b340401010d}
// 06.0e.2b.34.04.01.01.0d.03.02.03.20.03.00.00.00
const aafUID_t kAAFAudioLabel_SMPTEST20678DialogCentricMix =
{0x03020320, 0x0300, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};
//
const aafCharacter* kAAFAudioLabelSymbol_SMPTEST20678DialogCentricMix = L"Dcm";


#endif  // __AAFAudioLabels_h__


