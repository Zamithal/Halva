// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "Components/ArrowComponent.h"
#include "ItemDamageArrow.generated.h"

/**********************************************************************************************************
*    Class: UItemDamageArrow
*
*    Overview:
*        This component serves to indicate damaging edges or surfaces on items. Included in this is a
*		 magnitude component that indicates how damaging the edge or surface is. In example, a battle-axe
*		 would have a damage arrow perpendicular to the blade edge.

*    UProperties
*        float Magnitude
*			 The strength of damaging edge/surface/point.
**********************************************************************************************************/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HALVA_API UItemDamageArrow : public UArrowComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnitude")
		float Magnitude;

	UItemDamageArrow();
};
