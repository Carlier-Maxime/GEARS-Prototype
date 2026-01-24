// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "GameplayTags/GEARS_GameplayTags.h"
#include "GridSettings.generated.h"

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
		{TAG_Grid_Cell_Size, 100},
		{TAG_Grid_Border_Thickness, 0.02}
	};
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	float GetCellSize() const;
	
#if WITH_EDITOR
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
private:
	void UpdateMPC();
#endif
};

