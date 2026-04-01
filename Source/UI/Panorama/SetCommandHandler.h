#pragma once

#include <Features/Combat/SniperRifles/NoScopeInaccuracyVis/NoScopeInaccuracyVisConfigVariables.h>
#include <Features/Skins/SkinsConfigVariables.h>
#include <Features/Visuals/PlayerInfoInWorld/PlayerInfoInWorld.h>
#include <GameClient/Panorama/Slider.h>
#include <GameClient/Panorama/TextEntry.h>
#include <Platform/Macros/FunctionAttributes.h>
#include <Utils/StringParser.h>
#include "Tabs/VisualsTab/HueSlider.h"
#include "Tabs/VisualsTab/IntSlider.h"

template <typename HookContext>
struct SetCommandHandler {
    SetCommandHandler(StringParser& parser, HookContext& hookContext) noexcept
        : parser{parser}
        , hookContext{hookContext}
    {
    }

    void operator()() noexcept
    {
        if (const auto section = parser.getLine('/'); section == "combat") {
            handleCombatSection();
        } else if (section == "hud") {
            handleHudSection();
        } else if (section == "visuals") {
            handleVisualsSection();
        } else if (section == "sound") {
            handleSoundSection();
        } else if (section == "skins") {
            handleSkinsSection();
        }
    }

private:
    void handleSkinsSection() const noexcept
    {
        if (const auto feature = parser.getLine('/'); feature == "skins_ak47") {
            handleTextEntry<skins_vars::Ak47Skin>("skins_ak47");
        } else if (feature == "skins_m4a4") {
            handleTextEntry<skins_vars::M4A4Skin>("skins_m4a4");
        } else if (feature == "skins_m4a1s") {
            handleTextEntry<skins_vars::M4A1SSkin>("skins_m4a1s");
        } else if (feature == "skins_awp") {
            handleTextEntry<skins_vars::AwpSkin>("skins_awp");
        } else if (feature == "skins_music_kit") {
            handleTextEntry<skins_vars::MusicKit>("skins_music_kit");
        }
    }

    template <typename ConfigVariable>
    void handleTextEntry(const char* panelId) const noexcept
    {
        std::uint16_t value{};
        if (parser.parseInt(value)) {
            hookContext.config().template setVariable<ConfigVariable>(value);
        }
    }

    void handleCombatSection() noexcept
    {
    }

    void handleHudSection() const noexcept
    {
    }

    void handleSoundSection() const noexcept
    {
    }

    void handleVisualsSection() const noexcept
    {
        if (const auto feature = parser.getLine('/'); feature == "viewmodel_fov") {
            handleIntSlider<viewmodel_mod_vars::Fov>("viewmodel_fov");
        } else if (feature == "viewmodel_fov_text") {
            handleIntSliderTextEntry<viewmodel_mod_vars::Fov>("viewmodel_fov");
        }
    }

    template <typename ConfigVariable>
    void handleIntSlider(const char* sliderId) const noexcept
    {
        const auto newVariableValue = handleIntSlider(sliderId, ConfigVariable::ValueType::kMin, ConfigVariable::ValueType::kMax, GET_CONFIG_VAR(ConfigVariable));
        hookContext.config().template setVariable<ConfigVariable>(typename ConfigVariable::ValueType{newVariableValue});
    }

    [[nodiscard]] std::uint8_t handleIntSlider(const char* sliderId, std::uint8_t min, std::uint8_t max, std::uint8_t current) const noexcept
    {
        std::uint8_t value{};
        if (!parser.parseInt(value) || value == current || value < min || value > max)
            return current;

        auto&& hueSlider = getIntSlider(sliderId);
        hueSlider.updateTextEntry(value);
        return value;
    }

    template <typename ConfigVariable>
    void handleIntSliderTextEntry(const char* sliderId) const noexcept
    {
        const auto newVariableValue = handleIntSliderTextEntry(sliderId, ConfigVariable::ValueType::kMin, ConfigVariable::ValueType::kMax, GET_CONFIG_VAR(ConfigVariable));
        hookContext.config().template setVariable<ConfigVariable>(typename ConfigVariable::ValueType{newVariableValue});
    }

    [[nodiscard]] std::uint8_t handleIntSliderTextEntry(const char* sliderId, std::uint8_t min, std::uint8_t max, std::uint8_t current) const noexcept
    {
        auto&& slider = getIntSlider(sliderId);
        std::uint8_t value{};
        if (!parser.parseInt(value) || value < min || value > max) {
            slider.updateTextEntry(current);
            return current;
        }

        if (value == current)
            return current;

        slider.updateSlider(value);
        return value;
    }

    [[nodiscard]] decltype(auto) getIntSlider(const char* sliderId) const noexcept
    {
        const auto mainMenuPointer = hookContext.patternSearchResults().template get<MainMenuPanelPointer>();
        auto&& mainMenu = hookContext.template make<ClientPanel>(mainMenuPointer ? *mainMenuPointer : nullptr).uiPanel();
        return hookContext.template make<IntSlider>(mainMenu.findChildInLayoutFile(sliderId));
    }

    StringParser& parser;
    HookContext& hookContext;
};
