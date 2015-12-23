#include "stdafx.h"

// Macros
#define GL_GLEXT_PROTOTYPES

// Constants
static const float DEFAULT_X_MIN = 0.0f;
static const float DEFAULT_X_MAX = 2.0f;
static const float DEFAULT_Y_MIN = 0.0f;
static const float DEFAULT_Y_MAX = 2.0f;
static const float DEFAULT_Z_MIN = 0.0f;
static const float DEFAULT_Z_MAX = 2.0f;
static const float DEFAULT_Z_STEP = 0.2f;
static const float DEFAULT_TIME_STEP = 0.05f;
static const float DEFAULT_VALUES_MULTIPLIER = 10.0f;
static const float DEFAULT_GRAPH_STEP = 0.2f;
static const float DEFAULT_GRAPH_MIN = 0.0f;
static const float DEFAULT_GRAPH_MAX = 2.0f;
static const float DEFAULT_GRID_MAX = 1.0f;
static const float DEFAULT_GRID_STEP = 0.2f;
static const int DEFAULT_WINDOW_WIDTH = 800;
static const int DEFAULT_WINDOW_HEIGHT = 600;
static const int DEFAULT_SIGNAL_SIZE_MAX = 200;

// Global Variables
float rotate_y = -90.0f; // Starting eyes_x, eyes_y camera angle
float rotate_x = 90.0f;
float angle = 0.0f; // angle of rotation for the camera direction
float look_x = 1.0f, look_z = 1.0f; // actual vector representing the camera's direction
float eyes_x = 0.5f, eyes_z = 0.5f, eyes_y = 1.0f; // XYZ position of the camera
float scale_percentage = 1.0f;
bool show_grid = true; // Toggle grid on/off
bool show_areas = true; // Toggle grid on/off


enum {
	BRASS, RED_PLASTIC, EMERALD, SLATE
} MaterialType;
enum {
	TORUS_MATERIAL = 1, TEAPOT_MATERIAL = 2, ICO_MATERIAL = 3
} MaterialDisplayList;
enum {
	LIGHT_OFF, LIGHT_RED, LIGHT_WHITE, LIGHT_GREEN
} LightValues;

GLfloat red_light[] =
{ 1.0, 0.0, 0.0, 1.0 }, green_light[] =
{ 0.0, 1.0, 0.0, 1.0 }, white_light[] =
{ 1.0, 1.0, 1.0, 1.0 };
GLfloat left_light_position[] =
{ -1.0, 0.0, 1.0, 0.0 }, right_light_position[] =
{ 1.0, 0.0, 1.0, 0.0 };
GLfloat brass_ambient[] =
{ 0.33, 0.22, 0.03, 1.0 }, brass_diffuse[] =
{ 0.78, 0.57, 0.11, 1.0 }, brass_specular[] =
{ 0.99, 0.91, 0.81, 1.0 }, brass_shininess = 27.8;
GLfloat red_plastic_ambient[] =
{ 0.0, 0.0, 0.0 }, red_plastic_diffuse[] =
{ 0.5, 0.0, 0.0 }, red_plastic_specular[] =
{ 0.7, 0.6, 0.6 }, red_plastic_shininess = 32.0;
GLfloat emerald_ambient[] =
{ 0.0215, 0.1745, 0.0215 }, emerald_diffuse[] =
{ 0.07568, 0.61424, 0.07568 }, emerald_specular[] =
{ 0.633, 0.727811, 0.633 }, emerald_shininess = 76.8;
GLfloat slate_ambient[] =
{ 0.02, 0.02, 0.02 }, slate_diffuse[] =
{ 0.02, 0.01, 0.01 }, slate_specular[] =
{ 0.4, 0.4, 0.4 }, slate_shininess = .78125;

int shade_model = GL_SMOOTH;
char *left_light, *right_light;
char *ico_material, *teapot_material, *torus_material;

int spinning = 0, moving = 0;
int beginx, beginy;
int scaling;

