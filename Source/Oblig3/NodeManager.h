// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeManager.generated.h"

UCLASS()
class OBLIG3_API ANodeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void spawnNodes();
	void connectNodes();
	bool checkConnect();
	void showConnect();

	void RunAlgorithm(bool bRunAStar);
	float DistanceToEnd(class ANodeSphere* CurrentNode);

	class ANodeSphere* EndNode;
	
	bool bConnecting;
	int loopCount;
	bool bAlgoReachedEnd;
	float shortestPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mytools)
		TArray<class ANodeSphere*> SphereArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mytools)
		TArray<class ANodeSphere*> SearchNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mytools)
		TArray<int> WhichPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mytools)
		TArray<class ANodeSphere*> SearchedNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mytools)
		TArray<class ANodeSphere*> AlgoPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mytools)
		TArray<float> ArrDistanceToEnd;

	FVector CurrentNodeLocation;
	FVector NextNodeLocation;
};
