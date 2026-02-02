// Fill out your copyright notice in the Description page of Project Settings.

#include "GridSettings.h"

#include "GameplayTags/GEARS_GameplayTags.h"
#include "Materials/MaterialParameterCollection.h"

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

void UGridSettings::Update()
{
	float& OldSize = MPCSharedScalar.FindOrAdd(TAG_Grid_Cell_Size);
	OldSize = static_cast<float>(FMath::RoundUpToPowerOfTwo(FMath::Max(1, FMath::RoundToInt(OldSize))));
	RefreshFastAccessVariables();
	MPCSharedScalar.FindOrAdd(TAG_Grid_Cell_InvSize) = InvCellSize;
	if (MPC.IsNull()) return;
	const auto Mpc = MPC.LoadSynchronous();
	if (!Mpc) return;
	bool Updated = UpdateMPCParam(Mpc->ScalarParameters, MPCSharedScalar);
	if (UpdateMPCParam(Mpc->VectorParameters, MPCSharedLinearColor)) Updated = true;
	if (Updated)
	{
		Mpc->PostEditChange();
		if (!Mpc->MarkPackageDirty())
		{
			UE_LOG(LogTemp, Error, TEXT("MPC (%s) cannot be marked dirty !"), *Mpc->GetPathName());
		} else
		{
			UE_LOG(LogTemp, Display, TEXT("Updated MPC Grid : %s"), *Mpc->GetPathName());
		}
	}
	OnUpdated.Broadcast();
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

#endif

float UGridSettings::GetCellSize() const
{
	return CachedCellSize;
}

float UGridSettings::GetInvCellSize() const
{
	return InvCellSize;
}

void UGridSettings::RefreshFastAccessVariables()
{
	const auto Size = MPCSharedScalar.Find(TAG_Grid_Cell_Size);
	checkf(Size, TEXT("CellSize Missing in GridSettings !"));
	CachedCellSize = *Size;
	InvCellSize = 1.f / CachedCellSize;
}
