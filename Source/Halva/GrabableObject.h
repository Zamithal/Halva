// © 2016 SKYLER SWENSON SOME RIGHTS RESERVED

#pragma once

#include "GameFramework/Actor.h"
#include "GrabableObject.generated.h"
/**********************************************************************************************************
*	Class: GrabableObject
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
*	UE4 Functions:
*		AProceduralDungeon()
*			Constructor.
*		OnConstruction()
*			Creates the dungeon each time a parameter is changed, the object is loaded or spawned.
*
*	Mutators:
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
*		CreateTileLightMap()
*			Creates the lightmap for all created tiles allowing the use of static lighting.
*
*	Data Members:
**********************************************************************************************************/
UCLASS()
class HALVA_API AGrabableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrabableObject();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
