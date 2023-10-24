#ifndef OLIFE_OCTREE_ENTITY_LIST_HH
#define OLIFE_OCTREE_ENTITY_LIST_HH

#include <cstdint>
#include <vector>

namespace oLife {
	template <typename T>
	struct OctreeEntityListNode {
		std::uint32_t next;

		T data;
	};

	template <typename T>
	class OctreeEntityList {
		public:
			OctreeEntityList<T>(std::uint32_t size);

			std::uint32_t insert(T data);
			std::uint32_t remove(std::uint32_t index);

			void clear();
		
		private:
			std::vector<OctreeEntityListNode<T> *> inner;

			std::uint32_t free;
	};
}

#endif
