// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Assets/GeneratedAsset.h"
#include "Grid/Generator/Context/NoiseContext.h"
#include "Materials/MaterialParameterCollection.h"
#include "Engine/Texture2DArray.h"
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
	UPROPERTY(EditAnywhere, config, Category = "Grid", meta=(ToolTip="The Static Mesh plane for the grid floor"))
	TSoftObjectPtr<UStaticMesh> GridSoftMesh;
	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> GridMesh;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid")
	FGeneratedAssetData GeneratedMPC;
	TGeneratedAsset<UMaterialParameterCollection> MPCAsset;

	UPROPERTY(EditAnywhere, Config, Category = "Grid")
	FGeneratedAssetData GeneratedBiomeIndexMap;
	TGeneratedAsset<UTexture2DArray> BiomeIndexMap;
	
	UPROPERTY(EditAnywhere, Config, Category = "Grid")
	FGeneratedAssetData GeneratedBiomeAtlas;
	TGeneratedAsset<UTexture2DArray> BiomeAtlas;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid", meta=(ClampMin=4, UIMin=16, UIMax=256, ToolTip="Size of the chunk in cell units (see below for cell size)"))
	uint32 ChunkSize = 16;
	UPROPERTY(EditAnywhere, config, Category = "Grid", meta=(ClampMin=0, UIMin=4, UIMax=32, ToolTip="Size of the map in chunk radius format"))
	uint16 MapRadius = 8;
	UPROPERTY(EditAnywhere, config, Category = "Grid", meta=(ClampMin=1, UIMin=4, UIMax=64, ToolTip="Number of chunks per side mapped to a single Texture 2D Array slice. A value of 8 means one slice covers an 8x8 grid of chunks."))
	uint16 BiomeChunkFactor = 8;
	UPROPERTY(EditAnywhere, config, Category = "Grid", meta=(ToolTip="Select a specific seed for debug, keep -1 for random seed."))
	int32 DebugSeed = -1;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid|Cell", meta=(ClampMin=1, UIMin=16, UIMax=256, ToolTip="Size of a cell in world units (uu)"))
	float CellSize = 128;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Cell", meta=(ClampMin=0, ClampMax=1, ToolTip="Factor to multiply the cell size with to get the small cell size"))
	float CellSmallFactor = 0.1f;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Cell", meta=(ClampMin=1, UIMax=128, ToolTip="Factor to multiply the cell size with to get the big cell size"))
	float CellBigFactor = 10.f;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Cell")
	FLinearColor CellColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid|Border", meta=(ClampMin=0, UIMax=0.3, ClampMax=1, ToolTip="Thickness of the border in percentage of cell"))
	float BorderThickness = 0.02f;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Border")
	FLinearColor BorderColor = FLinearColor::Black;
	
	UPROPERTY(EditAnywhere, config, Category = "Grid|Transition", meta=(ClampMin=1, UIMax=128, ToolTip="Factor to multiply the cell size with to get the transition length"))
	float TransitionLengthFactor = 10.f;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Transition", meta=(ClampMin=1, UIMax=32, ToolTip="Threshold for the small cell count to trigger the transition"))
	float TransitionSmallCellCountThreshold = 5;
	UPROPERTY(EditAnywhere, config, Category = "Grid|Transition", meta=(ClampMin=16, UIMax=128, ToolTip="Threshold for the big cell count to trigger the transition"))
	float TransitionBigCellCountThreshold = 100;
	
	virtual void PostInitProperties() override;
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	void UpdateMPC();
private:
	void Update();
	void RefreshFastAccessVariables();
	void SyncSharedParams();
	template <typename FCollectionParameterType, typename FValueType>
	static bool UpdateMPCParam(TArray<FCollectionParameterType>& MPCParams, const TMap<FGameplayTag, FValueType>& SharedParams);
	
	TMap<FGameplayTag, float> MPCSharedScalar;
	TMap<FGameplayTag, FLinearColor> MPCSharedLinearColor;
	
	UPROPERTY(EditAnywhere, config, Category = "Resources", meta=(AllowPrivateAccess = true))
	FName ResourcesPath;
	
	UPROPERTY(EditAnywhere, config, Category = "Biomes", meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UBiomeType>> BiomeSoftRegistry;
	
	UPROPERTY(EditAnywhere, config, Category = "Biomes", meta = (AllowPrivateAccess = true))
	FNoiseContext Temperature;
	UPROPERTY(EditAnywhere, config, Category = "Biomes", meta = (AllowPrivateAccess = true))
	FNoiseContext Humidity;
};

