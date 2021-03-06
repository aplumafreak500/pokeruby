#include "global.h"
#include "../data/git.h"
#if DEBUG
#include "menu.h"
#include "main.h"
#include "script.h"
#include "sound.h"
#include "constants/songs.h"
#include "debug.h"
#include "gba/m4a_internal.h"
#include "event_data.h"
#include "string_util.h"
#include "save.h"
#include "overworld.h"
#include "constants/map_groups.h"
#include "fieldmap.h"
#include "field_fadetransition.h"
#include "region_map.h"
#include "pokemon_storage_system.h"

UNUSED u8 LumaDebugMenu_AddNewPKMN();
static void DS_Adjust_Task(u8 taskId);
static u8 LumaDebugMenu_ProcessInput();
static u8 LumaDebugMenu_CloseMenu();
u8 LumaDebugMenu_OpenMemoryEditorMenu();
static u8 LumaDebugMenu_OpenMemoryEditor(u32);
static u8 MemoryEditorMenu_ProcessInput();
static u8 MemoryEditor_Bios();
static u8 MemoryEditor_Ewram();
static u8 MemoryEditor_Iwram();
static u8 MemoryEditor_IO();
static u8 MemoryEditor_Vram();
static u8 MemoryEditor_Pal();
static u8 MemoryEditor_Oam();
static u8 MemoryEditor_Rom();
static u8 MemoryEditor_Fram();
UNUSED static u8 MemoryEditor_Sram();
UNUSED static u8 MemoryEditor_Eeprom();
u8 LumaDebugMenu_OpenFlagEditor();
u8 LumaDebugMenu_OpenVarEditor();
static void LumaDebugMenu_InitFlagEditor();
static void LumaDebugMenu_FlagEditorTask(u8);
static void LumaDebugMenu_DrawFlagStatus(u16);
static void LumaDebugMenu_FlagEditorHandleInput(u8);
static void LumaDebugMenu_ExitFlagEditor(u8);
static void LumaDebugMenu_BackToMainMenu();
static void LumaDebugMenu_MenuCB(u8);
static u8 LumaDebugMenu_ReloadMainMenu();
static void LumaDebugMenu_InitVarEditor();
static void LumaDebugMenu_VarEditorTask(u8);
static void LumaDebugMenu_DrawVarStatus(u16);
static void LumaDebugMenu_VarEditorHandleInput(u8);
static void LumaDebugMenu_ExitVarEditor(u8);
u8 LumaDebugMenu_FixBadEggs();
u8 LumaDebugMenu_SaveSerializedGame();
u8 LumaDebugMenu_ClearPKMN();
//u8 LumaDebugMenu_EditPokedex();
u8 LumaDebugMenu_Warp();
static void LumaDebugMenu_WarpProcessInput(u8);

extern u8 (*gMenuCallback)();
extern u8 DebugScript_New0;
extern u16 *gSpecialVars[];
extern u8 gDifferentSaveFile;

static const u8 Str_AddNewPKMN[] = _("Add Victini to party");
static const u8 Str_FlagEdit[] = _("Change Flag");
static const u8 Str_VarEdit[] = _("Change State");
static const u8 Str_SaveGame[] = _("SaveSerializedGame");
static const u8 Str_FixBadEggs[] = _("Fix Bad EGG");
static const u8 Str_MemoryEditor[] = _("View/edit memory region");
static const u8 Str_ClearPKMN[] = _("Clear {PKMN}");
static const u8 Str_EditPokedex[] = _("Edit {POKE}dex");
static const u8 Str_MemoryEditorBios[] = _("Bios");
static const u8 Str_MemoryEditorIO[] = _("AGB I/O");
static const u8 Str_MemoryEditorVram[] = _("VRAM");
static const u8 Str_MemoryEditorIwram[] = _("IWRAM");
static const u8 Str_MemoryEditorEwram[] = _("EWRAM");
static const u8 Str_MemoryEditorPal[] = _("PRAM");
static const u8 Str_MemoryEditorOam[] = _("ORAM");
static const u8 Str_MemoryEditorRom[] = _("ROM");
static const u8 Str_MemoryEditorSram[] = _("SRAM");
static const u8 Str_MemoryEditorEeprom[] = _("EEPROM");
static const u8 Str_MemoryEditorFram[] = _("Flash RAM");
static const u8 Str_MemoryEditorBank[] = _("Bank no. $");
static const u8 Str_MemoryEditorOther[] = _("$");
static const u8 Str_Warp[] = _("Warp");

