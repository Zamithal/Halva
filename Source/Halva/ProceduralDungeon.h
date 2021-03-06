// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "DungeonLayout.h"
#include "GameFramework/Actor.h"
#include "ProceduralDungeon.generated.h"

/**********************************************************************************************************
*	Class: ProceduralDungeon
*
*	Overview:
*		This is an Actor that generates a complete procedural level for a dungeon. See the below
*		subsections for details.
*
*		Layout:
*			The dungeon is made up of many instanced static mesh tiles 100 Unreal units in length and
*			width. When a tile is needed, a random tile will be chosen from the corresponding TArray. In
*			example, if a floor tile is required, a random will be spawned from FloorTiles. The tiles
*			will be laid out in such a way that rooms and paths will be generated. Each room will be
*			accessible from any other room.
*
*	Methods:
*
*		GenerateTiles()
*			Generates a new set of tiles for the level using the tile arrays. The layout of the dungeon is
*			dependent on the random seed. If no tiles are in the particular array, instead nothing will be
*			created.
*		InitializeTileArrays()
*			Empties each tile array and sets its static mesh to the corresponding static mesh in types.
*		CreateTileMeshes()
*			Takes the data from the dungeon layout and creates tiles. Tiles are instanced static meshes and
*			there is one for each tile type specified.
*		
*	Data Members:
*		int RandomSeed
*			The random seed to generate a dungeon with.
*		FVector dungeonSize
*			The total number of tiles in each direction. 100x100 is 100 tiles across each.
*		FVector smallestRoomSize
*			The smallest number of tiles a room can be in each direction.
*		int desiredRooms
*			The number of rooms to attempt to make.
*		int pathWidth
*			The width of paths connecting rooms.
*		FVector tileDimensions
*			How big each tile is in unreal units. Affects spacing of each tile.
*		TArray<class UStaticMesh *> EmptyTiles
*			An array containing a list of all the types of tiles that could be used when an empty tile is 
*			required. There is one for each type of tile.
*		TArray<UInstancedStaticMeshComponent *> tileMeshes[]
*			Contains all actual static meshes used to build the level.
*		FRandomStream m_randomStream
*			The random stream used to generate randomization for the dungeon.
*		DungeonLayout m_dungeonLayout
*			Contains the layout information for the dungeon.
**********************************************************************************************************/
UCLASS()
class HALVA_API AProceduralDungeon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralDungeon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void OnConstruction(const FTransform & Transform) override;

	void GenerateTiles();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomSeed")
		int randomSeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonLayout")
		FVector dungeonSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonLayout")
		FVector smallestRoomSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonLayout")
		int desiredRooms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonLayout")
		int pathWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonLayout")
		FVector tileDimensions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonLayout")
		int erosionPasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonLayout")
		float erosionChance;

	// Parallel arrays are used for user entering data's convenience.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> emptyTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> floorTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> singleWallTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> doubleWallTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> tripleWallTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> outsideCornerTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> singleInsideCornerTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> doubleAdjacentInsideCornerTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> doubleOppositeInsideCornerTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> tripleInsideCornerTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> quadraInsideCornerTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> pillarTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> lBendTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> tJunctionTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> wallCornerCompositeTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> wallCornerCompositeReversedTiles;

protected:

	TArray<UInstancedStaticMeshComponent *> m_tileMeshes[TileType::TileType_MAX];
	
	TArray<class UStaticMesh*> m_TILE_TYPE_CONTAINER[TileType::TileType_MAX];

	void InitializeTileArrays();
	void CreateTileMeshes();

	FRandomStream m_randomStream;
	DungeonLayout m_dungeonLayout;

};
