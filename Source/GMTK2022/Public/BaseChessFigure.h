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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FGridCoords> GetPossibleMoves();
	UFUNCTION(BlueprintCallable)
	static bool CanEnterCell(
		const AChessGrid* Grid, const FGridCoords& Cell, EChessColor OurColor, EFigureType OurType);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanGoTo(const FGridCoords& Cell);
	UFUNCTION(BlueprintCallable)
	bool TryGoTo(const FGridCoords& Cell);

	UFUNCTION(BlueprintCallable)
	bool InitialPositionSet(const FGridCoords& Cell);

	UFUNCTION(BlueprintPure)
	bool IsMoveInProgress() const;
	FGridCoords GetPendingDestination() const;

	EChessColor GetFigureColor() const;

	EFigureType GetFigureType() const;
	UFUNCTION(BlueprintCallable)
	void SetFigureType(EFigureType NewType);

	ABaseChessFigure* GetPendingTarget() const;
	UFUNCTION(BlueprintPure)
	bool IsInteractionPending() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ClearInteraction();
	UFUNCTION(BlueprintPure)
	bool IsEnemy(ABaseChessFigure* Target) const;
	UFUNCTION(BlueprintPure)
	bool IsAlly(ABaseChessFigure* Target) const;

	UFUNCTION(BlueprintCallable)
	int PerformRoll(int AdditiveMod = 0);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PrepareToInteract(ABaseChessFigure* Target);

	UFUNCTION(BlueprintCallable)
	void BeginInteraction(ABaseChessFigure* Target);
	UFUNCTION()
	void BeginCombat(ABaseChessFigure* Target);
	UFUNCTION()
	void BeginMerge(ABaseChessFigure* Target);

	UFUNCTION()
	void EndInteraction();
	UFUNCTION()
	void EndCombat();
	UFUNCTION()
	void EndMerge();

	UPROPERTY(BlueprintAssignable)
	FChessMoveEndedSignature MoveEndedDispatcher;

	UFUNCTION(BlueprintCallable)
	static TArray<FGridCoords> GetPawnMoves(
		const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color, bool bIsFirstMove = false);
	UFUNCTION(BlueprintCallable)
	static TArray<FGridCoords> GetRookMoves(
		const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color, bool bIsFirstMove = false);
	UFUNCTION(BlueprintCallable)
	static TArray<FGridCoords> GetKnightMoves(
		const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color);
	UFUNCTION(BlueprintCallable)
	static TArray<FGridCoords> GetBishopMoves(
		const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color);
	UFUNCTION(BlueprintCallable)
	static TArray<FGridCoords> GetQueenMoves(
		const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color);
	UFUNCTION(BlueprintCallable)
	static TArray<FGridCoords> GetKingMoves(
		const AChessGrid* Grid, const FGridCoords& StartingPoint, EChessColor Color, bool bIsFirstMove = false);

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
	void OnTypeChanged(EFigureType NewType);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AChessGrid* CachedGrid;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGridCoords CurrentPosition;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGridCoords PendingDestination;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABaseChessFigure* PendingTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ExposeOnSpawn))
	EChessColor FigureColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EFigureType FigureType;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bHasMoved;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int AttackBonus;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int OurSavedRoll;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int EnemySavedRoll;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bAwaitingDestroy;

};
