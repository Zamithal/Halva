// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "Item.h"
#include "UseInterface.h"
#include "InventoryItem.generated.h"

/**********************************************************************************************************
*    Class: AInventoryItem
*
*    Overview:
*        An inventory item is an item that can be stored in the player's inventory. Inventory items have
*		 additional properties over normal items including stats, value, and the ability to hold the item.
*		 Inventory items use the UseInterface in order to be held. Upon being used, the item will attach
*		 to the user until used again. At that point the item will be detached.
*
*    Accessors/Mutators:
*        Example
*            -Get
*            -Set
*
*    Methods:
*        bool Use(USceneComponent* AttachPoint, TArray<FString>& Args);
*			 Allows the overwriting of the Use function in blueprints inheriting from this class. See
*			 Use_Implementation for functionality.
*		 Use_Implementation(AActor* Sender, TArray<FString>& Args) override;
*        bool Highlight();
*			 Allows the overwriting of the Highlight function in blueprints inheriting from this class.
*			 See Highlight_Implementation for functionality.
*        bool Dehighlight();
*			 Allows the overwriting of the Dehighlight function in blueprints inheriting from this class.
*			 See Dehighlight_Implementation for functionality.
*
*    Data Members:
*        bool m_held
*			Indicates if this item is held or not.
**********************************************************************************************************/
UCLASS()
class HALVA_API AInventoryItem : public AItem, public IUseInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
		bool Use(USceneComponent* AttachPoint, UPARAM(ref) TArray<FString>& Args);
	virtual bool Use_Implementation(USceneComponent* AttachPoint, UPARAM(ref) TArray<FString>& Args) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
		bool Highlight();
		virtual bool Highlight_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
		bool Dehighlight();
		virtual bool Dehighlight_Implementation() override;

protected:
	
	bool m_held = false;
};
