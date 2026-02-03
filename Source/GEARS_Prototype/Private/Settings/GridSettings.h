// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Definitions/GEARS_Delegates.h"
#include "GameplayTags/GEARS_GameplayTags.h"
#include "GridSettings.generated.h"

class UResourceType;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig)
class UGridSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = "Grid")
	TSoftObjectPtr<UMaterialParameterCollection> MPC;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid", meta = (ReadOnlyKeys))
	TMap<FGameplayTag, float> MPCSharedScalar = {
		{TAG_Grid_Cell_Size, 128},
		{TAG_Grid_Cell_InvSize, 0},
		{TAG_Grid_Cell_Small_Factor, 0.1},
		{TAG_Grid_Cell_Big_Factor, 10},
		{TAG_Grid_Border_Thickness, 0.02},
		{TAG_Grid_Transition_Length_Factor, 10},
		{TAG_Grid_Transition_Small_CellCountThreshold, 5},
		{TAG_Grid_Transition_Big_CellCountThreshold, 100}
	};
	
	UPROPERTY(EditAnywhere, config, Category = "Grid", meta = (ReadOnlyKeys))
	TMap<FGameplayTag, FLinearColor> MPCSharedLinearColor = {
		{TAG_Grid_Cell_Color, {1, 1, 1}},
		{TAG_Grid_Border_Color, {0.1, 0.1, 0.1}}
	};
	
	FOnVoidChanged OnUpdated;
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	float GetCellSize() const;
	UFUNCTION(BlueprintCallable, Category = "Grid")
	float GetInvCellSize() const;
	
	virtual void PostInitProperties() override;
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:
	void Update();
	template <typename FCollectionParameterType, typename FValueType>
	static bool UpdateMPCParam(TArray<FCollectionParameterType>& MPCParams, const TMap<FGameplayTag, FValueType>& SharedParams);
	void RefreshFastAccessVariables();
	
	float CachedCellSize;
	float InvCellSize;
	
	UPROPERTY(EditAnywhere, config, Category = "Resources", meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UResourceType>> ResourceRegister;
};

