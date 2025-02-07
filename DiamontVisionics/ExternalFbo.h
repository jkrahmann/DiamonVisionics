//==============================================================================
// Copyright © 2014-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================

#ifndef DVC_EXTERNAL_FBO_H
#define DVC_EXTERNAL_FBO_H

// User Includes

// System Includes
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include "gig/GenesisIG_UserDefined_ImageProcessor200.h"
#define GLEW_STATIC
#include "GL/glew.h"
#include <gl/GL.h>

#define VIOSOWARPBLEND_DYNAMIC_DEFINE
#include "../../vioso_api/Include/VIOSOWarpBlend.h"


class ExternalFbo
{
public:
	ExternalFbo();

	void Load( bool multisample, unsigned int width, unsigned int height, VWB_Warper* pWarper );
	void Unload();

	void BindFbo();
	void UnBindFbo();

	void RenderWarp() const;

	void UpdateWindowSize( unsigned int width, unsigned int height );

	GLuint GetSceneColorTexture() const { return scene_color_texture_; }
	GLuint GetSceneDepthTexture() const { return scene_depth_texture_; }

	unsigned int GetWidth() const { return window_w_; }
	unsigned int GetHeight() const { return window_h_; }
	VWB_Warper* GetWarper() const { return warper_; }

private:
	bool use_multisampling_;
	GLuint scene_color_texture_, scene_depth_texture_;
	GLuint scene_rb_, scene_depth_rb_;
	GLuint fbo_, ms_fbo_;
	GLint existing_fbo_;
	VWB_Warper* warper_;

	unsigned int window_w_;
	unsigned int window_h_;

	unsigned int target_;
	unsigned int internal_format_;
	unsigned int depth_format_;
	unsigned int format_;
	unsigned int type_;

	unsigned int coverage_samples_;
	unsigned int color_samples_;
};

#endif	// DVC_EXTERNAL_FBO_H

//==============================================================================
// Copyright © 2014-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
