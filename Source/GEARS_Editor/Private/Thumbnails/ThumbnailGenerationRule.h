#pragma once

#include "GameplayTagContainer.h"
#include "ThumbnailGenerationRule.generated.h"

USTRUCT(BlueprintType)
struct FThumbnailGenerationRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(ContentDir))
	FDirectoryPath SourcePath;

	UPROPERTY(EditAnywhere)
	FString SubFolderDest;

	UPROPERTY(EditAnywhere)
	bool bRecursive = true;
	
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer AdditionalThemes;
};