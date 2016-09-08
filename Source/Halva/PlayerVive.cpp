// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "PlayerVive.h"


/**********************************************************************************************************
*	APlayerVive()
*		Purpose:	Default constructor.
**********************************************************************************************************/
APlayerVive::APlayerVive()
{
 	// Set this pawn to call Tick() every frame.
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create camera.
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Head Mounted Display"));
	m_Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create player simple hit-box.
	UVRCameraCapsuleComponent * VRCapsule = CreateDefaultSubobject<UVRCameraCapsuleComponent>(TEXT("Player Collider"));
	VRCapsule->SetCamera(m_Camera);
	VRCapsule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	UArrowComponent * visualizer = CreateDefaultSubobject<UArrowComponent>(TEXT("Visualizer"));
	visualizer->SetVisibility(true);
	visualizer->SetHiddenInGame(false);
	visualizer->AttachToComponent(VRCapsule, FAttachmentTransformRules::KeepRelativeTransform);

	// Create motion controllers.
	m_Controller1 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 1"));
	m_Controller2 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 2"));
	m_Controller1->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	m_Controller2->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create chaperone.
	m_Chaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVR Chaperone"));
	AddOwnedComponent(m_Chaperone);

	// Create vision restrictor.
	m_FOVRestrictorComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FOV Restrictor"));


	m_LeftController = m_Controller1;
	m_RighController = m_Controller2;
}

void APlayerVive::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
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

UPawnMovementComponent* APlayerVive::GetMovementComponent() const
{
	return m_MovementComponent;
}

