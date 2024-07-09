#include "pch.h"
#include "FlipOverlay.h"


BAKKESMOD_PLUGIN(FlipOverlay, "Displays overlay telling player if there is a flip available or not.", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;


void FlipOverlay::onLoad()
{
	_globalCvarManager = cvarManager;

	LOG("FlipOverlay plugin loaded.");
	//startCheckForFlip();

	cvarManager->registerCvar("FlipOverlayEnabled", "0", "Enable Flip Overlay", true, true, 0, true, 1).addOnValueChanged(
		[this](std::string oldValue, CVarWrapper cvar) {
			flipOverlayEnabled = cvar.getBoolValue();
			if (flipOverlayEnabled && !isCheckingForFlip) {
					isCheckingForFlip = true;
					startCheckForFlip();
			}
			else if (!flipOverlayEnabled){
				isCheckingForFlip = false;
			}
		}
	);

}

void FlipOverlay::startCheckForFlip() {
	if (!flipOverlayEnabled) {
		isCheckingForFlip = false;
		return;
	}

	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		if (flipOverlayEnabled && isCheckingForFlip) { // Only run if the plugin is still enabled and not already checking
			checkForFlip();
			startCheckForFlip(); // Schedule the next check
		}
		else {
			isCheckingForFlip = false;
		}
		}, 0.001f);

}

void FlipOverlay::checkForFlip() {
	
	if (!gameWrapper->IsInGame()) {
		return;
	}

	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server) { return; }

	CarWrapper car = gameWrapper->GetLocalCar();
	if (!car) { return; }

	unsigned long int flip = car.HasFlip();
	if (flip != previousFlipState) {
		previousFlipState = flip;

		gameWrapper->Execute([this](GameWrapper* gw) {
			// Trigger the Render method to update the overlay text
			if (flipOverlayEnabled) {

				this->Render();
			}
			});
	}
}

void FlipOverlay::RenderSettings() {
	ImGui::TextUnformatted("FlipOverlay");

	

	CVarWrapper enableCvar = cvarManager->getCvar("FlipOverlayEnabled");
	if (!enableCvar) { return; }
	bool enabled = enableCvar.getBoolValue();

	if (ImGui::Checkbox("Enable FlipOverlay", &enabled)) {
		enableCvar.setValue(enabled);
	}

}

std::string FlipOverlay::GetPluginName() {
	return "Flip Overlay";
}

void FlipOverlay::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

void FlipOverlay::Render() {


	ImGuiWindowFlags WindowFlags =
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoDecoration
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoInputs
		| ImGuiWindowFlags_NoBackground;

	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, WindowFlags))
	{
		ImGui::End();
		return;
	}

	if (flipOverlayEnabled) {
		ServerWrapper server = gameWrapper->GetCurrentGameState();
		if (!server) {
			ImGui::End();
			return;
		}

		CarWrapper car = gameWrapper->GetLocalCar();
		if (!car) {
			ImGui::End();
			return;
		}
		ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, WindowFlags);
		unsigned long int flip = car.HasFlip();
		if (flip == 1) {
			LOG("Has Flip!");
			ImGui::Text("Has Flip");
		}
		else {
			LOG("No Flip!");
			ImGui::Text("No Flip");
		}

	ImGui::End();
	}

		
}

void FlipOverlay::onUnload()
{
	LOG("FlipOverlay Plugin unloaded.");
	flipOverlayEnabled = false;
	isCheckingForFlip = false;
}