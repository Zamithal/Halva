// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemyAIController.h"


/**********************************************************************************************************
*	ABaseEnemyAIController()
*		Purpose:	Constructor.
**********************************************************************************************************/
ABaseEnemyAIController::ABaseEnemyAIController()
{

}
/**********************************************************************************************************
*	void OnConstruction(const FTransform& Transform)
*		Purpose:	UE4 ConstructionScript
**********************************************************************************************************/
void ABaseEnemyAIController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
/**********************************************************************************************************
*    void Possess(class APawn* Pawn)
*        Purpose:    Fires whenever this AI takes control of a pawn.
*
*        Parameters:
*            class APawn* Pawn
*				 The type of pawn being controlled.
**********************************************************************************************************/
void ABaseEnemyAIController::Possess(class APawn* Pawn)
{
	Super::Possess(Pawn);

}
/**********************************************************************************************************
*    void EnteredAggressionRange(...)
*        Purpose:    Triggers when an actor begins overlapping with the enemies aggression range. The
*					 enemies tags are checked to see if any matches this enemy's hated tags list. If it
*					 is, that enemy is added to the attack list.
*
*        Parameters:
*            Standard Overlap parameters.
*
*        Changes:
*            BB - Attacking
*				The overlapping actor may be added to the attack list.
**********************************************************************************************************/
void ABaseEnemyAIController::EnteredAggressionRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}