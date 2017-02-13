// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "GameFramework/Character.h"
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
*		float CurrentHealth
*			 The current amount of health this actor has.
*		float MaxHealth
*			 The maximum health this enemy can have.
*		float AgressionRange
*			 Indicates how far away the player needs to be to not be attacked.
*	
*	Data Members:
*		USphereComponent * AggressionCollider
*			 This sphere indicates the range at which an enemy will switch from passive to aggressive.
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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AggressionRange;

private:

	USphereComponent* m_aggressionCollider;

};