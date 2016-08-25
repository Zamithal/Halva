// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Quad.h"

struct QuadSlices
{
	Quad northWest;
	Quad northEast;
	Quad southEast;
	Quad southWest;
};

/**********************************************************************************************************
*	Class: QuadTreeNode
*
*	Overview:
*		This is a node in a quad tree. It contains its size in the bounds variable and its location using
*		the offset. It can contain 4 children, all of which are this quad cut up into 4 quads but may not
*		if this quad is a leaf node.
*
*	Manager Functions:
*
*		QuadTreeNode(FVector Bounds, FVector offset, FVector MinimumQuadSize, FRandomStream Stream);
*			Creates a new node in the quad tree and recursively creates children beneath it. If depth is
*			set to 1, no children will be created. If set to 2, 1 set of children will be created. 2 will
*			cause the children to have children and so forth.
*			
*		QuadTreeNode(const QuadTreeNode & source);
*			Copy constructor. takes in source and creates a copy of it.
*		~QuadTreeNode();
*			Destructor. Frees all children.
*		QuadTreeNode & operator=(const QuadTreeNode & source);
*			Assignment operator.
*
*	Mutators
*		Children
*			-Get
*		Quad
*			-Get
*		Room
*			-Get
*			-Set
*		MinimumRoomSize
*			-Get
*			-Set
*		RandomStream
*			-Get
*			-Set
*		
*	Methods:
*
*		bool CreateChildren(int Depth);
*			Populates the children array with 4 quads that are composed of the parent quad cut into 4
*			slices. The slices are not equal sizes and are determined by the function Slice();.
*		QuadSlices Slice(int PlannedDivisions);
*			Slices up the quad into 4 random slices. The random slices are at least big enough to support
*			PlannedDivisions rooms of minimum room size. This means that if planned divisions is 2 each 
*			child will have enough space for 4 rooms (2 across and 2 down). A planned divisions value of 3
*			will support 8 rooms.
*		
*			
*	Data Members:
*
*		QuadTreeNode * m_children[4] - Array of quads this quad is split up into.
*		Quad m_quad - The space that makes up this quad.
*		Quad * m_room - The room belonging to this node. This will only be ever set in leaf nodes and is
*						not set by the node.
*		FVector m_minimumQuadSize - The smallest leaf node Quad that can be generated.	
*		FRandomStream m_stream - The random stream used to generate randomness.
**********************************************************************************************************/
class HALVA_API QuadTreeNode
{
public:

	QuadTreeNode();
	QuadTreeNode(int Depth, Quad Bounds, FVector MinimumQuadSize, FRandomStream Stream);
	QuadTreeNode(const QuadTreeNode & source);
	QuadTreeNode & operator=(const QuadTreeNode & source);
	~QuadTreeNode();

	QuadTreeNode ** GetChildren();
	Quad GetQuad();
	Quad * GetRoom();
	void SetRoom(Quad * Room);
	FVector GetMinimumQuadSize();
	void SetMinimumQuadSize(FVector MinimumRoomSize);
	FRandomStream GetRandomStream();
	void SetRandomStream(FRandomStream RandomStream);

private:

	bool CreateChildren(int Depth);
	QuadSlices Slice(int PlannedDivisions);

	QuadTreeNode * m_children[4];
	Quad m_quad;
	Quad * m_room;
	FVector m_minimumQuadSize;
	FRandomStream m_stream;
};

