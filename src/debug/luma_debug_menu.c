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
u8 LumaDebugMenu_AddNewPKMN();
void DS_Adjust_Task(u8 taskId);
static u8 LumaDebugMenu_ProcessInput();
u8 LumaDebugMenu_OpenIwramEditor();
u8 LumaDebugMenu_OpenEwramEditor();
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

extern u8 (*gMenuCallback)();
// extern u8 DebugScript_New0;

const u8 Str_AddNewPKMN[] = _("Add Victini to party");
const u8 Str_MemoryEditor1[] = _("Edit IWRAM");
const u8 Str_MemoryEditor2[] = _("Edit EWRAM");
const u8 Str_FlagEdit[] = _("Change Flag");
const u8 Str_VarEdit[] = _("Change State");
UNUSED const u8 Str_SaveGame[] = _("SaveSerializedGame");

const struct MenuAction LumaDebugMenuItems[] = {
	{ Str_AddNewPKMN, LumaDebugMenu_AddNewPKMN },
	{ Str_MemoryEditor1, LumaDebugMenu_OpenIwramEditor },
	{ Str_MemoryEditor2, LumaDebugMenu_OpenEwramEditor },
	{ Str_FlagEdit, LumaDebugMenu_OpenFlagEditor },
	{ Str_VarEdit, LumaDebugMenu_OpenVarEditor },
};

#include "../data/git.h"

bool8 InitLumaDebugMenu() {
    Menu_EraseScreen();
    Menu_DrawStdWindowFrame(0, 0, 20, 3);
    Menu_PrintItems(1, 1, 5, LumaDebugMenuItems);
    InitMenu(0, 1, 1, 5, 0, 19);
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

u8 LumaDebugMenu_AddNewPKMN() {
    // ScriptContext1_SetupScript(&DebugScript_New0);
    // PlaySE(SE_EXPMAX);
    CloseMenu();
    return 1;
}

u8 LumaDebugMenu_OpenIwramEditor() {
    // TODO
    CloseMenu();
    return 1;
}

u8 LumaDebugMenu_OpenEwramEditor() {
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
    // TODO
    CloseMenu();
    return 1;
}

IWRAM_DATA u16 currentFlag;

void LumaDebugMenu_InitFlagEditor() {
	u8 taskId = CreateTask(LumaDebugMenu_FlagEditorTask, 80);
	Menu_EraseScreen();
	if (!currentFlag) currentFlag = 0x400;
	gTasks[taskId].data[1] = currentFlag;
}

const u8 Str_ChangeFlagState[] = _("Edit flag state     LR: Edit");

void LumaDebugMenu_FlagEditorTask(u8 taskId) {
	struct Task *task = gTasks + taskId;
	Menu_DisplayDialogueFrame();
	Menu_PrintText(Str_ChangeFlagState, 2, 15);
	LumaDebugMenu_DrawFlagStatus(task->data[1]);
	task->func = LumaDebugMenu_FlagEditorHandleInput;
}

void LumaDebugMenu_DrawFlagStatus(u16 flag) {
	u8 flag_state;
	Menu_BlankWindowRect(4, 17, 22, 18);
	ConvertIntToHexStringN(gStringVar1, flag, STR_CONV_MODE_RIGHT_ALIGN, 3);
	Menu_PrintText(gStringVar1, 4, 17);
	flag_state = FlagGet(flag);
	ConvertIntToDecimalStringN(gStringVar1, flag_state, STR_CONV_MODE_RIGHT_ALIGN, 3);
	Menu_PrintText(gStringVar1, 18, 17);
}

void LumaDebugMenu_FlagEditorHandleInput(u8 taskId) {
	struct Task *task = gTasks + taskId;
	if (gMain.newKeys & (A_BUTTON | B_BUTTON)) {
		task->func = LumaDebugMenu_ExitFlagEditor;
	}
	else if (gMain.newKeys & DPAD_LEFT) {
		task->data[1]--;
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	else if (gMain.newKeys & DPAD_RIGHT) {
		task->data[1]++;
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	else if (gMain.newKeys & DPAD_UP) {
		task->data[1]+=16;
		task->func = LumaDebugMenu_FlagEditorTask;
	}
	else if (gMain.newKeys & DPAD_DOWN) {
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
}

void LumaDebugMenu_ExitFlagEditor(u8 taskId) {
    Menu_EraseScreen();
    ScriptContext2_Disable();
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

// Sound test

const u8 dsStr[] = _("DS Num");

IWRAM_DATA s8 DS_Sound_Count;

void DS_Adjust_Num(u8 taskId) {

    REG_DISPCNT = 0x3140;
    Menu_DrawStdWindowFrame(0, 0, 29, 19);
    Menu_PrintText(dsStr, 3, 2);
    REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
    REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    gTasks[taskId].func = DS_Adjust_Task;
}

void DS_Adjust_Task(u8 taskId)
{
    if (gMain.newKeys & B_BUTTON)
    {
        REG_DISPCNT = 0x7140;
        REG_WIN0H = WIN_RANGE(17, 223);
        REG_WIN0V = WIN_RANGE(1, 31);
        Menu_EraseWindowRect(0, 0, 29, 19);
        gTasks[taskId].func = Task_InitSoundCheckMenu;
        return;
    }
    if (gMain.newAndRepeatedKeys & DPAD_RIGHT)
    {
        DS_Sound_Count++;
        if (DS_Sound_Count > 10)
            DS_Sound_Count = 0;
        m4aSoundMode(DS_Sound_Count << SOUND_MODE_MAXCHN_SHIFT);
    }
    else if (gMain.newAndRepeatedKeys & DPAD_LEFT)
    {
        DS_Sound_Count--;
        if (DS_Sound_Count < 0)
           DS_Sound_Count = 10;
        m4aSoundMode(DS_Sound_Count << SOUND_MODE_MAXCHN_SHIFT);
    }
    PrintSignedNumber(DS_Sound_Count, 7, 2, 3);
}

