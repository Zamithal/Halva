// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "Components/BoxComponent.h"
#include "ItemHitBox.generated.h"

/**********************************************************************************************************
*    Class: UItemHitBox
*
*    Overview:
*        A UItemHitBox is a BoxCollider designed to be used on parts of an item. Each ItemHitBox includes
*		 a strength variable that indicates how effective a hit from this box would be on a damageable
*		 source. For Example, A battle-axe may have an ItemHitBox for the blade and one for the handle.
*		 The strength of the blade hit-box would be much higher than the strength of the handle hit-box.

*    UProperties
*        float Strength
*			 The strength of the hit-box.
**********************************************************************************************************/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HALVA_API UItemHitBox : public UBoxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strength")
		float Strength;

	UItemHitBox();
};
