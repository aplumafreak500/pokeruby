#include "global.h"
#include "sprite.h"
#include "palette.h"
#include "task.h"
#include "m4a.h"
#include "main.h"
#include "text.h"
#include "menu.h"
#include "constants/songs.h"
#include "title_screen.h"
#include "sound.h"
#include "pokedex_cry_screen.h"
#include "debug.h"

// local task defines
#define tWindowSelected data[0]
#define tBgmIndex data[1]
#define tSeIndex data[2]

// window selections
enum
{
    MUS_WINDOW,
    SE_WINDOW
};

// driver test cry enums
enum
{
    CRY_TEST_VOICE,
    CRY_TEST_VOLUME,
    CRY_TEST_PANPOT,
    CRY_TEST_PITCH,
    CRY_TEST_LENGTH,
    CRY_TEST_RELEASE,
    CRY_TEST_PROGRESS,
    CRY_TEST_CHORUS,
    CRY_TEST_PRIORITY
};

// minmax range enums
enum
{
    MIN,
    MAX
};

extern struct ToneData gCryTable[];
extern struct ToneData gCryTable2[];

static EWRAM_DATA u8 gUnknown_020387B0 = 0;
static EWRAM_DATA u8 gUnknown_020387B1 = 0;
static EWRAM_DATA u8 gUnknown_020387B2 = 0;
static EWRAM_DATA s8 sDriverTestSelection = 0;
static EWRAM_DATA int sSoundTestParams[9] = {0};
static EWRAM_DATA u8 gUnknown_020387D8 = 0;
static EWRAM_DATA u8 gUnknown_020387D9 = 0;

u16 gSoundTestCryNum;
extern u8 gUnknown_03005E98;
extern s8 DS_Sound_Count;

struct MusicPlayerInfo *gUnknown_03005D30;

extern struct MusicPlayerInfo gMPlayInfo_BGM;

void Task_InitSoundCheckMenu(u8);
void sub_80BA384(u8);
void sub_80BA65C(u8);
void sub_80BA68C(u8);
void HighlightSelectedWindow(u8);
void PrintSoundNumber(u16, u16, u16);
void sub_80BA79C(const u8 *const, u16, u16);
void Task_DrawDriverTestMenu(u8);
void Task_ProcessDriverTestInput(u8);
void AdjustSelectedDriverParam(s8);
void PrintDriverTestMenuText(void);
void sub_80BAE10(u8, u8);
void PrintSignedNumber(int, u16, u16, u8);
void sub_80BAF84(u8);
void sub_80BB038(u8);
void sub_80BB1D4(void);
void Task_InitCryTest(u8);
void Task_ProcessCryTestInput(u8);
void PrintCryNumber(void);

void CB2_SoundCheckMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void VBlankCB_SoundCheckMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();

    if (gUnknown_020387B0 != 0)
    {
        m4aSoundMain();
        m4aSoundMain();
        m4aSoundMain();
    }
}

// unused
void CB2_StartSoundCheckMenu(void)
{
    u8 taskId;

    SetVBlankCallback(NULL);
    REG_DISPCNT = 0;
    REG_BG2CNT = 0;
    REG_BG1CNT = 0;
    REG_BG0CNT = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    ResetPaletteFade();
    ResetTasks();
    ResetSpriteData();
    Text_LoadWindowTemplate(&gWindowTemplate_81E6C3C);
    InitMenuWindow(&gMenuTextWindowTemplate);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
    REG_WIN0H = WIN_RANGE(0, 0);
    REG_WIN0V = WIN_RANGE(0, 0);
    REG_WIN1H = WIN_RANGE(0, 0);
    REG_WIN1V = WIN_RANGE(0, 0);
    REG_WININ = 0x1111;
    REG_WINOUT = 0x31;
    REG_BLDCNT = 0xE1;
    REG_BLDALPHA = 0;
    REG_BLDY = 7;
    REG_IE = 1; // could be a typo of REG_IME
    REG_IE |= 1;
    REG_DISPSTAT |= 8;
    SetVBlankCallback(VBlankCB_SoundCheckMenu);
    SetMainCallback2(CB2_SoundCheckMenu);
    REG_DISPCNT = 0x7140;
    taskId = CreateTask(Task_InitSoundCheckMenu, 0);
    gTasks[taskId].tWindowSelected = MUS_WINDOW;
    gTasks[taskId].tBgmIndex = 0;
    gTasks[taskId].tSeIndex = 0;
    gTasks[taskId].data[3] = 0;
    gUnknown_020387B0 = 0;
    gTasks[taskId].data[3] = 0; // why?
    DS_Sound_Count=5;
    m4aSoundInit();
}

void Task_InitSoundCheckMenu(u8 taskId)
{
    u8 soundcheckStr[] = _("Sound Test");
    u8 bgmStr[] = _("BGM");
    u8 seStr[] = _("SE ");
    u8 abDescStr[] = _("A: Play　B: Exit");
    u8 upDownStr[] = _("{LEFT_ARROW}{RIGHT_ARROW}: Edit");
    u8 driverStr[] = _("R: Cry Test");

    if (!gPaletteFade.active)
    {
        Menu_DrawStdWindowFrame(2, 0, 27, 3);
        Menu_DrawStdWindowFrame(2, 5, 27, 10);
        Menu_DrawStdWindowFrame(2, 12, 27, 17);
        Menu_PrintText(soundcheckStr, 4, 1);
        Menu_PrintText(abDescStr, 14, 1);
        Menu_PrintText(bgmStr, 4, 6);
        Menu_PrintText(upDownStr, 14, 6);
        Menu_PrintText(seStr, 4, 13);
        Menu_PrintText(upDownStr, 14, 13);
        Menu_PrintText(driverStr, 14, 18);
        gTasks[taskId].func = sub_80BA384;
        REG_WIN0H = WIN_RANGE(17, 223);
        REG_WIN0V = WIN_RANGE(1, 31);
    }
}

// ideally this should be a multi Coords8 struct, but it wont match when its treated like a struct.
static const u8 gUnknown_083D0300[] = { 1, 1, 1, 3, 1, 5, 1, 7, 1, 9, 1, 11, 1, 13, 1, 15, 1, 17 };

extern const u8 *const gBGMNames[];
extern const u8 *const gSENames[];

void sub_80BA384(u8 taskId) // Task_HandleDrawingSoundCheckMenuText
{
    HighlightSelectedWindow(gTasks[taskId].tWindowSelected);
    PrintSoundNumber(gTasks[taskId].tBgmIndex + MUS_STOP, 7, 8); // print by BGM index
    sub_80BA79C(gBGMNames[gTasks[taskId].tBgmIndex], 11, 8);
    PrintSoundNumber(gTasks[taskId].tSeIndex, 7, 15);
    sub_80BA79C(gSENames[gTasks[taskId].tSeIndex], 11, 15);
    gTasks[taskId].func = sub_80BA65C;
}