float oldMouseX;
float mouseX;
float oldMouseY;
float mouseY;

int thread_count = DEFAULT_NUMBER_OF_THREADS;
float gravity = GRAVITATIONAL_CONSTANT;
size_t particle_count = DEFAULT_PARTICLE_COUNT;
float total_time_steps = 1.0f;
float time_step = 1.0f;
size_t universe_size_x = UNIVERSE_SIZE_X;
size_t universe_size_y = UNIVERSE_SIZE_Y;
size_t universe_size_z = UNIVERSE_SIZE_Z;

std::vector<Particle> particles;
tbb::concurrent_vector<Particle, tbb::cache_aligned_allocator<Particle>> particles_tbb;
// Function Prototypes
void display();
#include <windows.h>  // for MS Windows

/* Global variables */
char title[] = "N-Body";
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 16;        // refresh interval in milliseconds [NEW]

/* Initialize OpenGL Graphics */

void add_particles() {	
	ParticleHandler::allocate_random_particles(particle_count, particles, universe_size_x, universe_size_y, universe_size_z);
	particles_tbb = ParticleHandler::to_concurrent_vector(particles);
	particle_count = particles_tbb.size();
}

void reset_particles() {
	particles.clear();
	particles_tbb.clear();
	ParticleHandler::allocate_random_particles(particle_count, particles, universe_size_x, universe_size_y, universe_size_z);
	particles_tbb = ParticleHandler::to_concurrent_vector(particles);
}

// Helpful function to draw a char* array of characters
void draw_string(void * font, char *s, float x, float y, float z) {
	unsigned int i;
	glRasterPos3f(x, y, z);
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter(font, s[i]);
}

