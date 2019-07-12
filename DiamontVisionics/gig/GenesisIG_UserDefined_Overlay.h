//==============================================================================
// File:GenesisIG_UserDefined_Overlay.h  
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines a user defined overlay
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Overlay_H
#define DVC_GenesisIG_UserDefined_Overlay_H

#include "GenesisIGDllSpec.h"

#pragma message( "Exporting Overlay DLL Symbols (1.0.0)" )

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

/*!
 * @note:	When using the mouse and keyboard processing functions (processMouse, processMouseMotion, processMousePassiveMotion, processKey, and processSpecialKey),
 * 			the x and y paramaters are given in Opengl coordinates (i.e. 0,0 is in the lower left corner) and can be outside any viewports in
 *			a window when a single viewport does not fill a window.
 *			
 *			For cases where the coordinates need to be in window coordinates (i.e. 0,0 is in the upper left corner) and need to be 
 *			represented as an offset into the viewport, the following equations need to be applied:
 *			A)	int viewport_x_offset = x - viewport[0];
 *			B)	int viewport_y_offset = viewport[3] - y + viewport[1];
 *
 *			If the x and y coordinates need to be validated against a viewport and must be completely inside it, then following equation can be used:
 *			C) bool inside_viewport = ( viewport[0] <= x && x <= viewport[0] + viewport[2] ) && ( viewport[1] <= y && y <= viewport[1] + viewport[3] );
 *
 *			where viewport is the last active viewport assigned via setActiveView and is is laid out in the following order (x offset, y offset, width, height)
 *			where x and y are the values passed via processMouse, processMouseMotion, processMousePassiveMotion, processKey, and processSpecialKey.
**/
class DVC_GenesisIG_API IUserDefinedOverlay
{
public:
//======================================================================================
// Helper functions required for runtime version checking.
//======================================================================================
	virtual unsigned getMajorVersion() const		{ return 1; }
	virtual unsigned getMinorVersion() const		{ return 0; }
	virtual unsigned getSubMinorVersion() const		{ return 0; }

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================
	virtual ~IUserDefinedOverlay() {}

	/*!
	 * Method called once during the lifetime of the plugin. Allocate resources needed by the plugin.
	 * Note: versions 2.4.0.0 and higher of the IG will call this prior to having an openGL context.
	 * If you were calling OpenGL functionality within this function use the initializeGraphics function.
	 *
	 * @return 
	 *  Value <= 0 indicates failure, value >= 1 indicates sucess.
	**/
	virtual int initialize(const char *config_filename = 0) = 0;

	/*!
	 * Method called once during the lifetime of the plugin. Cleanup all resources allocated by the plugin.
	**/
	virtual void shutdown() = 0;

	/*!
	 * Funcation called once per frame. This should be applied to all windows and views as defined in window_definition.xml
	 *  
	 * @param[in] frame_delta_time
	 *  Clock time in seconds between the last frame and the current frame.
	 * @param[in] param
	 *	UNUSED
	 * @param[in] buffer_size_in_bytes
	 *  UNUSED
	**/
	virtual void update(float frame_delta_time = 0.0166, void *param = 0, unsigned int buffer_size_in_bytes = 0) = 0;

	/*!
	 * Process alphanumeric keyboard information passed to the plugin. Must return true if information is processed.
	 *
	 * @param[in] c
	 *  Key pressed represented by the actual alphanumeric symbol for the key ('W', 'A', 'S', 'D').
	 * @param[in] x
	 *  X coordinates of the mouse position at the time the key was pressed.
	 * @param[in] y
	 *  Y coordinates of the mouse position at the time the key was pressed.
	 *
	 * @return
	 *  True if information was processed, false if not.
	**/
	virtual bool processKey( unsigned char c, int x, int y ) = 0;

