// Fill out your copyright notice in the Description page of Project Settings.

#include "NodeSphere.h"
#include "DrawDebugHelpers.h"
#include "NodeManager.h"

// Sets default values
ANodeManager::ANodeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereArray.Init(NULL, 9);
	WhichPlayer.Init(0, 9);
	
}

// Called when the game starts or when spawned
void ANodeManager::BeginPlay()
{
	Super::BeginPlay();

	spawnNodes();
	connectNodes();
	showConnect();
}

// Called every frame
void ANodeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	UE_LOG(LogTemp, Display, TEXT("Nodes spawned!"))

	// Setting the start & end nodes
	SphereArray[0]->bStart = true;
	SphereArray[8]->bEnd = true;
	UE_LOG(LogTemp, Display, TEXT("Start & End node set!"))
}

void ANodeManager::connectNodes()
{
	bool bConnecting = true;
	int loopCount = 0;
	
	UE_LOG(LogTemp, Display, TEXT("Connect started!"));
	do {
		// Prevents an endless loop
		if (loopCount > 10000)
		{
			UE_LOG(LogTemp, Error, TEXT("Endless loop detected, ending connecting..."));
			break;
		}

		// Checks if all nodes are connected
		if (checkConnect()) 
		{
			for (int i = 0; i < SphereArray.Num(); i++)
			{
				SphereArray[i]->bConnecting = false;
				bConnecting = false;
			}
			UE_LOG(LogTemp, Display, TEXT("Connecting completed!"));
		}

		//else
		//{
		//	for (int i = 0; i < SphereArray.Num(); i++)
		//	{
		//		SphereArray[i]->CollisionSphere->SetSphereRadius(loopCount);
		//	}
		//	//UE_LOG(LogTemp, Display, TEXT("Sphere Radius increased!"));
		//}

		else
		{
			for (int i = 0; i < SphereArray.Num(); i++)
			{
				for (int j = 0; j < SphereArray.Num(); j++) {

					if (i != j)
					{
						SphereArray[i]->connectTo(SphereArray[j]);
					}
				}
			}
		}

		loopCount++;
		//UE_LOG(LogTemp, Display, TEXT("Loop completed!"));
	} while (bConnecting);
}

bool ANodeManager::checkConnect()
{
	bool bConnecting;
	for (int i = 0; i < SphereArray.Num(); i++) 
	{
		for (int j = 0; j < SphereArray[i]->ConnectedNodesList.Num(); j++)
		{
			bConnecting = false;
			if (SphereArray[i]->ConnectedNodesList[j] != nullptr)
			{
				bConnecting = true;
				continue;
			}
		}
		if (bConnecting == false)
			break;
	}

	return bConnecting;
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

				DrawDebugLine(GetWorld(), Start, End, FColor::Black, true, -1, 0, 3.f);
			}
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Connections shown!"))
}