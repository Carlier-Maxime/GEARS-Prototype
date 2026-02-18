#pragma once
#include "GameplayTagContainer.h"

template<typename T, typename TDef>
concept DataWrapper = requires(T A) {
	{ A.Data } -> std::same_as<TDef&>;
	{ A.Data.Tag } -> std::same_as<FGameplayTag&>;
};

template<typename TDefinition, typename TWrapper, std::integral TIndexType = int32>
requires DataWrapper<TWrapper, TDefinition>
class TDataRegistry
{
public:
	TDataRegistry(const FString& RegistryName, const TArray<TSoftObjectPtr<TWrapper>>& InWrappedData)
	{
		Definitions.Reserve(InWrappedData.Num());
		TagToIndex.Reserve(InWrappedData.Num());

		for (auto& Soft : InWrappedData)
		{
			if (Definitions.Num() >= std::numeric_limits<TIndexType>::max())
			{
				UE_LOG(LogTemp, Fatal, TEXT("[%s] Registry overflow! Increase TIndexType size."), *RegistryName);
			}
			
			TWrapper* LoadedAsset = Soft.LoadSynchronous();
			if (!LoadedAsset)
			{
				UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load a DataAsset (Null or Invalid path)"), *RegistryName);
				continue;
			}
			
			auto& Data = LoadedAsset->Data;
			if (!Data.Tag.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("[%s] Asset skipped: Tag is 'None'. Check asset: %s"), 
					*RegistryName, *LoadedAsset->GetName());
				continue;
			}
			if (TagToIndex.Contains(Data.Tag))
			{
				UE_LOG(LogTemp, Error, TEXT("[%s] Duplicate Tag detected: %s"), *RegistryName, *Data.Tag.ToString());
				continue;
			}

			const TIndexType Index = static_cast<TIndexType>(Definitions.Add(Data));
			TagToIndex.Add(Data.Tag, Index);
          
			UE_LOG(LogTemp, Verbose, TEXT("[%s] Registered: %s at Index %d"), *RegistryName, *Data.Tag.ToString(), (int32)Index);
		}
       
		UE_LOG(LogTemp, Log, TEXT("[%s] Initialized with %d definitions."), *RegistryName, Definitions.Num());
	}
	~TDataRegistry() = default;
	
	[[nodiscard]] FORCEINLINE const TDefinition& operator[] (const TIndexType Index) const { return Definitions[Index]; }
	[[nodiscard]] FORCEINLINE int16 GetIndex(const FGameplayTag& Tag) const
	{
		const TIndexType* Found = TagToIndex.Find(Tag);
		return Found ? *Found : INVALID_INDEX;
	}
	[[nodiscard]] FORCEINLINE const TDefinition& operator[] (const FGameplayTag& Tag) const { return Definitions[TagToIndex.FindChecked(Tag)]; }
	[[nodiscard]] FORCEINLINE TIndexType Num() const { return Definitions.Num(); }

	static constexpr auto INVALID_INDEX = static_cast<TIndexType>(-1);
private:
	TArray<TDefinition> Definitions;
	TMap<FGameplayTag, TIndexType> TagToIndex;
};
