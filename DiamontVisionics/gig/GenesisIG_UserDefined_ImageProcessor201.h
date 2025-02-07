//==============================================================================
// File:GenesisIG_ImageProcessor201.h  
// Copyright (C) 2015-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines an image processor plugin
//==============================================================================

#ifndef DVC_GenesisIG_ImageProcessor201_H
#define DVC_GenesisIG_ImageProcessor201_H

#include "GenesisIG_UserDefined_ImageProcessor200.h"

#pragma message( "Exporting Image Processor DLL Symbols (2.0.1)" )

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
class DVC_GenesisIG_API IUserDefinedImageProcessor201 : public IUserDefinedImageProcessor200
{
public:

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================
	
	virtual ~IUserDefinedImageProcessor201() {}
	
	/*!
	 * Tells the Image Processor to SwapBuffers if it needs to. The swap will be applied to the window specified
	 * by the last call to setActiveWindow
	 *
	 * @return
	 *  Boolean indicating if the plugin has swapped the buffers. If false is returned the IG will swap buffers.
	**/
	virtual bool SwapBuffers() { return false; }

};

/*!
 * Function should return a reference to an instance of the user defined object
 * as defined by the inherited interface. THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @return
 *  A reference to a new instance of the plugin class
**/
DVC_GenesisIG_API IUserDefinedImageProcessor200* gigCreateImageProcessorPlugin();

/*!
 * Function is used to delete the previously created instance of the
 * the user defined object.THIS MUST BE IMPLEMENTED IN THE PLUGIN DLL WITH THE SAME SIGNATURE.
 *
 * @param[in] user_object_instance
 *  IG Plugin object to delete.
**/
DVC_GenesisIG_API void gigDeleteImageProcessorPlugin( IUserDefinedImageProcessor200* user_object_instance );

#ifdef __cplusplus
}
#endif

#endif //DVC_GenesisIG_ImageProcessor201_H
