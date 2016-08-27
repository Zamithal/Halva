// Fill out your copyright notice in the Description page of Project Settings.

#include "Halva.h"
#include "Quad.h"
/**********************************************************************************************************
*	Quad()
*		Purpose:	Default constructor. Results in a 0x0 quad at 0x0.
**********************************************************************************************************/
Quad::Quad()
{
	m_bounds = FVector(0, 0, 0);
	m_position = FVector(0, 0, 0);
}
/**********************************************************************************************************
*	Quad(FVector Size, FVector Position)
*		Purpose:	Generates a Quad of the given size at the given position.
*
*		Parameters:
*			Size
*				The size of the quad to make.
*			Position
*				The offset of the bottom left corner from its parent.
**********************************************************************************************************/
Quad::Quad(FVector Size, FVector Position)
{
	m_bounds = Size;
	m_position = Position;
}
/**********************************************************************************************************
*	Quad(const Quad & Source)
*		Purpose:	copy Constructor. The created quad is an exact copy of the existing quad Source.
*
*		Parameters:
*			Source
*				The Quad to copy.
**********************************************************************************************************/
Quad::Quad(const Quad & Source)
{
	m_bounds = Source.m_bounds;
	m_position = Source.m_position;
}
/**********************************************************************************************************
*	~Quad()
*		Purpose:	Destructor.
**********************************************************************************************************/
Quad::~Quad()
{
}
/**********************************************************************************************************
*	Quad & operator=(const Quad & Source)
*		Purpose:	Causes this node's children to generate. The children will have quads that sum to the
*					area of the parent but are not equal slices. If a depth of greater than 1 is,
*					specified, this function will recurse dividing each child node as well.
*
*		Parameters:
*			const Quad & Source
*				The Quad to copy. This will become an exact copy of Source.
*
*		Changes:	This quad performs a deep copy of Source.
*
*		Return - This is returned by reference for operator chaining.
**********************************************************************************************************/
Quad & Quad::operator=(const Quad & Source)
{
	if (&Source != this)
	{
		m_bounds = Source.m_bounds;
		m_position = Source.m_position;
	}

	return *this;
}
/**********************************************************************************************************
*	bool operator==(const Quad & rhs)
*		Purpose:	Compares the quad passed at rhs to this. If the bounds and position of both quads are
*					the same. They are the same.
*
*		Parameters:
*			const Quad & rhs
*				The Quad to compare this to.
*
*		Return - Returns if this quad is the same as the quad passed in.
**********************************************************************************************************/
bool Quad::operator==(const Quad & rhs) const
{
	bool equal = true;

	if (m_position != rhs.GetPosition())
		equal = false;

	if (m_bounds != rhs.GetBounds())
		equal = false;

	return equal;
}
/**********************************************************************************************************
*	FVector GetBounds()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector Quad::GetBounds() const
{
	return m_bounds;
}
/**********************************************************************************************************
*	void SetBounds(FVector NewBounds)
*		Purpose:	Setter.
**********************************************************************************************************/
void Quad::SetBounds(FVector NewBounds)
{
	m_bounds = NewBounds;
}
/**********************************************************************************************************
*	FVector GetPosition()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector Quad::GetPosition() const
{
	return m_position;
}
/**********************************************************************************************************
*	void SetPosition()
*		Purpose:	Setter.
**********************************************************************************************************/
void Quad::SetPosition(FVector newPosition)
{
	m_position = newPosition;
}