//==============================================================================
// File:GenesisIG_ImageProcessor102.h  
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines an image processor plugin
//==============================================================================

#ifndef DVC_GenesisIG_ImageProcessor102_H
#define DVC_GenesisIG_ImageProcessor102_H

#include "GenesisIG_UserDefined_ImageProcessor101.h"

#pragma message( "Exporting Image Processor DLL Symbols (1.0.2)" )

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
class DVC_GenesisIG_API IUserDefinedImageProcessor102 : public IUserDefinedImageProcessor101
{
public:
//======================================================================================
// Helper functions required for runtime version checking.
//======================================================================================
	virtual unsigned getMajorVersion() const		{ return 1; }
	virtual unsigned getMinorVersion() const		{ return 0; }
	virtual unsigned getSubMinorVersion() const		{ return 2; }

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================
	
	virtual ~IUserDefinedImageProcessor102() {}
	
	/*!
	 * The IG calls this function to get the heading pitch roll from the plugin
	**/
	virtual void GetHPR( double &heading, double& pitch, double& roll ) = 0;
	
	/*!
	 * The IG calls this function to get the clipping planes from the plugin
	**/
	virtual void getClipPlanesd( double& left, double& right, double& top, double& bottom, float& nearPlane, float& farPlane, bool& overrideNearFar ) = 0;
	
	/*!
	 * The IG calls this function to get the model view offset matrix from the plugin
	**/
	virtual const double* getModelViewOffsetsd() const = 0;
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

#endif //DVC_GenesisIG_ImageProcessor102_H
