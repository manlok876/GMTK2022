// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessStructs.generated.h"


USTRUCT(BlueprintType)
struct FGridCoords
{
	GENERATED_BODY()

public:
	FGridCoords();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Column;
};
