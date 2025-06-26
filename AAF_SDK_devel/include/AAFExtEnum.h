#ifndef __AAFExtEnum_h__
#define __AAFExtEnum_h__

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
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"

// AAF extensible enumeration member UIDs.
//

// Members of OperationCategoryType
//
//{0d010102-0101-0100-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.01.00
const aafUID_t kAAFOperationCategory_Effect =
{0x0d010102, 0x0101, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

// Members of TransferCharacteristicType
//
//{04010101-0101-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.01.01.00.00
const aafUID_t kAAFTransferCharacteristic_ITU470_PAL =
{0x04010101, 0x0101, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0102-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.01.02.00.00
const aafUID_t kAAFTransferCharacteristic_ITU709 =
{0x04010101, 0x0102, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0103-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.01.03.00.00
const aafUID_t kAAFTransferCharacteristic_SMPTE240M =
{0x04010101, 0x0103, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0104-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.01.04.00.00
const aafUID_t kAAFTransferCharacteristic_274M_296M =
{0x04010101, 0x0104, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0105-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.01.05.00.00
const aafUID_t kAAFTransferCharacteristic_ITU1361 =
{0x04010101, 0x0105, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0106-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.01.06.00.00
const aafUID_t kAAFTransferCharacteristic_linear =
{0x04010101, 0x0106, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0107-0000-060e-2b3404010108}
//06.0e.2b.34.04.01.01.08.04.01.01.01.01.07.00.00
const aafUID_t kAAFTransferCharacteristic_SMPTE_DCDM =
{0x04010101, 0x0107, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x08}};

//{04010101-0108-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.01.08.00.00
const aafUID_t kAAFTransferCharacteristic_IEC6196624_xvYCC =
{0x04010101, 0x0108, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};

//{04010101-0109-0000-060e-2b340401010e}
//06.0e.2b.34.04.01.01.0e.04.01.01.01.01.09.00.00
const aafUID_t kAAFTransferCharacteristic_ITU2020 =
{0x04010101, 0x0109, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0e}};

//{04010101-010a-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.01.0a.00.00
const aafUID_t kAAFTransferCharacteristic_SMPTEST2084 =
{0x04010101, 0x010a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};

//{04010101-010b-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.01.0b.00.00
const aafUID_t kAAFTransferCharacteristic_HLG_OETF =
{0x04010101, 0x010b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};

// Members of PluginCategoryType
//
//{0d010102-0101-0200-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.02.00
const aafUID_t kAAFPluginCategory_Effect =
{0x0d010102, 0x0101, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0300-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.03.00
const aafUID_t kAAFPluginCategory_Codec =
{0x0d010102, 0x0101, 0x0300, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0400-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.04.00
const aafUID_t kAAFPluginCategory_Interpolation =
{0x0d010102, 0x0101, 0x0400, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

// Members of UsageType
//
//{0d010102-0101-0500-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.05.00
const aafUID_t kAAFUsage_SubClip =
{0x0d010102, 0x0101, 0x0500, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0600-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.06.00
const aafUID_t kAAFUsage_AdjustedClip =
{0x0d010102, 0x0101, 0x0600, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0700-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.07.00
const aafUID_t kAAFUsage_TopLevel =
{0x0d010102, 0x0101, 0x0700, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0800-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.08.00
const aafUID_t kAAFUsage_LowerLevel =
{0x0d010102, 0x0101, 0x0800, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0900-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.0d.01.01.02.01.01.09.00
const aafUID_t kAAFUsage_Template =
{0x0d010102, 0x0101, 0x0900, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

// Members of ColorPrimariesType
//
//{04010101-0301-0000-060e-2b3404010106}
//06.0e.2b.34.04.01.01.06.04.01.01.01.03.01.00.00
const aafUID_t kAAFColorPrimaries_SMPTE170M =
{0x04010101, 0x0301, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};

//{04010101-0302-0000-060e-2b3404010106}
//06.0e.2b.34.04.01.01.06.04.01.01.01.03.02.00.00
const aafUID_t kAAFColorPrimaries_ITU470_PAL =
{0x04010101, 0x0302, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};

//{04010101-0303-0000-060e-2b3404010106}
//06.0e.2b.34.04.01.01.06.04.01.01.01.03.03.00.00
const aafUID_t kAAFColorPrimaries_ITU709 =
{0x04010101, 0x0303, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};

//{04010101-0304-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.03.04.00.00
const aafUID_t kAAFColorPrimaries_ITU2020 =
{0x04010101, 0x0304, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};

//{04010101-0305-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.03.05.00.00
const aafUID_t kAAFColorPrimaries_SMPTE_DCDM =
{0x04010101, 0x0305, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};

//{04010101-0306-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.03.06.00.00
const aafUID_t kAAFColorPrimaries_P3D65 =
{0x04010101, 0x0306, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};

// Members of CodingEquationsType
//
//{04010101-0201-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.02.01.00.00
const aafUID_t kAAFCodingEquations_ITU601 =
{0x04010101, 0x0201, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0202-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.02.02.00.00
const aafUID_t kAAFCodingEquations_ITU709 =
{0x04010101, 0x0202, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0203-0000-060e-2b3404010101}
//06.0e.2b.34.04.01.01.01.04.01.01.01.02.03.00.00
const aafUID_t kAAFCodingEquations_SMPTE240M =
{0x04010101, 0x0203, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0204-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.02.04.00.00
const aafUID_t kAAFCodingEquations_YCgCo =
{0x04010101, 0x0204, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};

//{04010101-0205-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.02.05.00.00
const aafUID_t kAAFCodingEquations_GBR =
{0x04010101, 0x0205, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};

//{04010101-0206-0000-060e-2b340401010d}
//06.0e.2b.34.04.01.01.0d.04.01.01.01.02.06.00.00
const aafUID_t kAAFCodingEquations_ITU2020_NCL =
{0x04010101, 0x0206, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x0d}};


#endif // ! __AAFExtEnum_h__
