// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "AIIncomingAttack.generated.h"

/**********************************************************************************************************
*    Struct: FAIIncomingAttack
*
*		Purpose:
*			This class encompasses an incoming source of damage from a weapon. This differs from an
*			attack action as an attack action is the ability to attack while this is an attack itself.
*			The attacks contain a hit result and a triggering actor.
*
*		UProperties:
*			FVector Location
*				The Location of the attack.
*			FVector Velocity
*				The Velocity of the attack.
*			AActor* TriggeringActor
*				The actor that triggered the attack.
**********************************************************************************************************/
USTRUCT(BlueprintType)
struct HALVA_API FAIIncomingAttack
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		AActor* TriggeringActor;

	FAIIncomingAttack();
	FAIIncomingAttack(FVector Loc, FVector Vel, AActor* Actor);

private:
};
