// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemy.h"
#include "BaseEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTT_BaseEnemyAttack.h"

/**********************************************************************************************************
*    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
*
*        Purpose:   If the owning AI does not inherit from BaseEnemyAIController, this will do nothing
*					and fail. When this node is activated, the pawn will attempt to play an attack
*					animation appropriate to hit the current enemy. If the pawn has no animation that can
*					hit the enemy from the current location. This node will do nothing instead and will
*					fail.
*
*        Parameters:
*           UBehaviorTreeComponent& OwnerComp
*				The behavior tree that fired this node.
*			uint8* NodeMemory
*				This is not well documented and will not be modified in any way.
*
*        Return:
*			Returns if an attack animation was played.
**********************************************************************************************************/
EBTNodeResult::Type UBTT_BaseEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseEnemyAIController* ai = Cast<ABaseEnemyAIController>(OwnerComp.GetAIOwner());

	if (ai == nullptr)
		return EBTNodeResult::Failed;

	AActor* currentEnemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (currentEnemy == nullptr)
		return EBTNodeResult::Failed;

	UAnimMontage* animation = ai->PickAttackAnimation(currentEnemy);

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
void UBTT_BaseEnemyAttack::AnimationCompleted(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

