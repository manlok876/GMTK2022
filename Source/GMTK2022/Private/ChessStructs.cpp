// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessStructs.h"


FGridCoords::FGridCoords()
{
	Row = 0;
	Column = 0;
}

FFigureInfo::FFigureInfo()
{
	Type = EFigureType::EFT_Pawn;
	Color = EChessColor::ECC_White;
}

FFigureSetupData::FFigureSetupData()
{

}

UBoardSetup::UBoardSetup()
{

}
