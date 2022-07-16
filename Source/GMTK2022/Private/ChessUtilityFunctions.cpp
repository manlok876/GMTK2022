// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessUtilityFunctions.h"


int UChessUtilityFunctions::GetValueForType(EFigureType Type)
{
	return (int)Type;
}

EFigureType UChessUtilityFunctions::GetTypeForValue(int Value)
{
	return EFigureType(Value);
}
