// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessStructs.h"
#include "BaseChessFigure.generated.h"

class AChessGrid;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChessMoveEndedSignature, ABaseChessFigure*, MovedFigure);

UCLASS()
class GMTK2022_API ABaseChessFigure : public APawn
{
	GENERATED_BODY()

public:
	ABaseChessFigure();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure = false)
	TArray<FGridCoords> GetPossibleMoves() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure = false)
	bool CanGoTo(const FGridCoords& Cell) const;
	UFUNCTION(BlueprintCallable)
	bool TryGoTo(const FGridCoords& Cell);

	UFUNCTION(BlueprintPure)
	bool IsMoveInProgress() const;
	FGridCoords GetPendingDestination() const;

	EChessColor GetFigureColor() const;

	ABaseChessFigure* GetPendingCombatTarget() const;
	UFUNCTION(BlueprintPure)
	bool IsCombatPending() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PrepareToDefend(ABaseChessFigure* Attacker);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearPendingCombat();

	UPROPERTY(BlueprintAssignable)
	FChessMoveEndedSignature MoveEndedDispatcher;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	AChessGrid* GetGrid();

	UFUNCTION(BlueprintImplementableEvent)
	void StartMovementAnimation(const FGridCoords& Cell);
	UFUNCTION(BlueprintCallable)
	void EndMovementAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void StartCombatAnimation(ABaseChessFigure* Target, bool bIsAttacker);
	UFUNCTION(BlueprintCallable)
	void EndCombatAnimation();

	UFUNCTION()
	void EndMove();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveEnded();

	UFUNCTION(BlueprintImplementableEvent)
	void OnColorChanged(EChessColor NewColor);
	UFUNCTION(BlueprintImplementableEvent)
	void OnValueChanged(int NewValue);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AChessGrid* CachedGrid;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsMoveInProgress;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGridCoords PendingDestination;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABaseChessFigure* PendingCombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ExposeOnSpawn))
	EChessColor FigureColor;

};
