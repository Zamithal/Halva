// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BTT_Strafe.h"

UBTT_Strafe::UBTT_Strafe()
{
	RotationMethod = ERotationQuantity::Degrees;
	Direction = ERotationDirection::Clockwise;
	StrafeSpeed = 1;
	RotationAmount = 0;
	MaximumRotationVariance = 0;
	m_rotationActor = nullptr;
	m_locationLastFrame = FVector::ZeroVector;
	bNotifyTick = true;
}

void UBTT_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// update the rotation to face the center.
	FVector direction = m_rotationActor->GetActorLocation() - OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	FRotator newRotation = direction.Rotation();

	newRotation.Pitch = 0;
	newRotation.Roll = 0;

	OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(newRotation);


	FVector tangent = FRotator(0, newRotation.Yaw + 90, 0).Vector();

	if (Direction == ERotationDirection::Clockwise)
		tangent *= -1;


	// move tangent to the circle
	OwnerComp.GetAIOwner()->GetPawn()->AddMovementInput(tangent, DeltaSeconds * StrafeSpeed);



	if (RotationMethod == ERotationQuantity::Degrees)
	{
		FVector a = m_locationLastFrame - m_rotationActor->GetActorLocation();
		FVector b = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation() - m_rotationActor->GetActorLocation();

		// Angle between last frame and this frame.
		m_rotationRemaining -= FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(a, b) / (a.Size() * b.Size())));

		m_locationLastFrame = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	}
	else
	{
		m_rotationRemaining -= DeltaSeconds;
	}
	

	if (m_rotationRemaining <= 0)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_rotationActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (m_rotationActor == nullptr)
		return EBTNodeResult::Failed;

	m_rotationRemaining = RotationAmount;

	m_rotationRemaining += FMath::RandRange(-MaximumRotationVariance, MaximumRotationVariance);

	if (m_rotationRemaining < 0)
		m_rotationRemaining = 0;

	if (Direction == ERotationDirection::Random)
	{
		bool clockwise = FMath::RandBool();

		if (clockwise)
			Direction = ERotationDirection::Clockwise;
		else
			Direction = ERotationDirection::CounterClockwise;
	}

	m_locationLastFrame = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	return EBTNodeResult::InProgress;
}