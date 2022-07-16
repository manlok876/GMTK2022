// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessStructs.h"
#include "BaseChessFigure.generated.h"

class AChessGrid;

UCLASS()
class GMTK2022_API ABaseChessFigure : public APawn
{
	GENERATED_BODY()

public:
	ABaseChessFigure();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<FGridCoords> GetPossibleMoves() const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool CanGoTo(const FGridCoords& Cell) const;
	UFUNCTION(BlueprintCallable)
	bool TryGoTo(const FGridCoords& Cell);

protected:
	virtual void BeginPlay() override;

	AChessGrid* GetGrid();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		AChessGrid* Grid;

};
