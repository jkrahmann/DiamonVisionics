//==============================================================================
// File:GenesisIG_UserDefined_NetworkAdapter.h  
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines a user defined Network Adapter
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Network_Adapter_H
#define DVC_GenesisIG_UserDefined_Network_Adapter_H

#include "GenesisIGDllSpec.h"

#pragma message( "Exporting Network Adapter DLL Symbols (1.0.0)" )

#ifdef __cplusplus
extern "C"{
#endif

#define CREATE_GENESISIG_NETWORKADAPTER_PLUGIN		"CreateGenesisIGNetworkAdapterPlugin"
#define DELETE_GENESISIG_NETWORKADAPTER_PLUGIN		"DeleteGenesisIGNetworkAdapterPlugin"

/********************************************************************
 *NOTES:                                                            *
 ********************************************************************/

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
class DVC_GenesisIG_API IUserDefinedNetworkAdapter
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

	virtual ~IUserDefinedNetworkAdapter() {}

	/*!
	 * Method called once during the lifetime of the plugin. Allocate all resources needed by the plugin.
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

	
	/*!
	 * HandleMessages is when processing of incoming packets should occur.
	 * This is also when packets that need to be sent to the IG should be sent as well ( via network ).
	 * Currently this should be CIGI messages since network adapters should be translating
	 * legacy message protocols to the IG's expected CIGI protocol.
	 * Sending messages via the network allows communication to be sent to any number of desired IGs.
	 **/
	virtual void HandleMessages() = 0;

	/*!
	 * If a message recieved by the plugin and the main host needs to be notified
	 * set state to a positive value ( typically 1 ) and set value to some arbitrary event id.
	 * Currently this is sent to a CIGI host via an EventNotification.
	**/
	virtual void GetMessageState( int * value , int * state ) = 0;

	/*!
	 * This allows for a dirrect pass of the message buffer to the IG. It would be the same buffer passed 
	 * via the network when using the CIGI protocol except without the network latency.
	 * This limits communication to a single IG.
	 *
	 * @param[out] interface_id 
	 *  The interface to post messages on. This is typically 0 to be able to use a cigi host
	 *	to be able to attach the eye to inserted entities. Otherwise it will coincide with the additional_cigi sections of 
	 *	configuration.ini where :
	 *	 - [NETWORK] cigi connection is interface_id of 0
	 *	 - [ADDITIONAL_CIGI_0] cigi connection is interface_id of 1
	 *	 - [ADDITIONAL_CIGI_1] cigi connection is interface_id of 2, etc
	 *	 
	 * @param[out] buffer_size
	 *  The size of the buffer passes.
	 *	 
	 * @return 
	 *  Pointer to the message buffer.
	**/
	virtual const char* GetMessageBuffer( unsigned int& interface_id, unsigned int& buffer_size ) = 0;

};

/*!
 * Function should return a reference to an instance of the user defined object
 * as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @return 
 *  A reference to a new instance of the plugin class
**/ 
DVC_GenesisIG_API IUserDefinedNetworkAdapter* CreateGenesisIGNetworkAdapterPlugin();

/*!
 * Function is used to delete the previously created instance of the
 * the user defined object.THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 * 
 * @param[in] user_object_instance
 *  IG Plugin object to delete.
**/ 
DVC_GenesisIG_API void DeleteGenesisIGNetworkAdapterPlugin( IUserDefinedNetworkAdapter* user_object_instance );

#ifdef __cplusplus
}
#endif

#endif // DVC_GenesisIG_UserDefined_Network_Adapter_H
