// © 2016 Skyler Swenson, some rights reserved.
#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"
/**********************************************************************************************************
*    Class: AItem
*
*    Overview:
*        This is a base class that all items inherit from. All items have a name, a description and a mesh.
*		 Items cannot be interacted with in any way unless they incorporate some level of the use interface.
*
*
*    UProperties
*        FString Name
*			The name of the item.
*		 FString Description
*			Brief text describing the item.
*		 UStaticMesh* Mesh
*			A pointer to the static mesh the item uses.
**********************************************************************************************************/

UCLASS()
class HALVA_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomSeed")
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomSeed")
		UStaticMesh * Mesh;


	
};
