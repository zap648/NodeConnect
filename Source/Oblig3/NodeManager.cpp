// Fill out your copyright notice in the Description page of Project Settings.

#include "NodeManager.h"
#include "NodeSphere.h"
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"
#include <math.h>

// Sets default values
ANodeManager::ANodeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereArray.Init(NULL, 9);
	WhichPlayer.Init(0, 9);

	bConnecting = true;
	bAlgoReachedEnd = false;
	loopCount = 0;	
}

// Called when the game starts or when spawned
void ANodeManager::BeginPlay()
{
	Super::BeginPlay();

	spawnNodes();
	UE_LOG(LogTemp, Display, TEXT("Connect started!"));
}

// Called every frame
void ANodeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bConnecting)
		connectNodes();

	if (!bConnecting && !bAlgoReachedEnd)
		RunAlgorithm();

	//for (int i = 0; i < SphereArray.Num(); i++)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("This a testing statement. %s"), *SphereArray[i]->GetName());
	//}
}

void ANodeManager::spawnNodes()
{
	// Spawning the nodes
	SphereArray[0] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(1000.f, -1000.f, 400.f), FRotator(0.f, 0.f, 0.f));
	SphereArray[1] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(1000.f, 0.f, -800.f), FRotator(0.f, 0.f, 0.f));
	SphereArray[2] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(1000.f, 1000.f, -400.f), FRotator(0.f, 0.f, 0.f));
	SphereArray[3] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(0.f, -1000.f, 0.f), FRotator(0.f, 0.f, 0.f));
	SphereArray[4] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(0.f, 0.f, 400.f), FRotator(0.f, 0.f, 0.f));
	SphereArray[5] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(0.f, 1000.f, 200.f), FRotator(0.f, 0.f, 0.f));
	SphereArray[6] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(-1000.f, -1000.f, 600.f), FRotator(0.f, 0.f, 0.f));
	SphereArray[7] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(-1000.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	SphereArray[8] = GetWorld()->SpawnActor<ANodeSphere>(ANodeSphere::StaticClass(), FVector(-1000.f, 1000.f, -800.f), FRotator(0.f, 0.f, 0.f));
	UE_LOG(LogTemp, Display, TEXT("Nodes spawned!"));

	// Setting the start & end nodes
	SphereArray[0]->setStartNode(true);
	SphereArray[8]->setEndNode(false);
	UE_LOG(LogTemp, Display, TEXT("Start & End node set!"));
}

void ANodeManager::connectNodes()
{
	// Prevents an endless loop
	if (loopCount > 10000)
	{
		UE_LOG(LogTemp, Error, TEXT("Endless loop detected, ending connecting..."));
		bConnecting = false;
	}

	// Checks if all nodes are connected
	if (checkConnect()) 
	{
		for (int i = 0; i < SphereArray.Num(); i++)
		{
			bConnecting = false;
		}
		UE_LOG(LogTemp, Display, TEXT("Connecting completed! Showing nodes..."));
		showConnect();
	}

	else
	{
		for (int i = 0; i < SphereArray.Num(); i++)
		{
			SphereArray[i]->CollisionSphere->SetSphereRadius(loopCount);
		}
		//UE_LOG(LogTemp, Display, TEXT("Sphere Radius increased!"));
	}

	loopCount++;
	//UE_LOG(LogTemp, Display, TEXT("Loop completed!"));
}

// Checks through all nodes if they have at least ONE connected node
bool ANodeManager::checkConnect()
{
	bool bConnected;
	// UE_LOG(LogTemp, Display, TEXT("Checking connections"));
	for (int i = 0; i < SphereArray.Num(); i++) 
	{
		//UE_LOG(LogTemp, Display, TEXT("Checking connections"));
		bConnected = false;
		for (int j = 0; j < SphereArray[i]->ConnectedNodesList.Num(); j++)
		{
			bConnected = false;
			if (SphereArray[i]->ConnectedNodesList[j] != nullptr)
			{
				bConnected = true;
				// UE_LOG(LogTemp, Display, TEXT("A sphere is connected!!!"));
				break;
			}
		}
		if (!bConnected)
			break;
	}

	return bConnected;
}

