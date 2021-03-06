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
	m_paths = TArray<Quad>();
	m_minimumRoomSize = FVector(0, 0, 0);
	m_dungeonDimensions = FVector(0, 0, 0);
	m_targetNumRooms = 0;
	m_quadTreeRoot = QuadTreeNode(0, Quad(), m_minimumRoomSize, FRandomStream(0));
	m_pathWidth = 0;
	m_dungeonLayout = nullptr;
	m_erosionPasses = 0;
	m_erosionChance = 0;
	m_randomStream = FRandomStream(0);
}
/**********************************************************************************************************
*	DungeonLayout(...)
*		Purpose:	Constructs a dungeon layout. The layout is stored in m_dungeonLayout.
*
*		Parameters:
*			FVector DungeonSize
*				The size of the entire dungeon map. A tile will be generated for each space in this map.
*				For example, if a 32x32 size is given, 32*32 tiles will be generated.
*			FVector MinimumRoomSize
*				The smallest possible room that can be created. Rooms can be larger than this.
*			int DesiredRooms
*				The number of rooms to try and create. Given the room size and dungeon size The number of
*				rooms created might be smaller than this but this number will attempt to be reached.
*			int PathWidth
*				The width of all paths between rooms.
*			int ErosionPasses
*				The number of times to erode walls.
*			float ErosionChance
*				The chance a wall is eroded.
**********************************************************************************************************/
DungeonLayout::DungeonLayout(FVector DungeonSize, FVector MinimumRoomSize, int DesiredRooms, int PathWidth, int ErosionPasses, float ErosionChance, FRandomStream RNG)
{
	// Get the number of cuts to make. This is log4(DesiredRooms) rounded up.
	int Depth = 1;

	if (DesiredRooms > 0)
		Depth = ceil(log(DesiredRooms) / log(4));

	// remove 1 from each edge for walls.
	Quad DungeonBounds = Quad(DungeonSize - FVector(2, 2, 0), FVector(1, 1, 0));

	m_quadTreeRoot = QuadTreeNode(Depth, DungeonBounds, MinimumRoomSize, RNG);

	m_paths = TArray<Quad>();

	m_targetNumRooms = DesiredRooms;
	m_dungeonLayout = nullptr;
	m_dungeonDimensions = DungeonSize;

	m_dungeonLayout = new TileData *[m_dungeonDimensions.Y];

	for (int i = 0; i < m_dungeonDimensions.Y; i++)
	{
		m_dungeonLayout[i] = new TileData[m_dungeonDimensions.X];
	}
	// Initialize dungeon layout.
	ClearDungeonLayout();

	m_minimumRoomSize = MinimumRoomSize;
	m_pathWidth = PathWidth;

	m_erosionPasses = ErosionPasses;
	m_erosionChance = ErosionChance;

	m_randomStream = RNG;

	// Verify all operands are valid before attempting to make the dungeon.
	bool OKToGenerate = true;

	// Verify that the dungeon will have area.
	OKToGenerate = OKToGenerate && floor(DungeonSize.X) > 0 && floor(DungeonSize.Y) > 0;
	// Verify the paths will have area.
	OKToGenerate = OKToGenerate && PathWidth > 0;

	if (OKToGenerate)
	{
		// Create Dungeon.
		GenerateDungeonLayout();
	}
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
	m_paths = Source.m_paths;
	m_pathWidth = Source.m_pathWidth;
	m_dungeonDimensions = Source.m_dungeonDimensions;
	m_minimumRoomSize = Source.m_minimumRoomSize;
	m_targetNumRooms = Source.m_targetNumRooms;
	m_erosionPasses = Source.m_erosionPasses;
	m_erosionChance = Source.m_erosionChance;
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
		if (m_dungeonLayout != nullptr)
		{
			for (int i = 0; i < m_dungeonDimensions.Y; i++)
			{
				delete[] m_dungeonLayout[i];
				m_dungeonLayout[i] = nullptr;
			}
			delete[] m_dungeonLayout;
			m_dungeonLayout = nullptr;
		}

		m_paths = Source.m_paths;
		m_pathWidth = Source.m_pathWidth;
		m_dungeonDimensions = Source.m_dungeonDimensions;
		m_minimumRoomSize = Source.m_minimumRoomSize;
		m_targetNumRooms = Source.m_targetNumRooms;
		m_erosionPasses = Source.m_erosionPasses;
		m_erosionChance = Source.m_erosionChance;
		m_randomStream = Source.m_randomStream;
		m_quadTreeRoot = Source.m_quadTreeRoot;

		if (m_dungeonDimensions.Y > 0 && m_dungeonDimensions.X > 0)
		{
			m_dungeonLayout = new TileData *[m_dungeonDimensions.Y];

			// create a Tile Data array for each element of the TileData pointer array.
			for (int i = 0; i < m_dungeonDimensions.Y; i++)
				m_dungeonLayout[i] = new TileData[m_dungeonDimensions.X];

			// copy the values over.
			for (int y = 0; y < m_dungeonDimensions.Y; y++)
				for (int x = 0; x < m_dungeonDimensions.X; x++)
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
		for (int i = 0; i < m_dungeonDimensions.Y; i++)
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
*	int CountRooms()
*		Purpose:	Counts the rooms in the dungeon.
*
*		Return:
*			The number of rooms in the dungeon.
**********************************************************************************************************/
int DungeonLayout::CountRooms()
{
	return CountRoomsRecursive(&m_quadTreeRoot);
}
/**********************************************************************************************************
*	int GetListOfAllRooms()
*		Purpose:	Gets a list of every room in the dungeon. This is returned by value and modifications
*					to this list do not affect the dungeon.
*
*		Return:
*			A TArray of Quads containing every room in the dungeon.
**********************************************************************************************************/
TArray<Quad> DungeonLayout::GetListOfAllRooms()
{
	return GetListOfAllRoomsRecursive(&m_quadTreeRoot);
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
	ErodeRoomLayout();
	CreateTiles();
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
	int roomCount = CountRooms();

	while (roomCount > m_targetNumRooms)
	{
		DropRandomRoomRecursive(&m_quadTreeRoot);
		roomCount--;
	}
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

	int roomCount = CountRooms();
	
	TArray<Quad> listOfRooms = GetListOfAllRooms();

	for (int i = 0; i < listOfRooms.Num(); i++)
		CreateFloorQuad(listOfRooms[i]);

	for (int i = 0; i < m_paths.Num(); i++)
		CreateFloorQuad(m_paths[i]);
}
/**********************************************************************************************************
*	void ErodeRoomLayout()
*		Purpose:	Causes each empty adjacent to a floor tile to have a chance to be replaced with a
*					floor tile. The process is then repeated for erosion passes times. This allows semi
*					random looking rooms to look natural.
*		Changes:
*			m_dungeonLayout - The values of tiles touching floor tiles have a chance to become floor tiles.
**********************************************************************************************************/
void DungeonLayout::ErodeRoomLayout()
{
	// If no erosion to be done.
	if (m_erosionPasses == 0 || m_erosionChance == 0)
		return;

	// edges cannot be eroded.
	bool edge = false;

	// for each pass
	for (int pass = 0; pass < m_erosionPasses; pass++)
	{
		for (int y = 0; y < m_dungeonDimensions.Y; y++)
		{
			for (int x = 0; x < m_dungeonDimensions.X; x++)
			{
				edge = false;

				int adjacentFloorTiles = 0;

				// Check adjacent tiles for floor tile count
				for (int adjY = -1; adjY <= 1 && !edge; adjY++)
				{
					for (int adjX = -1; adjX <= 1 && !edge; adjX++)
					{
						// XOR - get tiles adjacent to the center.
						if ((adjX != 0) != (adjY != 0))
						{
							// range check.
							if (y + adjY >= 0 && x + adjX >= 0 && y + adjY < m_dungeonDimensions.Y && x + adjX < m_dungeonDimensions.X)
							{
								if (m_dungeonLayout[y + adjY][x + adjX].tileType == floorTile)
									adjacentFloorTiles++;
							}
							else
								edge = true;
						}
					}
				}

				// The more floor tiles the tile is touching, the more likely it is to be
				// eroded. This will cause a more rounder look.
				if (!edge && m_randomStream.FRand() * 100 < m_erosionChance * adjacentFloorTiles)
				{
					m_dungeonLayout[y][x].tileType = floorTile;
					m_dungeonLayout[y][x].tileRotation = FRotator::ZeroRotator;
				}
			}

		}
	}
}
/**********************************************************************************************************
*	void CreateTiles()
*		Purpose:	Scans through each tile on the map and picks which tile will best fit the current
*					location. The tile's type and rotation will be set to the tile that best solves the
*					given scenario.
*
*		Changes:
*			m_dungeonLayout - All tiles will be solved for.
**********************************************************************************************************/
void DungeonLayout::CreateTiles()
{
	if (m_dungeonLayout == nullptr)
		return;

	for (int y = 0; y < m_dungeonDimensions.Y; y++)
	{
		for (int x = 0; x < m_dungeonDimensions.X; x++)
		{
			TileData solvedTile = m_dungeonLayout[y][x];

			if (solvedTile.tileType != TileType::floorTile)
			{
				bool successfulSolve = SolveTile(x, y, solvedTile);

				if (successfulSolve)
					m_dungeonLayout[y][x] = solvedTile;
				else
				{
					// Unsolvable tile error.
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
		if (children != nullptr && children[0] != nullptr)
		{
			for (int i = 0; i < 4; i++)
				GenerateRoomRecursive(children[i]);
		}
		// else this is a leaf node, create a random room for it.
		else
		{
			// Create a random room and store it in this node.
			CurrentNode->SetRoom(GenerateRandomRoom(CurrentNode->GetQuad()));
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
	FVector TopRight = MaximumBounds.GetBounds();
	FVector BottomLeft = MaximumBounds.GetPosition();

	FVector BottomLeftMin = FVector(0, 0, 0);
	FVector BottomLeftMax = FVector(0, 0, 0);
	FVector TopRightMin = FVector(0, 0, 0);
	FVector TopRightMax = FVector(0, 0, 0);

	// The closest position the bottom left corner can be to the bottom left corner of the quad and still
	// generate a room of minimumSize.
	BottomLeftMin = BottomLeft;

	// The closest position the bottom left corner can be to the top right corner and  still generate a
	// room of at least minimumSize.
	BottomLeftMax = TopRight - m_minimumRoomSize;

	// Pick a random bottom left corner.
	FVector newRoomPosition = FVector(0, 0, 0);
	newRoomPosition.X = m_randomStream.RandRange(BottomLeftMin.X, BottomLeftMax.X);
	newRoomPosition.Y = m_randomStream.RandRange(BottomLeftMin.Y, BottomLeftMax.Y);

	// The closest position the top right corner can be to the bottom left of the quad and still
	// generate a room of minimumSize.
	TopRightMin = newRoomPosition + m_minimumRoomSize;

	// The closest position the top right corner can be to the top right corner of the quad and still
	// generate a room of minimumSize.
	TopRightMax = TopRight;

	// Pick a random top right corner.
	FVector newRoomBounds = FVector(0, 0, 0);
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
			int failedTries = 0;
			int failedChildren[4] = { -1, -1, -1, -1 };

			bool removedARoom = false;
			bool childHasFailed = false;
			int randomChild = 0;

			// While there are still children that have not failed.
			while (removedARoom == false && failedTries < 4)
			{
				randomChild = m_randomStream.RandRange(0, 3);
				childHasFailed = false;

				// make sure the random child has not already been tried.
				for (int i = 0; i < failedTries; i++)
				{
					if (randomChild == failedChildren[i])
						childHasFailed = true;
				}

				// If this child has not been tried before
				if (childHasFailed == false)
					removedARoom = DropRandomRoomRecursive(children[randomChild]);

				if (removedARoom == false)
				{
					failedChildren[failedTries] = randomChild;
					failedTries++;
				}
				// The child succeeded.
				else
					return true;
			}
		}
		// This is a leaf node.
		else
		{
			CurrentNode->SetRoom(Quad());
			return true;
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

	// Casting to bool not allowed.
	bool randomBool = false;

	if (randomBendDirection)
		randomBool = true;


	bool succeeded = false;



	succeeded = GenerateLBendPathWithKnownOrientation(Room1, Room2, randomBool);

	// If the bend could not be completed try it the other way.
	if (!succeeded)
		succeeded = GenerateLBendPathWithKnownOrientation(Room1, Room2, !randomBool);

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
	if (!(XGenerated && YGenerated))
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
		
		// If this node has children
		if (children != nullptr && children[0] != nullptr)
		{
			// Pick a random number to not generate a path for.
			int DontBuildPathIndex = m_randomStream.FRandRange(0, 3);

			// Pick a random point on the intersect between each sibling.

			// ORDER:
			// 0 = Bottom Left
			// 1 = Top Left
			// 2 = Top Right
			// 3 = Bottom Right

			Quad currentChildQuad = Quad();
			FVector randomPointBetweenQuads = FVector(0, 0, 0);
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
					Quad room1 = FindClosestRoom(children[i], randomPointBetweenQuads);
					Quad room2 = Quad();

					// If there are no rooms to connect to, give up.
					if (room1.GetBounds() != FVector(0, 0, 0))
					{
						// This for loop allows the connection of diagonal rooms if there isn't
						// a room at child[i+1].
						for (int j = 0; room2.GetBounds() == FVector(0, 0, 0) && j < 2; j++)
						{
							// Find the closest room belonging to the child next to this, or, if that
							// doesn't exist, the child diagonal from this.
							room2 = FindClosestRoom(children[(i + j + 1) % 4], randomPointBetweenQuads);
						}

						// If 2 rooms were found generate a path between them.
						if (room2.GetBounds() != FVector(0, 0, 0))
							GeneratePathBetweenQuads(room1, room2);
					}
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
/**********************************************************************************************************
*	Quad FindClosestRoom(QuadTreeNode * ParentNode, FVector Point)
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
Quad DungeonLayout::FindClosestRoom(QuadTreeNode * ParentNode, FVector Point)
{
	Quad closestRoom = Quad();

	if (ParentNode != nullptr)
	{
		QuadTreeNode ** children = ParentNode->GetChildren();
		// If this node has children.
		if (children != nullptr && children[0] != nullptr)
		{
			Quad newClosestRoom = Quad();
			float newDistance = 0;
			float oldDistance = 0;

			for (int i = 0; i < 4; i++)
			{
				newClosestRoom = FindClosestRoom(children[i], Point);
				
				// If the newClosestRoom has an area.
				if (newClosestRoom.GetBounds() != FVector(0, 0, 0))
					newDistance = (FindCenterOfClosestEdge(newClosestRoom, Point) - Point).Size();

				// If there is an old closest room and a new closest room compare the values.
				if (closestRoom.GetBounds() != FVector(0, 0, 0) && newClosestRoom.GetBounds() != FVector(0, 0, 0))
				{
					oldDistance = (FindCenterOfClosestEdge(closestRoom, Point) - Point).Size();

					if (newDistance < oldDistance)
						newClosestRoom = closestRoom;
				}
				else if (closestRoom.GetBounds() == FVector(0, 0, 0))
					closestRoom = newClosestRoom;
			}

		}
		else
			closestRoom = ParentNode->GetRoom();
	}

	return closestRoom;
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
			m_dungeonLayout[y][x].tileLocation = FVector2D(x, y);
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
				//newFloor.tileRotation.Yaw = randomRotation;

				// Assign the tile.
				m_dungeonLayout[y][x] = newFloor;
			}
		}
	}
}
/**********************************************************************************************************
*	bool SolveTile(int X, int Y, TileData& TileOut)
*		Purpose:	Attempts to solve the tile so that it completes the dungeon walls without leaving any
*					gaps. This function should be called on empties. When called on an empty, the
*					surrounding tiles are checked to see what the best tile in this slot would be.
*
*		Parameters:
*			int X
*				The x position in the dungeon layout of the tile to solve for.
*			int Y
*				The y position in the dungeon layout of the tile to solve for.
*			TileData& TileOut
*				The tile solved.
*
*		Return: Returns the tile could be solved for.
**********************************************************************************************************/
bool DungeonLayout::SolveTile(int X, int Y, TileData& TileOut)
{
	if (m_dungeonLayout == nullptr)
		return false;

	// Valid tile?
	if (X < 0 || X >= m_dungeonDimensions.X || Y < 0 || Y >= m_dungeonDimensions.Y)
		return false;

	TArray<FVector2D> adjFloors = TArray<FVector2D>();
	TArray<FVector2D> adjOthers = TArray<FVector2D>();

	TArray<FVector2D> cornerFloors = TArray<FVector2D>();
	TArray<FVector2D> cornerOthers = TArray<FVector2D>();

	// DATA COLLECTION

	// Get surrounding tile data.
	// NOTE: For standard rotation the y axis will be backwards.
	for (int yOffset = -1; yOffset <= 1; yOffset++)
	{
		for (int xOffset = -1; xOffset <= 1; xOffset++)
		{
			// bounds check.
			if (Y + yOffset >= 0 && X + xOffset >= 0 && Y + yOffset < m_dungeonDimensions.Y && X + xOffset < m_dungeonDimensions.X)
			{
				// XOR - get tiles adjacent to the center.
				if ((xOffset != 0) != (yOffset != 0))
				{
					// mark each tile adjacent to the center
					if (m_dungeonLayout[Y + yOffset][X + xOffset].tileType == TileType::floorTile)
						adjFloors.Add(FVector2D(xOffset, -yOffset));
					else
						adjOthers.Add(FVector2D(xOffset, -yOffset));
				}
				else
				{
					// mark each corner to the center.
					if (m_dungeonLayout[Y + yOffset][X + xOffset].tileType == TileType::floorTile)
						cornerFloors.Add(FVector2D(xOffset, -yOffset));
					else
						cornerOthers.Add(FVector2D(xOffset, -yOffset));
				}
			}
			else
			{
				// XOR - get tiles adjacent to the center.
				if ((xOffset != 0) != (yOffset != 0))
					adjOthers.Add(FVector2D(xOffset, -yOffset));
				else
					cornerOthers.Add(FVector2D(xOffset, -yOffset));
			}
		}
	}

	// DECISION MAKING

	// Floor tiles should be considered already solved, avoid solving a floor tile if you can.
	if (TileOut.tileType == TileType::floorTile)
	{
		TileOut.tileRotation = FRotator::ZeroRotator;
		return true;
	}


	// Empty.
	if (adjFloors.Num() == 0 && cornerFloors.Num() == 0)
	{
		TileOut.tileType = TileType::emptyTile;
		TileOut.tileRotation = FRotator::ZeroRotator;
		return true;
	}

	// Pillar or floor.
	else if (adjFloors.Num() == 4)
	{
		TileOut.tileType = TileType::pillarTile;

		TileOut.tileRotation = FRotator::ZeroRotator;
		return true;
	}

	// 1 sided wall or T junction or Wall corner composite
	else if (adjFloors.Num() == 1)
	{

		TileOut.tileType = TileType::oneSidedWallTile;
		float angle = FMath::RadiansToDegrees(FMath::Atan2(adjFloors[0].Y, adjFloors[0].X));

		angle = FMath::RoundToFloat(angle);

		if (angle < 0)
			angle += 360;

		TileOut.tileRotation = FRotator(0, -angle, 0);
		
		for (int i = 0; i < cornerFloors.Num(); i++)
		{
			float cornerFloorAngle = FMath::RadiansToDegrees(FMath::Atan2(cornerFloors[i].Y, cornerFloors[i].X));

			cornerFloorAngle = FMath::RoundToFloat(cornerFloorAngle);

			if (cornerFloorAngle < 0)
				cornerFloorAngle += 360;
			
			if (cornerFloorAngle == FMath::Fmod(angle + 135, 360))
			{
				if (TileOut.tileType == TileType::oneSidedWallTile)
					TileOut.tileType = TileType::wallCornerCompositeTile;
				else
					TileOut.tileType = TileType::tJuctionTile;
			}
			else if (cornerFloorAngle == FMath::Fmod(angle + 225, 360))
			{
				if (TileOut.tileType == TileType::oneSidedWallTile)
					TileOut.tileType = TileType::wallCornerCompositeReversedTile;
				else
					TileOut.tileType = TileType::tJuctionTile;
			}

		}

		
		return true;



	}

	// 3 sided wall.
	else if (adjFloors.Num() == 3)
	{
		TileOut.tileType = TileType::threeSidedWallTile;

		// Set the angle to the floor tile across from the other tile.
		float angle = FMath::Atan2(-adjOthers[0].Y, -adjOthers[0].X);

		TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle), 0);

		return true;
	}

	// Outside corner or 2 sided wall or L bend.
	else if (adjFloors.Num() == 2)
	{
		// 2 sided wall.
		if (adjFloors[0].X + adjFloors[1].X == 0)
		{
			TileOut.tileType = TileType::twoSidedWallTile;

			float angle = FMath::Atan2(adjFloors[0].Y, adjFloors[0].X);

			TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle), 0);

			return true;
		}
		// Outside corner or L Bend.
		else
		{
			TileOut.tileType = TileType::outsideCornerTile;

			for (int i = 0; i < cornerFloors.Num(); i++)
			{
				// if there is a corner floor opposite the two legs
				if (cornerFloors[i].X == -(adjFloors[0].X + adjFloors[1].X) &&
					cornerFloors[i].Y == -(adjFloors[0].Y + adjFloors[1].Y))
				{
					// This is an L bend.
					TileOut.tileType = TileType::lBendTile;
					break;
				}
			}

			float angle0 = FMath::Atan2(adjFloors[0].Y, adjFloors[0].X);
			if (angle0 < 0)
				angle0 += 2.0F * PI;

			float angle1 = FMath::Atan2(adjFloors[1].Y, adjFloors[1].X);
			if (angle1 < 0)
				angle1 += 2.0F * PI;

			// Find the clockwise-most leg of the right triangle the two angles form.
			if (angle1 == FMath::Fmod(angle0 + (PI / 2.0F), 2.0F * PI))
				TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle0), 0);
			else
				TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle1), 0);

			return true;
		}
	}

	// Inside corner.
	else if (adjFloors.Num() == 0 && cornerFloors.Num() > 0)
	{
		// The number of diagonal floors corresponds to the type of tile.
		switch (cornerFloors.Num())
		{
		//Single corner
		case 1:
		{
			TileOut.tileType = TileType::insideSingleCornerTile;
			// subtract pi/4 for the clockwise-most leg.
			float angle = FMath::Atan2(cornerFloors[0].Y, cornerFloors[0].X) - PI / 4;
			TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle), 0);
			break;
		}
		// Double corner
		case 2:
		{
			// Opposite inside double corner.
			if (cornerFloors[0].X + cornerFloors[1].X == 0 && cornerFloors[0].Y + cornerFloors[1].Y == 0)
			{
				TileOut.tileType = TileType::insideDoubleOppositeCornerTile;
				float angle = FMath::Atan2(cornerFloors[0].Y, cornerFloors[0].X) - PI / 4;
				TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle), 0);
			}
			// Adjacent inside double corner.
			else
			{
				TileOut.tileType = TileType::insideDoubleAdjacentCornerTile;

				float angle0 = FMath::Atan2(cornerFloors[0].Y, cornerFloors[0].X);
				if (angle0 < 0)
					angle0 += 2.0F * PI;
				float angle1 = FMath::Atan2(cornerFloors[1].Y, cornerFloors[1].X);
				if (angle1 < 0)
					angle1 += 2.0F * PI;

				// Find the clockwise-most leg of the right triangle the two angles form.
				if (angle1 == FMath::Fmod(angle0 + (PI / 2.0F), 2.0F * PI))
					TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle0 - PI / 4), 0);
				else
					TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle1 - PI / 4), 0);
			}
			break;
		}
		// Triple corner.
		case 3:
		{
			TileOut.tileType = TileType::insideTripleCornerTile;
			// subtract pi/4 for the clockwise-most leg.
			float angle = FMath::Atan2(-cornerOthers[0].Y, -cornerOthers[0].X) - PI / 4;
			TileOut.tileRotation = FRotator(0, -FMath::RadiansToDegrees(angle), 0);
			break;
		}
		// Quadra corner.
		case 4:
		{
			TileOut.tileType = TileType::insideQuadraCornerTile;
			TileOut.tileRotation = FRotator::ZeroRotator;
			break;
		}
		default:
			return false;
		}

		return true;
	}

	return false;
}
/**********************************************************************************************************
*	int CountRoomsRecursive(QuadTreeNode * CurrentNode)
*		Purpose:	Counts all the rooms below this in the tree. This includes the current node.
*
*		Parameters:
*			QuadTreeNode * CurrentNode
*				The highest node in the tree to search.
*
*		Return:
*			Returns the number of rooms found.
**********************************************************************************************************/
int DungeonLayout::CountRoomsRecursive(QuadTreeNode * CurrentNode)
{
	int roomSum = 0;

	if (CurrentNode != nullptr)
	{
		QuadTreeNode ** children = CurrentNode->GetChildren();

		// If this node has children
		if (children != nullptr && children[0] != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				roomSum += CountRoomsRecursive(children[i]);
			}
		}
		else
		{
			if (CurrentNode->GetRoom().GetBounds() != FVector(0, 0, 0))
				roomSum++;
		}
	}

	return roomSum;
}
/**********************************************************************************************************
*	TArray<Quad> GetListOfAllRoomsRecursive(QuadTreeNode * CurrentNode)
*		Purpose:	Gets a list of all rooms below this in the tree. This includes this node.
*
*		Parameters:
*			QuadTreeNode * CurrentNode
*				The highest node in the tree to search.
*
*		Return:
*			Returns a list of every room found.
**********************************************************************************************************/
TArray<Quad> DungeonLayout::GetListOfAllRoomsRecursive(QuadTreeNode * CurrentNode)
{
	TArray<Quad> allRooms = TArray<Quad>();

	if (CurrentNode != nullptr)
	{
		QuadTreeNode ** children = CurrentNode->GetChildren();

		// If this node has children
		if (children != nullptr && children[0] != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				TArray<Quad> childRooms = GetListOfAllRoomsRecursive(children[i]);

				for (int j = 0; j < childRooms.Num(); j++)
					allRooms.Add(childRooms[j]);
			}
		}
		else
		{
			if (CurrentNode->GetRoom().GetBounds() != FVector(0, 0, 0))
				allRooms.Add(CurrentNode->GetRoom());
		}
	}

	return allRooms;
}