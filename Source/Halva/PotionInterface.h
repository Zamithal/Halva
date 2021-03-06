// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "PotionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UPotionInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**********************************************************************************************************
*    Interface: IPotionInterface
*
*    Overview:
*        This interface is to be implemented by actors who can use potions. It contains various different
*		 methods for the different types of potions in the game such as Heath, Stamina, and Mana potions.
*		 actors may choose to implement any of the below functions if they wish to be able to use a
*		 potion of the given type. If the function is not implemented, the default will be called causing
*		 nothing to happen.
*
*    Methods:
*        void ModifyHealth(float Amount)
*			Change the health of the actor. Could be used for healing or damage potions.
**********************************************************************************************************/
class HALVA_API IPotionInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Potion")
		void ModifyHealth(float Amount);
};
