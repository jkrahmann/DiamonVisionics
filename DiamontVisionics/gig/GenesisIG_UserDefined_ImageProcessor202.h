//==============================================================================
// File:GenesisIG_ImageProcessor202.h  
// Copyright (C) 2016-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines an image processor plugin
//==============================================================================

#ifndef DVC_GenesisIG_ImageProcessor202_H
#define DVC_GenesisIG_ImageProcessor202_H

#include "GenesisIG_UserDefined_ImageProcessor201.h"

#pragma message( "Exporting Image Processor DLL Symbols (2.0.2)" )

#ifdef __cplusplus
extern "C"{
#endif

#define GIG_CREATE_IMAGEPROCESSOR_PLUGIN		"gigCreateImageProcessorPlugin"
#define GIG_DELETE_IMAGEPROCESSOR_PLUGIN		"gigDeleteImageProcessorPlugin"

/*!
 * This interface defines an image processing plugin
 *   - All render states should be preserved during the draw method.
 *   - If a state is enabled upon entry, it should be disabled upon
 *     exit and vice-versa.
 *   - Subjected to udpates.
 */
class DVC_GenesisIG_API IUserDefinedImageProcessor202 : public IUserDefinedImageProcessor201
{
public:

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================

	virtual ~IUserDefinedImageProcessor202() {}

	/*!
	 * Method called once per window during the lifetime of the plugin, this happens between the calls to initialize() and initializeGraphics().
	 * OpenGL functions cannot be called within this function.
	 *
	 * @param[in] window
	 *  The id of the window as defined by the window_definition.xml.
	 * @param[in] window_dimensions
	 *  The dimensions of the window at startup.
	**/
	virtual void RegisterWindowSize( int window, int window_dimensions[2] ) {}
	
	/*!
	 * Method called once per view during the lifetime of the plugin, this happens between the calls to initialize() and initializeGraphics().
	 * OpenGL functions cannot be called within this function.
	 *
	 * @param[in] window
	 *  The id of the window as defined by the window_definition.xml.
	 * @param[in] view
	 *  The id of the view as defined by the window_definition.xml.
	 * @param[in] viewport
	 *  The OpenGL viewport of the view at startup.
	**/
	virtual void RegisterViewport( int window, int view, int viewport[4] ) {}
	
	/*!
	 * Method called every time the frustum of the specified view changes.
	 *
	 * @param[in] window
	 *  The id of the window.
	 * @param[in] view
	 *  The id of the view.
	 * @param[in] params
	 *  The frustum of the specified view.
	**/
	virtual void UpdateProjection( int window, int view, const FrustumParameters& params ) {}
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

#endif //DVC_GenesisIG_ImageProcessor202_H
