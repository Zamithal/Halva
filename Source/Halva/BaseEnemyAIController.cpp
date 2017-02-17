// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemyAIController.h"
#include "BehaviorTree/BlackBoardComponent.h"

/**********************************************************************************************************
*	ABaseEnemyAIController()
*		Purpose:	Constructor.
**********************************************************************************************************/
ABaseEnemyAIController::ABaseEnemyAIController()
{
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
}
/**********************************************************************************************************
*	void OnConstruction(const FTransform& Transform)
*		Purpose:	UE4 ConstructionScript
**********************************************************************************************************/
void ABaseEnemyAIController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
/**********************************************************************************************************
*    void Possess(class APawn* Pawn)
*        Purpose:    Fires whenever this AI takes control of a pawn.
*
*        Parameters:
*            class APawn* Pawn
*				 The type of pawn being controlled.
**********************************************************************************************************/
void ABaseEnemyAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	m_pawn = Cast<ABaseEnemy>(InPawn);


	if (m_pawn != nullptr && m_pawn->BehaviorTree != nullptr)
	{
		Blackboard->InitializeBlackboard(*m_pawn->BehaviorTree->BlackboardAsset);
		Cast<UBehaviorTreeComponent>(BrainComponent)->StartTree(*m_pawn->BehaviorTree);

		Blackboard->SetValueAsFloat("CurrentHealth", Blackboard->GetValueAsFloat("MaximumHealth"));

		// Bind events.
		m_pawn->GetAggressionCollider()->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemyAIController::EnteredAggressionRange);
	}

}
/**********************************************************************************************************
*    void EnteredAggressionRange(...)
*        Purpose:    Triggers when an actor begins overlapping with the enemies aggression range. The
*					 enemies tags are checked to see if any matches this enemy's hated tags list. If it
*					 is, that enemy is added to the attack list.
*
*        Parameters:
*            Standard Overlap parameters.
*
*        Changes:
*            BB - Attacking
*				The overlapping actor may be added to the attack list.
**********************************************************************************************************/
void ABaseEnemyAIController::EnteredAggressionRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// No Blackboard
	if (Blackboard == nullptr)
		return;

	if (Blackboard->GetValueAsObject("CurrentEnemy") == nullptr)
	{
		for(TArray<FName>::TIterator iter = m_pawn->HatedTags.CreateIterator(); iter; ++iter)
			if (OtherActor->ActorHasTag(*iter))
			{
				Blackboard->SetValueAsObject("CurrentEnemy", OtherActor);
				break;
			}
	}


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
float ABaseEnemyAIController::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Blackboard == nullptr)
		return 0.0f;

	float currentHealth = Blackboard->GetValueAsFloat("currentHealth");
	currentHealth -= DamageAmount;
	Blackboard->SetValueAsFloat("CurrentHealth", currentHealth);

	return DamageAmount;
}
