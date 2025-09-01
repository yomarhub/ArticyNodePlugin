
#include "FLNodes.h"
#include "ArticyPackage.h"
#include "ArticyEntity.h"
#include "ArticytestInterfaces.h"
#include "ArticytestArticyTypes.h"
#include "Engine/Engine.h"

TMap<FName, TSoftObjectPtr<UArticyObject>> UFLNodes::GetAssetsDict(UArticyPackage* Database)
{
	if (!ensure(Database != nullptr))
		return {};

	return Database->GetAssetsDict();
}

TArray<TSoftObjectPtr<UArticyObject>> UFLNodes::GetAssetsByName(UArticyPackage* Database, const FName& NameFilter)
{
	if (!ensure(Database != nullptr))
		return {};

	TArray<TSoftObjectPtr<UArticyObject>> FilteredAssets;
	const TMap<FName, TSoftObjectPtr<UArticyObject>> AssetsDict = Database->GetAssetsDict();

	if (AssetsDict.Contains(NameFilter))
	{
		FilteredAssets.Add(AssetsDict[NameFilter]);
	}
	return FilteredAssets;
}

template<typename T>
TArray<TSoftObjectPtr<T>> UFLNodes::GetAssetByType(UArticyPackage* Database)
{
	static_assert(std::is_base_of<UArticyObject, T>::value, "T must be a subclass of UArticyObject");

	TArray<TSoftObjectPtr<T>> FilteredAssets;
	if (!ensure(Database != nullptr))
		return FilteredAssets;

	const TMap<FName, TSoftObjectPtr<UArticyObject>> AssetsDict = Database->GetAssetsDict();
	for (const auto& Pair : AssetsDict)
	{
		if (Pair.Value.IsValid() && Pair.Value->IsA(T::StaticClass()))
		{
			FilteredAssets.Add(TSoftObjectPtr<T>(Pair.Value.ToSoftObjectPath()));
		}
	}
	return FilteredAssets;
}

TSoftObjectPtr<UArticyObject> UFLNodes::GetAssetById(UArticyPackage* Database, const FArticyId& Id)
{
	if (!ensure(Database != nullptr))
		return nullptr;
	return Database->GetAssetById(Id);
}

TArray<TSoftObjectPtr<UArticyObject>> UFLNodes::GetAssetsByType(UArticyPackage* Database, const TSubclassOf<UArticyObject> Type)
{
	if (!ensure(Database != nullptr))
		return {};

	return GetAssetByType<UArticyObject>(Database);
}

TArray<TSoftObjectPtr<UArticyEntity>> UFLNodes::GetEntities(UArticyPackage* Database)
{
	if (!ensure(Database != nullptr))
		return {};

	return GetAssetByType<UArticyEntity>(Database);
}

TArray<TSoftObjectPtr<UArticytestNode>> UFLNodes::GetNodes(UArticyPackage* Database)
{
	if (!ensure(Database != nullptr))
		return {};

	return GetAssetByType<UArticytestNode>(Database);
}

UArticytestNodePropFeature* UFLNodes::GetNodePropFeature(const UArticytestNode* Object)
{
	if (!ensure(Object != nullptr))
		return nullptr;

	return Object->GetFeatureNodeProp();
}

void UFLNodes::TempLog(const FString& Message, const FString& Prefix)
{
	UE_LOG(LogTemp, Log, TEXT("%s%s"), *Prefix, *Message);
}

UTexture2D* UFLNodes::GetTextureFromNode(UObject* WorldContext, const UArticytestNode* Node, const int Index)
{
	if (!Node->GetFeatureNodeProp()) return nullptr;
	UArticytestNodePropFeature* NodeProp = Node->GetFeatureNodeProp();
	if (NodeProp->Images.Num() <= 0 || Index >= NodeProp->Images.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("No Images available."));
		return nullptr;
	}
	FArticyId ImageId = NodeProp->Images[Index];

	return Cast<UArticytestAsset>(ImageId.GetObject(WorldContext))->LoadAsTexture2D();
}

FArticyId UFLNodes::GetImageId(const UArticytestNode* Node) {
	if (!Node->GetFeatureNodeProp()) return FArticyId();

	UArticytestNodePropFeature* NodeProp = Node->GetFeatureNodeProp();
	if (NodeProp->Images.Num() <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("No Images available."));
		return FArticyId();
	}
	// If Images is not empty, we can use the first image as a reference
	FArticyId FirstImageId = NodeProp->Images[0];
	UE_LOG(LogTemp, Log, TEXT("First Image ID: %s"), *FirstImageId.ToString());
	return FirstImageId;
}

void UFLNodes::PrintNode(const UArticytestNode* Node, const FString& Prefix)
{
	if (!ensure(Node != nullptr))
		return;

	FString NodeName = Node->GetDisplayName().ToString();
	if (NodeName.IsEmpty())
	{
		NodeName = Node->GetId().ToString();
	}
	UE_LOG(LogTemp, Log, TEXT("%s%s"), *Prefix, *NodeName);
	if (Node->GetFeatureNodeProp())
	{
		UArticytestNodePropFeature* NodeProp = Node->GetFeatureNodeProp();
		/*
		* UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Type"))
		* EArticytestType Type;
		* UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Activé"))
		* bool Active = false;
		* UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Evolution"))
		* float Evolution = 0.f;
		* UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Descriptions"))
		* TArray<FArticyId> Descriptions;
		* UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Images (Manual)"))
		* TArray<FArticyId> Images_Manual;
		* UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Images"))
		* TArray<FArticyId> Images;
		*/
		FName type = StaticEnum<EArticytestType>()->GetNameByValue((int64)NodeProp->Type);
		FText FormattedText = FText::Format(
			FText::FromString(TEXT("Type= {0}, Active= {1}, Evolution= {2}, Descriptions= {3}, Images Number= {4}")),
			FText::FromName(type),
			FText::FromString(NodeProp->Active ? TEXT("true") : TEXT("false")),
			FText::FromString(FString::SanitizeFloat(NodeProp->Evolution)),
			FText::AsNumber(NodeProp->Descriptions.Num()),
			FText::AsNumber(NodeProp->Images.Num())
		);
		UE_LOG(LogTemp, Log, TEXT("NodeProp: %s"), *FormattedText.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s  No NodeProp feature available."), *Prefix);
	}
}
