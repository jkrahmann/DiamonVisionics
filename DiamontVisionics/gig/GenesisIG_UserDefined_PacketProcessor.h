//==============================================================================
// File: GenesisIG_UserDefined_PacketProcessor.h  
// Copyright (C) 2014-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines the interface required by the IG in order
// pass CIGI packets into the plugins.
//==============================================================================

#ifndef DVC_GenesisIG_UserDefined_Packet_Processor_H
#define DVC_GenesisIG_UserDefined_Packet_Processor_H

#include "GenesisIGDllSpec.h"

#pragma message( "Exporting Packet Processor DLL Symbols (1.0.0)" )

class CigiBaseEventProcessor;

#ifdef __cplusplus
extern "C"{
#endif

#define CREATE_GENESISIG_PACKETPROCESSOR_PLUGIN		"CreateGenesisIGPacketProcessorPlugin"
#define DELETE_GENESISIG_PACKETPROCESSOR_PLUGIN		"DeleteGenesisIGPacketProcessorPlugin"

/********************************************************************
 * This interface defines a user defined packet processor.          *
 *                                                                  *
 * NOTES:                                                           *
 *                                                                  *
 * 1) CigiBaseEventProcessor is defined by the CIGI Class Library.  *
 *	- This interface was tested using version 3.3.0.                *
 ********************************************************************/

/*!
 *
**/
class DVC_GenesisIG_API IUserDefinedPacketProcessor
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

	virtual ~IUserDefinedPacketProcessor() {}

	/*!
	 * Method the IG will call once during the lifetime of the plugin. The implementation needs to allocate all resources needed by the plugin.
	 *
	 * @return 
	 *  Value <= 0 indicates a plugin initialization failure, value >= 1 indicates success.
	**/
	virtual int initialize(const char *config_filename = 0) = 0;

	/*!
	 * Method the IG will call once during the lifetime of the plugin. The implementation needs to cleanup all resources allocated by the plugin.
	**/
	virtual void shutdown() = 0;

	/*!
	 * The IG calls this function to query the plugin for the number of event processors that the plugin needs to register.
	 * @return The number of event processors.
	**/
	virtual unsigned int GetProcessorCount() = 0;

	/*!
	 * The IG calls this function to query for the CigiBaseEventProcessor and packet id that is associate with given index.
	 * @param in index: The index being queried.
	 * @param inout packet_id: The packet id that the returned event processor is meant to process.
	 * @return the CigiBaseEventProcessor that will process packets of type packet_id.
	 * @note: CigiBaseEventProcessor and packet ids are defined by the CIGI CCL library. 
	**/
	virtual CigiBaseEventProcessor* GetEventProcessor( unsigned int index, int& packet_id ) =  0;
};

/*!
 * Function should return a reference to an instance of the IUserDefinedPacketProcessor
 * as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @return 
 *  A reference to a new instance of the plugin class
**/ 
DVC_GenesisIG_API IUserDefinedPacketProcessor* CreateGenesisIGPacketProcessorPlugin();

/*!
 * Function is used to delete the previously created instance of the IUserDefinedPacketProcessor
 * THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 * 
 * @param[in] user_object_instance
 *  IG Plugin object to delete.
**/ 
DVC_GenesisIG_API void DeleteGenesisIGPacketProcessorPlugin( IUserDefinedPacketProcessor* user_object_instance );


#ifdef __cplusplus
}
#endif

#endif //DVC_GenesisIG_UserDefined_Packet_Processor_H
