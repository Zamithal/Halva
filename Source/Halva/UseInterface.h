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
*		 the player activates an item. An example of using an item would be pressing a button or picking
*		 up an item. Additionally, this interface has 2 virtual function Highlight() and
*		 Dehighlight(). These functions are intended to be overwritten with an indicator of when you can
*		 and cannot Use() an object.
*
*    Methods:
*        virtual void Use()
*			Virtual method to be overridden by other interfaces. This does nothing on it's own.
*		 virtual void Highlight()
*			Applies a post process effect causing all meshes in the HighlightedMeshes list to have an
*			outline effect.
*		 virtual void Dehighlight()
*			Removes the post process effect causing the outline effect around all the meshes in
*			HighlightedMeshes.
**********************************************************************************************************/
class HALVA_API IUseInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Use();
	virtual void Highlight();
	virtual void Dehighlight();
};
