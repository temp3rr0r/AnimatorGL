/*
Generated OpenGL code to display a 3D graph
*/
#include "stdafx.h"
// Macros
#define GL_GLEXT_PROTOTYPES

// Constants
static const float DEFAULT_X_MIN = 0.0f;
static const float DEFAULT_X_MAX = 10.0f;
static const float DEFAULT_Y_MIN = 0.0f;
static const float DEFAULT_Y_MAX = 10.0f;
static const float DEFAULT_Z_MIN = 0.0f;
static const float DEFAULT_Z_MAX = 10.0f;
static const float DEFAULT_Z_STEP = 0.1f;
static const float DEFAULT_TIME_STEP = 0.05f;
static const float DEFAULT_VALUES_MULTIPLIER = 10.0f;
static const float DEFAULT_GRAPH_STEP = 0.1f;
static const float DEFAULT_GRAPH_MIN = 0.0f;
static const float DEFAULT_GRAPH_MAX = 10.0f;
static const float DEFAULT_GRID_MAX = 1.0f;
static const float DEFAULT_GRID_STEP = 0.1f;
static const int DEFAULT_WINDOW_WIDTH = 400;
static const int DEFAULT_WINDOW_HEIGHT = 400;
static const int DEFAULT_SIGNAL_SIZE_MAX = 200;

// Function Prototypes
void display();
void special_keys();

// Global Variables
float rotate_y = -20.0f; // Starting eyes_x, eyes_y camera angle
float rotate_x = 70.0f;
float angle = 0.0f; // angle of rotation for the camera direction
float look_x = 0.0f, look_z = 1.0f; // actual vector representing the camera's direction
float eyes_x = 0.5f, eyes_z = 0.5f, eyes_y = 0.5f; // XZ position of the camera
float scale_percentage = 1.0f;
bool show_grid = true; // Toggle grid on/off
bool show_areas = true; // Toggle grid on/off

// Helpful function to draw a char* array of characters
void draw_string(void * font, char *s, float x, float y, float z) {
	unsigned int i;
	glRasterPos3f(x, y, z);
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter(font, s[i]);
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

// Helpful function to draw CBD signals from a float array
void draw_signal(char* label, float* points, int signal_id) {

	set_colour(signal_id % 6);// Set Colour rotation based on ID

	draw_string(GLUT_BITMAP_HELVETICA_10, label, -0.1f, 0.0f, DEFAULT_Z_STEP * signal_id); // Label

	// Draw points
	glBegin(GL_POINTS);
	glPushMatrix();
	glTranslated(0.0f, 0.0f, 0.0f); // draw an empty circle, hack to remove a bug
	glutSolidSphere(0.0f, 50, 50);
	glPopMatrix();

	for (unsigned int i = 0; i < DEFAULT_SIGNAL_SIZE_MAX; i++) {
		glPushMatrix();
		glTranslated(static_cast<float>(DEFAULT_TIME_STEP * i), points[i] / DEFAULT_VALUES_MULTIPLIER, DEFAULT_Z_STEP * signal_id);
		glutWireSphere(0.01f, 50, 50);
		glPopMatrix();
	}
	glEnd();

	//Draw Lines or 2D Areas with the xX' axis
	unsigned int first = 0; // ID of the previous points' eyes_y value	
	if (show_areas == false)
	{
		// Connect each point with a line
		glLineWidth(1);
		glBegin(GL_LINES);
		glColor3f(0.8f, 0.8f, 1.0f);
		set_colour(signal_id % 6);
		unsigned int first_value = 0; // ID of the previous points' eyes_y value
		for (unsigned int i = 1; i < DEFAULT_SIGNAL_SIZE_MAX; i++) {
			glVertex3f(static_cast<float>(DEFAULT_TIME_STEP * (i - 1)), points[first_value] / DEFAULT_VALUES_MULTIPLIER, DEFAULT_Z_STEP * signal_id);
			glVertex3f(static_cast<float>(DEFAULT_TIME_STEP * i), points[i] / DEFAULT_VALUES_MULTIPLIER, DEFAULT_Z_STEP * signal_id);
			first_value = i;
		}
		glEnd();
	}
	else {
		// Draw trapezoids - integrals below each line
		glLineWidth(1);
		for (unsigned int i = 1; i < DEFAULT_SIGNAL_SIZE_MAX; i++) {
		glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 1.0f);
		set_colour(signal_id % 6);
			glVertex3f(static_cast<float>(DEFAULT_TIME_STEP * (i - 1)), points[first] / DEFAULT_VALUES_MULTIPLIER, DEFAULT_Z_STEP * signal_id);
			glVertex3f(static_cast<float>(DEFAULT_TIME_STEP * i), points[i] / DEFAULT_VALUES_MULTIPLIER, DEFAULT_Z_STEP * signal_id);
			glVertex3f(static_cast<float>(DEFAULT_TIME_STEP * (i - 1)), 0.0f, DEFAULT_Z_STEP * signal_id);
			first = i;
		glEnd();
		}
	}
}

