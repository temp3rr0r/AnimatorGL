/*
Generated OpenGL code to display a 3D graph
*/
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
static const int DEFAULT_WINDOW_HEIGHT = 800;
static const int DEFAULT_SIGNAL_SIZE_MAX = 200;

// Global Variables
float rotate_y = -90.0f; // Starting eyes_x, eyes_y camera angle
float rotate_x = 90.0f;
float angle = 0.0f; // angle of rotation for the camera direction
float look_x = 0.5f, look_z = 1.0f; // actual vector representing the camera's direction
float eyes_x = 1.0f, eyes_z = 1.0, eyes_y = 4.0f; // XZ position of the camera
float scale_percentage = 1.2f;
bool show_grid = true; // Toggle grid on/off
bool show_areas = true; // Toggle grid on/off

int thread_count = DEFAULT_NUMBER_OF_THREADS;
double gravity = GRAVITATIONAL_CONSTANT;
size_t particle_count = DEFAULT_PARTICLE_COUNT;
double total_time_steps = DEFAULT_TOTAL_TIME_STEPS;
double time_step = TIME_STEP;
size_t universe_size_x = UNIVERSE_SIZE_X;
size_t universe_size_y = UNIVERSE_SIZE_Y;

std::vector<Particle> particles;
tbb::concurrent_vector<Particle, tbb::cache_aligned_allocator<Particle>> particles_tbb;
// Function Prototypes
void display();
#include <windows.h>  // for MS Windows

/* Global variables */
char title[] = "N-Body";
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 50;        // refresh interval in milliseconds [NEW]

/* Initialize OpenGL Graphics */


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
	glColor3f(1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Signals vs Time Steps", eyes_x - 0.2f, eyes_y + 1.2f, eyes_z); // Chart Legend	
	glColor3f(1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_12, "Rotation: UP, DOWN, LEFT, RIGHT keys.", eyes_x - 0.9f, eyes_y - 1.35f, eyes_z);
	draw_string(GLUT_BITMAP_HELVETICA_12, "Scale IN/OUT: F1, F2 keys.", eyes_x - 0.9f, eyes_y - 1.3f, eyes_z);
	draw_string(GLUT_BITMAP_HELVETICA_12, "Toggle grid: F3 key.", eyes_x - 0.9f, eyes_y - 1.25f, eyes_z);
	draw_string(GLUT_BITMAP_HELVETICA_12, "Toggle 2D Signal Areas: F4 key.", eyes_x - 0.9f, eyes_y - 1.4f, eyes_z);

	// Rotate when user changes rotate_x and rotate_y angles
	glRotatef(rotate_x, 2.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 2.0, 0.0);

	// Set the zoom scale
	glScalef(scale_percentage, scale_percentage, scale_percentage);

	// Draw eyes_x, eyes_y, eyes_z Axis
	glLineWidth(4);
	glBegin(GL_LINES);
	// Axis X
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(DEFAULT_X_MIN, 0.0, 0.0);
	glVertex3f(DEFAULT_X_MAX, 0.0, 0.0);
	// Axis Y
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.0, DEFAULT_Y_MIN, 0.0);
	glVertex3f(0.0, DEFAULT_Y_MAX, 0.0);
	// Axis Z
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, DEFAULT_Z_MIN);
	glVertex3f(0.0, 0.0, DEFAULT_Z_MAX);
	glEnd();

	// Draw Axis labels
	glColor3f(1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "eyes_x (Time Step)", 1.1f, -0.2f, 0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "eyes_y (Value)", -0.2f, 1.1f, 0);
	draw_string(GLUT_BITMAP_HELVETICA_18, "eyes_z (Signal)", 0, -0.2f, 1.1f);

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

void simulate_tbb2(tbb::concurrent_vector<Particle>& particles, double total_time_steps, double time_step, size_t particle_count,
	size_t universe_size_x, size_t universe_size_y) {

	// Simulate
	for (double current_time_step = 0.0; current_time_step < total_time_steps; current_time_step += time_step) {

		parallel_for(tbb::blocked_range<size_t>(0, particle_count),
			[&](const tbb::blocked_range<size_t>& r) {
			Particle current_particle = particles[r.begin()]; // Thread local variable
			for (size_t i = r.begin(); i != r.end(); ++i) {
				current_particle = particles[i];
				for (size_t j = i + 1; j < particle_count; ++j) { // Calculate pairs of accelerations
					current_particle.add_acceleration_pairwise(particles[j]);
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

/* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

// Callback for key presses
void special_keys(int key_pressed, int x, int y) {
	switch(key_pressed) {
	case GLUT_KEY_F9:
		eyes_x -= 0.25f;
		break;
	case GLUT_KEY_F10:
		eyes_x += 0.25f;
		break;
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
	}

	//  Request display update
	glutPostRedisplay();
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

	//simulate_serial2(particles, total_time_steps + 0.05, time_step, particle_count, universe_size_x, universe_size_y);
	simulate_tbb2(particles_tbb, total_time_steps, time_step, particle_count, universe_size_x, universe_size_y); // Advance Simulation with TBB

	for (Particle& current_particle : particles_tbb) {

		set_colour(static_cast<int>(current_particle.mass_) * 10);// Set Colour rotation based on ID

		float x = static_cast<float>(current_particle.x_) / 400.0f;
		float y = static_cast<float>(current_particle.y_) / 400.0f;
		glBegin(GL_QUADS);
		glVertex3f(x - 0.02 * current_particle.mass_, 0.0f, y - 0.012 * current_particle.mass_);
		glVertex3f(x - 0.02 * current_particle.mass_, 0.0f, y);
		glVertex3f(x, 0.0f, y);
		glVertex3f(x, 0.0f, y - 0.02 * current_particle.mass_);
		glEnd();
	}	

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {

	// User input data
	particle_count = 400;
	total_time_steps = 1;
	universe_size_x = 800;
	universe_size_y = 800;
	thread_count = 4;

	// Put random live cells
	ParticleHandler::allocate_random_particles(particle_count, particles, universe_size_x, universe_size_y);
	particles_tbb = ParticleHandler::to_concurrent_vector(particles);
	simulate_tbb2(particles_tbb, total_time_steps + 0.05, time_step, particle_count, universe_size_x, universe_size_y); // Advance Simulation with TBB

	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(800, 800);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutSpecialFunc(special_keys);
	initGL();                       // Our own OpenGL initialization
	glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
}