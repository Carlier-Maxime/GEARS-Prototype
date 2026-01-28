// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Extensions/UIComponent.h"
#include "StylizerBase.generated.h"

class UStyleData;
/**
 * 
 */
UCLASS(Abstract)
class GEARS_PROTOTYPE_API UStylizerBase : public UUIComponent
{
	GENERATED_BODY()
public:
	UStylizerBase();
	virtual void BeginDestroy() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void OnPreConstruct(bool bIsDesignTime) override;
	virtual UStyleData* GetStyleAsset() const {return nullptr;}
	template <typename T>
	T* GetWidget() {
		if (!GetOwner().IsValid()) return nullptr;
		const auto Widget = Cast<T>(GetOwner().Get());
		if (!Widget) {
			UE_LOG(LogTemp, Error, TEXT("Stylizer [%s]: Wrong Widget Type !"), *GetName());
			return nullptr;
		}
		return Widget;
	}
	UFUNCTION()
	virtual void ApplyStyle();
};
