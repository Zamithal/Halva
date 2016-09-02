// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "PlayerVive.h"


// Sets default values
APlayerVive::APlayerVive()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerVive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerVive::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APlayerVive::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