static const u8 Str_ChangeFlagState[] = _("Edit flag state");
static const u8 Str_ChangeVarState[] = _("Edit var state");
static const u8 Str_ChangeFlagState2[] = _("LR: Edit");
static const u8 dsStr[] = _("DS Num");

IWRAM_DATA s8 DS_Sound_Count;
IWRAM_DATA static u16 currentFlag;
IWRAM_DATA static u16 currentVar;

static const u8* const RamPointerStrings[] = {
	Str_MemoryEditorBios,       // 0x00000000
	Str_MemoryEditorOther,  // 0x01000000
	Str_MemoryEditorEwram,      // 0x02000000
	Str_MemoryEditorIwram,      // 0x03000000
	Str_MemoryEditorIO,         // 0x04000000
	Str_MemoryEditorVram,       // 0x05000000
	Str_MemoryEditorPal,        // 0x06000000
	Str_MemoryEditorOam,        // 0x07000000
	Str_MemoryEditorRom,        // 0x08000000
	Str_MemoryEditorRom,        // 0x09000000
	Str_MemoryEditorRom,        // 0x0a000000
	Str_MemoryEditorRom,        // 0x0b000000
	Str_MemoryEditorRom,        // 0x0c000000
	Str_MemoryEditorRom,        // 0x0d000000
	Str_MemoryEditorSram,       // 0x0e000000
	Str_MemoryEditorOther       // 0x0f000000
};

static const struct MenuAction LumaDebugMenuItems[] = {
	{Str_AddNewPKMN, LumaDebugMenu_CloseMenu},
	{Str_MemoryEditor, LumaDebugMenu_OpenMemoryEditorMenu},
	{Str_SaveGame, LumaDebugMenu_SaveSerializedGame},
	{Str_FlagEdit, LumaDebugMenu_OpenFlagEditor},
	{Str_VarEdit, LumaDebugMenu_OpenVarEditor},
	{Str_FixBadEggs, LumaDebugMenu_FixBadEggs},
	{Str_ClearPKMN, LumaDebugMenu_ClearPKMN},
	{Str_EditPokedex, LumaDebugMenu_CloseMenu/*LumaDebugMenu_EditPokedex*/},
	{Str_Warp, LumaDebugMenu_Warp}
};

static const struct MenuAction MemoryEditorItems[] = {
	{Str_MemoryEditorBios, MemoryEditor_Bios},
	{Str_MemoryEditorEwram, MemoryEditor_Ewram},
	{Str_MemoryEditorIwram, MemoryEditor_Iwram},
	{Str_MemoryEditorIO, MemoryEditor_IO},
	{Str_MemoryEditorVram, MemoryEditor_Vram},
	{Str_MemoryEditorPal, MemoryEditor_Pal},
	{Str_MemoryEditorOam, MemoryEditor_Oam},
	{Str_MemoryEditorRom, MemoryEditor_Rom},
	{Str_MemoryEditorFram, MemoryEditor_Fram},
};

bool8 InitLumaDebugMenu() {
	Menu_EraseScreen();
	Menu_DrawStdWindowFrame(0, 0, 17, 19);
	Menu_PrintItems(1, 1, 9, LumaDebugMenuItems);
	InitMenu(0, 1, 1, 9, 0, 16);
	gMenuCallback = LumaDebugMenu_ProcessInput;
	return 0;
}

static u8 LumaDebugMenu_ProcessInput() {
	s8 input = Menu_ProcessInput();

	if (input == -2) {
		return 0;
	}
	else if (input == -1) {
		CloseMenu();
		return 1;
	}
	else {
		gMenuCallback = LumaDebugMenuItems[input].func;
		return 0;
	}
}

UNUSED u8 LumaDebugMenu_AddNewPKMN() {
	ScriptContext1_SetupScript(&DebugScript_New0);
	PlaySE(SE_EXP_MAX);
	CloseMenu();
	return 1;
}

u8 LumaDebugMenu_CloseMenu() {
	CloseMenu();
	return 1;
}

u8 LumaDebugMenu_OpenMemoryEditorMenu() {
	Menu_EraseScreen();
	Menu_DrawStdWindowFrame(0, 0, 10, 19);
	Menu_PrintItems(1, 1, 9, MemoryEditorItems);
	InitMenu(0, 1, 1, 9, 0, 9);
	gMenuCallback = MemoryEditorMenu_ProcessInput;
	return 0;
}

