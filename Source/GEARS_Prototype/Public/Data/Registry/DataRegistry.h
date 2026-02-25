#pragma once
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"

template<typename T, typename TDef>
concept DataWrapper = requires(T A) {
	{ T::StaticClass() } -> std::convertible_to<UClass*>;
	requires std::derived_from<T, UObject>;
	{ A.Data } -> std::same_as<TDef&>;
	{ A.Data.Tag } -> std::same_as<FGameplayTag&>;
};

template<typename TDefinition, typename TWrapper, std::integral TIndexType = int32>
requires DataWrapper<TWrapper, TDefinition>
class TDataRegistry
{
public:
	TDataRegistry(const FString& InRegistryName, const FName& Path);
	TDataRegistry(const FString& InRegistryName, const TArray<TSoftObjectPtr<TWrapper>>& InWrappedData);
	~TDataRegistry();
	
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
	void Reserve(int32 Num);
	void RegisterAsset(TWrapper* Asset);
	void FinalizeInitialization();
	
	FString RegistryName;
	TArray<TDefinition> Definitions;
	TMap<FGameplayTag, TIndexType> TagToIndex;
#if WITH_EDITOR
	FDelegateHandle OnAnyTWrapperChangedHandle;
#endif
};

template <typename TDefinition, typename TWrapper, std::integral TIndexType> requires DataWrapper<TWrapper, TDefinition>
TDataRegistry<TDefinition, TWrapper, TIndexType>::TDataRegistry(const FString& InRegistryName, const FName& Path) : RegistryName(InRegistryName)
{
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	TArray<FAssetData> Assets;
	AssetRegistry.ScanPathsSynchronous({Path.ToString()});
	AssetRegistry.GetAssetsByPath(Path, Assets, true);

	Reserve(Assets.Num());
	for (const auto& Asset : Assets) RegisterAsset(Cast<TWrapper>(Asset.GetAsset()));
	FinalizeInitialization();
}

template<typename TDefinition, typename TWrapper, std::integral TIndexType = int32> requires DataWrapper<TWrapper, TDefinition>
TDataRegistry<TDefinition, TWrapper, TIndexType>::TDataRegistry(const FString& InRegistryName, const TArray<TSoftObjectPtr<TWrapper>>& InWrappedData) : RegistryName(InRegistryName)
{
	Reserve(InWrappedData.Num());
	for (auto& Soft : InWrappedData) RegisterAsset(Soft.LoadSynchronous());
	FinalizeInitialization();
}

template <typename TDefinition, typename TWrapper, std::integral TIndexType> requires DataWrapper<TWrapper, TDefinition>
void TDataRegistry<TDefinition, TWrapper, TIndexType>::Reserve(int32 Num)
{
	Definitions.Reserve(Num);
	TagToIndex.Reserve(Num);
}

template <typename TDefinition, typename TWrapper, std::integral TIndexType> requires DataWrapper<TWrapper, TDefinition>
void TDataRegistry<TDefinition, TWrapper, TIndexType>::RegisterAsset(TWrapper* Asset)
{
	if (!Asset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load a DataAsset (Null or Invalid path)"), *RegistryName);
		return;
	}
	if (Definitions.Num() >= std::numeric_limits<TIndexType>::max())
	{
		UE_LOG(LogTemp, Fatal, TEXT("[%s] Registry overflow! Increase TIndexType size."), *RegistryName);
	}
	auto& Data = Asset->Data;
	if (!Data.Tag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Asset skipped: Tag is 'None'. Check asset: %s"), 
			*RegistryName, *Asset->GetName());
		return;
	}
	if (TagToIndex.Contains(Data.Tag))
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Duplicate Tag detected: %s"), *RegistryName, *Data.Tag.ToString());
		return;
	}

	const TIndexType Index = static_cast<TIndexType>(Definitions.Add(Data));
	TagToIndex.Add(Data.Tag, Index);
          
	UE_LOG(LogTemp, Verbose, TEXT("[%s] Registered: %s at Index %lld"), *RegistryName, *Data.Tag.ToString(), (int64)Index);
}

template <typename TDefinition, typename TWrapper, std::integral TIndexType> requires DataWrapper<TWrapper, TDefinition>
void TDataRegistry<TDefinition, TWrapper, TIndexType>::FinalizeInitialization()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Initialized with %d definitions."), *RegistryName, Definitions.Num());
	
#if WITH_EDITOR
	OnAnyTWrapperChangedHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddLambda([this](UObject* Object, FPropertyChangedEvent& Event)
	{
		if (Event.ChangeType == EPropertyChangeType::Interactive) return;
		if (!Object->IsA(TWrapper::StaticClass())) return;
		const auto& Data = static_cast<TWrapper*>(Object)->Data;
		if (!Data.Tag.IsValid()) return;
		const auto Index = GetIndex(Data.Tag);
		if (Index == INVALID_INDEX) return;
		Definitions[Index] = Data;
		UE_LOG(LogTemp, Verbose, TEXT("[%s] Updated definition of %s at Index %lld"), *RegistryName, *Data.Tag.ToString(), (int64)Index);
	});
#endif
}

template <typename TDefinition, typename TWrapper, std::integral TIndexType> requires DataWrapper<TWrapper, TDefinition>
TDataRegistry<TDefinition, TWrapper, TIndexType>::~TDataRegistry()
{
#if WITH_EDITOR
	FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(OnAnyTWrapperChangedHandle);
#endif
}
