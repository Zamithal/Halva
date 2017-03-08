// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "VRPawn.h"
#include "Runtime/Engine/Classes/Haptics/HapticFeedbackEffect_Curve.h"
#include "VRPlayerController.generated.h"

/**********************************************************************************************************
*	Class: AVRPlayerController
*
*	Overview:
*		Player controller that contains all functionality required to control a VR pawn. This should be
*		Inherited from in blueprints to give definitions where required.	
*
*	Methods:
*		virtual void SetupInputComponent() override
*			Binds inputs from the player's motion controllers to movement in game.
*		virtual void PlayerTick(float DeltaTime) override
*			Each tick calls the locomotion function assigned to move the pawn.
*		void LocomotionWalkForward()
*			Locomotion function. Walks the player forward from their initial direction when the button
*			was pressed. The player will continue to walk forward until the button is released.
*		virtual void RestrictVision()
*			Restricts the player's vision using the FOV Restrictor widget. The specifics of this are
*			implemented by child classes.
*		virtual void UnrestrictVision()
*			Frees the players vision using the FOV Restrictor widget. The specifics of this are
*			implemented by child classes.
*		void BeginMoveLeft()
*			Fires upon the move button being pressed on the left motion controller.
*		void EndMoveLeft()
*			Fires upon the move button being released on the left motion controller.
*		void BeginMoveRight()
*			Fires upon the move button being pressed on the right motion controller.
*		void EndMoveRight()
*			Fires upon the move button being released on the right motion controller.
*       void Recalibrate()
*           Sets the player's height to the current height of the HMD.
*		void HoverOverObject(...)
*			Fires when an object is hovered over by a hand.
*		void EndHoverOverObject(...)
*			Fires when an object is no longer hovered over by a hand.
*		void EndHoverOverObjectRight
*			Fires when an object is no longer hovered over by the right hand.
*		void GrabPressedLeft()
*			Fires when the grab button is pressed on the left controller.
*		void GrabPressedRight()
*			Fires when the grab button is pressed on the right controller.
*		void GrabPressed(UPrimitiveComponent* HandCollider, AActor* GrabbedObject)
*			Called by one of the grab pressed functions. Executes behavior for manipulating the grabbed
*			object.
*		bool HighlightUsable(AActor* Object, UPrimitiveComponent* TriggeringObject);
*			Highlights the passed in object.
*		bool DehighlightUsable(AActor* Object, UPrimitiveComponent* TriggeringObject);
*			Dehighlights the passed in object.
*		bool InformShoulderBoxHit(USceneComponent * ShoulderBox, UPrimitiveComponent* TriggeringObject);
*			Creates a haptic feedback event when a shoulder box is overlapped.
*		bool SheatheItem(USceneComponent*  ShoulderBox, AActor* SheathedActor);
*			Overrides default use behavior to instead attach the item to the ShoulderBox
*
*	Data Members:
*		TSubclassOf<class UUserWidget> FOVRestrictorType
*			Type of widget used to restrict the player's vision during actions prone to cause player motion
*			sickness.
*		UUserWidget * FOVRestrictor
*			Reference to widget used to restrict player vision.
*		bool m_LeftMovePressed
*			Is the left motion controller's move button held down?
*		bool m_RightMovePressed
*			Is the right motion controller's move button held down?
*		IUseInterface* m_LeftHandObject
*			The object the left hand is hovering over.
*		IUseInterface* m_RightHandObject
*			The object the right hand is hovering over.
*		void(*m_LocomotionFunction)()
*			Function used for locomotion. This gets called each tick to update the position of the player
*			pawn.
*		UHapticFeedbackEffect_Curve* m_SinglePulseHapticEffect
*			The haptic effect that occurs when a haptic event is played.
**********************************************************************************************************/
UCLASS(abstract)
class HALVA_API AVRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AVRPlayerController();
	void OnConstruction(const FTransform& Transform);

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

	void LocomotionWalkForward(float DeltaTime);

	// These functions should be overridden to implement intended behavior.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Appearance")
		void RestrictVision();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Appearance")
		void UnrestrictVision();

	void BeginMoveLeft();
	void EndMoveLeft();
	void BeginMoveRight();
	void EndMoveRight();

    void Recalibrate();

	// Type of widget to use for the vision restrictor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		TSubclassOf<class UUserWidget> FOVRestrictorType;

	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
		UUserWidget* FOVRestrictor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		float maxObscuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		UHapticFeedbackEffect_Curve* m_SinglePulseHapticEffect;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	UFUNCTION()
		void GrabPressedLeft();
	UFUNCTION()
		void GrabPressedRight();
	UFUNCTION()
		void HoverOverObject(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndHoverOverObject(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	void GrabPressed(UPrimitiveComponent* HandCollider, AActor* GrabbedObject);

	bool HighlightUsable(AActor* Object, UPrimitiveComponent* TriggeringObject);
	bool DehighlightUsable(AActor* Object, UPrimitiveComponent* TriggeringObject);

	bool InformShoulderBoxHit(USceneComponent * ShoulderBox, UPrimitiveComponent* TriggeringObject);
	bool SheatheItem(USceneComponent*  ShoulderBox, AActor* SheathedActor);

	bool m_LeftMovePressed;
	bool m_RightMovePressed;

	AActor* m_LeftHandObject;
	AActor* m_RightHandObject;

	AVRPawn* m_player;

	void(AVRPlayerController::*m_LocomotionFunction)(float);
};
