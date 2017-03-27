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
	m_minimumQuadSize = FVector(0, 0, 0);
	m_room = Quad();

	for (int i = 0; i < 4; i++)
		m_children[i] = nullptr;
}
/**********************************************************************************************************
*	QuadTreeNode(int Depth, Quad Bounds, FVector MinimumQuadSize, FRandomStream Stream)
*		Purpose:	Constructor. Recursively creates children node until depth reaches 0. The children will
*					be their parent sliced up into 4ths. No slice shall be smaller than minimum room size
*					and the size of each slice is randomized. Stream is used for randomization.
*
*		Parameters:
*			int Depth
*				The number of times to create children. If 1, 4 children will be created. If 2, those 4
*				children will have 4 children and so on.
*			FVector MinimumQuadSize
*				The smallest possible leaf node Quad that can be built.
*			FRandomStream Stream
*				A random stream object to generate randomness.
**********************************************************************************************************/
QuadTreeNode::QuadTreeNode(int Depth, Quad Bounds, FVector MinimumQuadSize, FRandomStream Stream)
{
	m_quad = Bounds;
	m_minimumQuadSize = MinimumQuadSize;
	m_stream = Stream;
	m_room = Quad();

	bool childrenMade = false;
	
	for (int i = 0; i < 4; i++)
		m_children[i] = nullptr;

	// If the quad is 0 sized don't do anything.
	if (m_quad.GetBounds().X > 0 && m_quad.GetBounds().Y > 0)
	{
		// Attempt to make a quad tree with the given depth, if it cant be done try a smaller depth until
		//it can be done.
		while (childrenMade != true)
		{
			if (Depth > 0)
				childrenMade = CreateChildren(Depth);
			else
				break;
			Depth--;
		}
	}
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
	m_minimumQuadSize = Source.m_minimumQuadSize;
	m_room = Source.m_room;

	for (int i = 0; i < 4; i++)
	{
		m_children[i] = nullptr;

		if (Source.m_children[i] != nullptr)
			m_children[i] = new QuadTreeNode(*(Source.m_children[i]));
	}
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
		m_minimumQuadSize = Source.m_minimumQuadSize;
		m_room = Source.m_room;

		for (int i = 0; i < 4; i++)
		{
			if (m_children[i] != nullptr)
				delete m_children[i];

			m_children[i] = nullptr;

			if (Source.m_children[i] != nullptr)
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

		m_children[i] = nullptr;
	}
}
/**********************************************************************************************************
*	bool CreateChildren(int Depth)
*		Purpose:	Causes this node's children to generate. The children will have quads that sum to the
*					area of the parent but are not equal slices. If a depth of greater than 1 is,
*					specified, this function will recurse dividing each child node as well. If the node
*					does not have enough space to support the number divisions requested, false will be
*					returned and no divisions will be made.
*
*		Parameters: 
*			int Depth
*				The number of times to cut the children. If 1 4 children will be created, if 2,
*				Those children will be sliced creating 16 leaf nodes and so on.
*
*		Changes:	m_children can be populated.
*
*		Return - Returns if the children were created. If not enough space is given the children might not
*				 be created.
**********************************************************************************************************/
bool QuadTreeNode::CreateChildren(int Depth)
{
	// Don't create anymore children if 4 children won't fit in any scenario.
	FVector QuadSize = m_quad.GetBounds() - m_quad.GetPosition();

	bool willFit = true;

	willFit = willFit && QuadSize.X >= m_minimumQuadSize.X * pow(2, Depth);
	willFit = willFit && QuadSize.Y >= m_minimumQuadSize.Y * pow(2, Depth);

	if (willFit)
	{
		QuadSlices childQuads = Slice(Depth);

		for (int i = 0; i < 4; i++)
			if (m_children[i] != nullptr)
				delete m_children[i];

		m_children[0] = new QuadTreeNode(Depth - 1, childQuads.southWest, m_minimumQuadSize, m_stream);
		m_children[1] = new QuadTreeNode(Depth - 1, childQuads.northWest, m_minimumQuadSize, m_stream);
		m_children[2] = new QuadTreeNode(Depth - 1, childQuads.northEast, m_minimumQuadSize, m_stream);
		m_children[3] = new QuadTreeNode(Depth - 1, childQuads.southEast, m_minimumQuadSize, m_stream);
	}

	return willFit;
}
/**********************************************************************************************************
*	QuadSlices Slice()
*		Purpose:	Slices this quad into 4 smaller quads. these quads are not of even size but the sum
*					of their area will add up to the area of this quad. The 4 quads will be returned as
*					a QuadSlices struct which just contains 4 quads.
*
*		Parameters:
*			int PlannedDivisions
*				The number times this quad will be sliced up. This parameter changes the minimum size of
*				this quad to support at least 2^X minimuimRoomSized Quads. If the number of divisions
*				results in more rooms than can ever fit in the quad, empty quads are returned (size of 0).
*
*		Return - Returns a QuadSlices struct containing 4 quads. These quads will sum to the original
*					quad sliced into 4ths.
**********************************************************************************************************/
QuadSlices QuadTreeNode::Slice(int PlannedDivisions)
{
	QuadSlices slicedUp;

	FVector quadBounds = m_quad.GetBounds();
	FVector quadOffset = m_quad.GetPosition();

	// define the minimum and maximum slice locations.
	// 1 is subtracted because this is a division.
	int minX = m_minimumQuadSize.X * pow(2, PlannedDivisions - 1);
	int minY = m_minimumQuadSize.Y * pow(2, PlannedDivisions - 1);

	int maxX = quadBounds.X - m_minimumQuadSize.X * pow(2, PlannedDivisions - 1);
	int maxY = quadBounds.Y - m_minimumQuadSize.Y * pow(2, PlannedDivisions - 1);

	if (maxX >= minX && maxY >= minY)
	{
		int xSlice = m_stream.RandRange(minX, maxX);
		int ySlice = m_stream.RandRange(minY, maxY);

		// Wait until the rooms are somewhat similar sized.
		while ((float)xSlice / (float)ySlice > 2 || (float)xSlice / (float)ySlice < .5)
		{
			xSlice = m_stream.RandRange(minX, maxX);
			ySlice = m_stream.RandRange(minY, maxY);
		}

		// Bottom left quad.
		slicedUp.southWest.SetPosition(quadOffset);
		slicedUp.southWest.SetBounds(FVector(xSlice, ySlice, 0));

		// Bottom right quad.
		slicedUp.southEast.SetPosition(FVector(xSlice, quadOffset.Y, 0));
		slicedUp.southEast.SetBounds(FVector(quadBounds.X, ySlice, 0));

		// Top left quad.
		slicedUp.northWest.SetPosition(FVector(quadOffset.X, ySlice, 0));
		slicedUp.northWest.SetBounds(FVector(xSlice, quadBounds.Y, 0));

		// Top right quad.
		slicedUp.northEast.SetPosition(FVector(xSlice, ySlice, 0));
		slicedUp.northEast.SetBounds(FVector(quadBounds.X, quadBounds.Y, 0));
	}

	return slicedUp;
}
/**********************************************************************************************************
*	QuadTreeNode ** GetChildren()
*		Purpose:	Getter.
**********************************************************************************************************/
QuadTreeNode ** QuadTreeNode::GetChildren()
{
	//TODO: This should be const like all other getters.
	return m_children;
}
/**********************************************************************************************************
*	Quad GetQuad()
*		Purpose:	Getter.
**********************************************************************************************************/
Quad QuadTreeNode::GetQuad() const
{
	return m_quad;
}
/**********************************************************************************************************
*	Quad GetRoom()
*		Purpose:	Getter.
**********************************************************************************************************/
Quad QuadTreeNode::GetRoom() const
{
	return m_room;
}
/**********************************************************************************************************
*	void SetRoom(Quad Room)
*		Purpose:	Setter.
**********************************************************************************************************/
void QuadTreeNode::SetRoom(Quad Room)
{
	m_room = Room;
}
/**********************************************************************************************************
*		FVector GetMinimumQuadSize()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector QuadTreeNode::GetMinimumQuadSize() const
{
	return m_minimumQuadSize;
}
/**********************************************************************************************************
*	void SetMinimumQuadSize(FVector MinimumRoomSize)
*		Purpose:	Setter.
**********************************************************************************************************/
void QuadTreeNode::SetMinimumQuadSize(FVector MinimumRoomSize)
{
	m_minimumQuadSize = MinimumRoomSize;
}
/**********************************************************************************************************
*	FRandomStream GetRandomStream()
*		Purpose:	Getter.
**********************************************************************************************************/
FRandomStream QuadTreeNode::GetRandomStream() const
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