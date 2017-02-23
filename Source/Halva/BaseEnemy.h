// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "AIAttackAction.h"
#include "AIDodgeAction.h"
#include "BaseEnemy.generated.h"

/**********************************************************************************************************
*    Enum: EEnemyAggressionType
*
*		 Purpose:
*			 Indicates if an enemy will attack a player or not.
**********************************************************************************************************/
UENUM(BlueprintType)
enum class EEnemyAggressionType : uint8
{
	Passive		UMETA(DisplayName = "Passive"),
	Aggressive	UMETA(DisplayName = "Aggressive")
};
/**********************************************************************************************************
*	Class: ABaseEnemy
*
*	Overview:
*		This is the abstract base class all enemies inherit from.
*	
*	Mutators:
*		AgressionCollider
*			-Get
*
*	Methods:
*		ABaseEnemy();
*			Constructor.
*		virtual void OnConstruction(const FTransform& Transform);
*			UE4 ConstructionScript
*		virtual float TakeDamage(...) override;
*			Causes this enemy to take damage.
*		virtual void Faint()
*			Causes the enemy to be incapacitated.
*
*	UProperties:
*		float MaxHealth
*			The maximum health this enemy can have.
*		float AgressionRange
*			Indicates how far away the player needs to be to not be attacked.
*		TArray<FAIAttackAction> AttackActions
*			A list of all the attacks this actor can perform.
*		TArray<FAIDodgeAction> DodgeActions
*			A list of all the dodges this actor can perform.
*		TArray<UAnimMontage*> HitAnimations
*			A list of animations that could play when this actor takes damage.
*		TArray<UAnimMontage*> FaintAnimations
*			A list of animations that could play when this actor faints.
*		UBehaviorTreeComponent* BehaviorTree
*			The behavior tree responsible for decision making.
*		TArray<FName> HatedTags
*			If an actor has any of these tags it should be considered an enemy to this.
*	
*	Data Members:
*		USphereComponent * AggressionCollider
*			This sphere indicates the range at which an enemy will switch from passive to aggressive.
**********************************************************************************************************/
UCLASS()
class HALVA_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	virtual void OnConstruction(const FTransform& Transform);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool Faint();

	USphereComponent* GetAggressionCollider();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AggressionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<FAIAttackAction> AttackActions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<FAIDodgeAction> DodgeActions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<UAnimMontage*> HitAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<UAnimMontage*> FaintAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		UBehaviorTree* BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TArray<FName> HatedTags;

protected:
	USphereComponent* m_aggressionCollider;
};