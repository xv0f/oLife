#include <cmath>

#include "octree.hh"
#include "octree_node.hh"

namespace oLife {
	bool OctreeEntity::operator==(const OctreeEntity &other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	OctreeNode::OctreeNode(Octree *octree, std::uint32_t index, std::uint32_t level) :
		octree(octree), index(index), level(level) {}

	std::int32_t OctreeNode::x() {
		return w() * (index & 1 ? -1 : 1);
	}

	std::int32_t OctreeNode::y() {
		return h() * ((index >> 1) & 1 ? -1 : 1);
	}

	std::int32_t OctreeNode::z() {
		return l() * (index >> 2 ? -1 : 1);
	}

	std::uint32_t OctreeNode::w() {
		return octree->w >> level;
	}

	std::uint32_t OctreeNode::h() {
		return octree->h >> level;
	}

	std::uint32_t OctreeNode::l() {
		return octree->l >> level;
	}

	void OctreeNode::insert(OctreeEntity *entity) {
		if (children.size() != 0) {
			std::uint32_t index = 0;

			if (entity->x >= x()) index |= 1;
			if (entity->y >= y()) index |= 2;
			if (entity->z >= z()) index |= 4;

			children[index]->insert(entity);

			return;
		}

		entities.push_back(entity);

		if (entities.size() == (w() * h() * l()) >> 7 && w() && h() && l()) {
			for (std::size_t i = 0; i < 8; i++)
				children[i] = new OctreeNode(octree, i, level + 1);

			for (OctreeEntity *entity : entities) {
				std::uint32_t index = 0;

				if (entity->x >= x()) index |= 1;
				if (entity->y >= y()) index |= 2;
				if (entity->z >= z()) index |= 4;

				children[index]->insert(entity);
			}

			children.clear();
		}
	}

	void OctreeNode::scale(float scalar) {
		octree->w *= scalar;
		octree->h *= scalar;
		octree->l *= scalar;

		update();
	}

	void OctreeNode::update() {
		for (OctreeNode *child : children) {
			if (child == nullptr) continue;

			child->update();
		}

		// Unfinished
	}

	OctreeEntity *OctreeNode::retrieve(
		std::uint32_t w,
		std::uint32_t h,
		std::uint32_t l,
		std::int32_t x,
		std::int32_t y,
		std::int32_t z
	) {
		// Unfinished
	}

	void OctreeNode::empty() {
		for (OctreeNode *child : children) {
			if (child == nullptr) continue;

			child->empty();
		}

		entities.clear();
	}

	void OctreeNode::clear() {
		entities.clear();
		children.clear();
	}
}