bool8 Task_ProcessSoundCheckMenuInput(u8 taskId)
{
    if (gMain.newKeys & R_BUTTON) // driver test
    {
        gTasks[taskId].func = Task_DrawDriverTestMenu;
    }
    else if (gMain.newKeys & L_BUTTON)
    {
        // gTasks[taskId].func = sub_80BAF84;
        gTasks[taskId].func = DS_Adjust_Num;
    }
    else if (gMain.newKeys & START_BUTTON)
    {
        gTasks[taskId].func = Task_InitCryTest;
    }
    else if (gMain.newKeys & A_BUTTON)
    {
        if (gTasks[taskId].tWindowSelected != 0) // is playing?
        {
            if (gTasks[taskId].data[4] != 0)
            {
                if (gTasks[taskId].tSeIndex != 0)
                {
                    m4aSongNumStop(gTasks[taskId].data[4]);
                    m4aSongNumStart(gTasks[taskId].tSeIndex);
                    gTasks[taskId].data[4] = gTasks[taskId].tSeIndex;
                }
                else
                {
                    m4aSongNumStop(gTasks[taskId].data[4]);
                    gTasks[taskId].data[4] = 0;
                }
            }
            else if (gTasks[taskId].tSeIndex != 0)
            {
                m4aSongNumStart(gTasks[taskId].tSeIndex);
                gTasks[taskId].data[4] = gTasks[taskId].tSeIndex;
            }
        }
        else
        {
            if (gTasks[taskId].data[3] != 0)
            {
                if (gTasks[taskId].tBgmIndex != 0)
                {
                    m4aSongNumStop(gTasks[taskId].data[3] + MUS_STOP);
                    m4aSongNumStart(gTasks[taskId].tBgmIndex + MUS_STOP);
                    gTasks[taskId].data[3] = gTasks[taskId].tBgmIndex;
                }
                else
                {
                    m4aSongNumStop(gTasks[taskId].data[3] + MUS_STOP);
                    gTasks[taskId].data[3] = 0;
                }
            }
            else if (gTasks[taskId].tBgmIndex != 0)
            {
                m4aSongNumStart(gTasks[taskId].tBgmIndex + MUS_STOP);
                gTasks[taskId].data[3] = gTasks[taskId].tBgmIndex;
            }
        }
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        m4aSongNumStart(SE_SELECT);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
        m4aSoundMode(5 << SOUND_MODE_MAXCHN_SHIFT);
        gTasks[taskId].func = sub_80BA68C;
    }
    else if (gMain.newAndRepeatedKeys & DPAD_UP)
    {
        gTasks[taskId].tWindowSelected ^= 1;
        return TRUE;
    }
    else if (gMain.newAndRepeatedKeys & DPAD_DOWN)
    {
        gTasks[taskId].tWindowSelected ^= 1;
        return TRUE;
    }
    else if (gMain.newAndRepeatedKeys & DPAD_RIGHT)
    {
        if (gTasks[taskId].tWindowSelected != 0)
        {
            if (gTasks[taskId].tSeIndex > 0)
                gTasks[taskId].tSeIndex--;
            else
                gTasks[taskId].tSeIndex = 269;
        }
        else
        {
            if (gTasks[taskId].tBgmIndex > 0)
                gTasks[taskId].tBgmIndex--;
            else
                gTasks[taskId].tBgmIndex = 211;
        }
        return TRUE;
    }
    else if (gMain.newAndRepeatedKeys & DPAD_LEFT)
    {
        if (gTasks[taskId].tWindowSelected != 0)
        {
            if (gTasks[taskId].tSeIndex < 269)
                gTasks[taskId].tSeIndex++;
            else
                gTasks[taskId].tSeIndex = 0;
        }
        else
        {
            if (gTasks[taskId].tBgmIndex < 211)
                gTasks[taskId].tBgmIndex++;
            else
                gTasks[taskId].tBgmIndex = 0;
        }
        return TRUE;
    }
    else if (gMain.heldKeys & SELECT_BUTTON)
    {
        gUnknown_020387B0 = 1;
    }
    else
    {
        gUnknown_020387B0 = 0;
    }
    return FALSE;
}

void sub_80BA65C(u8 taskId)
{
    if (Task_ProcessSoundCheckMenuInput(taskId) != FALSE)
        gTasks[taskId].func = sub_80BA384;
}

void sub_80BA68C(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        SetMainCallback2(CB2_InitTitleScreen);
    }
}

void HighlightSelectedWindow(u8 windowType)
{
    switch (windowType)
    {
    case MUS_WINDOW:
    default:
        REG_WIN1H = WIN_RANGE(17, 223);
        REG_WIN1V = WIN_RANGE(41, 87);
        break;
    case SE_WINDOW:
        REG_WIN1H = WIN_RANGE(17, 223);
        REG_WIN1V = WIN_RANGE(97, 143);
        break;
    }
}

void PrintSoundNumber(u16 soundIndex, u16 x, u16 y) // PrintSoundNumber ?
{
    u8 i;
    u8 str[5];
    bool8 someBool;
    u8 divisorValue;

    for (i = 0; i < 3; i++)
        str[i] = 0; // initialize array

    str[3] = CHAR_ELLIPSIS;
    str[4] = EOS;
    someBool = FALSE;

    divisorValue = soundIndex / 100;
    if (divisorValue)
    {
        str[0] = divisorValue + CHAR_0;
        someBool = TRUE;
    }

    divisorValue = (soundIndex % 100) / 10;
    if (divisorValue || someBool)
        str[1] = divisorValue + CHAR_0;

    str[2] = ((soundIndex % 100) % 10) + CHAR_0;
    Menu_PrintText(str, x, y);
}

void sub_80BA79C(const u8 *const string, u16 x, u16 y)
{
    u8 i;
    u8 str[11];

    for (i = 0; i <= 10; i++)
        str[i] = 0; // format string.

    str[10] = EOS; // the above for loop formats the last element of the array unnecessarily.

    for (i = 0; string[i] != EOS && i < 10; i++)
        str[i] = string[i];

    Menu_PrintText(str, x, y);
}

void Task_DrawDriverTestMenu(u8 taskId) // Task_DrawDriverTestMenu
{
    u8 bbackStr[] = _("B: Back");
    u8 aplayStr[] = _("A: Play");
    u8 voiceStr[] = _("Voice");
    u8 volumeStr[] = _("Volume");
    u8 panpotStr[] = _("Pan");
    u8 pitchStr[] = _("Pitch");
    u8 lengthStr[] = _("Length");
    u8 releaseStr[] = _("Release");
    u8 progressStr[] = _("Progress");
    u8 chorusStr[] = _("Chorus");
    u8 priorityStr[] = _("Priority");
    u8 playingStr[] = _("Playing");
    u8 reverseStr[] = _("Reverse");
    u8 stereoStr[] = _("Stereo");

    REG_DISPCNT = 0x3140;
    Menu_DrawStdWindowFrame(0, 0, 29, 19);
    Menu_PrintText(bbackStr, 19, 4);
    Menu_PrintText(aplayStr, 19, 2);
    Menu_PrintText(voiceStr, 2, 1);
    Menu_PrintText(volumeStr, 2, 3);
    Menu_PrintText(panpotStr, 2, 5);
    Menu_PrintText(pitchStr, 2, 7);
    Menu_PrintText(lengthStr, 2, 9);
    Menu_PrintText(releaseStr, 2, 11);
    Menu_PrintText(progressStr, 2, 13);
    Menu_PrintText(chorusStr, 2, 15);
    Menu_PrintText(priorityStr, 2, 17);
    Menu_PrintText(playingStr, 19, 16);
    Menu_PrintText(reverseStr, 19, 14);
    Menu_PrintText(stereoStr, 19, 12);
    REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
    REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    sDriverTestSelection = 0;
    gUnknown_020387B1 = 0;
    gUnknown_020387B2 = 0;
    gUnknown_03005D30 = NULL;
    gUnknown_020387D8 = 0;
    gUnknown_020387D9 = 1;
    sSoundTestParams[CRY_TEST_VOICE] = 0;
    sSoundTestParams[CRY_TEST_VOLUME] = 120;
    sSoundTestParams[CRY_TEST_PANPOT] = 0;
    sSoundTestParams[CRY_TEST_PITCH] = 15360;
    sSoundTestParams[CRY_TEST_LENGTH] = 180;
    sSoundTestParams[CRY_TEST_PROGRESS] = 0;
    sSoundTestParams[CRY_TEST_RELEASE] = 0;
    sSoundTestParams[CRY_TEST_CHORUS] = 0;
    sSoundTestParams[CRY_TEST_PRIORITY] = 2;
    PrintDriverTestMenuText();
    sub_80BAE10(0, 0);
    gTasks[taskId].func = Task_ProcessDriverTestInput;
}

