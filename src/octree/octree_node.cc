#include <algorithm>

#include "octree.hh"
#include "octree_node.hh"

template <typename T>
void fast_erase(std::vector<T> &vector, std::size_t index) {
	vector[index] = std::move(vector.back());

	vector.pop_back();
};

namespace oLife {
	bool OctreeEntity::operator==(const OctreeEntity &other) const {
		return
			x == other.x &&
			y == other.y &&
			z == other.z;
	}

	OctreeNode::OctreeNode(OctreeNodeData *data) : data(data) {}

	OctreeNode::~OctreeNode() {
		for (OctreeNode *child : children)
			delete child;

		delete data;
	}

	OctreeNode *OctreeNode::create_node(OctreeNodeData *data) {
		return new OctreeNode(data);
	}

	void OctreeNode::insert(OctreeEntity *entity) {
		if (
			std::abs(entity->x) > data->octree->w >> 1 &&
			std::abs(entity->y) > data->octree->h >> 1 &&
			std::abs(entity->z) > data->octree->l >> 1
		)
			scale(2);

		if (children.size() != 0) {
			std::uint32_t index = 0;

			if (entity->x >= data->x) index |= 1;
			if (entity->y >= data->y) index |= 2;
			if (entity->z >= data->z) index |= 4;

			children[index]->insert(entity);

			return;
		}

		entities.push_back(entity);

		if (entities.size() == std::max((data->w * data->h * data->l) >> 7, 16u) && data->w && data->h && data->l) {
			for (std::uint32_t index = 0; index < 8; index++)
				children.push_back(OctreeNode::create_node(new OctreeNodeData {
					data->octree,
					this,

					data->x + static_cast<std::int32_t>(data->w >> 1 * (index & 1 ? -1 : 1)),
					data->y + static_cast<std::int32_t>(data->h >> 1 * ((index >> 1) & 1 ? -1 : 1)),
					data->z + static_cast<std::int32_t>(data->h >> 1 * ((index >> 2) & 1 ? -1 : 1)),

					data->octree->w >> (data->level + 1),
					data->octree->h >> (data->level + 1),
					data->octree->l >> (data->level + 1),

					data->level + 1,
					index
				}));
			
			for (OctreeEntity *entity : entities) {
				std::uint32_t index = 0;

				if (entity->x >= data->x) index |= 1;
				if (entity->y >= data->y) index |= 2;
				if (entity->z >= data->z) index |= 4;

				children[index]->insert(entity);
			}

			children.clear();
		}
	}

	void OctreeNode::scale(std::uint32_t scalar) {
		data->octree->w *= scalar;
		data->octree->h *= scalar;
		data->octree->l *= scalar;

		update();
	}

	void OctreeNode::update() {
		for (OctreeNode *child : children) {
			if (child != nullptr)
				child->update();
		}

		for (std::size_t i = 0; i < entities.size(); i++) {
			data->octree->root->insert(entities[i]);

			fast_erase(entities, i);
		}

		if (entities.size() == 0) {
			fast_erase(data->parent->children, data->index);

			delete this;
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

			if (x_pos >= data->x) index |= 1;
			if (y_pos >= data->y) index |= 2;
			if (z_pos >= data->z) index |= 4;

			return children[index]->query(w, h, l, x_pos, y_pos, z_pos);
		}

		std::vector<OctreeEntity *> results;

		std::copy_if(
			entities.begin(),
			entities.end(),
			std::back_inserter(results),
			[&](OctreeEntity *entity) -> bool {
				return
					std::abs(entity->x - x_pos) <= w >> 1 &&
					std::abs(entity->y - y_pos) <= h >> 1 &&
					std::abs(entity->z - z_pos) <= l >> 1;
			}
		);

		return results;
	}

	void OctreeNode::empty() {
		for (OctreeNode *child : children) {
			if (child != nullptr)
				child->empty();
		}

		for (OctreeNode *child : children) {
			if (child != nullptr) {
				if (child->children.size() == 0) {
					delete child;
					continue;
				}

				child->empty();
			}
		}

		entities.clear();
	}

	void OctreeNode::clear() {
		for (OctreeNode *child : children)
			delete child;

		entities.clear();
		children.clear();
	}
}