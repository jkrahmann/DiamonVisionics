//==============================================================================
// File:GenesisIG_UserDefined_Overlay101.h  
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines a user defined overlay
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Overlay101_H
#define DVC_GenesisIG_UserDefined_Overlay101_H

#include "GenesisIG_UserDefined_Overlay.h"

#pragma message( "Exporting Overlay DLL Symbols (1.0.1)" )

#ifdef __cplusplus
extern "C"{
#endif

#define CREATE_GENESISIG_OVERLAY_PLUGIN				"CreateGenesisIGOverlayPlugin"
#define DELETE_GENESISIG_OVERLAY_PLUGIN				"DeleteGenesisIGOverlayPlugin"

/********************************************************************
 * This interface defines a user defined overlay.                   *
 * NOTES:													        *
 * (1) All rendering done in draw should be done using openGL.      *
 * (2) All render states should be preserved during the draw method.*
 *     If a state is enable upon entry, it should be disable upon   *
 *     exit and vice-versa.                                         *
 * -Subjected to udpates.                                           *
 ********************************************************************/
class DVC_GenesisIG_API IUserDefinedOverlay101 : public IUserDefinedOverlay
{
public:
//======================================================================================
// Helper functions required for runtime version checking.
//======================================================================================
	virtual unsigned getMajorVersion() const		{ return 1; }
	virtual unsigned getMinorVersion() const		{ return 0; }
	virtual unsigned getSubMinorVersion() const		{ return 1; }

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================
	
	virtual ~IUserDefinedOverlay101() {}
	
	/*
	 * Do any post frame drawing (This happens after IUserDefinedImageProcessor post processing)
	 * This should be for the active window and active view within that window.
	*/
	virtual void PostFrameDraw() = 0;

};

/*!
 * Function should return a reference to an instance of the user defined object
 * as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @return 
 *  A reference to a new instance of the plugin class
**/ 
DVC_GenesisIG_API IUserDefinedOverlay* CreateGenesisIGOverlayPlugin();

/*!
 * Function is used to delete the previously created instance of the
 * the user defined object.THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 * 
 * @param[in] user_object_instance
 *  IG Plugin object to delete.
**/ 
DVC_GenesisIG_API void DeleteGenesisIGOverlayPlugin( IUserDefinedOverlay* user_object_instance );

#ifdef __cplusplus
}
#endif

#endif //DVC_GenesisIG_UserDefined_Overlay101_H
