#include "global.h"
#include "menu.h"
#include "main.h"
#include "script.h"
#include "sound.h"
#include "constants/songs.h"

// void InitLumaDebugMenu(void);
u8 debug_sub_new0(void);
u8 LumaDebugMenu_AddNewPKMN(void);

extern u8 (*gMenuCallback)(void);
extern u8 DebugScript_New0;

const u8 Str_AddNewPKMN[] = _("Add Victini to party");
#ifdef VERSION_NUMBER
const u8 GitBuildString[] = _("Git Rev: " VERSION_NUMBER);
#else
const u8 GitBuildString[] = _("$");
#endif

const struct MenuAction LumaDebugMenuItems[] =
{
	{ Str_AddNewPKMN, LumaDebugMenu_AddNewPKMN },
};

u8 InitLumaDebugMenu(void)
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

