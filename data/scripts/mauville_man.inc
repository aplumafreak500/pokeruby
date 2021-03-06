#include "constants/mauville_man.h"

MauvilleCity_PokemonCenter_1F_EventScript_1AE744:: @ 81AE744
	special ScrSpecial_GetCurrentMauvilleMan
	switch VAR_RESULT
	case MAUVILLE_MAN_BARD,        SpeakToBard
	case MAUVILLE_MAN_HIPSTER,     SpeakToHipster
	case MAUVILLE_MAN_TRADER,      SpeakToTrader
	case MAUVILLE_MAN_STORYTELLER, SpeakToStoryteller
	case MAUVILLE_MAN_GIDDY,       SpeakToGiddy
	end


@-------------------------------------------------------------------------------
@ Bard
@-------------------------------------------------------------------------------

SpeakToBard:
	lock
	faceplayer
	msgbox gTextBard_HiImTheBard, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq yes_hear_song
	compare VAR_RESULT, NO
	goto_if_eq dont_hear_song
	end

yes_hear_song:
	setvar VAR_0x8004, 0
	@ Give the player ear rape
	special ScrSpecial_PlayBardSong
	delay 60
	special ScrSpecial_HasBardSongBeenChanged
	compare VAR_RESULT, FALSE
	@ Prompt new lyrics only if song hasn't been changed
	goto_if_eq prompt_write_lyrics
	msgbox gTextBard_OhWhatAMovingSong, MSGBOX_DEFAULT
	release
	end

dont_hear_song:
	msgbox gTextBard_OhYouveLeftMe, MSGBOX_DEFAULT
	release
	end

prompt_write_lyrics:
	msgbox gTextBard_SoHowDoYouLikeMySong, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq write_lyrics
	compare VAR_RESULT, NO
	goto_if_eq dont_write_lyrics
	end

write_lyrics:
	setvar VAR_0x8004, 6
	call Common_EventScript_ShowEasyChatScreen
	lock
	faceplayer
	compare VAR_RESULT, NO
	goto_if_eq dont_write_lyrics
	msgbox gTextBard_ThankYouKindly, MSGBOX_DEFAULT
	setvar VAR_0x8004, 1
	@ Give the player ear rape again
	special ScrSpecial_PlayBardSong
	delay 60
	msgbox gTextBard_WasThatHowYouWanted, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq write_lyrics		@ Keep looping until player responds YES
	special ScrSpecial_SaveBardSongLyrics
	msgbox gTextBard_OkayThatsIt, MSGBOX_DEFAULT
	release
	end

dont_write_lyrics:
	msgbox gTextBard_OhYouveLeftMe2, MSGBOX_DEFAULT
	release
	end


@-------------------------------------------------------------------------------
@ Hipster
@-------------------------------------------------------------------------------

SpeakToHipster:
	lock
	faceplayer
	setflag FLAG_SYS_HIPSTER_MEET
	msgbox gTextHipster_TheyCallMeTheHipster, MSGBOX_DEFAULT
	special ScrSpecial_GetHipsterSpokenFlag
	compare VAR_RESULT, FALSE
	goto_if_eq hipster_first_time
	msgbox gTextHipster_TaughtYouAlready, MSGBOX_DEFAULT
	release
	end

hipster_first_time:
	special ScrSpecial_HipsterTeachWord
	compare VAR_RESULT, TRUE		@ TRUE if player learned a new word
	goto_if_eq teach_new_word
	msgbox gTextHipster_YouAlreadyKnowALot, MSGBOX_DEFAULT
	release
	end

teach_new_word:
	msgbox gTextHipster_HaveYouHeardAbout, MSGBOX_DEFAULT
	special ScrSpecial_SetHipsterSpokenFlag
	release
	end


@-------------------------------------------------------------------------------
@ Trader
@-------------------------------------------------------------------------------

	.ifdef ENGLISH
	.include "data/text/trader.inc"
	.else
	.include "data/text_de/trader.inc"
	.endif

