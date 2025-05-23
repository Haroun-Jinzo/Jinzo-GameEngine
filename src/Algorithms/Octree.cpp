#include "Octree.h"

void Octree::CalculateBounds(BoundingRegion* out, Octant octant, BoundingRegion parentRegion)
{
    glm::vec3 center = parentRegion.calculateCenter();
    if (octant == Octant::Q1) {
        out = new BoundingRegion(center, parentRegion.max);
    }
    else if (octant == Octant::Q2) {
        out = new BoundingRegion(glm::vec3(parentRegion.min.x, center.y, center.z), glm::vec3(center.x, parentRegion.max.y, parentRegion.max.z));
    }
    else if (octant == Octant::Q3) {
        out = new BoundingRegion(glm::vec3(parentRegion.min.x, parentRegion.min.y, center.z), glm::vec3(center.x, center.y, parentRegion.max.z));
    }
    else if (octant == Octant::Q4) {
        out = new BoundingRegion(glm::vec3(center.x, parentRegion.min.y, center.z), glm::vec3(parentRegion.max.x, center.y, parentRegion.max.z));
    }
    else if (octant == Octant::Q5) {
        out = new BoundingRegion(glm::vec3(center.x, center.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, parentRegion.max.y, center.z));
    }
    else if (octant == Octant::Q6) {
        out = new BoundingRegion(glm::vec3(parentRegion.min.x, center.y, parentRegion.min.z), glm::vec3(center.x, parentRegion.max.y, center.z));
    }
    else if (octant == Octant::Q7) {
        out = new BoundingRegion(parentRegion.min, center);
    }
    else if (octant == Octant::Q8) {
        out = new BoundingRegion(glm::vec3(center.x, parentRegion.min.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, center.y, center.z));
    }

}

Octree::node::node()
    : region(BoundTypes::AABB)
{
}

Octree::node::node(BoundingRegion bound)
    :region(bound) {
}

Octree::node::node(BoundingRegion bounds, std::vector<BoundingRegion> objectList)
    : region(bounds)
{
    objects.insert(objects.end(), objectList.begin(), objectList.end());
}

void Octree::node::Build()
{
    /*
        termination conditions
        - no objects (ie an empty leaf node)
        - diemnsions are too small
    */


    // <= 1 object
    if (objects.size() <= 1)
    {
        return;
    }

    // too small
    glm::vec3 dimensions = region.calculateDimensions();
    for (int i = 0; i < 3; i++)
    {
        if (dimensions[i] <= 0.5f)
        {
            return;
        }
    }

    //create regions
    BoundingRegion octants[8];
    for (int i = 0; i < 8; i++)
    {
        CalculateBounds(&octants[i], (Octant)(1 << i), region);
    }

    //determin which octant to place objects in
	std::vector<BoundingRegion> octLists[8]; //array of lists of objects in each octant
	std::stack<int> delList; //list of objects to delete

	for (int i = 0, length = objects.size(); i < length; i++)
	{
		BoundingRegion br = objects[i];
    
		for (int j = 0; j < 8; j++)
		{
			if (octants[j].intersectsWith(br))
			{
				octLists[j].push_back(br);
				delList.push(i);
				break;
			}
		}
    }

    //remove objects on dellist
    while (delList.size() != 0)
    {
		objects.erase(objects.begin() + delList.top());
		delList.pop();
    }

    //populate octants
    for (int i = 0; i < 8; i++)
    {
		if (octLists[i].size() != 0)
		{
			children[i] = new node(octants[i], octLists[i]);
			children[i]->parent = this;
			hasChildren = true;
		}
    }

    treeBuilt = true;
	treeReady = true;
}
void Octree::node::Update()
{
    if (treeBuilt && treeReady)
    {
        // get moved objects that were in this leaf in previous frame
		std::vector<BoundingRegion> movedObjects(objects.size());

        // remove objects that dont exist anymore
		for (int i = 0, listsize = objects.size(); i < objects.size(); i++)
		{
			// remove if on list of dead objects
			//TODO: implement dead object list
		}

		// update child nodes
        if (children != NULL)
        {
            for (unsigned char flags = activeOctants, i = 0; flags > 0; flags >>= 1, i++)
            {
                if (States::isActive(&flags, 0))
                {
                    //active octant
                    if (children[i] != nullptr)
                    {
                        //update child
                        children[i]->Update();
                    }
                }
            }
        }

        //move moved objects in new nodes
		BoundingRegion movedObject;
        while (movedObjects.size() != 0)
        {
            /*
				for each moved object
				- traverse up tree (start with current node) until find node that completely enlocoses object
                - call insert tp oush as far down as possible
            */

            movedObject = movedObjects[0];
			node* currentNode = this;

            while (!currentNode->region.containsRegion(movedObject))
            {
                if (currentNode->parent != nullptr)
                {
					currentNode = currentNode->parent;
				}
                else
                {
                    //no parent, this is the root node
                    break;
                }
            }

            //remove first object, second object now becomes first
			movedObjects.erase(movedObjects.begin());
			objects.erase(objects.begin() + List::getIndexOf<BoundingRegion>(objects, movedObject));
			currentNode->Insert(movedObject);

            //Collision Detection
            //TODO
        }
    }
    else
    {
		// process pending results
		if (queue.size() > 0)
		{
			ProcessPending();
		}
    }
}
void Octree::node::ProcessPending()
{
    if (!treeBuilt)
    {
         //add objects to be sorted into branches when built
		while (queue.size() != 0)
		{
			//add to object list
			objects.push_back(queue.front());
			queue.pop();
		}
        Build();
    }
    else
    {
		// insert the objects immediately
        while (queue.size() != 0)
        {
			Insert(queue.front());
			queue.pop();
        }
    }
}
bool Octree::node::Insert(BoundingRegion obj)
{
    /*
        termination conditions
		- no objects (ie an empty leaf node)
        - dimensions are less then MIN_Bounds
    */

	glm::vec3 dimensions = region.calculateDimensions();
	if (objects.size() == 0 || dimensions.x < 0.5f || dimensions.y < 0.5f || dimensions.z < 0.5f)
	{
		//empty node, add object
		objects.push_back(obj);
		return true;
	}
	// check if object is in this node
    if (!region.containsRegion(obj))
    {
		return parent == nullptr ? false : parent->Insert(obj);
    }

    // create regions if not defined
	BoundingRegion octants[8];
    for (int i = 0; i < 8; i++)
    {
		if (children[i] != nullptr)
		{
			octants[i] = children[i]->region;
		}
        else
        {
			CalculateBounds(&octants[i], (Octant)(1 << i), region);
        }
    }

    //find region that fits item entirely
	bool found = false;
    for (int i = 0; i < 8; i++)
    {
        if (octants[i].containsRegion(obj))
        {
            if (children[i] != nullptr)
            {
                return children[i]->Insert(obj);
            }
            else
            {
				//create new node
				children[i] = new node(octants[i], {obj});
				States::Activate(&activeOctants,  i);
                return true;
            }
        }
    }
    // dosnt fit into children
	objects.push_back(obj);
    return true;
}
void Octree::node::Destory()
{
    //clearing out children
	if (children != nullptr)
	{
		for (int i = 0, flags = activeOctants; flags > 0, flags >> 1; i++)
		{
			if (States::isActive(&flags, 0))
			{
				//active octant
				if (children[i] != nullptr)
				{
					children[i]->Destory();
					delete children[i];
					children[i] = nullptr;
				}
			}
		}
	}
	//clearing out objects
	objects.clear();
    while (queue.size() != 0)
    {
		queue.pop();
    }
}