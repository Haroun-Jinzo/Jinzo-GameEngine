#pragma once

#pragma once

#include<vector>
#include<queue>
#include<stack>

#include"List.hpp"
#include"States.hpp"
#include"Bounds.h"

namespace Octree
{
	enum class Octant : unsigned char
	{
		Q1 = 0x01, // = 0b00000001
		Q2 = 0x02, // = 0b00000010
		Q3 = 0x04, // = 0b00000100
		Q4 = 0x08, // = 0b00001000
		Q5 = 0x10, // = 0b00010000
		Q6 = 0x20, // = 0b00100000
		Q7 = 0x40, // = 0b01000000
		Q8 = 0x80, // = 0b10000000
	};

	/*
		Utility methods Callbacks
	*/


	//calculate bounds of specified quadrant in bounding Region
	void CalculateBounds(BoundingRegion* out, Octant octant, BoundingRegion parentRegion);
	class node
	{
	public:
		node* parent;

		node* children[8];


		unsigned char activeOctants;
		bool hasChildren = false;

		bool treeReady = false;
		bool treeBuilt = false;

		std::vector<BoundingRegion> objects;
		std::queue<BoundingRegion> queue;

		BoundingRegion region;

		node();
		node(BoundingRegion bounds);

		node(BoundingRegion bounds, std::vector<BoundingRegion> objectList);

		void Build();
		void Update();
		void ProcessPending();
		bool Insert(BoundingRegion obj);
		void Destory();
	};
}