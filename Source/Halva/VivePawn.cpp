// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "VivePawn.h"


// Sets default values
AVivePawn::AVivePawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVivePawn::BeginPlay()
{
	Super::BeginPlay();
	if (this)
		return;
}

// Called every frame
void AVivePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVivePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

