//==============================================================================
// Copyright © 2014-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================

// User Includes
#include "ExternalFbo.h"

// System Includes
#include <iostream>

#define VIOSOWARPBLEND_DYNAMIC_IMPLEMENT
#include "../../vioso_api/Include/VIOSOWarpBlend.h"

// Using Namespaces

namespace
{
	GLuint CreateTexture( unsigned internal_format, unsigned width, unsigned height, unsigned format, unsigned type )
	{
		GLuint tex = 0;
		glGenTextures( 1, &tex );

		glTextureParameteriEXT( tex, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S    , GL_CLAMP_TO_EDGE );
		glTextureParameteriEXT( tex, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T    , GL_CLAMP_TO_EDGE );
		glTextureParameteriEXT( tex, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR       );
		glTextureParameteriEXT( tex, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR       );

		glTextureImage2DEXT( tex, GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, 0 );

		return tex;
	}

	GLuint CreateMultiTexture( unsigned num_samples, unsigned internal_format, unsigned width, unsigned height )
	{
		GLuint tex = 0;
		glGenTextures( 1, &tex );

		glBindMultiTextureEXT( GL_TEXTURE0, GL_TEXTURE_2D_MULTISAMPLE, tex );
		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, num_samples, internal_format, width, height, GL_TRUE );
		glBindMultiTextureEXT( GL_TEXTURE0, GL_TEXTURE_2D_MULTISAMPLE, 0 );

		return tex;
	}
}

ExternalFbo::
ExternalFbo()
	: scene_color_texture_( 0 )
	, scene_depth_texture_( 0 )
	, scene_rb_( 0 )
	, scene_depth_rb_( 0 )
	, fbo_( 0 )
	, ms_fbo_( 0 )
	, use_multisampling_( false )
	, target_( 0 )
	, existing_fbo_( 0 )
	, window_w_( 0 )
	, window_h_( 0 )
	, depth_format_( GL_DEPTH_COMPONENT32F_NV )
	, internal_format_( GL_RGBA )
	, format_( GL_RGBA )
	, type_( GL_UNSIGNED_BYTE )
	, coverage_samples_( 4 )
	, color_samples_( 4 )
	, warper_(nullptr)
{

}

void
ExternalFbo::
Load( bool multisample, unsigned int width, unsigned int height, VWB_Warper* pWarper )
{
	use_multisampling_ = multisample;
	depth_format_ = GL_DEPTH_COMPONENT32F_NV;

	target_ = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	window_w_ = width;
	window_h_ = height;

	if ( use_multisampling_ )
	{
		scene_color_texture_ = CreateMultiTexture(    color_samples_, internal_format_, window_w_, window_h_ );
		scene_depth_texture_ = CreateMultiTexture( coverage_samples_,    depth_format_, window_w_, window_h_ );
	}
	else
	{
		scene_color_texture_ = CreateTexture( internal_format_, window_w_, window_h_,            format_, type_    );
		scene_depth_texture_ = CreateTexture(    depth_format_, window_w_, window_h_, GL_DEPTH_COMPONENT, GL_FLOAT );
	}

	glGenFramebuffers( 1, &fbo_ );
	glNamedFramebufferTextureEXT( fbo_, GL_COLOR_ATTACHMENT0, scene_color_texture_, 0 );
	glNamedFramebufferTextureEXT( fbo_, GL_DEPTH_ATTACHMENT , scene_depth_texture_, 0 );

	const GLenum status = glCheckNamedFramebufferStatusEXT( fbo_, GL_FRAMEBUFFER_EXT );
	if (GL_FRAMEBUFFER_COMPLETE_EXT != status)
	{
		std::cout << "external rendering will fail due to FBO error: " << status << std::endl;
		return;
	}
	warper_ = pWarper;
}

void ExternalFbo::UpdateWindowSize( unsigned int width, unsigned int height )
{
	if ( window_w_ == width && window_h_ == height ) return;

	window_w_ = width;
	window_h_ = height;

	if ( use_multisampling_ )
	{
		glBindMultiTextureEXT( GL_TEXTURE0, GL_TEXTURE_2D_MULTISAMPLE, scene_color_texture_ );
		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, color_samples_, internal_format_, window_w_, window_h_, GL_TRUE );
		glBindMultiTextureEXT( GL_TEXTURE0, GL_TEXTURE_2D_MULTISAMPLE, scene_depth_texture_ );
		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, coverage_samples_, depth_format_, window_w_, window_h_, GL_TRUE );
		glBindMultiTextureEXT( GL_TEXTURE0, GL_TEXTURE_2D_MULTISAMPLE, 0 );
	}
	else
	{
		glTextureImage2DEXT( scene_color_texture_, GL_TEXTURE_2D, 0, internal_format_, window_w_, window_h_, 0,            format_,    type_, 0 );
		glTextureImage2DEXT( scene_depth_texture_, GL_TEXTURE_2D, 0,    depth_format_, window_w_, window_h_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
	}
}

void
ExternalFbo::
Unload()
{
	if (nullptr != VWB_Destroy)
		VWB_Destroy(warper_);

	if ( scene_color_texture_ ) glDeleteTextures(     1, &scene_color_texture_ );
	if ( scene_depth_texture_ ) glDeleteTextures(     1, &scene_depth_texture_ );
	if ( fbo_ )					glDeleteFramebuffers( 1, &fbo_                 );
}

void
ExternalFbo::
BindFbo()
{
	glGetIntegerv( GL_FRAMEBUFFER_BINDING, &existing_fbo_ );
	glBindFramebuffer( GL_FRAMEBUFFER, fbo_ );
}

void
ExternalFbo::
UnBindFbo()
{
	glBindFramebuffer( GL_FRAMEBUFFER, existing_fbo_ );	
}

void
ExternalFbo::
RenderWarp() const
{
	//glBindFramebuffer(GL_FRAMEBUFFER, existing_fbo_);
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, existing_fbo_);
	VWB_render(warper_, (VWB_param)(size_t)scene_color_texture_, VWB_STATEMASK_DEFAULT | VWB_STATEMASK_VERTEX_SHADER | VWB_STATEMASK_SHADER_RESOURCE );
}

//==============================================================================
// Copyright © 2014-2018 Diamond Visionics, LLC. ALL RIGHTS RESERVED.
//==============================================================================
