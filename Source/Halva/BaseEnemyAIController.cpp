// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemyAIController.h"
#include "AIIncomingAttack.h"

#include "WeaponItem.h"
#include "BehaviorTree/BlackBoardComponent.h"

/**********************************************************************************************************
*	ABaseEnemyAIController()
*		Purpose:	Constructor.
**********************************************************************************************************/
ABaseEnemyAIController::ABaseEnemyAIController()
{
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));

	m_incomingAttacks = CreateDefaultSubobject<UAIIncomingAttackList>(TEXT("IncomingAttackList"));
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
*	void BeginDestroy()
*		Purpose:	UE4 Destruction script
**********************************************************************************************************/
void ABaseEnemyAIController::BeginDestroy()
{
	if (Blackboard != nullptr)
	{
		UAIIncomingAttackList* incomingHits = Cast<UAIIncomingAttackList>(Blackboard->GetValueAsObject("IncomingHits"));

		if (incomingHits != nullptr)
			delete incomingHits;
	}

	Super::BeginDestroy();
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

		Blackboard->SetValueAsFloat("CurrentHealth", m_pawn->MaximumHealth);
		Blackboard->SetValueAsObject("IncomingHits", m_incomingAttacks);

		m_pawn->GetDodgeSenseCollider()->bGenerateOverlapEvents = true;

		if (m_pawn->HealthBarWidgetComponent != nullptr)
			m_healthBar = Cast<UProgressBarWidget>(m_pawn->HealthBarWidgetComponent->GetUserWidgetObject());

		if (m_healthBar != nullptr)
			m_healthBar->SetFilledPercentage(1);

		// Bind events.
		m_pawn->GetAggressionCollider()->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemyAIController::EnteredAggressionRange);
		m_pawn->GetDodgeSenseCollider()->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemyAIController::EnteredDodgeSense);
		m_pawn->GetDodgeSenseCollider()->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemyAIController::ExitedDodgeSense);

		OnTakeAnyDamage.AddDynamic(this, &ABaseEnemyAIController::OnDamagingHit);
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
*            BB - CurrentEnemy
*				The overlapping actor may be added to set to the current enemy.
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
*	void EnteredDodgeSense(...)
*		Purpose:	Triggers when an actor enters this enemy's dodge sense collider. Checks to see if the
*					triggering component is a foreign weapon. If it is, it is added to blackboard.
*
*		Parameters:
*           Standard Overlap parameters.
*
*		Changes:
*			BB - The incoming hit is added to the list of incoming hits.
**********************************************************************************************************/
void ABaseEnemyAIController::EnteredDodgeSense(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If it was a weapon.
	if (Cast<AWeaponItem>(OtherActor) != nullptr)
	{
		// Get velocity
		FVector velocity = OtherComp->GetPhysicsLinearVelocity();

		// OnComponentOverlap fails to return a sweep result, do it ourselves.
		TArray<FHitResult> hitResults;
		// Move a sphere along the capsule from underneath looking for any hits.

		float sweepRadius = m_pawn->GetDodgeSenseCollider()->GetScaledCapsuleRadius();

		// Start below the actor, capsule radius underneath it.
		FVector sweepStart = OverlappedComp->GetOwner()->GetActorLocation();

		float offsetMagnitude = m_pawn->GetDodgeSenseCollider()->GetScaledCapsuleHalfHeight() + sweepRadius;
		sweepStart -= m_pawn->GetActorUpVector() * offsetMagnitude;

		FVector sweepEnd = OverlappedComp->GetOwner()->GetActorLocation();
		offsetMagnitude = m_pawn->GetDodgeSenseCollider()->GetScaledCapsuleHalfHeight_WithoutHemisphere();
		sweepEnd += m_pawn->GetActorUpVector() * offsetMagnitude;

		FCollisionObjectQueryParams collisionObject;
		collisionObject.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

		FCollisionQueryParams ignoreList;
		ignoreList.AddIgnoredActor(OverlappedComp->GetOwner());

		// Do a new sweep.
		GetWorld()->SweepMultiByObjectType(hitResults, sweepStart, sweepEnd, FQuat::Identity, collisionObject, FCollisionShape::MakeSphere(sweepRadius), ignoreList);

		for (TArray<FHitResult>::TIterator iter = hitResults.CreateIterator(); iter; ++iter)
		{

			if (OtherComp == iter->GetComponent())
			{
				
				if (iter->TraceStart != iter->ImpactPoint)
				{
					Blackboard->SetValueAsBool("AttackIncoming", true);
					m_incomingAttacks->IncomingAttacks.Add(FAIIncomingAttack(iter->ImpactPoint, velocity, OtherActor));
					break;
				}
				// Attack came from below, need to redo sweep from the top.
				else
				{
					sweepStart = OverlappedComp->GetOwner()->GetActorLocation();
					offsetMagnitude = m_pawn->GetDodgeSenseCollider()->GetScaledCapsuleHalfHeight() + sweepRadius;
					sweepStart += m_pawn->GetActorUpVector() * offsetMagnitude;

					sweepEnd = OverlappedComp->GetOwner()->GetActorLocation();
					offsetMagnitude = m_pawn->GetDodgeSenseCollider()->GetScaledCapsuleHalfHeight_WithoutHemisphere();
					sweepEnd -= m_pawn->GetActorUpVector() * offsetMagnitude;

					GetWorld()->SweepMultiByObjectType(hitResults, sweepStart, sweepEnd, FQuat::Identity, collisionObject, FCollisionShape::MakeSphere(sweepRadius), ignoreList);
					for (TArray<FHitResult>::TIterator reiter = hitResults.CreateIterator(); reiter; ++reiter)
					{

						if (OtherComp == reiter->GetComponent())
						{

							if (reiter->TraceStart != reiter->ImpactPoint)
							{
								Blackboard->SetValueAsBool("AttackIncoming", true);
								m_incomingAttacks->IncomingAttacks.Add(FAIIncomingAttack(reiter->ImpactPoint, velocity, OtherActor));
								break;
							}
							else
							{
								//Unsolvable.
								return;
							}
						}
					}
					break;
				}
			}
		}

		
	}
}
/**********************************************************************************************************
*	void ExitedDodgeSense(...)
*		Purpose:	Triggers when an actor leaves the enemy's dodge sense collider. Checks to see if the
*					actor was recently added as an incoming attack. If it was, it will be removed from the
*					list of incoming attacks as it is no longer valid.
*
*		Parameters:
*           Standard EndOverlap parameters.
*
*		Changes:
*			BB - The incoming hit is removed to the list of incoming hits.
**********************************************************************************************************/
void ABaseEnemyAIController::ExitedDodgeSense(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (int i = 0; i < m_incomingAttacks->IncomingAttacks.Num(); i++)
	{
		if (m_incomingAttacks->IncomingAttacks[i].TriggeringActor == Object)
		{
			m_incomingAttacks->IncomingAttacks.RemoveAt(i);
			if (m_incomingAttacks->IncomingAttacks.Num() == 0)
				Blackboard->SetValueAsBool("AttackIncoming", false);
			break;
		}
	}
}

