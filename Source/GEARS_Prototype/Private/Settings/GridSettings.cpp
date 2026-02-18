// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/GridSettings.h"

#include "Settings/GridParams.h"
#include "Data/Wrappers/BiomeType.h"
#include "Data/Wrappers/ResourceType.h"

#include "GameplayTags/GEARS_GameplayTags.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void UGridSettings::PostInitProperties()
{
	Super::PostInitProperties();
	MPCAsset.Bind(GeneratedMPC);
	BiomeAtlas.Bind(GeneratedBiomeAtlas);
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

void UGridSettings::UpdateMPC()
{
	auto* MPC = MPCAsset.Get();
	bool Updated = UpdateMPCParam(MPC->ScalarParameters, MPCSharedScalar);
	if (UpdateMPCParam(MPC->VectorParameters, MPCSharedLinearColor)) Updated = true;
	if (Updated)
	{
		#if WITH_EDITOR
			MPC->PostEditChange();
			if (!MPC->MarkPackageDirty())
			{
				UE_LOG(LogTemp, Error, TEXT("MPC (%s) cannot be marked dirty !"), *MPC->GetPathName());
			} else
			{
				UE_LOG(LogTemp, Display, TEXT("Updated MPC Grid : %s"), *MPC->GetPathName());
			}
		#else
			if (GetWorld())
			{
				auto MPCInst = GetWorld()->GetParameterCollectionInstance(MPC);
				if (MPCInst) MPCInst->UpdateRenderState(true);
			}
		#endif
	}
}

void UGridSettings::Update()
{
	CellSize = static_cast<float>(FMath::RoundUpToPowerOfTwo(FMath::Max(1, FMath::RoundToInt(CellSize))));
	ChunkSize = FMath::RoundUpToPowerOfTwo(ChunkSize);
	SyncSharedParams();
	UpdateMPC();
	LoadSoftPtr();
	RefreshFastAccessVariables();
}

void UGridSettings::LoadSoftPtr()
{
	if (!GridSoftMesh.IsNull()) GridMesh = GridSoftMesh.LoadSynchronous();
	LoadRegistry(ResourceSoftRegistry, ResourceRegistry);
	LoadRegistry(BiomeSoftRegistry, BiomeRegistry);
}

template <typename T>
void UGridSettings::LoadRegistry(const TArray<TSoftObjectPtr<T>>& RegistrySoft, TArray<TObjectPtr<T>>& Registry)
{
	Registry.Empty();
	Registry.Reserve(RegistrySoft.Num());
	for (auto& Soft : RegistrySoft)
	{
		if (!Soft.IsNull()) Registry.Emplace(Soft.LoadSynchronous());
	}
}

void UGridSettings::RefreshFastAccessVariables()
{
	GridParams::Get().CellSize = CellSize;
	GridParams::Get().InvCellSize = 1.f / CellSize;
	GridParams::Get().ChunkSize = ChunkSize;
	GridParams::Get().ChunkSizeSquared = ChunkSize * ChunkSize;
	GridParams::Get().ChunkShift = FMath::FloorLog2(ChunkSize);
	GridParams::Get().ChunkMask = ChunkSize - 1;
	GridParams::Get().ResourceRegistry = ResourceRegistry;
	GridParams::Get().BiomeRegistry = BiomeRegistry;
	GridParams::Get().Temperature = Temperature;
	GridParams::Get().Humidity = Humidity;
	GridParams::Get().GridMesh = GridMesh;
	GridParams::Get().BiomeAtlas = BiomeAtlas.Get();
}

void UGridSettings::SyncSharedParams()
{
	MPCSharedScalar.Add(TAG_Grid_Chunk_Size, ChunkSize);
	
	MPCSharedScalar.Add(TAG_Grid_Cell_Size, CellSize);
	MPCSharedScalar.Add(TAG_Grid_Cell_InvSize, 1.f / CellSize);
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
			const auto Str = DesiredName.ToString();
			Param->Id = FGuid::NewDeterministicGuid(FStringView(Str));
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
