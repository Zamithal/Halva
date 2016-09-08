// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "VRPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HALVA_API AVRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// Type of widget to use for the vision restrictor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		TSubclassOf<class UUserWidget> wFOVRestrictor;

	UUserWidget* FOVRestrictor;

	// Override BeginPlay()
	virtual void BeginPlay() override;
	
	
};
