#pragma once

#include "CoreMinimal.h"
#include "GeneratedAssetData.h"

DECLARE_DELEGATE_TwoParams(FOnRequestAssetSync, const FGeneratedAssetData&, UClass*);

struct GEARS_PROTOTYPE_API GeneratedAsset
{
	inline static FOnRequestAssetSync OnRequestCreateAssetSync;
};

template <typename T>
struct TGeneratedAsset
{
	TGeneratedAsset() = default;
	~TGeneratedAsset() = default;
	
	void Bind(const FGeneratedAssetData& InData);
	FORCEINLINE T* Get() {return Data ? Cast<T>(Data->Asset) : nullptr;}
	FORCEINLINE bool IsValid() const {return Data && Data->Asset;}
	void ForceLoad();
	
	const FGeneratedAssetData* Data = nullptr;

private:
	void LoadAsset();
};

template <typename T>
void TGeneratedAsset<T>::Bind(const FGeneratedAssetData& InData)
{
	this->Data = &InData;
	ForceLoad();
}

template <typename T>
void TGeneratedAsset<T>::LoadAsset()
{
	if (!Data || Data->Name.IsEmpty()) return;
	const auto Path = Data->GetAssetPath();
	if (!FPackageName::DoesPackageExist(Path)) return; 
	Data->Asset = StaticLoadObject(T::StaticClass(), nullptr, Path);
}

template <typename T>
void TGeneratedAsset<T>::ForceLoad()
{
	if (!Data) return;
	if (!Data->Asset) LoadAsset();
	if (!Data->Asset) GeneratedAsset::OnRequestCreateAssetSync.ExecuteIfBound(*Data, T::StaticClass());
}
