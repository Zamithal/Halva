// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemy.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackBoard/BlackboardKeyAllTypes.h"
/**********************************************************************************************************
*	ABaseEnemy()
*		Purpose:	Constructor.
**********************************************************************************************************/
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_aggressionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AgressionRangeCollider"));

	m_aggressionCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	m_dodgeSenseCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DodgeSenseCollider"));

	m_dodgeSenseCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

/**********************************************************************************************************
*	void OnConstruction(const FTransform& Transform)
*		Purpose:	UE4 Construction script.
**********************************************************************************************************/
void ABaseEnemy::OnConstruction(const FTransform& Transform)
{
	if (BehaviorTree == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("ABaseEnemy Actor requires a BehaviorTree Component."));
		return;
	}

	m_aggressionCollider->SetSphereRadius(AggressionRange);
	
	m_dodgeSenseCollider->SetCapsuleRadius(DodgeSenseRadius);
	m_dodgeSenseCollider->SetCapsuleHalfHeight(DodgeSenseHeight / 2);
}
/**********************************************************************************************************
*	void BeginPlay()
*		Purpose:	Fires once at the beginning of the game.
**********************************************************************************************************/
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

/**********************************************************************************************************
*	void Tick(float DeltaTime)
*		Purpose:	Fires once each frame.
**********************************************************************************************************/
void ABaseEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
/**********************************************************************************************************
*    float TakeDamage(float DamageAmount, FDamageEvent & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
*        Purpose:    Informs the AIController that this pawn took damage.
*
*        Parameters:
*            float DamageAmount
*				 The original, unmodified amount of damage to take.
*			 FDamageEvent&
*				 The precision of the damage type, EG head strike vs strike.
*			 AController* EventInstigator
*				 The controller belonging to the actor who triggered the damage.
*			 AActor* DamagingCauser
*				 The actor that actually caused the damage.
*
*        Return:
*				Returns the amount of damage dealt.
**********************************************************************************************************/
float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Controller->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
/**********************************************************************************************************
*    void Faint()
*        Purpose:    Purpose
*
*        Parameters:
*            ExampleType Example
*                Example description.
*
*        Changes:
*            m_example - change to member variable description.
*
*        Return: Return description:
*
*        NOTE: optional note.
**********************************************************************************************************/
bool ABaseEnemy::Faint()
{
	return true;
}
/**********************************************************************************************************
*    USphereComponent* GetAggressionCollider()
*        Purpose:    Getter.
**********************************************************************************************************/
USphereComponent* ABaseEnemy::GetAggressionCollider()
{
	return m_aggressionCollider;
}
/**********************************************************************************************************
*    UCapsuleComponent* GetDodgeSenseCollider()
*        Purpose:    Getter.
**********************************************************************************************************/
UCapsuleComponent* ABaseEnemy::GetDodgeSenseCollider()
{
	return m_dodgeSenseCollider;
}