#include "octree.hh"

namespace oLife {
	Octree::Octree(
		std::uint32_t initial_w,
		std::uint32_t initial_h,
		std::uint32_t initial_l
	) :
		root(OctreeNode::create_node(new OctreeNodeData {
			this,
			nullptr,
			
			0,
			0,
			0,

			initial_w,
			initial_h,
			initial_l,

			0
		})),

		w(initial_w),
		h(initial_h),
		l(initial_l) {}

	Octree::~Octree() {};
}