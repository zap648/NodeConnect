// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeSphere.h"
#include "Components/PrimitiveComponent.h"
#include "Containers/Array.h"
#include "DrawDebugHelpers.h"
#include "UObject/UObjectBaseUtility.h"
#include "Kismet\KismetSystemLibrary.h"

// Sets default values
ANodeSphere::ANodeSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    int r = rand() % 8;

    if (r == 0)
        size = 5;
    else if (r == 1)
        size = 8;
    else if (r == 2)
        size = 10;
    else if (r == 3)
        size = 12;
    else if (r == 4)
        size = 15;
    else if (r == 5)
        size = 18;
    else if (r == 6)
        size = 22;
    else if (r == 7)
        size = 26;

	NodeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NodeMesh"));
	NodeMesh->SetupAttachment(GetRootComponent());
    NodeMesh->SetWorldScale3D(FVector(size/5, size/5, size/5));
	SetRootComponent(NodeMesh);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(50);
	CollisionSphere->SetupAttachment(NodeMesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Sphere.Sphere'"));
	WhiteMaterial = MeshAsset.Object;

	bStart = false;
	bEnd = false;

	ConnectedNodesList.Init(nullptr, 100);
}

// Called when the game starts or when spawned
void ANodeSphere::BeginPlay()
{
	Super::BeginPlay();

	NodeMesh->SetStaticMesh(WhiteMaterial);
    
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ANodeSphere::OnOverlap);
}

// Called every frame
void ANodeSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    /*
    // Set what actors to seek out from it's collision channel
    TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
    traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

    // Ignore any specific actors
    TArray<AActor*> ignoreActors;
    // Ignore self or remove this line to not ignore any
    ignoreActors.Init(this, 1);

    // Array of actors that are inside the radius of the sphere
    TArray<AActor*> outActors;

    // Total radius of the sphere
    float radius = CollisionSphere->GetUnscaledSphereRadius();
    // Sphere's spawn loccation within the world
    FVector sphereSpawnLocation = GetActorLocation();
    // Class that the sphere should hit against and include in the outActors array (Can be null)
    UClass* seekClass = ANodeSphere::StaticClass(); // NULL;
    UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

    // Optional: Use to have a visual representation of the SphereOverlapActors
    DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::Red, true, 10.0f);

    // Finally iterate over the outActor array
    for (AActor* overlappedActor : outActors) {
        UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
    }
    */

}

void ANodeSphere::connectTo(ANodeSphere* connectSphere)
{
	bool bDuplicate = false;
	for (int i = 0; i < connectSphere->ConnectedNodesList.Num(); i++)
	{
		if (connectSphere->ConnectedNodesList[i] == this)
		{
			bDuplicate = true;
			break;
		}
	}

	if (!bDuplicate)
	{
		ConnectedNodesList.Add(connectSphere);
		connectSphere->ConnectedNodesList.Add(this);
	}

    UE_LOG(LogTemp, Display, TEXT("A node has connected with another node!"));
}

void ANodeSphere::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
    
    UE_LOG(LogTemp, Display, TEXT("A node has collided with another node!"));
    
    if (OtherActor->IsA(ANodeSphere::StaticClass())) {
        ANodeSphere* CollidedNode = Cast<ANodeSphere>(OtherActor);

        // First check if the node you just collided with is already in your ConnectedNodeList
        // If so we don't need to add it, otherwise we do
        if (ConnectedNodesList.Find(CollidedNode) == INDEX_NONE) {
            // Check now if the other node has any nodes in its list, if it does we need to see if
            // we are in that list
            if (CollidedNode->ConnectedNodesList.Num() != 0) {
                for (int i = 0; i < CollidedNode->ConnectedNodesList.Num(); i++) {
                    if (CollidedNode->ConnectedNodesList.Find(this) != INDEX_NONE) {
                        // If we are in that list, we add them to our list
                        // we do not add ourselves to their list, as that would create a dupilicate
                        this->ConnectedNodesList.Add(CollidedNode);
                        return;
                    }
                    else {
                        // If we are not in that list we will add ourselves to their list
                        // and we will add them to ours
                        CollidedNode->ConnectedNodesList.Add(this);
                        this->ConnectedNodesList.Add(CollidedNode);
                        return;
                    }
                }
            }
            else {
                // If the other node has an empty list we put them in our list and
                // add ourselves to their list
                this->ConnectedNodesList.Add(CollidedNode);
                CollidedNode->ConnectedNodesList.Add(this);
                return;
            }
        }
    }
    
}