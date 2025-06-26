#ifndef __AAFClassDefUIDs_h__
#define __AAFClassDefUIDs_h__

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

// AAF class definition UIDs.
//

// The AAF reference implementation uses shorter names than
// SMPTE. The names are shortened by the following aliases.
//
#define kAAFClassID_ClassDef                           kAAFClassID_ClassDefinition
#define kAAFClassID_CodecDef                           kAAFClassID_CodecDefinition
#define kAAFClassID_DataDef                            kAAFClassID_DataDefinition
#define kAAFClassID_DefObject                          kAAFClassID_DefinitionObject
#define kAAFClassID_Edgecode                           kAAFClassID_EdgeCode
#define kAAFClassID_OperationDef                       kAAFClassID_OperationDefinition
#define kAAFClassID_Object                             kAAFClassID_InterchangeObject
#define kAAFClassID_ParameterDef                       kAAFClassID_ParameterDefinition
#define kAAFClassID_InterpolationDef                   kAAFClassID_InterpolationDefinition
#define kAAFClassID_PropertyDef                        kAAFClassID_PropertyDefinition
#define kAAFClassID_TypeDef                            kAAFClassID_TypeDefinition
#define kAAFClassID_TypeDefCharacter                   kAAFClassID_TypeDefinitionCharacter
#define kAAFClassID_TypeDefEnum                        kAAFClassID_TypeDefinitionEnumeration
#define kAAFClassID_TypeDefExtEnum                     kAAFClassID_TypeDefinitionExtendibleEnumeration
#define kAAFClassID_TypeDefFixedArray                  kAAFClassID_TypeDefinitionFixedArray
#define kAAFClassID_TypeDefInt                         kAAFClassID_TypeDefinitionInteger
#define kAAFClassID_TypeDefRecord                      kAAFClassID_TypeDefinitionRecord
#define kAAFClassID_TypeDefRename                      kAAFClassID_TypeDefinitionRename
#define kAAFClassID_TypeDefSet                         kAAFClassID_TypeDefinitionSet
#define kAAFClassID_TypeDefStream                      kAAFClassID_TypeDefinitionStream
#define kAAFClassID_TypeDefString                      kAAFClassID_TypeDefinitionString
#define kAAFClassID_TypeDefIndirect                    kAAFClassID_TypeDefinitionIndirect
#define kAAFClassID_TypeDefOpaque                      kAAFClassID_TypeDefinitionOpaque
#define kAAFClassID_TypeDefStrongObjRef                kAAFClassID_TypeDefinitionStrongObjectReference
#define kAAFClassID_TypeDefVariableArray               kAAFClassID_TypeDefinitionVariableArray
#define kAAFClassID_TypeDefWeakObjRef                  kAAFClassID_TypeDefinitionWeakObjectReference
#define kAAFClassID_ContainerDef                       kAAFClassID_ContainerDefinition
#define kAAFClassID_PluginDef                          kAAFClassID_PluginDefinition
#define kAAFClassID_AvidTypeDefinitionGenericCharacter kAAFClassID_TypeDefinitionGenericCharacter

