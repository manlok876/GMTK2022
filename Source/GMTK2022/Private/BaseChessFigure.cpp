// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChessFigure.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ChessGrid.h"


ABaseChessFigure::ABaseChessFigure()
{
	PrimaryActorTick.bCanEverTick = true;

	CachedGrid = nullptr;
	bIsMoveInProgress = false;
	PendingCombatTarget = nullptr;
}

void ABaseChessFigure::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseChessFigure::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AChessGrid* Grid = GetGrid();
	if (IsValid(Grid))
	{
		Grid->FreeCell(this);
	}

	Super::EndPlay(EndPlayReason);
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
	if (!IsValid(CachedGrid))
	{
		for (TActorIterator<AChessGrid> It(GetWorld()); It; ++It)
		{
			CachedGrid = *It;
			break;
		}
	}

	return CachedGrid;
}

TArray<FGridCoords> ABaseChessFigure::GetPossibleMoves_Implementation() const
{
	return TArray<FGridCoords>();
}

bool ABaseChessFigure::CanGoTo_Implementation(const FGridCoords& Cell) const
{
	return false;
}

bool ABaseChessFigure::TryGoTo(const FGridCoords& Cell)
{
	if (!CanGoTo(Cell))
	{
		return false;
	}

	bIsMoveInProgress = true;
	PendingDestination = Cell;
	StartMovementAnimation(Cell);

	return true;
}

bool ABaseChessFigure::IsMoveInProgress() const
{
	return bIsMoveInProgress;
}

FGridCoords ABaseChessFigure::GetPendingDestination() const
{
	return PendingDestination;
}

ABaseChessFigure* ABaseChessFigure::GetPendingCombatTarget() const
{
	return PendingCombatTarget;
}

bool ABaseChessFigure::IsCombatPending() const
{
	return IsValid(GetPendingCombatTarget());
}

void ABaseChessFigure::PrepareToDefend_Implementation(ABaseChessFigure* Attacker)
{
	PendingCombatTarget = Attacker;
}

void ABaseChessFigure::ClearPendingCombat_Implementation()
{
	PendingCombatTarget = nullptr;
}

void ABaseChessFigure::EndMovementAnimation()
{
	if (IsCombatPending())
	{
		ABaseChessFigure* Target = GetPendingCombatTarget();
		if (IsValid(Target))
		{
			StartCombatAnimation(Target, true);
			Target->StartCombatAnimation(this, false);
		}
	}
	else
	{
		EndMove();
	}
}

void ABaseChessFigure::EndCombatAnimation()
{
	ClearPendingCombat();
}

void ABaseChessFigure::EndMove()
{
	AChessGrid* Grid = GetGrid();
	if (IsValid(Grid))
	{
		Grid->FreeCell(this);
		Grid->TryOccupyCell(this, GetPendingDestination());
	}

	bIsMoveInProgress = false;

	if (MoveEndedDispatcher.IsBound())
	{
		MoveEndedDispatcher.Broadcast(this);
	}
}
