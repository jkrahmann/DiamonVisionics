// VIOSO-Plugin.cpp : Defines the exported functions for the DLL application.
//

#include "VIOSO-Plugin.h"
#include "tinyxml2.h"
using namespace tinyxml2;

#include <iostream>
#include <sstream>

SimpleFBOImageProcessor::SimpleFBOImageProcessor()
	: active_window_(-1)
	, warper_bin_path_(
#if defined( _M_X64 )
		"ViosoWarpBlend64"
#else
		"ViosoWarpBlend"
#endif //def _M_X64
	)
	, warper_ini_path_("VIOSOWarpBlend.ini")
	, warper_log_path_("VIOSOWarpBlend.log")
{
}

SimpleFBOImageProcessor::~SimpleFBOImageProcessor()
{}

int SimpleFBOImageProcessor::initialize(const char *config_filename)
{
	while( 1 ) {
		if( !config_filename )
		{
			std::cout << "Warning: No configuration file specified for the SimpleFBOImageProcessor plugin. Using default values." << std::endl;
			break;
		}

		tinyxml2::XMLDocument doc;
		if( XML_SUCCESS != doc.LoadFile( config_filename ) )
		{
			std::cout << "Warning: Configuration file " << config_filename << " not found for the VIOSO_plugin. Using default values." << std::endl;
			break;
		}


		XMLElement* root = doc.FirstChildElement( "vioso_plugin" );
		if( !root )
		{
			std::cout << "Warning: " << config_filename << " missing the root <vioso_plugin> tag. Using default values." << std::endl;
			break;
		}

		for( XMLElement* element = root->FirstChildElement(); element; element = element->NextSiblingElement() )
		{
			std::string element_name = element->Value();

			for( XMLAttribute const* attribute = element->FirstAttribute(); attribute; attribute = attribute->Next() )
			{
				std::string attribute_name = attribute->Name();

				if( "path" == element_name )
				{
					if( "bin" == attribute_name ) {
						warper_bin_path_ = std::string( ExpandEnvironmentStringsA( attribute->Value(), NULL, 0 ), 0 );
						ExpandEnvironmentStringsA( attribute->Value(), &warper_bin_path_[0], (DWORD)warper_bin_path_.size() );
					} else if( "ini" == attribute_name )
					{
						warper_ini_path_ = attribute->Value();
					} else if( "log" == attribute_name )
					{
						warper_log_path_ = attribute->Value();
					}
				}

			}
		}
	}

#define VIOSOWARPBLEND_DYNAMIC_INITIALIZE
#define VIOSOWARPBLEND_FILE warper_bin_path_.c_str()
#include "../../vioso_api/Include/VIOSOWarpBlend.h"
	if (nullptr == VWB_Create)
	{
		std::cout << "could not load VIOSOWarpBlend library, check path to binary and dependencies ( MSVC" << _MSC_VER << ")" << std::endl;
		return 0;
	}
	else
	{ 
		std::cout << "VIOSOWarpBlend library successfully loaded." << std::endl;
		return 1;
	}
}

int SimpleFBOImageProcessor::initializeGraphics()
{
	return 1;
}

bool SimpleFBOImageProcessor::PluginBindsRenderTarget() const
{
	return true;
}

void SimpleFBOImageProcessor::GetRenderTargetParameters(RenderTargetParameters& target_params) const
{
	target_params.depth_target = GL_TEXTURE_2D_MULTISAMPLE;
	target_params.depth_internal_format = GL_DEPTH_COMPONENT32F_NV;
}

bool SimpleFBOImageProcessor::GetRenderTargetTextureParameters(RenderTargetTextureParameters& texture_params) const
{
	ExternalFboMap::const_iterator iter = external_fbo_map_.find(active_window_);

	if (iter != external_fbo_map_.end())
	{
		texture_params.color_texture_id = iter->second->GetSceneColorTexture();
		texture_params.depth_texture_id = iter->second->GetSceneDepthTexture();
		texture_params.num_samples = 4;
		return true;
	}
	return false;
}

void SimpleFBOImageProcessor::shutdown()
{
	for (ExternalFboMap::const_iterator id_fbo_pair = external_fbo_map_.begin(); id_fbo_pair != external_fbo_map_.end(); ++id_fbo_pair)
	{
		id_fbo_pair->second->Unload();
		delete id_fbo_pair->second;
	}
}

void SimpleFBOImageProcessor::update(float frame_delta_time, void *param, unsigned int buffer_size_in_bytes)
{
}

