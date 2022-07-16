// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessStructs.h"


FGridCoords::FGridCoords()
{
	Row = 0;
	Column = 0;
}

FGridCoords::FGridCoords(int InColumn, int InRow)
{
	Row = InRow;
	Column = InColumn;
}

bool FGridCoords::operator==(const FGridCoords& Point) const
{
	return this->Row == Point.Row && this->Column == Point.Column;
}

bool FGridCoords::operator!=(const FGridCoords& Point) const
{
	return this->Row != Point.Row || this->Column != Point.Column;
}

FGridCoords FGridCoords::operator+(const FGridCoords& Point) const
{
	return FGridCoords(this->Column + Point.Column, this->Row + Point.Row);
}

FGridCoords FGridCoords::operator-(const FGridCoords& Point) const
{
	return FGridCoords(this->Column - Point.Column, this->Row - Point.Row);
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
