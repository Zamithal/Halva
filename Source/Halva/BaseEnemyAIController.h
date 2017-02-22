// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "AIController.h"
#include "BaseEnemy.h"
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
*			Triggers when an actor enters this enemies aggression range.
*		virtual float TakeDamage(...)
*			Reduces the health of this actor.
*		bool PerformAttack(FVector TargetPoint)
*			Attempts to attack the given point from the current position. If no attack can reach, false
*			is returned.
*		bool PerformAttack(AActor* TargetActor)
*			Attempts to attack the give actor from the current position. If no attack can reach, false is
*			returned.
*
*	UProperties:
*		TArray<FAIAttackAction> AttackMethods
*			A list of all the attacks this actor can perform.
*
*	Data Members:
*		ABaseEnemy* m_pawn
*			The pawn this AIController controls.
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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<FAIAttackAction> AttackMethods;

	bool PerformAttack(FVector TargetPoint);
	bool PerformAttack(AActor* TargetActor);

protected:

	ABaseEnemy* m_pawn;

	UBehaviorTreeComponent* m_behaviorTreeComp;
	UBlackboardComponent* m_blackBoardComp;
};
