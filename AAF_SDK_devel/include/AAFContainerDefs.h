#ifndef __ContainerDefinition_h__
#define __ContainerDefinition_h__

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

// AAF well-known ContainerDefinition instances
//

//{4313b572-d8ba-11d2-809b-006008143e6f}
//80.9b.00.60.08.14.3e.6f.43.13.b5.72.d8.ba.11.d2
const aafUID_t kAAFContainerDef_External =
{0x4313b572, 0xd8ba, 0x11d2, {0x80, 0x9b, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{4b1c1a46-03f2-11d4-80fb-006008143e6f}
//80.fb.00.60.08.14.3e.6f.4b.1c.1a.46.03.f2.11.d4
const aafUID_t kAAFContainerDef_OMF =
{0x4b1c1a46, 0x03f2, 0x11d4, {0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{4313b571-d8ba-11d2-809b-006008143e6f}
//80.9b.00.60.08.14.3e.6f.43.13.b5.71.d8.ba.11.d2
const aafUID_t kAAFContainerDef_AAF =
{0x4313b571, 0xd8ba, 0x11d2, {0x80, 0x9b, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{42464141-000d-4d4f-060e-2b34010101ff}
//06.0e.2b.34.01.01.01.ff.42.46.41.41.00.0d.4d.4f
const aafUID_t kAAFContainerDef_AAFMSS =
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};


//{4b464141-000d-4d4f-060e-2b34010101ff}
//06.0e.2b.34.01.01.01.ff.4b.46.41.41.00.0d.4d.4f
const aafUID_t kAAFContainerDef_AAFKLV =
{0x4b464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};


//{58464141-000d-4d4f-060e-2b34010101ff}
//06.0e.2b.34.01.01.01.ff.58.46.41.41.00.0d.4d.4f
const aafUID_t kAAFContainerDef_AAFXML =
{0x58464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};


//{0d010301-0201-0101-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.01.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_DefinedTemplate =
{0x0d010301, 0x0201, 0x0101, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0102-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.01.02
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_ExtendedTemplate =
{0x0d010301, 0x0201, 0x0102, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-017f-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.01.01.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_PictureOnly =
{0x0d010301, 0x0201, 0x017f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0201-017f-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.01.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_PictureOnly_Bogus =
{0x0d010301, 0x0201, 0x017f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0201-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.02.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_DefinedTemplate =
{0x0d010301, 0x0201, 0x0201, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0202-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.02.02
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_ExtendedTemplate =
{0x0d010301, 0x0201, 0x0202, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-027f-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.01.02.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_PictureOnly =
{0x0d010301, 0x0201, 0x027f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0201-027f-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.02.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_PictureOnly_Bogus =
{0x0d010301, 0x0201, 0x027f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0301-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.03.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_DefinedTemplate =
{0x0d010301, 0x0201, 0x0301, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0302-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.03.02
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_ExtendedTemplate =
{0x0d010301, 0x0201, 0x0302, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-037f-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.01.03.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_PictureOnly =
{0x0d010301, 0x0201, 0x037f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0201-037f-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.03.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_PictureOnly_Bogus =
{0x0d010301, 0x0201, 0x037f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0401-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.04.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_DefinedTemplate =
{0x0d010301, 0x0201, 0x0401, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0402-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.04.02
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_ExtendedTemplate =
{0x0d010301, 0x0201, 0x0402, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-047f-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.01.04.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_PictureOnly =
{0x0d010301, 0x0201, 0x047f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0201-047f-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.04.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_PictureOnly_Bogus =
{0x0d010301, 0x0201, 0x047f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0501-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.05.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_DefinedTemplate =
{0x0d010301, 0x0201, 0x0501, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0502-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.05.02
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_ExtendedTemplate =
{0x0d010301, 0x0201, 0x0502, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-057f-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.01.05.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_PictureOnly =
{0x0d010301, 0x0201, 0x057f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0201-057f-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.05.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_PictureOnly_Bogus =
{0x0d010301, 0x0201, 0x057f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0601-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.06.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_DefinedTemplate =
{0x0d010301, 0x0201, 0x0601, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-0602-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.06.02
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_ExtendedTemplate =
{0x0d010301, 0x0201, 0x0602, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0201-067f-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.01.06.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_PictureOnly =
{0x0d010301, 0x0201, 0x067f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0201-067f-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.01.06.7f
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_PictureOnly_Bogus =
{0x0d010301, 0x0201, 0x067f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-0101-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.01.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps =
{0x0d010301, 0x0202, 0x0101, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-0102-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.01.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps =
{0x0d010301, 0x0202, 0x0102, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-0201-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.02.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps =
{0x0d010301, 0x0202, 0x0201, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-0202-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.02.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps =
{0x0d010301, 0x0202, 0x0202, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-0301-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.03.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps_SMPTE322M =
{0x0d010301, 0x0202, 0x0301, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-0302-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.03.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps_SMPTE322M =
{0x0d010301, 0x0202, 0x0302, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-0401-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.04.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps_SMPTE322M =
{0x0d010301, 0x0202, 0x0401, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-0402-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.04.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps_SMPTE322M =
{0x0d010301, 0x0202, 0x0402, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-3f01-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.3f.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_IECDV_UndefinedSource_25Mbps =
{0x0d010301, 0x0202, 0x3f01, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-3f02-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.3f.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_IECDV_UndefinedSource_25Mbps =
{0x0d010301, 0x0202, 0x3f02, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-4001-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.40.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_25Mbps =
{0x0d010301, 0x0202, 0x4001, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-4002-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.40.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_25Mbps =
{0x0d010301, 0x0202, 0x4002, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-4002-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.02.40.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_25Mbps_Bogus =
{0x0d010301, 0x0202, 0x4002, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0202-4101-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.41.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_25Mbps =
{0x0d010301, 0x0202, 0x4101, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-4102-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.41.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_25Mbps =
{0x0d010301, 0x0202, 0x4102, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-5001-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.50.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_50Mbps =
{0x0d010301, 0x0202, 0x5001, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-5002-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.50.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_50Mbps =
{0x0d010301, 0x0202, 0x5002, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-5101-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.51.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_625x50I_50Mbps =
{0x0d010301, 0x0202, 0x5101, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-5102-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.51.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_50Mbps =
{0x0d010301, 0x0202, 0x5102, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-6001-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.60.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_1080x5994I_100Mbps =
{0x0d010301, 0x0202, 0x6001, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-6002-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.60.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x5994I_100Mbps =
{0x0d010301, 0x0202, 0x6002, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-6101-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.61.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_1080x50I_100Mbps =
{0x0d010301, 0x0202, 0x6101, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-6102-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.61.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_1080x50I_100Mbps =
{0x0d010301, 0x0202, 0x6102, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-6201-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.62.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_720x5994P_100Mbps =
{0x0d010301, 0x0202, 0x6201, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-6202-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.62.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_720x5994P_100Mbps =
{0x0d010301, 0x0202, 0x6202, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-6301-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.63.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_720x50P_100Mbps =
{0x0d010301, 0x0202, 0x6301, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-6302-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.63.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_720x50P_100Mbps =
{0x0d010301, 0x0202, 0x6302, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-7f01-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.7f.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_DVbased_UndefinedSource =
{0x0d010301, 0x0202, 0x7f01, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-7f02-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.02.7f.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_UndefinedSource =
{0x0d010301, 0x0202, 0x7f02, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0202-7f02-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.02.7f.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_DVbased_UndefinedSource_Bogus =
{0x0d010301, 0x0202, 0x7f02, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0204-4000-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.04.40.00
const aafUID_t kAAFContainerDef_MXFGC_MPEGES_AudioStream0_SID_Node =
{0x0d010301, 0x0204, 0x4000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0204-6001-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.04.60.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_MPEGES_VideoStream0_SID =
{0x0d010301, 0x0204, 0x6001, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0204-6001-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.04.60.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_MPEGES_VideoStream0_SID_Bogus =
{0x0d010301, 0x0204, 0x6001, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0204-6107-060e-2b3404010102}
//06.0e.2b.34.04.01.01.02.0d.01.03.01.02.04.61.07
const aafUID_t kAAFContainerDef_MXFGC_CustomClosedGOPwrapped_MPEGES_VideoStream1_SID =
{0x0d010301, 0x0204, 0x6107, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x02}};


//{0d010301-0204-6107-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.04.61.07
const aafUID_t kAAFContainerDef_MXFGC_CustomClosedGOPwrapped_MPEGES_VideoStream1_SID_Bogus =
{0x0d010301, 0x0204, 0x6107, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-0101-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994I_720_422 =
{0x0d010301, 0x0205, 0x0101, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-0102-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994I_720_422 =
{0x0d010301, 0x0205, 0x0102, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-0103-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.03
const aafUID_t kAAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994I_720_422 =
{0x0d010301, 0x0205, 0x0103, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-0105-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.05
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50I_720_422 =
{0x0d010301, 0x0205, 0x0105, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-0106-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.06
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50I_720_422 =
{0x0d010301, 0x0205, 0x0106, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-0107-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.07
const aafUID_t kAAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50I_720_422 =
{0x0d010301, 0x0205, 0x0107, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-0119-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.19
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994P_960_422 =
{0x0d010301, 0x0205, 0x0119, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-011a-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.1a
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994P_960_422 =
{0x0d010301, 0x0205, 0x011a, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-011b-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.1b
const aafUID_t kAAFContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994P_960_422 =
{0x0d010301, 0x0205, 0x011b, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-011d-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.1d
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_Uncompressed_625x50P_960_422 =
{0x0d010301, 0x0205, 0x011d, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-011e-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.1e
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50P_960_422 =
{0x0d010301, 0x0205, 0x011e, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0205-011f-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.05.01.1f
const aafUID_t kAAFContainerDef_MXFGC_Linewrapped_Uncompressed_625x50P_960_422 =
{0x0d010301, 0x0205, 0x011f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0206-0100-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.06.01.00
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_Broadcast_Wave_audio_data =
{0x0d010301, 0x0206, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0206-0200-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.06.02.00
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_Broadcast_Wave_audio_data =
{0x0d010301, 0x0206, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0206-0300-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.06.03.00
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_AES3_audio_data =
{0x0d010301, 0x0206, 0x0300, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0206-0400-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.06.04.00
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_AES3_audio_data =
{0x0d010301, 0x0206, 0x0400, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-020a-0100-060e-2b3404010103}
//06.0e.2b.34.04.01.01.03.0d.01.03.01.02.0a.01.00
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_Alaw_Audio =
{0x0d010301, 0x020a, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x03}};


//{0d010301-020a-0100-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.0a.01.00
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_Alaw_Audio_Bogus =
{0x0d010301, 0x020a, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-020a-0200-060e-2b3404010103}
//06.0e.2b.34.04.01.01.03.0d.01.03.01.02.0a.02.00
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_Alaw_Audio =
{0x0d010301, 0x020a, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x03}};


//{0d010301-020a-0200-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.0a.02.00
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_Alaw_Audio_Bogus =
{0x0d010301, 0x020a, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-020a-0300-060e-2b3404010103}
//06.0e.2b.34.04.01.01.03.0d.01.03.01.02.0a.03.00
const aafUID_t kAAFContainerDef_MXFGC_Customwrapped_Alaw_Audio =
{0x0d010301, 0x020a, 0x0300, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x03}};


//{0d010301-020a-0300-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.0a.03.00
const aafUID_t kAAFContainerDef_MXFGC_Customwrapped_Alaw_Audio_Bogus =
{0x0d010301, 0x020a, 0x0300, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-020c-0100-060e-2b3404010107}
//06.0e.2b.34.04.01.01.07.0d.01.03.01.02.0c.01.00
const aafUID_t kAAFContainerDef_MXFGC_JPEG2000_FU_Framewrapped_Undefined_Interlace_Picture_Element =
{0x0d010301, 0x020c, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x07}};


//{0d010301-020c-0200-060e-2b3404010107}
//06.0e.2b.34.04.01.01.07.0d.01.03.01.02.0c.02.00
const aafUID_t kAAFContainerDef_MXFGC_JPEG2000_Cn_Clipwrapped_Picture_Element =
{0x0d010301, 0x020c, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x07}};


//{0d010301-020c-0300-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.0d.01.03.01.02.0c.03.00
const aafUID_t kAAFContainerDef_MXFGC_JPEG2000_I1_Interlaced_Frame_1_field_KLV =
{0x0d010301, 0x020c, 0x0300, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};


//{0d010301-020c-0400-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.0d.01.03.01.02.0c.04.00
const aafUID_t kAAFContainerDef_MXFGC_JPEG2000_I2_Interlaced_Frame_2_fields_KLV =
{0x0d010301, 0x020c, 0x0400, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};


//{0d010301-020c-0500-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.0d.01.03.01.02.0c.05.00
const aafUID_t kAAFContainerDef_MXFGC_JPEG2000_F1_Fieldwrapped_Picture_Element =
{0x0d010301, 0x020c, 0x0500, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};


//{0d010301-020c-0600-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.0d.01.03.01.02.0c.06.00
const aafUID_t kAAFContainerDef_MXFGC_JPEG2000_P1_Framewrapped_Picture_Element =
{0x0d010301, 0x020c, 0x0600, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};


//{0d010301-020d-0000-060e-2b3404010109}
//06.0e.2b.34.04.01.01.09.0d.01.03.01.02.0d.00.00
const aafUID_t kAAFContainerDef_MXFGC_Generic_VBI_Data_Mapping_Undefined_Payload =
{0x0d010301, 0x020d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x09}};


//{0d010301-020e-0000-060e-2b3404010109}
//06.0e.2b.34.04.01.01.09.0d.01.03.01.02.0e.00.00
const aafUID_t kAAFContainerDef_MXFGC_Generic_ANC_Data_Mapping_Undefined_Payload =
{0x0d010301, 0x020e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x09}};


//{0d010301-020e-0000-060e-2b3404010107}
//06.0e.2b.34.04.01.01.07.0d.01.03.01.02.0e.00.00
const aafUID_t kAAFContainerDef_MXFGC_Generic_ANC_Data_Mapping_Undefined_Payload_Bogus =
{0x0d010301, 0x020e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x07}};


//{0d010301-020f-6001-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6001, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-020f-6002-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6002, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-020f-6002-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.0f.60.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_AVCNALUnitStream_VideoStream0_SID_Bogus =
{0x0d010301, 0x020f, 0x6002, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-020f-6003-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.03
const aafUID_t kAAFContainerDef_MXFGC_CustomStripewrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6003, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-020f-6004-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.04
const aafUID_t kAAFContainerDef_MXFGC_CustomPESwrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6004, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-020f-6005-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.05
const aafUID_t kAAFContainerDef_MXFGC_CustomFixedAudioSizewrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6005, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-020f-6006-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.06
const aafUID_t kAAFContainerDef_MXFGC_CustomSplicewrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6006, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-020f-6007-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.07
const aafUID_t kAAFContainerDef_MXFGC_CustomClosedGOPwrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6007, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-020f-6008-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.08
const aafUID_t kAAFContainerDef_MXFGC_CustomSlavewrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6008, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-020f-6009-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.0d.01.03.01.02.0f.60.09
const aafUID_t kAAFContainerDef_MXFGC_Fieldwrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x6009, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};


//{0d010301-020f-607f-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.0f.60.7f
const aafUID_t kAAFContainerDef_MXFGC_CustomUnconstrainedwrapped_AVCNALUnitStream_VideoStream0_SID =
{0x0d010301, 0x020f, 0x607f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6001-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.01
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6001, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6002-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6002, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6002-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.03.01.02.10.60.02
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_AVCByteStream_VideoStream0_SID_Bogus =
{0x0d010301, 0x0210, 0x6002, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0d010301-0210-6003-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.03
const aafUID_t kAAFContainerDef_MXFGC_CustomStripewrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6003, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6004-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.04
const aafUID_t kAAFContainerDef_MXFGC_CustomPESwrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6004, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6005-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.05
const aafUID_t kAAFContainerDef_MXFGC_CustomFixedAudioSizewrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6005, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6006-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.06
const aafUID_t kAAFContainerDef_MXFGC_CustomSplicewrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6006, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6007-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.07
const aafUID_t kAAFContainerDef_MXFGC_CustomClosedGOPwrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6007, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6008-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.08
const aafUID_t kAAFContainerDef_MXFGC_CustomSlavewrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6008, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0210-6009-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.0d.01.03.01.02.10.60.09
const aafUID_t kAAFContainerDef_MXFGC_Fieldwrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x6009, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};


//{0d010301-0210-607f-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.10.60.7f
const aafUID_t kAAFContainerDef_MXFGC_CustomUnconstrainedwrapped_AVCByteStream_VideoStream0_SID =
{0x0d010301, 0x0210, 0x607f, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0211-0100-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.11.01.00
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_VC3 =
{0x0d010301, 0x0211, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0211-0200-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.11.02.00
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_VC3 =
{0x0d010301, 0x0211, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0212-0100-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.12.01.00
const aafUID_t kAAFContainerDef_MXFGC_Framewrapped_VC1 =
{0x0d010301, 0x0212, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-0212-0200-060e-2b340401010a}
//06.0e.2b.34.04.01.01.0a.0d.01.03.01.02.12.02.00
const aafUID_t kAAFContainerDef_MXFGC_Clipwrapped_VC1 =
{0x0d010301, 0x0212, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0a}};


//{0d010301-027f-0100-060e-2b3404010103}
//06.0e.2b.34.04.01.01.03.0d.01.03.01.02.7f.01.00
const aafUID_t kAAFContainerDef_MXFGC_Generic_Essence_Multiple_Mappings =
{0x0d010301, 0x027f, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x03}};


//{0d011301-0101-0100-060e-2b3404010106}
//06.0e.2b.34.04.01.01.06.0d.01.13.01.01.01.01.00
const aafUID_t kAAFContainerDef_RIFFWAVE =
{0x0d011301, 0x0101, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};


//{0d011301-0102-0200-060e-2b3404010107}
//06.0e.2b.34.04.01.01.07.0d.01.13.01.01.02.02.00
const aafUID_t kAAFContainerDef_JFIF =
{0x0d011301, 0x0102, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x07}};


//{0d011301-0104-0100-060e-2b3404010106}
//06.0e.2b.34.04.01.01.06.0d.01.13.01.01.04.01.00
const aafUID_t kAAFContainerDef_AIFFAIFC =
{0x0d011301, 0x0104, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};


//{0e040301-0206-0101-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.01.01
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_220X_1080p =
{0x0e040301, 0x0206, 0x0101, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0102-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.01.02
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_145_1080p =
{0x0e040301, 0x0206, 0x0102, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0103-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.01.03
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_220_1080p =
{0x0e040301, 0x0206, 0x0103, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0104-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.01.04
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_36_1080p =
{0x0e040301, 0x0206, 0x0104, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0201-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.02.01
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_220X_1080i =
{0x0e040301, 0x0206, 0x0201, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0202-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.02.02
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_145_1080i =
{0x0e040301, 0x0206, 0x0202, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0203-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.02.03
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_220_1080i =
{0x0e040301, 0x0206, 0x0203, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0204-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.02.04
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_145_1440_1080i =
{0x0e040301, 0x0206, 0x0204, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0301-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.03.01
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_220X_720p =
{0x0e040301, 0x0206, 0x0301, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0302-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.03.02
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_220_720p =
{0x0e040301, 0x0206, 0x0302, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{0e040301-0206-0303-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0e.04.03.01.02.06.03.03
const aafUID_t kAAFContainerDef_MXFGC_Avid_DNX_145_720p =
{0x0e040301, 0x0206, 0x0303, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


// AAF ContainerDefinition legacy aliases
//

const aafUID_t ContainerFile = kAAFContainerDef_External;
const aafUID_t ContainerOMF = kAAFContainerDef_OMF;
const aafUID_t ContainerAAF = kAAFContainerDef_AAF;
const aafUID_t ContainerAAFMSS = kAAFContainerDef_AAFMSS;
const aafUID_t ContainerAAFKLV = kAAFContainerDef_AAFKLV;
const aafUID_t ContainerAAFXML = kAAFContainerDef_AAFXML;

#endif // ! __ContainerDefinition_h__
