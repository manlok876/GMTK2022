// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessStructs.h"
#include "ChessGrid.generated.h"

class AChessGrid;

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
	AActor* GetActorOnCell(const FGridCoords& CellCoords) const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool IsActorOnGrid(const AActor* Target) const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool GetActorCoords(const AActor* Target, FGridCoords& Result) const;

	UFUNCTION(BlueprintCallable)
	bool IsValidCell(const FGridCoords& CellCoords) const;

	UFUNCTION(BlueprintCallable)
	bool TryOccupyCell(AActor* EnteringActor, const FGridCoords& CellCoords);
	UFUNCTION(BlueprintCallable)
	bool FreeCell(AActor* LeavingActor);

protected:
	virtual void BeginPlay() override;

private:
	int GetCellIndex1D(const FGridCoords& CellCoords) const;
	bool IsValidIndex1D(int Idx) const;
	FGridCoords GetCoordsFromIndex1D(int Idx) const;

	UPROPERTY()
	TArray<AActor*> ActorsOnGrid;

};
