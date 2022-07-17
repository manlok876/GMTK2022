// Copyright Denis Maximenko, July 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessStructs.h"
#include "ChessGrid.generated.h"

class AChessGrid;
class ABaseChessFigure;

UCLASS(Blueprintable, BlueprintType)
class GMTK2022_API AChessGrid : public AActor
{
	GENERATED_BODY()

public:
	AChessGrid();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Rows;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Columns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CellSize;

	UFUNCTION(BlueprintCallable)
	bool IsCellOccupied(const FGridCoords& CellCoords) const;
	UFUNCTION(BlueprintCallable)
	ABaseChessFigure* GetActorOnCell(const FGridCoords& CellCoords) const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool IsActorOnGrid(const ABaseChessFigure* Target) const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool GetActorCoords(const ABaseChessFigure* Target, FGridCoords& Result) const;

	UFUNCTION(BlueprintCallable)
	bool IsValidCell(const FGridCoords& CellCoords) const;

	UFUNCTION(BlueprintCallable)
	bool TryOccupyCell(ABaseChessFigure* EnteringActor, const FGridCoords& CellCoords);
	UFUNCTION(BlueprintCallable)
	bool FreeCell(ABaseChessFigure* LeavingActor);

protected:
	virtual void BeginPlay() override;

private:
	int GetCellIndex1D(const FGridCoords& CellCoords) const;
	bool IsValidIndex1D(int Idx) const;
	FGridCoords GetCoordsFromIndex1D(int Idx) const;

	UPROPERTY()
	TArray<ABaseChessFigure*> ActorsOnGrid;

};
