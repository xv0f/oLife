#include "octree_entity_list.hh"

namespace oLife {
	template <typename T>
	OctreeEntityList<T>::OctreeEntityList(std::uint32_t size) :
		inner(std::vector<OctreeEntityListNode<T> *>(size)),
		free(size) {};
}