void Task_ProcessDriverTestInput(u8 taskId)
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
    if (gMain.newAndRepeatedKeys & DPAD_UP) // _080BAAA8
    {
        u8 old = sDriverTestSelection;

        if(--sDriverTestSelection < 0)
            sDriverTestSelection = 8;
        sub_80BAE10(old, sDriverTestSelection);
        return;
    }
    if (gMain.newAndRepeatedKeys & DPAD_DOWN) // _080BAAD0
    {
        u8 old = sDriverTestSelection;

        if(++sDriverTestSelection > 8)
            sDriverTestSelection = 0;
        sub_80BAE10(old, sDriverTestSelection);
        return;
    }
    if (gMain.newKeys & START_BUTTON) // _080BAAF8
    {
        gUnknown_020387D8 ^= 1;
        PrintDriverTestMenuText();
        return;
    }
    if (gMain.newKeys & SELECT_BUTTON) // _080BAB14
    {
        gUnknown_020387D9 ^= 1;
        PrintDriverTestMenuText();
        SetPokemonCryStereo(gUnknown_020387D9);
        return;
    }
    if (gMain.newAndRepeatedKeys & R_BUTTON) // _080BAB38
    {
        AdjustSelectedDriverParam(10);
        PrintDriverTestMenuText();
        return;
    }
    if (gMain.newAndRepeatedKeys & L_BUTTON) // _080BAB46
    {
        AdjustSelectedDriverParam(-10);
        PrintDriverTestMenuText();
        return;
    }
    if (gMain.newAndRepeatedKeys & DPAD_LEFT) // _080BAB56
    {
        AdjustSelectedDriverParam(-1);
        PrintDriverTestMenuText();
        return;
    }
    if (gMain.newAndRepeatedKeys & DPAD_RIGHT) // _080BAB64
    {
        AdjustSelectedDriverParam(1);
        PrintDriverTestMenuText();
        return;
    }
    if (gMain.newKeys & A_BUTTON) // _080BAB78
    {
        u8 divide, remaining;

        SetPokemonCryVolume(sSoundTestParams[CRY_TEST_VOLUME]);
        SetPokemonCryPanpot(sSoundTestParams[CRY_TEST_PANPOT]);
        SetPokemonCryPitch(sSoundTestParams[CRY_TEST_PITCH]);
        SetPokemonCryLength(sSoundTestParams[CRY_TEST_LENGTH]);
        SetPokemonCryProgress(sSoundTestParams[CRY_TEST_PROGRESS]);
        SetPokemonCryRelease(sSoundTestParams[CRY_TEST_RELEASE]);
        SetPokemonCryChorus(sSoundTestParams[CRY_TEST_CHORUS]);
        SetPokemonCryPriority(sSoundTestParams[CRY_TEST_PRIORITY]);

        remaining = sSoundTestParams[CRY_TEST_VOICE] % 128;
        divide = sSoundTestParams[CRY_TEST_VOICE] / 128;

        switch (divide)
        {
        case 0:
            if (gUnknown_020387D8)
                gUnknown_03005D30 = SetPokemonCryTone(&gCryTable2[(128 * 0) + remaining]);
            else
                gUnknown_03005D30 = SetPokemonCryTone(&gCryTable[(128 * 0) + remaining]);
            break;
        case 1:
            if (gUnknown_020387D8)
                gUnknown_03005D30 = SetPokemonCryTone(&gCryTable2[(128 * 1) + remaining]);
            else
                gUnknown_03005D30 = SetPokemonCryTone(&gCryTable[(128 * 1) + remaining]);
            break;
        case 2:
            if (gUnknown_020387D8)
                gUnknown_03005D30 = SetPokemonCryTone(&gCryTable2[(128 * 2) + remaining]);
            else
                gUnknown_03005D30 = SetPokemonCryTone(&gCryTable[(128 * 2) + remaining]);
            break;
        case 3:
            if (gUnknown_020387D8)
                gUnknown_03005D30 = SetPokemonCryTone(&gCryTable2[(128 * 3) + remaining]);
            else
                gUnknown_03005D30 = SetPokemonCryTone(&gCryTable[(128 * 3) + remaining]);
            break;
        }
    }

    // _080BACA2
    if (gUnknown_03005D30 != NULL)
    {
        gUnknown_020387B1 = IsPokemonCryPlaying(gUnknown_03005D30);

        if (gUnknown_020387B1 != gUnknown_020387B2)
            PrintDriverTestMenuText();

        gUnknown_020387B2 = gUnknown_020387B1;
    }
}

void AdjustSelectedDriverParam(s8 delta)
{
    // also ideally should be a MinMax struct, but any attempt to make this into a struct causes it to not match due to the weird multi dim access.
    int paramRanges[] =
    {
        0, 388,         // Voice
        0, 127,         // Volume
        -64, 63,      // Panpot
        -128, 32639,    // Pitch
        0, 65535,       // Length
        0, 255,         // Release
        0, 65535,       // Progress
        -64, 63,         // Chorus
        -1, 15           // Priority
    };

    sSoundTestParams[sDriverTestSelection] += delta;

    if (sSoundTestParams[sDriverTestSelection] > paramRanges[MULTI_DIM_ARR(sDriverTestSelection, B_16, MAX)])
        sSoundTestParams[sDriverTestSelection] = paramRanges[MULTI_DIM_ARR(sDriverTestSelection, B_16, MIN)];

    if (sSoundTestParams[sDriverTestSelection] < paramRanges[MULTI_DIM_ARR(sDriverTestSelection, B_16, MIN)])
        sSoundTestParams[sDriverTestSelection] = paramRanges[MULTI_DIM_ARR(sDriverTestSelection, B_16, MAX)];
}

void PrintDriverTestMenuText(void)
{
    PrintSignedNumber(sSoundTestParams[CRY_TEST_VOICE] + 1, 11, 1, 5);
    PrintSignedNumber(sSoundTestParams[CRY_TEST_VOLUME], 11, 3, 5);
    PrintSignedNumber(sSoundTestParams[CRY_TEST_PANPOT], 11, 5, 5);
    PrintSignedNumber(sSoundTestParams[CRY_TEST_PITCH], 11, 7, 5);
    PrintSignedNumber(sSoundTestParams[CRY_TEST_LENGTH], 11, 9, 5);
    PrintSignedNumber(sSoundTestParams[CRY_TEST_RELEASE], 11, 11, 5);
    PrintSignedNumber(sSoundTestParams[CRY_TEST_PROGRESS], 11, 13, 5);
    PrintSignedNumber(sSoundTestParams[CRY_TEST_CHORUS], 11, 15, 5);
    PrintSignedNumber(sSoundTestParams[CRY_TEST_PRIORITY], 11, 17, 5);
    PrintSignedNumber(gUnknown_020387B1, 27, 16, 1);
    PrintSignedNumber(gUnknown_020387D8, 27, 14, 1);
    PrintSignedNumber(gUnknown_020387D9, 27, 12, 1);
}