// Draw the 3d Cartesian system without any points
void draw_3d_cartesian_system() {

	// Print stationary help text
	float hud_x = eyes_x - 6.0f;
	float hud_y = eyes_y + 1.7f;
	float hud_z = eyes_z + 2.3f;
	
//	std::cout << "= Parallel N-Body simulation serially and with Thread Building Blocks =" << std::endl;
//	std::cout << "Number of threads: " << thread_count << std::endl;
//	std::cout << "Total time steps: " << total_time_steps << std::endl;
//	std::cout << "Time step: " << time_step << std::endl;
//	std::cout << "Particle count: " << particle_count << std::endl << std::endl;
//	std::cout << "Universe Size: " << universe_size_x << " x " << universe_size_y << std::endl << std::endl;

	glColor3f(1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Parallel N-Body simulation with TBB", hud_x + 3.5f, hud_y, hud_z); // Chart Legend	
//	glColor3f(1.0, 1.0, 1.0);
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Toggle 2D Signal Areas: F4 key.", hud_x + 0.5f, hud_y, hud_z - 0.5f);
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Rotation: UP, DOWN, LEFT, RIGHT keys.", hud_x + 0.4f, hud_y, hud_z - 0.70f);
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Scale IN/OUT: F1, F2 keys.", hud_x + 0.3f, hud_y, hud_z - 1.5f);
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Toggle grid: F3 key.", hud_x + 0.1f, hud_y, hud_z - 2.5f);
//	
//	
//	char num_of_threads[30];
//	snprintf(num_of_threads, sizeof(num_of_threads), "Number of threads: %d", thread_count);
//	draw_string(GLUT_BITMAP_HELVETICA_12, num_of_threads, hud_x + 0.1f, hud_y, hud_z - 3.0f);
//
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Total Time steps:", hud_x + 0.5f, hud_y, hud_z - 0.5f);
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Particle Count:", hud_x + 0.5f, hud_y, hud_z - 0.5f);
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Time Step:", hud_x + 0.5f, hud_y, hud_z - 0.5f);
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Particle Count:", hud_x + 0.5f, hud_y, hud_z - 0.5f);
//	draw_string(GLUT_BITMAP_HELVETICA_12, "Universe Size:", hud_x + 0.5f, hud_y, hud_z - 0.5f);

	// Rotate when user changes rotate_x and rotate_y angles
	glRotatef(rotate_x, 2.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 2.0, 0.0);

	// Set the zoom scale
	glScalef(scale_percentage, scale_percentage, scale_percentage);

	// Draw Axis Lines & Labels
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	glColor3f(1.0, 0.0, 0.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "X", DEFAULT_X_MAX + 0.2f, 0.0, 0.0);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, 0.01f, 0.01f, DEFAULT_Y_MAX, 32, 32);
	glTranslatef(0.0f, 0.0f, DEFAULT_Y_MAX);
	gluCylinder(quadratic, 0.05f, 0.0f, 0.1f, 32, 32);
	glTranslatef(0.0f, 0.0f, -DEFAULT_Y_MAX);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	
	glColor3f(1.0, 1.0, 0.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "Y", 0.0, DEFAULT_Y_MAX + 0.2f, 0.0);
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	gluCylinder(quadratic, 0.01f, 0.01f, DEFAULT_Y_MAX, 32, 32);
	glTranslatef(0.0f, 0.0f, DEFAULT_Y_MAX);
	gluCylinder(quadratic, 0.05f, 0.0f, 0.1f, 32, 32);
	glTranslatef(0.0f, 0.0f, -DEFAULT_Y_MAX);
	glRotatef(-90.0f, -1.0f, 0.0f, 0.0f);

	glColor3f(0.0, 0.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "Z", 0.0, 0.0, DEFAULT_Z_MAX + 0.2f); 	
	gluCylinder(quadratic, 0.01f, 0.01f, DEFAULT_Z_MAX, 32, 32);
	glTranslatef(0.0f, 0.0f, DEFAULT_Z_MAX);
	gluCylinder(quadratic, 0.05f, 0.0f, 0.1f, 32, 32);
	glTranslatef(0.0f, 0.0f, -DEFAULT_Z_MAX);
	
	// Draw Grid lines
	if (show_grid) {
		glLineWidth(1);
		glBegin(GL_LINES);
		for (float i = DEFAULT_GRAPH_MIN; i < DEFAULT_GRAPH_MAX; i += 0.1f) {
			// Plane X
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(DEFAULT_X_MIN, i, 0.0f);
			glVertex3f(DEFAULT_X_MAX, i, 0.0f);
			if (static_cast<int>(i * 10) % 2 == 0) {
				glVertex3f(i, DEFAULT_Y_MIN, 0.0f);
				glVertex3f(i, DEFAULT_Y_MAX, 0.0f);
			}
			// Plane Z
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(DEFAULT_Z_MIN, 0.0f, i);
			glVertex3f(DEFAULT_Z_MAX, 0.0f, i);
			if (static_cast<int>(i * 10) % 2 == 0.0) {
				glVertex3f(i, 0.0f, DEFAULT_Z_MIN);
				glVertex3f(i, 0.0f, DEFAULT_Z_MAX);
			}
		}
		glEnd();
	}

	// Draw axis division digit labels
	glColor3f(1.0, 1.0, 0.0);
	for (float i = DEFAULT_X_MIN; i <= DEFAULT_X_MAX; i += DEFAULT_GRAPH_STEP) {
		char digits[5];

		snprintf(digits, sizeof(digits), "%f", i * DEFAULT_VALUES_MULTIPLIER);
		draw_string(GLUT_BITMAP_HELVETICA_10, digits, i, -0.05f, 0);
		draw_string(GLUT_BITMAP_HELVETICA_10, digits, -0.05f, i, 0);
	}
}

/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */


/* Called back when timer expired [NEW] */
void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
											   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

