// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGrid.h"
#include "BaseChessFigure.h"


AChessGrid::AChessGrid()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AChessGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Rows = FMath::Max(Rows, 1);
	Columns = FMath::Max(Columns, 1);

	ActorsOnGrid.SetNum(Rows * Columns);
}

void AChessGrid::BeginPlay()
{
	Super::BeginPlay();

}

bool AChessGrid::IsCellOccupied(const FGridCoords& CellCoords) const
{
	return IsValid(GetActorOnCell(CellCoords));
}

ABaseChessFigure* AChessGrid::GetActorOnCell(const FGridCoords& CellCoords) const
{
	if (!IsValidCell(CellCoords))
	{
		return nullptr;
	}

	int CellIdx1D = GetCellIndex1D(CellCoords);
	if (!ActorsOnGrid.IsValidIndex(CellIdx1D))
	{
		return nullptr;
	}

	return ActorsOnGrid[CellIdx1D];
}

bool AChessGrid::IsActorOnGrid(const ABaseChessFigure* Target) const
{
	if (!IsValid(Target))
	{
		return false;
	}

	for (const ABaseChessFigure* Actor : ActorsOnGrid)
	{
		if (Target == Actor)
		{
			return true;
		}
	}

	return false;
}

bool AChessGrid::GetActorCoords(const ABaseChessFigure* Target, FGridCoords& Result) const
{
	if (!IsValid(Target))
	{
		return false;
	}

	for (int Idx1D = 0; Idx1D < ActorsOnGrid.Num(); ++Idx1D)
	{
		if (ActorsOnGrid[Idx1D] == Target)
		{
			Result = GetCoordsFromIndex1D(Idx1D);
			return true;
		}
	}

	return false;
}

bool AChessGrid::IsValidCell(const FGridCoords& CellCoords) const
{
	bool bIsValidRow = CellCoords.Row >= 0 && CellCoords.Row <= Rows;
	bool bIsValidColumn = CellCoords.Column >= 0 && CellCoords.Column <= Columns;
	return bIsValidRow && bIsValidColumn;
}

bool AChessGrid::TryOccupyCell(ABaseChessFigure* EnteringActor, const FGridCoords& CellCoords)
{
	if (!IsValid(EnteringActor))
	{
		return false;
	}
	if (IsCellOccupied(CellCoords))
	{
		return false;
	}

	int CellIdx1D = GetCellIndex1D(CellCoords);
	if (!ActorsOnGrid.IsValidIndex(CellIdx1D))
	{
		return false;
	}

	ActorsOnGrid[CellIdx1D] = EnteringActor;

	return true;
}

bool AChessGrid::FreeCell(ABaseChessFigure* LeavingActor)
{
	FGridCoords CellCoords;
	if (!GetActorCoords(LeavingActor, CellCoords))
	{
		return false;
	}

	int CellIdx1D = GetCellIndex1D(CellCoords);
	if (!ActorsOnGrid.IsValidIndex(CellIdx1D))
	{
		return false;
	}

	ActorsOnGrid[CellIdx1D] = nullptr;

	return true;
}

int AChessGrid::GetCellIndex1D(const FGridCoords& CellCoords) const
{
	if (!IsValidCell(CellCoords))
	{
		return -1;
	}

	return CellCoords.Row * Columns + CellCoords.Column;
}

bool AChessGrid::IsValidIndex1D(int Idx) const
{
	return ActorsOnGrid.IsValidIndex(Idx);
}

FGridCoords AChessGrid::GetCoordsFromIndex1D(int Idx) const
{
	FGridCoords Result;

	if (!IsValidIndex1D(Idx))
	{
		return Result;
	}

	Result.Row = Idx / Columns;
	Result.Column = Idx % Columns;

	return Result;
}
