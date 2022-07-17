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
	PendingTarget = nullptr;
	FigureType = EFigureType::EFT_Pawn;
	FigureColor = EChessColor::ECC_White;
	bHasMoved = false;
	bAwaitingDestroy = false;

	AttackBonus = 0;
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
		if (bIsFirstMove && (!IsValid(SkipCellActor) ||
			(SkipCellActor->FigureColor == Color &&
				UChessUtilityFunctions::CanCombineFigures(SkipCellActor->GetFigureType(), EFigureType::EFT_Pawn))))
		{
			Result.Add(SkipCell);
		}
	}
	if (IsValid(LeftCellActor) && LeftCellActor->FigureColor != Color)
	{
		Result.Add(LeftCell);
	}
	if (IsValid(RightCellActor) && RightCellActor->FigureColor != Color)
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
		if (CanEnterCell(Grid, CurCell, Color, EFigureType::EFT_Knight))
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

	AChessGrid* Grid = GetGrid();
	if (IsValid(Grid))
	{
		if (Grid->IsCellOccupied(Cell))
		{
			BeginInteraction(Grid->GetActorOnCell(Cell));
		}
	}
	StartMovementAnimation(Cell);

	return true;
}

int ABaseChessFigure::PerformRoll(int AdditiveMod)
{
	int MaxValue = UChessUtilityFunctions::GetValueForType(GetFigureType());
	int RollResult = FMath::RandRange(1, MaxValue);
	return RollResult + FMath::Max(AdditiveMod, 0);
}

void ABaseChessFigure::BeginInteraction(ABaseChessFigure* Target)
{
	if (IsEnemy(Target))
	{
		BeginCombat(Target);
	}
	else if (IsAlly(Target))
	{
		BeginMerge(Target);
	}
}

void ABaseChessFigure::BeginCombat(ABaseChessFigure* Target)
{
	if (!IsValid(Target))
	{
		return;
	}

	if (!IsEnemy(Target))
	{
		return;
	}

	PrepareToInteract(Target);
	Target->PrepareToInteract(this);

	OurSavedRoll = EnemySavedRoll = 0;
	while (OurSavedRoll == EnemySavedRoll)
	{
		OurSavedRoll = PerformRoll(AttackBonus);
		EnemySavedRoll = Target->PerformRoll();
	}
}

void ABaseChessFigure::BeginMerge(ABaseChessFigure* Target)
{
	if (!IsValid(Target))
	{
		return;
	}

	if (!IsAlly(Target))
	{
		return;
	}

	PrepareToInteract(Target);
	Target->PrepareToInteract(this);
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

ABaseChessFigure* ABaseChessFigure::GetPendingTarget() const
{
	return PendingTarget;
}

bool ABaseChessFigure::IsInteractionPending() const
{
	return IsValid(GetPendingTarget());
}

bool ABaseChessFigure::IsEnemy(ABaseChessFigure* Target) const
{
	if (!IsValid(Target))
	{
		return false;
	}

	return Target->GetFigureColor() != GetFigureColor();
}

bool ABaseChessFigure::IsAlly(ABaseChessFigure* Target) const
{
	if (!IsValid(Target))
	{
		return false;
	}

	return Target->GetFigureColor() == GetFigureColor();
}

void ABaseChessFigure::PrepareToInteract_Implementation(ABaseChessFigure* Target)
{
	PendingTarget = Target;
}

void ABaseChessFigure::ClearInteraction_Implementation()
{
	PendingTarget = nullptr;
}

void ABaseChessFigure::EndMovementAnimation()
{
	if (IsInteractionPending())
	{
		ABaseChessFigure* Target = GetPendingTarget();
		if (IsValid(Target))
		{
			StartCombatAnimation(Target, true);
			Target->StartCombatAnimation(this, false);
		}
		else
		{
			EndMove();
		}
	}
	else
	{
		EndMove();
	}
}

void ABaseChessFigure::EndCombatAnimation()
{
	EndMove();
}

void ABaseChessFigure::EndInteraction()
{
	if (!IsInteractionPending())
	{
		return;
	}

	if (IsEnemy(GetPendingTarget()))
	{
		EndCombat();
	}
	else if (IsAlly(GetPendingTarget()))
	{
		EndMerge();
	}
}

void ABaseChessFigure::EndCombat()
{
	if (!IsEnemy(GetPendingTarget()))
	{
		return;
	}

	ABaseChessFigure* Target = GetPendingTarget();
	if (!IsValid(Target))
	{
		return;
	}

	if (OurSavedRoll >= EnemySavedRoll)
	{
		Target->Destroy();
		OnKill();
		ClearInteraction();
	}
	else
	{
		bAwaitingDestroy = true;
		OnDeath();
		Target->ClearInteraction();
	}
}

void ABaseChessFigure::EndMerge()
{
	if (!IsAlly(GetPendingTarget()))
	{
		return;
	}

	ABaseChessFigure* Target = GetPendingTarget();
	if (!IsValid(Target))
	{
		return;
	}

	Target->SetFigureType(UChessUtilityFunctions::GetMergeResult(GetFigureType(), Target->GetFigureType()));
	bAwaitingDestroy = true;
	OnMerge();
	Target->ClearInteraction();
}

void ABaseChessFigure::EndMove()
{
	if (!IsMoveInProgress())
	{
		return;
	}

	if (IsInteractionPending())
	{
		EndInteraction();
	}

	AChessGrid* Grid = GetGrid();
	if (IsValid(Grid))
	{
		Grid->FreeCell(this);
		if (!bAwaitingDestroy)
		{
			check(Grid->TryOccupyCell(this, GetPendingDestination()));
			CurrentPosition = GetPendingDestination();
			bHasMoved = true;
		}
	}

	OnMoveEnded();
	if (MoveEndedDispatcher.IsBound())
	{
		MoveEndedDispatcher.Broadcast(this);
	}

	if (bAwaitingDestroy)
	{
		Destroy();
	}
}
