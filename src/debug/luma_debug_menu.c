#include "global.h"
#include "menu.h"
#include "main.h"
#include "script.h"
#include "sound.h"
#include "constants/songs.h"
#include "debug.h"
#include "gba/m4a_internal.h"
#include "event_data.h"
#include "string_util.h"

u8 debug_sub_new0();
UNUSED u8 LumaDebugMenu_AddNewPKMN();
void DS_Adjust_Task(u8 taskId);
static u8 LumaDebugMenu_ProcessInput();
u8 LumaDebugMenu_CloseMenu();
u8 LumaDebugMenu_OpenMemoryEditorMenu();
u8 LumaDebugMenu_OpenMemoryEditor(u32);
static u8 MemoryEditorMenu_ProcessInput();
void MemoryEditor_Bios();
void MemoryEditor_Ewram();
void MemoryEditor_Iwram();
void MemoryEditor_IO();
void MemoryEditor_Vram();
void MemoryEditor_Pal();
void MemoryEditor_Oam();
void MemoryEditor_Rom();
void MemoryEditor_Fram();
UNUSED void MemoryEditor_Sram();
UNUSED void MemoryEditor_Eeprom();
u8 LumaDebugMenu_OpenFlagEditor();
u8 LumaDebugMenu_OpenVarEditor();
void LumaDebugMenu_InitFlagEditor();
void LumaDebugMenu_FlagEditorTask(u8);
void LumaDebugMenu_DrawFlagStatus(u16);
void LumaDebugMenu_FlagEditorHandleInput(u8);
void LumaDebugMenu_ExitFlagEditor(u8);
void LumaDebugMenu_BackToMainMenu();
void LumaDebugMenu_MenuCB(u8);
u8 LumaDebugMenu_ReloadMainMenu();
void LumaDebugMenu_InitVarEditor();
void LumaDebugMenu_VarEditorTask(u8);
void LumaDebugMenu_DrawVarStatus(u16);
void LumaDebugMenu_VarEditorHandleInput(u8);
void LumaDebugMenu_ExitVarEditor(u8);
u8 LumaDebugMenu_FixBadEggs();
// u8 LumaDebugMenu_SaveSerializedGame();

extern u8 (*gMenuCallback)();
extern u8 DebugScript_New0;
extern u16 *gSpecialVars[];

const u8 Str_AddNewPKMN[] = _("Add Victini to party");
const u8 Str_FlagEdit[] = _("Change Flag");
const u8 Str_VarEdit[] = _("Change State");
const u8 Str_SaveGame[] = _("SaveSerializedGame");
const u8 Str_FixBadEggs[] = _("Fix Bad EGG");
const u8 Str_MemoryEditor[] = _("View/edit memory region");
const u8 Str_MemoryEditorBios[] = _("Bios");
const u8 Str_MemoryEditorIO[] = _("AGB I/O");
const u8 Str_MemoryEditorVram[] = _("VRAM");
const u8 Str_MemoryEditorIwram[] = _("IWRAM");
const u8 Str_MemoryEditorEwram[] = _("EWRAM");
const u8 Str_MemoryEditorPal[] = _("PRAM");
const u8 Str_MemoryEditorOam[] = _("ORAM");
const u8 Str_MemoryEditorRom[] = _("ROM");
const u8 Str_MemoryEditorSram[] = _("SRAM");
const u8 Str_MemoryEditorEeprom[] = _("EEPROM");
const u8 Str_MemoryEditorFram[] = _("Flash RAM");
const u8 Str_MemoryEditorBank[] = _("Bank no. $");
const u8 Str_MemoryEditorOther[] = _("$");

const u8 Str_ChangeFlagState[] = _("Edit flag state");
const u8 Str_ChangeVarState[] = _("Edit var state");
const u8 Str_ChangeFlagState2[] = _("LR: Edit");
const u8 dsStr[] = _("DS Num");

#include "../data/git.h"

IWRAM_DATA s8 DS_Sound_Count;
IWRAM_DATA u16 currentFlag;
IWRAM_DATA u16 currentVar;

