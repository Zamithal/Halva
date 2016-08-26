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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> EmptyTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> FloorTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> WallTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> InsideCornerTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tiles")
		TArray<class UStaticMesh *> OutsideCornerTiles;
	
};
