#pragma once

// Class that stores information for every particle
class Particle {
public:
	float x_;
	float y_;
	float z_;
	float mass_;
	float velocity_x_, velocity_y_, velocity_z_;
	float acceleration_x_, acceleration_y_, acceleration_z_;

	// Default constructor
	Particle() {
		x_ = 0.0;
		y_ = 0.0;
		z_ = 0.0;
		velocity_x_ = 0.0;
		velocity_y_ = 0.0;
		velocity_z_ = 0.0;
		mass_ = 0.0;
		acceleration_x_ = 0.0;
		acceleration_y_ = 0.0;
		acceleration_z_ = 0.0;
	}

	// Constructor useful for center of mass particles
	Particle(float x, float y, float z, float mass) :
		x_(x), y_(y), z_(z), mass_(mass) {
		velocity_x_ = 0.0;
		velocity_y_ = 0.0;
		velocity_z_ = 0.0;
		acceleration_x_ = 0.0;
		acceleration_y_ = 0.0;
		acceleration_z_ = 0.0;
	};

	// Full constructor
	Particle(float x, float y, float z, float velocity_x, float velocity_y, float velocity_z, float mass,
		float acceleration_x, float acceleration_y, float acceleration_z) :
		x_(x), y_(y), z_(z), mass_(mass), velocity_x_(velocity_x), velocity_y_(velocity_y), velocity_z_(velocity_z),
		acceleration_x_(acceleration_x), acceleration_y_(acceleration_y), acceleration_z_(acceleration_z) {};

	void add_acceleration_pairwise(Particle& interacting_particle);
	float get_distance(const Particle& second_particle) const;
	void add_acceleration(float total_mass, float center_of_mass_x, float center_of_mass_y, float center_of_mass_z);
	void add_acceleration(const Particle& interacting_particle);
	void advance(float time_stamp);
	Particle operator+(const Particle& r) const;
	Particle operator-(const Particle& r) const;
	Particle operator*(float r) const;
};
