

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FLNodes.generated.h"

//class UArticytestNode;
//class ArticytestFlowFragment;
//class ArticyPackage;
class UArticyObject;
class UArticyEntity;
class UArticyPackage;
class UArticytestNode;
class UArticytestNodePropFeature;
struct FArticyId;

/**
 *
 */
UCLASS()
class ARTICYPLUSRUNTIME_API UFLNodes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//static TWeakObjectPtr<UArticytestNode> GetFirstNode(const ArticytestFlowFragment& FlowFragment);
	//static FString GetNodeName(const UArticytestNode* Node);

	//static TArray<TWeakObjectPtr<UArticytestNode>> GetAllNodes(const ArticyPackage& DataBase);
	UFUNCTION(BlueprintPure, Category = "FLNodes")
	static TMap<FName, TSoftObjectPtr<UArticyObject>> GetAssetsDict(UArticyPackage* Database);
	UFUNCTION(BlueprintPure, Category = "FLNodes")
	static TArray<TSoftObjectPtr<UArticyObject>> GetAssetsByName(UArticyPackage* Database, const FName& NameFilter);

	//UFUNCTION(BlueprintPure, Category = "FLNodes")
	template<typename T>
	static TArray<TSoftObjectPtr<T>> GetAssetByType(UArticyPackage* Database);

	UFUNCTION(BlueprintPure, Category = "FLNodes")
	TSoftObjectPtr<UArticyObject> GetAssetById(UArticyPackage* Database, const FArticyId& Id);

	UFUNCTION(BlueprintPure, Category = "FLNodes")
	static TArray<TSoftObjectPtr<UArticyObject>> GetAssetsByType(UArticyPackage* Database, const TSubclassOf<UArticyObject> Type);
	UFUNCTION(BlueprintPure, Category = "FLNodes")
	static TArray<TSoftObjectPtr<UArticyEntity>> GetEntities(UArticyPackage* Database);
	UFUNCTION(BlueprintPure, Category = "FLNodes")
	static TArray<TSoftObjectPtr<UArticytestNode>> GetNodes(UArticyPackage* Database);
	UFUNCTION(BlueprintPure, Category = "FLNodes")
	static UArticytestNodePropFeature* GetNodePropFeature(const UArticytestNode* Object);
	UFUNCTION(BlueprintCallable, Category = "FLNodes")
	static FArticyId GetImageId(const UArticytestNode* Node);
	UFUNCTION(BlueprintCallable, Category = "FLNodes")
	static void PrintNode(const UArticytestNode* Node, const FString& Prefix = TEXT("Node: "));
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1", DevelopmentOnly), Category = "FLNodes")
	static void TempLog(const FString& Message, const FString& Prefix);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", DefaultToSelf = "WorldContext"), Category = "FLNodes")
	static UTexture2D* GetTextureFromNode(UObject* WorldContext, const UArticytestNode* Node, const int Index = 0);
};
