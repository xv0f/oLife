#ifndef OLIFE_OCTREE_HH
#define OLIFE_OCTREE_HH

#include "octree_node.hh"

namespace oLife {
	class Octree {
		public:
			OctreeNode *root;

			std::uint32_t w, h, l;

			Octree(
				std::uint32_t initial_w,
				std::uint32_t initial_h,
				std::uint32_t initial_l
			);
			~Octree();
	};
}

#endif