// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "NodeSphere.generated.h"

UCLASS()
class OBLIG3_API ANodeSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bStart;
	bool bEnd;
	bool bConnecting;


	TArray<ANodeSphere*> ConnectedNodesList;

	void connectTo(ANodeSphere* connectSphere);

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* NodeMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		class UStaticMesh* WhiteMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		float size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		float cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		float collisionRadius;



	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
