// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "VRPawn.h"


// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create camera.
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Head Mounted Display"));
	m_Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create player simple hit-box.
	m_CharacterCollider = CreateDefaultSubobject<UVRCameraCapsuleComponent>(TEXT("Player Collider"));
	m_CharacterCollider->SetCamera(m_Camera);
	m_CharacterCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	UArrowComponent * visualizer = CreateDefaultSubobject<UArrowComponent>(TEXT("Visualizer"));
	visualizer->SetVisibility(true);
	visualizer->SetHiddenInGame(false);
	visualizer->AttachToComponent(m_CharacterCollider, FAttachmentTransformRules::KeepRelativeTransform);

	// Create motion controllers.
	m_Controller1 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 1"));
	m_Controller2 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 2"));
	m_Controller1->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	m_Controller2->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create vision restrictor.
	m_FOVRestrictorComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FOV Restrictor"));


	m_LeftController = m_Controller1;
	m_RighController = m_Controller2;
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	if (FOVRestrictorWidget)
		m_FOVRestrictorComponent->SetWidget(FOVRestrictorWidget);
	
}

// Called every frame
void AVRPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

