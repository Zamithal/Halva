// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "AIAttackAction.h"

/**********************************************************************************************************
*	FAIAttackAction()
*		Purpose:	Constructor.
**********************************************************************************************************/
FAIAttackAction::FAIAttackAction()
{
	MinimumRange = 0;
	MaximumRange = 0;
	Animation = nullptr;
	AttackMethod = EAttackType::MeleeAttack;
}


