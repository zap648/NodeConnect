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

    cost = size;

	NodeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NodeMesh"));
	NodeMesh->SetupAttachment(GetRootComponent());
    NodeMesh->SetWorldScale3D(FVector(size/5, size/5, size/5));
	SetRootComponent(NodeMesh);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(50);
	CollisionSphere->SetupAttachment(NodeMesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh>WhiteMeshAsset(TEXT("StaticMesh'/Game/WhiteSphere.WhiteSphere'"));
	WhiteSphere = WhiteMeshAsset.Object;
	
    ConstructorHelpers::FObjectFinder<UStaticMesh>RedMeshAsset(TEXT("StaticMesh'/Game/RedSphere.RedSphere'"));
	RedSphere = RedMeshAsset.Object;
	
    ConstructorHelpers::FObjectFinder<UStaticMesh>GreenMeshAsset(TEXT("StaticMesh'/Game/GreenSphere.GreenSphere'"));
	GreenSphere = GreenMeshAsset.Object;

    ConstructorHelpers::FObjectFinder<UStaticMesh>BlueMeshAsset(TEXT("StaticMesh'/Game/BlueSphere.BlueSphere'"));
    BlueSphere = BlueMeshAsset.Object;

    NodeMesh->SetStaticMesh(WhiteSphere);

	bStart = false;
	bEnd = false;

	ConnectedNodesList.Init(nullptr, 0);
}

// Called when the game starts or when spawned
void ANodeSphere::BeginPlay()
{
	Super::BeginPlay();
    
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ANodeSphere::OnOverlap);
    CollisionSphere->bHiddenInGame = false;
}

// Called every frame
void ANodeSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    SphereLocation = GetActorLocation();
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

void ANodeSphere::setStartNode(bool b)
{
    bStart = b;
    NodeMesh->SetStaticMesh(GreenSphere);
}

void ANodeSphere::setEndNode(bool b)
{
    bEnd = b;
    NodeMesh->SetStaticMesh(RedSphere);
}

bool ANodeSphere::isStartNode()
{
    return bStart;
}

bool ANodeSphere::isEndNode()
{
    return bEnd;
}

void ANodeSphere::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
    if (OtherActor->IsA(ANodeSphere::StaticClass())) {
        ANodeSphere* CollidedNode = Cast<ANodeSphere>(OtherActor);
        //UE_LOG(LogTemp, Display, TEXT("A node has collided with another node!"));

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