//{0d010101-0101-0100-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.01.00
const aafUID_t kAAFClassID_InterchangeObject =
{0x0d010101, 0x0101, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0200-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.02.00
const aafUID_t kAAFClassID_Component =
{0x0d010101, 0x0101, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0300-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.03.00
const aafUID_t kAAFClassID_Segment =
{0x0d010101, 0x0101, 0x0300, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0400-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.04.00
const aafUID_t kAAFClassID_EdgeCode =
{0x0d010101, 0x0101, 0x0400, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0500-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.05.00
const aafUID_t kAAFClassID_EssenceGroup =
{0x0d010101, 0x0101, 0x0500, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0600-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.06.00
const aafUID_t kAAFClassID_Event =
{0x0d010101, 0x0101, 0x0600, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0700-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.07.00
const aafUID_t kAAFClassID_GPITrigger =
{0x0d010101, 0x0101, 0x0700, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0800-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.08.00
const aafUID_t kAAFClassID_CommentMarker =
{0x0d010101, 0x0101, 0x0800, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0900-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.09.00
const aafUID_t kAAFClassID_Filler =
{0x0d010101, 0x0101, 0x0900, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0a00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.0a.00
const aafUID_t kAAFClassID_OperationGroup =
{0x0d010101, 0x0101, 0x0a00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0b00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.0b.00
const aafUID_t kAAFClassID_NestedScope =
{0x0d010101, 0x0101, 0x0b00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0c00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.0c.00
const aafUID_t kAAFClassID_Pulldown =
{0x0d010101, 0x0101, 0x0c00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0d00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.0d.00
const aafUID_t kAAFClassID_ScopeReference =
{0x0d010101, 0x0101, 0x0d00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0e00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.0e.00
const aafUID_t kAAFClassID_Selector =
{0x0d010101, 0x0101, 0x0e00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-0f00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.0f.00
const aafUID_t kAAFClassID_Sequence =
{0x0d010101, 0x0101, 0x0f00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.10.00
const aafUID_t kAAFClassID_SourceReference =
{0x0d010101, 0x0101, 0x1000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1100-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.11.00
const aafUID_t kAAFClassID_SourceClip =
{0x0d010101, 0x0101, 0x1100, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1200-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.12.00
const aafUID_t kAAFClassID_TextClip =
{0x0d010101, 0x0101, 0x1200, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1300-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.13.00
const aafUID_t kAAFClassID_HTMLClip =
{0x0d010101, 0x0101, 0x1300, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1400-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.14.00
const aafUID_t kAAFClassID_Timecode =
{0x0d010101, 0x0101, 0x1400, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1500-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.15.00
const aafUID_t kAAFClassID_TimecodeStream =
{0x0d010101, 0x0101, 0x1500, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1600-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.16.00
const aafUID_t kAAFClassID_TimecodeStream12M =
{0x0d010101, 0x0101, 0x1600, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1700-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.17.00
const aafUID_t kAAFClassID_Transition =
{0x0d010101, 0x0101, 0x1700, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1800-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.18.00
const aafUID_t kAAFClassID_ContentStorage =
{0x0d010101, 0x0101, 0x1800, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1900-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.19.00
const aafUID_t kAAFClassID_ControlPoint =
{0x0d010101, 0x0101, 0x1900, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1a00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.1a.00
const aafUID_t kAAFClassID_DefinitionObject =
{0x0d010101, 0x0101, 0x1a00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1b00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.1b.00
const aafUID_t kAAFClassID_DataDefinition =
{0x0d010101, 0x0101, 0x1b00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1c00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.1c.00
const aafUID_t kAAFClassID_OperationDefinition =
{0x0d010101, 0x0101, 0x1c00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1d00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.1d.00
const aafUID_t kAAFClassID_ParameterDefinition =
{0x0d010101, 0x0101, 0x1d00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1e00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.1e.00
const aafUID_t kAAFClassID_PluginDefinition =
{0x0d010101, 0x0101, 0x1e00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-1f00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.1f.00
const aafUID_t kAAFClassID_CodecDefinition =
{0x0d010101, 0x0101, 0x1f00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.20.00
const aafUID_t kAAFClassID_ContainerDefinition =
{0x0d010101, 0x0101, 0x2000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2100-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.21.00
const aafUID_t kAAFClassID_InterpolationDefinition =
{0x0d010101, 0x0101, 0x2100, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2200-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.22.00
const aafUID_t kAAFClassID_Dictionary =
{0x0d010101, 0x0101, 0x2200, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2300-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.23.00
const aafUID_t kAAFClassID_EssenceData =
{0x0d010101, 0x0101, 0x2300, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2400-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.24.00
const aafUID_t kAAFClassID_EssenceDescriptor =
{0x0d010101, 0x0101, 0x2400, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2500-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.25.00
const aafUID_t kAAFClassID_FileDescriptor =
{0x0d010101, 0x0101, 0x2500, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2600-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.26.00
const aafUID_t kAAFClassID_AIFCDescriptor =
{0x0d010101, 0x0101, 0x2600, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2700-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.27.00
const aafUID_t kAAFClassID_DigitalImageDescriptor =
{0x0d010101, 0x0101, 0x2700, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2800-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.28.00
const aafUID_t kAAFClassID_CDCIDescriptor =
{0x0d010101, 0x0101, 0x2800, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2900-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.29.00
const aafUID_t kAAFClassID_RGBADescriptor =
{0x0d010101, 0x0101, 0x2900, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2a00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.2a.00
const aafUID_t kAAFClassID_HTMLDescriptor =
{0x0d010101, 0x0101, 0x2a00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2b00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.2b.00
const aafUID_t kAAFClassID_TIFFDescriptor =
{0x0d010101, 0x0101, 0x2b00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2c00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.2c.00
const aafUID_t kAAFClassID_WAVEDescriptor =
{0x0d010101, 0x0101, 0x2c00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2d00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.2d.00
const aafUID_t kAAFClassID_FilmDescriptor =
{0x0d010101, 0x0101, 0x2d00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2e00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.2e.00
const aafUID_t kAAFClassID_TapeDescriptor =
{0x0d010101, 0x0101, 0x2e00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-2f00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.2f.00
const aafUID_t kAAFClassID_Header =
{0x0d010101, 0x0101, 0x2f00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.30.00
const aafUID_t kAAFClassID_Identification =
{0x0d010101, 0x0101, 0x3000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3100-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.31.00
const aafUID_t kAAFClassID_Locator =
{0x0d010101, 0x0101, 0x3100, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3200-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.32.00
const aafUID_t kAAFClassID_NetworkLocator =
{0x0d010101, 0x0101, 0x3200, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3300-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.33.00
const aafUID_t kAAFClassID_TextLocator =
{0x0d010101, 0x0101, 0x3300, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3400-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.34.00
const aafUID_t kAAFClassID_Mob =
{0x0d010101, 0x0101, 0x3400, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3500-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.35.00
const aafUID_t kAAFClassID_CompositionMob =
{0x0d010101, 0x0101, 0x3500, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3600-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.36.00
const aafUID_t kAAFClassID_MasterMob =
{0x0d010101, 0x0101, 0x3600, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3700-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.37.00
const aafUID_t kAAFClassID_SourceMob =
{0x0d010101, 0x0101, 0x3700, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3800-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.38.00
const aafUID_t kAAFClassID_MobSlot =
{0x0d010101, 0x0101, 0x3800, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3900-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.39.00
const aafUID_t kAAFClassID_EventMobSlot =
{0x0d010101, 0x0101, 0x3900, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3a00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.3a.00
const aafUID_t kAAFClassID_StaticMobSlot =
{0x0d010101, 0x0101, 0x3a00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3b00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.3b.00
const aafUID_t kAAFClassID_TimelineMobSlot =
{0x0d010101, 0x0101, 0x3b00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3c00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.3c.00
const aafUID_t kAAFClassID_Parameter =
{0x0d010101, 0x0101, 0x3c00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3d00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.3d.00
const aafUID_t kAAFClassID_ConstantValue =
{0x0d010101, 0x0101, 0x3d00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3e00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.3e.00
const aafUID_t kAAFClassID_VaryingValue =
{0x0d010101, 0x0101, 0x3e00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-3f00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.3f.00
const aafUID_t kAAFClassID_TaggedValue =
{0x0d010101, 0x0101, 0x3f00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.40.00
const aafUID_t kAAFClassID_KLVData =
{0x0d010101, 0x0101, 0x4000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4100-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.41.00
const aafUID_t kAAFClassID_DescriptiveMarker =
{0x0d010101, 0x0101, 0x4100, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4200-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.42.00
const aafUID_t kAAFClassID_SoundDescriptor =
{0x0d010101, 0x0101, 0x4200, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4300-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.43.00
const aafUID_t kAAFClassID_DataEssenceDescriptor =
{0x0d010101, 0x0101, 0x4300, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4400-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.44.00
const aafUID_t kAAFClassID_MultipleDescriptor =
{0x0d010101, 0x0101, 0x4400, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4500-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.45.00
const aafUID_t kAAFClassID_DescriptiveClip =
{0x0d010101, 0x0101, 0x4500, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4700-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.47.00
const aafUID_t kAAFClassID_AES3PCMDescriptor =
{0x0d010101, 0x0101, 0x4700, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4800-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.48.00
const aafUID_t kAAFClassID_PCMDescriptor =
{0x0d010101, 0x0101, 0x4800, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4900-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.49.00
const aafUID_t kAAFClassID_PhysicalDescriptor =
{0x0d010101, 0x0101, 0x4900, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4a00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.4a.00
const aafUID_t kAAFClassID_ImportDescriptor =
{0x0d010101, 0x0101, 0x4a00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4b00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.4b.00
const aafUID_t kAAFClassID_RecordingDescriptor =
{0x0d010101, 0x0101, 0x4b00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4c00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.4c.00
const aafUID_t kAAFClassID_TaggedValueDefinition =
{0x0d010101, 0x0101, 0x4c00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4d00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.4d.00
const aafUID_t kAAFClassID_KLVDataDefinition =
{0x0d010101, 0x0101, 0x4d00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4e00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.4e.00
const aafUID_t kAAFClassID_AuxiliaryDescriptor =
{0x0d010101, 0x0101, 0x4e00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-4f00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.4f.00
const aafUID_t kAAFClassID_RIFFChunk =
{0x0d010101, 0x0101, 0x4f00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-5000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.50.00
const aafUID_t kAAFClassID_BWFImportDescriptor =
{0x0d010101, 0x0101, 0x5000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-5100-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.51.00
const aafUID_t kAAFClassID_MPEGVideoDescriptor =
{0x0d010101, 0x0101, 0x5100, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0201-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.01.00.00
const aafUID_t kAAFClassID_ClassDefinition =
{0x0d010101, 0x0201, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0202-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.02.00.00
const aafUID_t kAAFClassID_PropertyDefinition =
{0x0d010101, 0x0202, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0203-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.03.00.00
const aafUID_t kAAFClassID_TypeDefinition =
{0x0d010101, 0x0203, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0204-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.04.00.00
const aafUID_t kAAFClassID_TypeDefinitionInteger =
{0x0d010101, 0x0204, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0205-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.05.00.00
const aafUID_t kAAFClassID_TypeDefinitionStrongObjectReference =
{0x0d010101, 0x0205, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0206-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.06.00.00
const aafUID_t kAAFClassID_TypeDefinitionWeakObjectReference =
{0x0d010101, 0x0206, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0207-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.07.00.00
const aafUID_t kAAFClassID_TypeDefinitionEnumeration =
{0x0d010101, 0x0207, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-5900-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.59.00
const aafUID_t kAAFClassID_SubDescriptor =
{0x0d010101, 0x0101, 0x5900, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-5a00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.5a.00
const aafUID_t kAAFClassID_JPEG2000SubDescriptor =
{0x0d010101, 0x0101, 0x5a00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-5b00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.5b.00
const aafUID_t kAAFClassID_VBIDataDescriptor =
{0x0d010101, 0x0101, 0x5b00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-5c00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.5c.00
const aafUID_t kAAFClassID_ANCDataDescriptor =
{0x0d010101, 0x0101, 0x5c00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-6700-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.67.00
const aafUID_t kAAFClassID_ContainerConstraintsSubDescriptor =
{0x0d010101, 0x0101, 0x6700, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-6800-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.68.00
const aafUID_t kAAFClassID_MPEG4VisualSubDescriptor =
{0x0d010101, 0x0101, 0x6800, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-6a00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.6a.00
const aafUID_t kAAFClassID_MCALabelSubDescriptor =
{0x0d010101, 0x0101, 0x6a00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-6b00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.6b.00
const aafUID_t kAAFClassID_AudioChannelLabelSubDescriptor =
{0x0d010101, 0x0101, 0x6b00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-6c00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.6c.00
const aafUID_t kAAFClassID_SoundfieldGroupLabelSubDescriptor =
{0x0d010101, 0x0101, 0x6c00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-6d00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.6d.00
const aafUID_t kAAFClassID_GroupOfSoundfieldGroupsLabelSubDescriptor =
{0x0d010101, 0x0101, 0x6d00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-6e00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.6e.00
const aafUID_t kAAFClassID_AVCSubDescriptor =
{0x0d010101, 0x0101, 0x6e00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0101-5e00-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.01.01.5e.00
const aafUID_t kAAFClassID_MPEGAudioDescriptor =
{0x0d010101, 0x0101, 0x5e00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0208-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.08.00.00
const aafUID_t kAAFClassID_TypeDefinitionFixedArray =
{0x0d010101, 0x0208, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0209-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.09.00.00
const aafUID_t kAAFClassID_TypeDefinitionVariableArray =
{0x0d010101, 0x0209, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-020a-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.0a.00.00
const aafUID_t kAAFClassID_TypeDefinitionSet =
{0x0d010101, 0x020a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-020b-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.0b.00.00
const aafUID_t kAAFClassID_TypeDefinitionString =
{0x0d010101, 0x020b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-020c-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.0c.00.00
const aafUID_t kAAFClassID_TypeDefinitionStream =
{0x0d010101, 0x020c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-020d-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.0d.00.00
const aafUID_t kAAFClassID_TypeDefinitionRecord =
{0x0d010101, 0x020d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-020e-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.0e.00.00
const aafUID_t kAAFClassID_TypeDefinitionRename =
{0x0d010101, 0x020e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0220-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.20.00.00
const aafUID_t kAAFClassID_TypeDefinitionExtendibleEnumeration =
{0x0d010101, 0x0220, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0221-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.21.00.00
const aafUID_t kAAFClassID_TypeDefinitionIndirect =
{0x0d010101, 0x0221, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0222-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.22.00.00
const aafUID_t kAAFClassID_TypeDefinitionOpaque =
{0x0d010101, 0x0222, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0223-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.23.00.00
const aafUID_t kAAFClassID_TypeDefinitionCharacter =
{0x0d010101, 0x0223, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0e040101-0000-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0e.04.01.01.00.00.00.00
const aafUID_t kAAFClassID_TypeDefinitionGenericCharacter =
{0x0e040101, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0224-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.24.00.00
const aafUID_t kAAFClassID_MetaDefinition =
{0x0d010101, 0x0224, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010101-0225-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.01.01.02.25.00.00
const aafUID_t kAAFClassID_MetaDictionary =
{0x0d010101, 0x0225, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010400-0000-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.04.00.00.00.00.00
const aafUID_t kAAFClassID_DescriptiveObject =
{0x0d010400, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

//{0d010401-0000-0000-060e-2b3402060101}
//06.0e.2b.34.02.53.01.01.0d.01.04.01.00.00.00.00
const aafUID_t kAAFClassID_DescriptiveFramework =
{0x0d010401, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};


#endif // ! __AAFClassDefUIDs_h__
