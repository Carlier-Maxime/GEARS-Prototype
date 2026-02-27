#pragma once

#include "CoreMinimal.h"
#include "NoisePreviewContext.h"
#include "Grid/Types.h"

struct FNoisePreviewState
{
	struct FPixelWriteContext
	{	
		FPixelWriteContext(const int32 X, const int32 Y, const int32 Res, const int32 Step) : X(X), Y(Y), Res(Res), Step(Step) {}
		
		FORCEINLINE FIntPoint ToTextureCoord() const {
			return {X, Y};
		}
		
		FORCEINLINE FIntPoint ToScaledCoord() const
		{
			return {X * Step, Y * Step};
		}
		
		FORCEINLINE FWorldGridPos ToWorld() const
		{
			return {
				(X - (Res >> 1)) * Step,
				(Y - (Res >> 1)) * Step
			};
		}
		
		FORCEINLINE FVector2D ToNormalizedUV(const bool bInvY = true, const bool bInvX = false) const
		{
			const float Divisor = Res>1 ? static_cast<float>(Res - 1) : 1.f;
			FVector2D Ret{X/Divisor, Y/Divisor};
			if (bInvY) Ret.Y = 1.f - Ret.Y;
			if (bInvX) Ret.X = 1.f - Ret.X;
			return Ret;
		}
		
		FORCEINLINE FVector2D ToSampledUV() const
		{
			return {
				(X + 0.5f) / Res,
				(Y + 0.5f) / Res
			};
		}
			
		private: 
			int32 X, Y, Res, Step;
	};
	
	using FGenColorFn = TFunction<FColor(FPixelWriteContext)>;

	FNoisePreviewState();
	
	TArray<FColor> PixelBuffer;
	TStrongObjectPtr<UTexture2D> Texture = nullptr;
	FSlateBrush Brush;
	FGenColorFn GenerateColorFn;
	
	void Update(const FNoisePreviewContext& Ctx);
};
