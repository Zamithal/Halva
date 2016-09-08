// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "VRPawn.h"
#include "SteamVRChaperoneComponent.h"
#include "VivePawn.generated.h"

/**********************************************************************************************************
*	Class: VivePawn
*
*	Overview:
*		This is a VRPawn for use with the HTC Vive. This class contains additional functionality specific
*		to the vive and should not be used if not using the Vive.
*
*	Data Members:
*		USteamVRChaperoneComponent * m_Chaperone
*			Shows the edges of the real world room to the user in the head mounted display.
**********************************************************************************************************/
UCLASS()
class HALVA_API AVivePawn : public AVRPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVivePawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	USteamVRChaperoneComponent * m_Chaperone;
};
