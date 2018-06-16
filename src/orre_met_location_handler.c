#include "global.h"
#include "pokemon.h"
#include "strings.h"
#include "constants/species.h"

extern const u8 gMapName_Agate[];
extern const u8 gMapName_DeepColl[];
extern const u8 gMapName_Lab[];
extern const u8 gMapName_Mayor[];
extern const u8 gMapName_Miror[];
extern const u8 gMapName_Outskirt[];
extern const u8 gMapName_Phenac[];
extern const u8 gMapName_PyriteBdg[];
extern const u8 gMapName_PyriteCave[];
extern const u8 gMapName_Pyrite[];
extern const u8 gMapName_Realgam[];
extern const u8 gMapName_Realgam2[];
extern const u8 gMapName_Snagem[];
extern const u8 gMapName_Under[];
extern const u8 gMapName_UnderSub[];
extern const u8 gMapName_Tower[];
extern const u8 gMapName_Cave[];
extern const u8 gMapName_CipherKey[];
extern const u8 gMapName_CipherLab[];
extern const u8 gMapName_Citadark[];
extern const u8 gMapName_Gateon[];
extern const u8 gMapName_MtBattle[];
extern const u8 gMapName_Oasis[];
extern const u8 gMapName_PokemonHQ[];
extern const u8 gMapName_Rock[];
extern const u8 gMapName_XDEevee[];
extern const u8 gMapName_CollStarter[];
extern const u8 gMapName_DukingPlusle[];

u8* DetermineOrreMetLocation(struct Pokemon *);
u8* GetOrreMetLocationString(struct Pokemon *);
u8* ResolveOrreMetLocationCollision(struct Pokemon *);

u8* DetermineOrreMetLocation(struct Pokemon *mon)
{
	u8 *stringToUse;
	u8 locationMet = GetMonData(mon, MON_DATA_MET_LOCATION);
	if(locationMet == 76 || locationMet == 109 || locationMet == 110 || locationMet == 111 || locationMet == 67 || locationMet == 69 || locationMet == 0 || locationMet == 254)
	{
		stringToUse = ResolveOrreMetLocationCollision(mon);
	}
	else
	{
		stringToUse = GetOrreMetLocationString(mon);
	}
	return (u8 *)stringToUse;
}

