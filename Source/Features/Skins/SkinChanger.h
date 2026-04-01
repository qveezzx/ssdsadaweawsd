#pragma once

#include <CS2/Classes/Entities/C_CSPlayerPawn.h>
#include <GameClient/Entities/BaseWeapon.h>
#include <GameClient/Entities/PlayerController.h>
#include <Features/Skins/SkinsConfigVariables.h>

template <typename HookContext>
class SkinChanger {
public:
    explicit SkinChanger(HookContext& hookContext) noexcept
        : hookContext{hookContext}
    {
    }

    void run() const noexcept
    {
        updateMusicKit();
    }

    void handleWeapon(auto&& weapon) const noexcept
    {
        // For now, let's just handle Ak47, M4A4, M4A1S, and AWP
        const auto weaponType = weapon.baseEntity().classify().typeIndex;
        
        std::uint16_t paintKit = 0;
        if (weaponType == EntityTypeInfo::indexOf<cs2::C_AK47>()) {
            paintKit = GET_CONFIG_VAR(skins_vars::Ak47Skin);
        } else if (weaponType == EntityTypeInfo::indexOf<cs2::C_WeaponM4A1>()) {
            paintKit = GET_CONFIG_VAR(skins_vars::M4A4Skin);
        } else if (weaponType == EntityTypeInfo::indexOf<cs2::C_WeaponM4A1Silencer>()) {
            paintKit = GET_CONFIG_VAR(skins_vars::M4A1SSkin);
        } else if (weaponType == EntityTypeInfo::indexOf<cs2::C_WeaponAWP>()) {
            paintKit = GET_CONFIG_VAR(skins_vars::AwpSkin);
        }

        if (paintKit != 0) {
            applySkin(weapon, paintKit);
        }
    }

private:
    void updateMusicKit() const noexcept
    {
        const auto musicKit = GET_CONFIG_VAR(skins_vars::MusicKit);
        if (musicKit == 0)
            return;

        const auto localPlayerController = hookContext.localPlayerController();
        if (!localPlayerController.baseEntity())
            return;

        const auto offset = hookContext.patternSearchResults().template get<OffsetToMusicKit>();
        if (offset) {
            offset.of(localPlayerController.get()).set(musicKit);
        }
    }

    void applySkin(auto&& weapon, std::uint16_t paintKit) const noexcept
    {
        const auto offset = hookContext.patternSearchResults().template get<OffsetToFallbackPaintKit>();
        if (offset) {
            offset.of(weapon.get()).set(paintKit);
        }
    }

    HookContext& hookContext;
};
