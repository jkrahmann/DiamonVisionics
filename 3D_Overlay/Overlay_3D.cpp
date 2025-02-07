//==============================================================================
// File:Overlay_3D.cpp
// Copyright (c) 2012-2025 by Diamond Visionics Company.  ALL RIGHTS RESERVED.
// 
// Code to use a FBO by VIOSO GmbH
// 
//==============================================================================
//
// Description: Plugin draws a simple textured square to demonstrate how to render a
//				3D overlay in world space instead of screen space.
//
//==============================================================================

#include "Overlay_3D.h"

#include <grt/grtCore.h>

#include <iostream>

#include "GL/glew.h"
#include "tinyxml2/tinyxml2.h"
#include <cmath>
#include <array>
#include <vector>

#ifndef LINUX_PORT
	#include <windows.h>
#endif

#include <stdlib.h>

using namespace std;

//Internal helper method to generate a test pattern image
std::vector<uint8_t> generateCheckerboardImage( float checkerPercentage )
{
	const int IMAGE_WIDTH = 1024;
	const int IMAGE_HEIGHT = 1024;

	// Create a 2D array to hold the pixel data (RGBA format, 4 bytes per pixel)
	std::vector<uint8_t> imageData( IMAGE_WIDTH * IMAGE_HEIGHT * 4, 255 ); // Initialize to white (255)

																		   // Determine the size of each square in the checkerboard pattern
	int checkerSize = static_cast<int>( checkerPercentage * IMAGE_WIDTH / 100.0f );
	if ( checkerSize <= 0 ) checkerSize = 1; // Ensure there's at least a minimal size

	for ( int y = 0; y < IMAGE_HEIGHT; ++y )
	{
		for ( int x = 0; x < IMAGE_WIDTH; ++x )
		{
			// Determine if we are in a black or white square
			bool isBlack = ( ( x / checkerSize ) % 2 == ( y / checkerSize ) % 2 );

			// Calculate the index for the pixel in the array
			int index = ( y * IMAGE_WIDTH + x ) * 4;

			if ( isBlack )
			{
				// Set to black (0, 0, 0, 255)
				imageData[index] = 0;     // Red
				imageData[index + 1] = 0; // Green
				imageData[index + 2] = 0; // Blue
				imageData[index + 3] = 64; // Alpha
			}
			else
			{
				// Set to white (255, 255, 255, 255)
				imageData[index] = 255;     // Red
				imageData[index + 1] = 255; // Green
				imageData[index + 2] = 255; // Blue
				imageData[index + 3] = 64; // Alpha
			}
		}
	}

	return imageData;
}

Overlay_3D::Overlay_3D()
	: active_view_( 0 )
	, overlay_texture_( -1 )
	, HUD_color_texture_( -1 )
	, HUD_FBO_( -1 )
	, viewport_{ 0,0,1920,1080 }
	, HUD_transparency_( 0.5f )
	, HUD_pixel_width_( 1024 )
	, HUD_pixel_height_( 1024 )
	, HUD_tl_{ -0.5f, -0.5f, -3.0f }
	, HUD_tr_{ 0.5f, -0.5f, -3.0f }
	, HUD_bl_{ -0.5f, 0.5f, -3.0f }
	, HUD_br_{ 0.5f, 0.5f, -3.0f }
{}

Overlay_3D::~Overlay_3D()
{}

