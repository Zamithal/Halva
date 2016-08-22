// Fill out your copyright notice in the Description page of Project Settings.

#include "Halva.h"
#include "DungeonLayout.h"

// Define an error log.
//DEFINE_LOG_CATEGORY(DungeonBuilding);

/**********************************************************************************************************
*	DungeonLayout()
*		Purpose:	Default constructor. Results in a dungeon that is 0x0.
**********************************************************************************************************/
DungeonLayout::DungeonLayout()
{
	m_rooms = TArray<Quad>();
	m_paths = TArray<Quad>();
	m_minimumRoomSize = FVector(0, 0, 0);
	m_quadTreeRoot = QuadTreeNode(0, Quad(), m_minimumRoomSize, FRandomStream(0));
	m_pathWidth = 0;
	m_dungeonLayout = nullptr;
	m_randomStream = FRandomStream(0);
}
/**********************************************************************************************************
*	DungeonLayout(FVector DungeonSize, FVector MinimumRoomSize, int PathWidth)
*		Purpose:	Constructs a dungeon layout. The layout is stored in m_dungeonLayout.
*
*		Parameters:
*			FVector DungeonSize
*				The size of the entire dungeon map. A tile will be generated for each space in this map.
*				For example, if a 32x32 size is given, 32*32 tiles will be generated.
*			FVector MinimumRoomSize
*				The smallest possible room that can be created. Rooms can be larger than this.
*			int PathWidth
*				The width of all paths between rooms.
**********************************************************************************************************/
DungeonLayout::DungeonLayout(FVector DungeonSize, FVector MinimumRoomSize, int PathWidth, FRandomStream RNG)
{

}
/**********************************************************************************************************
*	DungeonLayout(const DungeonLayout & Source)
*		Purpose:	Copy constructor. Creates a copy of the layout passed in at Source.
*
*		Parameters:
*			const DungeonLayout & Source
*				The object to copy from.
**********************************************************************************************************/
DungeonLayout::DungeonLayout(const DungeonLayout & Source)
{
	m_rooms = Source.m_rooms;
	m_paths = Source.m_paths;
	m_pathWidth = Source.m_pathWidth;
	m_minimumRoomSize = Source.m_minimumRoomSize;
	m_randomStream = Source.m_randomStream;
	m_quadTreeRoot = Source.m_quadTreeRoot;

	// Gets the dimensions of the level.
	FVector levelDimensions(m_quadTreeRoot.GetQuad().GetBounds());

	// Throw out any decimal the user might have entered.
	int xTileNumber = (int)floor(levelDimensions.X);
	int yTileNumber = (int)floor(levelDimensions.Y);

	m_dungeonLayout = nullptr;

	// make a non-contiguous 2d array thing that is the same size as the old one.
	// create an array of TileData array pointers.
	if (yTileNumber > 0 && xTileNumber > 0)
	{
		m_dungeonLayout = new TileData *[yTileNumber];

		// create a Tile Data array for each element of the TileData pointer array.
		for (int i = 0; i < yTileNumber; i++)
			m_dungeonLayout[i] = new TileData[xTileNumber];

		// copy the values over.
		for (int y = 0; y < yTileNumber; y++)
			for (int x = 0; x < xTileNumber; x++)
				m_dungeonLayout[y][x] = Source.m_dungeonLayout[y][x];
	}
}
/**********************************************************************************************************
*	~DungeonLayout()
*		Purpose:	Destructor. Clears dungeon layout to avoid memory leaks.
**********************************************************************************************************/
DungeonLayout::~DungeonLayout()
{
	if (m_dungeonLayout != nullptr)
	{
		FVector levelDimensions(m_quadTreeRoot.GetQuad().GetBounds());

		// Throw out any decimal the user might have entered.
		int yTileNumber = (int)floor(levelDimensions.Y);

		for (int i = 0; i < yTileNumber; i++)
			delete[] m_dungeonLayout[i];

		delete[] m_dungeonLayout;
		m_dungeonLayout = nullptr;
	}
}
/**********************************************************************************************************
*	DungeonLayout & operator=(const DungeonLayout & Source)
*		Purpose:	Takes Source and copies all its data to the left hand side of the operator.
*
*		Parameters:
*			const DungeonLayout & Source
*				The object to copy from.
**********************************************************************************************************/
DungeonLayout & DungeonLayout::operator=(const DungeonLayout & Source)
{
	if (&Source != this)
	{
		m_rooms = Source.m_rooms;
		m_paths = Source.m_paths;
		m_pathWidth = Source.m_pathWidth;
		m_minimumRoomSize = Source.m_minimumRoomSize;
		m_randomStream = Source.m_randomStream;
		m_quadTreeRoot = Source.m_quadTreeRoot;

		// Gets the dimensions of the level.
		FVector levelDimensions(m_quadTreeRoot.GetQuad().GetBounds());

		// Throw out any decimal the user might have entered.
		int xTileNumber = (int)floor(levelDimensions.X);
		int yTileNumber = (int)floor(levelDimensions.Y);

		if (m_dungeonLayout != nullptr)
		{
			for (int i = 0; i < yTileNumber; i++)
				delete[] m_dungeonLayout[i];

			delete[] m_dungeonLayout;
			m_dungeonLayout = nullptr;
		}

		if (yTileNumber > 0 && xTileNumber > 0)
		{
			m_dungeonLayout = new TileData *[yTileNumber];

			// create a Tile Data array for each element of the TileData pointer array.
			for (int i = 0; i < yTileNumber; i++)
				m_dungeonLayout[i] = new TileData[xTileNumber];

			// copy the values over.
			for (int y = 0; y < yTileNumber; y++)
				for (int x = 0; x < xTileNumber; x++)
					m_dungeonLayout[y][x] = Source.m_dungeonLayout[y][x];
		}


	}

	return *this;
}
/**********************************************************************************************************
*	TileData ** GetDungeonLayout()
*		Purpose:	Getter.
**********************************************************************************************************/
TileData ** DungeonLayout::GetDungeonLayout()
{
	return m_dungeonLayout;
}
/**********************************************************************************************************
*	FVector GetMinimumRoomSize()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector DungeonLayout::GetMinimumRoomSize()
{
	return m_minimumRoomSize;
}
/**********************************************************************************************************
*	void SetMinimumRoomSize(FVector MinimumRoomSize)
*		Purpose:	Setter.
**********************************************************************************************************/
void DungeonLayout::SetMinimumRoomSize(FVector MinimumRoomSize)
{
	m_minimumRoomSize = MinimumRoomSize;
}
/**********************************************************************************************************
*	int GetPathWidth()
*		Purpose:	Getter.
**********************************************************************************************************/
int DungeonLayout::GetPathWidth()
{
	return m_pathWidth;
}
/**********************************************************************************************************
*	void SetPathWidth(int PathWidth)
*		Purpose:	Setter.
**********************************************************************************************************/
void DungeonLayout::SetPathWidth(int PathWidth)
{
	m_pathWidth = PathWidth;
}
/**********************************************************************************************************
*	void GenerateDungeonLayout()
*		Purpose:	Generates a complete dungeon from start to finish. The finished result will be stored
*					in the 2D array m_dungeonLayout. If a layout already exists, it will be replaced with
*					a new one.
*
*		Changes:
*			m_rooms - Will be populated with a room for each quad in m_quadTreeRoot.
*			m_paths - Paths will be generated between rooms and stored here.
*			m_dungeonLayout - A new layout will be generated and stored here.
**********************************************************************************************************/
void DungeonLayout::GenerateDungeonLayout()
{

}
/**********************************************************************************************************
*	void GenerateRooms()
*		Purpose:	Populates m_rooms with a random sized room for each child quad in m_QuadTreeRoot. This
*					room will be no smaller than m_minimumRoomSize including walls. A room will be
*					generated for every quad regardless of if it is used in the final layout or not.
*
*		Changes:
*			m_rooms - Will be populated with a room for each quad in m_quadTreeRoot.
**********************************************************************************************************/
void DungeonLayout::GenerateRooms()
{
	m_rooms.Empty();
	GenerateRoomRecursive(&m_quadTreeRoot);
}

