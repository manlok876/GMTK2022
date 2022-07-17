// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChessStructs.h"
#include "ChessUtilityFunctions.generated.h"

/**
 * 
 */
UCLASS()
class GMTK2022_API UChessUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static int GetValueForType(EFigureType Type);

	UFUNCTION(BlueprintPure)
	static EFigureType GetTypeForValue(int Value);

	UFUNCTION(BlueprintPure)
	static int GetMaxValue();
	UFUNCTION(BlueprintPure)
	static bool CanCombineFigures(EFigureType FirstFigure, EFigureType SecondFigure);
	UFUNCTION(BlueprintPure)
	static EFigureType GetMergeResult(EFigureType FirstFigure, EFigureType SecondFigure);

};
