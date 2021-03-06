// © 2016 Skyler Swenson, some rights reserved.
#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"
/**********************************************************************************************************
*   Class: AItem
*
*   Overview:
*        This is a base class that all items inherit from. All items have a name, a description and a mesh.
*		 Items cannot be interacted with in any way unless they incorporate some level of the use interface.
*
*	Methods:
*		CalculateDamage(AActor* TriggeringActor = nullptr);
*			Calculates the damage this item would deal this frame. Unless overridden, this will always
*			return 0.
*
*    UProperties
*       FString Name
*			The name of the item.
*		FString Description
*			Brief text describing the item.
*		UStaticMeshComponent* ItemMesh
*			The mesh the item uses in the world.
**********************************************************************************************************/

UCLASS()
class HALVA_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual float CalculateDamage(AActor* TriggeringActor = nullptr);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		UStaticMeshComponent* ItemMesh;
};