void DungeonLayout::GeneratePaths()
{

}

void DungeonLayout::CreateRoomLayout()
{

}

void DungeonLayout::CreateSpecialTiles()
{

}

void DungeonLayout::CreateWalls()
{

}

void DungeonLayout::CreatePillars()
{

}

void DungeonLayout::CreateOutsideCorners()
{

}

void DungeonLayout::CreateInsideCorners()
{

}
/**********************************************************************************************************
*	void GenerateRoomRecursive(QuadTreeNode * CurrentNode)
*		Purpose:	If this node is a branch, call this function on it's children. If this node is a leaf
*					Generate a random room for it and add it to m_rooms.
*
*		Parameters:
*			QuadTreeNode * CurrentNode
*				The node to get the children of. If this is a child node, instead add a room to m_rooms
*				and set it in this node.
*
*		Changes:
*			m_rooms - A room will be added for each leaf node down the tree connected to the one passed
*					  in.
**********************************************************************************************************/
void DungeonLayout::GenerateRoomRecursive(QuadTreeNode * CurrentNode)
{
	if (CurrentNode != nullptr)
	{
		QuadTreeNode ** children = CurrentNode->GetChildren();

		// should never be nullptr but check anyway.
		if (children != nullptr)
		{
			// If the first child is not set none of them are. That makes this a leaf node.
			if (children[0] != nullptr)
			{

				//TODO: Could cause issues with TArrays, look into how TArrays work in more detail.
				Quad * newRoom = new Quad(GenerateRandomRoom(CurrentNode->GetQuad()));

				m_rooms.Add(*newRoom);

				CurrentNode->SetRoom(newRoom);
			}
			// else call this on the children.
			else
				for (int i = 0; i < 4; i++)
					GenerateRoomRecursive(children[i]);
		}
	}

}
/**********************************************************************************************************
*	Quad GenerateRandomRoom(Quad MaximumBounds)
*		Purpose:	Generates a random quad that fits inside MaximumBounds. This room has to be larger
*					than m_minimumRoomSize.
*
*		Parameters:
*			Quad MaximumBounds
*				The Quad generated will fit inside this, respecting position as well as bounds.
*
*		Return:		Returns a Quad that is at least as big as the minimum and fits inside the given space.
**********************************************************************************************************/
Quad DungeonLayout::GenerateRandomRoom(Quad MaximumBounds)
{
	FVector MaximumSize = MaximumBounds.GetBounds();
	FVector MaximumPosition = MaximumBounds.GetPosition();

	FVector BottomLeftMin;
	FVector BottomLeftMax;
	FVector TopRightMin;
	FVector TopRightMax;

	// The closest position the bottom left corner can be to the bottom left corner of the quad and still
	// generate a room of minimumSize.
	BottomLeftMin = MaximumPosition;

	// The closest position the bottom left corner can be to the top right corner and  still generate a
	// room of at least minimumSize.
	BottomLeftMax = MaximumSize + MaximumPosition - m_minimumRoomSize;

	// The closest position the top right corner can be to the bottom left of the quad and still
	// generate a room of minimumSize.
	TopRightMin = MaximumPosition + m_minimumRoomSize;

	// The closest position the top right corner can be to the top right corner of the quad and still
	// generate a room of minimumSize.
	TopRightMin = MaximumSize + MaximumPosition;

	FVector newRoomPosition;
	FVector newRoomBounds;

	newRoomPosition.X = m_randomStream.RandRange((int)BottomLeftMin.X, (int)BottomLeftMax.X);
	newRoomPosition.Y = m_randomStream.RandRange((int)BottomLeftMin.Y, (int)BottomLeftMax.Y);

	newRoomBounds.X = m_randomStream.RandRange((int)TopRightMin.X, (int)TopRightMax.X);
	newRoomBounds.Y = m_randomStream.RandRange((int)TopRightMin.Y, (int)TopRightMax.Y);

	return Quad(newRoomBounds, newRoomPosition);
}
/**********************************************************************************************************
*	void GeneratePathBetweenQuads(Quad Room1, Quad Room2)
*		Purpose:	Generates a path connecting Room1 to Room2. If a quad can be created directly between
*					the two rooms of width m_PathWidth, a straight away will be built. If the quad does
*					not fit, 2 quads will instead form an L Bend. The paths generated can overlap with
*					existing rooms or paths and only acknowledge the rooms passed in. If either of the
*					rooms passed in do not exist, no action is taken.
*
*		Parameters:
*			Quad Room1
*				One end of the path to build. The path built will connect to this quad.
*			Quad Room2
*				The other end of the path. The path built will connect to this quad.
*
*		Changes:
*			m_paths - Paths generated are stored here.
*
*		Return: Returns if a path was built or not.
**********************************************************************************************************/
bool DungeonLayout::GeneratePathBetweenQuads(Quad Room1, Quad Room2)
{
	bool PathGenerated = false;
	// Attempt to generate a straight path Y aligned.
	PathGenerated = GenerateYAlignedPath(Room1, Room2);

	// Attempt to generate a straight path X aligned.
	if (PathGenerated == false)
		PathGenerated = GenerateXAlignedPath(Room1, Room2);

	// Attempt to generate an L Bend.
	if (PathGenerated == false)
		PathGenerated = GenerateLBendPath(Room1, Room2);

	//TODO: What happens if all these fail? A C bend? give up?

	// Return if a path was made.
	return PathGenerated;
}
/**********************************************************************************************************
*	bool GenerateYAlignedPath(Quad Room1, Quad Room2)
*		Purpose:	Attempts to create a path between the rooms passed in. The path generated will be
*					m_pathWidth wide and will only generate if a straight path can be drawn from Room1 to
*					Room2 along the Y axis. See GenerateXAlignedPath() for this functions counterpart.
*
*		Parameters:
*			Quad Room1
*				One end of the path to build. The path built will connect to this quad.
*			Quad Room2
*				The other end of the path. The path built will connect to this quad.
*
*		Changes:
*			m_paths - Paths generated are stored here.
*
*		Return: Returns if a path was built or not.
**********************************************************************************************************/
bool DungeonLayout::GenerateYAlignedPath(Quad Room1, Quad Room2)
{
	//Find the room with the smaller width (X value).
	Quad * smallerRoom;
	Quad * largerRoom;

	int Room1Width = Room1.GetBounds().X - Room1.GetPosition().X;
	int Room2Width = Room2.GetBounds().X - Room2.GetPosition().X;

	if (Room1Width > Room2Width)
	{
		largerRoom = &Room1;
		smallerRoom = &Room2;
	}
	// The case where they are equal is fine to ignore.
	else
	{
		smallerRoom = &Room1;
		largerRoom = &Room2;
	}

	// 4 Scenarios: 
	//		1. The smaller room's edge is completely contained in the larger room's edge
	//		2. The smaller room's edge's maxima is contained by the larger room, but not the minima.
	//		3. The smaller room's edge's minima is contained by the larger room, but not the maxima.
	//		4. The smaller room's edge is not contained by the larger room at all.

	bool minimaContained = (largerRoom->GetBounds().X >= smallerRoom->GetPosition().X
							&&
							largerRoom->GetPosition().X <= smallerRoom->GetPosition().X);

	bool maximaContained = (largerRoom->GetBounds().X >= smallerRoom->GetBounds().X
							&&
							largerRoom->GetPosition().X <= smallerRoom->GetBounds().X);

	int rangeMinima = 0;
	int rangeMaxima = 0;

	// Scenario 1.
	if (minimaContained && maximaContained)
	{
		// The range of path buildable is smallerRoom minima to smallerRoom maxima.
		rangeMinima = smallerRoom->GetPosition().X;
		rangeMaxima = smallerRoom->GetBounds().X;
	}
	// Scenario 2.
	else if (maximaContained)
	{
		// The range of path buildable is largerRoom minima to smallerRoom maxima.
		rangeMinima = largerRoom->GetPosition().X;
		rangeMaxima = smallerRoom->GetBounds().X;
	}
	// Scenario 3.
	else if (minimaContained)
	{
		// The range of path buildable is smallerRoom minima to largerRoom maxima.
		rangeMinima = smallerRoom->GetPosition().X;
		rangeMaxima = largerRoom->GetBounds().X;
	}
	// Scenario 4.
	else
	{
		// There is no way to solve this path.
		return false;
	}

	// If the range found is not m_PathWidth wide, a path cannot be generated.
	if (rangeMaxima - rangeMinima < m_pathWidth)
		return false;

	// find the lower room in terms of Y.
	Quad * lowerRoom;
	Quad * higherRoom;

	if (Room1.GetPosition().Y > Room2.GetBounds().Y)
	{
		lowerRoom = &Room2;
		higherRoom = &Room1;
	}
	else
	{
		lowerRoom = &Room1;
		higherRoom = &Room2;
	}

	int BuildXMinima = m_randomStream.RandRange(rangeMinima, rangeMaxima - m_pathWidth);

	// Generate the path.
	m_paths.Add(
		Quad(FVector(BuildXMinima + m_pathWidth, higherRoom->GetPosition().Y, 0), 
			 FVector(BuildXMinima, lowerRoom->GetBounds().Y, 0)));

	// Path created successfully.
	return true;
}
/**********************************************************************************************************
*	bool GenerateXAlignedPath(Quad Room1, Quad Room2)
*		Purpose:	Attempts to create a path between the rooms passed in. The path generated will be
*					m_pathWidth wide and will only generate if a straight path can be drawn from Room1 to
*					Room2 along the X axis. See GenerateYAlignedPath() for this functions counterpart.
*
*		Parameters:
*			Quad Room1
*				One end of the path to build. The path built will connect to this quad.
*			Quad Room2
*				The other end of the path. The path built will connect to this quad.
*
*		Changes:
*			m_paths - Paths generated are stored here.
*
*		Return: Returns if a path was built or not.
**********************************************************************************************************/
bool DungeonLayout::GenerateXAlignedPath(Quad Room1, Quad Room2)
{
	//Find the room with the smaller height (Y value).
	Quad * smallerRoom;
	Quad * largerRoom;

	int Room1Height = Room1.GetBounds().Y - Room1.GetPosition().Y;
	int Room2Height = Room2.GetBounds().Y - Room2.GetPosition().Y;

	if (Room1Height > Room2Height)
	{
		largerRoom = &Room1;
		smallerRoom = &Room2;
	}
	// The case where they are equal is fine to ignore.
	else
	{
		smallerRoom = &Room1;
		largerRoom = &Room2;
	}

	// 4 Scenarios: 
	//		1. The smaller room's edge is completely contained in the larger room's edge
	//		2. The smaller room's edge's maxima is contained by the larger room, but not the minima.
	//		3. The smaller room's edge's minima is contained by the larger room, but not the maxima.
	//		4. The smaller room's edge is not contained by the larger room at all.

	bool minimaContained = (largerRoom->GetBounds().Y >= smallerRoom->GetPosition().Y
		&&
		largerRoom->GetPosition().Y <= smallerRoom->GetPosition().Y);

	bool maximaContained = (largerRoom->GetBounds().Y >= smallerRoom->GetBounds().Y
		&&
		largerRoom->GetPosition().Y <= smallerRoom->GetBounds().Y);

	int rangeMinima = 0;
	int rangeMaxima = 0;

	// Scenario 1.
	if (minimaContained && maximaContained)
	{
		// The range of path buildable is smallerRoom minima to smallerRoom maxima.
		rangeMinima = smallerRoom->GetPosition().Y;
		rangeMaxima = smallerRoom->GetBounds().Y;
	}
	// Scenario 2.
	else if (maximaContained)
	{
		// The range of path buildable is largerRoom minima to smallerRoom maxima.
		rangeMinima = largerRoom->GetPosition().Y;
		rangeMaxima = smallerRoom->GetBounds().Y;
	}
	// Scenario 3.
	else if (minimaContained)
	{
		// The range of path buildable is smallerRoom minima to largerRoom maxima.
		rangeMinima = smallerRoom->GetPosition().Y;
		rangeMaxima = largerRoom->GetBounds().Y;
	}
	// Scenario 4.
	else
	{
		// There is no way to solve this path.
		return false;
	}

	// If the range found is not m_PathWidth wide, a path cannot be generated.
	if (rangeMaxima - rangeMinima < m_pathWidth)
		return false;

	// find the left room in terms of X.
	Quad * leftRoom;
	Quad * rightRoom;

	if (Room1.GetPosition().X > Room2.GetBounds().X)
	{
		leftRoom = &Room2;
		rightRoom = &Room1;
	}
	else
	{
		leftRoom = &Room1;
		rightRoom = &Room2;
	}

	int BuildYMinima = m_randomStream.RandRange(rangeMinima, rangeMaxima - m_pathWidth);

	// Generate the path.
	m_paths.Add(
		Quad(FVector(rightRoom->GetPosition().X, BuildYMinima + m_pathWidth, 0), 
			 FVector(leftRoom->GetBounds().X, BuildYMinima, 0)));

	// Path created successfully.
	return true;
}
/**********************************************************************************************************
*	bool GenerateLBendPath(Quad Room1, Quad Room2)
*		Purpose:	Attempts to create an LBend to connect the two rooms.
*
*		Parameters:
*			Quad Room1
*				One end of the path to build. The path built will connect to this quad.
*			Quad Room2
*				The other end of the path. The path built will connect to this quad.
*
*		Changes:
*			m_paths - Paths generated are stored here.
*
*		Return: Returns if a path was built or not.
**********************************************************************************************************/
bool DungeonLayout::GenerateLBendPath(Quad Room1, Quad Room2)
{
	int randomBendDirection = m_randomStream.RandRange(0, 1);
	bool succeeded = false;

	// Try to build a bend one way.
	if (randomBendDirection == 1)
		succeeded = GenerateLBendPathWithKnownOrientation(Room1, Room2, true);
	else
		succeeded = GenerateLBendPathWithKnownOrientation(Room1, Room2, false);

	// If the bend could not be completed try it the other way.
	if (!succeeded)
	{
		randomBendDirection++;
		randomBendDirection %= 2;

		if (randomBendDirection == 1)
			succeeded = GenerateLBendPathWithKnownOrientation(Room1, Room2, true);
		else
			succeeded = GenerateLBendPathWithKnownOrientation(Room1, Room2, false);
	}

	// Return if a bend could be built.
	return succeeded;
}
/**********************************************************************************************************
*	bool GenerateLBendPathWithKnownOrientation(Quad Room1, Quad Room2, bool xFirst)
*		Purpose:	Attempts to create an LBend to connect the two rooms. The lBend will only be attempted
*					a single way. If xFirst is true, Room1 will have its path on the xAxis while Room2
*					has its path on the yAxis. If false the inverse is true.
*
*		Parameters:
*			Quad Room1
*				One end of the path to build. The path built will connect to this quad.
*			Quad Room2
*				The other end of the path. The path built will connect to this quad.
*			bool xFirst
*				If Room1 is the xAligned room.
*
*		Changes:
*			m_paths - Paths generated are stored here.
*
*		Return: Returns if a path was built or not.
**********************************************************************************************************/
bool DungeonLayout::GenerateLBendPathWithKnownOrientation(Quad Room1, Quad Room2, bool xFirst)
{
	// Designate which room is xAligned and which is yAligned.
	Quad * xAligned = nullptr;
	Quad * yAligned = nullptr;

	if (xFirst)
	{
		xAligned = &Room1;
		yAligned = &Room2;
	}
	else
	{
		xAligned = &Room2;
		yAligned = &Room1;
	}

	// The range of tiles that is outside the xAligned room but on the width of the yAligned room needs
	// to be found.

	// Find the valid building range on the yAlignedRoom's width (xAxis).
	int yAlignedRoomBuildableRangeMinima = 0;
	int yAlignedRoomBuildableRangeMaxima = 0;

	bool yAlignedMinimaContained =	yAligned->GetPosition().X >= xAligned->GetPosition().X
									&&
									yAligned->GetPosition().X <= xAligned->GetBounds().X;

	bool yAlignedMaximaContained =	yAligned->GetBounds().X >= xAligned->GetPosition().X
									&&
									yAligned->GetBounds().X <= xAligned->GetBounds().X;

	

	// 4 Scenarios that determine where the path should be built.
	// 1. The entire width of the yAligned room is contained. No L Bend can be made.
	// 2. The minima of the yAligned room is contained.
	// 3. The maxima of the yAligned room is contained.
	// 4. None of the yAligned room is.

	if (yAlignedMinimaContained && yAlignedMaximaContained)
	{
		//L bend impossible.
		return false;
	}
	else if (yAlignedMinimaContained)
	{
		// The range is xAligned maxima to yAligned maxima.
		yAlignedRoomBuildableRangeMinima = xAligned->GetBounds().X;
		yAlignedRoomBuildableRangeMaxima = yAligned->GetBounds().X;
	}
	else if (yAlignedMaximaContained)
	{
		// The range is yAligned minima to xAligned minima.
		yAlignedRoomBuildableRangeMinima = yAligned->GetPosition().X;
		yAlignedRoomBuildableRangeMaxima = xAligned->GetPosition().X;
	}
	else
	{
		// The range is all of the yAligned room's width.
		yAlignedRoomBuildableRangeMinima = yAligned->GetPosition().X;
		yAlignedRoomBuildableRangeMaxima = yAligned->GetBounds().X;
	}


	// The same process needs to be repeated for the width of the xAligned Room.



	// The range of tiles that are outside the yAligned room but contained in the width
	// of the xAligned room needs to be found.

	// Find the valid building range on the xAlignedRoom's width (yAxis).
	int xAlignedRoomBuildableRangeMinima = 0;
	int xAlignedRoomBuildableRangeMaxima = 0;

	bool xAlignedMinimaContained =	xAligned->GetPosition().Y >= yAligned->GetPosition().Y
									&&
									xAligned->GetPosition().Y <= yAligned->GetBounds().Y;

	bool xAlignedMaximaContained =	xAligned->GetBounds().Y >= yAligned->GetPosition().Y
									&&
									xAligned->GetBounds().Y <= yAligned->GetBounds().Y;



	// 4 Scenarios that determine where the path should be built.
	// 1. The entire width of the xAligned room is contained. No L Bend can be made.
	// 2. The minima of the xAligned room is contained.
	// 3. The maxima of the xAligned room is contained.
	// 4. None of the xAligned room is.

	if (xAlignedMinimaContained && xAlignedMaximaContained)
	{
		//L bend impossible.
		return false;
	}
	else if (xAlignedMinimaContained)
	{
		// The range is yAligned maxima to xAligned maxima.
		xAlignedRoomBuildableRangeMinima = yAligned->GetBounds().Y;
		xAlignedRoomBuildableRangeMaxima = xAligned->GetBounds().Y;
	}
	else if (xAlignedMaximaContained)
	{
		//The range is xAligned minima to yAligned minima.
		xAlignedRoomBuildableRangeMinima = xAligned->GetPosition().Y;
		xAlignedRoomBuildableRangeMaxima = yAligned->GetPosition().Y;
	}
	else
	{
		//The range is the width of xAligned.
		xAlignedRoomBuildableRangeMinima = xAligned->GetPosition().Y;
		xAlignedRoomBuildableRangeMaxima = xAligned->GetBounds().Y;
	}


	// Check to make sure there is enough width for a path.
	bool xAlignedRoomWidthOK = 
		xAlignedRoomBuildableRangeMaxima - xAlignedRoomBuildableRangeMinima >= m_pathWidth;

	bool yAlignedRoomWidthOK =
		yAlignedRoomBuildableRangeMaxima - yAlignedRoomBuildableRangeMinima >= m_pathWidth;

	if (!xAlignedRoomWidthOK || !yAlignedRoomWidthOK)
	{
		// Not enough room for an L bend.
		return false;
	}

	// Randomly pick a location for the intersection.

	// This is the y axis.
	int xAlignedBuildLocation = m_randomStream.RandRange(
									xAlignedRoomBuildableRangeMinima,
									xAlignedRoomBuildableRangeMaxima - m_pathWidth);

	// This is the x axis.
	int yAlignedBuildLocation = m_randomStream.RandRange(
									yAlignedRoomBuildableRangeMinima,
									yAlignedRoomBuildableRangeMaxima - m_pathWidth);

	// Build the intersection.
	Quad intersection = 
		Quad(
				FVector(yAlignedBuildLocation + m_pathWidth, xAlignedBuildLocation + m_pathWidth, 0),
				FVector(yAlignedBuildLocation, xAlignedBuildLocation, 0));

	// build paths to the intersection.

	bool XGenerated = GenerateXAlignedPath(*xAligned, intersection);
	bool YGenerated = GenerateYAlignedPath(*yAligned, intersection);

	// If either of those failed, garbage could have been created in the m_paths array. Warn about this.
	if (XGenerated || YGenerated)
	{
		UE_LOG(LogTemp, Warning, TEXT("Critical Error building path, path array may be corrupted."));
		return false;
	}
	
	// Add the intersection to paths.
	m_paths.Add(intersection);

	return true;
}
/**********************************************************************************************************
*	void GeneratePathsRecursive(QuadTreeNode * CurrentNode)
*		Purpose:	Connects the set of rooms belonging to each child passed in current node with a path.
*					The path can either be direct or indirect but each set will be reachable through
*					one of the siblings. This process is repeated down the tree connecting all sets of
*					sibling nodes.
*		Parameters:
*			QuadTreeNode * CurrentNode
*				The highest level of the tree to connect siblings at.
*
*		Changes:
*			m_paths - New paths will be added to the array.
**********************************************************************************************************/
void DungeonLayout::GeneratePathsRecursive(QuadTreeNode * CurrentNode)
{
	if (CurrentNode != nullptr)
	{
		QuadTreeNode ** children = CurrentNode->GetChildren();
		if (children != nullptr)
		{
			// If this node has children.
			if (children[0] != nullptr)
			{
				// Pick a random number to not generate a path for.
				int DontBuildPathIndex = m_randomStream.FRandRange(0, 4);

				// Pick a random point on the intersect between each sibling.

				// ORDER:
				// 0 = Bottom Left
				// 1 = Top Left
				// 2 = Top Right
				// 3 = Bottom Right

				Quad currentChildQuad = Quad();
				FVector randomPointBetweenQuads = FVector();
				int randomMin = 0;
				int randomMax = 0;

				// Connect children
				for (int i = 0; i < 4; i++)
				{
					if (i != DontBuildPathIndex)
					{
						currentChildQuad = children[i]->GetQuad();

						// Switch will result in getting a random point.
						switch (i)
						{
							// 0 = Bottom Left
						case 0:
							randomMin = currentChildQuad.GetPosition().X;
							randomMax = currentChildQuad.GetBounds().X;
							randomPointBetweenQuads.Y = currentChildQuad.GetBounds().Y;
							randomPointBetweenQuads.X = m_randomStream.RandRange(randomMin, randomMax);
							break;
							// 1 = Top Left
						case 1:
							randomMin = currentChildQuad.GetPosition().Y;
							randomMax = currentChildQuad.GetBounds().Y;
							randomPointBetweenQuads.X = currentChildQuad.GetBounds().X;
							randomPointBetweenQuads.Y = m_randomStream.RandRange(randomMin, randomMax);
							break;
							// 2 = Top Right
						case 2:
							randomMin = currentChildQuad.GetPosition().X;
							randomMax = currentChildQuad.GetBounds().X;
							randomPointBetweenQuads.Y = currentChildQuad.GetPosition().Y;
							randomPointBetweenQuads.X = m_randomStream.RandRange(randomMin, randomMax);
							break;
							// 3 = Bottom Right
						case 3:
							randomMin = currentChildQuad.GetPosition().Y;
							randomMax = currentChildQuad.GetBounds().Y;
							randomPointBetweenQuads.X = currentChildQuad.GetPosition().X;
							randomPointBetweenQuads.Y = m_randomStream.RandRange(randomMin, randomMax);
							break;
						default:
							break;
						}

						// Find the closest room belonging to child i.
						Quad Room1 = *FindClosestRoom(children[i], randomPointBetweenQuads);
						// Find the closest room belonging to child i+1.
						Quad Room2 = *FindClosestRoom(children[(i + 1) % 4], randomPointBetweenQuads);

						GeneratePathBetweenQuads(Room1, Room2);
					}
				}

				for (int i = 0; i < 4; i++)
				{
					// Call this function on each child.
					GeneratePathsRecursive(children[i]);
				}

			}
		}
	}
}
/**********************************************************************************************************
*	Quad * FindClosestRoom(QuadTreeNode * ParentNode, FVector Point)
*		Purpose:	Searches the tree passed in at parent node for the room that is closest to the point
*					passed in. The distance to the point is calculated at the center of each edge. This
*					is to ensure that room size does not affect distance to the point.
*		Parameters:
*			QuadTreeNode * ParentNode
*				The tree to find rooms in.
*			FVector Point
*				The point to find a room near.
*
*		Return: Returns the room that is the closest to the point given.
**********************************************************************************************************/
Quad * DungeonLayout::FindClosestRoom(QuadTreeNode * ParentNode, FVector Point)
{
	Quad * closestQuad = nullptr;

	if (ParentNode != nullptr)
	{
		QuadTreeNode ** children = ParentNode->GetChildren();
		if (children != nullptr)
		{
			// If this node has children.
			if (children[0] != nullptr)
			{
				Quad * newQuad = nullptr;
				float newDistance = 0;
				float oldDistance = 0;

				for (int i = 0; i < 4; i++)
				{
					newQuad = FindClosestRoom(children[i], Point);

					if (newQuad != nullptr)
						newDistance = (FindCenterOfClosestEdge(*newQuad, Point) - Point).Size();

					if (closestQuad != nullptr && newQuad != nullptr)
					{
						oldDistance = (FindCenterOfClosestEdge(*closestQuad, Point) - Point).Size();

						if (newDistance < oldDistance)
							newQuad = closestQuad;
					}
					else if (closestQuad == nullptr)
						closestQuad = newQuad;
				}
			}
			else
				closestQuad = ParentNode->GetRoom();
		}
	}

	return closestQuad;
}
/**********************************************************************************************************
*	FVector FindCenterOfClosestEdge(Quad Room, FVector Point)
*		Purpose:	Finds the center of the edge of the quad closest to the point and returns it.
*		Parameters:
*			Quad Room
*				The quad which needs the edge found for.
*			FVector Point
*				The point to find the edge nearest.
*
*		Return: Returns the center point to the edge closest to the point.
**********************************************************************************************************/
FVector DungeonLayout::FindCenterOfClosestEdge(Quad Room, FVector Point)
{
	// Find the center point
	FVector centerPoint = Room.GetPosition() + Room.GetBounds();
	centerPoint /= 2;

	int yDifference = Point.Y - centerPoint.Y;
	int xDifference = Point.X - centerPoint.X;

	// Left Edge
	if ((abs(yDifference) >= abs(xDifference) && yDifference >= 0))
	{
		centerPoint.X = Room.GetBounds().X;
	}
	// Right Edge
	else if ((abs(yDifference) >= abs(xDifference) && yDifference < 0))
	{
		centerPoint.X = Room.GetPosition().X;
	}
	// Top Edge
	else if ((abs(yDifference) < abs(xDifference) && xDifference >= 0))
	{
		centerPoint.Y = Room.GetBounds().Y;
	}
	// Bottom Edge
	else if ((abs(yDifference) < abs(xDifference) && xDifference < 0))
	{
		centerPoint.Y = Room.GetPosition().Y;
	}

	return centerPoint;
}