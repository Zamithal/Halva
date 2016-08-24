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
	m_dungeonDimensions = FVector(0, 0, 0);
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
	m_dungeonDimensions = Source.m_dungeonDimensions;
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
		m_dungeonDimensions = Source.m_dungeonDimensions;
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
*	FVector GetDungeonDimensions()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector DungeonLayout::GetDungeonDimensions()
{
	return m_dungeonDimensions;
}
/**********************************************************************************************************
*	void SetDungeonDimensions(FVector DungeonDimensions)
*		Purpose:	Setter.
*
*		Changes: m_dungeonLayout - The 2D array is resized to match the new dungeon dimensions. If data
*								   was held it is lost upon resizing. All values will be set to empty.
**********************************************************************************************************/
void DungeonLayout::SetDungeonDimensions(FVector DungeonDimensions)
{
	// Clear m_dungeonLayout.
	if (m_dungeonLayout != nullptr)
	{
		int rows = m_dungeonDimensions.Y;

		for (int i = 0; i < rows; i++)
		{
			if (m_dungeonLayout[i] != nullptr)
				delete[] m_dungeonLayout[i];
		}

		delete[] m_dungeonLayout;
		m_dungeonLayout = nullptr;
	}

	// Set new size.
	m_dungeonDimensions = DungeonDimensions;

	//Rebuild dungeonLayout.
	m_dungeonLayout = new TileData *[m_dungeonDimensions.Y];

	for (int i = 0; i < m_dungeonDimensions.Y; i++)
	{
		m_dungeonLayout[i] = new TileData[m_dungeonDimensions.X];
	}

	// Initialize
	ClearDungeonLayout();
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
	GenerateRooms();
	DropRooms();
	GeneratePaths();
	CreateRoomLayout();
	//CreateSpecialTiles();
	CreateWalls();
	//CreatePillars();
	CreateOutsideCorners();
	CreateInsideCorners();
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
/**********************************************************************************************************
*	void DropRooms()
*		Purpose:	Picks random rooms and removes them from the list of rooms until m_targetNumberRooms
*					is reached. If m_targetNumberRooms is larger than the number of existing rooms, no
*					action is performed.
*
*		Changes:
*			m_rooms - Will be populated with a room for each quad in m_quadTreeRoot.
**********************************************************************************************************/
void DungeonLayout::DropRooms()
{
	while (m_rooms.Num() > m_targetNumRooms)
		DropRandomRoomRecursive(&m_quadTreeRoot);
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
void DungeonLayout::GeneratePaths()
{
	m_paths.Empty();
	GeneratePathsRecursive(&m_quadTreeRoot);
}
/**********************************************************************************************************
*	void CreateRoomLayout()
*		Purpose:	Passes through each room and path in m_rooms and m_paths and assigns them as a floor
*					in the 2D array m_dungeonLayout. For example, If a room includes the point (33,44)
*					the value m_dungeonLayout[44][33] will be set to floor. All other tiles not in a room
*					or path are set to empty.
*
*		Changes:
*			m_dungeonLayout - The values are changed depending on if a room or path overlaps with the
*				array location.
**********************************************************************************************************/
void DungeonLayout::CreateRoomLayout()
{
	ClearDungeonLayout();

	int roomCount = m_rooms.Num();
	
	// Mark all tiles inside rooms as floor tiles.
	for (int i = 0; i < roomCount; i++)
	{
		CreateFloorQuad(m_rooms[i]);
	}

	int pathCount = m_paths.Num();

	for (int i = 0; i < pathCount; i++)
	{
		CreateFloorQuad(m_paths[i]);
	}
}
/**********************************************************************************************************
*	void CreateSpecialTiles
*		Purpose:	Marks existing floor tiles as special tiles. These tiles have their type changed to
*					special tiles.
*
*		Changes:
*			m_dungeonLayout - Floor tiles can be changed into special tiles.
*
*		NOTE: Rules for this function are not yet clear, this function may be removed in the future.
**********************************************************************************************************/
void DungeonLayout::CreateSpecialTiles()
{
	//TODO: Define special tiles or remove this function.
}
/**********************************************************************************************************
*	void CreateWalls
*		Purpose:	Scans through the dungeon layout and changes all floor tiles that are adjacent to at
*					least one empty into wall tiles. Rotation is set to face away from the first empty
*					tile found. Additionally, if the floor tile is touching the edge of the level it will
*					also become a wall. Touching corners is not considered touching, these tiles will
*					remain floors.
*
*		Changes:
*			m_dungeonLayout - All floor tiles touching at least one empty are changed to wall tiles. All
*							  floor tiles touching the edge of the level are changed to wall tiles.
**********************************************************************************************************/
void DungeonLayout::CreateWalls()
{
	if (m_dungeonLayout != nullptr)
	{
		for (int y = 0; y < m_dungeonDimensions.Y; y++)
		{
			for (int x = 0; x < m_dungeonDimensions.X; x++)
			{
				// If the tile is a floor tile.
				if (m_dungeonLayout[y][x].tileType == floorTile)
				{
					SolveWallTile( x, y );
				}
			}
		}
	}
}
/**********************************************************************************************************
*	void CreatePillars
*		Purpose:	Finds all walls within the dungeon layout and checks to see if they should be pillars.
*					Pillars spawn when a single wall is completely surrounded by floor tiles. All tiles
*					meeting this requirement will be replaced with pillars.
*
*		Changes:
*			m_dungeonLayout - All wall tiles that are touching only floor tiles will be replaced with
*							  pillars.
*
*		NOTE: The current implementation does not allow for the spawning of pillars. Because of this, this
*			  step is skipped entirely.
**********************************************************************************************************/
void DungeonLayout::CreatePillars()
{
	//TODO: Either change implementation so that pillars can spawn or delete this function.
}
/**********************************************************************************************************
*	void CreateOutsideCorners
*		Purpose:	Examines floors within the dungeon layout and checks to see if they have exactly 2
*					floor tiles touching. If it does, this tile can be replaced with an outside corner.
*
*		Changes:
*			m_dungeonLayout - All floors that are touching exactly 2 floor tiles and exactly 2 wall tiles
*							  will be replaced with an outside corner tile.
**********************************************************************************************************/
void DungeonLayout::CreateOutsideCorners()
{
	if (m_dungeonLayout != nullptr)
	{
		for (int y = 0; y < m_dungeonDimensions.Y; y++)
		{
			for (int x = 0; x < m_dungeonDimensions.X; x++)
			{
				// If the tile is a floor tile.
				if (m_dungeonLayout[y][x].tileType == floorTile)
				{
					SolveOutsideCornerTile(x, y);
				}
			}
		}
	}
}
/**********************************************************************************************************
*	void CreateInsideCorners
*		Purpose:	Replaces all walls that are touching exactly 2 other walls and 2 empties. The tiles
*					are replaced with inside corners and their rotators are set accordingly.
*
*		Changes:
*			m_dungeonLayout - All walls touching 2 empties and 2 other walls are replaced with inside
*							  corner tiles.
**********************************************************************************************************/
void DungeonLayout::CreateInsideCorners()
{
	if (m_dungeonLayout != nullptr)
	{
		for (int y = 0; y < m_dungeonDimensions.Y; y++)
		{
			for (int x = 0; x < m_dungeonDimensions.X; x++)
			{
				// If the tile is a floor tile.
				if (m_dungeonLayout[y][x].tileType == wallTile)
				{
					SolveInsideCornerTile(x, y);
				}
			}
		}
	}
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
				// Add the room to the list of rooms.
				int tArrayIndex = m_rooms.Add(GenerateRandomRoom(CurrentNode->GetQuad()));

				// Set this node's room to point at the newly created room.
				CurrentNode->SetRoom(&m_rooms[tArrayIndex]);
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
*	bool DropRandomRoomRecursive(QuadTreeNode * CurrentNode)
*		Purpose:	Traverses the tree from CurrentNode down looking for leaf nodes. When a leaf node is
*					found, Its room will attempt to be deleted. If this fails (the room doesn't exist),
*					another sibling leaf node will be attempted, If failure continues, the function will
*					proceed up the tree trying all leaf nodes in a random order until CurrentNode is hit
*					again. At this point, this function will return false.
*
*		Parameters:
*			QuadTreeNode * CurrentNode
*					The highest node in the tree to remove rooms from.
*		
*		Changes:
*			m_rooms - A random room will be removed from m_rooms unless empty.
*
*		Return:		Returns true if a leaf node belonging to this tree had a room removed. false if not.
**********************************************************************************************************/
bool DungeonLayout::DropRandomRoomRecursive(QuadTreeNode * CurrentNode)
{
	if (CurrentNode != nullptr)
	{
		QuadTreeNode ** children = CurrentNode->GetChildren();

		// Short circuit evaluation of "If the current node has children."
		if (children != nullptr && children[0] != nullptr)
		{
			//Pick a random
		}
		// This is a leaf node.
		else
		{
		}
	}

	return false;
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
	// Don't waist time if the room is 0 sized.
	FVector Room1Size = Room1.GetBounds() - Room1.GetPosition();
	FVector Room2Size = Room2.GetBounds() - Room2.GetPosition();

	// If any dimensions are 0 the room cannot be solved.
	if (Room1Size.X * Room1Size.Y * Room2Size.X * Room2Size.Y == 0)
		return false;

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
*
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

				// Go through each of the 4 children finding a random point along
				// the edge that can be connected to each other child.
				// EG: Since child 0 is the bottom left, find a random point along
				// its top edge to connect to child 1 (top left quad).
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
*
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
*
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
/**********************************************************************************************************
*	void ClearDungeonLayout()
*		Purpose:	Replaces all values in the dungeon layout with empty.
*
*		Changes:
*			m_dungeonLayout - All values will be empty.
**********************************************************************************************************/
void DungeonLayout::ClearDungeonLayout()
{
	TileData blankTile = TileData();

	blankTile.tileType = emptyTile;
	blankTile.tileRotation = FRotator(0, 0, 0);

	for (int y = 0; y < m_dungeonDimensions.Y; y++)
	{
		for (int x = 0; x < m_dungeonDimensions.X; x++)
		{
			m_dungeonLayout[y][x] = blankTile;
		}
	}
}
/**********************************************************************************************************
*	void CreateFloorQuad(Quad Room)
*		Purpose:	Changes all tiles inside the given quad to floor tiles in the dungeon layout.
*
*		Parameters:
*			Quad Room
*				The Quad that will be changed to floor tiles.
*
*		Changes:
*			m_dungeonLayout - All array elements falling within the bounds of Room will be changed to 
*							  floor tiles.
**********************************************************************************************************/
void DungeonLayout::CreateFloorQuad(Quad Room)
{
	// If the quad passed in has bad bounds resize it to fit inside the level.
	FVector bottomLeftCorner = Room.GetPosition();
	FVector topRightCorner = Room.GetBounds();

	if (bottomLeftCorner.X < 0)
		bottomLeftCorner.X = 0;
	if (bottomLeftCorner.Y < 0)
		bottomLeftCorner.Y = 0;
	if (topRightCorner.X > m_dungeonDimensions.X)
		topRightCorner.X = m_dungeonDimensions.X;
	if (topRightCorner.Y > m_dungeonDimensions.Y)
		topRightCorner.Y = m_dungeonDimensions.Y;

	// Create a floor tile.
	TileData newFloor = TileData();
	newFloor.tileType = floorTile;
	newFloor.tileRotation = FRotator(0, 0, 0);
	int randomRotation = 0;

	if (m_dungeonLayout != nullptr)
	{
		for (int y = bottomLeftCorner.Y; y < topRightCorner.Y; y++)
		{
			for (int x = bottomLeftCorner.X; x < topRightCorner.X; x++)
			{
				// Pick a random rotation for the floor tile.
				randomRotation = m_randomStream.RandRange(0, 3);
				randomRotation *= 90;
				newFloor.tileRotation.Yaw = randomRotation;

				// Assign the tile.
				m_dungeonLayout[y][x] = newFloor;
			}
		}
	}
}
/**********************************************************************************************************
*	bool SolveWallTile(int XPositon, int YPosition)
*		Purpose:	Checks to see if the tile at the given position is a floor tile and is contacting at
*					least one empty tile or the map edge. If these are true, The tile is changed to a wall
*					tile and a rotation is assigned.
*
*		Parameters:
*			int XPosition
*				The x position in the dungeon layout of the tile to solve for.
*			int YPosition
*				The y position in the dungeon layout of the tile to solve for.
*
*		Changes:
*			m_dungeonLayout - The tile at (x, y) could be changed to a wall.
*
*		Return: Returns if the tile was changed to a wall.
**********************************************************************************************************/
bool DungeonLayout::SolveWallTile(int XPosition, int YPosition)
{
	if (m_dungeonLayout == nullptr)
		return false;

	// Make sure the tile is a valid location in the map.
	bool inBounds = true;
	inBounds = inBounds && XPosition >= 0;
	inBounds = inBounds && YPosition >= 0;
	inBounds = inBounds && XPosition <= m_dungeonDimensions.X;
	inBounds = inBounds && YPosition <= m_dungeonDimensions.Y;

	if (!inBounds)
		return false;

	// Make sure the tile is a floor tile.
	if (m_dungeonLayout[YPosition][XPosition].tileType != floorTile)
		return false;
	
	TileData newWall = TileData();

	newWall.tileType = wallTile;

	// Get surrounding tile data.
	for (int y = -1; y < 1; y++)
	{
		for (int x = -1; x < 1; x++)
		{
			// XOR - get tiles adjacent to the center.
			if ((x != 0) != (y != 0))
			{
				inBounds = true;
				inBounds = inBounds && x + XPosition >= 0;
				inBounds = inBounds && y + YPosition >= 0;
				inBounds = inBounds && x + XPosition <= m_dungeonDimensions.X;
				inBounds = inBounds && y + YPosition <= m_dungeonDimensions.Y;

				// Make sure we don't check for a value outside the bounds of the array.
				if (inBounds)
				{
					TileData adjacentTile = m_dungeonLayout[y + YPosition][x + XPosition];

					if (adjacentTile.tileType == emptyTile)
					{
						// Set the tile rotation to face away from the empty tile.
						if (x == 0)
							newWall.tileRotation = FRotator(0, 0, (y + 1) * 90 + 90);
						else
							newWall.tileRotation = FRotator(0, 0, (x + 1) * 90);

						// Set the tile in the dungeon layout.
						m_dungeonLayout[YPosition][XPosition] = newWall;
						return true;
					}
				}
				else
				{
					// Set the tile rotation to face away from the empty tile.
					if (x == 0)
						newWall.tileRotation = FRotator(0, 0, (y + 1) * 90 + 90);
					else
						newWall.tileRotation = FRotator(0, 0, (x + 1) * 90);

					// Set the tile in the dungeon layout.
					m_dungeonLayout[YPosition][XPosition] = newWall;
					return true;
				}
			}
		}
	}

	// Tile could not be built because there are no surrounding empties.
	return false;
}
/**********************************************************************************************************
*	bool SolveOutsideCornerTile(int XPositon, int YPosition)
*		Purpose:	Checks to see if the tile at (X, Y) is a floor tile touching exactly 2 walls and 2
*					floors. If it is, attempt to solve for an corner orientation that will complete the
*					wall. Returns true on successfully building the tile, false on failure.
*
*		Parameters:
*			int XPosition
*				The x position in the dungeon layout of the tile to solve for.
*			int YPosition
*				The y position in the dungeon layout of the tile to solve for.
*
*		Changes:
*			m_dungeonLayout - The tile at (x, y) could be changed to an outside corner.
*
*		Return: Returns if the tile was changed to a corner.
**********************************************************************************************************/
bool DungeonLayout::SolveOutsideCornerTile(int XPosition, int YPosition)
{
	if (m_dungeonLayout == nullptr)
		return false;

	// Make sure the tile is a valid location in the map. An outside corner cannot be on the edge of the
	// map and generate correctly.
	bool inBounds = true;
	inBounds = inBounds && XPosition > 0;
	inBounds = inBounds && YPosition > 0;
	inBounds = inBounds && XPosition < m_dungeonDimensions.X;
	inBounds = inBounds && YPosition < m_dungeonDimensions.Y;

	if (!inBounds)
		return false;

	int floorTileCount = 0;
	int wallTileCount = 0;

	FVector floorTiles[2];

	// Count and store the locations of  wall and floor tiles around this tile.
	for (int y = -1; y < 1; y++)
	{
		for (int x = -1; x < 1; x++)
		{
			// XOR - get tiles adjacent to the center.
			if ((x != 0) != (y != 0))
			{
				TileData adjacentTile = m_dungeonLayout[y + YPosition][x + XPosition];

				if (adjacentTile.tileType == floorTile)
				{
					if (floorTileCount < 2)
						floorTiles[floorTileCount] = FVector(x, y, 0);
					floorTileCount++;
				}
				else if (adjacentTile.tileType == wallTile)
				{
					wallTileCount++;
				}
			}
		}
	}

	if (floorTileCount == 2 && wallTileCount == 2)
	{
		

		int xSum = floorTiles[0].X + floorTiles[1].X;
		int ySum = floorTiles[0].Y + floorTiles[1].Y;

		// This should be a 1 thick wall, not a corner.
		if (xSum == 0 || ySum == 0)
			return false;

		TileData newCorner = TileData();

		newCorner.tileType = outsideCornerTile;

		// either 180 or 0.
		if (xSum + ySum == 0)
		{
			// 0
			if (xSum == -1)
				newCorner.tileRotation.Yaw = 0;
			else
				newCorner.tileRotation.Yaw = 180;
		}
		// 90 or -90.
		else
			newCorner.tileRotation.Yaw = (xSum + ySum) * 45;

		m_dungeonLayout[YPosition][XPosition] = newCorner;
		return true;

	}

	// Incorrect tile configuration, do not build a corner.
	return false;
}
/**********************************************************************************************************
*	bool SolveInsideCornerTile(int XPositon, int YPosition)
*		Purpose:	Checks to see if the tile at the given position is a wall tile touching 2 empties and
*					2 other walls. If it is, attempts to create an inside corner tile at that location.
*					Returns true if the corner could be made, false if it could not.
*
*		Parameters:
*			int XPosition
*				The x position in the dungeon layout of the tile to solve for.
*			int YPosition
*				The y position in the dungeon layout of the tile to solve for.
*
*		Changes:
*			m_dungeonLayout - The tile at (x, y) could be changed to an inside corner.
*
*		Return: Returns if the tile was changed to a corner.
**********************************************************************************************************/
bool DungeonLayout::SolveInsideCornerTile(int XPosition, int YPosition)
{
	if (m_dungeonLayout == nullptr)
		return false;

	// Make sure the tile is a valid location in the map.
	bool inBounds = true;
	inBounds = inBounds && XPosition >= 0;
	inBounds = inBounds && YPosition >= 0;
	inBounds = inBounds && XPosition <= m_dungeonDimensions.X;
	inBounds = inBounds && YPosition <= m_dungeonDimensions.Y;

	if (!inBounds)
		return false;

	int emptyCount = 0;
	int wallCount = 0;

	FVector wallTiles[2];

	// Count and store the locations of  wall and floor tiles around this tile.
	for (int y = -1; y < 1; y++)
	{
		for (int x = -1; x < 1; x++)
		{
			// XOR - get tiles adjacent to the center.
			if ((x != 0) != (y != 0))
			{
				// Make sure the tile is not out of the level.
				inBounds = true;
				inBounds = inBounds && x + XPosition >= 0;
				inBounds = inBounds && y + YPosition >= 0;
				inBounds = inBounds && x + XPosition <= m_dungeonDimensions.X;
				inBounds = inBounds && y + YPosition <= m_dungeonDimensions.Y;

				if (inBounds)
				{
					TileData adjacentTile = m_dungeonLayout[y + YPosition][x + XPosition];

					if (adjacentTile.tileType == wallTile)
					{
						if (wallCount < 2)
						{
							wallTiles[wallCount] = FVector(y, x, 0);
						}
						wallCount++;
					}
					else if (adjacentTile.tileType == emptyTile)
					{
						emptyCount++;
					}
				}
				else
					emptyCount++;
			}
		}
	}

	if (wallCount == 2 && emptyCount == 2)
	{
		int xSum = wallTiles[0].X + wallTiles[1].X;
		int ySum = wallTiles[0].Y + wallTiles[1].Y;

		// Make sure the walls are not across from each other.
		if (xSum == 0 || ySum == 0)
			return false;

		TileData newCorner = TileData();

		newCorner.tileType = insideCornerTile;

		// either 180 or 0.
		if (xSum + ySum == 0)
		{
			// 0
			if (xSum == -1)
				newCorner.tileRotation.Yaw = 0;
			else
				newCorner.tileRotation.Yaw = 180;
		}
		// 90 or -90.
		else
			newCorner.tileRotation.Yaw = (xSum + ySum) * 45;

		m_dungeonLayout[YPosition][XPosition] = newCorner;
		return true;
	}

	return false;
}