	/*!
	 * Process non-alphanumeric keyboard information passed to the plugin. Must return true if information is processed.
	 *
	 * @param[in] c
	 *  Key pressed represented by the keycode for the key.
	 * @param[in] x
	 *  X coordinates of the mouse position at the time the key was pressed.
	 * @param[in] y
	 *  Y coordinates of the mouse position at the time the key was pressed.
	 *
	 * @return
	 *  True if information was processed, false if not.
	**/
	virtual bool processSpecialKey( unsigned int c, int x, int y ) = 0;

	/*!
	 * Process mouse information passed to the plugin. Must return true if information is processed.
	 * 
	 * @param[in] button
	 *  Button press represented as an int, 0 indicates left button, 1 indicates middle, 2 indicates 3.
	 * @param[in] state
 	 *  State of the button represented as an int, 1 indicates button pressed, 0 indicates button released.
	 * @param[in] x
	 *  X coordinates of the mouse position at the time of the event.
	 * @param[in] y
	 *  Y coordinates of the mouse position at the time of the event.
	 * @param[in] modifiers
	 *  Modifier keys active at the time of the event represetented as a sum of int values. SHIFT = 1, CTRL = 2, ALT = 4.
	**/
	virtual bool processMouse( int button, bool state, int x, int y, int modifiers ) = 0;

	/*!
	 * Process mouse motion information passed to the plugin. Must return true if information is processed.
	 * Mouse motion information is sent to the plugin when the mouse is clicked and dragged.
	 *
	 * @param[in] x 
	 *  X coordinates of the mouse motion.
	 * @param[in] y
	 *  Y coordinates of the mouse motion.
	 *
	 * @return
	 *  True if information was processed, false if not.
	**/
	virtual bool processMouseMotion( int x, int y ) = 0;

	/*!
	 * Process passive mouse motion information passed to the plugin. Must return true if information is processed.
	 * Passive mouse motion is always sent to the plugin whenever the mouse moves, regardless of a button press.
	 *
	 * @param[in] x 
	 *  X coordinates of the mouse motion.
	 * @param[in] y
	 *  Y coordinates of the mouse motion.  
	 *
	 * @return
	 *  True if information was processed, false if not.
	**/
	virtual bool processMousePassiveMotion( int x, int y ) = 0;

	/*
	 * Update a specific component of a multi-component overlay
	 * Component class must be system, component control must be short and instance id must be 1 in order for component information to be sent to plugin.
	 *
	 * @param[in] component_id 
	 *  Unique identifier for the component.
	 * @param[in] component_state
	 *  Flag used to turn component on/off.
	 * @param[in] data 
	 *  Data value to assign to the component with id component_id.
	 * @param[in] data_size 
	 *  The size in bytes of data.
	 *
	 * @return 
	 *  True if update was successful, false otherwise.
	**/
	virtual bool updateComponent( int component_id,bool component_state,void *data,int data_size) = 0;

	/*!
	 * When a window is set to be drawn it will be triggered as active and its extents will be reported.
	 * The draw function should be linked to this active window.
	 * 
	 * @param[in] window_id
	 *  The window identifier (matches what is set inside window_definitions.xml) 
	 * @param[in] window_extents
	 *  The window's extents (window_width, window_height)
	**/
	virtual void setActiveWindow( int window_id, int window_extents[2] ) = 0;

	/*!
	 * When a view is set to be drawn it will be triggered as active and the viewport will be reported.
	 * The draw function should be linked to this active view.
	 * @param[in] view_id
	 *  The view identifier (matches what is set inside window_definitions.xml)
	 * @param[in] viewport
	 *  The view's viewport info (x_offset, y_offset, width, height)
	**/
	virtual void setActiveView( int view_id, int viewport[4] ) = 0;

	/*
	 * Do any drawing (prior to IUserDefinedImageProcessor post processing)
	 * This should be for the active window and active view within that window.
	*/
	virtual void draw( bool on ) = 0;

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

#endif //DVC_GenesisIG_UserDefined_Overlay_H
