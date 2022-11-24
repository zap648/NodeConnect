// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../NodeManager.h"
#include "MainUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OBLIG3_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize();

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UButton* RunDijkstraButton;

	UFUNCTION()
		void DijkstraButton();

};
