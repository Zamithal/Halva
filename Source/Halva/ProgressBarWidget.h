// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ProgressBarWidget.generated.h"

/**********************************************************************************************************
*	Class: ABaseEnemy
*
*	Overview:
*		This is the abstract base class all progress bars inherit from. Examples of progress bars can
*		include health bars, stamina bars, and experience bars.
*
*	Methods:
*		void SetFilledPercentage(float Percentage)
*			Overwrite-able event that sets how full the progress bar is.
**********************************************************************************************************/
UCLASS()
class HALVA_API UProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Progress")
		void SetFilledPercentage(float Percentage);

protected:
};
