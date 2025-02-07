//==============================================================================
// File:GenesisIG_ImageProcessor101.h  
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines an image processor plugin
//==============================================================================

#ifndef DVC_GenesisIG_ImageProcessor101_H
#define DVC_GenesisIG_ImageProcessor101_H

#include "GenesisIG_UserDefined_ImageProcessor.h"

#pragma message( "Exporting Image Processor DLL Symbols (1.0.1)" )

#ifdef __cplusplus
extern "C"{
#endif

/*!
 * This interface defines an image processing plugin
 *  - All render states should be preserved during the draw method.
 *  - If a state is enabled upon entry, it should be disabled upon
 *    exit and vice-versa.
 *  - Subjected to updates.
 */
class DVC_GenesisIG_API IUserDefinedImageProcessor101 : public IUserDefinedImageProcessor
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
	
	virtual ~IUserDefinedImageProcessor101() {}
	
	/*!
	 * Tells the Image Processor how much additional radiance (above what can be sensed in the scene)
	 *
	 * @param[in] additional_radiance
	 *  Value of the additional radiance.
	**/
	virtual void setAdditionalRadiance( double additional_radiance ) = 0;

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

#endif //DVC_GenesisIG_ImageProcessor101_H
