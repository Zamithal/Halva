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
	


	UCapsuleComponent * caps = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Player Collider"));
	// enact physics on collider.
	caps->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	caps->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	caps->SetVisibility(true);
	caps->SetHiddenInGame(false);
	caps->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Used to keep the collision capsule in-line with the camera.
	UChildTransformConsumerComponent * cameraAligner = CreateDefaultSubobject<UChildTransformConsumerComponent>(TEXT("Camera Consumer"));
	//cameraAligner->SetTransformIgnored(FTransformAspects::allRoations);
	cameraAligner->SetTransformIgnored(FTransformAspects::allScaling);
	cameraAligner->SetTransformIgnored(FTransformAspects::zVector);
	cameraAligner->AttachToComponent(caps, FAttachmentTransformRules::KeepRelativeTransform);

	// Create camera.
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Head Mounted Display"));
	m_Camera->AttachToComponent(cameraAligner, FAttachmentTransformRules::KeepRelativeTransform);

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
	AddOwnedComponent(m_Chaperone);

	// Used to enforce that the player collider does not copy the rotation of the camera.
	//USpringArmComponent * springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Player Capsule Spring Arm"));
	//springArm->AttachTo(m_Camera);

	//springArm->TargetArmLength = 0;
	//springArm->SetAbsolute(false, true, false);

	// Create ground collider.
	//m_GroundCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Player Capsule Collider"));
	//m_GroundCollider->AttachTo(springArm);

	//m_GroundCollider->SetVisibility(true);
	//m_GroundCollider->SetHiddenInGame(false);

	// create movementComponent
	//m_MovementComponent = CreateDefaultSubobject<UViveMovementComponent>(TEXT("PlayerMovementComponent"));
	//AddOwnedComponent(m_MovementComponent);

	// enact physics on collider.
	//m_GroundCollider->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	//m_GroundCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


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