// Helps in looping through all the colours based on ID
void set_colour(int colour) {
	switch (colour)
	{
	case 0: // White
		glColor3f(1.0, 1.0, 1.0);
		break;
	case 1: // Green
		glColor3f(0.0, 1.0, 0.0);
		break;
	case 2: // Blue
		glColor3f(0.0, 0.0, 1.0);
		break;
	case 3: // Yellow
		glColor3f(1.0, 1.0, 0.0);
		break;
	case 4: //
		glColor3f(0.0, 1.0, 1.0);
		break;
	case 5: //
		glColor3f(1.0, 0.0, 1.0);
		break;
	case -1: // Red
		glColor3f(1.0, 0.0, 0.0);
		break;
	}
}


void simulate_serial_barnes_hut(std::vector<Particle>& particles, float total_time_steps, float time_step, size_t particle_count,
	size_t universe_size_x, size_t universe_size_y) {

	int png_step_counter = 0;
	QuadParticleTree* quad_tree;

	for (float current_time_step = 0.0; current_time_step < total_time_steps; current_time_step += time_step) {

		// (Re)Allocate all the vector particles into the tree
		quad_tree = ParticleHandler::to_quad_tree(particles, universe_size_x * 2, universe_size_y * 2, universe_size_z * 2);

		// Apply acceleration force to all the particles of the vector
		for (Particle& current_particle : particles)
			quad_tree->apply_acceleration(current_particle);

		// Advance the particles in time
		for (Particle& current_particle : particles)
			current_particle.advance(time_step); // Advance the particle positions in time

												 // Recursively de-allocate the tree
		delete quad_tree;

		++png_step_counter;
		if (SAVE_INTERMEDIATE_PNG_STEPS && SAVE_PNG && png_step_counter >= SAVE_PNG_EVERY) {

			png_step_counter = 0;
			std::string file_name = "universe_serial_barnes_hut_timestep_" + std::to_string(current_time_step) + ".png";

			// TODO: fix the ability to print universe
			//ParticleHandler::universe_to_png(particles, universe_size_x, universe_size_y, file_name.c_str());
		}
	}
}

void simulate_parallel_barnes_hut(tbb::concurrent_vector<Particle>& particles, float total_time_steps, float time_step, size_t particle_count,
	size_t universe_size_x, size_t universe_size_y) {

	int png_step_counter = 0;
	tbb::atomic<QuadParticleTree*> atomic_quad_tree;

	for (float current_time_step = 0.0; current_time_step < total_time_steps; current_time_step += time_step) {

		// (Re)Allocate all the vector particles into the tree			
		atomic_quad_tree = new QuadParticleTree(Particle(0.0f, 0.0f, 0.0f, 0.0f), //Crate a new quad tree with limits from zero, up to grid size x and y
			Particle(static_cast<float>(universe_size_x) * 2, static_cast<float>(universe_size_y) * 2, 0.0f, 0.0f)); // x2 due to an issue on the tree min/max bounds
		
	// Insert the points in the quad tree
		TreeParticle *quad_tree_particles = new TreeParticle[particle_count];

		parallel_for(tbb::blocked_range<size_t>(0, particle_count), // Get range for this thread
			[&](const tbb::blocked_range<size_t>& r) {
			for (size_t index = r.begin(); index != r.end(); ++index) { // Using index range
				quad_tree_particles[index].set_particle(particles[index]);
			}
		}); // Implicit barrier

			// Must be performed serially. Parallel version requires lots of safe regions anyway
		for (size_t i = 0; i < particle_count; ++i) {
			atomic_quad_tree->insert(quad_tree_particles + i);
		}

		parallel_for(tbb::blocked_range<size_t>(0, particle_count), // Get range for this thread
			[&](const tbb::blocked_range<size_t>& r) {
			for (size_t index = r.begin(); index != r.end(); ++index) { // Using index range
				atomic_quad_tree->apply_acceleration(particles[index]);
			}
		}); // Implicit barrier

			// Now that all the new accelerations were calculated, advance the particles in time
		parallel_for(tbb::blocked_range<size_t>(0, particle_count), // Get range for this thread
			[&](const tbb::blocked_range<size_t>& r) {
			for (size_t index = r.begin(); index != r.end(); ++index) { // Using index range
				particles[index].advance(time_step);
			}
		}
		); // Implicit barrier

		   // Recursively de-allocate the tree
		delete atomic_quad_tree;

		++png_step_counter;
		if (SAVE_INTERMEDIATE_PNG_STEPS && SAVE_PNG && png_step_counter >= SAVE_PNG_EVERY) {

			png_step_counter = 0;
			std::string file_name = "universe_serial_barnes_hut_timestep_" + std::to_string(current_time_step) + ".png";

			// TODO: fix the ability to print universe
			//ParticleHandler::universe_to_png(ParticleHandler::to_vector(particles), universe_size_x, universe_size_y, file_name.c_str());
		}
	}
}

