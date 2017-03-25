#pragma once
/**********************************************************************************************************
*	enum TileType
*
*		Purpose:
*			Used to determine what tile should be selected for the current location.
**********************************************************************************************************/
enum TileType
{
	emptyTile,
	floorTile,
	wallTile,
	outsideCornerTile,
	insideCornerTile,
	pillarTile
};
/**********************************************************************************************************
*	struct TileData
*
*		Purpose:
*			Contains useful data for placing and orienting tiles.
**********************************************************************************************************/
struct TileData
{
	TileType tileType;
	FRotator tileRotation;
	FVector2D tileLocation;
};