static u8 MemoryEditorMenu_ProcessInput() {
	s8 input = Menu_ProcessInput();

	if (input == -2) {
		return 0;
	}
	else if (input == -1) {
		CloseMenu();
		return 1;
	}
	else {
		gMenuCallback = MemoryEditorItems[input].func;
		return 0;
	}
}

static u8 MemoryEditor_Bios() {
	return LumaDebugMenu_OpenMemoryEditor(0x0000000);
}

static u8 MemoryEditor_Ewram() {
	return LumaDebugMenu_OpenMemoryEditor(0x2000000);
}

static u8 MemoryEditor_Iwram() {
	return LumaDebugMenu_OpenMemoryEditor(0x3000000);
}

static u8 MemoryEditor_IO() {
	return LumaDebugMenu_OpenMemoryEditor(0x4000000);
}

static u8 MemoryEditor_Vram() {
	return LumaDebugMenu_OpenMemoryEditor(0x5000000); // TODO: verify this
}

static u8 MemoryEditor_Pal() {
	return LumaDebugMenu_OpenMemoryEditor(0x6000000); // TODO: verify this
}

static u8 MemoryEditor_Oam() {
	return LumaDebugMenu_OpenMemoryEditor(0x7000000); // TODO: verify this
}

static u8 MemoryEditor_Rom() {
	return LumaDebugMenu_OpenMemoryEditor(0x8000000);
}

static u8 MemoryEditor_Fram() {
	return LumaDebugMenu_OpenMemoryEditor(0xe000000);
}

UNUSED static u8 MemoryEditor_Sram() {
	return LumaDebugMenu_OpenMemoryEditor(0xe000000);
}

UNUSED static u8 MemoryEditor_Eeprom() {
	return LumaDebugMenu_OpenMemoryEditor(0xd000000);
}

static u8 LumaDebugMenu_OpenMemoryEditor(u32 address) {
	// TODO
	CloseMenu();
	return 1;
}

u8 LumaDebugMenu_OpenFlagEditor() {
	CloseMenu();
	LumaDebugMenu_InitFlagEditor();
	ScriptContext2_Enable();
	return 1;
}

u8 LumaDebugMenu_OpenVarEditor() {
	CloseMenu();
	LumaDebugMenu_InitVarEditor();
	ScriptContext2_Enable();
	return 1;
}

static void LumaDebugMenu_InitFlagEditor() {
	u8 taskId = CreateTask(LumaDebugMenu_FlagEditorTask, 80);
	Menu_EraseScreen();
	if (!currentFlag) currentFlag = 0x20;
	gTasks[taskId].data[1] = currentFlag;
}

static void LumaDebugMenu_FlagEditorTask(u8 taskId) {
	struct Task *task = gTasks + taskId;
	Menu_DisplayDialogueFrame();
	Menu_PrintText(Str_ChangeFlagState, 2, 15);
	Menu_PrintText(Str_ChangeFlagState2, 15, 15);
	LumaDebugMenu_DrawFlagStatus(task->data[1]);
	task->func = LumaDebugMenu_FlagEditorHandleInput;
}

static void LumaDebugMenu_DrawFlagStatus(u16 flag) {
	u8 flag_state;
	Menu_BlankWindowRect(4, 17, 22, 18);
	ConvertIntToHexStringN(gStringVar1, flag, STR_CONV_MODE_RIGHT_ALIGN, 4);
	Menu_PrintText(gStringVar1, 4, 17);
	flag_state = FlagGet(flag);
	ConvertIntToDecimalStringN(gStringVar1, flag_state, STR_CONV_MODE_RIGHT_ALIGN, 1);
	Menu_PrintText(gStringVar1, 18, 17);
}

