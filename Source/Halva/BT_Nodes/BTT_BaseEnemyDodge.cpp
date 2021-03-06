// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemy.h"
#include "BaseEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIIncomingAttackList.h"
#include "BTT_BaseEnemyDodge.h"

/**********************************************************************************************************
*    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
*
*		Purpose:	If the AIController does not inherit from BaseEnemyAIController, this will fail.
*					When called, the pawn will process the top of the queue for incoming
*					attacks and attempt to pick a dodge that most successfully results in the pawn not 
*					getting hit. If there are no incoming attacks, the incoming attack BB value is invalid
*					or not set, this will fail.
*
*		Parameters:
*			UBehaviorTreeComponent& OwnerComp
*				The behavior tree that fired this node.
*			uint8* NodeMemory
*				This is not well documented and will not be modified in any way.
*
*		Return:
*			Returns if a dodge animation was played.
**********************************************************************************************************/
EBTNodeResult::Type UBTT_BaseEnemyDodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseEnemyAIController* ai = Cast<ABaseEnemyAIController>(OwnerComp.GetAIOwner());

	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UAIIncomingAttackList* attackList = Cast<UAIIncomingAttackList>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (attackList == nullptr)
		return EBTNodeResult::Failed;

	if (attackList->IncomingAttacks.Num() <= 0)
		return EBTNodeResult::Failed;

	FAIIncomingAttack attack = attackList->IncomingAttacks.Pop();

	UAnimMontage* animation = ai->PickDodgeAnimation(attack.Location, attack.Velocity);

	if (animation == nullptr)
		return EBTNodeResult::Failed;

	ABaseEnemy* pawn = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (pawn == nullptr)
		return EBTNodeResult::Failed;

	float animLength = pawn->GetMesh()->GetAnimInstance()->Montage_Play(animation);

	FTimerHandle animationFinished = FTimerHandle::FTimerHandle();

	FTimerDelegate timerDel;

	timerDel.BindUFunction(this, FName("AnimationCompleted"), &OwnerComp);

	GetWorld()->GetTimerManager().SetTimer(animationFinished, timerDel, animLength, false);


	if (attackList->IncomingAttacks.Num() == 0)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("AttackIncoming", false);

	return EBTNodeResult::InProgress;
}

/**********************************************************************************************************
*    void AnimationCompleted(UBehaviorTreeComponent& OwnerComp)
*
*       Purpose:   Wrapper function that is called when the animation is completed.
*
*		Parameters:
*			UBehaviorTreeComponent& OwnerComp
*				The behavior tree to complete for.
**********************************************************************************************************/
void UBTT_BaseEnemyDodge::AnimationCompleted(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}


