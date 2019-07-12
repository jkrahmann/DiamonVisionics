//==============================================================================
// File:GenesisIG_UserDefined_Plugin.h  
// Copyright (C) 2014-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines a user defined overlay
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Plugin_H
#define DVC_GenesisIG_UserDefined_Plugin_H

#include "GenesisIGDllSpec.h"

#pragma message( "Exporting Plugin DLL Symbols" )

#ifdef __cplusplus
extern "C"{
#endif

class DVC_GenesisIG_API IUserDefinedPlugin
{
public:
	virtual ~IUserDefinedPlugin() {}
};

#ifdef __cplusplus
}
#endif

#endif //DVC_GenesisIG_UserDefined_Plugin_H
