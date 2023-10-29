#include <algorithm>

#include "octree.hh"
#include "octree_node.hh"

namespace oLife {
	template <typename T>
	void fast_remove(std::vector<T> &vector, std::size_t index) {
		vector[index] = std::move(vector.back());

		vector.pop_back();
	};

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
		if (
			std::abs(entity->x) > octree->w >> 1 &&
			std::abs(entity->y) > octree->h >> 1 &&
			std::abs(entity->z) > octree->l >> 1
		)
			scale(2);

		if (children.size() != 0) {
			std::uint32_t index = 0;

			if (entity->x >= x()) index |= 1;
			if (entity->y >= y()) index |= 2;
			if (entity->z >= z()) index |= 4;

			children[index]->insert(entity);

			return;
		}

		entities.push_back(entity);

		if (entities.size() == std::max((w() * h() * l()) >> 7, 16u) && w() && h() && l()) {
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

	void OctreeNode::scale(std::uint32_t scalar) {
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

		for (std::size_t i = 0; i < entities.size(); i++) {
			octree->root->insert(entities[i]);

			fast_remove(entities, i);
		}
	}

	std::vector<OctreeEntity *> OctreeNode::query(
		std::uint32_t w,
		std::uint32_t h,
		std::uint32_t l,
		std::int32_t x_pos,
		std::int32_t y_pos,
		std::int32_t z_pos
	) {
		if (children.size() != 0) {
			std::uint32_t index = 0;

			if (x_pos >= x()) index |= 1;
			if (y_pos >= y()) index |= 2;
			if (z_pos >= z()) index |= 4;

			return children[index]->query(w, h, l, x_pos, y_pos, z_pos);
		}

		std::vector<OctreeEntity *> results;

		std::copy_if(
			entities.begin(),
			entities.end(),
			std::back_inserter(results),
			[&](OctreeEntity *entity) -> bool {
				return
					entity->x >= x_pos - w / 2 &&
					entity->x <= x_pos + w / 2 &&
					entity->y >= y_pos - h / 2 &&
					entity->y <= y_pos + h / 2 &&
					entity->z >= z_pos - l / 2 &&
					entity->z <= z_pos + l / 2;
			}
		);

		return results;
	}

	void OctreeNode::empty() {
		for (OctreeNode *child : children) {
			if (child == nullptr) continue;

			child->empty();
		}

		entities.clear();
	}

	void OctreeNode::clear() {
		for (OctreeNode *child : children) {
			delete child;
		}

		entities.clear();
		children.clear();
	}
}
