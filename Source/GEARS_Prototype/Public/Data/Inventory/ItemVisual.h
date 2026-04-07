#pragma once

#include "ItemVisual.generated.h"

USTRUCT()
struct FItemVisual
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSoftObjectPtr<UStaticMesh> WorldMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TArray<TSoftObjectPtr<UMaterialInterface>> MaterialOverrides;
};