#pragma once

#include <CS2/Panorama/CTextEntry.h>
#include <MemoryPatterns/PatternTypes/TextEntryPatternTypes.h>
#include "PanoramaUiEngine.h"
#include "PanoramaUiPanel.h"

template <typename HookContext>
class TextEntry {
public:
    using RawType = cs2::CTextEntry;

    TextEntry(HookContext& hookContext, cs2::CTextEntry* textEntry) noexcept
        : hookContext{hookContext}
        , textEntry{textEntry}
    {
    }

    [[nodiscard]] decltype(auto) uiPanel() const noexcept
    {
        return hookContext.template make<PanoramaUiPanel>(textEntry ? textEntry->uiPanel : nullptr);
    }

    void registerTextSubmittedHandler(cs2::CTextEntry::TextEntrySubmitEventHandler* func)
    {
        auto&& uiEngine = hookContext.template make<PanoramaUiEngine>();
        uiEngine.registerEventHandler(
            uiEngine.makeSymbol(0, "TextEntrySubmit"),
            uiPanel(),
            cs2::CUtlAbstractDelegate{func}
        );
    }

    void setText(const char* text) noexcept
    {
        if (textEntry && setTextFunction())
            setTextFunction()(textEntry, text);
    }

private:
    [[nodiscard]] decltype(auto) setTextFunction() const noexcept
    {
        return hookContext.patternSearchResults().template get<TextEntrySetTextFunction>();
    }

    HookContext& hookContext;
    cs2::CTextEntry* textEntry;
};
