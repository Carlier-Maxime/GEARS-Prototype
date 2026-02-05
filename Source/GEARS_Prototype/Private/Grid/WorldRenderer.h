// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldRenderer.generated.h"

class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class GEARS_PROTOTYPE_API AWorldRenderer : public AActor
{
	GENERATED_BODY()

public:
	AWorldRenderer();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HISMs", meta=(AllowPrivateAccess = true))
	TArray<TObjectPtr<UHierarchicalInstancedStaticMeshComponent>> ResourcesComponents;
};
