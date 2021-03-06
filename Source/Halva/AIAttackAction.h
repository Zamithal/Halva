// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Animation/AnimMontage.h"
#include "AIAttackAction.generated.h"
/**********************************************************************************************************
*	enum: EAttackType
*
*	Overview:
*		Specifies the type of attack.
*
*	Options:
*		MeleeAttack
*			The attack made is with a melee weapon.
**********************************************************************************************************/
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	MeleeAttack		UMETA(DisplayName="MeleeAttack")
	// Ranged attacks to be supported later.
};
/**********************************************************************************************************
*	Struct:	FAIAttackAction
*
*	Overview:
*		This class encompasses an attack from an AI. It contains an animation, minimum and maximum ranges
*		for that attack, and the type of attack it is.
*
*	UProperties:
*		float MinimumRange
*			The closest the target can be for this attack to be used.
*		float MaximumRange
*			The farthest a target can be for this attack to be used.
*		UAnimSequence Animation
*			The animation to play for this attack.
*		EAttackType AttackMethod
*			The attack type of this attack.
**********************************************************************************************************/
USTRUCT(BlueprintType)
struct HALVA_API FAIAttackAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float MinimumRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float MaximumRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* Animation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		EAttackType AttackMethod;

	FAIAttackAction();

private:
};