void sub_80BAE10(u8 var1, u8 var2)
{
    u8 str1[] = _("▶");
    u8 str2[] = _(" ");

    Menu_PrintText(str2, gUnknown_083D0300[MULTI_DIM_ARR(var1, B_16, 0)], gUnknown_083D0300[MULTI_DIM_ARR(var1, B_16, 1)]);
    Menu_PrintText(str1, gUnknown_083D0300[MULTI_DIM_ARR(var2, B_16, 0)], gUnknown_083D0300[MULTI_DIM_ARR(var2, B_16, 1)]);
}

void PrintSignedNumber(int n, u16 x, u16 y, u8 digits)
{
    int powersOfTen[6] =
    {
              1,
             10,
            100,
           1000,
          10000,
         100000
    };
    u8 str[8];
    s8 i;
    s8 negative;
    s8 someVar2;

    for (i = 0; i <= digits; i++)
        str[i] = CHAR_SPACE;
    str[digits + 1] = EOS;

    negative = FALSE;
    if (n < 0)
    {
        n = -n;
        negative = TRUE;
    }

    if (digits == 1)
        someVar2 = TRUE;
    else
        someVar2 = FALSE;

    for (i = digits - 1; i >= 0; i--)
    {
        s8 d = n / powersOfTen[i];

        if (d != 0 || someVar2 || i == 0)
        {
            if (negative && !someVar2)
                str[digits - i - 1] = CHAR_HYPHEN;
            str[digits - i] = CHAR_0 + d;
            someVar2 = TRUE;
        }
        n %= powersOfTen[i];
    }

    Menu_PrintText(str, x, y);
}

static const s8 gUnknown_083D03F8[5] = { 0x3F, 0x00, 0xC0, 0x7F, 0x80 };

void sub_80BAF84(u8 taskId)
{
    u8 seStr[] = _("SE");
    u8 panStr[] = _("Pan");
    u8 playingStr[] = _("Playing");

    REG_DISPCNT = 0x3140;
    Menu_DrawStdWindowFrame(0, 0, 29, 19);
    Menu_PrintText(seStr, 3, 2);
    Menu_PrintText(panStr, 3, 4);
    Menu_PrintText(playingStr, 3, 8);
    REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
    REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    sSoundTestParams[CRY_TEST_VOICE] = 1;
    sSoundTestParams[CRY_TEST_PANPOT] = 0;
    sSoundTestParams[CRY_TEST_CHORUS] = 0;
    sSoundTestParams[CRY_TEST_PROGRESS] = 0;
    sSoundTestParams[CRY_TEST_RELEASE] = 0;
    sub_80BB1D4();
    gTasks[taskId].func = sub_80BB038;
}

void sub_80BB038(u8 taskId)
{
    sub_80BB1D4();
    if (sSoundTestParams[CRY_TEST_PROGRESS])
    {
        if (sSoundTestParams[CRY_TEST_RELEASE])
        {
            sSoundTestParams[CRY_TEST_RELEASE]--;
        }
        else // _080BB05C
        {
            s8 panpot = gUnknown_083D03F8[sSoundTestParams[CRY_TEST_PANPOT]];
            if (panpot != -128)
            {
                if (panpot == 127)
                {
                    sSoundTestParams[CRY_TEST_CHORUS] += 2;
                    if (sSoundTestParams[CRY_TEST_CHORUS] < 63)
                        SE12PanpotControl(sSoundTestParams[CRY_TEST_CHORUS]);
                }
            }
            else // _080BB08C
            {
                sSoundTestParams[CRY_TEST_CHORUS] -= 2;
                if (sSoundTestParams[CRY_TEST_CHORUS] > -64)
                    SE12PanpotControl(sSoundTestParams[CRY_TEST_CHORUS]);
            }
        }
    }
     // _080BB0A2
    if (gMain.newKeys & B_BUTTON)
    {
        REG_DISPCNT = 0x7140;
        REG_WIN0H = WIN_RANGE(17, 223);
        REG_WIN0V = WIN_RANGE(1, 31);
        Menu_EraseWindowRect(0, 0, 29, 19);
        gTasks[taskId].func = Task_InitSoundCheckMenu;
        return;
    }
    if (gMain.newKeys & A_BUTTON) // _080BB104
    {
        s8 panpot = gUnknown_083D03F8[sSoundTestParams[CRY_TEST_PANPOT]];
        if (panpot != -128)
        {
            if (panpot == 127)
            {
                PlaySE12WithPanning(sSoundTestParams[CRY_TEST_VOICE], -64);
                sSoundTestParams[CRY_TEST_CHORUS] = -64;
                sSoundTestParams[CRY_TEST_PROGRESS] = 1;
                sSoundTestParams[CRY_TEST_RELEASE] = 30;
                return;
            }
        }
        else // _080BB140
        {
            PlaySE12WithPanning(sSoundTestParams[CRY_TEST_VOICE], 63);
            sSoundTestParams[CRY_TEST_CHORUS] = 63;
            sSoundTestParams[CRY_TEST_PROGRESS] = 1;
            sSoundTestParams[CRY_TEST_RELEASE] = 30;
            return;
        }
        // _080BB154
        PlaySE12WithPanning(sSoundTestParams[CRY_TEST_VOICE], panpot);
        sSoundTestParams[CRY_TEST_PROGRESS] = 0;
        return;
    }
    if (gMain.newKeys & L_BUTTON) // _080BB15E
    {
        sSoundTestParams[CRY_TEST_PANPOT]++;
        if (sSoundTestParams[CRY_TEST_PANPOT] > 4)
            sSoundTestParams[CRY_TEST_PANPOT] = 0;
    }
    if (gMain.newKeys & R_BUTTON) // _080BB176
    {
        sSoundTestParams[CRY_TEST_PANPOT]--;
        if (sSoundTestParams[CRY_TEST_PANPOT] < 0)
            sSoundTestParams[CRY_TEST_PANPOT] = 4;
    }
    if (gMain.newAndRepeatedKeys & DPAD_RIGHT) // _080BB192
    {
        sSoundTestParams[CRY_TEST_VOICE]++;
        if (sSoundTestParams[CRY_TEST_VOICE] > 247)
            sSoundTestParams[CRY_TEST_VOICE] = 0;
    }
    else if (gMain.newAndRepeatedKeys & DPAD_LEFT) // _080BB1B0
    {
        sSoundTestParams[CRY_TEST_VOICE]--;
        if (sSoundTestParams[CRY_TEST_VOICE] < 0)
            sSoundTestParams[CRY_TEST_VOICE] = 247;
    }
}

void sub_80BB1D4(void)
{
    u8 lrStr[] = _("  LR");
    u8 rlStr[] = _("  RL");

    PrintSignedNumber(sSoundTestParams[CRY_TEST_VOICE], 7, 2, 3);

    switch (gUnknown_083D03F8[sSoundTestParams[CRY_TEST_PANPOT]])
    {
    case 127:
        Menu_PrintText(lrStr, 7, 4);
        break;
    case -128:
        Menu_PrintText(rlStr, 7, 4);
        break;
    default:
        PrintSignedNumber(gUnknown_083D03F8[sSoundTestParams[CRY_TEST_PANPOT]], 7, 4, 3);
        break;
    }
    PrintSignedNumber(IsSEPlaying(), 12, 8, 1);
}

