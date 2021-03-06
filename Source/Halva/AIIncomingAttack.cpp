// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "AIIncomingAttack.h"

/**********************************************************************************************************
*	ABaseEnemyAIController()
*		Purpose:	Constructor.
**********************************************************************************************************/
FAIIncomingAttack::FAIIncomingAttack()
{
	Location = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;

	TriggeringActor = nullptr;
}
/**********************************************************************************************************
*	ABaseEnemyAIController(FHitResult Hit, AActor* Actor)
*		Purpose:	Constructor.
**********************************************************************************************************/
FAIIncomingAttack::FAIIncomingAttack(FVector Loc, FVector Vel, AActor* Actor)
{
	Location = Loc;
	Velocity = Vel;
	TriggeringActor = Actor;
}

