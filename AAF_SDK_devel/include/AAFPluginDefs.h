#ifndef __PluginDefinition_h__
#define __PluginDefinition_h__

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

// AAF well-known PluginDefinition instances
//

//{3d1dd891-e793-11d2-809e-006008143e6f}
//80.9e.00.60.08.14.3e.6f.3d.1d.d8.91.e7.93.11.d2
const aafUID_t kAAFPlatform_Independent =
{0x3d1dd891, 0xe793, 0x11d2, {0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{9fdef8c1-e847-11d2-809e-006008143e6f}
//80.9e.00.60.08.14.3e.6f.9f.de.f8.c1.e8.47.11.d2
const aafUID_t kAAFEngine_None =
{0x9fdef8c1, 0xe847, 0x11d2, {0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{69c870a1-e793-11d2-809e-006008143e6f}
//80.9e.00.60.08.14.3e.6f.69.c8.70.a1.e7.93.11.d2
const aafUID_t kAAFPluginAPI_EssenceAccess =
{0x69c870a1, 0xe793, 0x11d2, {0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{56905e0b-537d-11d4-a36c-009027dfca6a}
//a3.6c.00.90.27.df.ca.6a.56.90.5e.0b.53.7d.11.d4
const aafUID_t kAAFPluginCategory_None =
{0x56905e0b, 0x537d, 0x11d4, {0xa3, 0x6c, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}};


// AAF PluginDefinition legacy aliases
//

const aafUID_t kAAFPlatformIndependant = kAAFPlatform_Independent;
const aafUID_t kAAFNoEngine = kAAFEngine_None;
const aafUID_t kAAFEssencePluginAPI = kAAFPluginAPI_EssenceAccess;
const aafUID_t kAAFPluginNoCategory = kAAFPluginCategory_None;

#endif // ! __PluginDefinition_h__
