// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TileStructure.h"
#include "QuadTreeNode.h"
/**********************************************************************************************************
*	Class: DungeonLayout
*
*	Overview:
*		This class generates and stores a tile layout for a level. The layout will be a two dimensional
*		array with information about the tile at each location. No actual tiles are generated or handled
*		by this class. This class operates with a QuadTree to generate rooms and decide passageways.
*
*	Manager Functions:
*
*		DungeonLayout();
*			Default constructor. Generates an empty dungeon.
*		DungeonLayout(FVector DungeonSize, FVector MinimumRoomSize, int PathWidth, FRandomStream RNG);
*			Constructor. Generates a dungeon that has an area of DungeonSize given that each tile has an
*			area of 1. No rooms shall be generated that are smaller than MinimumRoomSize and all passage
*			ways will have a width of PathWidth.
*		DungeonLayout(const DungeonLayout & source);
*			Copy constructor. takes in source and creates a copy of it.
*		~DungeonLayout();
*			Destructor.
*		DungeonLayout & operator=(const DungeonLayout & source);
*			Assignment operator.
*
*	Mutators:
*
*		DungeonLayout
*			-Get
*		DungeonDimensions
*			-Get
*			-Set+
*		MinimumRoomSize
*			-Get
*			-Set
*		PathWidth
*			-Get
*			-Set
*	Methods:
*
*		void GenerateDungeonLayout()
*			Creates a complete dungeon tile map to be used. If one was already present, it is overwritten.
*		void GenerateRooms()
*			Generates a set of rooms and populates m_rooms.
*		void GeneratePaths()
*			Generates a set of paths between rooms and populates m_paths.
*		void CreateRoomLayout()
*			Sets the values in the m_dungeonLayout 2d array to match the values in m_rooms and m_paths.
*			The resulting layout will only be made of walls and empties, run CreateSpecialTiles() to get
*			other tile types.
*		void CreateSpecialTiles()
*			Generates walls, inside corners, outside corners, and pillars in m_dungeonLayout.
*		void CreateWalls()
*			Generates walls around the rooms and paths in the dungeon layout. This will result in a few
*			errors that can be fixed by running the remaining create functions.
*		void CreatePillars()
*			Generates a pillar where ever there is a wall completely surrounded by only floors.
*		void CreateOutsideCorners()
*			Replaces walls with outside corners where they are needed. These are commonly found where a
*			path merges with a room.
*		void Generate InsideCorners()
*			Replaces walls with inside corners where they are needed. These are at the corners of every
*			room.
*		void GenerateRoomRecursive(QuadTreeNode * CurrentNode)
*			Finds all the children below this node and creates a random room for them. The room is then
*			added to m_rooms.
*		Quad GenerateRandomRoom(Quad MaximumBounds)
*			Creates a random Quad within MaximumBounds that is at least the size of m_minimumRoomSize.
*		bool GeneratePathBetweenQuads(Quad Room1, Quad Room2)
*			Generates a path between the two rooms passed in and places the path in m_paths.
*		bool GenerateYAlignedPath(Quad Room1, Quad Room2)
*			Attempts to generate a path along the Y axis between the two rooms. If it turns out the rooms
*			are not Y aligned, no path is generated.
*		bool GenerateXAlignedPath(Quad Room1, Quad Room2)
*			Attempts to generate a path along the X axis between the two rooms. If it turns out the rooms
*			are not X aligned, no path is generated.
*		bool GenerateLBendPath(Quad Room1, Quad Room2)
*			Attempts to generate a path in the shape of an L bend between the two rooms.
*		bool GenerateLBendPathWithKnownOrientation(Quad Room1, Quad Room2, bool xFirst)
*			Attempts to generate an L bend between the two rooms. Will only attempt one direction based
*			on the bool xFirst.
*		Quad GetExtentsOfAllChildrenRooms(QuadTreeNode * ContainerNode)
*			Gets a Quad that contains every room down the tree from ContainerRoom.
*		void AdjustRoomBoundsRecursive(QuadTreeNode * CurrentNode, Quad & Extents)
*			Follows the given node down the tree and adjusts Extents so that each room found fits inside
*			it.
*		void GeneratePathsRecursive(QuadTreeNode * CurrentNode)
*			Walks down the tree connecting all siblings it finds along the way. The siblings will have at
*			most a single connection but are guaranteed to be at least indirectly connected.
*		Quad * FindClosestRoom(QuadTreeNode * ParentNode, FVector Point)
*			Finds the room that is closest to the point given in the tree. The distance is calculated at
*			the edge of the room.
*		FVector FindCenterOfClosestEdge(Quad Room, FVector Point)
*			Finds the center of the edge that is closest to the point given.
*		void ClearDungeonLayout()
*			Clears all tiles in the created room leaving only empty in each slot.
*		void CreateFloorQuad(Quad Room)
*			All array elements in the dungeon layout that are contained within Room are changed to floor
*			tiles.
*		bool SolveWallTile(int XPosition, int YPosition);
*			Determines if the tile at the given position should be a wall tile or not. If it should be,
*			also determines the rotation it should face. The tile is then assigned the type of wall tile
*			and given the rotation. Return true on successful assignment, false on failure.
*
*	Data Members:
*
*		QuadTreeNode m_quadTreeRoot - The root of the quad tree.
*		TArray<Quad> m_rooms - A list of all the rooms in the dungeon.
*		TArray<Quad> m_paths - A list of all the paths in the dungeon.
*		TileData ** m_dungeonLayout - Holds all the information needed to pick a tile in each element. This
*									  will be an array of arrays but will be used as a simple 2D array.
*		FVector m_dungeonDimensions - The maximum dimensions of the dungeon. This is how many tiles across
*									  the level will be once generated.
*		FVector m_minimumRoomSize - The smallest room that can be generated in the level.
*		int m_pathWidth - The width of all paths between rooms or other paths.
*		FRandomStream m_randomStream - Stream for generating random numbers.
**********************************************************************************************************/
class HALVA_API DungeonLayout
{
public:

