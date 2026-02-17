// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Assets/GeneratedAsset.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Materials/MaterialParameterCollection.h"
#include "GridSettings.generated.h"

class UBiomeType;
class UResourceType;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig)
class GEARS_PROTOTYPE_API UGridSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = "Grid")
	TSoftObjectPtr<UStaticMesh> GridSoftMesh;
	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> GridMesh;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid")
	FGeneratedAssetData GeneratedMPC;
	TGeneratedAsset<UMaterialParameterCollection> MPCAsset;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid")
	uint32 ChunkSize = 16;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid|Cell")
	float CellSize = 128;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Cell")
	float CellSmallFactor = 0.1f;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Cell")
	float CellBigFactor = 10.f;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Cell")
	FLinearColor CellColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid|Border")
	float BorderThickness = 0.02f;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Border")
	FLinearColor BorderColor = FLinearColor::Black;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid|Transition")
	float TransitionLengthFactor = 10.f;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Transition")
	float TransitionSmallCellCountThreshold = 5;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Transition")
	float TransitionBigCellCountThreshold = 100;
	
	virtual void PostInitProperties() override;
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	void UpdateMPC();
private:
	void Update();
	void LoadSoftPtr();
	template <typename T>
	static void LoadRegistry(const TArray<TSoftObjectPtr<T>>& RegistrySoft, TArray<TObjectPtr<T>>& Registry);
	void RefreshFastAccessVariables() const;
	void SyncSharedParams();
	template <typename FCollectionParameterType, typename FValueType>
	static bool UpdateMPCParam(TArray<FCollectionParameterType>& MPCParams, const TMap<FGameplayTag, FValueType>& SharedParams);
	
	TMap<FGameplayTag, float> MPCSharedScalar;
	TMap<FGameplayTag, FLinearColor> MPCSharedLinearColor;
	
	UPROPERTY(EditAnywhere, config, Category = "Resources", meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UResourceType>> ResourceSoftRegistry;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UResourceType>> ResourceRegistry;
	
	UPROPERTY(EditAnywhere, config, Category = "Biomes", meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UBiomeType>> BiomeSoftRegistry;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UBiomeType>> BiomeRegistry;
	
	UPROPERTY(EditAnywhere, config, Category = "Biomes", meta = (AllowPrivateAccess = true))
	FNoiseContext Temperature;
	UPROPERTY(EditAnywhere, config, Category = "Biomes", meta = (AllowPrivateAccess = true))
	FNoiseContext Humidity;
};