void simulate_tbb2(tbb::concurrent_vector<Particle>& particles, float total_time_steps, float time_step, size_t particle_count) {

	// Simulate
	for (float current_time_step = 0.0f; current_time_step < total_time_steps; current_time_step += time_step) {

		parallel_for(tbb::blocked_range<size_t>(0, particle_count),
			[&](const tbb::blocked_range<size_t>& r) {
			Particle current_particle = particles[r.begin()]; // Thread local variable
			for (size_t i = r.begin(); i != r.end(); ++i) {
				current_particle = particles[i];
				for (size_t j = i; j != particle_count; ++j) { // Calculate pairs of accelerations
					current_particle.add_acceleration_pairwise(particles[j]); // Pairwise eliminates branching
				}
				particles[i] = current_particle;
			}
		}
		);
				
		parallel_for(tbb::blocked_range<size_t>(0, particle_count),
			[&](const tbb::blocked_range<size_t>& r) {
			for (size_t index = r.begin(); index != r.end(); ++index) { // Using index range
				particles[index].advance(time_step);
			}
		}
		);
	}
}

// Callback for key presses
void special_keys(int key_pressed, int x, int y) {
	switch(key_pressed) {
	case GLUT_KEY_END:
		eyes_y -= 0.25f;
		break;
	case GLUT_KEY_HOME:
		eyes_y += 0.25f;
		break;
	// Right arrow - increase rotation by 5 degrees
	case GLUT_KEY_RIGHT:
		rotate_y -= 5;
		break;
	// Left arrow - decrease rotation by 5 degree
	case GLUT_KEY_LEFT:
		rotate_y += 5;
		break;
	case GLUT_KEY_UP:
		rotate_x -= 5;
		break;
	case GLUT_KEY_DOWN:
		rotate_x += 5;
		break;
	// Keys F1, F2 for scale in/out, F3 to toggle grid 
	case GLUT_KEY_F1:
		scale_percentage += 0.1f;
		break;
	case GLUT_KEY_F2:
		scale_percentage -= 0.1f;
		break;
	case GLUT_KEY_F3:
		if (show_grid)
			show_grid = false;
		else
			show_grid = true;
		break;
	case GLUT_KEY_F4:
		if (show_areas)
			show_areas = false;
		else
			show_areas = true;
		break;
	case GLUT_KEY_F7:
		add_particles();
		break;
	case GLUT_KEY_F8:
		reset_particles();
		break;
	case GLUT_KEY_F9:
		eyes_x -= 0.25f;
		break;
	case GLUT_KEY_F10:
		eyes_x += 0.25f;
		break;
	}
	//  Request display update
	glutPostRedisplay();
}