/**********************************************************************************************************
*    void OnDamagingHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
*        Purpose:    Causes this enemy to take damage. The amount of damage may differ from the amount
*					 passed in depending on if the enemy is resistant to the damage.
*
*        Parameters:
*            Standard OnTakeAnyDamage parameters.
*
*        Changes:
*            CurrentHealth
*				 The current health of this enemy will be decreased.
**********************************************************************************************************/
void ABaseEnemyAIController::OnDamagingHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Blackboard == nullptr)
		return;

	float currentHealth = Blackboard->GetValueAsFloat("CurrentHealth");
	currentHealth -= Damage;
	Blackboard->SetValueAsFloat("CurrentHealth", currentHealth);

	if (m_healthBar != nullptr && m_pawn->MaximumHealth != 0)
		m_healthBar->SetFilledPercentage(currentHealth / m_pawn->MaximumHealth);
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

	int RandomIndex = FMath::RoundToInt(FMath::RandRange(0, viableActions.Num() -1));

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
*       Purpose:	Processes the hit and attempts to pick a dodge animation that is most likely to
*					result in a successful dodge. If multiple animations are equally likely to result in
*					a successful dodge, one is returned at random.
*
*       Parameters:
*           FVector Location
*				The location of the attack.
*			FVector Velocity
*				The Velocity of the attack.
*
*       Return:
*			Returns the animation that best fits the criteria.
**********************************************************************************************************/
UAnimMontage* ABaseEnemyAIController::PickDodgeAnimation(FVector Location, FVector Velocity)
{
	TArray<FAIDodgeAction> viableActions;

	FVector actorCenterPoint = m_pawn->GetCapsuleComponent()->GetComponentLocation();

	// The angle between the impact normal and the plane in which to consider relatively parallel.
	static const float acceptableAngleVariance = 30.0f;

	FPlane actorUpPlane = FPlane(m_pawn->GetActorUpVector(), 0);
	FPlane actorRightPlane = FPlane(m_pawn->GetActorRightVector(), 0);
	FPlane actorForwardPlane = FPlane(m_pawn->GetActorForwardVector(), 0);

	// Returns the angle between the impact vector and the axis perpendicular to the plane in degrees.
	float impactAngleOffUpAxis = FMath::RadiansToDegrees(FMath::Acos(FMath::Abs(actorUpPlane.PlaneDot(Velocity) / (Velocity.Size() * actorUpPlane.Size()))));

	float dot = FMath::Abs(actorUpPlane.PlaneDot(Velocity)) / (Velocity.Size() * actorUpPlane.Size());
	float angle = FMath::Acos(dot);
	float angleDegrees = FMath::RadiansToDegrees(angle);

	float impactAngleOffForwardAxis = FMath::RadiansToDegrees(FMath::Acos(FMath::Abs(actorForwardPlane.PlaneDot(Velocity) / (Velocity.Size() * actorForwardPlane.Size()))));
	float impactAngleOffRightAxis = FMath::RadiansToDegrees(FMath::Acos(FMath::Abs(actorRightPlane.PlaneDot(Velocity) / (Velocity.Size() * actorRightPlane.Size()))));

	// Vector from center of actor to impact point.
	FVector positionVector = Location - actorCenterPoint;

	//Determine the incoming attack axis

	// Ducks should occur when the attack is above the center point and relatively along the up plane
	// or the forward plane.
	// Jumps should occur when the attack is below the center point and relatively along the up plane
	// or the forward plane.


	// Ducks or Jumps are OK.
	if (impactAngleOffUpAxis >= 90 - acceptableAngleVariance)
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
	if (impactAngleOffRightAxis >= 90 - acceptableAngleVariance)
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

	int randomIndex = FMath::RoundToInt(FMath::RandRange(0, viableActions.Num() - 1));

	return viableActions[randomIndex].Animation;
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

	int randomIndex = FMath::RoundToInt(FMath::RandRange(0, m_pawn->HitAnimations.Num() - 1));

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

	int randomIndex = FMath::RoundToInt(FMath::RandRange(0, m_pawn->FaintAnimations.Num() - 1));

	return m_pawn->FaintAnimations[randomIndex];
}