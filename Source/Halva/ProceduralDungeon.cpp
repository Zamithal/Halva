// Fill out your copyright notice in the Description page of Project Settings.

#include "Halva.h"
#include "ProceduralDungeon.h"
#include "DungeonLayout.h"

/**********************************************************************************************************
*	DungeonLayout()
*		Purpose:	Constructor.
**********************************************************************************************************/
AProceduralDungeon::AProceduralDungeon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	randomStream = FRandomStream(randomSeed);
}

// Called when the game starts or when spawned
void AProceduralDungeon::BeginPlay()
{
	Super::BeginPlay();
	GenerateTiles();
}

// Called every frame
void AProceduralDungeon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

/**********************************************************************************************************
*	void GenerateTiles()
*		Purpose:	Creates a dungeon layout and places a tile at each location in the layout. The tile
*					chosen must match the type stated by the layout but can be of any variety provided in
*					its respective array. Example: If a floor tile is required, any floor tile from
*					floorTiles could be chosen for that location. If the required tile type has no valid
*					tile meshes, no mesh is picked.
*
*		Changes:
*			emptyTiles - Static meshes instances will be created if possible.
*			floorTiles - Static meshes instances will be created if possible.
*			wallTiles - Static meshes instances will be created if possible.
*			outsideCornerTiles - Static meshes instances will be created if possible.
*			insideCornerTiles - Static meshes instances will be created if possible.
**********************************************************************************************************/
void AProceduralDungeon::GenerateTiles()
{
	InitializeTileArrays();

	DungeonLayout newDungeon = DungeonLayout(dungeonSize, smallestRoomSize, desiredRooms, pathWidth, randomStream);
}
/**********************************************************************************************************
*	void InitializeTileArrays()
*		Purpose:	Clears the tile arrays and initializes them for tile data.
*
*		Changes:
*			emptyTiles - Will be cleared and set to the static mesh at it's parallel array.
*			floorTiles - Will be cleared and set to the static mesh at it's parallel array.
*			wallTiles - Will be cleared and set to the static mesh at it's parallel array.
*			outsideCornerTiles - Will be cleared and set to the static mesh at it's parallel array.
*			insideCornerTiles - Will be cleared and set to the static mesh at it's parallel array.
**********************************************************************************************************/
void AProceduralDungeon::InitializeTileArrays()
{
	// UE4 Does not allow nested containers to interact with blueprints.
	// Because of this parallel arrays will be used instead of nested arrays.
	emptyTiles.Empty();
	floorTiles.Empty();
	wallTiles.Empty();
	outsideCornerTiles.Empty();
	insideCornerTiles.Empty();

	// Set the size of each.
	emptyTiles.SetNum(emptyTileTypes.Num(), false);
	floorTiles.SetNum(floorTileTypes.Num(), false);
	wallTiles.SetNum(wallTileTypes.Num(), false);
	outsideCornerTiles.SetNum(outsideCornerTileTypes.Num(), false);
	insideCornerTiles.SetNum(insideCornerTileTypes.Num(), false);

	// Initialize their static mesh's with no instances.
	for (int i = 0; i < emptyTiles.Num(); i++)
	{
		emptyTiles[i] = NewObject<UInstancedStaticMeshComponent>();
		emptyTiles[i]->SetStaticMesh(emptyTileTypes[i]);
	}
	for (int i = 0; i < floorTiles.Num(); i++)
	{
		floorTiles[i] = NewObject<UInstancedStaticMeshComponent>();
		floorTiles[i]->SetStaticMesh(floorTileTypes[i]);
	}
	for (int i = 0; i < wallTiles.Num(); i++)
	{
		wallTiles[i] = NewObject<UInstancedStaticMeshComponent>();
		wallTiles[i]->SetStaticMesh(wallTileTypes[i]);
	}
	for (int i = 0; i < outsideCornerTiles.Num(); i++)
	{
		outsideCornerTiles[i] = NewObject<UInstancedStaticMeshComponent>();
		outsideCornerTiles[i]->SetStaticMesh(outsideCornerTileTypes[i]);
	}
	for (int i = 0; i < insideCornerTiles.Num(); i++)
	{
		insideCornerTiles[i] = NewObject<UInstancedStaticMeshComponent>();
		insideCornerTiles[i]->SetStaticMesh(insideCornerTileTypes[i]);
	}

}