SpeakToTrader:
	lock
	faceplayer
	msgbox gTextTrader_Introduction, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq dont_want_to_trade
	special ScrSpecial_GetTraderTradedFlag
	compare VAR_RESULT, TRUE
	goto_if_eq already_traded
	message gTextTrader_MenuPrompt
	waitmessage
	goto do_trader_menu_get
	end

dont_want_to_trade:
	msgbox gTextTrader_FeelUnwanted1, MSGBOX_NPC
	end

already_traded:
	msgbox gTextTrader_TradedAlready, MSGBOX_NPC
	end

do_trader_menu_get:
	special ScrSpecial_TraderMenuGetDecoration
	waitstate
	compare VAR_0x8004, 0
	goto_if_eq cancelled_get_menu
	compare VAR_0x8004, -1
	goto_if_eq rare_item_cant_trade_away
	msgbox gTextTrader_ItemOnceBelongedTo, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq dont_want_item
	special ScrSpecial_DoesPlayerHaveNoDecorations
	compare VAR_RESULT, TRUE
	goto_if_eq player_has_no_decorations
	goto do_trader_menu_give
	end

cancelled_get_menu:
	msgbox gTextTrader_DontWantAnything, MSGBOX_NPC
	end

rare_item_cant_trade_away:
	message gTextTrader_ICantTradeThatOneAway
	waitmessage
	goto do_trader_menu_get
	end

dont_want_item:
	message gTextTrader_MenuPrompt
	waitmessage
	goto do_trader_menu_get
	end

player_has_no_decorations:
	msgbox gTextTrader_YouDontHaveDecorations, MSGBOX_NPC
	end

do_trader_menu_give:
	msgbox gTextTrader_PickDecorationYoullTrade, MSGBOX_DEFAULT
	special ScrSpecial_TraderMenuGiveDecoration
	waitstate
	compare VAR_0x8006, 0
	goto_if_eq cancelled_give_menu
	compare VAR_0x8006, -1
	goto_if_eq decoration_is_in_use
	special ScrSpecial_IsDecorationFull
	compare VAR_RESULT, 1
	goto_if_eq decorations_full
	msgbox gTextTrader_SoWellTrade, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq do_trader_menu_give
	special ScrSpecial_TraderDoDecorationTrade
	msgbox gTextTrader_ThenWellTrade, MSGBOX_NPC
	end

cancelled_give_menu:
	msgbox gTextTrader_FeelUnwanted2, MSGBOX_NPC
	end

decoration_is_in_use:
	msgbox gTextTrader_InUseYouCantTradeIt, MSGBOX_DEFAULT
	goto do_trader_menu_give
	end

decorations_full:
	msgbox gTextTrader_NoRoomForThis, MSGBOX_NPC
	end


@-------------------------------------------------------------------------------
@ Storyteller
@-------------------------------------------------------------------------------

	.ifdef ENGLISH
	.include "data/text/storyteller.inc"
	.else
	.include "data/text_de/storyteller.inc"
	.endif

SpeakToStoryteller:
	setvar VAR_0x8008, 0
	setvar VAR_0x8009, 0
	setvar VAR_0x800A, 0
	setvar VAR_0x800B, 0
	lock
	faceplayer
	msgbox gTextStoryteller_Introduction, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq dont_hear_story
	specialvar VAR_RESULT, ScrSpecial_StorytellerGetFreeStorySlot
	compare VAR_RESULT, 0		@ If slot is 0, then the list is empty
	goto_if_eq no_stories_recorded
choose_story:
	message gTextStoryteller_WhichTale
	waitmessage
	special ScrSpecial_StorytellerStoryListMenu
	waitstate
	compare VAR_RESULT, FALSE
	goto_if_eq cancel_story_menu
	setvar VAR_0x8008, 1
	special ScrSpecial_StorytellerDisplayStory
	waitmessage
	waitbuttonpress
	specialvar VAR_RESULT, ScrSpecial_StorytellerUpdateStat
	compare VAR_RESULT, FALSE
	goto_if_eq no_stat_update
	goto stat_update
