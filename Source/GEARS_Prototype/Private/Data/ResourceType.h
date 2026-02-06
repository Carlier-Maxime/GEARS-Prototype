// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ResourceType.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UResourceType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FGameplayTag ResourceTag;
	
	UPROPERTY(EditAnywhere, Category = "Visuals")
	TObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditAnywhere, Category = "Generation", meta = (ClampMin = 0, ClampMax = 1))
	float NoiseThreshold = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Generation", meta = (ClampMin = 0, ClampMax = 1))
	float NoiseScale = 0.05f;

	virtual void PostInitProperties() override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Generation|Preview")
	TObjectPtr<UTexture2D> NoisePreview;
	
	UPROPERTY(EditDefaultsOnly, Category = "Generation|Preview", meta=(ClampMin = 8))
	int32 PreviewResolution = 256;
	
	UPROPERTY(EditDefaultsOnly, Category = "Generation|Preview", meta=(UIMin = 0, UIMax = 10000))
	int32 SeedPreview = 0;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void UpdatePreview();
#endif
};
