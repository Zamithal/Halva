// Fill out your copyright notice in the Description page of Project Settings.

#include "Halva.h"
#include "ProceduralDungeon.h"


// Sets default values
AProceduralDungeon::AProceduralDungeon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

/**********************************************************************************************************
*	void GenerateTiles()
*		Purpose:	Creates a dungeon layout and places a tile at each location in the layout. The tile
*					chosen must match the type stated by the layout but can be of any variety provided in
*					its respective array. Example: If a floor tile is required, any floor tile from
*					floorTiles could be chosen for that location. If the required tile type has no valid
*					tile meshes, no mesh is picked.
*
*		Changes:
*			m_tiles - Static meshes instances will be created for each tile if possible.
**********************************************************************************************************/
void AProceduralDungeon::GenerateTiles()
{
	// Nested containers are not supported by UE4.
	//tiles.Empty();
}