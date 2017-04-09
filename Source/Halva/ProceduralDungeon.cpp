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

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Procedural Dungeon Root"));

	randomSeed = 0;
	dungeonSize = FVector(10, 10, 0);
	smallestRoomSize = FVector(4, 4, 0);
	desiredRooms = 1;
	pathWidth = 3;
	tileDimensions = FVector(10, 10, 0);
	erosionPasses = 0;
	erosionChance = 0;
}

// Called when the game starts or when spawned
void AProceduralDungeon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProceduralDungeon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
void AProceduralDungeon::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	m_randomStream = FRandomStream(randomSeed);

	GenerateTiles();
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
	TArray<USceneComponent*> oldChildren = TArray<USceneComponent*>();

	RootComponent->GetChildrenComponents(true, oldChildren);

	for (int i = 0; i < oldChildren.Num(); i++)
	{
		oldChildren[i]->DestroyComponent();
	}

	InitializeTileArrays();

	m_dungeonLayout = DungeonLayout(dungeonSize, smallestRoomSize, desiredRooms, pathWidth, erosionPasses, erosionChance, m_randomStream);

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
	for (int i = 0; i < TileType::TileType_MAX; i++)
	{
		m_tileMeshes[i] = TArray<UInstancedStaticMeshComponent *>();
	}

	// reformats the parallel arrays into a much more workable format.
	// NOTE: This is only safe as long as parallel array values never change.
	m_TILE_TYPE_CONTAINER[TileType::emptyTile] = emptyTiles;
	m_TILE_TYPE_CONTAINER[TileType::floorTile] = floorTiles;
	m_TILE_TYPE_CONTAINER[TileType::oneSidedWallTile] = singleWallTiles;
	m_TILE_TYPE_CONTAINER[TileType::twoSidedWallTile] = doubleWallTiles;
	m_TILE_TYPE_CONTAINER[TileType::threeSidedWallTile] = tripleWallTiles;
	m_TILE_TYPE_CONTAINER[TileType::outsideCornerTile] = outsideCornerTiles;
	m_TILE_TYPE_CONTAINER[TileType::insideSingleCornerTile] = singleInsideCornerTiles;
	m_TILE_TYPE_CONTAINER[TileType::insideDoubleAdjacentCornerTile] = doubleAdjacentInsideCornerTiles;
	m_TILE_TYPE_CONTAINER[TileType::insideDoubleOppositeCornerTile] = doubleOppositeInsideCornerTiles;
	m_TILE_TYPE_CONTAINER[TileType::insideTripleCornerTile] = tripleInsideCornerTiles;
	m_TILE_TYPE_CONTAINER[TileType::insideQuadraCornerTile] = quadraInsideCornerTiles;
	m_TILE_TYPE_CONTAINER[TileType::pillarTile] = pillarTiles;


	for (int i = 0; i < TileType::TileType_MAX; i++)
	{
		for (int j = 0; j < m_TILE_TYPE_CONTAINER[i].Num(); j++)
		{
			// create container.
			m_tileMeshes[i].Add(NewObject<UInstancedStaticMeshComponent>(this));

			m_tileMeshes[i][j]->bCastDynamicShadow = false;

			if (m_TILE_TYPE_CONTAINER[i][j] != nullptr)
				m_tileMeshes[i][j]->SetStaticMesh(m_TILE_TYPE_CONTAINER[i][j]);

			// register container.
			m_tileMeshes[i][j]->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			m_tileMeshes[i][j]->RegisterComponent();
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

	for (int i = 0; i < TileType::TileType_MAX; i++)
	{
		if (m_tileMeshes[i].Num() > 0)
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
						int randomIndex = m_randomStream.RandRange(0, m_tileMeshes[i].Num() - 1);

						// Add a instance
						if (m_tileMeshes[i][randomIndex]->GetStaticMesh() != nullptr)
							m_tileMeshes[i][randomIndex]->AddInstance(tileTransform);
					}
				}
			}

		}
	}
}