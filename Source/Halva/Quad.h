// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**********************************************************************************************************
*	Class: Quad
*
*	Overview:
*		Data structure for containing 4 points in space. These quads are axis aligned rectangular prisms.
*
*	Manager Functions:
*
*		Quad()
*			Default constructor. Results in a 0x0x0 quad at position 0,0,0.
*		Quad(FVector Size, FVector Position)
*			Results in a quad of size Size at position Position.
*		Quad(const Quad & source)
*			Creates a new quad that is a copy of the quad passed in.
*		~Quad()
*			Destructor.
*		Quad & operator=(const Quad & source)
*			Overrides the = operator to cause expected behavior.
*		bool operator==(const Quad & rhs)
*			Compares rhs to see if it is equal to this. This is only true if bounds and position are the
*			same.
*
*	Mutators:
*	
*		Bounds
*			-Get
*			-Set
*		Position
*			-Get
*			-Set
*
*	Data Members:
*
*		FVector m_bounds - The extents of the quad.
*		FVector m_position - The position of the quad.
**********************************************************************************************************/
class HALVA_API Quad
{
public:
	Quad();
	Quad(FVector Size, FVector Position);
	Quad(const Quad & source);
	~Quad();
	Quad & operator=(const Quad & source);
	bool operator==(const Quad & rhs) const;

	FVector GetBounds();
	void SetBounds(FVector newBounds);
	FVector GetPosition();
	void SetPosition(FVector newPosition);

private:
	FVector m_bounds;
	FVector m_position;
};
