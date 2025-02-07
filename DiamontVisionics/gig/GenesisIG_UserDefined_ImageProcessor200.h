//==============================================================================
// File:GenesisIG_ImageProcessor200.h
// Copyright (C) 2013-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
// Description: This interface defines an image processor plugin
//==============================================================================

#ifndef DVC_GenesisIG_ImageProcessor200_H
#define DVC_GenesisIG_ImageProcessor200_H

#include "GenesisIGDllSpec.h"
#include "GenesisIG_UserDefined_Plugin.h"

#pragma message( "Exporting Image Processor DLL Symbols (2.0.0)" )

#ifdef __cplusplus
extern "C"{
#endif

#define GIG_CREATE_IMAGEPROCESSOR_PLUGIN		"gigCreateImageProcessorPlugin"
#define GIG_DELETE_IMAGEPROCESSOR_PLUGIN		"gigDeleteImageProcessorPlugin"

struct DVC_GenesisIG_API FrustumParameters
{
	double left_degrees;		// The coordinate of the left vertical clipping plane in degrees.
	double right_degrees;		// The coordinate of the right vertical clipping plane in degrees.
	double bottom_degrees;		// The coordinate of the bottom horizontal clipping plane in degrees.
	double top_degrees;			// The coordinate of the top horizontal clipping plane in degrees.
	float  near_plane;			// The coordinate of the near depth clipping plane.
	float  far_plane;			// The coordinate of the far depth clipping plane.
	bool   override_near_far;	// True if the plugin will adjust the near or far planes, false if not.
};

struct DVC_GenesisIG_API RenderTargetTextureParameters
{
	unsigned int color_texture_id;	// The texture_id returned by glGenTextures that is attached as a color attachment to the currently bound framebuffer.
	unsigned int depth_texture_id;	// The texture_id returned by glGenTextures that is attached as a depth attachment to the currently bound framebuffer.
	int num_samples;				// The number of samples for the framebuffer.
};

struct DVC_GenesisIG_API RenderTargetParameters
{
	unsigned int depth_target;			// The depth target that the bound framebuffer has attached to it.
										// This would normally be GL_TEXTURE_2D_MULTISAMPLE for a multisampled buffer or GL_TEXTURE_2D otherwise.
	unsigned int depth_internal_format;	// The depth internal format that the bound framebuffer has attached to it.
										// This would normally be GL_DEPTH24_STENCIL8_EXT but for the improved depth buffer GL_DEPTH_COMPONENT32F_NV would be appropriate.
};

/*!
 * This interface defines an image processing plugin
 *   - All render states should be preserved during the draw method.
 *   - If a state is enabled upon entry, it should be disabled upon
 *     exit and vice-versa.
 *   - Subjected to udpates.
**/
class DVC_GenesisIG_API IUserDefinedImageProcessor200 : public IUserDefinedPlugin
{
public:

//======================================================================================
// interface functions that must be implemented by the derived class.
//======================================================================================

	virtual ~IUserDefinedImageProcessor200() {}

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
	virtual int initializeGraphics() = 0;

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
	virtual void update( float frame_delta_time = 0.0166, void *param = 0, unsigned int buffer_size_in_bytes = 0 ) = 0;

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
	virtual bool useClipPlanes() const { return false; }

	/*!
	 * Return true if the plugin will pass to the IG an adjusted viewport to be used for rendering.
	 *
	 * @return
	 *  True if the plugin will return an adjusted viewport to the IG, false if not.
	**/
	virtual bool useViewport() const { return false; }

	/*!
	 * Return true if the plugin will pass to the IG an adjusted model view offset or a heading offset to be used for rendering.
	 * The translation applied where the 13th value is rightward, the 14th value is forward, and the 15th value is upward.
	 *
	 * @return
	 *  True if the plugin will return an adjusted model view offset, false if not.
	**/
	virtual bool useModelViewOffsets() const { return false; }

	/*!
	 * Returns to the IG the clip planes to be used for rendering.
	 * useClipPlanes must return true in order for this function to be processed.
	 *
	 * @param[in,out] frustum_params
	 *  The coordinates for the clipping frustum.
	**/
	virtual void getClipPlanes( FrustumParameters& frustum_params ) const { }

	/*!
	 * Returns to the IG the viewport to be used for rendering.
	 * useViewport must return true in order for this function to be processed.
	 *
	 * @param[in,out] viewport
	 *  The viewport in pixels within the window for the x, y, width, and height respectively.
	**/
	virtual void getViewport( int viewport[4] ) const { }

	/*!
	 * Returns to the IG the model view offset matrix if useModelViewOffsets is set to true.
	 *
	 * @return
	 *  The model view offset matrix.
	**/
	virtual const double* getModelViewOffsets() const { return 0; }

	/*!
	 * Tells the Image Processor how much additional radiance is in the scene (above what can be sensed in the scene)
	 * When lightning strikes close to the viewpoint the additional radiance will be calculated between 0 and "lightning_overgain_max"
	 * This can also be configured to provide additional radiance when specific entities are close to the viewpoint.
	 *
	 * @param[in] additional_radiance
	 *  Value of the additional radiance.
	**/
	virtual void setAdditionalRadiance( double additional_radiance ) { }

	/*!
	 * If this plugin sets up a framebuffer that is bound as a render target when the scene is drawn, this function must return true.
	 * If this returns true the user will need to set the "Anti-aliasing - Mode" to "Application-controlled" in the NVIDIA Control Panel.
	 * Note, there is undefined behavior when two or more ImageProcessors for which this function returns true.
	 *
	 * @return
	 *  True if this plugin binds a render target when the scene is drawn, false otherwise.
	**/
	virtual bool PluginBindsRenderTarget() const { return false; }

	/*!
	 * If this plugin sets up a framebuffer that is bound as a render target when the scene is drawn,
	 * then this function must fill in the framebuffer parameters used when setting up the buffer.
	**/
	virtual void GetRenderTargetParameters( RenderTargetParameters& target_params ) const
	{
		target_params.depth_target			= 0x9100; /*GL_TEXTURE_2D_MULTISAMPLE*/
		target_params.depth_internal_format = 0x88F0; /*GL_DEPTH24_STENCIL8_EXT*/
	}

	/*!
	 * If this plugin sets up a framebuffer that is bound as a render target when the scene is drawn,
	 * and the framebuffer has textures attached to it then this function must fill in the texture parameters used when setting up the buffer.
	 *
	 * @return
	 *  True if this plugin attaches textures to the render target and have set them within this function, false otherwise.
	**/
	virtual bool GetRenderTargetTextureParameters( RenderTargetTextureParameters& texture_params ) const { return false; }

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

#endif //DVC_GenesisIG_ImageProcessor200_H
