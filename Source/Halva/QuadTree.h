// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "QuadTreeNode.h"
/**
 * 
 */
class HALVA_API QuadTree
{
public:
	QuadTree(FVector LevelBounds, FVector MinimumRoomSize, FRandomStream RandomStream);
	~QuadTree();

	FVector m_levelBounds;
	FVector m_minimumRoomSize;
	FRandomStream m_randomStream;
	QuadTreeNode m_root;
};
