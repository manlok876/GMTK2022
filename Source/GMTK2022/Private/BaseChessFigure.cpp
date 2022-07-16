// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChessFigure.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ChessGrid.h"


ABaseChessFigure::ABaseChessFigure()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseChessFigure::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseChessFigure::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseChessFigure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AChessGrid* ABaseChessFigure::GetGrid()
{
	return nullptr;
}

TArray<FGridCoords> ABaseChessFigure::GetPossibleMoves() const
{
	return TArray<FGridCoords>();
}

bool ABaseChessFigure::CanGoTo(const FGridCoords& Cell) const
{
	return false;
}

bool ABaseChessFigure::TryGoTo(const FGridCoords& Cell)
{
	return false;
}
