#include "constants/flags.h"
#include "constants/items.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/secret_bases.h"
#include "constants/vars.h"
#include "constants/weather.h"
	.include "include/macros.inc"
	.include "constants/constants.inc"

	.section .rodata

	.include "data/maps/events/PetalburgCity.inc"
	.include "data/maps/events/SlateportCity.inc"
	.include "data/maps/events/MauvilleCity.inc"
	.include "data/maps/events/RustboroCity.inc"
	.include "data/maps/events/FortreeCity.inc"
	.include "data/maps/events/LilycoveCity.inc"
	.include "data/maps/events/MossdeepCity.inc"
	.include "data/maps/events/SootopolisCity.inc"
	.include "data/maps/events/EverGrandeCity.inc"
	.include "data/maps/events/LittlerootTown.inc"
	.include "data/maps/events/OldaleTown.inc"
	.include "data/maps/events/DewfordTown.inc"
	.include "data/maps/events/LavaridgeTown.inc"
	.include "data/maps/events/FallarborTown.inc"
	.include "data/maps/events/VerdanturfTown.inc"
	.include "data/maps/events/PacifidlogTown.inc"
	.include "data/maps/events/Route101.inc"
	.include "data/maps/events/Route102.inc"
	.include "data/maps/events/Route103.inc"
	.include "data/maps/events/Route104.inc"
	.include "data/maps/events/Route105.inc"
	.include "data/maps/events/Route106.inc"
	.include "data/maps/events/Route107.inc"
	.include "data/maps/events/Route108.inc"
	.include "data/maps/events/Route109.inc"
	.include "data/maps/events/Route110.inc"
	.include "data/maps/events/Route111.inc"
	.include "data/maps/events/Route112.inc"
	.include "data/maps/events/Route113.inc"
	.include "data/maps/events/Route114.inc"
	.include "data/maps/events/Route115.inc"
	.include "data/maps/events/Route116.inc"
	.include "data/maps/events/Route117.inc"
	.include "data/maps/events/Route118.inc"
	.include "data/maps/events/Route119.inc"
	.include "data/maps/events/Route120.inc"
	.include "data/maps/events/Route121.inc"
	.include "data/maps/events/Route122.inc"
	.include "data/maps/events/Route123.inc"
	.include "data/maps/events/Route124.inc"
	.include "data/maps/events/Route125.inc"
	.include "data/maps/events/Route126.inc"
	.include "data/maps/events/Route127.inc"
	.include "data/maps/events/Route128.inc"
	.include "data/maps/events/Route129.inc"
	.include "data/maps/events/Route130.inc"
	.include "data/maps/events/Route131.inc"
	.include "data/maps/events/Route132.inc"
	.include "data/maps/events/Route133.inc"
	.include "data/maps/events/Route134.inc"
	.include "data/maps/events/Underwater1.inc"
	.include "data/maps/events/Underwater2.inc"
	.include "data/maps/events/Underwater3.inc"
	.include "data/maps/events/Underwater4.inc"
	.include "data/maps/events/LittlerootTown_BrendansHouse_1F.inc"
	.include "data/maps/events/LittlerootTown_BrendansHouse_2F.inc"
	.include "data/maps/events/LittlerootTown_MaysHouse_1F.inc"
	.include "data/maps/events/LittlerootTown_MaysHouse_2F.inc"
	.include "data/maps/events/LittlerootTown_ProfessorBirchsLab.inc"
	.include "data/maps/events/OldaleTown_House1.inc"
	.include "data/maps/events/OldaleTown_House2.inc"
	.include "data/maps/events/OldaleTown_PokemonCenter_1F.inc"
	.include "data/maps/events/OldaleTown_PokemonCenter_2F.inc"
	.include "data/maps/events/OldaleTown_Mart.inc"
	.include "data/maps/events/DewfordTown_House1.inc"
	.include "data/maps/events/DewfordTown_PokemonCenter_1F.inc"
	.include "data/maps/events/DewfordTown_PokemonCenter_2F.inc"
	.include "data/maps/events/DewfordTown_Gym.inc"
	.include "data/maps/events/DewfordTown_Hall.inc"
	.include "data/maps/events/DewfordTown_House2.inc"
	.include "data/maps/events/LavaridgeTown_HerbShop.inc"
	.include "data/maps/events/LavaridgeTown_Gym_1F.inc"
	.include "data/maps/events/LavaridgeTown_Gym_B1F.inc"
	.include "data/maps/events/LavaridgeTown_House.inc"
	.include "data/maps/events/LavaridgeTown_Mart.inc"
	.include "data/maps/events/LavaridgeTown_PokemonCenter_1F.inc"
	.include "data/maps/events/LavaridgeTown_PokemonCenter_2F.inc"
	.include "data/maps/events/FallarborTown_Mart.inc"
	.include "data/maps/events/FallarborTown_ContestLobby.inc"
	.include "data/maps/events/FallarborTown_ContestHall.inc"
	.include "data/maps/events/FallarborTown_PokemonCenter_1F.inc"
	.include "data/maps/events/FallarborTown_PokemonCenter_2F.inc"
	.include "data/maps/events/FallarborTown_House1.inc"
	.include "data/maps/events/FallarborTown_House2.inc"
	.include "data/maps/events/VerdanturfTown_ContestLobby.inc"
	.include "data/maps/events/VerdanturfTown_ContestHall.inc"
	.include "data/maps/events/VerdanturfTown_Mart.inc"
	.include "data/maps/events/VerdanturfTown_PokemonCenter_1F.inc"
	.include "data/maps/events/VerdanturfTown_PokemonCenter_2F.inc"
	.include "data/maps/events/VerdanturfTown_WandasHouse.inc"
	.include "data/maps/events/VerdanturfTown_FriendshipRatersHouse.inc"
	.include "data/maps/events/VerdanturfTown_House.inc"
	.include "data/maps/events/PacifidlogTown_PokemonCenter_1F.inc"
	.include "data/maps/events/PacifidlogTown_PokemonCenter_2F.inc"
	.include "data/maps/events/PacifidlogTown_House1.inc"
	.include "data/maps/events/PacifidlogTown_House2.inc"
	.include "data/maps/events/PacifidlogTown_House3.inc"
	.include "data/maps/events/PacifidlogTown_House4.inc"
	.include "data/maps/events/PacifidlogTown_House5.inc"
	.include "data/maps/events/PetalburgCity_WallysHouse.inc"
	.include "data/maps/events/PetalburgCity_Gym.inc"
	.include "data/maps/events/PetalburgCity_House1.inc"
	.include "data/maps/events/PetalburgCity_House2.inc"
	.include "data/maps/events/PetalburgCity_PokemonCenter_1F.inc"
	.include "data/maps/events/PetalburgCity_PokemonCenter_2F.inc"
	.include "data/maps/events/PetalburgCity_Mart.inc"
	.include "data/maps/events/SlateportCity_SternsShipyard_1F.inc"
	.include "data/maps/events/SlateportCity_SternsShipyard_2F.inc"
	.include "data/maps/events/SlateportCity_ContestLobby.inc"
	.include "data/maps/events/SlateportCity_ContestHall.inc"
	.include "data/maps/events/SlateportCity_House1.inc"
	.include "data/maps/events/SlateportCity_PokemonFanClub.inc"
	.include "data/maps/events/SlateportCity_OceanicMuseum_1F.inc"
	.include "data/maps/events/SlateportCity_OceanicMuseum_2F.inc"
	.include "data/maps/events/SlateportCity_Harbor.inc"
	.include "data/maps/events/SlateportCity_House2.inc"
	.include "data/maps/events/SlateportCity_PokemonCenter_1F.inc"
	.include "data/maps/events/SlateportCity_PokemonCenter_2F.inc"
	.include "data/maps/events/SlateportCity_Mart.inc"
	.include "data/maps/events/MauvilleCity_Gym.inc"
	.include "data/maps/events/MauvilleCity_BikeShop.inc"
	.include "data/maps/events/MauvilleCity_House1.inc"
	.include "data/maps/events/MauvilleCity_GameCorner.inc"
	.include "data/maps/events/MauvilleCity_House2.inc"
	.include "data/maps/events/MauvilleCity_PokemonCenter_1F.inc"
	.include "data/maps/events/MauvilleCity_PokemonCenter_2F.inc"
	.include "data/maps/events/MauvilleCity_Mart.inc"
	.include "data/maps/events/RustboroCity_DevonCorp_1F.inc"
	.include "data/maps/events/RustboroCity_DevonCorp_2F.inc"
	.include "data/maps/events/RustboroCity_DevonCorp_3F.inc"
	.include "data/maps/events/RustboroCity_Gym.inc"
	.include "data/maps/events/RustboroCity_PokemonSchool.inc"
	.include "data/maps/events/RustboroCity_PokemonCenter_1F.inc"
	.include "data/maps/events/RustboroCity_PokemonCenter_2F.inc"
	.include "data/maps/events/RustboroCity_Mart.inc"
	.include "data/maps/events/RustboroCity_Flat1_1F.inc"
	.include "data/maps/events/RustboroCity_Flat1_2F.inc"
	.include "data/maps/events/RustboroCity_House1.inc"
	.include "data/maps/events/RustboroCity_CuttersHouse.inc"
	.include "data/maps/events/RustboroCity_House2.inc"
	.include "data/maps/events/RustboroCity_Flat2_1F.inc"
	.include "data/maps/events/RustboroCity_Flat2_2F.inc"
	.include "data/maps/events/RustboroCity_Flat2_3F.inc"
	.include "data/maps/events/RustboroCity_House3.inc"
	.include "data/maps/events/FortreeCity_House1.inc"
	.include "data/maps/events/FortreeCity_Gym.inc"
	.include "data/maps/events/FortreeCity_PokemonCenter_1F.inc"
	.include "data/maps/events/FortreeCity_PokemonCenter_2F.inc"
	.include "data/maps/events/FortreeCity_Mart.inc"
	.include "data/maps/events/FortreeCity_House2.inc"
	.include "data/maps/events/FortreeCity_House3.inc"
	.include "data/maps/events/FortreeCity_House4.inc"
	.include "data/maps/events/FortreeCity_House5.inc"
	.include "data/maps/events/FortreeCity_DecorationShop.inc"
	.include "data/maps/events/LilycoveCity_CoveLilyMotel_1F.inc"
	.include "data/maps/events/LilycoveCity_CoveLilyMotel_2F.inc"
	.include "data/maps/events/LilycoveCity_LilycoveMuseum_1F.inc"
	.include "data/maps/events/LilycoveCity_LilycoveMuseum_2F.inc"
	.include "data/maps/events/LilycoveCity_ContestLobby.inc"
	.include "data/maps/events/LilycoveCity_ContestHall.inc"
	.include "data/maps/events/LilycoveCity_PokemonCenter_1F.inc"
	.include "data/maps/events/LilycoveCity_PokemonCenter_2F.inc"
	.include "data/maps/events/LilycoveCity_UnusedMart.inc"
	.include "data/maps/events/LilycoveCity_PokemonTrainerFanClub.inc"
	.include "data/maps/events/LilycoveCity_Harbor.inc"
	.include "data/maps/events/LilycoveCity_EmptyMap.inc"
	.include "data/maps/events/LilycoveCity_MoveDeletersHouse.inc"
	.include "data/maps/events/LilycoveCity_House1.inc"
	.include "data/maps/events/LilycoveCity_House2.inc"
	.include "data/maps/events/LilycoveCity_House3.inc"
	.include "data/maps/events/LilycoveCity_House4.inc"
	.include "data/maps/events/LilycoveCity_DepartmentStore_1F.inc"
	.include "data/maps/events/LilycoveCity_DepartmentStore_2F.inc"
	.include "data/maps/events/LilycoveCity_DepartmentStore_3F.inc"
	.include "data/maps/events/LilycoveCity_DepartmentStore_4F.inc"
	.include "data/maps/events/LilycoveCity_DepartmentStore_5F.inc"
	.include "data/maps/events/LilycoveCity_DepartmentStoreRooftop.inc"
	.include "data/maps/events/LilycoveCity_DepartmentStoreElevator.inc"
	.include "data/maps/events/MossdeepCity_Gym.inc"
	.include "data/maps/events/MossdeepCity_House1.inc"
	.include "data/maps/events/MossdeepCity_House2.inc"
	.include "data/maps/events/MossdeepCity_PokemonCenter_1F.inc"
	.include "data/maps/events/MossdeepCity_PokemonCenter_2F.inc"
	.include "data/maps/events/MossdeepCity_Mart.inc"
	.include "data/maps/events/MossdeepCity_House3.inc"
	.include "data/maps/events/MossdeepCity_StevensHouse.inc"
	.include "data/maps/events/MossdeepCity_House4.inc"
	.include "data/maps/events/MossdeepCity_SpaceCenter_1F.inc"
	.include "data/maps/events/MossdeepCity_SpaceCenter_2F.inc"
	.include "data/maps/events/MossdeepCity_GameCorner_1F.inc"
	.include "data/maps/events/MossdeepCity_GameCorner_B1F.inc"
	.include "data/maps/events/SootopolisCity_Gym_1F.inc"
	.include "data/maps/events/SootopolisCity_Gym_B1F.inc"
	.include "data/maps/events/SootopolisCity_PokemonCenter_1F.inc"
	.include "data/maps/events/SootopolisCity_PokemonCenter_2F.inc"
	.include "data/maps/events/SootopolisCity_Mart.inc"
	.include "data/maps/events/SootopolisCity_House1.inc"
	.include "data/maps/events/SootopolisCity_House2.inc"
	.include "data/maps/events/SootopolisCity_House3.inc"
	.include "data/maps/events/SootopolisCity_House4.inc"
	.include "data/maps/events/SootopolisCity_House5.inc"
	.include "data/maps/events/SootopolisCity_House6.inc"
	.include "data/maps/events/SootopolisCity_House7.inc"
	.include "data/maps/events/SootopolisCity_House8.inc"
	.include "data/maps/events/EverGrandeCity_SidneysRoom.inc"
	.include "data/maps/events/EverGrandeCity_PhoebesRoom.inc"
	.include "data/maps/events/EverGrandeCity_GlaciasRoom.inc"
	.include "data/maps/events/EverGrandeCity_DrakesRoom.inc"
	.include "data/maps/events/EverGrandeCity_ChampionsRoom.inc"
	.include "data/maps/events/EverGrandeCity_Corridor1.inc"
	.include "data/maps/events/EverGrandeCity_Corridor2.inc"
	.include "data/maps/events/EverGrandeCity_Corridor3.inc"
	.include "data/maps/events/EverGrandeCity_Corridor4.inc"
	.include "data/maps/events/EverGrandeCity_Corridor5.inc"
	.include "data/maps/events/EverGrandeCity_PokemonLeague.inc"
	.include "data/maps/events/EverGrandeCity_HallOfFame.inc"
	.include "data/maps/events/EverGrandeCity_PokemonCenter_1F.inc"
	.include "data/maps/events/EverGrandeCity_PokemonCenter_2F.inc"
	.include "data/maps/events/Route104_MrBrineysHouse.inc"
	.include "data/maps/events/Route104_PrettyPetalFlowerShop.inc"
	.include "data/maps/events/Route111_WinstrateFamilysHouse.inc"
	.include "data/maps/events/Route111_OldLadysRestStop.inc"
	.include "data/maps/events/Route112_CableCarStation.inc"
	.include "data/maps/events/MtChimney_CableCarStation.inc"
	.include "data/maps/events/Route114_FossilManiacsHouse.inc"
	.include "data/maps/events/Route114_FossilManiacsTunnel.inc"
	.include "data/maps/events/Route114_LanettesHouse.inc"
	.include "data/maps/events/Route116_TunnelersRestHouse.inc"
	.include "data/maps/events/Route117_PokemonDayCare.inc"
	.include "data/maps/events/Route121_SafariZoneEntrance.inc"
	.include "data/maps/events/MeteorFalls_1F_1R.inc"
	.include "data/maps/events/MeteorFalls_1F_2R.inc"
	.include "data/maps/events/MeteorFalls_B1F_1R.inc"
	.include "data/maps/events/MeteorFalls_B1F_2R.inc"
	.include "data/maps/events/RusturfTunnel.inc"
	.include "data/maps/events/Underwater_SootopolisCity.inc"
	.include "data/maps/events/DesertRuins.inc"
	.include "data/maps/events/GraniteCave_1F.inc"
	.include "data/maps/events/GraniteCave_B1F.inc"
	.include "data/maps/events/GraniteCave_B2F.inc"
	.include "data/maps/events/GraniteCave_StevensRoom.inc"
	.include "data/maps/events/PetalburgWoods.inc"
	.include "data/maps/events/MtChimney.inc"
	.include "data/maps/events/JaggedPass.inc"
	.include "data/maps/events/FieryPath.inc"
	.include "data/maps/events/MtPyre_1F.inc"
	.include "data/maps/events/MtPyre_2F.inc"
	.include "data/maps/events/MtPyre_3F.inc"
	.include "data/maps/events/MtPyre_4F.inc"
	.include "data/maps/events/MtPyre_5F.inc"
	.include "data/maps/events/MtPyre_6F.inc"
	.include "data/maps/events/MtPyre_Exterior.inc"
	.include "data/maps/events/MtPyre_Summit.inc"
	.include "data/maps/events/AquaHideout_1F.inc"
	.include "data/maps/events/AquaHideout_B1F.inc"
	.include "data/maps/events/AquaHideout_B2F.inc"
	.include "data/maps/events/Underwater_SeafloorCavern.inc"
	.include "data/maps/events/SeafloorCavern_Entrance.inc"
	.include "data/maps/events/SeafloorCavern_Room1.inc"
	.include "data/maps/events/SeafloorCavern_Room2.inc"
	.include "data/maps/events/SeafloorCavern_Room3.inc"
	.include "data/maps/events/SeafloorCavern_Room4.inc"
	.include "data/maps/events/SeafloorCavern_Room5.inc"
	.include "data/maps/events/SeafloorCavern_Room6.inc"
	.include "data/maps/events/SeafloorCavern_Room7.inc"
	.include "data/maps/events/SeafloorCavern_Room8.inc"
	.include "data/maps/events/SeafloorCavern_Room9.inc"
	.include "data/maps/events/CaveOfOrigin_Entrance.inc"
	.include "data/maps/events/CaveOfOrigin_1F.inc"
	.include "data/maps/events/CaveOfOrigin_B1F.inc"
	.include "data/maps/events/CaveOfOrigin_B2F.inc"
	.include "data/maps/events/CaveOfOrigin_B3F.inc"
	.include "data/maps/events/CaveOfOrigin_B4F.inc"
	.include "data/maps/events/VictoryRoad_1F.inc"
	.include "data/maps/events/VictoryRoad_B1F.inc"
	.include "data/maps/events/VictoryRoad_B2F.inc"
	.include "data/maps/events/ShoalCave_LowTideEntranceRoom.inc"
	.include "data/maps/events/ShoalCave_LowTideInnerRoom.inc"
	.include "data/maps/events/ShoalCave_LowTideStairsRoom.inc"
	.include "data/maps/events/ShoalCave_LowTideLowerRoom.inc"
	.include "data/maps/events/ShoalCave_HighTideEntranceRoom.inc"
	.include "data/maps/events/ShoalCave_HighTideInnerRoom.inc"
	.include "data/maps/events/NewMauville_Entrance.inc"
	.include "data/maps/events/NewMauville_Inside.inc"
	.include "data/maps/events/AbandonedShip_Deck.inc"
	.include "data/maps/events/AbandonedShip_Corridors_1F.inc"
	.include "data/maps/events/AbandonedShip_Rooms_1F.inc"
	.include "data/maps/events/AbandonedShip_Corridors_B1F.inc"
	.include "data/maps/events/AbandonedShip_Rooms_B1F.inc"
	.include "data/maps/events/AbandonedShip_Rooms2_B1F.inc"
	.include "data/maps/events/AbandonedShip_Underwater1.inc"
	.include "data/maps/events/AbandonedShip_Room_B1F.inc"
	.include "data/maps/events/AbandonedShip_Rooms2_1F.inc"
	.include "data/maps/events/AbandonedShip_CaptainsOffice.inc"
	.include "data/maps/events/AbandonedShip_Underwater2.inc"
	.include "data/maps/events/AbandonedShip_HiddenFloorCorridors.inc"
	.include "data/maps/events/AbandonedShip_HiddenFloorRooms.inc"
	.include "data/maps/events/IslandCave.inc"
	.include "data/maps/events/AncientTomb.inc"
	.include "data/maps/events/Underwater_Route134.inc"
	.include "data/maps/events/Underwater_SealedChamber.inc"
	.include "data/maps/events/SealedChamber_OuterRoom.inc"
	.include "data/maps/events/SealedChamber_InnerRoom.inc"
	.include "data/maps/events/ScorchedSlab.inc"
	.include "data/maps/events/MagmaHideout_1F.inc"
	.include "data/maps/events/MagmaHideout_B1F.inc"
	.include "data/maps/events/MagmaHideout_B2F.inc"
	.include "data/maps/events/SkyPillar_Entrance.inc"
	.include "data/maps/events/SkyPillar_Outside.inc"
	.include "data/maps/events/SkyPillar_1F.inc"
	.include "data/maps/events/SkyPillar_2F.inc"
	.include "data/maps/events/SkyPillar_3F.inc"
	.include "data/maps/events/SkyPillar_4F.inc"
	.include "data/maps/events/ShoalCave_LowTideIceRoom.inc"
	.include "data/maps/events/SkyPillar_5F.inc"
	.include "data/maps/events/SkyPillar_Top.inc"
	.include "data/maps/events/SecretBase_RedCave1.inc"
	.include "data/maps/events/SecretBase_BrownCave1.inc"
	.include "data/maps/events/SecretBase_BlueCave1.inc"
	.include "data/maps/events/SecretBase_YellowCave1.inc"
	.include "data/maps/events/SecretBase_Tree1.inc"
	.include "data/maps/events/SecretBase_Shrub1.inc"
	.include "data/maps/events/SecretBase_RedCave2.inc"
	.include "data/maps/events/SecretBase_BrownCave2.inc"
	.include "data/maps/events/SecretBase_BlueCave2.inc"
	.include "data/maps/events/SecretBase_YellowCave2.inc"
	.include "data/maps/events/SecretBase_Tree2.inc"
	.include "data/maps/events/SecretBase_Shrub2.inc"
	.include "data/maps/events/SecretBase_RedCave3.inc"
	.include "data/maps/events/SecretBase_BrownCave3.inc"
	.include "data/maps/events/SecretBase_BlueCave3.inc"
	.include "data/maps/events/SecretBase_YellowCave3.inc"
	.include "data/maps/events/SecretBase_Tree3.inc"
	.include "data/maps/events/SecretBase_Shrub3.inc"
	.include "data/maps/events/SecretBase_RedCave4.inc"
	.include "data/maps/events/SecretBase_BrownCave4.inc"
	.include "data/maps/events/SecretBase_BlueCave4.inc"
	.include "data/maps/events/SecretBase_YellowCave4.inc"
	.include "data/maps/events/SecretBase_Tree4.inc"
	.include "data/maps/events/SecretBase_Shrub4.inc"
	.include "data/maps/events/SingleBattleColosseum.inc"
	.include "data/maps/events/TradeCenter.inc"
	.include "data/maps/events/RecordCorner.inc"
	.include "data/maps/events/DoubleBattleColosseum.inc"
	.include "data/maps/events/LinkContestRoom1.inc"
	.include "data/maps/events/LinkContestRoom2.inc"
	.include "data/maps/events/LinkContestRoom3.inc"
	.include "data/maps/events/LinkContestRoom4.inc"
	.include "data/maps/events/LinkContestRoom5.inc"
	.include "data/maps/events/LinkContestRoom6.inc"
	.include "data/maps/events/UnknownMap_25_29.inc"
	.include "data/maps/events/UnknownMap_25_30.inc"
	.include "data/maps/events/UnknownMap_25_31.inc"
	.include "data/maps/events/UnknownMap_25_32.inc"
	.include "data/maps/events/UnknownMap_25_33.inc"
	.include "data/maps/events/UnknownMap_25_34.inc"
	.include "data/maps/events/InsideOfTruck.inc"
	.include "data/maps/events/SSTidalCorridor.inc"
	.include "data/maps/events/SSTidalLowerDeck.inc"
	.include "data/maps/events/SSTidalRooms.inc"
	.include "data/maps/events/SafariZone_Northwest.inc"
	.include "data/maps/events/SafariZone_Northeast.inc"
	.include "data/maps/events/SafariZone_Southwest.inc"
	.include "data/maps/events/SafariZone_Southeast.inc"
	.include "data/maps/events/BattleTower_Outside.inc"
	.include "data/maps/events/BattleTower_Lobby.inc"
	.include "data/maps/events/BattleTower_Elevator.inc"
	.include "data/maps/events/BattleTower_Corridor.inc"
	.include "data/maps/events/BattleTower_BattleRoom.inc"
	.include "data/maps/events/SouthernIsland_Exterior.inc"
	.include "data/maps/events/SouthernIsland_Interior.inc"
	.include "data/maps/events/SafariZone_RestHouse.inc"
	.include "data/maps/events/Route104_Prototype.inc"
	.include "data/maps/events/Route104_PrototypePrettyPetalFlowerShop.inc"
	.include "data/maps/events/Route109_SeashoreHouse.inc"
	.include "data/maps/events/Route110_TrickHouseEntrance.inc"
	.include "data/maps/events/Route110_TrickHouseEnd.inc"
	.include "data/maps/events/Route110_TrickHouseCorridor.inc"
	.include "data/maps/events/Route110_TrickHousePuzzle1.inc"
	.include "data/maps/events/Route110_TrickHousePuzzle2.inc"
	.include "data/maps/events/Route110_TrickHousePuzzle3.inc"
	.include "data/maps/events/Route110_TrickHousePuzzle4.inc"
	.include "data/maps/events/Route110_TrickHousePuzzle5.inc"
	.include "data/maps/events/Route110_TrickHousePuzzle6.inc"
	.include "data/maps/events/Route110_TrickHousePuzzle7.inc"
	.include "data/maps/events/Route110_TrickHousePuzzle8.inc"
	.include "data/maps/events/Route110_SeasideCyclingRoadSouthEntrance.inc"
	.include "data/maps/events/Route110_SeasideCyclingRoadNorthEntrance.inc"
	.include "data/maps/events/Route113_GlassWorkshop.inc"
	.include "data/maps/events/Route123_BerryMastersHouse.inc"
	.include "data/maps/events/Route119_WeatherInstitute_1F.inc"
	.include "data/maps/events/Route119_WeatherInstitute_2F.inc"
	.include "data/maps/events/Route119_House.inc"
	.include "data/maps/events/Route124_DivingTreasureHuntersHouse.inc"