int Overlay_3D::initialize( const char *config_filename )
{	
	std::cout << "3D_Overlay: Info: Initializing..." << std::endl;
	while( 1 ) {
		using namespace tinyxml2;

		if( !config_filename )
		{
			std::cout << "3D_Overlay: Warning: No configuration file specified for the  plugin. Using default values." << std::endl;
			break;
		}

		tinyxml2::XMLDocument doc;
		if( XML_SUCCESS != doc.LoadFile( config_filename ) )
		{
			std::cout << "3D_Overlay: Warning: Configuration file " << config_filename << " not found. Using default values." << std::endl;
			break;
		}


		XMLElement* root = doc.FirstChildElement( "_3DOverlay" ); // we need the underscore, as XML tags cannot start with a number
		if( !root )
		{
			std::cout << "3D_Overlay: Warning: " << config_filename << " missing the root <_3DOverlay> tag. Using default values." << std::endl;
			break;
		}

		XMLElement* node = root->FirstChildElement( "transparency" );
		if( node )
		{
			node->QueryFloatText( &HUD_transparency_ );
			std::cout << "3D_Overlay: Info: transparency = " << HUD_transparency_ << std::endl;
		} else {
			std::cout << "3D_Overlay: Warning: missing transparency tag. Using default values." << std::endl;
		}
		node = root->FirstChildElement( "rect3D" );
		if( node ) 
		{
			XMLElement* nodeTL = node->FirstChildElement( "top-left" );
			XMLElement* nodeTR = node->FirstChildElement( "top-right" );
			XMLElement* nodeBL = node->FirstChildElement( "bottom-left" );
			XMLElement* nodeBR = node->FirstChildElement( "bottom-right" );

			if( nodeTL && nodeTR && nodeBL && nodeBR )
			{
				XMLElement* nodeTLx = nodeTL->FirstChildElement( "x" );
				XMLElement* nodeTLy = nodeTL->FirstChildElement( "y" );
				XMLElement* nodeTLz = nodeTL->FirstChildElement( "z" );

				XMLElement* nodeTRx = nodeTR->FirstChildElement( "x" );
				XMLElement* nodeTRy = nodeTR->FirstChildElement( "y" );
				XMLElement* nodeTRz = nodeTR->FirstChildElement( "z" );

				XMLElement* nodeBLx = nodeBL->FirstChildElement( "x" );
				XMLElement* nodeBLy = nodeBL->FirstChildElement( "y" );
				XMLElement* nodeBLz = nodeBL->FirstChildElement( "z" );

				XMLElement* nodeBRx = nodeBR->FirstChildElement( "x" );
				XMLElement* nodeBRy = nodeBR->FirstChildElement( "y" );
				XMLElement* nodeBRz = nodeBR->FirstChildElement( "z" );

				if( nodeTLx && nodeTLy && nodeTLz && nodeTRx && nodeTRy && nodeTRz && nodeBLx && nodeBLy && nodeBLz && nodeBRx && nodeBRy && nodeBRz )
				{
					nodeTLx->QueryFloatText( &HUD_tl_[0] );
					nodeTLy->QueryFloatText( &HUD_tl_[1] );
					nodeTLz->QueryFloatText( &HUD_tl_[2] );
					nodeTRx->QueryFloatText( &HUD_tr_[0] );
					nodeTRy->QueryFloatText( &HUD_tr_[1] );
					nodeTRz->QueryFloatText( &HUD_tr_[2] );
					nodeBLx->QueryFloatText( &HUD_bl_[0] );
					nodeBLy->QueryFloatText( &HUD_bl_[1] );
					nodeBLz->QueryFloatText( &HUD_bl_[2] );
					nodeBRx->QueryFloatText( &HUD_br_[0] );
					nodeBRy->QueryFloatText( &HUD_br_[1] );
					nodeBRz->QueryFloatText( &HUD_br_[2] );
					std::cout << "3D_Overlay: Info: HUD rect = \n" << 
						"  TL(" << HUD_tl_[0] << ", " << HUD_tl_[1] << ", " << HUD_tl_[2] << ")\n" << 
						"  TR(" << HUD_tr_[0] << ", " << HUD_tr_[1] << ", " << HUD_tr_[2] << ")\n" << 
						"  BL(" << HUD_bl_[0] << ", " << HUD_bl_[1] << ", " << HUD_bl_[2] << ")\n" << 
						"  BR(" << HUD_br_[0] << ", " << HUD_br_[1] << ", " << HUD_br_[2] << ")\n" << 
						std::endl;
				} else {
					std::cout << "3D_Overlay: Warning: missing corner values tag. Tag names must be x, y and z. Using default values." << std::endl;
				}
			} else {
				std::cout << "3D_Overlay: Warning: missing corner tag. Tag names must be top-left, top-right, bottom-left and bottom-right. Using default values." << std::endl;
			}
		} else {
			std::cout << "3D_Overlay: Warning: missing rect3D tag. Using default values." << std::endl;
		}
		node = root->FirstChildElement( "size" );
		if( node )
		{
			XMLElement* nodeW = node->FirstChildElement( "width" );
			XMLElement* nodeH = node->FirstChildElement( "height" );
			if( nodeW && nodeH )
			{
				nodeW->QueryUnsignedText( &HUD_pixel_width_ );
				nodeH->QueryUnsignedText( &HUD_pixel_height_ );
				std::cout << "3D_Overlay: Info: size = (" << HUD_pixel_width_ << ", " << HUD_pixel_height_ << ")" << std::endl;
			} else {
				std::cout << "3D_Overlay: Warning: missing width or height tag. Using default values." << std::endl;
			}
		} else {
			std::cout << "3D_Overlay: Warning: missing size tag. Using default values." << std::endl;
		}
		break;
	}

	std::cout << "3D_Overlay: Info: Done Initializing." << std::endl;
	return 1;
}

