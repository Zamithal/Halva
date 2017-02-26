// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "AIController.h"
#include "BaseEnemy.h"
#include "AIIncomingAttackList.h"
#include "BaseEnemyAIController.generated.h"
/**********************************************************************************************************
*	Class: ABaseEnemyAIController
*
*	Overview:
*		This is the AI controller all enemy AI controllers inherit from. Contains several methods common
*		to all enemy AIs.
*
*	Mutators:
*		Pawn
*			-Get
*
*	Methods:
*		ABaseEnemyAIController();
*			Constructor
*		virtual void OnConstruction(const FTransform& Transform);
*			UE4 ConstructionScript.
*		virtual void Possess(class APawn* Pawn)
*			Fires when this AI begins possessing a pawn.
*		virtual void EnteredAggressionRange(...)
*			Triggers when an actor enters this enemy's aggression range.
*		virtual void EnteredDodgeSense(...)
*			Triggers when an actor enters this enemy's dodge sense collider.
*		virtual void ExitedDodgeSense(...)
*			Triggers when an actor leaves this enemy's dodge sense collider.
*		virtual float TakeDamage(...)
*			Reduces the health of this actor.
*		UAnimMontage* PickAttackAnimation(FVector TargetPoint)
*			Attempts to pick an attack animation that will hit the target location from the controlled
*			pawn's current location.
*		UAnimMontage* PickAttackAnimation(AActor* TargetActor)
*			Attempts to pick an attack animation that will hit the target actor from the controlled pawn's
*			current position.
*		UAnimMontage* PickDodgeAnimation(FVector Location, FVector Velocity)
*			Attempts to find the dodge animation that is most likely to result in a successful dodge and
*			returns it.
*		UAnimMontage* PickDamagedAnimation();
*			Picks one of the pawn's damage animations and returns it.
*		UAnimMontage* PickFaintAnimation();
*			Picks one of the pawn's faint animations and returns it.
*		void BeginDestroy()
*			Called right before destruction to clean up memory.
*
*	UProperties:
*
*	Data Members:
*		ABaseEnemy* m_pawn
*			The pawn this AIController controls.
*		UAIIncomingAttackList* m_incomingAttacks
*			A list off all attacks that could hit this actor.
**********************************************************************************************************/
UCLASS()
class HALVA_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseEnemyAIController();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Possess(class APawn* Pawn) override;

	UFUNCTION()
		void EnteredAggressionRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EnteredDodgeSense(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void ExitedDodgeSense(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UAnimMontage* PickAttackAnimation(FVector TargetPoint);
	UAnimMontage* PickAttackAnimation(AActor* TargetActor);

	UAnimMontage* PickDodgeAnimation(FVector Location, FVector Velocity);

	UAnimMontage* PickDamagedAnimation();
	UAnimMontage* PickFaintAnimation();

	virtual void BeginDestroy() override;

protected:

	ABaseEnemy* m_pawn;

	UAIIncomingAttackList* m_incomingAttacks;
};
