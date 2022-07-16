// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessUtilityFunctions.h"


int UChessUtilityFunctions::GetValueForType(EFigureType Type)
{
	int Result = 0;

	switch (Type)
	{
	case EFigureType::EFT_Pawn:
		Result = 2;
		break;
	case EFigureType::EFT_Rook:
		Result = 8;
		break;
	case EFigureType::EFT_Knight:
		Result = 4;
		break;
	case EFigureType::EFT_Bishop:
		Result = 6;
		break;
	case EFigureType::EFT_Queen:
		Result = 10;
		break;
	case EFigureType::EFT_King:
		Result = 12;
		break;
	default:
		break;
	}

	return Result;
}

EFigureType UChessUtilityFunctions::GetTypeForValue(int Value)
{
	EFigureType Result = EFigureType::EFT_None;

	switch (Value)
	{
	case 2:
		Result = EFigureType::EFT_Pawn;
		break;
	case 8:
		Result = EFigureType::EFT_Rook;
		break;
	case 4:
		Result = EFigureType::EFT_Knight;
		break;
	case 6:
		Result = EFigureType::EFT_Bishop;
		break;
	case 10:
		Result = EFigureType::EFT_Queen;
		break;
	case 12:
		Result = EFigureType::EFT_King;
		break;
	default:
		break;
	}

	return Result;
}
