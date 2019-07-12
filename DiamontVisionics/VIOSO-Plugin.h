#ifndef VIOSO_Plugin_H
#define VIOSO_Plugin_H

#include "ExternalFbo.h"
#include <map>
#include <string>

class SimpleFBOImageProcessor : public IUserDefinedImageProcessor200
{
public:
	SimpleFBOImageProcessor();
	~SimpleFBOImageProcessor();

	// ======================================================
	// Methods required by IGenesisIGPlugin
	// ======================================================

		/*!
		 * Method called once during the lifetime of the plugin. Allocate resources needed by the plugin.
		 * Note: versions 2.4.0.0 and higher of the IG will call this prior to having an openGL context.
		 * If you were calling OpenGL functionality within this function use the initializeGraphics function.
		 *
		 * @return
		 *  Value <= 0 indicates failure, value >= 1 indicates sucess.
		**/
	int initialize(const char *config_filename = 0);

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
	 * Function called once per frame. This should be applied to all windows and views as defined in window_definition.xml
	 *
	 * @param[in] frame_delta_time : Clock time in seconds between the last frame and the current frame.
	 * @param[in] param : UNUSED
	 * @param[in] buffer_size_in_bytes : UNUSED
	**/
	void update(float frame_delta_time = 0.0166, void *param = 0, unsigned int buffer_size_in_bytes = 0);

	// ======================================================
	// Methods required by IUserDefinedImageProcessor
	// ======================================================

		/*!
		 * When a window is set to be drawn it will be triggered as active and its extents will be reported.
		 * The draw function should be linked to this active window.
		 *
		 * @param[in] window_id :  Window identifier (matches what is set inside window_definitions.xml)
		 * @param[in] window_extents : Window's extents (window_width, window_height)
		**/
	void setActiveWindow(int window_id, int window_extents[2]);

	/*!
	 * When a view is set to be drawn it will be triggered as active and the viewport will be reported.
	 * The draw function should be linked to this active view.
	 *
	 * @param[in] view_id : View identifier (matches what is set inside window_definitions.xml)
	 * @param[in] viewport : View's viewport info (x_offset, y_offset, width, height)
	**/
	void setActiveView(int view_id, int viewport[4]);

	/*!
	 * Perform all window specific processing prior to the active window getting rendered. This should be for the active window.
	**/
	void preWindowProcess();

	/*!
	 * Perform all window specific processing after the active window is rendered. This should be for the active window.
	**/
	void postWindowProcess();

	/*!
	 * Perform all window specific processing prior to the active view getting rendered. This should be for the active view.
	**/
	void preViewProcess();

	/*!
	 * Perform all window specific processing after the active view is rendered. This should be for the active view.
	**/
	void postViewProcess();

	virtual bool useClipPlanes() const;
	virtual void getClipPlanes(FrustumParameters& frustum_params) const;

	/*!
	 * If a model view offset or a heading offset is to be used and adjusted in the plugin, this function must return true.
	 *
	 * @return
	 *  True if using a model view offset, false if not.
	**/
	bool useModelViewOffsets() const;

	/*!
	 * Returns the model view offset matrix if useModelViewOffsets is set to true.
	 *
	 * @return
	 *  The model view offset matrix.
	**/
	const double* getModelViewOffsets() const;

	// ======================================================
	// Methods required by IUserDefinedImageProcessor103
	// ======================================================

	bool PluginBindsRenderTarget() const;
	void GetRenderTargetParameters(RenderTargetParameters& target_params) const;
	bool GetRenderTargetTextureParameters(RenderTargetTextureParameters& texture_params) const;

private:
	int	active_window_;		/* Id of the active window */

	typedef std::map<int, ExternalFbo*> ExternalFboMap;
	ExternalFboMap external_fbo_map_; /* ExternalFbo class, represents the buffer that will be rendered into */
	std::string warper_bin_path_;
	std::string warper_ini_path_;
	std::string warper_log_path_;
};

#endif //def VIOSO-Plugin_H