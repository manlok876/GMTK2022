// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChessFigure.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ChessUtilityFunctions.h"
#include "ChessGrid.h"


ABaseChessFigure::ABaseChessFigure()
{
	PrimaryActorTick.bCanEverTick = true;

	CachedGrid = nullptr;
	PendingCombatTarget = nullptr;
	FigureType = EFigureType::EFT_Pawn;
	FigureColor = EChessColor::ECC_White;
	bHasMoved = false;
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

TArray<FGridCoords> ABaseChessFigure::GetPossibleMoves_Implementation()
{
	TArray<FGridCoords> Result;

	AChessGrid* Grid = GetGrid();
	if (!IsValid(Grid))
	{
		return Result;
	}

	switch (GetFigureType())
	{
	case EFigureType::EFT_Pawn:
		Result = GetPawnMoves(Grid, CurrentPosition, GetFigureColor(), !bHasMoved);
		break;
	case EFigureType::EFT_Rook:
		Result = GetRookMoves(Grid, CurrentPosition, GetFigureColor(), !bHasMoved);
		break;
	case EFigureType::EFT_Knight:
		Result = GetKnightMoves(Grid, CurrentPosition, GetFigureColor());
		break;
	case EFigureType::EFT_Bishop:
		Result = GetBishopMoves(Grid, CurrentPosition, GetFigureColor());
		break;
	case EFigureType::EFT_Queen:
		Result = GetQueenMoves(Grid, CurrentPosition, GetFigureColor());
		break;
	case EFigureType::EFT_King:
		Result = GetKingMoves(Grid, CurrentPosition, GetFigureColor(), !bHasMoved);
		break;
	default:
		break;
	}

	return Result;
}

bool ABaseChessFigure::CanEnterCell(
	const AChessGrid* Grid, const FGridCoords& Cell, EChessColor OurColor, EFigureType OurType)
{
	if (!IsValid(Grid))
	{
		return false;
	}
	if (OurType == EFigureType::EFT_None)
	{
		return false;
	}

	if (!Grid->IsValidCell(Cell))
	{
		return false;
	}

	ABaseChessFigure* OccupyingFigure = Grid->GetActorOnCell(Cell);
	bool bIsOccupied = IsValid(OccupyingFigure);
	if (!bIsOccupied)
	{
		return true;
	}

	bool bCanAttack = OccupyingFigure->GetFigureColor() != OurColor;

	bool bCanUpgrade = OccupyingFigure->GetFigureColor() == OurColor &&
		UChessUtilityFunctions::CanCombineFigures(OccupyingFigure->GetFigureType(), OurType);

	bool bCanInterract = bCanAttack || bCanUpgrade;

	return bCanInterract;
}

TArray<FGridCoords> ABaseChessFigure::GetPawnMoves(
	const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color, bool bIsFirstMove)
{
	TArray<FGridCoords> Result;

	if (!IsValid(Grid))
	{
		return Result;
	}

	FGridCoords FrontCell;
	FGridCoords SkipCell;
	FGridCoords LeftCell;
	FGridCoords RightCell;
	if (Color == EChessColor::ECC_White)
	{
		FrontCell = StartingPoint + FGridCoords(0, 1);
		SkipCell = StartingPoint + FGridCoords(0, 2);
		LeftCell = StartingPoint + FGridCoords(-1, 1);
		RightCell = StartingPoint + FGridCoords(1, 1);
	}
	else
	{
		FrontCell = StartingPoint + FGridCoords(0, -1);
		SkipCell = StartingPoint + FGridCoords(0, -2);
		LeftCell = StartingPoint + FGridCoords(-1, -1);
		RightCell = StartingPoint + FGridCoords(1, -1);
	}

	ABaseChessFigure* FrontCellActor = Grid->GetActorOnCell(FrontCell);
	ABaseChessFigure* SkipCellActor = Grid->GetActorOnCell(SkipCell);
	ABaseChessFigure* LeftCellActor = Grid->GetActorOnCell(LeftCell);
	ABaseChessFigure* RightCellActor = Grid->GetActorOnCell(RightCell);

	if (!IsValid(FrontCellActor) || 
		(FrontCellActor->FigureColor == Color && 
			UChessUtilityFunctions::CanCombineFigures(FrontCellActor->GetFigureType(), EFigureType::EFT_Pawn)))
	{
		Result.Add(FrontCell);
		if (bIsFirstMove && !IsValid(SkipCellActor) ||
			(SkipCellActor->FigureColor == Color &&
				UChessUtilityFunctions::CanCombineFigures(SkipCellActor->GetFigureType(), EFigureType::EFT_Pawn)))
		{
			Result.Add(SkipCell);
		}
	}
	if (IsValid(LeftCellActor) && LeftCellActor->FigureColor != Color)
	{
		Result.Add(LeftCell);
	}
	if (IsValid(LeftCellActor) && LeftCellActor->FigureColor != Color)
	{
		Result.Add(RightCell);
	}

	return Result;
}

TArray<FGridCoords> ABaseChessFigure::GetRookMoves(
	const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color, bool bIsFirstMove)
{
	TArray<FGridCoords> Result;

	if (!IsValid(Grid))
	{
		return Result;
	}

	TArray<FGridCoords> Directions =
	{
		FGridCoords(1, 0),
		FGridCoords(-1, 0),
		FGridCoords(0, 1),
		FGridCoords(0, -1)
	};

	for (const FGridCoords& Direction : Directions)
	{
		FGridCoords CurCell = StartingPoint + Direction;
		while (Grid->IsValidCell(CurCell))
		{
			if (!Grid->IsCellOccupied(CurCell))
			{
				Result.Add(CurCell);
			}
			else
			{
				if (CanEnterCell(Grid, CurCell, Color, EFigureType::EFT_Rook))
				{
					Result.Add(CurCell);
				}
				break;
			}
			CurCell = CurCell + Direction;
		}
	}

	return Result;
}

TArray<FGridCoords> ABaseChessFigure::GetKnightMoves(
	const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color)
{
	TArray<FGridCoords> Result;

	if (!IsValid(Grid))
	{
		return Result;
	}

	TArray<FGridCoords> Directions =
	{
		FGridCoords(2, 1),
		FGridCoords(2, -1),
		FGridCoords(-2, 1),
		FGridCoords(-2, -1),
		FGridCoords(1, 2),
		FGridCoords(-1, 2),
		FGridCoords(1, -2),
		FGridCoords(-1, -2)
	};

	for (const FGridCoords& Direction : Directions)
	{
		FGridCoords CurCell = StartingPoint + Direction;
		if (CanEnterCell(Grid, CurCell, Color, EFigureType::EFT_King))
		{
			Result.Add(CurCell);
		}
	}

	return Result;
}

TArray<FGridCoords> ABaseChessFigure::GetBishopMoves(
	const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color)
{
	TArray<FGridCoords> Result;

	if (!IsValid(Grid))
	{
		return Result;
	}

	TArray<FGridCoords> Directions =
	{
		FGridCoords(1, 1),
		FGridCoords(-1, -1),
		FGridCoords(1, -1),
		FGridCoords(-1, 1)
	};

	for (const FGridCoords& Direction : Directions)
	{
		FGridCoords CurCell = StartingPoint + Direction;
		while (Grid->IsValidCell(CurCell))
		{
			if (!Grid->IsCellOccupied(CurCell))
			{
				Result.Add(CurCell);
			}
			else
			{
				if (CanEnterCell(Grid, CurCell, Color, EFigureType::EFT_Bishop))
				{
					Result.Add(CurCell);
				}
				break;
			}
			CurCell = CurCell + Direction;
		}
	}

	return Result;
}

TArray<FGridCoords> ABaseChessFigure::GetQueenMoves(
	const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color)
{
	TArray<FGridCoords> Result;

	if (!IsValid(Grid))
	{
		return Result;
	}

	TArray<FGridCoords> Directions =
	{
		FGridCoords(1, 0),
		FGridCoords(-1, 0),
		FGridCoords(0, 1),
		FGridCoords(0, -1),
		FGridCoords(1, 1),
		FGridCoords(-1, -1),
		FGridCoords(1, -1),
		FGridCoords(-1, 1)
	};

	for (const FGridCoords& Direction : Directions)
	{
		FGridCoords CurCell = StartingPoint + Direction;
		while (Grid->IsValidCell(CurCell))
		{
			if (!Grid->IsCellOccupied(CurCell))
			{
				Result.Add(CurCell);
			}
			else
			{
				if (CanEnterCell(Grid, CurCell, Color, EFigureType::EFT_Queen))
				{
					Result.Add(CurCell);
				}
				break;
			}
			CurCell = CurCell + Direction;
		}
	}

	return Result;
}

TArray<FGridCoords> ABaseChessFigure::GetKingMoves(
	const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color, bool bIsFirstMove)
{
	TArray<FGridCoords> Result;

	if (!IsValid(Grid))
	{
		return Result;
	}

	TArray<FGridCoords> Directions =
	{
		FGridCoords(1, 0),
		FGridCoords(-1, 0),
		FGridCoords(0, 1),
		FGridCoords(0, -1),
		FGridCoords(1, 1),
		FGridCoords(-1, -1),
		FGridCoords(1, -1),
		FGridCoords(-1, 1)
	};

	for (const FGridCoords& Direction : Directions)
	{
		FGridCoords CurCell = StartingPoint + Direction;
		if (CanEnterCell(Grid, CurCell, Color, EFigureType::EFT_King))
		{
			Result.Add(CurCell);
		}
	}

	return Result;
}

bool ABaseChessFigure::CanGoTo_Implementation(const FGridCoords& Cell)
{
	return GetPossibleMoves().Contains(Cell);
}

bool ABaseChessFigure::InitialPositionSet(const FGridCoords& Cell)
{
	AChessGrid* Grid = GetGrid();
	if (IsValid(Grid))
	{
		check(Grid->TryOccupyCell(this, Cell));
		PendingDestination = CurrentPosition = Cell;
		return true;
	}

	return false;
}

bool ABaseChessFigure::TryGoTo(const FGridCoords& Cell)
{
	if (!CanGoTo(Cell))
	{
		return false;
	}

	PendingDestination = Cell;
	StartMovementAnimation(Cell);

	return true;
}

bool ABaseChessFigure::IsMoveInProgress() const
{
	return CurrentPosition != PendingDestination;
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
		check(Grid->TryOccupyCell(this, GetPendingDestination()));
		CurrentPosition = GetPendingDestination();
	}

	OnMoveEnded();
	if (MoveEndedDispatcher.IsBound())
	{
		MoveEndedDispatcher.Broadcast(this);
	}
}
