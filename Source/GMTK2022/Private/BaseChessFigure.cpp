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
	FigureType = EFigureType::EFT_Pawn;
	FigureColor = EChessColor::ECC_White;
}

void ABaseChessFigure::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	OnTypeChanged(FigureType);
	OnColorChanged(FigureColor);
}

void ABaseChessFigure::BeginPlay()
{
	Super::BeginPlay();

	CachedGrid = GetGrid();
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

EChessColor ABaseChessFigure::GetFigureColor() const
{
	return FigureColor;
}

EFigureType ABaseChessFigure::GetFigureType() const
{
	return FigureType;
}

void ABaseChessFigure::SetFigureType(EFigureType NewType)
{
	if (GetFigureType() == NewType)
	{
		return;
	}

	FigureType = NewType;

	OnTypeChanged(NewType);
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

	OnMoveEnded();
	if (MoveEndedDispatcher.IsBound())
	{
		MoveEndedDispatcher.Broadcast(this);
	}
}
