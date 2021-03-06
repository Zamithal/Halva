// © 2016 Skyler Swenson, some rights reserved.
#include "Halva.h"
#include "Item.h"

/**********************************************************************************************************
*    AItem::AItem()
*        Purpose:    Default constructor.
**********************************************************************************************************/
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));

	RootComponent = ItemMesh;


	Name = "Mysterious object";
	Description = "Seems like the developer forgot to write a description for this item. Whoops!";
}
/**********************************************************************************************************
*    AItem::OnConstruction(const FTransform& Transform)
*        Purpose:    UE4 Construction script.
**********************************************************************************************************/
void AItem::OnConstruction(const FTransform& Transform)
{

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
/**********************************************************************************************************
*    AItem::CalculateDamage(AActor* TriggeringActor = nullptr)
*        Purpose:	 Returns the damage this will deal upon collision this frame. Always returns 0.
*
*		 Parameters:
*			 AActor* TriggeringActor
*				 The actor that triggered this call, ignored unless used in an override.
**********************************************************************************************************/
float AItem::CalculateDamage(AActor* TriggeringActor)
{
	return 0;
}