//==============================================================================
// File: GenesisIG_UserDefined_PacketProcessor200.h
// Copyright (C) 2014-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines the interface required by the IG in order
// pass CIGI packets into the plugins.
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Packet_Processor200_H
#define DVC_GenesisIG_UserDefined_Packet_Processor200_H

#include "GenesisIGDllSpec.h"
#include "GenesisIG_UserDefined_Plugin.h"

#pragma message( "Exporting Packet Processor DLL Symbols (2.0.0)" )

class CigiBaseEventProcessor;

#ifdef __cplusplus
extern "C"{
#endif

#define GIG_CREATE_PACKETPROCESSOR_PLUGIN		"gigCreatePacketProcessorPlugin"
#define GIG_DELETE_PACKETPROCESSOR_PLUGIN		"gigDeletePacketProcessorPlugin"

/********************************************************************
 * This interface defines a user defined packet processor.          *
 *                                                                  *
 * NOTES:                                                           *
 *                                                                  *
 * 1) CigiBaseEventProcessor is defined by the CIGI Class Library.  *
 *	- This interface was tested using version 3.3.0.                *
 ********************************************************************/
class DVC_GenesisIG_API IUserDefinedPacketProcessor200 : public IUserDefinedPlugin
{
public:

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================

	virtual ~IUserDefinedPacketProcessor200() {}

	/*!
	 * Queries for the number of event processors that the plugin needs to register.
	 * @return The number of event processors.
	**/
	virtual unsigned int GetProcessorCount() const = 0;

	/*!
	 * Queries for the CigiBaseEventProcessor and packet id that is associate with given index.
	 * @param in index: The index being queried.
	 * @param inout packet_id: The packet id that the returned event processor is meant to process.
	 * @return the CigiBaseEventProcessor that will process packets of type packet_id.
	 * @note: CigiBaseEventProcessor and packet ids are defined by the CIGI CCL library.
	**/
	virtual CigiBaseEventProcessor* GetEventProcessor( unsigned int index, int& packet_id ) const = 0;
};

/*!
 * Function should return a reference to an instance of the IUserDefinedPacketProcessor
 * as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @return
 *  A reference to a new instance of the plugin class
**/
DVC_GenesisIG_API IUserDefinedPacketProcessor200* gigCreatePacketProcessorPlugin();

/*!
 * Function is used to delete the previously created instance of the IUserDefinedPacketProcessor
 * THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @param[in] user_object_instance
 *  IG Plugin object to delete.
**/
DVC_GenesisIG_API void gigDeletePacketProcessorPlugin( IUserDefinedPacketProcessor200* user_object_instance );


#ifdef __cplusplus
}
#endif

#endif //DVC_GenesisIG_UserDefined_Packet_Processor200_H
