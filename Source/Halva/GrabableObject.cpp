// © 2016 SKYLER SWENSON SOME RIGHTS RESERVED

#include "Halva.h"
#include "GrabableObject.h"


// Sets default values
AGrabableObject::AGrabableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGrabableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrabableObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

