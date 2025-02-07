//==============================================================================
// File:GenesisIG_UserDefined_NetworkAdapter200.h
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines a user defined Network Adapter
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Network_Adapter200_H
#define DVC_GenesisIG_UserDefined_Network_Adapter200_H

#include "GenesisIGDllSpec.h"
#include "GenesisIG_UserDefined_Plugin.h"

#pragma message( "Exporting Network Adapter DLL Symbols (2.0.0)" )

#ifdef __cplusplus
extern "C"{
#endif

#define GIG_CREATE_NETWORKADAPTER_PLUGIN		"gigCreateNetworkAdapterPlugin"
#define GIG_DELETE_NETWORKADAPTER_PLUGIN		"gigDeleteNetworkAdapterPlugin"

/********************************************************************
 *NOTES:                                                            *
 ********************************************************************/

class DVC_GenesisIG_API IUserDefinedNetworkAdapter200 : public IUserDefinedPlugin
{
public:

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================

	virtual ~IUserDefinedNetworkAdapter200() {}

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
	virtual int initializeGraphics() { return 1; }

	/*!
	 * Method called bye the IG once during the lifetime of the plugin. Cleanup all resources allocated by the plugin.
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
	 * HandleMessages is called by the IG when the plugin should process incoming packets.
	 * This is also when packets that need to be sent to the IG should be sent as well ( via network ).
	 * Currently this should be CIGI messages since network adapters should be translating
	 * legacy message protocols to the IG's expected CIGI protocol.
	 * Sending messages via the network allows communication to be sent to any number of desired IGs.
	 **/
	virtual void HandleMessages() = 0;

	/*!
	 * Function called by the IG to query if the plugin needs to notify the host, to send a message the plugin
	 * will implement this function to set state to a positive value ( typically 1 ) and set value to some arbitrary event id.
	 * The IG will then send the value to a CIGI host via an EventNotification packet.
	**/
	virtual void GetMessageState( int* value, int* state ) const = 0;

	/*!
	 * GetMessageBuffer is called by the IG to query the plugin for a CIGI message buffer that bypasses networking.
	 * The buffer returned by the plugin should be the same buffer that would be passed via the network when using the 
	 * CIGI protocol except without the network latency. This limits communication to a single IG.
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
	virtual const char* GetMessageBuffer( unsigned int& interface_id, unsigned int& buffer_size ) const = 0;

};

/*!
 * Function should return a reference to an instance of the user defined object
 * as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @return
 *  A reference to a new instance of the plugin class
**/
DVC_GenesisIG_API IUserDefinedNetworkAdapter200* gigCreateNetworkAdapterPlugin();

/*!
 * Function is used to delete the previously created instance of the
 * the user defined object. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @param[in] user_object_instance
 *  IG Plugin object to delete.
**/
DVC_GenesisIG_API void gigDeleteNetworkAdapterPlugin( IUserDefinedNetworkAdapter200* user_object_instance );

#ifdef __cplusplus
}
#endif

#endif // DVC_GenesisIG_UserDefined_Network_Adapter200_H