const u8* const RamPointerStrings[] = {
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

const struct MenuAction LumaDebugMenuItems[] = {
	{ Str_AddNewPKMN, LumaDebugMenu_CloseMenu },
	{ Str_MemoryEditor, LumaDebugMenu_OpenMemoryEditorMenu },
	{ Str_SaveGame, /* LumaDebugMenu_SaveSerializedGame*/ LumaDebugMenu_CloseMenu },
	{ Str_FlagEdit, LumaDebugMenu_OpenFlagEditor },
	{ Str_VarEdit, LumaDebugMenu_OpenVarEditor },
	{ Str_FixBadEggs, LumaDebugMenu_FixBadEggs }
};

const struct MenuAction MemoryEditorItems[] = {
	{ Str_MemoryEditorBios, MemoryEditor_Bios },
	{ Str_MemoryEditorEwram, MemoryEditor_Ewram },
	{ Str_MemoryEditorIwram, MemoryEditor_Iwram },
	{ Str_MemoryEditorIO, MemoryEditor_IO },
	{ Str_MemoryEditorVram, MemoryEditor_Vram },
	{ Str_MemoryEditorPal, MemoryEditor_Pal },
	{ Str_MemoryEditorOam, MemoryEditor_Oam },
	{ Str_MemoryEditorRom, MemoryEditor_Rom },
	{ Str_MemoryEditorFram, MemoryEditor_Fram },
};

bool8 InitLumaDebugMenu() {
	Menu_EraseScreen();
	Menu_DrawStdWindowFrame(0, 0, 16, 13);
	Menu_PrintItems(1, 1, 6, LumaDebugMenuItems);
	InitMenu(0, 1, 1, 6, 0, 15);
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
	PlaySE(SE_EXPMAX);
	CloseMenu();
	return 1;
}

u8 LumaDebugMenu_CloseMenu() {
	CloseMenu();
	return 1;
}

u8 LumaDebugMenu_OpenMemoryEditorMenu() {
	Menu_EraseScreen();
	Menu_DrawStdWindowFrame(0, 0, 10, 20);
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

void MemoryEditor_Bios() {
	LumaDebugMenu_OpenMemoryEditor(0x0000000);
}

void MemoryEditor_Ewram() {
	LumaDebugMenu_OpenMemoryEditor(0x2000000);
}

void MemoryEditor_Iwram() {
	LumaDebugMenu_OpenMemoryEditor(0x3000000);
}

void MemoryEditor_IO() {
	LumaDebugMenu_OpenMemoryEditor(0x4000000);
}

void MemoryEditor_Vram() {
	LumaDebugMenu_OpenMemoryEditor(0x5000000); // TODO: verify this
}

void MemoryEditor_Pal() {
	LumaDebugMenu_OpenMemoryEditor(0x6000000); // TODO: verify this
}

void MemoryEditor_Oam() {
	LumaDebugMenu_OpenMemoryEditor(0x7000000); // TODO: verify this
}

void MemoryEditor_Rom() {
	LumaDebugMenu_OpenMemoryEditor(0x8000000);
}

void MemoryEditor_Fram() {
	LumaDebugMenu_OpenMemoryEditor(0xe000000);
}

UNUSED void MemoryEditor_Sram() {
	LumaDebugMenu_OpenMemoryEditor(0xe000000);
}

UNUSED void MemoryEditor_Eeprom() {
	LumaDebugMenu_OpenMemoryEditor(0xd000000);
}

u8 LumaDebugMenu_OpenMemoryEditor(u32 address) {
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

void LumaDebugMenu_InitFlagEditor() {
	u8 taskId = CreateTask(LumaDebugMenu_FlagEditorTask, 80);
	Menu_EraseScreen();
	if (!currentFlag) currentFlag = 0x20;
	gTasks[taskId].data[1] = currentFlag;
}

void LumaDebugMenu_FlagEditorTask(u8 taskId) {
	struct Task *task = gTasks + taskId;
	Menu_DisplayDialogueFrame();
	Menu_PrintText(Str_ChangeFlagState, 2, 15);
	Menu_PrintText(Str_ChangeFlagState2, 15, 15);
	LumaDebugMenu_DrawFlagStatus(task->data[1]);
	task->func = LumaDebugMenu_FlagEditorHandleInput;
}

void LumaDebugMenu_DrawFlagStatus(u16 flag) {
	u8 flag_state;
	Menu_BlankWindowRect(4, 17, 22, 18);
	ConvertIntToHexStringN(gStringVar1, flag, STR_CONV_MODE_RIGHT_ALIGN, 4);
	Menu_PrintText(gStringVar1, 4, 17);
	flag_state = FlagGet(flag);
	ConvertIntToDecimalStringN(gStringVar1, flag_state, STR_CONV_MODE_RIGHT_ALIGN, 1);
	Menu_PrintText(gStringVar1, 18, 17);
}

void LumaDebugMenu_FlagEditorHandleInput(u8 taskId) {
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

void LumaDebugMenu_ExitFlagEditor(u8 taskId) {
	struct Task *task = gTasks + taskId;
	Menu_EraseScreen();
	ScriptContext2_Disable();
	currentFlag = task->data[1];
	DestroyTask(taskId);
	LumaDebugMenu_BackToMainMenu();
}

void LumaDebugMenu_BackToMainMenu() {
	gMenuCallback = LumaDebugMenu_ReloadMainMenu;
	ScriptContext2_Enable();
	CreateTask(LumaDebugMenu_MenuCB, 80);
}

void LumaDebugMenu_MenuCB(u8 taskId) {
	if (gMenuCallback() == TRUE)
		DestroyTask(taskId);
}

u8 LumaDebugMenu_ReloadMainMenu() {
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

void LumaDebugMenu_InitVarEditor() {
	u8 taskId = CreateTask(LumaDebugMenu_VarEditorTask, 80);
	Menu_EraseScreen();
	if (!currentVar) currentVar = 0x4000;
	gTasks[taskId].data[1] = currentVar;
}

void LumaDebugMenu_VarEditorTask(u8 taskId) {
	struct Task *task = gTasks + taskId;
	Menu_DisplayDialogueFrame();
	Menu_PrintText(Str_ChangeVarState, 2, 15);
	Menu_PrintText(Str_ChangeFlagState2, 15, 15);
	LumaDebugMenu_DrawVarStatus(task->data[1]);
	task->func = LumaDebugMenu_VarEditorHandleInput;
}

void LumaDebugMenu_DrawVarStatus(u16 var) {
	u16 var_state;
	Menu_BlankWindowRect(4, 17, 22, 18);
	ConvertIntToHexStringN(gStringVar1, var, STR_CONV_MODE_RIGHT_ALIGN, 4);
	Menu_PrintText(gStringVar1, 4, 17);
	var_state = DebugVarGet(var);
	ConvertIntToHexStringN(gStringVar1, var_state, STR_CONV_MODE_RIGHT_ALIGN, 4);
	Menu_PrintText(gStringVar1, 17, 17);
}

void LumaDebugMenu_VarEditorHandleInput(u8 taskId) {
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

void LumaDebugMenu_ExitVarEditor(u8 taskId) {
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
			PlaySE(SE_C_GAJI);
		}
	}
	CloseMenu();
	return 1;
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

void DS_Adjust_Task(u8 taskId) {
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

