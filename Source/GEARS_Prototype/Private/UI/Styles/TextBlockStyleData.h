// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Framework/Styling/TextBlockWidgetStyle.h"
#include "TextBlockStyleData.generated.h"

/**
 * 
 */
UCLASS()
class GEARS_PROTOTYPE_API UTextBlockStyleData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FSlateFontInfo Font;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FLinearColor ColorAndOpacity = FLinearColor::White;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FVector2D ShadowOffset = FVector2D::ZeroVector;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FLinearColor ShadowColorAndOpacity = FLinearColor::Black;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FSlateBrush StrikeBrush;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    ETextTransformPolicy TransformPolicy = ETextTransformPolicy::None;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    ETextOverflowPolicy OverflowPolicy = ETextOverflowPolicy::Clip;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    TEnumAsByte<ETextJustify::Type> Justification = ETextJustify::Left;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    bool AutoWrap = false;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FMargin Margin = FMargin(0.f);
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    float LineHeightPercentage = 1.f;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FLinearColor SelectedBackgroundColor = FLinearColor::Transparent;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FLinearColor HighlightColor = FLinearColor::Transparent;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FSlateBrush UnderlineBrush;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style")
    FSlateBrush HighlightBrush;
};
