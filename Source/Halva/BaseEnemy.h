// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"


/**********************************************************************************************************
*	Class: ABaseEnemy
*
*	Overview:
*		This is the abstract base class all enemies inherit from.
*		
*	Methods:
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
*		EEnemyAggressionType CurrentAggression
*			 Indicates if the enemy will currently attack the player.
*		EEnemyAggressionType NaturalAggression
*			 Indicates the default aggression type for this enemy.
**********************************************************************************************************/
UCLASS()
class HALVA_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Faint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		EEnemyAggressionType CurrentAggression;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		EEnemyAggressionType NaturalAggression;

private:


};
/**********************************************************************************************************
*    Enum: EEnemyAggressionType
*
*		 Purpose:
*			 Indicates if an enemy will attack a player or not.
**********************************************************************************************************/
UENUM(BlueprintType)
enum class EEnemyAggressionType : uint8
{
	Passive		UMETA(DisplayName="Passive"),
	Neutral		UMETA(DisplayName="Neutral"),
	Aggressive	UMETA(DisplayName="Aggressive")
};