cancel_story_menu:
	compare VAR_0x8008, 0
	goto_if_eq dont_hear_story
	goto yes_hear_story
no_stat_update:
	msgbox gTextStoryteller_CouldThereBeOtherTrainers, MSGBOX_DEFAULT
	msgbox gTextStoryteller_HearAnotherLegendaryTale, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq choose_story
yes_hear_story:
	specialvar VAR_RESULT, ScrSpecial_HasStorytellerAlreadyRecorded
	compare VAR_RESULT, TRUE
	goto_if_eq cant_record_story		@ already recorded story
	specialvar VAR_RESULT, ScrSpecial_StorytellerGetFreeStorySlot
	compare VAR_RESULT, 4
	goto_if_eq cant_record_story		@ story list is full
	goto prompt_record_story
no_stories_recorded:
	msgbox gTextStoryteller_ButIKnowOfNoLegendaryTrainers, MSGBOX_DEFAULT
prompt_record_story:
	msgbox gTextStoryteller_HaveYouAnyTales, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq dont_hear_story
	specialvar VAR_RESULT, ScrSpecial_StorytellerInitializeRandomStat
	compare VAR_RESULT, TRUE
	goto_if_eq stat_update
	msgbox gTextStoryteller_ImNotSatisfied, MSGBOX_DEFAULT
	closemessage
	release
	end

stat_update:
	msgbox gTextStoryteller_BirthOfANewLegend, MSGBOX_DEFAULT
	closemessage
	release
	end

dont_hear_story:
	msgbox gTextStoryteller_OhIFeelStifled, MSGBOX_DEFAULT
	closemessage
	release
	end

cant_record_story:
	msgbox gTextStoryteller_WishMorePeopleWould, MSGBOX_DEFAULT
	closemessage
	release
	end


@-------------------------------------------------------------------------------
@ Giddy
@-------------------------------------------------------------------------------

	.ifdef ENGLISH
	.include "data/text/giddy.inc"
	.else
	.include "data/text_de/giddy.inc"
	.endif

SpeakToGiddy:
	lock
	faceplayer
	msgbox gTextGiddy_Introduction, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq yes_hear_giddy
	compare VAR_RESULT, NO
	goto_if_eq dont_hear_giddy
	end

yes_hear_giddy:
	special ScrSpecial_GiddyShouldTellAnotherTale
	compare VAR_RESULT, TRUE
	goto_if_eq tell_giddy_tale
	compare VAR_RESULT, FALSE
	goto_if_eq bye_bye
	end

tell_another_giddy_tale:
	special ScrSpecial_GiddyShouldTellAnotherTale
	compare VAR_RESULT, TRUE
	goto_if_eq also_i_was_thinking
	compare VAR_RESULT, FALSE
	goto_if_eq bye_bye
	end

also_i_was_thinking:
	msgbox gTextGiddy_AlsoIWasThinking, MSGBOX_DEFAULT
	goto tell_giddy_tale
	end

tell_giddy_tale:
	special ScrSpecial_GenerateGiddyLine
	special ShowFieldMessageStringVar4
	waitmessage
	yesnobox 20, 8
	compare VAR_RESULT, 1
	goto_if_eq tell_another_giddy_tale
	compare VAR_RESULT, 0
	goto_if_eq tell_another_giddy_tale
	end

dont_hear_giddy:
	msgbox gTextGiddy_YouveDeflatedMe, MSGBOX_DEFAULT
	release
	end

bye_bye:
	msgbox gTextGiddy_ByeBye, MSGBOX_DEFAULT
	release
	end

	.ifdef ENGLISH
	.include "data/text/bard.inc"
	.include "data/text/hipster.inc"
	.else
	.include "data/text_de/bard.inc"
	.include "data/text_de/hipster.inc"
	.endif
