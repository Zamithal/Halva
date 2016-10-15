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
*        bool Use(AActor* Sender, TArray<FString>& Args);
*			 Allows the overwriting of the Use function in blueprints inheriting from this class. See
*			 Use_Implementation for functionality.
*		 Use_Implementation(AActor* Sender, TArray<FString>& Args) override;
*        bool Highlight(AActor* Sender,  TArray<FString>& Args);
*			 Allows the overwriting of the Highlight function in blueprints inheriting from this class.
*			 See Highlight_Implementation for functionality.
*        bool Dehighlight(AActor* Sender, TArray<FString>& Args);
*			 Allows the overwriting of the Dehighlight function in blueprints inheriting from this class.
*			 See Dehighlight_Implementation for functionality.
*
*    Data Members:
*        int m_example
*            Example description.
**********************************************************************************************************/
UCLASS()
class HALVA_API AInventoryItem : public AItem, public IUseInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
		bool Use(AActor * Sender, UPARAM(ref) TArray<FString>& Args);
	virtual bool Use_Implementation(AActor* Sender, UPARAM(ref) TArray<FString>& Args) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
		bool Highlight(AActor * Sender, UPARAM(ref) TArray<FString>& Args);
	virtual bool Highlight_Implementation(AActor* Sender, UPARAM(ref) TArray<FString>& Args) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
		bool Dehighlight(AActor * Sender, UPARAM(ref) TArray<FString>& Args);
	virtual bool Dehighlight_Implementation(AActor* Sender, UPARAM(ref) TArray<FString>& Args) override;

protected:
	
	
};
