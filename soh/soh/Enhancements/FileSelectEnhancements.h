#ifndef FILE_SELECT_ENHANCEMENTS_H
#define FILE_SELECT_ENHANCEMENTS_H

#include <libultraship/libultra/types.h>

struct FileChooseContext;

#ifdef __cplusplus
extern "C" {
#endif
const char* SohFileSelect_GetSettingText(u8 optionIndex, u8 language);
void DrawMoreInfo(struct FileChooseContext* thisx, s16 fileIndex, u8 alpha);
void DrawSeedHashSprites(struct FileChooseContext* thisx);
#ifdef __cplusplus
};
#endif

typedef enum {
    RSM_START_RANDOMIZER,
    RSM_GENERATE_RANDOMIZER,
    RSM_OPEN_RANDOMIZER_SETTINGS,
    RSM_GENERATING,
    RSM_NO_RANDOMIZER_GENERATED,
    RSM_MAX,
} RandomizerSettingsMenuEnums;

#endif
