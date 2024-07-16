#include "pch.h"
#include "FlipOverlay.h"

BAKKESMOD_PLUGIN(FlipOverlay, "Displays overlay telling player if there is a flip available or not.", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void FlipOverlay::onLoad()
{
    _globalCvarManager = cvarManager;

    LOG("FlipOverlay plugin loaded.");

    // Register the Render function to be called every frame
    gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) {
        RenderOverlay(canvas);
        });

    cvarManager->registerCvar("FlipOverlayEnabled", "0", "Enable Flip Overlay", true, true, 0, true, 1).addOnValueChanged(
        [this](std::string oldValue, CVarWrapper cvar) {
            flipOverlayEnabled = cvar.getBoolValue();
        }
    );
}
void FlipOverlay::Render() {};

void FlipOverlay::RenderOverlay(CanvasWrapper canvas) {
    if (!flipOverlayEnabled) return;

    if (!gameWrapper->IsInGame()) return;

    ServerWrapper server = gameWrapper->GetCurrentGameState();
    if (!server) return;

    CarWrapper car = gameWrapper->GetLocalCar();
    if (!car) return;

    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoBackground;



    unsigned long int flip = car.HasFlip();
    if (flip == 1) {
        //canvas.SetPosition(Vector2F{ 100.0, 100.0 });
        canvas.SetColor(96, 255, 51, 0);
        canvas.DrawString("Has Flip", 2.0, 2.0, false);
    }
    else {
        //canvas.SetPosition(Vector2F{ 100.0, 100.0 });
        //canvas.SetColor(255, 51, 51, 0);
        canvas.DrawString("No Flip", 2.0, 2.0, false);
    }
}

void FlipOverlay::RenderSettings() {
    ImGui::TextUnformatted("FlipOverlay");

    CVarWrapper enableCvar = cvarManager->getCvar("FlipOverlayEnabled");
    if (!enableCvar) return;

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

std::string FlipOverlay::GetMenuName() {
    return "FlipOverlay";
}

std::string FlipOverlay::GetMenuTitle() {
    return "FlipOverlay";
}

bool FlipOverlay::ShouldBlockInput() {
    return false;
}

bool FlipOverlay::IsActiveOverlay() {
    return flipOverlayEnabled;
}

void FlipOverlay::OnOpen() {
    isWindowOpen_ = true;
}

void FlipOverlay::OnClose() {
    isWindowOpen_ = false;
}

void FlipOverlay::onUnload() {
    LOG("FlipOverlay Plugin unloaded.");
    flipOverlayEnabled = false;
}
