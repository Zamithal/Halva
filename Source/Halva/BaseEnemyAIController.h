// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "AIController.h"
#include "BaseEnemyAIController.generated.h"

/**********************************************************************************************************
*	Class: ABaseEnemyAIController
*
*	Overview:
*		This is the AI controller all enemy AI controllers inherit from. Contains several methods common
*		to all enemy AIs.
*
*	Methods:
*		ABaseEnemyAIController();
*			Constructor
*		virtual void OnConstruction(const FTransform& Transform);
*			UE4 ConstructionScript.
*		virtual void Possess(class APawn* Pawn)
*			Fires when this AI begins possessing a pawn.
*		virtual void EnteredAggressionRange(...)
*			Triggers when an actor enters this enemies aggression range.
*
*	UProperties:
**********************************************************************************************************/
UCLASS()
class HALVA_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseEnemyAIController();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Possess(class APawn* Pawn) override;

	virtual void EnteredAggressionRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
};
