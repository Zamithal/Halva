// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "InventoryItem.h"
#include "ItemHitBox.h"
#include "WeaponItem.generated.h"

/**********************************************************************************************************
*	Class: AWeaponItem
*
*	Overview:
*		WeaponItems are Inventory items designed for combat. The default CalculateDamage Function is
*		overridden with a new function that factors in the speed the weapon is traveling as well as the
*		angle of travel relative to a point/edge/strike-zone.
*
*	Mutators:
*		DamageVectors
*			-Get
*		HitBoxes
*			-Get
*		PreviousFrameVectors
*			-Get
*		InputSum
*			-Get
*
*	Methods:
*		float CalculateDamage(AActor* triggeringActor = nullptr)
*			Calculates the amount of damage this should do to the triggering actor.
*		void CreateDamageVectors();
*			Creates a set of damage vectors for the weapon.
*		void WeaponHit(...)
*			Triggers when this weapon begins overlapping with another actor.
*		void Tick();
*			Fires once per frame.
*	Data Members:
*		TArray<FVector> m_damageVectors
*			Contains a list of all directional damage vectors. A damage vector indicates a weapon's
*			damaging edge, point, or surface.
*		TQueue<FVector> m_previousFrameVectors
*			Contains the last 45 frames of input direction.
*		FVector m_inputSum
*			The sum of location changes of the last 45 frames.
*		FVector m_lastFrameLocation
*			The location of the weapon last frame.
**********************************************************************************************************/
UCLASS()
class HALVA_API AWeaponItem : public AInventoryItem
{
	GENERATED_BODY()
	
public:

	AWeaponItem();
	void OnConstruction(const FTransform& Transform) override;

	float CalculateDamage(AActor* TriggeringActor) override;

	TArray<FVector> GetDamageVectors();
	TArray<UItemHitBox *> GetHitBoxes();
	FVector GetInputSum();
	
	UFUNCTION()
		void WeaponHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float WeaponWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float WeaponBaseDamage;

	// Override BeginPlay()
	virtual void BeginPlay() override;
	// Override Tick()
	void Tick(float DeltaTime) override;
private:
	void CreateDamageVectors();

	TArray<FVector> m_damageVectors;
	TQueue<FVector> m_previousFrameVectors;
	TArray<UItemHitBox *> m_hitBoxes;
	FVector m_inputSum;
	FVector m_lastFrameLocation;
};
