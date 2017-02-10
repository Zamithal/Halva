// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "InventoryItem.h"
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
*		DamageArrows
*			-Get
*
*	Methods:
*		CreateImpliedVectors()
*			Sums each set of vectors defined by the user and places their sum as a new damage vector. If
*			The vectors sum to 0 or their direction is not unique, they are not added.
*
*	Data Members:
*		TArray<UArrowComponent*> DamageVectors
*			
**********************************************************************************************************/
UCLASS()
class HALVA_API AWeaponItem : public AInventoryItem
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<FVector> DamageVectors;
	
};
