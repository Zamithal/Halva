// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

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
*	Manager Functions:
*		?
*
*	Mutators:
*
*	Methods:
*
*		GenerateTiles()
*			Generates a new set of tiles for the level using the tile arrays. The layout of the dungeon is
*			dependent on the random seed. If no tiles are in the particular array, instead nothing will be
*			created.
*		
*	Data Members:
*		int RandomSeed - The random seed to generate a dungeon with.
*		TArray<class UStaticMesh *> EmptyTiles - An array containing a list of all the types of tiles that
*												 could be used when an empty tile is required.
*		TArray<class UStaticMesh *> floorTiles - An array containing a list of all the types of tiles that
*												 could be used when an floor tile is required.
*		TArray<class UStaticMesh *> wallTiles - An array containing a list of all the types of tiles that
*												 could be used when an wall tile is required.
*		TArray<class UStaticMesh *> outsideCornerTiles - An array containing a list of all the types of 
*														 tiles that could be used when an outside corner
*														 tile is required.
*		TArray<class UStaticMesh *> insideCornerTiles - An array containing a list of all the types
*														of tiles that could be used when an inside corner
*														tile is required.
*		TArray<UInstancedStaticMeshComponent *> emptyTiles - Container for actual mesh.
*		TArray<UInstancedStaticMeshComponent *> floorTiles - Container for actual mesh.
*		TArray<UInstancedStaticMeshComponent *> wallTiles - Container for actual mesh.
*		TArray<UInstancedStaticMeshComponent *> outsideCornerTiles - Container for actual mesh.
*		TArray<UInstancedStaticMeshComponent *> insideCornerTiles - Container for actual mesh.
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

	UFUNCTION(BlueprintCallable, Category = "ProceduralLevel")
		void GenerateTiles();

	// UE4 Does not allow nested containers to interact with blueprints.
	// Because of this parallel arrays will be used instead of nested arrays.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomSeed")
		int RandomSeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> emptyTileTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> floorTileTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> wallTileTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> insideCornerTileTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> outsideCornerTileTypes;

	UPROPERTY(BlueprintReadOnly, Category = "Tiles")
		TArray<UInstancedStaticMeshComponent *> emptyTiles;

	UPROPERTY(BlueprintReadOnly, Category = "Tiles")
		TArray<UInstancedStaticMeshComponent *> floorTiles;

	UPROPERTY(BlueprintReadOnly, Category = "Tiles")
		TArray<UInstancedStaticMeshComponent *> wallTiles;

	UPROPERTY(BlueprintReadOnly, Category = "Tiles")
		TArray<UInstancedStaticMeshComponent *> outsideCornerTiles;

	UPROPERTY(BlueprintReadOnly, Category = "Tiles")
		TArray<UInstancedStaticMeshComponent *> insideCornerTiles;
};
