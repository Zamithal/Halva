// © 2016 Skyler Swenson, some rights reserved.
#include "Halva.h"
#include "Item.h"

/**********************************************************************************************************
*    AItem()
*        Purpose:    Default constructor.
**********************************************************************************************************/
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Name = "Mysterious object";
	Description = "Seems like the developer forgot to write a description for this item. Whoops!";
	Mesh = nullptr;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}