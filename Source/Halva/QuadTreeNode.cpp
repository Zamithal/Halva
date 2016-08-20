// Fill out your copyright notice in the Description page of Project Settings.

#include "Halva.h"
#include "QuadTreeNode.h"
/**********************************************************************************************************
*	QuadTreeNode()
*		Purpose:	Default constructor. Results in a blank object.
**********************************************************************************************************/
QuadTreeNode::QuadTreeNode()
{
	m_quad = Quad(FVector(0, 0, 0), FVector(0, 0, 0));
	m_stream = FRandomStream(0);
	m_minimumRoomSize = FVector(0, 0, 0);

	for (int i = 0; i < 4; i++)
		m_children[i] = nullptr;
}
/**********************************************************************************************************
*	QuadTreeNode(int Depth, Quad Bounds, FVector MinimumRoomSize, FRandomStream Stream)
*		Purpose:	Constructor. Recursively creates children node until depth reaches 0. The children will
*					be their parent sliced up into 4ths. No slice shall be smaller than minimum room size
*					and the size of each slice is randomized. Stream is used for randomization.
*
*		Parameters:
*			int Depth
*				The number of times to create children. If 1, 4 children will be created. If 2, those 4
*				children will have 4 children and so on.
*			FVector MinimumRoomSize
*				The smallest possible room that can be built.
*			FRandomStream Stream
*				A random stream object to generate randomness.
**********************************************************************************************************/
QuadTreeNode::QuadTreeNode(int Depth, Quad Bounds, FVector MinimumRoomSize, FRandomStream Stream)
{
	m_quad = Bounds;
	m_minimumRoomSize = MinimumRoomSize;
	m_stream = Stream;

	if (Depth > 0)
		CreateChildren(Depth);
}
/**********************************************************************************************************
*	QuadTreeNode(const QuadTreeNode & Source)
*		Purpose:	Copy constructor. Deep copy.
*
*		Parameters:
*			const QuadTreeNode & Source
*				The QuadTreeNode to copy from.
**********************************************************************************************************/
QuadTreeNode::QuadTreeNode(const QuadTreeNode & Source)
{
	m_quad = Source.m_quad;
	m_stream = Source.m_stream;
	m_minimumRoomSize = Source.m_minimumRoomSize;

	for (int i = 0; i < 4; i++)
		if (Source.m_children[i] != nullptr)
			m_children[i] = new QuadTreeNode(*Source.m_children[i]);
}
/**********************************************************************************************************
*	QuadTreeNode & operator=(const QuadTreeNode & Source)
*		Purpose:	operator=. Assigns RHS to LHS. Deep copy.
*
*		Parameters:
*			const QuadTreeNode & Source
*				The object to copy data from.
*
*		Return:		Returns this by reference.
**********************************************************************************************************/
QuadTreeNode & QuadTreeNode::operator=(const QuadTreeNode & Source)
{
	if (&Source != this)
	{
		m_quad = Source.m_quad;
		m_stream = Source.m_stream;
		m_minimumRoomSize = Source.m_minimumRoomSize;

		for (int i = 0; i < 4; i++)
			if (Source.m_children[i] != nullptr)
			{
				if (m_children[i] != nullptr)
					delete m_children[i];

				m_children[i] = new QuadTreeNode(*Source.m_children[i]);
			}
	}
	return *this;
}
/**********************************************************************************************************
*	~QuadTreeNode()
*		Purpose:	Destructor. Frees all children belonging to this node.
*
*		Changes:	m_children will be deleted.
**********************************************************************************************************/
QuadTreeNode::~QuadTreeNode()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_children[i] != nullptr)
			delete(m_children[i]);
	}
}
/**********************************************************************************************************
*	void CreateChildren(int Depth)
*		Purpose:	Causes this node's children to generate. The children will have quads that sum to the
*					area of the parent but are not equal slices. If a depth of greater than 1 is,
*					specified, this function will recurse dividing each child node as well.
*
*		Parameters: 
*			int Depth
*				The number of times to cut the children. If 1 4 children will be created, if 2,
*				Those children will be sliced creating 16 leaf nodes and so on.
*
*		Changes:	m_children will be populated.
**********************************************************************************************************/
void QuadTreeNode::CreateChildren(int Depth)
{
	QuadSlices childQuads = Slice();

	for (int i = 0; i < 4; i++)
		if (m_children[i] == nullptr)
			delete m_children[i];

	m_children[0] = new QuadTreeNode(Depth - 1, childQuads.southWest, m_minimumRoomSize, m_stream);
	m_children[1] = new QuadTreeNode(Depth - 1, childQuads.northWest, m_minimumRoomSize, m_stream);
	m_children[2] = new QuadTreeNode(Depth - 1, childQuads.northEast, m_minimumRoomSize, m_stream);
	m_children[3] = new QuadTreeNode(Depth - 1, childQuads.southEast, m_minimumRoomSize, m_stream);

}
/**********************************************************************************************************
*	QuadSlices Slice()
*		Purpose:	Slices this quad into 4 smaller quads. these quads are not of even size but the sum
*					of their area will add up to the area of this quad. The 4 quads will be returned as
*					a QuadSlices struct which just contains 4 quads.
*
*		Return:		Returns a QuadSlices struct containing 4 quads. These quads will sum to the original
*					quad sliced into 4ths.
*
*		Changes:	None.
**********************************************************************************************************/
QuadSlices QuadTreeNode::Slice()
{
	QuadSlices slicedUp;

	FVector quadBounds = m_quad.GetBounds();
	FVector quadOffset = m_quad.GetPosition();

	// Throw out any decimals the user might have entered.
	quadBounds = FVector(floor(quadBounds.X), floor(quadBounds.Y), floor(quadBounds.Z));
	quadOffset = FVector(floor(quadOffset.X), floor(quadOffset.Y), floor(quadOffset.Z));

	// define the minimum and maximum slice locations.
	int minX = (int)m_minimumRoomSize.X;
	int minY = (int)m_minimumRoomSize.Y;

	int maxX = ((int)quadBounds.X - (int)m_minimumRoomSize.X);
	int maxY = ((int)quadBounds.Y - (int)m_minimumRoomSize.Y);


	int xSlice = m_stream.RandRange(minX, maxX);
	int ySlice = m_stream.RandRange(minY, maxY);

	// Bottom left quad.
	slicedUp.southWest.SetPosition(quadOffset);
	slicedUp.southWest.SetBounds(FVector(xSlice, ySlice, 0));

	// Bottom right quad.
	slicedUp.southEast.SetPosition(FVector(xSlice + quadOffset.X, quadOffset.Y, 0));
	slicedUp.southEast.SetBounds(FVector(quadBounds.X - xSlice, ySlice, 0));

	// Top left quad.
	slicedUp.southWest.SetPosition(FVector(quadOffset.X, quadOffset.Y + ySlice, 0));
	slicedUp.southWest.SetBounds(FVector(xSlice, quadBounds.Y - ySlice, 0));

	// Top right quad.
	slicedUp.northEast.SetPosition(FVector(quadOffset.X + xSlice, quadOffset.Y + ySlice, 0));
	slicedUp.northEast.SetBounds(FVector(quadBounds.X - xSlice, quadBounds.Y - ySlice, 0));

	return slicedUp;
}
/**********************************************************************************************************
*	QuadTreeNode ** GetChildren()
*		Purpose:	Getter.
**********************************************************************************************************/
QuadTreeNode ** QuadTreeNode::GetChildren()
{
	return m_children;
}
/**********************************************************************************************************
*	Quad GetQuad()
*		Purpose:	Getter.
**********************************************************************************************************/
Quad QuadTreeNode::GetQuad()
{
	return m_quad;
}
/**********************************************************************************************************
*	Quad * GetRoom()
*		Purpose:	Getter.
**********************************************************************************************************/
Quad * QuadTreeNode::GetRoom()
{
	return m_room;
}
/**********************************************************************************************************
*	void SetRoom(Quad * Room)
*		Purpose:	Setter.
**********************************************************************************************************/
void QuadTreeNode::SetRoom(Quad * Room)
{
	m_room = Room;
}
/**********************************************************************************************************
*		FVector GetMinimumRoomSize()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector QuadTreeNode::GetMinimumRoomSize()
{
	return m_minimumRoomSize;
}
/**********************************************************************************************************
*	void SetMinimumRoomSize(FVector MinimumRoomSize)
*		Purpose:	Setter.
**********************************************************************************************************/
void QuadTreeNode::SetMinimumRoomSize(FVector MinimumRoomSize)
{
	m_minimumRoomSize = MinimumRoomSize;
}
/**********************************************************************************************************
*	FRandomStream GetRandomStream()
*		Purpose:	Getter.
**********************************************************************************************************/
FRandomStream QuadTreeNode::GetRandomStream()
{
	return m_stream;
}
/**********************************************************************************************************
*	void SetRandomStream(FRandomStream RandomStream)
*		Purpose:	Setter.
**********************************************************************************************************/
void QuadTreeNode::SetRandomStream(FRandomStream RandomStream)
{
	m_stream = RandomStream;
}