#define SOUND_LIST_BGM \
	X(MUS_STOP, "STOP") \
	X(MUS_TETSUJI, "TETSUJI") \
	X(MUS_FIELD13, "FIELD13") \
	X(MUS_KACHI22, "KACHI22") \
	X(MUS_KACHI2, "KACHI2") \
	X(MUS_KACHI3, "KACHI3") \
	X(MUS_KACHI5, "KACHI5") \
	X(MUS_PCC, "PCC") \
	X(MUS_NIBI, "NIBI") \
	X(MUS_SUIKUN, "SUIKUN") \
	X(MUS_DOORO1, "DOORO1") \
	X(MUS_DOORO_X1, "DOORO-X1") \
	X(MUS_DOORO_X3, "DOORO-X3") \
	X(MUS_MACHI_S2, "MACHI-S2") \
	X(MUS_MACHI_S4, "MACHI-S4") \
	X(MUS_GIM, "GIM") \
	X(MUS_NAMINORI, "NAMINORI") \
	X(MUS_DAN01, "DAN01") \
	X(MUS_FANFA1, "FANFA1") \
	X(MUS_ME_ASA, "ME-ASA") \
	X(MUS_ME_BACHI, "ME-BACHI") \
	X(MUS_FANFA4, "FANFA4") \
	X(MUS_FANFA5, "FANFA5") \
	X(MUS_ME_WAZA, "ME-WAZA") \
	X(MUS_BIJYUTU, "BIJYUTU") \
	X(MUS_DOORO_X4, "DOORO-X4") \
	X(MUS_FUNE_KAN, "FUNE-KAN") \
	X(MUS_ME_SHINKA, "ME-SHINKA") \
	X(MUS_SHINKA, "SHINKA") \
	X(MUS_ME_WASURE, "ME-WASURE") \
	X(MUS_SYOUJOEYE, "SYOUJOEYE") \
	X(MUS_BOYEYE, "BOYEYE") \
	X(MUS_DAN02, "DAN02") \
	X(MUS_MACHI_S3, "MACHI-S3") \
	X(MUS_ODAMAKI, "ODAMAKI") \
	X(MUS_B_TOWER, "B-TOWER") \
	X(MUS_SWIMEYE, "SWIMEYE") \
	X(MUS_DAN03, "DAN03") \
	X(MUS_ME_KINOMI, "ME-KINOMI") \
	X(MUS_ME_TAMA, "ME-TAMA") \
	X(MUS_ME_B_BIG, "ME-B-BIG") \
	X(MUS_ME_B_SMALL, "ME-B-SMALL") \
	X(MUS_ME_ZANNEN, "ME-ZANNEN") \
	X(MUS_BD_TIME, "BD-TIME") \
	X(MUS_TEST1, "TEST1") \
	X(MUS_TEST2, "TEST2") \
	X(MUS_TEST3, "TEST3") \
	X(MUS_TEST4, "TEST4") \
	X(MUS_TEST, "TEST") \
	X(MUS_GOMACHI0, "GOMACHI0") \
	X(MUS_GOTOWN, "GOTOWN") \
	X(MUS_POKECEN, "POKECEN") \
	X(MUS_NEXTROAD, "NEXTROAD") \
	X(MUS_GRANROAD, "GRANROAD") \
	X(MUS_CYCLING, "CYCLING") \
	X(MUS_FRIENDLY, "FRIENDLY") \
	X(MUS_MISHIRO, "MISHIRO") \
	X(MUS_TOZAN, "TOZAN") \
	X(MUS_GIRLEYE, "GIRLEYE") \
	X(MUS_MINAMO, "MINAMO") \
	X(MUS_ASHROAD, "ASHROAD") \
	X(MUS_EVENT0, "EVENT0") \
	X(MUS_DEEPDEEP, "DEEPDEEP") \
	X(MUS_KACHI1, "KACHI1") \
	X(MUS_TITLE3, "TITLE3") \
	X(MUS_DEMO1, "DEMO1") \
	X(MUS_GIRL_SUP, "GIRL-SUP") \
	X(MUS_HAGESHII, "HAGESHII") \
	X(MUS_KAKKOII, "KAKKOII") \
	X(MUS_KAZANBAI, "KAZANBAI") \
	X(MUS_AQA_0, "AQA-0") \
	X(MUS_TSURETEK, "TSURETEK") \
	X(MUS_BOY_SUP, "BOY-SUP") \
	X(MUS_RAINBOW, "RAINBOW") \
	X(MUS_AYASII, "AYASII") \
	X(MUS_KACHI4, "KACHI4") \
	X(MUS_ROPEWAY, "ROPEWAY") \
	X(MUS_CASINO, "CASINO") \
	X(MUS_HIGHTOWN, "HIGHTOWN") \
	X(MUS_SAFARI, "SAFARI") \
	X(MUS_C_ROAD, "C-ROAD") \
	X(MUS_AJITO, "AJITO") \
	X(MUS_M_BOAT, "M-BOAT") \
	X(MUS_M_DUNGON, "M-DUNGON") \
	X(MUS_FINECITY, "FINECITY") \
	X(MUS_MACHUPI, "MACHUPI") \
	X(MUS_P_SCHOOL, "P-SCHOOL") \
	X(MUS_DENDOU, "DENDOU") \
	X(MUS_TONEKUSA, "TONEKUSA") \
	X(MUS_MABOROSI, "MABOROSI") \
	X(MUS_CON_FAN, "CON-FAN") \
	X(MUS_CONTEST0, "CONTEST0") \
	X(MUS_MGM0, "MGM0") \
	X(MUS_T_BATTLE, "T-BATTLE") \
	X(BGM_OOAME, "OOAME") \
	X(MUS_HIDERI, "HIDERI") \
	X(MUS_RUNECITY, "RUNECITY") \
	X(MUS_CON_K, "CON-K") \
	X(MUS_EIKOU_R, "EIKOU-R") \
	X(MUS_KARAKURI, "KARAKURI") \
	X(MUS_HUTAGO, "HUTAGO") \
	X(MUS_SITENNOU, "SITENNOU") \
	X(MUS_YAMA_EYE, "YAMA-EYE") \
	X(MUS_CONLOBBY, "CONLOBBY") \
	X(MUS_INTER_V, "INTER-V") \
	X(MUS_DAIGO, "DAIGO") \
	X(MUS_THANKFOR, "THANKFOR") \
	X(MUS_END, "END") \
	X(MUS_BATTLE27, "BATTLE27") \
	X(MUS_BATTLE31, "BATTLE31") \
	X(MUS_BATTLE20, "BATTLE20") \
	X(MUS_BATTLE32, "BATTLE32") \
	X(MUS_BATTLE33, "BATTLE33") \
	X(MUS_BATTLE36, "BATTLE36") \
	X(MUS_BATTLE34, "BATTLE34") \
	X(MUS_BATTLE35, "BATTLE35") \
	X(MUS_BATTLE38, "BATTLE38") \
	X(MUS_BATTLE30, "BATTLE30") \
	X(BGM_RADIO_TOWER_OCCUPIED, "RADIO-TOWER") \
	X(MUS_OOAME, "EMR-OOAME") \
	X(MUS_B_FRONTIER, "B-FRONTIER") \
	X(MUS_B_ARENA, "B-ARENA") \
	X(MUS_ME_POINTGET, "ME-POINTGET") \
	X(MUS_ME_TORE_EYE, "ME-TORE-EYE") \
	X(MUS_PYRAMID, "PYRAMID") \
	X(MUS_PYRAMID_TOP, "PYRAMID-TOP") \
	X(MUS_B_PALACE, "B-PALACE") \
	X(MUS_REKKUU_KOURIN, "REKKUU-KOURIN") \
	X(MUS_SATTOWER, "SATTOWER") \
	X(MUS_ME_SYMBOLGET, "ME-SYMBOLGET") \
	X(MUS_B_DOME, "B-DOME") \
	X(MUS_B_TUBE, "B-TUBE") \
	X(MUS_B_FACTORY, "B-FACTORY") \
	X(MUS_VS_REKKU, "VS-REKKU") \
	X(MUS_VS_FRONT, "VS-FRONT") \
	X(MUS_VS_MEW, "VS-MEW") \
	X(MUS_B_DOME1, "B-DOME1") \
	X(MUS_RG_ANNAI, "RG-ANNAI") \
	X(MUS_RG_SLOT, "RG-SLOT") \
	X(MUS_RG_AJITO, "RG-AJITO") \
	X(MUS_RG_GYM, "RG-GYM") \
	X(MUS_RG_PURIN, "RG-PURIN") \
	X(MUS_RG_DEMO, "RG-DEMO") \
	X(MUS_RG_TITLE, "RG-TITLE") \
	X(MUS_RG_GUREN, "RG-GUREN") \
	X(MUS_RG_SHION, "RG-SHION") \
	X(MUS_RG_KAIHUKU, "RG-KAIHUKU") \
	X(MUS_RG_CYCLING, "RG-CYCLING") \
	X(MUS_RG_ROCKET, "RG-ROCKET") \
	X(MUS_RG_SHOUJO, "RG-SHOUJO") \
	X(MUS_RG_SHOUNEN, "RG-SHOUNEN") \
	X(MUS_RG_DENDOU, "RG-DENDOU") \
	X(MUS_RG_T_MORI, "RG-T-MORI") \
	X(MUS_RG_OTSUKIMI, "RG-OTSUKIMI") \
	X(MUS_RG_POKEYASHI, "RG-POKEYASHI") \
	X(MUS_RG_ENDING, "RG-ENDING") \
	X(MUS_RG_LOAD01, "RG-LOAD01") \
	X(MUS_RG_OPENING, "RG-OPENING") \
	X(MUS_RG_LOAD02, "RG-LOAD02") \
	X(MUS_RG_LOAD03, "RG-LOAD03") \
	X(MUS_RG_CHAMP_R, "RG-CHAMP-R") \
	X(MUS_RG_VS_GYM, "RS-VS-GYM") \
	X(MUS_RG_VS_TORE, "RG-VS-TORE") \
	X(MUS_RG_VS_YASEI, "RG-VS-YASEI") \
	X(MUS_RG_VS_LAST, "RG-VS-LAST") \
	X(MUS_RG_MASARA, "RG-MASARA") \
	X(MUS_RG_KENKYU, "RG-KENKYU") \
	X(MUS_RG_OHKIDO, "RG-OHKIDO") \
	X(MUS_RG_POKECEN, "RG-POKECEN") \
	X(MUS_RG_SANTOAN, "RG-SANTOAN") \
	X(MUS_RG_NAMINORI, "RG-NAMINORI") \
	X(MUS_RG_P_TOWER, "RG-P-TOWER") \
	X(MUS_RG_SHIRUHU, "RG-SHIRUHU") \
	X(MUS_RG_HANADA, "RG-HANADA") \
	X(MUS_RG_TAMAMUSI, "RG-TAMAMUSI") \
	X(MUS_RG_WIN_TRE, "RG-WIN-TRE") \
	X(MUS_RG_WIN_YASEI, "RG-WIN-YASEI") \
	X(MUS_RG_WIN_GYM, "RG-WIN-GYM") \
	X(MUS_RG_KUCHIBA, "RG-KUCHIBA") \
	X(MUS_RG_NIBI, "RG-NIBI") \
	X(MUS_RG_RIVAL1, "RG-RIVAL1") \
	X(MUS_RG_RIVAL2, "RG-RIVAL2") \
	X(MUS_RG_FAN2, "RG-FAN2") \
	X(MUS_RG_FAN5, "RG-FAN5") \
	X(MUS_RG_FAN6, "RG-FAN6") \
	X(MUS_ME_RG_PHOTO, "ME-RG-PHOTO") \
	X(MUS_RG_TITLEROG, "RG-TITLEROG") \
	X(MUS_RG_GET_YASEI, "RG-GET-YASEI") \
	X(MUS_RG_SOUSA, "RG-SOUSA") \
	X(MUS_RG_SEKAIKAN, "RG-SEKAIKAN") \
	X(MUS_RG_SEIBETU, "RG-SEIBETU") \
	X(MUS_RG_JUMP, "RG-JUMP") \
	X(MUS_RG_UNION, "RG-UNION") \
	X(MUS_RG_NETWORK, "RG-NETWORK") \
	X(MUS_RG_OKURIMONO, "RG-OKURIMONO") \
	X(MUS_RG_KINOMIKUI, "RG-KINOMIKUI") \
	X(MUS_RG_NANADUNGEON, "RG-NANADUNGEON") \
	X(MUS_RG_OSHIE_TV, "RG-OSHIE-TV") \
	X(MUS_RG_NANASHIMA, "RG-NANASHIMA") \
	X(MUS_RG_NANAISEKI, "RG-NANAISEKI") \
	X(MUS_RG_NANA123, "RG-NANA123") \
	X(MUS_RG_NANA45, "RG-NANA45") \
	X(MUS_RG_NANA67, "RG-NANA67") \
	X(MUS_RG_POKEFUE, "RG-POKEFUE") \
	X(MUS_RG_VS_DEO, "RG-VS-DEO") \
	X(MUS_RG_VS_MYU2, "RG-VS-MYU2") \
	X(MUS_RG_VS_DEN, "RG-VS-DEN") \
	X(MUS_RG_EXEYE, "RG-EXEUE") \
	X(MUS_RG_DEOEYE, "RG-DEOEYE") \
	X(MUS_RG_T_TOWER, "RG-T-TOWER") \
	X(MUS_RG_SLOWMASARA, "RG-SLOWMASARA") \
	X(MUS_RG_TVNOIZE, "RG-TVNOIZE")