// Display Callback function
void display() {

	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera viewpoint
	gluLookAt(eyes_x, eyes_y, eyes_z,
		eyes_x + look_x, 1.0f, eyes_z + look_z,
		0.0f, 2.0f, 0.0f);

	// Print helpful information to the console
	printf("scale, eyesx, eyesy, eyesz, rotate_x, rotate_y: %f %f %f %f %f %f \n", scale_percentage, eyes_x, eyes_y, eyes_z, rotate_x, rotate_y);

	// Draw the grid, axis, labels, divisions
	draw_3d_cartesian_system();

	// Create the host (local) variables
	unsigned int i = 0;
	float *ansiC_C1 = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));
	float *ansiC_C2 = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));
	float *ansiC_C3 = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));
	float *ansiC_C4 = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));
	float *ansiC_C5 = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));
	float *ansiC_C6 = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));
	float *ansiC_C7 = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));
	float *ansiC_C8 = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));
	float *ansiC_OUT = static_cast<float*>(malloc(sizeof(float)*DEFAULT_SIGNAL_SIZE_MAX));

	// Pack Ansi C variables with data
	for (i = 0; i < DEFAULT_SIGNAL_SIZE_MAX; i++) {
		ansiC_C1[i] = 12.0f;
		ansiC_C2[i] = 3.3f;
		ansiC_C3[i] = 5.0f;
		ansiC_C4[i] = 12.0f;
		ansiC_C5[i] = 5.0f;
		ansiC_C6[i] = 5.0f;
		ansiC_C7[i] = 12.0f;
		ansiC_C8[i] = 5.0f;
	}

	// Initialize intermediate variables
	float ansiC_D = 0.0f;
	float ansiC_A1 = 0.0f;
	float ansiC_A2 = 0.0f;
	float ansiC_N1 = 0.0f;
	float ansiC_A3 = 0.0f;
	float ansiC_M1 = 0.0f;
	float ansiC_ML1 = 0.0f;
	float ansiC_A4 = 0.0f;
	float ansiC_R1 = 0.0f;
	float ansiC_I1 = 0.0f;
	float ansiC_G1 = 0.0f;
	float ansiC_A6 = 0.0f;
	float ansiC_I2 = 0.0f;
	float ansiC_M2 = 0.0f;
	float ansiC_M3 = 0.0f;

	// Calculate data
	for (i = 0; i < DEFAULT_SIGNAL_SIZE_MAX; i++) {
		ansiC_D = 0.0f;
		ansiC_A1 = 0.0f;
		ansiC_A2 = 0.0f;
		ansiC_N1 = 0.0f;
		ansiC_A3 = 0.0f;
		ansiC_M1 = 0.0f;
		ansiC_ML1 = 0.0f;
		ansiC_A4 = 0.0f;
		ansiC_R1 = 0.0f;
		ansiC_I1 = 0.0f;
		ansiC_G1 = 0.0f;
		ansiC_A6 = 0.0f;
		ansiC_I2 = 0.0f;
		ansiC_M2 = 0.0f;
		ansiC_M3 = 0.0f;

		ansiC_A1 = ansiC_D + ansiC_C3[i];
		ansiC_A2 = ansiC_C4[i] + ansiC_C5[i];
		ansiC_N1 = (-1.0f) * ansiC_A2;
		ansiC_A3 = ansiC_A2 + ansiC_C5[i];
		ansiC_M1 = ansiC_A1 * ansiC_N1;
		ansiC_ML1 = ansiC_M1 * ansiC_A2;
		ansiC_A4 = ansiC_C1[i] + ansiC_C2[i];
		ansiC_R1 = static_cast<float>(pow(ansiC_A4, ansiC_C2[i]));
		ansiC_I1 = 1.0f / ansiC_R1;
		ansiC_G1 = static_cast<float>(fabs(ansiC_N1));
		ansiC_A6 = ansiC_G1 + ansiC_C6[i];
		ansiC_I2 = 1.0f / ansiC_C7[i];
		ansiC_M2 = ansiC_I2 * ansiC_A6;
		ansiC_M3 = ansiC_C8[i] * ansiC_M2;
		ansiC_OUT[i] = ansiC_M3;
	}

	// Do draw all the signals
	draw_signal("OUT", ansiC_OUT, 0);
	draw_signal("C1", ansiC_C1, 1);
	draw_signal("C2", ansiC_C2, 2);
	draw_signal("C3", ansiC_C3, 3);
	draw_signal("C4", ansiC_C4, 4);
	draw_signal("C5", ansiC_C5, 5);
	draw_signal("C6", ansiC_C6, 6);
	draw_signal("C7", ansiC_C7, 7);
	draw_signal("C8", ansiC_C8, 8);

	glFlush();
	glutSwapBuffers();
}

