// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "WeaponItem.h"
#include "ItemDamageArrow.h"

/**********************************************************************************************************
*    AWeaponItem()
*        Purpose:    Default constructor.
**********************************************************************************************************/
AWeaponItem::AWeaponItem()
{
	WeaponBaseDamage = 1;
	WeaponWeight = 1;
}
/**********************************************************************************************************
*    AWeaponItem()
*        Purpose:    UE4 Construction script.
**********************************************************************************************************/
void AWeaponItem::OnConstruction(const FTransform& Transform)
{
	m_inputSum = FVector::ZeroVector;
	m_lastFrameLocation = FVector::ZeroVector;
}
/**********************************************************************************************************
*    void AWeaponItem::BeginPlay()
*        Purpose:    Triggers once at the beginning of the game.
**********************************************************************************************************/
void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();

	GetComponents<UItemHitBox, FDefaultAllocator>(m_hitBoxes);

	CreateDamageVectors();

	// Enqueue the 45 frames from before the game started.
	for (int i = 0; i < 45; i++)
		m_previousFrameVectors.Enqueue(FVector::ZeroVector);

	// Bind events.
	UStaticMeshComponent* RootMesh = Cast<UStaticMeshComponent>(RootComponent);
	if (RootMesh != nullptr)
	{
		RootMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponItem::WeaponHit);
	}
}
/**********************************************************************************************************
*    AWeaponItem::Tick(float Deltatime)
*        Purpose:    Fires once each tick.
**********************************************************************************************************/
void AWeaponItem::Tick(float Deltatime)
{
	Super::Tick(Deltatime);


	static FVector vector = FVector::ZeroVector;

	m_previousFrameVectors.Dequeue(vector);

	m_inputSum -= vector;

	vector = ActorToWorld().GetTranslation() - m_lastFrameLocation;

	// Optimal frame-rate is 90fps
	vector *= (Deltatime * 90);

	m_inputSum += vector;

	m_previousFrameVectors.Enqueue(vector);

	m_lastFrameLocation = ActorToWorld().GetTranslation();
}
/**********************************************************************************************************
*    void WeaponHit(...)
*        Purpose:    Triggers when this weapon comes into contact with another actor. First checks to
*					 make sure that the actor is not the owner, if it's not, The weapon deals damage to
*					 that actor.
*
*        Parameters:
*            Standard Overlap parameters.
**********************************************************************************************************/
void AWeaponItem::WeaponHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComp->GetPhysicsLinearVelocity() == FVector::ZeroVector)
		return;
	if (this->GetAttachParentActor() == OtherActor)
		return;
	// ASSUMPTION: Only root components should be damageable. If a part of an enemy needs to be damaged
	// EG Bongo Bongo's hands, use a child actor component.
	if (OtherComp != OtherActor->GetRootComponent())
		return;

	float damage = CalculateDamage(OtherActor);

	FDamageEvent damageEvent = FDamageEvent();

	OtherActor->TakeDamage(damage, FDamageEvent(), nullptr, this);
}
/**********************************************************************************************************
*    void CreateDamageVectors()
*        Purpose:    Creates a set of damage vectors from the attached damage arrows. A damage vector is
*					 created for each damage arrow attached to the weapon. Each vector is then summed with
*					 each other vector. The new vectors are then added to the damage vectors, culling any
*					 zero vectors or vectors with the same direction as an existing vector.
*
*        Changes:
*            m_damageVectors
*				 Vectors will be added to this.
**********************************************************************************************************/
void AWeaponItem::CreateDamageVectors()
{
	TArray<UItemDamageArrow *> damageArrows = TArray<UItemDamageArrow *>();
	GetComponents<UItemDamageArrow, FDefaultAllocator>(damageArrows);

	TArray<FVector> unitDamageVectors = TArray<FVector>();
	FVector newVector = FVector::ZeroVector;

	m_damageVectors.Empty();

	for (TArray<UItemDamageArrow *>::TIterator iter = damageArrows.CreateIterator(); iter; ++iter)
	{
		newVector = (*iter)->GetForwardVector();

		unitDamageVectors.Add(newVector);

		m_damageVectors.Add(newVector * (*iter)->Magnitude);
	}

	// O-n^2
	int initialVectors = m_damageVectors.Num();
	TQueue<FVector> impliedVectors;

	for (int i = 0; i < initialVectors; i++)
	{
		for (int j = i; j < initialVectors; j++)
		{
			if (i != j)
				impliedVectors.Enqueue(m_damageVectors[i] + m_damageVectors[j]);
		}
	}


	while (impliedVectors.IsEmpty() == false)
	{
		impliedVectors.Dequeue(newVector);

		if (newVector != FVector::ZeroVector)
		{
			int matchingIndex = 0;

			if (unitDamageVectors.Find(newVector / newVector.Size(), matchingIndex))
			{
				if (newVector.Size() > m_damageVectors[matchingIndex].Size())
				{
					m_damageVectors[matchingIndex] = newVector;
				}
			}
			else
			{
				unitDamageVectors.Add(newVector / newVector.Size());
				m_damageVectors.Add(newVector);
			}

		}
	}
}
/**********************************************************************************************************
*    float CalculateDamage(AActor* TriggeringActor = nullptr)
*        Purpose:    Calculates the amount of damage that should be done to the triggering actor. This is
*					 dependent on the angle the weapon is currently traveling, the hit boxes the actor
*					 overlaps with, and the base damage of the weapon. If no actor is specified, the 
*					 overlap check is skipped with a assumed return value of 1.
*
*        Parameters:
*            AActor* TriggeringActor
*				 The actor that triggered the call to this function. Defaults to nullptr.
*
*        Return:
*			 Returns how much damage the weapon should do to the triggering actor at the time of this call.
**********************************************************************************************************/
float AWeaponItem::CalculateDamage(AActor* TriggeringActor)
{
	// DIRECTION

	// unit vector in current direction of object's movement.
	FVector direction = ActorToWorld().GetTranslation() - m_lastFrameLocation;
	direction /= direction.Size();

	float maximumDirectionMultiplier = 0;

	// Projection of direction onto each damageVector. Pick the largest multiplier achievable.
	for (int i = 0; i < m_damageVectors.Num(); i++)
	{
		float currentMultiplier = (direction.ProjectOnTo(m_damageVectors[i]) * m_damageVectors[i].Size()).Size();

		FMath::Clamp(currentMultiplier, 0.0f, m_damageVectors[i].Size());

		if (currentMultiplier > maximumDirectionMultiplier)
			maximumDirectionMultiplier = currentMultiplier;
	}


	// SPEED
	float speedMultiplier = 1;
	if (WeaponWeight > 0)
	 speedMultiplier = m_inputSum.Size() / WeaponWeight;

	// LOCATION
	float locationMultiplier = 0;
	bool hit = false;
	if (TriggeringActor != nullptr)
	{
		for (TArray<UItemHitBox *>::TIterator iter = m_hitBoxes.CreateIterator(); iter; ++iter)
		{
			if ((*iter)->IsOverlappingActor(TriggeringActor))
			{
				hit = true;
				if ((*iter)->Strength > locationMultiplier)
				{
					locationMultiplier = (*iter)->Strength;
				}
			}
		}
	}

	// Default for no hitbox setup.
	if (hit == false)
		locationMultiplier = 1;

	return WeaponBaseDamage * maximumDirectionMultiplier * speedMultiplier * locationMultiplier;
}
/**********************************************************************************************************
*    TArray<FVector> AWeaponItem::GetDamageVectors()
*        Purpose:    Getter.
**********************************************************************************************************/
TArray<FVector> AWeaponItem::GetDamageVectors()
{
	return m_damageVectors;
}
/**********************************************************************************************************
*    TArray<UItemHitBox *> AWeaponItem::GetHitBoxes()
*        Purpose:    Getter.
**********************************************************************************************************/
TArray<UItemHitBox *> AWeaponItem::GetHitBoxes()
{
	return m_hitBoxes;
}
/**********************************************************************************************************
*    FVector AWeaponItem::GetInputSum()
*        Purpose:    Getter.
**********************************************************************************************************/
FVector AWeaponItem::GetInputSum()
{
	return m_inputSum;
}