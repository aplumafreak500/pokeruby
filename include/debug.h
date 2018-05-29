#ifndef GUARD_DEBUG_H
#define GUARD_DEBUG_H

struct UnkDebugMenu3Items {
    const u8 * text;
    size_t (*func)(u8 * dest);
};

// start_menu_debug
void DebugMenu_8077048(void);

// matsuda_debug_menu
void sub_80AAF30(void);
void Crash(const u8 *text);
bool8 InitMatsudaDebugMenu(void);

// tomomichi_debug_menu
void debug_nullsub_66(void);
bool8 InitTomomichiDebugWindow(void);

// sound_check_menu
void Task_InitSoundCheckMenu(u8 taskId);
void CB2_StartSoundCheckMenu(void);
void PrintSignedNumber(int n, u16 x, u16 y, u8 digits);

// nakamura_debug_menu
void NakaGenderTest(void);

// unk debug menu 3
extern const u8 gUnknown_Debug_842E350;
extern const struct UnkDebugMenu3Items gUnknown_Debug_842E2D0[];

// unknown debug menu
int InitSogabeDebugMenu(void);

// kagaya_debug_menu
bool8 InitKagayaDebugMenu_A(void);
bool8 debug_sub_80B0770(void);
bool8 debug_sub_80B07B0(void);
bool8 debug_sub_80B0800(void);

// nohara_debug_menu
bool8 InitNoharaDebugMenu(void);

// watanabe_debug_menu
void InitWatanabeDebugMenu(void);
void InitSizeComparison(void);
void InitBattleForDebug(void);
void InitCreatePokemon(void);
void InitSeePokemonGraphics(void);
void InitSeeTrainers(void);

// taya_debug_menu
bool8 InitTayaDebugWindow(void);

// nakamura_debug_menu
bool8 InitNakamuraDebugMenu(void);

// Luma Debug Menu
bool8 InitLumaDebugMenu(void);
void DS_Adjust_Num(u8 taskId);

#endif // GUARD_DEBUG_H
