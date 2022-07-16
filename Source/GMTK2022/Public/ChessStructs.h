// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ChessStructs.generated.h"


USTRUCT(BlueprintType)
struct FGridCoords
{
	GENERATED_BODY()

public:
	FGridCoords();
	FGridCoords(int InColumn, int InRow);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Column;

	bool operator==(const FGridCoords& Point) const;
	bool operator!=(const FGridCoords& Point) const;

	FGridCoords operator+(const FGridCoords& Point) const;
	FGridCoords operator-(const FGridCoords& Point) const;

};

UENUM(BlueprintType)
enum class EChessColor : uint8 {
	ECC_White	UMETA(DisplayName = "White"),
	ECC_Black	UMETA(DisplayName = "Black")
};

UENUM(BlueprintType)
enum class EFigureType : uint8 {
	EFT_Pawn = 2	UMETA(DisplayName = "Pawn (2)"),
	EFT_Knight = 4	UMETA(DisplayName = "Knight (4)"),
	EFT_Bishop = 6	UMETA(DisplayName = "Bishop (6)"),
	EFT_Rook = 8	UMETA(DisplayName = "Rook (8)"),
	EFT_Queen = 10	UMETA(DisplayName = "Queen (10)"),
	EFT_King = 12	UMETA(DisplayName = "King (12)"),
	EFT_None = 0	UMETA(DisplayName = "Invalid")
};

USTRUCT(BlueprintType)
struct FFigureInfo
{
	GENERATED_BODY()

public:
	FFigureInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFigureType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EChessColor Color;
};

USTRUCT(BlueprintType)
struct FFigureSetupData
{
	GENERATED_BODY()

public:
	FFigureSetupData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFigureInfo Figure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridCoords Position;
};

UCLASS(BlueprintType)
class GMTK2022_API UBoardSetup : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UBoardSetup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFigureSetupData> Figures;
};

