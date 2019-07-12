//==============================================================================
// File:GenesisIG_UserDefined_Overlay201.h
// Copyright (C) 2016-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines a user defined overlay
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Overlay201_H
#define DVC_GenesisIG_UserDefined_Overlay201_H

#include "GenesisIG_UserDefined_Overlay200.h"

#pragma message( "Exporting Overlay DLL Symbols (2.0.1)" )

#ifdef __cplusplus
extern "C"{
#endif

#define GIG_CREATE_OVERLAY_PLUGIN				"gigCreateOverlayPlugin"
#define GIG_DELETE_OVERLAY_PLUGIN				"gigDeleteOverlayPlugin"

/********************************************************************
 * This interface defines a user defined overlay.                   *
 * NOTES:													        *
 * (1) All rendering done in draw should be done using openGL.      *
 * (2) All render states should be preserved during the draw method.*
 *     If a state is enable upon entry, it should be disable upon   *
 *     exit and vice-versa.                                         *
 * -Subjected to udpates.                                           *
 ********************************************************************/
class DVC_GenesisIG_API IUserDefinedOverlay201 : public IUserDefinedOverlay200
{
public:

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================
	virtual ~IUserDefinedOverlay201() {}

	/*
	 * Do any post window drawing (This happens before IUserDefinedImageProcessor post processing)
	 * This should be for the active window and active view within that window.
	*/
	virtual void PostWindowDraw() = 0;

};

/*!
 * Function should return a reference to an instance of the user defined object
 * as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @return
 *  A reference to a new instance of the plugin class
**/
DVC_GenesisIG_API IUserDefinedOverlay200* gigCreateOverlayPlugin();

/*!
 * Function is used to delete the previously created instance of the
 * the user defined object.THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @param[in] user_object_instance
 *  IG Plugin object to delete.
**/
DVC_GenesisIG_API void gigDeleteOverlayPlugin( IUserDefinedOverlay200* user_object_instance );

#ifdef __cplusplus
}
#endif

#endif //DVC_GenesisIG_UserDefined_Overlay201_H
