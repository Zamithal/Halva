// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "AIIncomingAttackList.generated.h"

/**********************************************************************************************************
*    class UAIIncomingAttackList
*
*		 Purpose:
*			 This encapsulates an array of hit results into a blackboard compatible UObject.
**********************************************************************************************************/
UCLASS()
class HALVA_API UAIIncomingAttackList : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		TArray<FHitResult> IncomingAttacks;

private:
};
