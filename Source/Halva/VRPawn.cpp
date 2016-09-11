// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "VRPawn.h"

/**********************************************************************************************************
*	DungeonLayout()
*		Purpose:	Constructor. Creates default components for pawn.
**********************************************************************************************************/
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent * rootCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	RootComponent = rootCapsule;

	// Create motion controllers.
	m_Controller1 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 1"));
	m_Controller2 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 2"));
	m_Controller1->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	m_Controller2->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create camera.
	m_Camera = CreateDefaultSubobject<UVRCameraComponent>(TEXT("Camera"));
	m_Camera->SetLockToHmdRotation(true);
	m_Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// Cause the root of the object to move with the camera.
	m_Camera->GetPostCameraUpdateTickFunction()->BindUObject<AVRPawn>(this, &AVRPawn::MoveRootWithCamera);

	// Create vision restrictor.
	m_FOVRestrictorComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FOV Restrictor"));
	// 8.5 is the closest to the camera achievable but results in flickering.
	m_FOVRestrictorComponent->AddLocalOffset(FVector(11, 0, 0));
	m_FOVRestrictorComponent->AddLocalRotation(FRotator(0, 180, 0));
	m_FOVRestrictorComponent->SetWorldScale3D(FVector(1, .108, .06));
	m_FOVRestrictorComponent->SetVisibility(true);
	m_FOVRestrictorComponent->SetHiddenInGame(false);
	m_FOVRestrictorComponent->SetBlendMode(EWidgetBlendMode::Transparent);

	m_FOVRestrictorComponent->AttachToComponent(m_Camera, FAttachmentTransformRules::KeepRelativeTransform);

	m_LeftController = m_Controller1;
	m_RightController = m_Controller2;
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

/**********************************************************************************************************
*	UMotionControllerComponent * GetLeftMotionController()
*		Purpose:	Getter.
**********************************************************************************************************/
UMotionControllerComponent *  AVRPawn::GetLeftMotionController()
{
	return m_LeftController;
}
/**********************************************************************************************************
*	UMotionControllerComponent * GetRightMotionController()
*		Purpose:	Getter.
**********************************************************************************************************/
UMotionControllerComponent * AVRPawn::GetRightMotionController()
{
	return m_RightController;
}

void AVRPawn::MoveRootWithCamera(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	const FTransform parentWorld = RootComponent->GetComponentToWorld();
	GEngine->HMDDevice->SetupLateUpdate(parentWorld, RootComponent);

	if (m_Camera->GetCouldGetHmd())
		RootComponent->AddLocalOffset(m_Camera->GetHmdDelta());
}
