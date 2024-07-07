#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "GuiBase.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class FlipOverlay: public BakkesMod::Plugin::BakkesModPlugin, /*public BakkesMod::Plugin::PluginWindow,*/ public BakkesMod::Plugin::PluginSettingsWindow
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();
	void checkForFlip();
	void startCheckForFlip();
	// Inherited via PluginSettingsWindow

	virtual void RenderSettings() override;
	virtual std::string GetPluginName() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;

	// Inherited via PluginWindow

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "FlipOverlay";

	/*virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	//virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;*/
private:
	unsigned long int previousFlipState = 0;
	
};