void ANodeManager::showConnect()
{
	for (int i = 0; i < SphereArray.Num(); i++)
	{
		for (int j = 0; j < SphereArray[i]->ConnectedNodesList.Num(); j++)
		{
			if (SphereArray[i]->ConnectedNodesList[j] != nullptr)
			{
				FVector Start = SphereArray[i]->GetActorLocation();
				FVector End = SphereArray[i]->ConnectedNodesList[j]->GetActorLocation();

				DrawDebugLine(GetWorld(), Start, End, FColor::Black, true, -1, 0, 10.f);
			}
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Connections shown!"));

	if (checkConnect())
		UE_LOG(LogTemp, Display, TEXT("All nodes are connected!"));
}

void ANodeManager::RunAlgorithm()
{
	//UE_LOG(LogTemp, Display, TEXT("This a testing statement. %s"), *SphereArray[0]->GetName());

	int NodeNumber = 0;

	if (!checkConnect())
		return;

	bAlgoReachedEnd = false;

	// We initialize all Sphere costs to infinite, except the Start Node
	for (int i = 0; i < SphereArray.Num(); i++)
	{
		if (!SphereArray[i]->isStartNode())
		{
			SphereArray[i]->cost = INT_MAX;
		}
		else
		{
			NodeNumber = i;
			SphereArray[i]->cost = 0;

			// Adding the starting node to the Searched Nodes array
			SearchedNodes.Add(SphereArray[i]);
		}
	}

	float tempPath;
	float shortestNode = 0;
	bool bSearchedNode;

	float xDifference;
	float yDifference;
	float zDifference;

	float xDistance;
	float yDistance;
	float zDistance;

	float Distance;

	int AlgoPathSize;
	int LoopCount = 0;

	while (!bAlgoReachedEnd)
	{
		CurrentNodeLocation = SphereArray[NodeNumber]->SphereLocation;
		shortestPath = INT_MAX;

		for (int i = 0; i < SphereArray[NodeNumber]->ConnectedNodesList.Num(); i++)
		{
			NextNodeLocation = SphereArray[NodeNumber]->ConnectedNodesList[i]->SphereLocation;

			xDifference = SphereArray[NodeNumber]->SphereLocation.X - SphereArray[NodeNumber]->ConnectedNodesList[i]->SphereLocation.X;
			yDifference = SphereArray[NodeNumber]->SphereLocation.Y - SphereArray[NodeNumber]->ConnectedNodesList[i]->SphereLocation.Y;
			zDifference = SphereArray[NodeNumber]->SphereLocation.Z - SphereArray[NodeNumber]->ConnectedNodesList[i]->SphereLocation.Z;

			xDistance = pow(xDifference, 2);
			yDistance = pow(yDifference, 2);
			zDistance = pow(zDifference, 2);

			Distance = xDistance + yDistance + zDistance;

			tempPath = pow(Distance, 0.5);

			UE_LOG(LogTemp, Display, TEXT("Checking distance of node: %f, %d"), tempPath, i);

			if (shortestPath > tempPath)
			{
				for (int j = 0; j < SearchedNodes.Num(); j++)
				{
					bSearchedNode = false;

					// Checks if the next sphere has already been searched
					if (SearchedNodes[j] == SphereArray[NodeNumber]->ConnectedNodesList[i])
					{
						bSearchedNode = true;
						break;
					}
				}

				if (!bSearchedNode)
				{
					for (int j = 0; j < SphereArray.Num(); j++)
					{
						if (SphereArray[j] == SphereArray[NodeNumber]->ConnectedNodesList[i])
						{
							shortestPath = tempPath;
							shortestNode = j;
							//UE_LOG(LogTemp, Display, TEXT("Shortest node: %f"), shortestNode);
						}
					}
				}
			}
		}

		UE_LOG(LogTemp, Display, TEXT("Shortest path & node: %f, %f"), shortestPath, shortestNode);

		// Checks if the algorithm has reached a dead end, and if so go back
		if (shortestPath == INT_MAX)
		{
			UE_LOG(LogTemp, Display, TEXT("Shortest path INT_MAX detected"));
			AlgoPathSize = AlgoPath.Num() - 1;
			for (int i = 0; i < SphereArray.Num(); i++)
			{
				if (AlgoPathSize > 0)
				{
					UE_LOG(LogTemp, Display, TEXT("Algopathsize if gate opened"));
					if (SphereArray[i] == AlgoPath[AlgoPathSize - 1])
					{
						shortestNode = i;
						AlgoPath.RemoveAt(AlgoPathSize);
						UE_LOG(LogTemp, Display, TEXT("New shortest node:, %f"), shortestNode);
					}
				}
			}

			//for (int i = 0; i < AlgoPath.Num(); i++)
			//{
			//	UE_LOG(LogTemp, Display, TEXT("New shortest node:, %s"), *AlgoPath[i]->GetName());
			//}
		}
		
		bSearchedNode = false;

		for (int i = 0; i < AlgoPath.Num(); i++)
		{
			if (AlgoPath[AlgoPathSize] == AlgoPath[i])
			{
				UE_LOG(LogTemp, Display, TEXT("If gate opened:"));
				bSearchedNode = true;
			}
		}

		if (!bSearchedNode)
			AlgoPath.Add(SphereArray[AlgoPathSize]);

		NodeNumber = shortestNode;
		SearchedNodes.Add(SphereArray[NodeNumber]);

		if (SphereArray[NodeNumber]->isEndNode())
			bAlgoReachedEnd = true;

		LoopCount++;
		if (LoopCount >= 30)
		{
			bAlgoReachedEnd = true;
			UE_LOG(LogTemp, Display, TEXT("Infinite loop detected!!!!"));
			break;
		}
	}


	for (int i = 0; i < AlgoPath.Num(); i++)
	{
		if (!AlgoPath[i]->isStartNode() || !AlgoPath[i]->isEndNode())
			AlgoPath[i]->NodeMesh->SetStaticMesh(AlgoPath[i]->BlueSphere);
	}
}
