//==============================================================================
// File:GenesisIG_ImageProcessor.h  
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines an image processor plugin
//==============================================================================

#ifndef DVC_GenesisIG_ImageProcessor_H
#define DVC_GenesisIG_ImageProcessor_H

#include "GenesisIGDllSpec.h"

#pragma message( "Exporting Image Processor DLL Symbols (1.0.0)" )

#ifdef __cplusplus
extern "C"{
#endif

#define CREATE_GENESISIG_IMAGEPROCESSOR_PLUGIN		"CreateGenesisIGImageProcessorPlugin"
#define DELETE_GENESISIG_IMAGEPROCESSOR_PLUGIN		"DeleteGenesisIGImageProcessorPlugin"

/*!
 * This interface defines an image processing plugin                
 *   - All render states should be preserved during the draw method.
 *   - If a state is enabled upon entry, it should be disabled upon
 *     exit and vice-versa.
 *   - Subjected to udpates.
**/

/*!
 * @note:	When using the mouse and keyboard processing functions (processMouse, processMouseMotion, processMousePassiveMotion, processKey, and processSpecialKey),
 * 			the x and y paramaters are given in Opengl coordinates (0,0 is in the lower left corner) and can be outside any viewports in
 *			a window when a single viewport does not fill a window.
 *			
 *			For cases where the coordinates need to be in window coordinates (0,0 is in the upper left corner) and need to be 
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
class DVC_GenesisIG_API IUserDefinedImageProcessor
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
	
	virtual ~IUserDefinedImageProcessor() {}
	
	/*!
	 * Method called by the IG once during the lifetime of the plugin. Allocate resources needed by the plugin.
	 * Note: versions 2.4.0.0 and higher of the IG will call this prior to having an OpenGL context.
	 * If you were calling OpenGL functionality within this function use the initializeGraphics function.
	 *
	 * @return 
	 *  Value <= 0 indicates failure, value >= 1 indicates sucess.
	**/
	virtual int initialize(const char *config_filename = 0) = 0;

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
	virtual void update(float frame_delta_time = 0.0166, void *param = 0, unsigned int buffer_size_in_bytes = 0) = 0;

	/*!
	 * Process alphanumeric keyboard information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
	 *
	 * @param[in] c
	 *  Key pressed represented by the actual alphanumeric symbol for the key (for example 'W', 'A', 'S', 'D').
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
	 * Process non-alphanumeric keyboard information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
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
	 * Process mouse information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
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
	 * Process mouse motion information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
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
	 * Process passive mouse motion information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
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

	/*!
	 * The IG will call this function prior to drawing each window passing to the plugin the ID of that window and its extents.
	 * The draw function should be linked to this active window.
	 * 
	 * @param[in] window_id
     *  Window identifier (matches what is set inside window_definitions.xml) 
	 * @param[in] window_extents
	 *  Window's extents (window_width, window_height)
	**/
	virtual void setActiveWindow( int window_id, int window_extents[2] ) = 0;

	/*!
	 * The IG will call this function prior to drawing each view passing to the plugin the ID of that view and its viewport.
	 * The draw function should be linked to this active view.
	 *
	 * @param[in] view_id
	 *  View identifier (matches what is set inside window_definitions.xml)
	 * @param[in] viewport
	 *  View's viewport info (x_offset, y_offset, width, height)
	**/
	virtual void setActiveView( int view_id, int viewport[4] ) = 0;

	/*!
	 * Perform all window specific processing prior to the active window getting rendered. This should be for the active window.
	**/
	virtual void preWindowProcess() = 0;

	/*!
	 * Perform all window specific processing after the active window is rendered. This should be for the active window.
	**/
	virtual void postWindowProcess() = 0;

	/*!
	 * Perform all window specific processing prior to the active view getting rendered. This should be for the active view.
	**/
	virtual void preViewProcess() = 0;

	/*!
	 * Perform all window specific processing after the active view is rendered. This should be for the active view.
	**/
	virtual void postViewProcess() = 0;

	/*!
	 * Return true if the plugin will pass to the IG adjusted clip planes to be used for rendering.
	 *
	 * @return
	 *  True if the plugin will return adjusted clip planes to the IG, false if not.
	**/
	virtual bool useClipPlanes() const = 0;

	/*!
	 * Return true if the plugin will pass to the IG an adjusted model view offset or a heading offset to be used for rendering.
	 *
	 * @return
	 *  True if the plugin will return an adjusted model view offset, false if not.
	**/
	virtual bool useModelViewOffsets() const = 0;

	/*!
	 * Returns to the IG the clip planes to be used for rendering.
	 * useClipPlanes must return true in order for this function to be processed.
	 *
	 * @param[in,out] left
	 *  Left coordinates for the left and right vertical clipping plane
	 * @param[in,out] right
	 *  Right coordinates for the left and right vertical clipping plane
	 * @param[in,out] top
	 *  Top coordinates for the bottom and top horizontal clipping plane
	 * @param[in,out] bottom
	 *  Bottom coordinates for the bottom and top horizontal clipping plane
	 * @param[in,out] nearPlane
	 *  Coordinates for the near depth clipping plane
	 * @param[in,out] farPlane
	 *  Coordinates for the far depth clipping plane
	 * @param[in,out] overrideNearFar
	 *  True if the plugin will adjust the neat far planes, false if not.
	**/
	virtual void getClipPlanes( float& left, float& right, float& top, float& bottom, float& nearPlane, float& farPlane, bool& overrideNearFar ) = 0;

	/*!
	 * Returns to the IG the model view offset matrix if useModelViewOffsets is set to true.
	 * The matrix returned is in the same layout as defined by OpenGL.
	 *
	 * @return 
	 *  The model view offset matrix.
	**/
	virtual const float* getModelViewOffsets() const = 0;

};

/*!
 * Function should return a reference to an instance of the user defined object
 * as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @return 
 *  A reference to a new instance of the plugin class
**/
DVC_GenesisIG_API IUserDefinedImageProcessor* CreateGenesisIGImageProcessorPlugin();

/*!
 * Function is used to delete the previously created instance of the
 * the user defined object.THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 * 
 * @param[in] user_object_instance
 *  IG Plugin object to delete.
**/
DVC_GenesisIG_API void DeleteGenesisIGImageProcessorPlugin( IUserDefinedImageProcessor* user_object_instance );

#ifdef __cplusplus
}
#endif

#endif //DVC_GenesisIG_ImageProcessor_H
