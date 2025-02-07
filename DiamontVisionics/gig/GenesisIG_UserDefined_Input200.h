//==============================================================================
// File:GenesisIG_UserDefined_Input200.h
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines a user defined overlay
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Input200_H
#define DVC_GenesisIG_UserDefined_Input200_H

#include "GenesisIGDllSpec.h"
#include "GenesisIG_UserDefined_Plugin.h"

// Special keyboard definitions
#define  GIG_KEY_F1           0x0001
#define  GIG_KEY_F2           0x0002
#define  GIG_KEY_F3           0x0003
#define  GIG_KEY_F4           0x0004
#define  GIG_KEY_F5           0x0005
#define  GIG_KEY_F6           0x0006
#define  GIG_KEY_F7           0x0007
#define  GIG_KEY_F8           0x0008
#define  GIG_KEY_F9           0x0009
#define  GIG_KEY_F10          0x000A
#define  GIG_KEY_F11          0x000B
#define  GIG_KEY_F12          0x000C
#define  GIG_KEY_LEFT         0x0064
#define  GIG_KEY_UP           0x0065
#define  GIG_KEY_RIGHT        0x0066
#define  GIG_KEY_DOWN         0x0067
#define  GIG_KEY_PAGE_UP      0x0068
#define  GIG_KEY_PAGE_DOWN    0x0069
#define  GIG_KEY_HOME         0x006A
#define  GIG_KEY_END          0x006B
#define  GIG_KEY_INSERT       0x006C

//Mouse definitions
#define  GIG_MOUSE_LEFT_BUTTON				0
#define  GIG_MOUSE_SCROLL_BUTTON			1
#define  GIG_MOUSE_RIGHT_BUTTON				2
#define  GIG_MOUSE_SCROLL_FORWARD			3
#define  GIG_MOUSE_SCROLL_BACKWARD			4
#define  GIG_MOUSE_SCROLL_BUTTON_FORWARD	35
#define  GIG_MOUSE_SCROLL_BUTTON_BACKWARD	36

#define GIG_SHIFT							1
#define GIG_CTRL							2
#define GIG_ALT								4

#pragma message( "Exporting Input DLL Symbols (2.0.0)" )

#ifdef __cplusplus
extern "C"{
#endif

#define GIG_CREATE_INPUT_PLUGIN				"gigCreateInputPlugin"
#define GIG_DELETE_INPUT_PLUGIN				"gigDeleteInputPlugin"

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
	class DVC_GenesisIG_API IUserDefinedInput200 : public IUserDefinedPlugin
	{
	public:

		//======================================================================================
		// interface functions that must be implemented by the derived class.
		//======================================================================================
		virtual ~IUserDefinedInput200() {}

		/*!
		* Process alphanumeric keyboard information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
		*
		* @param[in] c
		*  Key pressed represented by the actual alphanumeric symbol for the key ('W', 'A', 'S', 'D').
		* @param[in] x
		*  X coordinates of the mouse position at the time the key was pressed.
		* @param[in] y
		*  Y coordinates of the mouse position at the time the key was pressed.
		* @param[in] modifiers
		*  Modifier keys active at the time of the event represetented as a sum of int values. SHIFT = 1, CTRL = 2, ALT = 4.
		*
		* @return
		*  True if information was processed and the IG should not process it further, false otherwise.
		**/
		virtual bool processKey( unsigned char c, int x, int y, int modifiers ) = 0;

		/*!
		* Process non-alphanumeric keyboard information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
		*
		* @param[in] c
		*  Key pressed represented by the keycode for the key.
		* @param[in] x
		*  X coordinates of the mouse position at the time the key was pressed.
		* @param[in] y
		*  Y coordinates of the mouse position at the time the key was pressed.
		* @param[in] modifiers
		*  Modifier keys active at the time of the event represetented as a sum of int values. SHIFT = 1, CTRL = 2, ALT = 4.
		*
		* @return
		*  True if information was processed and the IG should not process it further, false otherwise.
		**/
		virtual bool processSpecialKey( unsigned int c, int x, int y, int modifiers ) = 0;

		/*!
		* Process mouse information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
		*
		* @param[in] button
		*  Button press represented as an int, 0 indicates left button, 1 indicates middle, 2 indicates right.
		* @param[in] state
		*  State of the button represented as an int, 1 indicates button pressed, 0 indicates button released.
		* @param[in] x
		*  X coordinates of the mouse position at the time of the event.
		* @param[in] y
		*  Y coordinates of the mouse position at the time of the event.
		* @param[in] modifiers
		*  Modifier keys active at the time of the event represetented as a sum of int values. SHIFT = 1, CTRL = 2, ALT = 4.
		*
		* @return
		*  True if information was processed and the IG should not process it further, false otherwise.
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
		* @param[in] modifiers
		*  Modifier keys active at the time of the event represetented as a sum of int values. SHIFT = 1, CTRL = 2, ALT = 4.
		*
		* @return
		*  True if information was processed and the IG should not process it further, false otherwise.
		**/
		virtual bool processMouseMotion( int x, int y, int modifiers ) = 0;

		/*!
		* Process passive mouse motion information passed to the plugin. Must return true if information is processed to prevent further processing by the IG.
		* Passive mouse motion is always sent to the plugin whenever the mouse moves, regardless of a button press.
		*
		* @param[in] x
		*  X coordinates of the mouse motion.
		* @param[in] y
		*  Y coordinates of the mouse motion.
		* @param[in] modifiers
		*  Modifier keys active at the time of the event represetented as a sum of int values. SHIFT = 1, CTRL = 2, ALT = 4.
		*
		* @return
		*  True if information was processed and the IG should not process it further, false otherwise.
		**/
		virtual bool processMousePassiveMotion( int x, int y, int modifiers ) = 0;

	};

	/*!
	* Function should return a reference to an instance of the user defined object
	* as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
	*
	* @return
	*  A reference to a new instance of the plugin class
	**/
	DVC_GenesisIG_API IUserDefinedInput200* gigCreateInputPlugin();

	/*!
	* Function is used to delete the previously created instance of the
	* the user defined object.THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
	*
	* @param[in] user_object_instance
	*  IG Plugin object to delete.
	**/
	DVC_GenesisIG_API void gigDeleteInputPlugin( IUserDefinedInput200* user_object_instance );

#ifdef __cplusplus
}
#endif

#endif //DVC_GenesisIG_UserDefined_Input200_H
