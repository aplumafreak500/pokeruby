#include "global.h"
#include "menu.h"
#include "main.h"
#include "script.h"
#include "sound.h"
#include "constants/songs.h"

u8 debug_sub_new0(void);
u8 LumaDebugMenu_AddNewPKMN(void);
void DS_Adjust_Task(u8 taskId);

extern u8 (*gMenuCallback)(void);
extern u8 DebugScript_New0;

const u8 Str_AddNewPKMN[] = _("Add Victini to party");

const struct MenuAction LumaDebugMenuItems[] =
{
	{ Str_AddNewPKMN, LumaDebugMenu_AddNewPKMN },
};

#include "../data/git.h"

bool8 InitLumaDebugMenu(void)
{
    Menu_EraseScreen();
    Menu_DrawStdWindowFrame(0, 0, 16, 3);
    Menu_PrintItems(2, 1, 1, LumaDebugMenuItems);
    InitMenu(0, 1, 1, 1, 0, 15);
    gMenuCallback = debug_sub_new0;
    return 0;
}

u8 debug_sub_new0(void)
{
    s8 input = Menu_ProcessInput();

    if (input == -2)
    {
        return 0;
    }
    else if (input == -1)
    {
        CloseMenu();
        return 1;
    }
    else
    {
        gMenuCallback = LumaDebugMenuItems[input].func;
        return 0;
    }
}

u8 LumaDebugMenu_AddNewPKMN(void)
{
    ScriptContext1_SetupScript(&DebugScript_New0);
    PlaySE(SE_EXPMAX);
    CloseMenu();
    return 1;
}

// Sound test

void DS_Adjust_Num(u8 taskId)
{
    u8 dsStr[] = _("DS #");

    REG_DISPCNT = 0x3140;
    Menu_DrawStdWindowFrame(0, 0, 29, 19);
    Menu_PrintText(dsStr, 3, 2);
    REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
    REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    gTasks[taskId].func = sub_80BB038;
}

void DS_Adjust_Task(u8 taskId)
{
    u8 DS_Sound_Count=5;
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