u8* GetOrreMetLocationString(struct Pokemon *mon)
{
	u8 locationMet = GetMonData(mon, MON_DATA_MET_LOCATION);
	const u8 *locationString;
	switch (locationMet)
	{
			//valid Colosseum locations
			case 39: //only valid Agate Village location
				locationString = gMapName_Agate;
				break;
			case 125: //only valid Deep Colosseum location
				locationString = gMapName_DeepColl;
				break;
			case 68: //only valid Laboratory location
				locationString = gMapName_Lab;
				break;
			case 5: //only valid Mayor's House location
				locationString = gMapName_Mayor;
				break;
			case 30: //only valid Miror's Hideout location, Remoraid and Mantine 2nd chance here or Pyrite Cave?
				locationString = gMapName_Miror;
				break;
			case 1: //only valid Outskirt Stand location
				locationString = gMapName_Outskirt;
				break;
			case 3: //only normally valid Phenac City location
			case 128: //eReader Pokemon are from here in Phenac City
				locationString = gMapName_Phenac;
				break;
			case 25:
			case 28: //only valid Pyrite Building locations
				locationString = gMapName_PyriteBdg;
				break;
			case 29:
			case 31:
			case 32: //only valid Pyrite Cave locations
				locationString = gMapName_PyriteCave;
				break;
			case 15: //only valid Pyrite Town location
				locationString = gMapName_Pyrite;
				break;
			case 115:
			case 117: //valid Realgam Tower locations; see below comment
				locationString = gMapName_Realgam;
				break;
			case 104:
			case 106: //Do these three display as Realgam Tower instead in game?
			case 113: //only valid RealgamTwr Dome locations
				locationString = gMapName_Realgam2;
				break;
			case 132:
			case 133:
			case 134: //only valid Snagem Hideout locations
				locationString = gMapName_Snagem;
				break;
			case 47:
			case 55: //only valid The Under locations
				locationString = gMapName_Under;
				break;
			case 58: //only valid The Under Subway location
				locationString = gMapName_UnderSub;
				break;
			case 118: //only valid Tower Colosseum location
				locationString = gMapName_Tower;
				break;
			//valid XD locations
			case 92: //only valid Cave Poke Spot location
				locationString = gMapName_Cave;
				break;
			case 64:
			case 65:
			case 66:
			case 70:
			case 71: //only valid Cipher Key Lair locations
				locationString = gMapName_CipherKey;
				break;
			case 8:
			case 9:
			case 10:
			case 11: //only valid Cipher Lab locations
				locationString = gMapName_CipherLab;
				break;
			case 73:
			case 74:
			case 75:
			case 77:
			case 80:
			case 81:
			case 84:
			case 85:
			case 87:
			case 88: //only valid Citadark Isle locations
				locationString = gMapName_Citadark;
				break;
			case 153:
			case 162: //only valid Gateon Port locations
				locationString = gMapName_Gateon;
				break;
			case 16: //only valid Mt. Battle location; gift Johto Starters
				locationString = gMapName_MtBattle;
				break;
			case 91: //only valid Oasis Poke Spot location
				locationString = gMapName_Oasis;
				break;
			case 164: //only valid Outskirt Stand location
				locationString = gMapName_Outskirt;
				break;
			case 94:
			case 96:
			case 97:
			case 100:
			case 107: //only valid Phenac City locations
				locationString = gMapName_Phenac;
				break;
			case 143: //only valid Pokemon HQ Lab location
				locationString = gMapName_PokemonHQ;
				break;
			case 116:
			case 119: //only valid Pyrite Town locations
				locationString = gMapName_Pyrite;
				break;
			case 59: //only valid Realgam Tower location
				locationString = gMapName_Realgam;
				break;
			case 90: //only valid Rock Poke Spot location
				locationString = gMapName_Rock;
				break;
			default: //if any other location index is present, use generic distant land text.
				locationString = gOtherText_MetDistantLand;
				break;
	}
	return (u8 *)locationString;
}

u8* ResolveOrreMetLocationCollision(struct Pokemon *mon)
{
	u16 species;
	u8 locationMet;
	const u8 *locationString;
	locationMet = GetMonData(mon, MON_DATA_MET_LOCATION);
	species = GetMonData(mon, MON_DATA_SPECIES);
	switch (locationMet)
	{
		case 67:
		case 69: //Colosseum: Laboratory; XD: Cipher Key Lair
			if(species == SPECIES_PRIMEAPE || species == SPECIES_HYPNO || species == SPECIES_TANGELA || species == SPECIES_BUTTERFREE || species == SPECIES_MAGNETON)
				locationString = gMapName_CipherKey;
			else
				locationString = gMapName_Lab;
			break;
		case 76: //Colosseum: Mt. Battle; XD: Citadark Isle
			if(species == SPECIES_ENTEI)
				locationString = gMapName_MtBattle;
			else
				locationString = gMapName_Citadark;
			break;
		case 109:
		case 110:
		case 111: //Colosseum: RealgamTwr Dome; XD: Pyrite Town
			if(species == SPECIES_SUNFLORA || species == SPECIES_DELIBIRD || species == SPECIES_SUICUNE || species == SPECIES_HERACROSS)
				locationString = gMapName_Realgam2;
			else
				locationString = gMapName_Pyrite;
			break;
		case 0: //XD Starter Eevee
			if((species >= SPECIES_EEVEE && species <= SPECIES_FLAREON) || species == SPECIES_ESPEON || species == SPECIES_UMBREON)
				locationString = gMapName_XDEevee;
			break;
		case 254: //Colosseum Starter Espeon and Umbreon and Duking's Plusle
			if(species == SPECIES_ESPEON || species == SPECIES_UMBREON)
				locationString = gMapName_CollStarter;
			else
				locationString = gMapName_DukingPlusle;
			break;
		default: //error handler; default to generic distant land text
			locationString = gOtherText_MetDistantLand;
			break;
	}
	return (u8 *)locationString;
}

