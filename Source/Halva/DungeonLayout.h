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
*		DungeonLayout(FVector DungeonSize, FVector MinimumRoomSize, int DesiredRooms, int PathWidth, FRandomStream RNG);
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
*			-Set
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
*		void DropRooms()
*			Drops random rooms until the number of rooms existing is not larger than m_targetNumberRooms.
*		void GeneratePaths()
*			Generates a set of paths between rooms and populates m_paths.
*		void CreateRoomLayout()
*			Sets the values in the m_dungeonLayout 2d array to match the values in m_rooms and m_paths.
*			The resulting layout will only be made of walls and empties.
*		void ErodeRoomLayout()
*			Causes the edges of rooms and paths to be replaced with floors in a random fashion. This
*			allows rooms generated to look more natural than perfect rectangles if desired.
*		void CreateTiles()
*			Scans over each tile and picks the tile that best solves the scenario.
*		int CountRooms()
*			Counts all rooms in the dungeon.
*		TArray<Quad> GetListOfAllRooms()
*			Returns a list of each room in the dungeon.
*		void GenerateRoomRecursive(QuadTreeNode * CurrentNode)
*			Finds all the children below this node and creates a random room for them. The room is then
*			added to m_rooms.
*		Quad GenerateRandomRoom(Quad MaximumBounds)
*			Creates a random Quad within MaximumBounds that is at least the size of m_minimumRoomSize.
*		bool DropRandomRoomRecursive(QuadTreeNode * CurrentNode)
*			Follows the tree down from CurrentNode until a leaf node is found. The room from this leaf
*			node is removed from m_rooms.
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
*		void GeneratePathsRecursive(QuadTreeNode * CurrentNode)
*			Walks down the tree connecting all siblings it finds along the way. The siblings will have at
*			most a single connection but are guaranteed to be at least indirectly connected.
*		Quad FindClosestRoom(QuadTreeNode * ParentNode, FVector Point)
*			Finds the room that is closest to the point given in the tree. The distance is calculated at
*			the edge of the room.
*		FVector FindCenterOfClosestEdge(Quad Room, FVector Point)
*			Finds the center of the edge that is closest to the point given.
*		void ClearDungeonLayout()
*			Clears all tiles in the created room leaving only empty in each slot.
*		void CreateFloorQuad(Quad Room)
*			All array elements in the dungeon layout that are contained within Room are changed to floor
*			tiles.
*		TileData SolveTile(int XPositon, int YPosition, TileData& TileOut)
*			Determines the type of tile and the rotation of the tile needed to complete the dungeon wall.
*		int CountRoomsRecursive(QuadTreeRoot * CurrentNode)
*			Counts all rooms below this in the tree including this node.
*		TArray<Quad> GetListOfAllRoomsRecursive(QuadTreeRoot * CurrentNode)
*			returns a list of all rooms below this node in the tree including this node.
*
*	Data Members:
*
*		QuadTreeNode m_quadTreeRoot
*			The root of the quad tree.
*		TArray<Quad> m_paths
*			A list of all the paths in the dungeon.
*		int m_roomCount
*			The number of rooms contained in this dungeon.
*		int m_targetNumberRooms
*			The number of rooms to create on generation. If this number is too large instead the maximum
*			number of rooms will be given instead dependent on level size and minimum room size.
*		TileData ** m_dungeonLayout
*			Holds all the information needed to pick a tile in each element. This will be an array of 
*			arrays but will be used as a simple 2D array.
*		FVector m_dungeonDimensions
*			The maximum dimensions of the dungeon. This is how many tiles across the level will be once
*			generated.
*		FVector m_minimumRoomSize
*			The smallest room that can be generated in the level.
*		int m_pathWidth
*			The width of all paths between rooms or other paths.
*		FRandomStream m_randomStream 
*			Stream for generating random numbers.
*		int m_erosionPasses
*			The number of times to attempt to replace edges with floor, making the room appear jagged.
*		float m_erosionChance
*			The chance of a wall being replaced with a floor on an erosion pass.
**********************************************************************************************************/
class HALVA_API DungeonLayout
{
public:

	// Manager functions
	DungeonLayout();
	DungeonLayout(FVector DungeonSize, FVector MinimumRoomSize, int DesiredRooms, int PathWidth, int ErosionPasses, float ErosionChance, FRandomStream RNG);
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

	int CountRooms();
	TArray<Quad> GetListOfAllRooms();

	//  Dungeon Generation
	void GenerateDungeonLayout();

private:

	// Dungeon generation steps
	void GenerateRooms();
	void DropRooms();
	void GeneratePaths();
	void CreateRoomLayout();
	void CreateTiles();
	void ErodeRoomLayout();

	// Helper functions
	void GenerateRoomRecursive(QuadTreeNode * CurrentNode);
	Quad GenerateRandomRoom(Quad MaximumBounds);
	bool DropRandomRoomRecursive(QuadTreeNode * CurrentNode);
	bool GeneratePathBetweenQuads(Quad Room1, Quad Room2);
	bool GenerateYAlignedPath(Quad Room1, Quad Room2);
	bool GenerateXAlignedPath(Quad Room1, Quad Room2);
	bool GenerateLBendPath(Quad Room1, Quad Room2);
	bool GenerateLBendPathWithKnownOrientation(Quad Room1, Quad Room2, bool xFirst);
	void GeneratePathsRecursive(QuadTreeNode * CurrentNode);
	Quad FindClosestRoom(QuadTreeNode * ParentNode, FVector Point);
	FVector FindCenterOfClosestEdge(Quad Room, FVector Point);
	void ClearDungeonLayout();
	void CreateFloorQuad(Quad Room);
	bool SolveTile(int XPosition, int YPosition, TileData& TileOut);
	int CountRoomsRecursive(QuadTreeNode * CurrentNode);
	TArray<Quad> GetListOfAllRoomsRecursive(QuadTreeNode * CurrentNode);


	// member variables
	QuadTreeNode m_quadTreeRoot;
	TArray<Quad> m_paths;
	int m_targetNumRooms;
	TileData ** m_dungeonLayout;
	FVector m_dungeonDimensions;
	FVector m_minimumRoomSize;
	int m_pathWidth;
	int m_erosionPasses;
	float m_erosionChance;
	FRandomStream m_randomStream;
};