	// Manager functions
	DungeonLayout();
	DungeonLayout(FVector DungeonSize, FVector MinimumRoomSize, int PathWidth, FRandomStream RNG);
	DungeonLayout(const DungeonLayout & source);
	~DungeonLayout();

	DungeonLayout & operator=(const DungeonLayout & source);

	// Mutators
	TileData ** GetDungeonLayout();
	FVector GetDungeonDimensions();
	void SetDungeonDimensions(FVector DungeonDimensions);
	FVector GetMinimumRoomSize();
	void SetMinimumRoomSize(FVector MinimumRoomSize);
	int GetPathWidth();
	void SetPathWidth(int PathWidth);

	//  Dungeon Generation
	void GenerateDungeonLayout();

private:

	// Dungeon generation steps
	void GenerateRooms();
	void GeneratePaths();
	void CreateRoomLayout();
	void CreateSpecialTiles();
	void CreateWalls();
	void CreatePillars();
	void CreateOutsideCorners();
	void CreateInsideCorners();

	// Helper functions
	void GenerateRoomRecursive(QuadTreeNode * CurrentNode);
	Quad GenerateRandomRoom(Quad MaximumBounds);
	bool GeneratePathBetweenQuads(Quad Room1, Quad Room2);
	bool GenerateYAlignedPath(Quad Room1, Quad Room2);
	bool GenerateXAlignedPath(Quad Room1, Quad Room2);
	bool GenerateLBendPath(Quad Room1, Quad Room2);
	bool GenerateLBendPathWithKnownOrientation(Quad Room1, Quad Room2, bool xFirst);
	void GeneratePathsRecursive(QuadTreeNode * CurrentNode);
	Quad * FindClosestRoom(QuadTreeNode * ParentNode, FVector Point);
	FVector FindCenterOfClosestEdge(Quad Room, FVector Point);
	void ClearDungeonLayout();
	void CreateFloorQuad(Quad Room);
	bool SolveWallTile(int XPosition, int YPosition);

	// member variables
	QuadTreeNode m_quadTreeRoot;
	TArray<Quad> m_rooms;
	TArray<Quad> m_paths;
	TileData ** m_dungeonLayout;
	FVector m_dungeonDimensions;
	FVector m_minimumRoomSize;
	int m_pathWidth;
	FRandomStream m_randomStream;
};
