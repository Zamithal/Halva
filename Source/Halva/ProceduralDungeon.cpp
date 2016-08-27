// Fill out your copyright notice in the Description page of Project Settings.

#include "Halva.h"
#include "ProceduralDungeon.h"

/**********************************************************************************************************
*	DungeonLayout()
*		Purpose:	Constructor.
**********************************************************************************************************/
AProceduralDungeon::AProceduralDungeon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_randomStream = FRandomStream(randomSeed);
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
*				Assuming that there is at least one tile type for each given tile, a static mesh instance
*				will be added for each tile on the map. The tile will be a random choice between all tiles
*				of the specified tile type.
**********************************************************************************************************/
void AProceduralDungeon::GenerateTiles()
{
	InitializeTileArrays();

	m_dungeonLayout = DungeonLayout(dungeonSize, smallestRoomSize, desiredRooms, pathWidth, m_randomStream);

	CreateTileMeshes();
}
/**********************************************************************************************************
*	void InitializeTileArrays()
*		Purpose:	Clears the tile arrays and initializes them for tile data.
*
*		Changes:
*			m_tileMeshes
*				Each type of tile will get an initialized static mesh instance with no
*				instances.
**********************************************************************************************************/
void AProceduralDungeon::InitializeTileArrays()
{
	// UE4 Does not allow nested containers to interact with blueprints.
	// This step restructures the layout of data so that it is not blueprint accessible but much more 
	// usable. To find a tile of a specific type use tileMeshes[TileType][i].
	for (int i = 0; i < NUMBER_OF_TILE_TYPES; i++)
		tileMeshes[i].Empty();

	// Set the size of each.
	tileMeshes[emptyTile].SetNum(emptyTileTypes.Num(), false);
	tileMeshes[floorTile].SetNum(floorTileTypes.Num(), false);
	tileMeshes[wallTile].SetNum(wallTileTypes.Num(), false);
	tileMeshes[outsideCornerTile].SetNum(outsideCornerTileTypes.Num(), false);
	tileMeshes[insideCornerTile].SetNum(insideCornerTileTypes.Num(), false);

	// Initialize their static mesh's with no instances.
	for (int i = 0; i < tileMeshes[emptyTile].Num(); i++)
	{
		tileMeshes[emptyTile][i] = NewObject<UInstancedStaticMeshComponent>();
		tileMeshes[emptyTile][i]->SetStaticMesh(emptyTileTypes[i]);
	}
	for (int i = 0; i < tileMeshes[floorTile].Num(); i++)
	{
		tileMeshes[floorTile][i] = NewObject<UInstancedStaticMeshComponent>();
		tileMeshes[floorTile][i]->SetStaticMesh(floorTileTypes[i]);
	}
	for (int i = 0; i < tileMeshes[wallTile].Num(); i++)
	{
		tileMeshes[wallTile][i] = NewObject<UInstancedStaticMeshComponent>();
		tileMeshes[wallTile][i]->SetStaticMesh(wallTileTypes[i]);
	}
	for (int i = 0; i < tileMeshes[outsideCornerTile].Num(); i++)
	{
		tileMeshes[outsideCornerTile][i] = NewObject<UInstancedStaticMeshComponent>();
		tileMeshes[outsideCornerTile][i]->SetStaticMesh(outsideCornerTileTypes[i]);
	}
	for (int i = 0; i < tileMeshes[insideCornerTile].Num(); i++)
	{
		tileMeshes[insideCornerTile][i] = NewObject<UInstancedStaticMeshComponent>();
		tileMeshes[insideCornerTile][i]->SetStaticMesh(insideCornerTileTypes[i]);
	}

	// attach each to the root component.
	for (int i = 0; i < NUMBER_OF_TILE_TYPES; i++)
	{
		for (int j = 0; j < tileMeshes[i].Num(); j++)
		{
			tileMeshes[i][j]->AttachTo(GetRootComponent());
		}
	}

}
/**********************************************************************************************************
*	void CreateTileMeshes()
*		Purpose:	Constructs the meshes for the tiles. Iterates through the entire dungeon layout once
*					for each tile type there is. If a tile is found that should match the type of tile,
*					a random one is picked from the tile types and a mesh is generated.
*
*		Changes:
*			m_tileMeshes
*				Assuming that there is at least one tile type for each given tile, a static mesh instance
*				will be added for each tile on the map. The tile will be a random choice between all tiles
*				of the specified tile type.
**********************************************************************************************************/
void AProceduralDungeon::CreateTileMeshes()
{
	FVector dungeonDimensions = m_dungeonLayout.GetDungeonDimensions();
	TileData ** layout = m_dungeonLayout.GetDungeonLayout();

	for (int i = 0; i < NUMBER_OF_TILE_TYPES; i++)
	{
		if (tileMeshes[i].Num() > 0)
		{
			for (int y = 0; y < dungeonDimensions.Y; y++)
			{
				for (int x = 0; x < dungeonDimensions.X; x++)
				{
					// If the tile at that location is the type of this pass.
					if (layout[y][x].tileType == i)
					{
						// Create translation.
						FVector tileLocation = FVector(tileDimensions.X * x, tileDimensions.Y * y, 0);

						// Create Transform
						FTransform tileTransform = FTransform(layout[y][x].tileRotation, tileLocation, FVector(1, 1, 1));

						// Pick a random tile.
						int randomIndex = m_randomStream.RandRange(0, tileMeshes[i].Num() - 1);

						tileMeshes[i][randomIndex]->AddInstance(tileTransform);
					}
				}
			}

		}
	}
}