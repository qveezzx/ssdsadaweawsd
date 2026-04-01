#pragma once

#include <GameClient/Panorama/PanoramaDropDown.h>
#include <GameClient/Panorama/TextEntry.h>
#include <EntryPoints/GuiEntryPoints.h>
#include <Platform/Macros/FunctionAttributes.h>
#include <Features/Skins/SkinsConfigVariables.h>
#include <Utils/StringBuilder.h>
#include "PanoramaGUI.h"

template <typename HookContext>
class SkinsTab {
public:
    explicit SkinsTab(HookContext& hookContext) noexcept
        : hookContext{hookContext}
    {
    }

    void init(auto&& guiPanel) const
    {
        initTextEntry<skins_vars::Ak47Skin>(guiPanel, "skins_ak47");
        initTextEntry<skins_vars::M4A4Skin>(guiPanel, "skins_m4a4");
        initTextEntry<skins_vars::M4A1SSkin>(guiPanel, "skins_m4a1s");
        initTextEntry<skins_vars::AwpSkin>(guiPanel, "skins_awp");
        initTextEntry<skins_vars::MusicKit>(guiPanel, "skins_music_kit");
    }

    void updateFromConfig(auto&& mainMenu) const noexcept
    {
        updateTextEntry<skins_vars::Ak47Skin>(mainMenu, "skins_ak47");
        updateTextEntry<skins_vars::M4A4Skin>(mainMenu, "skins_m4a4");
        updateTextEntry<skins_vars::M4A1SSkin>(mainMenu, "skins_m4a1s");
        updateTextEntry<skins_vars::AwpSkin>(mainMenu, "skins_awp");
        updateTextEntry<skins_vars::MusicKit>(mainMenu, "skins_music_kit");
    }

private:
    template <typename ConfigVariable>
    void initTextEntry(auto&& guiPanel, const char* panelId) const
    {
        auto&& textEntry = guiPanel.findChildInLayoutFile(panelId).clientPanel().template as<TextEntry>();
        textEntry.registerTextSubmittedHandler(&GuiEntryPoints<HookContext>::template textEntrySubmitted<ConfigVariable>);
    }

    template <typename ConfigVariable>
    void updateTextEntry(auto&& mainMenu, const char* panelId) const noexcept
    {
        auto&& textEntry = mainMenu.findChildInLayoutFile(panelId).clientPanel().template as<TextEntry>();
        StringBuilderStorage<20> storage;
        storage.builder().put(GET_CONFIG_VAR(ConfigVariable));
        textEntry.setText(storage.builder().cstring());
    }
    HookContext& hookContext;
};
