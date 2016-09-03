// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "PlayerVive.h"


/**********************************************************************************************************
*	DungeonLayout()
*		Purpose:	Constructor.
**********************************************************************************************************/
APlayerVive::APlayerVive()
{
 	// Set this pawn to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create camera.
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Head Mounted Display"));
	m_Camera->AttachTo(RootComponent);


	// Create motion controllers.
	m_Controller1 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 1"));
	m_Controller2 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 2"));

	m_Controller1->AttachTo(RootComponent);
	m_Controller2->AttachTo(RootComponent);

	// default to left first.
	m_LeftController = m_Controller1;
	m_RighController = m_Controller2;

	// Create chaperone.
	m_Chaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVR Chaperone"));
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

