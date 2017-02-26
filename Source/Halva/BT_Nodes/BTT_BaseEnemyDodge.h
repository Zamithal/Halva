// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BaseEnemyDodge.generated.h"

/**********************************************************************************************************
*	Task: UBTT_BaseEnemyDodge
*
*	Overview:
*		This is a BT task node. If the AIController does not inherit from BaseEnemyAIController, this node
*		will fail. When this node is activated, the pawn will process the top of the queue for incoming
*		attacks and attempt to pick a dodge that most successfully results in the pawn not getting hit.
*		If there are no incoming attacks, the incoming attack BB value is invalid or not set, this node
*		will fail.
*
*	UProperties:
*		BlackBoardKey
*			The name of the black board entry that contains the list of incoming attacks. If this entry
*			does not exist, is not a UAIIncomingAttack, is not set, or is empty, this node will fail.
*
*	Methods:
*		AnimationCompleted(UBehaviorTreeComponent& OwnerComp)
*			Wrapper function that fires when the played animation is completed.
**********************************************************************************************************/
UCLASS()
class HALVA_API UBTT_BaseEnemyDodge : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	// On execute.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		FName BlackBoardKey;

	UFUNCTION()
		void AnimationCompleted(UBehaviorTreeComponent* OwnerComp);
	
private:
};