int Overlay_3D::initializeGraphics()
{
	std::cout << "3D_Overlay: Initializing graphics..." << std::endl;
	//Set up test image overlay texture
	glCreateTextures( GL_TEXTURE_2D, 1, &overlay_texture_ );
	glTextureParameteri( overlay_texture_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTextureParameteri( overlay_texture_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTextureParameteri( overlay_texture_, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTextureParameteri( overlay_texture_, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	auto checkerboard_data = generateCheckerboardImage( 12.5f );
	glTextureImage2DEXT( overlay_texture_, GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard_data.data() );

	// set up teture for FBO for HUD
	glGenTextures( 1, &HUD_color_texture_ );
	glTextureParameteriEXT( HUD_color_texture_, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S    , GL_CLAMP_TO_EDGE );
	glTextureParameteriEXT( HUD_color_texture_, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T    , GL_CLAMP_TO_EDGE );
	glTextureParameteriEXT( HUD_color_texture_, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR       );
	glTextureParameteriEXT( HUD_color_texture_, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR       );
	glTextureImage2DEXT( HUD_color_texture_, GL_TEXTURE_2D, 0, GL_RGBA8, HUD_pixel_width_, HUD_pixel_height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );

	// generate FBO for HUD
	glGenFramebuffers( 1, &HUD_FBO_ );
	glNamedFramebufferTextureEXT( HUD_FBO_, GL_COLOR_ATTACHMENT0, HUD_color_texture_, 0 );

	const GLenum status = glCheckNamedFramebufferStatusEXT( HUD_FBO_, GL_FRAMEBUFFER_EXT );
	if (GL_FRAMEBUFFER_COMPLETE_EXT != status)
	{
		std::cout << "external rendering will fail due to FBO error: " << status << std::endl;
		return 0;
	}
	std::cout << "3D_Overlay: Graphics initialized." << std::endl;

	return 1;
}

void Overlay_3D::shutdown()
{
	GLint fbo = -1;
	glGetIntegerv( GL_FRAMEBUFFER_BINDING, &fbo );
	if( fbo == HUD_FBO_ )
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	if ( -1 != HUD_FBO_ )
		glDeleteFramebuffers( 1, &HUD_FBO_                 );
	if ( -1 != HUD_color_texture_ )
		glDeleteTextures(     1, &HUD_color_texture_ );
	if ( -1 != overlay_texture_ )
		glDeleteTextures(     1, &overlay_texture_ );
}
	
void Overlay_3D::update(float frame_delta_time, void *param, unsigned int buffer_size_in_bytes )
{ }

void Overlay_3D::setActiveWindow( int window_id, int window_extents[2] )
{ }

void Overlay_3D::setActiveView( int view_id, int viewport[4] )
{
	active_view_ = view_id;

	for ( int i = 0; i < 4; ++i )
		viewport_[i] = viewport[i];
}

/*!
 * IMPLEMENTATION
 * Does the actual drawing to the screen. Some light openGL setup is done here.
**/
void Overlay_3D::draw()
{
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glPushClientAttrib( GL_CLIENT_ALL_ATTRIB_BITS );

	// set HUD FBO
	GLint fbo = -1;
	glGetIntegerv( GL_FRAMEBUFFER_BINDING, &fbo );
	glBindFramebuffer( GL_FRAMEBUFFER, HUD_FBO_ );

	/////////////////////////////////////////////////////////////////////////////////////
	// BEGIN draw to HUD FBO
	// this is where the code to draw the HUD's content goes
	// this is a simple example that draws a checkerboard pattern to the HUD FBO

	glUseProgram(0);

	glPushMatrix(); // save scene matrices

	// use ortho-matrix to draw to FOB like a 2D overlay on screen 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, HUD_pixel_width_, 0.0, HUD_pixel_height_, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable( GL_DEPTH_TEST );
	glDepthMask( GL_FALSE );
	glDisable( GL_LIGHTING );
	glDisable( GL_ALPHA_TEST );
	glDisable( GL_FOG );
	glDisable( GL_CULL_FACE );
	glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE_ARB );
	glDisable( GL_BLEND );

	glEnable( GL_TEXTURE_2D );
	glViewport( 0, 0, HUD_pixel_width_, HUD_pixel_height_ );

	glBindTexture( GL_TEXTURE_2D, overlay_texture_ );

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBegin( GL_QUADS );
	{
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f); // Bottom-left
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1024.0f, 0.0f); // Bottom-right
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1024.0f, 1024.0f); // Top-right
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 1024.0f); // Top-left
	}
	glEnd();

	glPopMatrix(); // restore scene matrices

	// END draw to HUD FBO
	/////////////////////////////////////////////////////////////////////////////////////

	// unbind HUD FBO
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );

	// draw HUD texture to scene
	// make sure, view matrix is in cockpit view
	glUseProgram(0);

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_ALPHA_TEST );
	glDisable( GL_FOG );
	glDisable( GL_CULL_FACE );
	glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE_ARB );

	glEnable( GL_TEXTURE_2D );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDepthMask( GL_FALSE );

	glViewport( viewport_[0], viewport_[1], viewport_[2], viewport_[3] );

	glBindTexture( GL_TEXTURE_2D, HUD_color_texture_ );
	glColor4f( 1.0f, 1.0f, 1.0f, HUD_transparency_ );

	//// DEBUG: get current matrix from GL
	//GLfloat mm[16];
	//GLfloat mp[16];
	//glGetFloatv( GL_MODELVIEW_MATRIX, mm );
	//glGetFloatv( GL_PROJECTION_MATRIX, mp );

	//// DEBUG: create sane view projection matrices for test purposes,
	//// as the demo uses a super narrow FoV, thus the HUD is not visible
	//// This will not work with projection! We need to use matrices generated by warp library
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glFrustum(-1.0, 1, -1, 1, 1.0, 100000.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glBegin( GL_QUADS );
	{
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( HUD_bl_[0], HUD_bl_[1], HUD_bl_[2] );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( HUD_br_[0], HUD_br_[1], HUD_br_[2] );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f( HUD_tr_[0], HUD_tr_[1], HUD_tr_[2] );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f( HUD_tl_[0], HUD_tl_[1], HUD_tl_[2] );
	}
	glEnd();

	glBindTexture( GL_TEXTURE_2D, 0 );

	glPopClientAttrib();
	glPopAttrib();
}

void Overlay_3D::PostFrameDraw()
{ }

std::unique_ptr<Overlay_3D> overlay_3D;

DVC_GenesisIG_API IUserDefinedOverlay200* gigCreateOverlayPlugin()
{
	std::cout << "3D_Overlay: New" << std::endl;
	overlay_3D.reset( new Overlay_3D() );
	return overlay_3D.get();
}

DVC_GenesisIG_API void gigDeleteOverlayPlugin( IUserDefinedOverlay200* user_object_instance ) 
{ 
	std::cout << "3D_Overlay: Destroy" << std::endl;
	overlay_3D.reset();	
}

#ifndef LINUX_PORT

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
	UNREFERENCED_PARAMETER( hinstDLL );
	UNREFERENCED_PARAMETER( lpReserved );

    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
			break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
			break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

#endif 