void SimpleFBOImageProcessor::setActiveWindow(int window_id, int window_extents[2])
{
	active_window_ = window_id;

	ExternalFboMap::const_iterator fbo_itr = external_fbo_map_.find(window_id);
	// create a new fbo if we haven't seen this window_id yet
	if (fbo_itr == external_fbo_map_.end())
	{
		VWB_Warper* pWarper = nullptr;
		std::ostringstream s;
		s << "WIN" << window_id;
		VWB_ERROR err = VWB_Create(nullptr, warper_ini_path_.c_str(), s.str().c_str(), &pWarper, 2, warper_log_path_.c_str());
		if (VWB_ERROR_NONE != err)
		{
			std::cout << "FATAL ERROR: could not create warper for window_id " << window_id << " Error code " << (int)err << std::endl;
			return;
		}
		err = VWB_Init(pWarper);
		if (VWB_ERROR_NONE != err)
		{
			std::cout << "FATAL ERROR: could not initialize warper for window_id " << window_id << " Error code " << (int)err << std::endl;
			VWB_Destroy(pWarper);
			return;
		}


		ExternalFbo* new_fbo = new ExternalFbo;
		new_fbo->Load(true, window_extents[0], window_extents[1], pWarper);

		external_fbo_map_[window_id] = new_fbo;
	}
	else if (fbo_itr->second->GetWidth() != window_extents[0] || fbo_itr->second->GetHeight() != window_extents[1])
	{
		fbo_itr->second->UpdateWindowSize(window_extents[0], window_extents[1]);
	}
}

void SimpleFBOImageProcessor::setActiveView(int view_id, int viewport[4])
{
}

//#define BUFFERS_ON
void SimpleFBOImageProcessor::preWindowProcess()
{
#ifdef BUFFERS_ON
	ExternalFboMap::iterator fbo_itr = external_fbo_map_.find(active_window_);
	if (external_fbo_map_.end() != fbo_itr) 
	{
		fbo_itr->second->BindFbo();
	}
#endif
}

void SimpleFBOImageProcessor::postWindowProcess()
{
	ExternalFboMap::iterator fbo_itr = external_fbo_map_.find(active_window_);
	if (external_fbo_map_.end() != fbo_itr)
	{
#ifdef BUFFERS_ON
		fbo_itr->second->UnBindFbo();
		fbo_itr->second->RenderWarp();
#else
		VWB_render(fbo_itr->second->GetWarper(), VWB_UNDEFINED_GL_TEXTURE, VWB_STATEMASK_ALL );
#endif //def BUFFERS_ON
	}
}

void SimpleFBOImageProcessor::preViewProcess()
{
}

void SimpleFBOImageProcessor::postViewProcess()
{
}

bool SimpleFBOImageProcessor::useClipPlanes() const
{
	return true;
}

void SimpleFBOImageProcessor::getClipPlanes(FrustumParameters& frustum_params) const
{
	ExternalFboMap::const_iterator fbo_itr = external_fbo_map_.find(active_window_);
	if (external_fbo_map_.end() != fbo_itr)
	{
		VWB_float eye[3] = { 0 };
		VWB_float dir[3] = { 0 };
		VWB_float matV[16] = { 0 };
		VWB_float clip[6] = { 0 };
		if (nullptr != VWB_getViewProj && VWB_ERROR_NONE == VWB_getViewClip(fbo_itr->second->GetWarper(), eye, dir, matV, clip))
		{
			frustum_params.left_degrees = -clip[0];
			frustum_params.right_degrees = clip[2];
			frustum_params.top_degrees = clip[1];
			frustum_params.bottom_degrees = -clip[3];
			frustum_params.near_plane = clip[4];
			frustum_params.far_plane = clip[5];
			frustum_params.override_near_far = false;
			return;
		}
	}
	{
		frustum_params.left_degrees = -32;
		frustum_params.right_degrees = 32;
		frustum_params.top_degrees = 18;
		frustum_params.bottom_degrees = -18;
		frustum_params.far_plane = 20000;
		frustum_params.near_plane = 0.2f;
		frustum_params.override_near_far = false;
	}
}


bool SimpleFBOImageProcessor::useModelViewOffsets() const
{
	return true;
}

const double* SimpleFBOImageProcessor::getModelViewOffsets() const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	ExternalFboMap::const_iterator fbo_itr = external_fbo_map_.find(active_window_);
	if (external_fbo_map_.end() != fbo_itr)
	{
		VWB_float eye[3] = { 0 };
		VWB_float dir[3] = { 0 };
		VWB_float matV[16] = { 0 };
		VWB_float matP[16] = { 0 };
		if (nullptr != VWB_getViewProj && VWB_ERROR_NONE == VWB_getViewProj(fbo_itr->second->GetWarper(), eye, dir, matV, matP))
		{
			glLoadMatrixf(matV);
		}
		else
		{
			glLoadIdentity();
		}
	}
	else
	{
		glLoadIdentity();
	}

	static double mvm[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mvm);

	glPopMatrix();

	return mvm;
}


SimpleFBOImageProcessor* simple_fbo_image_processor = 0;

DVC_GenesisIG_API IUserDefinedImageProcessor200* gigCreateImageProcessorPlugin()
{
	simple_fbo_image_processor = new SimpleFBOImageProcessor();
	return simple_fbo_image_processor;
}

DVC_GenesisIG_API void gigDeleteImageProcessorPlugin(IUserDefinedImageProcessor200* user_object_instance)
{
	if (simple_fbo_image_processor)
		delete simple_fbo_image_processor;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

