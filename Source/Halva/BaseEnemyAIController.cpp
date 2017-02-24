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
/**********************************************************************************************************
*   UAnimMontage* PickAttackAnimation(FVector TargetPoint)
*       Purpose:	Searches through this controller's collection of attack actions searching for an
*					attack that matches the criteria to hit the target point. If none can be found this
*					function returns nullptr . If multiple meet the criteria, one is selected at random
*					from all actions that match.
*
*       Parameters:
*           FVector TargetPoint
*				The point to try to attack.
*
*       Return:
*			Returns the animation that best fits the criteria.
**********************************************************************************************************/
UAnimMontage* ABaseEnemyAIController::PickAttackAnimation(FVector TargetPoint)
{
	//TODO: Add direction and type checking.

	TArray<FAIAttackAction> viableActions;

	float distanceToPoint = (TargetPoint - m_pawn->GetActorLocation()).Size();

	for (TArray<FAIAttackAction>::TIterator iter = m_pawn->AttackActions.CreateIterator(); iter; ++iter)
	{
		if (distanceToPoint >= iter->MinimumRange && (distanceToPoint <= iter->MaximumRange || iter->MaximumRange <= 0))
		{
			viableActions.Add(*iter);
		}
	}

	if (viableActions.Num() == 0)
		return nullptr;

	int RandomIndex = FMath::RoundToInt(FMath::RandRange(0, viableActions.Num()));

	return viableActions[RandomIndex].Animation;
}
/**********************************************************************************************************
*   UAnimMontage* PickAttackAnimation(FVector TargetPoint)
*       Purpose:	Overload for PickAttackAnimation, See the FVector variant.
**********************************************************************************************************/
UAnimMontage* ABaseEnemyAIController::PickAttackAnimation(AActor* TargetActor)
{
	return PickAttackAnimation(TargetActor->GetActorLocation());
}
/**********************************************************************************************************
*   UAnimMontage* PickDodgeAnimation(const FHitResult& HitResult)
*       Purpose:	Processes the hit result and attempts to pick a dodge animation that is most likely to
*					result in a successful dodge. If multiple animations are equally likely to result in
*					a successful dodge, one is returned at random.
*
*       Parameters:
*           const FHitResult& HitResult
*				The incoming hit to attempt to dodge.
*
*       Return:
*			Returns the animation that best fits the criteria.
**********************************************************************************************************/
UAnimMontage* ABaseEnemyAIController::PickDodgeAnimation(const FHitResult& HitResult)
{
	TArray<FAIDodgeAction> viableActions;

	FVector actorCenterPoint = m_pawn->GetCapsuleComponent()->GetComponentLocation();

	// The angle between the impact normal and the plane in which to consider relatively parallel.
	static const float acceptableAngleVariance = 30.0f;

	FPlane actorUpPlane = FPlane(m_pawn->GetActorUpVector(), 0);
	FPlane actorRightPlane = FPlane(m_pawn->GetActorRightVector(), 0);
	FPlane actorForwardPlane = FPlane(m_pawn->GetActorForwardVector(), 0);

	// Returns the angle between the impact vector and the plane in degrees.
	float impactAngleOffUpPlane = FMath::RadiansToDegrees(FMath::Asin(actorUpPlane.PlaneDot(HitResult.ImpactNormal) / (HitResult.ImpactNormal.Size() * actorUpPlane.Size())));
	float impactAngleOffForwardPlane = FMath::RadiansToDegrees(FMath::Asin(actorForwardPlane.PlaneDot(HitResult.ImpactNormal) / (HitResult.ImpactNormal.Size() * actorForwardPlane.Size())));
	float impactAngleOffRightPlane = FMath::RadiansToDegrees(FMath::Asin(actorRightPlane.PlaneDot(HitResult.ImpactNormal) / (HitResult.ImpactNormal.Size() * actorRightPlane.Size())));

	// Vector from center of actor to impact point.
	FVector positionVector = HitResult.ImpactPoint - actorCenterPoint;

	// Ducks should occur when the attack is above the center point and relatively along the up plane
	// or the forward plane.
	// Jumps should occur when the attack is below the center point and relatively along the up plane
	// or the forward plane.

	// Ducks or Jumps are OK.
	if (FMath::Abs(impactAngleOffUpPlane) <= acceptableAngleVariance || FMath::Abs(impactAngleOffForwardPlane) <= acceptableAngleVariance)
	{
		// Determine if the strike was above or below the actor's center point.
		float angleFromUpPlane = FMath::RadiansToDegrees(FMath::Acos(actorUpPlane.PlaneDot(positionVector) / (actorUpPlane.Size() * positionVector.Size())));

		EDodgeType typeToAdd = EDodgeType::Duck;

		// If the angle is larger than 90 the hit is below the center point.
		if (FMath::Abs(angleFromUpPlane) > 90)
			typeToAdd = EDodgeType::Jump;

		for (TArray<FAIDodgeAction>::TIterator iter = m_pawn->DodgeActions.CreateIterator(); iter; ++iter)
		{
			if (iter->DodgeType == typeToAdd)
				viableActions.Add(*iter);
		}
	}

	// Side step left should occur when the attack is right of the center point and is relatively along
	// the right or forward plane.
	// Side step right should occur when the attack is left of the center point and is relatively along
	// the right or forward plane.

	// Sidesteps are OK.
	if (FMath::Abs(impactAngleOffRightPlane) <= acceptableAngleVariance || FMath::Abs(impactAngleOffForwardPlane) <= acceptableAngleVariance)
	{
		// Determine if the strike was to the left or the right of the actor's center point.		
		float angleFromRightPlane = FMath::RadiansToDegrees(FMath::Acos(actorRightPlane.PlaneDot(positionVector) / (actorRightPlane.Size() * positionVector.Size())));

		EDodgeType typeToAdd = EDodgeType::SideStepLeft;

		// If angle is < 90 hit is on the right side.
		if (FMath::Abs(angleFromRightPlane) > 90)
			typeToAdd = EDodgeType::SideStepRight;

		for (TArray<FAIDodgeAction>::TIterator iter = m_pawn->DodgeActions.CreateIterator(); iter; ++iter)
		{
			if (iter->DodgeType == typeToAdd)
				viableActions.Add(*iter);
		}
	}

	// TODO: Include back step?

	// No dodge is sufficient.
	if (viableActions.Num() <= 0)
		return nullptr;

	int randomIndex = FMath::RoundToInt(FMath::RandRange(0, m_pawn->HitAnimations.Num()));

	return m_pawn->DodgeActions[randomIndex].Animation;
}
/**********************************************************************************************************
*   UAnimMontage* PickDamagedAnimation()
*       Purpose:	Picks a random damaged animation from the pawn.
*
*       Return:
*			Returns the animation.
**********************************************************************************************************/
UAnimMontage* ABaseEnemyAIController::PickDamagedAnimation()
{
	if (m_pawn->HitAnimations.Num() <= 0)
		return nullptr;

	int randomIndex = FMath::RoundToInt(FMath::RandRange(0, m_pawn->HitAnimations.Num()));

	return m_pawn->HitAnimations[randomIndex];
}
/**********************************************************************************************************
*   UAnimMontage* PickFaintAnimation()
*       Purpose:	Picks a random faint animation from the controlled pawn.
*
*       Return:
*			Returns the animation.
**********************************************************************************************************/
UAnimMontage* ABaseEnemyAIController::PickFaintAnimation()
{
	if (m_pawn->FaintAnimations.Num() <= 0)
		return nullptr;

	int randomIndex = FMath::RoundToInt(FMath::RandRange(0, m_pawn->FaintAnimations.Num()));

	return m_pawn->FaintAnimations[randomIndex];
}