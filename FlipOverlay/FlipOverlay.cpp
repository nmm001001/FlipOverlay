#include "pch.h"
#include "FlipOverlay.h"


BAKKESMOD_PLUGIN(FlipOverlay, "Displays overlay telling player if there is a flip available or not.", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void FlipOverlay::onLoad()
{
	_globalCvarManager = cvarManager;
	//cvarManager->log("Plugin loaded!");
	LOG("Plugin loaded.");
	startCheckForFlip();
	//cvarManager->registerCvar("FlipOverlayEnabled", "0", "Enable Flip Overlay", true, true, 0, true, 1);

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	cvarManager->log("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	cvarManager->log("the cvar with name: " + cvarName + " changed");
	//	cvarManager->log("the new value is:" + newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&FlipOverlay::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&FlipOverlay::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	cvarManager->log("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", std::bind(&FlipOverlay::YourPluginMethod, this);
}

void FlipOverlay::startCheckForFlip() {
	checkForFlip();
	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		startCheckForFlip();
		}, 0.0001f);
}

void FlipOverlay::checkForFlip() {
	_globalCvarManager = cvarManager;

	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server) { return; }

	CarWrapper car = gameWrapper->GetLocalCar();
	if (!car) { return; }

	unsigned long int flip = car.HasFlip();
	if (flip != previousFlipState) {
		if (flip == 1) {
			LOG("Has Flip");
			previousFlipState = flip;
		} else{
			LOG("No Flip");
			previousFlipState = flip;
		}
	}
}

void FlipOverlay::RenderSettings() {
	ImGui::TextUnformatted("Flip");
}

std::string FlipOverlay::GetPluginName() {
	return "Flip Overlay";
}

void FlipOverlay::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}


void FlipOverlay::onUnload()
{
	LOG("Plugin unloaded.");
}