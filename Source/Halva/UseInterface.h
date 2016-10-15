// © 2016 Skyler Swenson, some rights reserved.
#pragma once

#include "UseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUseInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**********************************************************************************************************
*    Interface: IUseInterface
*
*    Overview:
*        This interface includes a Virtual function Use, that is intended to be triggered whenever
*		 activates something. An example of using an item would be pressing a button or picking
*		 up an item. Additionally, this interface has 2 virtual function Highlight() and
*		 Dehighlight(). These functions are intended to be overwritten with an indicator of when you can
*		 and cannot Use() an object.
*
*    Methods:
*        bool Use(AActor* Sender, TArray<FString>& Args)
*			Use the object.
*		 bool Highlight(AActor* Sender, TArray<FString>& Args)
*			Highlights the object.
*		 bool Dehighlight(AActor* Sender, TArray<FString>& Args)
*			Removes the highlighting from the object.
**********************************************************************************************************/
class HALVA_API IUseInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Usable")
		bool Use(AActor * Sender, UPARAM(ref) TArray<FString>& Args);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
		bool Highlight(AActor * Sender, UPARAM(ref) TArray<FString>& Args);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
		bool Dehighlight(AActor * Sender, UPARAM(ref) TArray<FString>& Args);
};