#define SOUND_LIST_SE \
	X(SE_STOP, "STOP") \
	X(SE_KAIFUKU, "KAIFUKU") \
	X(SE_PC_LOGON, "PC-LOGON") \
	X(SE_PC_OFF, "PC-OFF") \
	X(SE_PC_ON, "PC-ON") \
	X(SE_SELECT, "SELECT") \
	X(SE_WIN_OPEN, "WIN-OPEN") \
	X(SE_WALL_HIT, "WALL-HIT") \
	X(SE_DOOR, "DOOR") \
	X(SE_KAIDAN, "KAIDAN") \
	X(SE_DANSA, "DANSA") \
	X(SE_JITENSYA, "JITENSYA") \
	X(SE_KOUKA_L, "KOUKA-L") \
	X(SE_KOUKA_M, "KOUKA-M") \
	X(SE_KOUKA_H, "KOUKA-H") \
	X(SE_BOWA2, "BOWA2") \
	X(SE_POKE_DEAD, "POKE-DEAD") \
	X(SE_NIGERU, "NIGERU") \
	X(SE_JIDO_DOA, "JIDO-DOA") \
	X(SE_NAMINORI, "NAMINORI") \
	X(SE_BAN, "BAN") \
	X(SE_PIN, "PIN") \
	X(SE_BOO, "BOO") \
	X(SE_BOWA, "BOWA") \
	X(SE_JYUNI, "JYUNI") \
	X(SE_A, "A") \
	X(SE_I, "I") \
	X(SE_U, "U") \
	X(SE_E, "E") \
	X(SE_O, "O") \
	X(SE_N, "N") \
	X(SE_SEIKAI, "SEIKAI") \
	X(SE_HAZURE, "HAZURE") \
	X(SE_EXP, "EXP") \
	X(SE_JITE_PYOKO, "JITE-PYOKO") \
	X(SE_MU_PACHI, "MU-PACHI") \
	X(SE_TK_KASYA, "TK-KASYA") \
	X(SE_FU_ZAKU, "FU-ZAKU") \
	X(SE_FU_ZAKU2, "FU-ZAKU2") \
	X(SE_FU_ZUZUZU, "FU-ZUZUZU") \
	X(SE_RU_GASHIN, "RU-GASHIN") \
	X(SE_RU_GASYAN, "RU-GASYAN") \
	X(SE_RU_BARI, "RU-BARI") \
	X(SE_RU_HYUU, "RU-HYUU") \
	X(SE_KI_GASYAN, "KI-GASYAN") \
	X(SE_TK_WARPIN, "TK-WARPIN") \
	X(SE_TK_WARPOUT, "TK-WARPOUT") \
	X(SE_TU_SAA, "TU-SAA") \
	X(SE_HI_TURUN, "HI-TURUN") \
	X(SE_TRACK_MOVE, "TRACK-MOVE") \
	X(SE_TRACK_STOP, "TRACK-STOP") \
	X(SE_TRACK_HAIK, "TRACK-HAIK") \
	X(SE_TRACK_DOOR, "TRACK-DOOR") \
	X(SE_MOTER, "MOTER") \
	X(SE_CARD, "CARD") \
	X(SE_SAVE, "SAVE") \
	X(SE_KON, "KON") \
	X(SE_KON2, "KON2") \
	X(SE_KON3, "KON3") \
	X(SE_KON4, "KON4") \
	X(SE_SUIKOMU, "SUIKOMU") \
	X(SE_NAGERU, "NAGERU") \
	X(SE_TOY_C, "TOY-C") \
	X(SE_TOY_D, "TOY-D") \
	X(SE_TOY_E, "TOY-E") \
	X(SE_TOY_F, "TOY-F") \
	X(SE_TOY_G, "TOY-G") \
	X(SE_TOY_A, "TOY-A") \
	X(SE_TOY_B, "TOY-B") \
	X(SE_TOY_C1, "TOY-C1") \
	X(SE_MIZU, "MIZU") \
	X(SE_HASHI, "HASHI") \
	X(SE_DAUGI, "DAUGI") \
	X(SE_PINPON, "PINPON") \
	X(SE_FUUSEN1, "FUUSEN1") \
	X(SE_FUUSEN2, "FUUSEN2") \
	X(SE_FUUSEN3, "FUUSEN3") \
	X(SE_TOY_KABE, "TOY-KABE") \
	X(SE_TOY_DANGO, "TOY-DANGO") \
	X(SE_DOKU, "DOKU") \
	X(SE_ESUKA, "ESUKA") \
	X(SE_T_AME, "T-AME") \
	X(SE_T_AME_E, "T-AME-E") \
	X(SE_T_OOAME, "T-OOAME") \
	X(SE_T_OOAME_E, "T-OOAME-E") \
	X(SE_T_KOAME, "T-KOAME") \
	X(SE_T_KOAME_E, "T-KOAME-E") \
	X(SE_T_KAMI, "T-KAMI") \
	X(SE_T_KAMI2, "T-KAMI2") \
	X(SE_ELEBETA, "ELEBETA") \
	X(SE_HINSI, "HINSI") \
	X(SE_EXPMAX, "EXPMAX") \
	X(SE_TAMAKORO, "TAMAKORO") \
	X(SE_TAMAKORO_E, "TAMAKORO-E") \
	X(SE_BASABASA, "BASABASA") \
	X(SE_REGI, "REGI") \
	X(SE_C_GAJI, "C-GAJI") \
	X(SE_C_MAKU_U, "C-MAKU-U") \
	X(SE_C_MAKU_D, "C-MAKU-D") \
	X(SE_C_PASI, "C-PASI") \
	X(SE_C_SYU, "C-SYU") \
	X(SE_C_PIKON, "C-PIKON") \
	X(SE_REAPOKE, "REAPOKE") \
	X(SE_OP_BASYU, "OP-BASYU") \
	X(SE_BT_START, "BT-START") \
	X(SE_DENDOU, "DENDOU") \
	X(SE_JIHANKI, "JIHANKI") \
	X(SE_TAMA, "TAMA") \
	X(SE_Z_SCROLL, "Z-SCROLL") \
	X(SE_Z_PAGE, "Z-PAGE") \
	X(SE_PN_ON, "PN-ON") \
	X(SE_PN_OFF, "PN-OFF") \
	X(SE_Z_SEARCH, "Z-SEARCH") \
	X(SE_TAMAGO, "TAMAGO") \
	X(SE_TB_START, "TB-START") \
	X(SE_TB_KON, "TB-KON") \
	X(SE_TB_KARA, "TB-KARA") \
	X(SE_BIDORO, "BIDORO") \
	X(SE_W085, "W085") \
	X(SE_W085B, "W085B") \
	X(SE_W231, "W231") \
	X(SE_W171, "W171") \
	X(SE_W233, "W233") \
	X(SE_W233B, "W233B") \
	X(SE_W145, "W145") \
	X(SE_W145B, "W145B") \
	X(SE_W145C, "W145C") \
	X(SE_W240, "W240") \
	X(SE_W015, "W015") \
	X(SE_W081, "W081") \
	X(SE_W081B, "W081B") \
	X(SE_W088, "W088") \
	X(SE_W016, "W016") \
	X(SE_W016B, "W016B") \
	X(SE_W003, "W003") \
	X(SE_W104, "W104") \
	X(SE_W013, "W013") \
	X(SE_W196, "W196") \
	X(SE_W086, "W086") \
	X(SE_W004, "W004") \
	X(SE_W025, "W025") \
	X(SE_W025B, "W025B") \
	X(SE_W152, "W152") \
	X(SE_W026, "W026") \
	X(SE_W172, "W172") \
	X(SE_W172B, "W172B") \
	X(SE_W053, "W053") \
	X(SE_W007, "W007") \
	X(SE_W092, "W092") \
	X(SE_W221, "W221") \
	X(SE_W221B, "W221B") \
	X(SE_W052, "W052") \
	X(SE_W036, "W036") \
	X(SE_W059, "W059") \
	X(SE_W059B, "W059B") \
	X(SE_W010, "W010") \
	X(SE_W011, "W011") \
	X(SE_W017, "W017") \
	X(SE_W019, "W019") \
	X(SE_W028, "W028") \
	X(SE_W013B, "W013B") \
	X(SE_W044, "W044") \
	X(SE_W029, "W029") \
	X(SE_W057, "W057") \
	X(SE_W056, "W056") \
	X(SE_W250, "W250") \
	X(SE_W030, "W030") \
	X(SE_W039, "W039") \
	X(SE_W054, "W054") \
	X(SE_W077, "W077") \
	X(SE_W020, "W020") \
	X(SE_W082, "W082") \
	X(SE_W047, "W047") \
	X(SE_W195, "W195") \
	X(SE_W006, "W006") \
	X(SE_W091, "W091") \
	X(SE_W146, "W146") \
	X(SE_W120, "W120") \
	X(SE_W153, "W153") \
	X(SE_W071B, "W071B") \
	X(SE_W071, "W071") \
	X(SE_W103, "W103") \
	X(SE_W062, "W062") \
	X(SE_W062B, "W062B") \
	X(SE_W048, "W048") \
	X(SE_W187, "W187") \
	X(SE_W118, "W118") \
	X(SE_W155, "W155") \
	X(SE_W122, "W122") \
	X(SE_W060, "W060") \
	X(SE_W185, "W185") \
	X(SE_W014, "W014") \
	X(SE_W043, "W043") \
	X(SE_W207, "W207") \
	X(SE_W207B, "W207B") \
	X(SE_W215, "W215") \
	X(SE_W109, "W109") \
	X(SE_W173, "W173") \
	X(SE_W280, "W280") \
	X(SE_W202, "W202") \
	X(SE_W060B, "W060B") \
	X(SE_W076, "W076") \
	X(SE_W080, "W080") \
	X(SE_W100, "W100") \
	X(SE_W107, "W107") \
	X(SE_W166, "W166") \
	X(SE_W129, "W129") \
	X(SE_W115, "W115") \
	X(SE_W112, "W112") \
	X(SE_W197, "W197") \
	X(SE_W199, "W199") \
	X(SE_W236, "W236") \
	X(SE_W204, "W204") \
	X(SE_W268, "W268") \
	X(SE_W070, "W070") \
	X(SE_W063, "W063") \
	X(SE_W127, "W127") \
	X(SE_W179, "W179") \
	X(SE_W151, "W151") \
	X(SE_W201, "W201") \
	X(SE_W161, "W161") \
	X(SE_W161B, "W161B") \
	X(SE_W227, "W227") \
	X(SE_W227B, "W227B") \
	X(SE_W226, "W226") \
	X(SE_W208, "W208") \
	X(SE_W213, "W213") \
	X(SE_W213B, "W213B") \
	X(SE_W234, "W234") \
	X(SE_W260, "W260") \
	X(SE_W328, "W328") \
	X(SE_W320, "W320") \
	X(SE_W255, "W255") \
	X(SE_W291, "W291") \
	X(SE_W089, "W089") \
	X(SE_W239, "W239") \
	X(SE_W230, "W230") \
	X(SE_W281, "W281") \
	X(SE_W327, "W327") \
	X(SE_W287, "W287") \
	X(SE_W257, "W257") \
	X(SE_W253, "W253") \
	X(SE_W258, "W258") \
	X(SE_W322, "W322") \
	X(SE_W298, "W298") \
	X(SE_W287B, "W287B") \
	X(SE_W114, "W114") \
	X(SE_W063B, "W063B") \
	X(SE_RG_W_DOOR, "RG-W-DOOR") \
	X(SE_RG_CARD1, "RG-CARD1") \
	X(SE_RG_CARD2, "RG-CARD2") \
	X(SE_RG_CARD3, "RG-CARD3") \
	X(SE_RG_BAG1, "RG-BAG1") \
	X(SE_RG_BAG2, "RG-BAG2") \
	X(SE_RG_GETTING, "RG-GETTING") \
	X(SE_RG_SHOP, "RG-SHOP") \
	X(SE_RG_KITEKI, "RG-KITEKI") \
	X(SE_RG_HELP_OP, "RG-HELP-CL") \
	X(SE_RG_HELP_CL, "RG-HELP-CL") \
	X(SE_RG_HELP_NGM, "RG-HELP-NGM") \
	X(SE_RG_DEOMOV, "RG-DEOMOV") \
	X(SE_RG_EXCELLENT, "RG-EXCELLENT") \
	X(SE_RG_NAWAMISS, "RG-NAWAMISS") \
	X(SE_TOREEYE, "TOREEYE") \
	X(SE_TOREOFF, "TOREOFF") \
	X(SE_HANTEI1, "HANTEI1") \
	X(SE_HANTEI2, "HANTEI2") \
	X(SE_CURTAIN, "CURTAIN") \
	X(SE_CURTAIN1, "CURTAIN1") \
	X(SE_USSOKI, "USSOKI")

