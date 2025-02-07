//==============================================================================
// File:Overlay_3D.h
// Copyright (c) 2012-2025 by Diamond Visionics Company.  ALL RIGHTS RESERVED.
//==============================================================================
//
// Description: Plugin draws a simple textured square to demonstrate how to render a
//				3D overlay in world space instead of screen space.
//
//==============================================================================

#ifndef THREE_D_OVERLAY_H
#define THREE_D_OVERLAY_H

#include <gig/GenesisIG_UserDefined_Overlay200.h>

#include <memory>

#ifndef LINUX_PORT
	#define OVERLAY_DLLSPEC    __declspec( dllexport )
	#pragma message( "Exporting Overlay DLL Symbols" )
#else
	#define OVERLAY_DLLSPEC 
#endif 
 
#ifdef __cplusplus
	extern "C"{
#endif

/*!
 * Plugin draws a simple red square to demonstrate how to render a
 * 3D overlay in world space instead of screen space.
**/
class OVERLAY_DLLSPEC Overlay_3D : public IUserDefinedOverlay200
{
public:
	Overlay_3D();
	~Overlay_3D();
	
	/*!
	 * Method called once during the lifetime of the plugin. Allocate resources needed by the plugin.
	 * Note: versions 2.4.0.0 and higher of the IG will call this prior to having an openGL context.
	 * If you were calling OpenGL functionality within this function use the initializeGraphics function.
	 *
	 * @return
	 *  Value <= 0 indicates failure, value >= 1 indicates sucess.
	**/
	int initialize( const char *config_filename = 0 );

	/*!
	 * Method called once during the lifetime of the plugin after we have an OpenGL context.
	 * OpenGL functions may be called within this function.
	 *
	 * @return
	 *  Value <= 0 indicates failure, value >= 1 indicates sucess.
	**/
	int initializeGraphics();

	/*!
	 * Method called once during the lifetime of the plugin. Cleanup all resources allocated by the plugin.
	**/
	void shutdown();

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
	void update( float frame_delta_time = 0.0166, void *param = 0, unsigned int buffer_size_in_bytes = 0 );
	
	//////////////////////////////////////////////////
	//       IUserDefinedOverlay200 Functions       //
	//////////////////////////////////////////////////

	/*!
	 * When a window is set to be drawn it will be triggered as active and its extents will be reported.
	 * The draw function should be linked to this active window.
	 * 
	 * @param[in] window_id
	 *  The window identifier (matches what is set inside window_definitions.xml) 
	 * @param[in] window_extents
	 *  The window's extents (window_width, window_height)
	**/
	void setActiveWindow( int window_id, int window_extents[2] );

	/*!
	 * When a view is set to be drawn it will be triggered as active and the viewport will be reported.
	 * The draw function should be linked to this active view.
	 * @param[in] view_id
	 *  The view identifier (matches what is set inside window_definitions.xml)
	 * @param[in] viewport
	 *  The view's viewport info (x_offset, y_offset, width, height)
	**/
	void setActiveView( int view_id, int viewport[4] );

	/*
	 * Do any drawing
	 * This should be for the active window and active view within that window.
	*/
	void draw();
	
	/*
	 * Do any post frame drawing (This happens after IUserDefinedImageProcessor post processing)
	 * This should be for the active window and active view within that window.
	*/
	void PostFrameDraw();

private:
	int				viewport_[4];							/* Viewport: left, right, width, height */
	int				active_view_;							/* Id of the active view */
	unsigned int	overlay_texture_;
	unsigned int	HUD_FBO_;
	unsigned int	HUD_color_texture_;
	unsigned int	HUD_pixel_width_;
	unsigned int	HUD_pixel_height_;
	float 			HUD_transparency_;
	float			HUD_tl_[3]; // top-left corner of HUD
	float			HUD_tr_[3]; // top-right corner of HUD
	float			HUD_bl_[3]; // bottom-left corner of HUD
	float			HUD_br_[3]; // bottom-right corner of HUD
};

#ifdef __cplusplus
}
#endif

#endif
