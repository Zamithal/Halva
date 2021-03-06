// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "PotionItem.h"

/**********************************************************************************************************
*    APotionItem()
*        Purpose:    Default constructor.
**********************************************************************************************************/
APotionItem::APotionItem()
{
	MaximumQuantity = 100;
	CurrentQuantity = MaximumQuantity;
	TipAngle = 30;
	m_pouring = false;

	PourEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Pour Particles"));

	if (RootComponent != nullptr)
		PourEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
/**********************************************************************************************************
*    APotionItem()
*        Purpose:    UE4 Construction script.
**********************************************************************************************************/
void APotionItem::OnConstruction(const FTransform& Transform)
{
}
/**********************************************************************************************************
*    void APotionItem::BeginPlay()
*        Purpose:    Triggers once at the beginning of the game.
**********************************************************************************************************/
void APotionItem::BeginPlay()
{
	Super::BeginPlay();
}
/**********************************************************************************************************
*    AWeaponItem::Tick(float Deltatime)
*        Purpose:    Fires once each tick.
**********************************************************************************************************/
void APotionItem::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	float currentRotation = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorUpVector(), FVector(0,0,1))));

	if (currentRotation >= TipAngle && CurrentQuantity > 0)
	{
		PourEffect->Activate();

		FHitResult actorBelow = FHitResult();

		FVector traceStart = PourEffect->GetComponentLocation();
		// Down a bunch.
		FVector traceEnd = traceStart - FVector(0, 0, -1000);

		FCollisionObjectQueryParams collisionObject;
		collisionObject.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		collisionObject.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
		collisionObject.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

		FCollisionQueryParams ignoreList;
		ignoreList.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByObjectType(actorBelow, traceStart, traceEnd, collisionObject, ignoreList);

		if (Cast<IPotionInterface>(actorBelow.GetActor()) != nullptr)
		{
			PotionEffect(actorBelow.GetActor(), Deltatime);
		}

		m_pouring = true;
	}
	else
	{
		PourEffect->Deactivate();
		m_pouring = false;
	}
}
/**********************************************************************************************************
*    bool Empty()
*        Purpose:    Causes this potion to become empty.
*
*        Return:
*			 Returns true if the potion was emptied, false if it was already empty.
**********************************************************************************************************/
bool APotionItem::Empty()
{
	if (CurrentQuantity == 0)
		return false;

	CurrentQuantity = 0;

	return true;
}
/**********************************************************************************************************
*    bool EmptyAmount(float Amount)
*        Purpose:    Causes this potion to empty by the given amount.
*
*        Return:
*			 Returns true if potion emptied by the amount specified, false if there was not enough left.
*********************************************************************************************************/
bool APotionItem::EmptyAmount(float Amount)
{
	if (CurrentQuantity < Amount)
		return false;

	CurrentQuantity -= Amount;

	return true;
}
/**********************************************************************************************************
*    bool Fill()
*        Purpose:    Fills the potion to its maximum quantity it can hold.
*
*        Return:
*			 Returns true if the potion was filled at all. Returns false if the potion was already full.
**********************************************************************************************************/
bool APotionItem::Fill()
{
	if (CurrentQuantity == MaximumQuantity)
		return false;

	CurrentQuantity = MaximumQuantity;

	return true;
}
/**********************************************************************************************************
*    bool FillAmount(float Amount)
*        Purpose:    Increases the quantity of this potion by the amount specified.
*
*        Return:
*			 Returns true if the potion could be filled with the given amount and false if there was not
*			 enough space.
**********************************************************************************************************/
bool APotionItem::FillAmount(float Amount)
{
	if (CurrentQuantity > MaximumQuantity - Amount)
		return false;

	CurrentQuantity += Amount;

	return true;
}
/**********************************************************************************************************
*    bool IsPouring()
*        Purpose:    Returns if this potion is pouring or not.
*
*        Return:
*			 Returns true if the potion is pouring out.
**********************************************************************************************************/
bool APotionItem::IsPouring()
{
	return m_pouring;
}