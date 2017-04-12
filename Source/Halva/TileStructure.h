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
	oneSidedWallTile,
	twoSidedWallTile,
	threeSidedWallTile,
	outsideCornerTile,
	insideSingleCornerTile,
	insideDoubleAdjacentCornerTile,
	insideDoubleOppositeCornerTile,
	insideTripleCornerTile,
	insideQuadraCornerTile,
	pillarTile,
	lBendTile,
	tJuctionTile,
	wallCornerCompositeTile,
	wallCornerCompositeReversedTile,

	// The number of tile types there are.
	TileType_MAX
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