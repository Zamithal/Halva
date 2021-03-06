// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "AIIncomingAttack.h"
#include "AIIncomingAttackList.generated.h"

/**********************************************************************************************************
*    class UAIIncomingAttackList
*
*		Purpose:
*			This encapsulates an a list of hit results into a blackboard compatible UObject.
*
*		UProperties:
*			TArray<FHitResult> IncomingAttacks;
*				A list of incoming attacks.
**********************************************************************************************************/
UCLASS(BlueprintType)
class HALVA_API UAIIncomingAttackList : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		TArray<FAIIncomingAttack> IncomingAttacks;
private:
};