// Create BGM list
#define X(songId, name) static const u8 sBGMName_##songId[] = _(name);
SOUND_LIST_BGM;
#undef X

#define X(songId, name) sBGMName_##songId,
static const u8 *const gBGMNames[] =
{
SOUND_LIST_BGM
};
#undef X

// Create SE list
#define X(songId, name) static const u8 sSEName_##songId[] = _(name);
SOUND_LIST_SE
#undef X

#define X(songId, name) sSEName_##songId,
static const u8 *const gSENames[] =
{
SOUND_LIST_SE
};
#undef X

void Task_InitCryTest(u8 taskId)
{
    struct CryRelatedStruct cryStruct, cryStruct2;
    u8 zero;

    Text_LoadWindowTemplate(&gWindowTemplate_81E6C3C);
    InitMenuWindow(&gMenuTextWindowTemplate);
    gSoundTestCryNum = 1;
    ResetSpriteData();
    FreeAllSpritePalettes();

    cryStruct.unk0 = 0x2000;
    cryStruct.unk2 = 29;
    cryStruct.paletteNo = 12;
    cryStruct.yPos = 30;
    cryStruct.xPos = 4;

    zero = 0; // wtf?
    gUnknown_03005E98 = 0;

    while (sub_8119E3C(&cryStruct, 3) == FALSE)
        ;

    cryStruct2.unk0 = 0;
    cryStruct2.unk2 = 15;
    cryStruct2.paletteNo = 13;
    cryStruct2.xPos = 12;
    cryStruct2.yPos = 12;

    zero = 0; // wtf?
    gUnknown_03005E98 = 0;

    while (ShowPokedexCryScreen(&cryStruct2, 2) == FALSE)
        ;

    Menu_DrawStdWindowFrame(0, 16, 5, 19);
    PrintCryNumber();
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2CNT = 0xF01;
    REG_BG3CNT = 0x1D03;
    REG_DISPCNT = 0x1d40;
    m4aMPlayFadeOutTemporarily(&gMPlayInfo_BGM, 2);
    gTasks[taskId].func = Task_ProcessCryTestInput;
}

void Task_ProcessCryTestInput(u8 taskId)
{
    sub_8119F88(3);

    if (gMain.newKeys & A_BUTTON)
    {
        sub_811A050(gSoundTestCryNum);
    }
    if (gMain.newKeys & R_BUTTON)
    {
        StopCryAndClearCrySongs();
    }
    if (gMain.newAndRepeatedKeys & DPAD_UP)
    {
        if(--gSoundTestCryNum == 0)
            gSoundTestCryNum = 412; // total species
        PrintCryNumber();
    }
    if (gMain.newAndRepeatedKeys & DPAD_DOWN)
    {
        if(++gSoundTestCryNum > 412)
            gSoundTestCryNum = 1;
        PrintCryNumber();
    }
    if (gMain.newKeys & B_BUTTON)
    {
        REG_DISPCNT = 0x7140;
        REG_WIN0H = WIN_RANGE(17, 223);
        REG_WIN0V = WIN_RANGE(1, 31);
        Menu_EraseWindowRect(0, 0, 29, 19);
        gTasks[taskId].func = Task_InitSoundCheckMenu;
        DestroyCryMeterNeedleSprite();
    }
}

void PrintCryNumber(void)
{
    PrintSignedNumber(gSoundTestCryNum, 1, 17, 3);
}
