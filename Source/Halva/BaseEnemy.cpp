// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemy.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

/**********************************************************************************************************
*    float TakeDamage(float DamageAmount, FDamageEvent & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
*        Purpose:    Causes this enemy to take damage. The amount of damage may differ from the amount
*					 passed in depending on if the enemy is resistant to the damage.
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
*        Changes:
*            CurrentHealth
*				 The current health of this enemy will be decreased.
*			 CurrentAggression
*				 The current aggression of this enemy may change.
*
*        Return: Returns the actual amount of damage dealt.
**********************************************************************************************************/
float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;

	CurrentAggression = EEnemyAggressionType::Aggressive;

	if (CurrentHealth <= 0)
		Faint();

	return DamageAmount;
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
void ABaseEnemy::Faint()
{

	return void;
}
