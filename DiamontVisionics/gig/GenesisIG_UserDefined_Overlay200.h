//==============================================================================
// File:GenesisIG_UserDefined_Overlay200.h
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines a user defined overlay
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Overlay200_H
#define DVC_GenesisIG_UserDefined_Overlay200_H

#include "GenesisIGDllSpec.h"
#include "GenesisIG_UserDefined_Plugin.h"

#pragma message( "Exporting Overlay DLL Symbols (2.0.0)" )

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
class DVC_GenesisIG_API IUserDefinedOverlay200 : public IUserDefinedPlugin
{
public:

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================
	virtual ~IUserDefinedOverlay200() {}

	/*!
	 * Method called by the IG once during the lifetime of the plugin. Allocate resources needed by the plugin.
	 * Note: versions 2.4.0.0 and higher of the IG will call this prior to having an OpenGL context.
	 * If you were calling OpenGL functionality within this function use the initializeGraphics function.
	 *
	 * @return
	 *  Value <= 0 indicates failure, value >= 1 indicates sucess.
	**/
	virtual int initialize( const char *config_filename = 0 ) = 0;

	/*!
	 * Method called by the IG once during the lifetime of the plugin after creating an OpenGL context.
	 * OpenGL functions may be called within this function.
	 *
	 * @return
	 *  Value <= 0 indicates failure, value >= 1 indicates sucess.
	**/
	virtual int initializeGraphics() = 0;

	/*!
	 * Method called by the IG once during the lifetime of the plugin. Cleanup all resources allocated by the plugin.
	**/
	virtual void shutdown() = 0;

	/*!
	 * Function called by the IG once per frame to pass to the plugin the current delta time.
	 *
	 * @param[in] frame_delta_time
	 *  Clock time in seconds between the last frame and the current frame.
	 * @param[in] param
	 *	UNUSED
	 * @param[in] buffer_size_in_bytes
	 *  UNUSED
	**/
	virtual void update( float frame_delta_time = 0.0166, void *param = 0, unsigned int buffer_size_in_bytes = 0 ) = 0;

	/*!
	 * The IG will call this function prior to drawing each window passing to the plugin the ID of that window and its extents.
	 * The draw function should be linked to this active window.
	 *
	 * @param[in] window_id
	 *  The window identifier (matches what is set inside window_definitions.xml)
	 * @param[in] window_extents
	 *  The window's extents (window_width, window_height)
	**/
	virtual void setActiveWindow( int window_id, int window_extents[2] ) = 0;

	/*!
	 * The IG will call this function prior to drawing each view passing to the plugin the ID of that view and its viewport.
	 * The draw function should be linked to this active view.
	 * @param[in] view_id
	 *  The view identifier (matches what is set inside window_definitions.xml)
	 * @param[in] viewport
	 *  The view's viewport info (x_offset, y_offset, width, height)
	**/
	virtual void setActiveView( int view_id, int viewport[4] ) = 0;

	/*
	 * The plugin should do any per-view drawing in this function. This will be called prior to any IUserDefinedImageProcessor post processing.
	 * This should be for the active window and active view within that window.
	*/
	virtual void draw() = 0;

	/*
	 * The plugin should do any per-window drawing in this function. This will be called after IUserDefinedImageProcessor post processing.
	 * This should be for the active window.
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

#endif //DVC_GenesisIG_UserDefined_Overlay200_H
