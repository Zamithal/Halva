// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "Item.h"
#include "UseInterface.h"
#include "GrabableItem.generated.h"

/**********************************************************************************************************
*    Class: AInventoryItem
*
*    Overview:
*        A grab-able item is an item that can be picked up by the player. This class implements the
*		 IUseInterface. When an object is used, it becomes attached to the user using optional attachment
*		 options. When used again, the object becomes detached and falls freely.
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
**********************************************************************************************************/
UCLASS()
class HALVA_API AGrabableItem : public AItem, public IUseInterface
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
	
	
	
};
