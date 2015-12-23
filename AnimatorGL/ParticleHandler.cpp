#include "stdafx.h"

void ParticleHandler::allocate_random_particles(size_t particle_count, std::vector<Particle>& particles, size_t size_x, size_t size_y, size_t size_z) {
	if (particle_count > 0) {
		std::random_device random_device_;
		std::mt19937 mt_engine(random_device_());
		std::uniform_real_distribution<> real_position_x(0, static_cast<float>(size_x));
		std::uniform_real_distribution<> real_position_y(static_cast<float>(size_y) * 0.4f, static_cast<float>(size_y)* 0.5f);
		std::uniform_real_distribution<> real_position_z(0, static_cast<float>(size_z));
		std::normal_distribution<float> normal_mass(0.0, MAX_MASS);
		std::normal_distribution<float> normal_speed(0.0, MAX_SPEED);
		std::normal_distribution<float> normal_speed_y(0.0, MAX_SPEED);
		std::uniform_real_distribution<> real_speed(0.0, MAX_SPEED);
		std::uniform_real_distribution<> real_acceleration(0.0, MAX_ACCELERATION);
		std::uniform_real_distribution<> real_mass(0.0, MAX_MASS);

		for (size_t i = 0; i < particle_count; ++i)
			particles.push_back(Particle(
				//real_position_x(mt_engine), real_position_y(mt_engine), real_position_z(mt_engine),
				real_position_x(mt_engine), real_position_x(mt_engine), real_position_z(mt_engine),
				//, real_position_y(mt_engine), real_position_z(mt_engine),
				// 0.0, 0.0, 0.0, real_mass(mt_engine), 0.0, 0.0, 0.0));
				normal_speed_y(mt_engine), normal_speed(mt_engine), normal_speed_y(mt_engine),
				//MAX_SPEED * MAX_SPEED, real_speed(mt_engine), MAX_SPEED * MAX_SPEED,
				//real_position_x(mt_engine), real_position_x(mt_engine), real_position_x(mt_engine),
				normal_mass(mt_engine),
				real_acceleration(mt_engine), real_acceleration(mt_engine), real_acceleration(mt_engine)));
	}

}

tbb::concurrent_vector<Particle> ParticleHandler::to_concurrent_vector(const std::vector<Particle>& input_particles) {
	tbb::concurrent_vector<Particle, tbb::cache_aligned_allocator<Particle>> returning_concurrent_vector;

	for (const Particle& current_particle : input_particles)
		returning_concurrent_vector.push_back(current_particle);
	return returning_concurrent_vector;
}

std::vector<Particle> ParticleHandler::to_vector(const tbb::concurrent_vector<Particle>& input_particles) {
	std::vector<Particle> returning_vector;

	for (const Particle& current_particle : input_particles)
		returning_vector.push_back(current_particle);

	return returning_vector;
}

bool ParticleHandler::are_equal(const std::vector<Particle>& first_particles, const std::vector<Particle>& second_particles) {
	bool are_equal = false;

	if (first_particles.size() == second_particles.size()) {
		are_equal = true;
		for (size_t i = 0; i < first_particles.size(); ++i) {
			if (first_particles[i].velocity_x_ != second_particles[i].velocity_x_ && first_particles[i].velocity_y_ != second_particles[i].velocity_y_ &&
				first_particles[i].x_ != second_particles[i].x_ && first_particles[i].y_ != second_particles[i].y_ &&
				first_particles[i].acceleration_x_ != second_particles[i].acceleration_x_ &&
				first_particles[i].acceleration_y_ != second_particles[i].acceleration_y_ && first_particles[i].mass_ != second_particles[i].mass_) {

				are_equal = false;
				break;

			}
		}
	}
	

	return are_equal;
}

QuadParticleTree* ParticleHandler::to_quad_tree(const std::vector<Particle>& input_particles, size_t size_x, size_t size_y, size_t size_z) {

	// Crate a new quad tree with limits from zero, up to grid size x and y
	QuadParticleTree *quad_particle_tree = new QuadParticleTree(Particle(0.0f, 0.0f, 0.0f, 0.0f),
		Particle(static_cast<float>(size_x), static_cast<float>(size_y), static_cast<float>(size_z), 0.0f));

	// Insert the points in the quad tree
	TreeParticle *quad_tree_particles = new TreeParticle[input_particles.size()];
	for (size_t i = 0; i < input_particles.size(); ++i) {
		quad_tree_particles[i].set_particle(input_particles[i]);
		quad_particle_tree->insert(quad_tree_particles + i);
	}

	return quad_particle_tree;
}