#pragma once

#ifdef __cplusplus
#include <string>
#include <cstdint>
#include <libultraship/libultra/types.h>

struct GraphicsContext;

namespace SohUtils {
    typedef struct {
        char tex[512];
        u16 width;
        u16 height;
        u8 im_fmt;
        u8 im_siz;
        u8 id;
    } Sprite;

    const std::string& GetSceneName(int32_t scene);

    const std::string& GetItemName(int32_t item);

    const std::string& GetQuestItemName(int32_t item);

    const std::string& GetRandomizerCheckAreaPrefix(int32_t rcarea);

    // Copies a string and ensures the destination is null terminated if the source string is larger than size
    // Only up to size-1 characters are copied from the source string
    void CopyStringToCharArray(char* destination, std::string source, size_t size);

    std::string Sanitize(std::string stringValue);

    // Copies a string into a char buffer up to maxBufferSize characters. This does NOT insert a null terminator
    // on the end, as this is used for in-game messages which are not null-terminated.
    size_t CopyStringToCharBuffer(char* buffer, const std::string& source, size_t maxBufferSize);

    bool IsStringEmpty(std::string str);

    void SpriteLoad(struct GraphicsContext* gfxCtx, SohUtils::Sprite* sprite);

    void SpriteDraw(struct GraphicsContext* gfxCtx, SohUtils::Sprite* sprite, int left, int top, int width, int height);
} // namespace SohUtils
#endif