static void LumaDebugMenu_FlagEditorHandleInput(u8 taskId) {
	struct Task *task = gTasks + taskId;
	if (gMain.newKeys & (A_BUTTON | B_BUTTON | START_BUTTON)) {
		task->func = LumaDebugMenu_ExitFlagEditor;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_LEFT) {
		task->data[1]--;
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_RIGHT) {
		task->data[1]++;
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_UP) {
		task->data[1]+=16;
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_DOWN) {
		task->data[1]-=16;
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	else if (gMain.newKeys & L_BUTTON) {
		FlagClear(task->data[1]);
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	else if (gMain.newKeys & R_BUTTON) {
		FlagSet(task->data[1]);
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	// TODO: Select toggle hex/dec display
}

static void LumaDebugMenu_ExitFlagEditor(u8 taskId) {
	struct Task *task = gTasks + taskId;
	Menu_EraseScreen();
	ScriptContext2_Disable();
	currentFlag = task->data[1];
	DestroyTask(taskId);
	LumaDebugMenu_BackToMainMenu();
}

static void LumaDebugMenu_BackToMainMenu() {
	gMenuCallback = LumaDebugMenu_ReloadMainMenu;
	ScriptContext2_Enable();
	CreateTask(LumaDebugMenu_MenuCB, 80);
}

static void LumaDebugMenu_MenuCB(u8 taskId) {
	if (gMenuCallback() == TRUE)
		DestroyTask(taskId);
}

static u8 LumaDebugMenu_ReloadMainMenu() {
	InitMenuWindow(&gMenuTextWindowTemplate);
	InitLumaDebugMenu();
	return FALSE;
}

static u16 *DebugGetVarPointer(u16 id)
{
    if (id < 0x8000)
        return &gSaveBlock1.vars[id - VARS_START];
    return gSpecialVars[id - 0x8000];
}

static u16 DebugVarGet(u16 id)
{
    u16 *ptr = DebugGetVarPointer(id);
    if (!ptr)
        return id;
    return *ptr;
}

static bool8 DebugVarSet(u16 id, u16 value)
{
    u16 *ptr = DebugGetVarPointer(id);
    if (!ptr)
        return FALSE;
    *ptr = value;
    return TRUE;
}

static void LumaDebugMenu_InitVarEditor() {
	u8 taskId = CreateTask(LumaDebugMenu_VarEditorTask, 80);
	Menu_EraseScreen();
	if (!currentVar) currentVar = 0x4000;
	gTasks[taskId].data[1] = currentVar;
}

static void LumaDebugMenu_VarEditorTask(u8 taskId) {
	struct Task *task = gTasks + taskId;
	Menu_DisplayDialogueFrame();
	Menu_PrintText(Str_ChangeVarState, 2, 15);
	Menu_PrintText(Str_ChangeFlagState2, 15, 15);
	LumaDebugMenu_DrawVarStatus(task->data[1]);
	task->func = LumaDebugMenu_VarEditorHandleInput;
}

static void LumaDebugMenu_DrawVarStatus(u16 var) {
	u16 var_state;
	Menu_BlankWindowRect(4, 17, 22, 18);
	ConvertIntToHexStringN(gStringVar1, var, STR_CONV_MODE_RIGHT_ALIGN, 4);
	Menu_PrintText(gStringVar1, 4, 17);
	var_state = DebugVarGet(var);
	ConvertIntToHexStringN(gStringVar1, var_state, STR_CONV_MODE_RIGHT_ALIGN, 4);
	Menu_PrintText(gStringVar1, 17, 17);
}

static void LumaDebugMenu_VarEditorHandleInput(u8 taskId) {
	struct Task *task = gTasks + taskId;
	if (gMain.newKeys & START_BUTTON) {
		task->func = LumaDebugMenu_ExitVarEditor;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_LEFT) {
		task->data[1]--;
		task->func = LumaDebugMenu_VarEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_RIGHT) {
		task->data[1]++;
		task->func = LumaDebugMenu_VarEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_UP) {
		task->data[1]+=16;
		task->func = LumaDebugMenu_VarEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_DOWN) {
		task->data[1]-=16;
		task->func = LumaDebugMenu_VarEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & L_BUTTON) {
		DebugVarSet(task->data[1], DebugVarGet(task->data[1])-16);
		task->func = LumaDebugMenu_VarEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & R_BUTTON) {
		DebugVarSet(task->data[1], DebugVarGet(task->data[1])+16);
		task->func = LumaDebugMenu_VarEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & B_BUTTON) {
		DebugVarSet(task->data[1], DebugVarGet(task->data[1])-1);
		task->func = LumaDebugMenu_VarEditorTask;
	}
	else if (gMain.newAndRepeatedKeys & A_BUTTON) {
		DebugVarSet(task->data[1], DebugVarGet(task->data[1])+1);
		task->func = LumaDebugMenu_VarEditorTask;
	}
	// TODO: Select toggle hex/dec display
}

static void LumaDebugMenu_ExitVarEditor(u8 taskId) {
	struct Task *task = gTasks + taskId;
	Menu_EraseScreen();
	ScriptContext2_Disable();
	currentVar = task->data[1];
	DestroyTask(taskId);
	LumaDebugMenu_BackToMainMenu();
}

u8 LumaDebugMenu_FixBadEggs() {
	u8 i;
	for (i = 0; i < gPlayerPartyCount; i++) {
		if (GetMonData(gPlayerParty[i], MON_DATA_SANITY_BIT1, NULL)) {
			SetMonData(gPlayerParty[i], MON_DATA_SANITY_BIT1, 0);
			// SetMonData(gPlayerParty[i], MON_DATA_CHECKSUM, CalculateBoxMonChecksum(gPlayerParty[i]));
			PlaySE(SE_CONTEST_HEART);
		}
	}
	CloseMenu();
	return 1;
}

u8 LumaDebugMenu_SaveSerializedGame() {
	u8 ret;
	CloseMenu();
	save_serialize_map();
	IncrementGameStat(GAME_STAT_SAVED_GAME);
	if (gDifferentSaveFile) {
		ret = Save_WriteData(SAVE_OVERWRITE_DIFFERENT_FILE);
		gDifferentSaveFile = FALSE;
	}
	else {
		ret = Save_WriteData(SAVE_NORMAL);
	}
	if (ret == SAVE_STATUS_OK) {
		PlaySE(SE_SAVE);
	}
	else {
		PlaySE(SE_BOO);
	}
	while(!IsSEPlaying());
	return ret;
}

u8 LumaDebugMenu_ClearPKMN() {
	CloseMenu();
	ResetPokemonStorageSystem();
	PlaySE(SE_EXP_MAX);
	return 1;
}

static const u8 Str_WarpMenuText[] = _("{COLOR BLUE}{UP_ARROW}{DOWN_ARROW} - Map {LEFT_ARROW}{RIGHT_ARROW} - Bank\n");
static const u8 Str_WarpMenuBank[] = _("Map Bank: {STR_VAR_1}");
static const u8 Str_WarpMenuNum[] = _("Map Num: {STR_VAR_1}");
static const u8 Str_WarpMenuText2[] = _("A/Start: Warp to map");

static void LumaDebugMenu_WarpPrintMapName(u8 bank, u8 map, u8 x, u8 y) {
	struct MapHeader* map_hdr = Overworld_GetMapHeaderByGroupAndId(bank, map);
	CopyMapName(gStringVar1, map_hdr->regionMapSectionId);
	Menu_PrintText(gStringVar1, x, y);
}

static void LumaDebugMenu_DrawWarpMenu(u8 taskid) {
	Menu_PrintText(Str_WarpMenuText, 1, 1);
	ConvertIntToDecimalStringN(gStringVar1, gTasks[taskid].data[1], STR_CONV_MODE_RIGHT_ALIGN, 3);
	Menu_PrintText(Str_WarpMenuBank, 1, 3);
	ConvertIntToDecimalStringN(gStringVar1, gTasks[taskid].data[2], STR_CONV_MODE_RIGHT_ALIGN, 3);
	Menu_PrintText(Str_WarpMenuNum, 13, 3);
	Menu_BlankWindowRect(1, 5, 14, 6);
	LumaDebugMenu_WarpPrintMapName(gTasks[taskid].data[1], gTasks[taskid].data[2], 1, 5);
	Menu_PrintText(Str_WarpMenuText2, 15, 5);
	gTasks[taskid].func = LumaDebugMenu_WarpProcessInput;
}

u8 LumaDebugMenu_Warp() {
	u8 taskid;
	CloseMenu();
	Menu_EraseScreen();
	Menu_DrawStdWindowFrame(0, 0, 29, 7);
	taskid = CreateTask(LumaDebugMenu_DrawWarpMenu, 80);
	gTasks[taskid].data[1] = 0;
	gTasks[taskid].data[2] = 0;
	ScriptContext2_Enable();
	return 1;
}

static void LumaDebugMenu_WarpProcessInput(u8 taskid) {
	if (gMain.newKeys & B_BUTTON) {
		Menu_EraseScreen();
		PlaySE(SE_SELECT);
		DestroyTask(taskid);
		ScriptContext2_Disable();
		return;
	}
	if (gMain.newKeys & (A_BUTTON | START_BUTTON)) {
		Menu_EraseScreen();
		PlaySE(SE_PIN);
		DestroyTask(taskid);
		Overworld_SetWarpDestination(gTasks[taskid].data[1], gTasks[taskid].data[2], -1, -1, -1);
		sub_8080E88();
		return;
	}
	if (gMain.newAndRepeatedKeys & DPAD_UP) {
		gTasks[taskid].data[1]++;
		if (gTasks[taskid].data[1] >= 256/*MAP_GROUPS_COUNT*/) {
			gTasks[taskid].data[1] = 0;
		}
		PlaySE(SE_SELECT);
		gTasks[taskid].func = LumaDebugMenu_DrawWarpMenu;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_DOWN) {
		gTasks[taskid].data[1]--;
		if (gTasks[taskid].data[1] < 0) {
			gTasks[taskid].data[1] = 255/*MAP_GROUPS_COUNT - 1*/;
		}
		PlaySE(SE_SELECT);
		gTasks[taskid].func = LumaDebugMenu_DrawWarpMenu;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_RIGHT) {
		gTasks[taskid].data[2]++;
		if (gTasks[taskid].data[2] > 255) {
			gTasks[taskid].data[2] = 0;
		}
		PlaySE(SE_SELECT);
		gTasks[taskid].func = LumaDebugMenu_DrawWarpMenu;
	}
	else if (gMain.newAndRepeatedKeys & DPAD_LEFT) {
		gTasks[taskid].data[2]--;
		if (gTasks[taskid].data[2] < 0) {
			gTasks[taskid].data[2] = 255;
		}
		PlaySE(SE_SELECT);
		gTasks[taskid].func = LumaDebugMenu_DrawWarpMenu;
	}
}

// Sound test

void DS_Adjust_Num(u8 taskId) {
	REG_DISPCNT = 0x3140;
	Menu_DrawStdWindowFrame(0, 0, 29, 19);
	Menu_PrintText(dsStr, 3, 2);
	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
	gTasks[taskId].func = DS_Adjust_Task;
}

static void DS_Adjust_Task(u8 taskId) {
	if (gMain.newKeys & B_BUTTON) {
		REG_DISPCNT = 0x7140;
		REG_WIN0H = WIN_RANGE(17, 223);
		REG_WIN0V = WIN_RANGE(1, 31);
		Menu_EraseWindowRect(0, 0, 29, 19);
		gTasks[taskId].func = Task_InitSoundCheckMenu;
		return;
	}
	if (gMain.newAndRepeatedKeys & DPAD_RIGHT) {
		DS_Sound_Count++;
		if (DS_Sound_Count > 10)
			DS_Sound_Count = 0;
		m4aSoundMode(DS_Sound_Count << SOUND_MODE_MAXCHN_SHIFT);
	}
	else if (gMain.newAndRepeatedKeys & DPAD_LEFT) {
		DS_Sound_Count--;
		if (DS_Sound_Count < 0)
			DS_Sound_Count = 10;
		m4aSoundMode(DS_Sound_Count << SOUND_MODE_MAXCHN_SHIFT);
	}
	PrintSignedNumber(DS_Sound_Count, 7, 2, 3);
}

static const u8 VersionName_Invalid[] = _("-----");
static const u8 VersionName_None[] = _("None");
static const u8 VersionName_Sapphire[] = _("Sapphire");
static const u8 VersionName_Ruby[] = _("Ruby");
static const u8 VersionName_Emerald[] = _("Emerald");
static const u8 VersionName_FR[] = _("Fire Red");
static const u8 VersionName_LG[] = _("Leaf Green");
static const u8 VersionName_BB[] = _("Blue$GBA");
static const u8 VersionName_HG[] = _("Heart Gold");
static const u8 VersionName_SS[] = _("Soul Silver");
static const u8 VersionName_CDS[] = _("Crystal$DS");
static const u8 VersionName_Diamond[] = _("Diamond");
static const u8 VersionName_Pearl[] = _("Pearl");
static const u8 VersionName_Platinum[] = _("Platinum");
static const u8 VersionName_GC[] = _("XD / Colosseum");
static const u8 VersionName_White[] = _("White");
static const u8 VersionName_Black[] = _("Black");
static const u8 VersionName_White2[] = _("White 2");
static const u8 VersionName_Black2[] = _("Black 2");
static const u8 VersionName_X[] = _("X");
static const u8 VersionName_Y[] = _("Y");
static const u8 VersionName_OR[] = _("Omega Ruby");
static const u8 VersionName_AS[] = _("Alpha Sapphire");
static const u8 VersionName_TE[] = _("Emerald$3DS");
static const u8 VersionName_Z[] = _("Z");
static const u8 VersionName_Sun[] = _("Sun");
static const u8 VersionName_Moon[] = _("Moon");
static const u8 VersionName_US[] = _("Ultra Sun");
static const u8 VersionName_UM[] = _("Ultra Moon");
static const u8 VersionName_Go[] = _("Pokémon Go");
static const u8 VersionName_Red[] = _("Red");
static const u8 VersionName_Blue[] = _("Blue");
static const u8 VersionName_Green[] = _("Green");
static const u8 VersionName_Yellow[] = _("Yellow");
static const u8 VersionName_Gold[] = _("Gold");
static const u8 VersionName_Silver[] = _("Silver");
static const u8 VersionName_Crystal[] = _("Crystal");
static const u8 VersionName_LGP[] = _("Let's Go Pikachu");
static const u8 VersionName_LGE[] = _("Let's Go Eevee");
static const u8 VersionName_Sword[] = _("Sword");
static const u8 VersionName_Shield[] = _("Shield");
static const u8 VersionName_SwShThird[] = _("Gen 8 Third Version");
static const u8 VersionName_Gen8SinnohRemakeDia[] = _("Diamond$Switch");
static const u8 VersionName_Gen8SinnohRemakePrl[] = _("Pearl$Switch");
static const u8 VersionName_Gen8SinnohRemakePla[] = _("Platinum$Switch");
static const u8 VersionName_Gen9First[] = _("Gen 9 First Version");
static const u8 VersionName_Gen9Second[] = _("Gen 9 Second Version");
static const u8 VersionName_Gen9Third[] = _("Gen 9 Third Version");

static const u8 LanguageName_None[] = _("None");
static const u8 LanguageName_Jap[] = _("Japanese");
static const u8 LanguageName_Eng[] = _("English");
static const u8 LanguageName_Fra[] = _("French");
static const u8 LanguageName_Ita[] = _("Italian");
static const u8 LanguageName_Ger[] = _("German");
static const u8 LanguageName_Ntl[] = _("Dutch");
static const u8 LanguageName_Spa[] = _("Spanish");
static const u8 LanguageName_Kor[] = _("Korean");
static const u8 LanguageName_Chn[] = _("Chinese T");
static const u8 LanguageName_Tia[] = _("Chinese S");

const u8* const VersionNameTable[VERSION_SHIELD+2] = {
	VersionName_None,
	VersionName_Sapphire,
	VersionName_Ruby,
	VersionName_Emerald,
	VersionName_FR,
	VersionName_LG,
	VersionName_BB,
	VersionName_HG,
	VersionName_SS,
	VersionName_CDS,
	VersionName_Diamond,
	VersionName_Pearl,
	VersionName_Platinum,
	VersionName_Invalid,
	VersionName_Invalid,
	VersionName_GC,
	VersionName_Invalid,
	VersionName_Invalid,
	VersionName_Invalid,
	VersionName_Invalid,
	VersionName_White,
	VersionName_Black,
	VersionName_White2,
	VersionName_Black2,
	VersionName_X,
	VersionName_Y,
	VersionName_AS,
	VersionName_OR,
	VersionName_TE,
	VersionName_Z,
	VersionName_Sun,
	VersionName_Moon,
	VersionName_US,
	VersionName_UM,
	VersionName_Go,
	VersionName_Red,
	VersionName_Green,
	VersionName_Blue,
	VersionName_Yellow,
	VersionName_Gold,
	VersionName_Silver,
	VersionName_Crystal,
	VersionName_LGP,
	VersionName_LGE,
	VersionName_Sword,
	VersionName_Shield,
	VersionName_SwShThird
};

const u8* const LanguageNameTable[NUM_LANGUAGES+1] = {
	LanguageName_None,
	LanguageName_Jap,
	LanguageName_Eng,
	LanguageName_Fra,
	LanguageName_Ita,
	LanguageName_Ger,
	LanguageName_Ntl,
	LanguageName_Spa,
	LanguageName_Kor,
	LanguageName_Chn,
	LanguageName_Tia
};
#endif
