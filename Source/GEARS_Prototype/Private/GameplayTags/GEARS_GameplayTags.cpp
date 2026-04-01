#include "GameplayTags/GEARS_GameplayTags.h"

#pragma region grid
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Chunk_Size, "Grid.Chunk.Size")

UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Cell_Size, "Grid.Cell.Size")
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Cell_InvSize, "Grid.Cell.invSize")
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Cell_Small_Factor, "Grid.Cell.Small.Factor")
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Cell_Big_Factor, "Grid.Cell.Big.Factor")
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Cell_Color, "Grid.Cell.Color")

UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Border_Color, "Grid.Border.Color")
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Border_Thickness, "Grid.Border.Thickness")

UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Transition_Length_Factor, "Grid.Transition.Length.Factor")
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Transition_Small_CellCountThreshold, "Grid.Transition.Small.CellCountThreshold")
UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Transition_Big_CellCountThreshold, "Grid.Transition.Big.CellCountThreshold")

UE_DEFINE_GAMEPLAY_TAG(TAG_Grid_Biome_Chunk_Factor, "Grid.Biome.Chunk.Factor")
#pragma endregion

#pragma region biome
UE_DEFINE_GAMEPLAY_TAG(TAG_Biome, "Biome")
UE_DEFINE_GAMEPLAY_TAG(TAG_Biome_Forest_Autumn, "Biome.Forest.Autumn")
UE_DEFINE_GAMEPLAY_TAG(TAG_Biome_Forest_Deciduous, "Biome.Forest.Deciduous")
UE_DEFINE_GAMEPLAY_TAG(TAG_Biome_Forest_Coniferous, "Biome.Forest.Coniferous")
UE_DEFINE_GAMEPLAY_TAG(TAG_Biome_Forest_Dead, "Biome.Forest.Dead")

UE_DEFINE_GAMEPLAY_TAG(TAG_Biome_Meadow_Lush, "Biome.Meadow.Lush")
UE_DEFINE_GAMEPLAY_TAG(TAG_Biome_Meadow_Arid, "Biome.Meadow.Arid")

UE_DEFINE_GAMEPLAY_TAG(TAG_Biome_Desert_Rocky, "Biome.Desert.Rocky")

UE_DEFINE_GAMEPLAY_TAG(TAG_Biome_Jungle_Edge, "Biome.Jungle.Edge")
#pragma endregion

#pragma region resource
UE_DEFINE_GAMEPLAY_TAG(TAG_Resource, "Resource")

UE_DEFINE_GAMEPLAY_TAG(TAG_Resource_Organic_Wood, "Resource.Organic.Wood")
UE_DEFINE_GAMEPLAY_TAG(TAG_Resource_Organic_Biomass, "Resource.Organic.Biomass")

UE_DEFINE_GAMEPLAY_TAG(TAG_Resource_Mineral_Stone, "Resource.Mineral.Stone")
UE_DEFINE_GAMEPLAY_TAG(TAG_Resource_Mineral_Coal, "Resource.Mineral.Coal")
UE_DEFINE_GAMEPLAY_TAG(TAG_Resource_Mineral_Iron, "Resource.Mineral.Iron")
UE_DEFINE_GAMEPLAY_TAG(TAG_Resource_Mineral_Copper, "Resource.Mineral.Copper")
UE_DEFINE_GAMEPLAY_TAG(TAG_Resource_Mineral_Gold, "Resource.Mineral.Gold")
#pragma endregion

#pragma region items
UE_DEFINE_GAMEPLAY_TAG(TAG_Item, "Item")
UE_DEFINE_GAMEPLAY_TAG(TAG_Item_None, "Item.None")
#pragma endregion

#pragma region ability
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Move_Pathfinding, "Ability.Move.Pathfinding")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Interact, "Ability.Interact")
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Interact_Harvest, "Ability.Interact.Harvest")
#pragma endregion

#pragma region state
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Moving, "State.Moving")
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Moving_Manual, "State.Moving.Manual")
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Moving_Auto, "State.Moving.Auto")
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Interacting, "State.Interacting")
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Harvesting, "State.Harvesting")
#pragma endregion

#pragma region cue
UE_DEFINE_GAMEPLAY_TAG(TAG_Cue_Move_Confirm, "GameplayCue.Move.Confirm")
UE_DEFINE_GAMEPLAY_TAG(TAG_Cue_Damage, "GameplayCue.Damage")
UE_DEFINE_GAMEPLAY_TAG(TAG_Cue_Damage_Hit, "GameplayCue.Damage.Hit")
UE_DEFINE_GAMEPLAY_TAG(TAG_Cue_Damage_Destroyed, "GameplayCue.Damage.Destroyed")
#pragma endregion

#pragma region event
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Movement_Arrived, "Event.Movement.Arrived")
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Movement_Cancel, "Event.Movement.Cancel")
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Interact_Execute, "Event.Interact.Execute")
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Harvest_Impact, "Event.Harvest.Impact")
#pragma endregion

#pragma region IK
UE_DEFINE_GAMEPLAY_TAG(TAG_IK, "IK")
UE_DEFINE_GAMEPLAY_TAG(TAG_IK_Arm, "IK.Arm")
UE_DEFINE_GAMEPLAY_TAG(TAG_IK_Arm_Left, "IK.Arm.Left")
UE_DEFINE_GAMEPLAY_TAG(TAG_IK_Arm_Right, "IK.Arm.Right")
#pragma endregion