// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessStructs.h"


FGridCoords::FGridCoords()
{
	Row = 0;
	Column = 0;
}

FFigureInfo::FFigureInfo()
{
	Color = EChessColor::ECC_White;
	Type = EFigureType::EFT_Pawn;
}

FFigureSetupData::FFigureSetupData()
{

}

UBoardSetup::UBoardSetup()
{

}
