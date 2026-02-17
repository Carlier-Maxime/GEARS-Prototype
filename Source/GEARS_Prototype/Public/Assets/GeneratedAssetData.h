#pragma once

#include "CoreMinimal.h"
#include "GeneratedAssetData.generated.h"

USTRUCT()
struct GEARS_PROTOTYPE_API FGeneratedAssetData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Config")
	FString SubFolder = TEXT("/");

	UPROPERTY(EditAnywhere, Category = "Config")
	FString Name = TEXT("Asset_Generated_Name");
	
	UPROPERTY(Transient)
	mutable TObjectPtr<UObject> Asset = nullptr;
	
	FString GetPackagePath() const;
	FString GetAssetPath(const FString& PackagePath) const;
	FString GetAssetPath() const;
	
protected:
	inline static FString GeneratedFolderStr = TEXT("/Game/_Generated");
};