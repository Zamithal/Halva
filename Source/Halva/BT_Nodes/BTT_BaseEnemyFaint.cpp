// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemy.h"
#include "BaseEnemyAIController.h"
#include "BTT_BaseEnemyFaint.h"

/**********************************************************************************************************
*    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
*
*		Purpose:	If the AIController does not inherit from BaseEnemyAIController, this will fail.
*					When called, a random faint animation from the pawn will be played. If there are no
*					faint animations, this will fail. When the animation completes, AnimaitonCompleted
*					will be called.
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
EBTNodeResult::Type UBTT_BaseEnemyFaint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseEnemyAIController* ai = Cast<ABaseEnemyAIController>(OwnerComp.GetAIOwner());

	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UAnimMontage* animation = ai->PickFaintAnimation();

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
*       Purpose:   Wrapper function that is called when the animation is completed. Destroys the pawn.
*
*		Parameters:
*			UBehaviorTreeComponent& OwnerComp
*				The behavior tree to complete for.
**********************************************************************************************************/
void UBTT_BaseEnemyFaint::AnimationCompleted(UBehaviorTreeComponent* OwnerComp)
{
	ABaseEnemy* pawn = Cast<ABaseEnemy>(OwnerComp->GetAIOwner()->GetPawn());

	if (pawn != nullptr)
		pawn->Destroy();

	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
