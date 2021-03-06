// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "GrabableItem.h"

/**********************************************************************************************************
*    bool Use_Implementation(USceneComponent * AttachPoint, TArray<FString>& Args)
*        Purpose:    This function implements the ability to pick up  and drop grab-able items. When an
*					 item is used, it will attach to the using actor. If the item is again used,
*					 it will instead detach from that actor.
*        Parameters:
*            USceneComponent* Sender
*				 The component to attach to. Behavior may change depending on the arguments.
*			 TArray<FString>& Args
*				 The first argument is the attachment method. "Free" or "Strict". Snap will cause the
*				 object to snap to the correct orientation.
*				 The second argument, if specified, is the socket of the sending actor to attach to.
*
*        Changes:
*            This item will be attached to the sending actor.
*
*        Return: Returns if the object was successfully attached or not.
**********************************************************************************************************/
bool AGrabableItem::Use_Implementation(USceneComponent* AttachPoint, TArray<FString>& Args)
{
	bool grab = true;

	// If explicit instructions
	if (Args.Num() >= 2)
	{
		if (Args[1] == "Drop")
			grab = false;
	}
	// decide based on context
	else
	{
		TArray<USceneComponent *> children;
		AttachPoint->GetChildrenComponents(false, children);

		for (int i = 0; i < children.Num(); i++)
		{
			if (children[i] == this->GetRootComponent())
				grab = false;
		}
	}

	if (grab)
	{
		if (AttachPoint == nullptr || Args.Num() == 0)
			return false;

		// Detach from old if attached
		if (this->GetAttachParentActor())
			this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// Disable physics if necessary
		UPrimitiveComponent* physicsRoot = Cast<UPrimitiveComponent>(RootComponent);
		if (physicsRoot)
			physicsRoot->SetSimulatePhysics(false);

		if (Args[0] == "Free")
			this->AttachToComponent(AttachPoint, FAttachmentTransformRules::KeepWorldTransform);
		else
			this->AttachToComponent(AttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		this->Dehighlight_Implementation();
	}
	else
	{
		// Detach from previous actor.
		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);


		// Physics enabled object?
		UPrimitiveComponent* physicsRoot = Cast<UPrimitiveComponent>(RootComponent);

		if (physicsRoot)
		{
			TArray<UPrimitiveComponent *> overlappingComponents;

			// Enable overlap for colliding objects for this check.
			physicsRoot->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
			physicsRoot->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
			physicsRoot->GetOverlappingComponents(overlappingComponents);
			// Cause blocking again.
			physicsRoot->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
			physicsRoot->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
			bool blockingComponent = false;

			for (int i = 0; i < overlappingComponents.Num(); i++)
			{
				if (overlappingComponents[i]->GetCollisionResponseToChannel(physicsRoot->GetCollisionObjectType()) == ECollisionResponse::ECR_Block)
				{
					blockingComponent = true;
					break;
				}
			}

			physicsRoot->SetSimulatePhysics(!blockingComponent);
		}

		this->Highlight_Implementation();
	}

	return true;
}
/**********************************************************************************************************
*    bool Highlight_Implementation()
*        Purpose:    Indicates when the grab-able item is usable. In most circumstances, this just
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
bool AGrabableItem::Highlight_Implementation()
{
	UPrimitiveComponent* root = Cast<UPrimitiveComponent>(RootComponent);

	if (root == nullptr)
		return false;

	root->SetRenderCustomDepth(true);

	return true;
}
/**********************************************************************************************************
*    bool Dehighlight_Implementation()
*        Purpose:    Indicates when an grab-able item is no longer usable. This usually occurs when the
*					 player's hand moves away from the item or the player uses the item. This is indicated
*					 by removing the highlighting around the object. This assumes the item's post
*					 processing material is properly configured for outlining.
*
*        Changes:
*            The item will no longer be highlighted.
*
*        Return: Returns if the item was successfully dehighlighted.
**********************************************************************************************************/
bool AGrabableItem::Dehighlight_Implementation()
{
	UPrimitiveComponent* root = Cast<UPrimitiveComponent>(RootComponent);

	if (root == nullptr)
		return false;

	root->SetRenderCustomDepth(false);

	return true;
}


