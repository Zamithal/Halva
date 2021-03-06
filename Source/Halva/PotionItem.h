// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "InventoryItem.h"
#include "PotionInterface.h"
#include "Particles/ParticleSystem.h"
#include "PotionItem.generated.h"

/**********************************************************************************************************
*	Class: APotionItem
*
*	Overview:
*		Potions are inventory items capable of performing an effect on the actor who triggers them.
*		An actor must implement the IPotionInterface to receive an effect from a potion.
*		The effect is triggered when the potion is tipped above the actor at more than an X degree angle.
*		The effect will continue until either the potion is empty, the actor is no longer under the
*		potion, or the potion is no longer tilted. If there is no actor under a potion it will continue
*		to drain but will not provide any benefit.
*
*	Methods:
*		bool Empty()
*			Causes this potion to become empty.
*		bool EmptyAmount(float Amount)
*			Causes this potion to empty by the give amount.
*		bool Fill()
*			Fills this potion to the maximum amount it can hold.
*		bool FillAmount(float Amount)
*			Fills the potion with he given amount of liquid.
*		bool IsPouring()
*			Returns if this potion is currently pouring.
*		void PotionEffect(AActor* Target, float Deltatime)
*			Overridable effect that triggers when the drinking conditions are met. This will only be
*			called if Target implements the IPotionInterface but Unreal seems to have a problem with
*			passing interfaces to blueprints.
*
*	Data Members:
*		float MaximumQuantity
*			The maximum amount of liquid this potion can contain.
*		float CurrentQuantity
*			The current amount of liquid this potion contains.
*		float TipAngle
*			The minimum angle this must tip off of the Z axis to be considered pouring.
*		EPotionType PotionType
*			The type of potion this is.
*		UParticleSystemComponent* PourEffect
*			ParticleSystem in charge of playing effects when the potion is pouring.
*		bool m_pouring;
*			Indicates if this potion is currently pouring.
**********************************************************************************************************/
UCLASS()
class HALVA_API APotionItem : public AInventoryItem
{
	GENERATED_BODY()
	
public:

	// Constructor.
	APotionItem();
	// Override BeginPlay()
	virtual void BeginPlay() override;
	// Override OnConstruction.
	void OnConstruction(const FTransform& Transform) override;
	// Override Tick()
	void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
		float MaximumQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
		float CurrentQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
		float TipAngle;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	//	EPotionType PotionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
		UParticleSystemComponent* PourEffect;

	UFUNCTION(BlueprintCallable, Category = "Potion")
		bool Empty();
	UFUNCTION(BlueprintCallable, Category = "Potion")
		bool EmptyAmount(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Potion")
		bool Fill();
	UFUNCTION(BlueprintCallable, Category = "Potion")
		bool FillAmount(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Potion")
		bool IsPouring();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Potion")
		void PotionEffect(AActor* Target, float Deltatime);

protected:
	bool m_pouring;

};
