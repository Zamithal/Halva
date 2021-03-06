// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Animation/AnimMontage.h"
#include "AIDodgeAction.generated.h"
/**********************************************************************************************************
*	enum: EDodgeType
*
*	Overview:
*		Specifies the type of dodge. The dodges type is considered when choosing which dodge action to
*		take.
*
*	Options:
*
*		Generic
*			The dodge type is not classified.
*		Duck
*			The dodge will cause the actor to duck down low.
*		Jump
*			The dodge will cause the actor to jump over the attack.
*		SideStepLeft
*			The dodge causes the actor to move to its left dodging an attack to the right.
*		SideStepRight
*			The dodge causes the actor to move to its right dodging an attack on the left.
*		BackStep
*			The dodge causes the actor to move backwards.
**********************************************************************************************************/
UENUM(BlueprintType)
enum class EDodgeType : uint8
{
	Duck			UMETA(DisplayName="Duck"),
	Jump			UMETA(DisplayName="Jump"),
	SideStepLeft	UMETA(DisplayName="SideStepLeft"),
	SideStepRight	UMETA(DisplayName="SideStepRight"),
	BackStep		UMETA(DisplayName="BackStep")
};
/**********************************************************************************************************
*	Class:	UAIAttackAction
*
*	Overview:
*		This class encompasses a dodge from an AI. It contains an animation and a type.
*
*	UProperties:
*		UAnimSequence* Animation
*			The dodge animation.
*		EDodgeType DodgeType
*			The type of dodge this is.
**********************************************************************************************************/
USTRUCT(BlueprintType)
struct HALVA_API FAIDodgeAction
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		EDodgeType DodgeType;

	FAIDodgeAction();

private:
};
