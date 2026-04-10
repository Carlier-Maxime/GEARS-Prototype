#pragma once

#include "GameplayTagContainer.h"
#include "ThumbnailMaterialTheme.generated.h"

USTRUCT(BlueprintType)
struct FThumbnailMaterialTheme
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Identity")
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, Category="Name")
	bool bUseCustomName = false;
	
protected:
	UPROPERTY(EditAnywhere, Category="Name", meta=(EditCondition = bUseCustomName))
	FName Name;
	
public:
	UPROPERTY(EditAnywhere, Category="Materials")
	TArray<TSoftObjectPtr<UMaterialInstance>> MaterialsOverrides;
	
	inline FString GetName() const;
};

inline FString FThumbnailMaterialTheme::GetName() const
{
	return (bUseCustomName && Name.GetStringLength()>0) ? Name.ToString() :  Tag.ToString().Replace(TEXT("."), TEXT("_"));
}
