#pragma once

#include "TreeParticle.h"
#include "Particle.h"

// An Octant tree that stores collections of particles
class OctParticleTree {
	static const uint8_t NUM_CHILDREN = 8; // Octant trees have 8 children
	Particle origin;	// The bounding box of this node
	Particle halfDimension; // Helps in spliting to children octants
	OctParticleTree *children[NUM_CHILDREN]; // Pointers to the children octants
	TreeParticle *data; // Temporary placeholder

	float center_of_mass_x_ = 0.0;
	float center_of_mass_y_ = 0.0;
	float center_of_mass_z_ = 0.0;
	float total_mass_ = 0.0;
	uint8_t depth = 0; // The depth of the node compared to the root
public:
	OctParticleTree(const Particle& origin, const Particle& halfDimension);
	float get_side_size() const;
	float get_total_mass() const;
	~OctParticleTree();
	int get_octant_containing_point(const Particle& point) const; // Find the child node octant
	bool isLeafNode() const; // Check if it is a leaf
	void insert(TreeParticle* point); // Insert point in the node
	void apply_acceleration(Particle& input_particle) const;
};