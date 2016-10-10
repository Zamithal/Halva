// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "UseInterface.h"


// This function does not need to be modified.
UUseInterface::UUseInterface(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

/**********************************************************************************************************
*    void Use()
*        Purpose:    This function can be overridden in order to interact with an object. This function is
*					 intended to perform an action when the player interacts with the object. An example
*					 of proper use of this function would be pressing a button or grabbing an object. This
*					 function does nothing in its current form.
**********************************************************************************************************/
void IUseInterface::Use()
{

}

/**********************************************************************************************************
*    void Highlight()
*        Purpose:    This function is intended to alert the player that the object they are interacting is
*					 use-able. An example of proper use would be if the player's hand is near a button.
*					 This function does nothing in its current form.
**********************************************************************************************************/
void IUseInterface::Highlight()
{
}

/**********************************************************************************************************
*    void Dehighlight()
*        Purpose:    This function is intended to alert the player when they can no longer use an object
*					 An example of proper use would be if the player's hand moves away from a button.
*					 This function does nothing in its current form.
**********************************************************************************************************/
void IUseInterface::Dehighlight()
{
}