void
mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		spinning = 0;
		glutIdleFunc(NULL);
		moving = 1;
		beginx = x;
		beginy = y;
		if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
			scaling = 1;
		}
		else {
			scaling = 0;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		moving = 0;
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	// Render a color-cube consisting of 6 quads with different colors
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(1.5f, 0.0f, -7.0f);  // Move right and into the screen

	gluLookAt(eyes_x + 0.0f, eyes_y + 0.0f, eyes_z + 0.0f,
		eyes_x, 2.0f, 0.0f,
		0.0f, 2.0f, 0.0f);

	draw_3d_cartesian_system();

	simulate_tbb2(particles_tbb, total_time_steps, time_step, particle_count); // Advance Simulation with TBB

	for (const Particle& current_particle : particles_tbb) {
		
		float x = static_cast<float>(current_particle.x_) / (universe_size_x / 2.0f);
		float y = static_cast<float>(current_particle.y_) / (universe_size_y / 2.0f);
		float z = static_cast<float>(current_particle.z_) / (universe_size_z / 2.0f);

		glPointSize(current_particle.mass_ * 4.0f);
		set_colour(static_cast<int>(current_particle.mass_ * 5.0f) % 10);// Set Colour rotation based on ID
		glBegin(GL_POINTS);
		glVertex3f(x, y, z);
		glEnd();

		/*glBegin(GL_TRIANGLES);
		glVertex3f(x + 0.1f * (current_particle.velocity_x_), y + 0.1f * (current_particle.velocity_y_), z + 0.1f * (current_particle.velocity_z_));
		glVertex3f(x + 0.1f, y, z);
		glVertex3f(x - 0.1f, y, z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(x, y, z);
		glVertex3f(x + 0.1f * (current_particle.velocity_x_), y + 0.1f * (current_particle.velocity_y_), z + 0.1f * (current_particle.velocity_z_));
		glEnd();*/
	
	}

	glutSwapBuffers();  // Swap the front and back frame buffers (float buffering)
}


void
controlLights(int value)
{
	switch (value) {

#ifdef GL_MULTISAMPLE_SGIS
	case 3:
		if (glIsEnabled(GL_MULTISAMPLE_SGIS)) {
			glDisable(GL_MULTISAMPLE_SGIS);
		}
		else {
			glEnable(GL_MULTISAMPLE_SGIS);
		}
		break;
#endif
	case 4:
		glutFullScreen();
		break;
	case 5:
		exit(0);
		break;
	}
	glutPostRedisplay();
}


void main_menu_select(int value)
{
	if (value == 666)
		exit(0);
	glShadeModel(shade_model = value);
	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {

	int left_light_m, right_light_m, torus_m, teapot_m, ico_m;
	// User input data
	particle_count = 1000;
	universe_size_x = 800;
	universe_size_y = 800;
	universe_size_z = 800;
	time_step = 1.5;
	total_time_steps = 1.5;
	thread_count = 4;

	// Put random live cells
	ParticleHandler::allocate_random_particles(particle_count, particles, universe_size_x, universe_size_y, universe_size_z);
	particles_tbb = ParticleHandler::to_concurrent_vector(particles);
	simulate_tbb2(particles_tbb, total_time_steps, time_step, particle_count); // Advance Simulation with TBB

	glutInit(&argc, argv);            // Initialize GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // Enable float buffered mode
	glutInitWindowSize(1600, 1200);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title

	glutMouseFunc(mouse);
	
	// Set the OpenGL texture mapping and blending parameters for this program.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections		
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//also try GL_LINE
	glutDisplayFunc(display);       // Register callback handler for window re-paint event

#define LIGHT_MENU_ENTRIES() \
    glutAddMenuEntry("Disable", LIGHT_OFF); \
    glutAddMenuEntry("Red", LIGHT_RED); \
    glutAddMenuEntry("White", LIGHT_WHITE); \
    glutAddMenuEntry("Green", LIGHT_GREEN);
#define MATERIAL_MENU_ENTRIES() \
    glutAddMenuEntry("Brass", BRASS); \
    glutAddMenuEntry("Red plastic", RED_PLASTIC); \
    glutAddMenuEntry("Emerald", EMERALD); \
    glutAddMenuEntry("Slate", SLATE);


	glutCreateMenu(main_menu_select);
	glutAddMenuEntry("Smooth shading", GL_SMOOTH);
	glutAddMenuEntry("Flat shading", GL_FLAT);
	glutAddMenuEntry("Quit", 666);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutSpecialFunc(special_keys);
	

	glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
}