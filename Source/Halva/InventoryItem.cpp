// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "InventoryItem.h"

/**********************************************************************************************************
*    bool Use_Implementation(AActor * Sender, TArray<FString>& Args)
*        Purpose:    This function implements the ability to pick up  and drop Inventory items. When an
*					 item is used, it will attach to the using actor. If the item is again used by the same
*					 it will instead detach from that actor. If the item is already attached to an actor
*					 and is used by another actor, it will detach from the original actor and attach to
*					 the new actor. Parameters can be passed in to change how the item is attached.
*
*        Parameters:
*            AActor* Sender
*				 The actor to attach to. Behavior may change depending on the arguments.
*			 TArray<FString>& Args
*				 The first argument, if specified, is the component name of the sending actor to attach to.
*				 The second argument, if specified, is the socket of the sending actor to attach to.
*
*        Changes:
*            This item will be attached to the sending actor.
*
*        Return: Returns if the object was successfully attached or not.
**********************************************************************************************************/
bool AInventoryItem::Use_Implementation(AActor* Sender, TArray<FString>& Args)
{

	return true;
}
/**********************************************************************************************************
*    bool Highlight_Implementation()
*        Purpose:    Indicates when the inventory item is usable. In most circumstances, this just
*					 indicates when the player can grab the object and triggers when the player's hand is
*					 close enough to the object. This object outlines the item to inform the player that
*					 it can now be used. This assumes the item's post processing material is properly
*					 configured for outlining.
*
*        Changes:
*            The item will be highlighted.
*
*        Return: Returns if the item was successfully highlighted.
**********************************************************************************************************/
bool AInventoryItem::Highlight_Implementation()
{

	return true;
}
/**********************************************************************************************************
*    bool Dehighlight_Implementation()
*        Purpose:    Indicates when an inventory item is no longer usable. This usually occurs when the
*					 player's hand moves away from the item or the player uses the item. This is indicated
*					 by removing the highlighting around the object. This assumes the item's post
*					 processing material is properly configured for outlining.
*
*        Changes:
*            The item will no longer be highlighted.
*
*        Return: Returns if the item was successfully dehighlighted.
**********************************************************************************************************/
bool AInventoryItem::Dehighlight_Implementation()
{

	return true;
}