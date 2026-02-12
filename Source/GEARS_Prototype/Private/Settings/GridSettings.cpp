// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/GridSettings.h"

#include "Settings/GridParams.h"
#include "GameplayTags/GEARS_GameplayTags.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void UGridSettings::PostInitProperties()
{
	Super::PostInitProperties();
	Update();
}

void UGridSettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);
	Update();
}

#if WITH_EDITOR

void UGridSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Update();
}

#endif

void UGridSettings::Update()
{
	CellSize = static_cast<float>(FMath::RoundUpToPowerOfTwo(FMath::Max(1, FMath::RoundToInt(CellSize))));
	ChunkSize = FMath::RoundUpToPowerOfTwo(ChunkSize);
	RefreshFastAccessVariables();
	SyncSharedParams();
	if (MPC.IsNull()) return;
	const auto Mpc = MPC.LoadSynchronous();
	if (!Mpc) return;
	bool Updated = UpdateMPCParam(Mpc->ScalarParameters, MPCSharedScalar);
	if (UpdateMPCParam(Mpc->VectorParameters, MPCSharedLinearColor)) Updated = true;
	if (Updated)
	{
		#if WITH_EDITOR
		Mpc->PostEditChange();
		if (!Mpc->MarkPackageDirty())
		{
			UE_LOG(LogTemp, Error, TEXT("MPC (%s) cannot be marked dirty !"), *Mpc->GetPathName());
		} else
		{
			UE_LOG(LogTemp, Display, TEXT("Updated MPC Grid : %s"), *Mpc->GetPathName());
		}
		#else
		if (GetWorld())
		{
			auto MPCInst = GetWorld()->GetParameterCollectionInstance(Mpc);
			if (MPCInst) MPCInst->UpdateRenderState(true);
		}
		#endif
	}
}

void UGridSettings::RefreshFastAccessVariables() const
{
	GridParams::Get().CellSize = CellSize;
	GridParams::Get().InvCellSize = 1.f / CellSize;
	GridParams::Get().ChunkSize = ChunkSize;
	GridParams::Get().ChunkSizeSquared = ChunkSize * ChunkSize;
	GridParams::Get().ChunkShift = FMath::FloorLog2(ChunkSize);
	GridParams::Get().ChunkMask = ChunkSize - 1;
	GridParams::Get().ResourceRegistry = ResourceRegister;
	GridParams::Get().BiomeRegistry = BiomeRegistry;
	GridParams::Get().Temperature = Temperature;
	GridParams::Get().Humidity = Humidity;
}

void UGridSettings::SyncSharedParams()
{
	MPCSharedScalar.Add(TAG_Grid_Chunk_Size, ChunkSize);
	
	MPCSharedScalar.Add(TAG_Grid_Cell_Size, CellSize);
	MPCSharedScalar.Add(TAG_Grid_Cell_InvSize, GridParams::Get().InvCellSize);
	MPCSharedScalar.Add(TAG_Grid_Cell_Big_Factor, CellBigFactor);
	MPCSharedScalar.Add(TAG_Grid_Cell_Small_Factor, CellSmallFactor);
	MPCSharedLinearColor.Add(TAG_Grid_Cell_Color, CellColor);
	
	MPCSharedScalar.Add(TAG_Grid_Border_Thickness, BorderThickness);
	MPCSharedLinearColor.Add(TAG_Grid_Border_Color, BorderColor);
	
	MPCSharedScalar.Add(TAG_Grid_Transition_Length_Factor, TransitionLengthFactor);
	MPCSharedScalar.Add(TAG_Grid_Transition_Small_CellCountThreshold, TransitionSmallCellCountThreshold);
	MPCSharedScalar.Add(TAG_Grid_Transition_Big_CellCountThreshold, TransitionBigCellCountThreshold);
}

template <typename FCollectionParameterType, typename FValueType>
bool UGridSettings::UpdateMPCParam(TArray<FCollectionParameterType>& MPCParams, const TMap<FGameplayTag, FValueType>& SharedParams)
{
	bool bUpdated = false;
	for (const auto& [Tag, DesiredValue] : SharedParams)
	{
		const FName DesiredName = Tag.GetTagName();
		auto Param = MPCParams.FindByPredicate([&](const FCollectionParameterType& Item) {
			return Item.ParameterName == DesiredName;
		});
		if (!Param)
		{
			Param = &MPCParams.AddDefaulted_GetRef();
			Param->ParameterName = DesiredName;
			bUpdated = true;
		}

		if (Param->DefaultValue != DesiredValue)
		{
			Param->DefaultValue = DesiredValue;
			bUpdated = true;
		}
	}
	return bUpdated;
}