// Callback for key presses
void special_keys(int key_pressed, int x, int y) {
	switch(key_pressed) {
	// Right arrow - increase rotation by 5 degrees
	case GLUT_KEY_RIGHT:
		rotate_y += 5;
		break;
	// Left arrow - decrease rotation by 5 degree
	case GLUT_KEY_LEFT:
		rotate_y -= 5;
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

// Main execution point
int main2(int argc, char* argv[]) {

	// Initialize GLUT and process user parameters
	glutInit(&argc, argv);

	// Request float buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// Create window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	glutCreateWindow("CBD 3D Graph");

	// Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	// Callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(special_keys);

	// Pass control to GLUT for events
	glutMainLoop();

	return 0;
}

void display2() {

	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera viewpoint
	//gluLookAt(eyes_x + 0.0f, eyes_y + 0.0f, eyes_z + 0.0f,
	//	eyes_x, 2.0f, 0.0f,
	//	0.0f, 2.0f, 0.0f);

	gluLookAt(
		1.0f, 1.0f, 1.0f, // EyeAt vector
		1.0f, 1.0f, 0.0f, // Center point
		0.5f, 0.0f, 0.0f);// Up vector

	// Print helpful information to the console
	printf("scale, eyesx, eyesy, eyesz, rotate_x, rotate_y: %f %f %f %f %f %f \n", scale_percentage, eyes_x, eyes_y, eyes_z, rotate_x, rotate_y);
	
	glBegin(GL_QUADS);
	set_colour(-1);
	glVertex3f(1.9f, 1.9f, 0.0f);
	glVertex3f(2.0f, 1.9f, 0.0f);
	glVertex3f(2.0f, 2.0f, 0.0f);
	glVertex3f(1.9f, 2.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	set_colour(-1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.1f, 0.0f);
	glVertex3f(0.0f, 0.1f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	set_colour(1);
	glVertex3f(0.0f, 0.1f, 0.0f);
	glVertex3f(0.1f, 0.1f, 0.0f);
	glVertex3f(0.1f, 0.2f, 0.0f);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	set_colour(2);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glVertex3f(0.1f, 0.2f, 0.0f);
	glVertex3f(0.1f, 0.3f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glEnd();
	
	glBegin(GL_QUADS);
	set_colour(3);
	glVertex3f(0.8f, 0.2f, 0.0f);
	glVertex3f(0.9f, 0.2f, 0.0f);
	glVertex3f(0.9f, 0.3f, 0.0f);
	glVertex3f(0.8f, 0.3f, 0.0f);
	glEnd();
	
	glBegin(GL_QUADS);
	set_colour(4);
	glVertex3f(0.9f, 0.2f, 0.0f);
	glVertex3f(1.0f, 0.2f, 0.0f);
	glVertex3f(1.0f, 0.3f, 0.0f);
	glVertex3f(0.9f, 0.3f, 0.0f);
	glEnd();
	
	glBegin(GL_QUADS);
	set_colour(5);
	glVertex3f(0.9f, 0.9f, 0.0f);
	glVertex3f(1.0f, 0.9f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.9f, 1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	set_colour(0);
	glVertex3f(-0.9f, -0.9f, 0.0f);
	glVertex3f(-1.0f, -0.9f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-0.9f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	set_colour(1);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {

	// Initialize GLUT and process user parameters
	glutInit(&argc, argv);

	// Request float buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// Create window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	glutCreateWindow("Agent Simulation");

	// Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	// Callback functions
	glutDisplayFunc(display2);
	//glutSpecialFunc(special_keys);

	// Pass control to GLUT for events
	glutMainLoop();

	return 0;
}