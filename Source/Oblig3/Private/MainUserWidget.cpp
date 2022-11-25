// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUserWidget.h"
#include "Components/Button.h"
#include "../NodeManager.h"

bool UMainUserWidget::Initialize() {
	Super::Initialize();

	if (RunDijkstraButton)
	{
		RunDijkstraButton->OnClicked.AddDynamic(this, &UMainUserWidget::DijkstraButton);
	}

	return true;
}

void UMainUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainUserWidget::DijkstraButton()
{
	UE_LOG(LogTemp, Warning, TEXT("Dijkstra button clicked"));
}
