#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "math.h"
#include <iostream>

#include "../utils/figure.hpp"
#include "config.hpp"
#include "../utils/matrix.hpp"

#define NOW ((1.0f*glutGet(GLUT_ELAPSED_TIME))/1000.0f)

#define RED 1.0f,0.0f,0.0f
#define GREEN 0.0f,1.0f,0.0f
#define BLUE 0.0f,0.0f,1.0f
#define YELLOW 1.0f, 1.0f, 0.0f
#define CYAN 0.0f, 1.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f

float alphaRoll = 0.0f;
float alpha = PI / 4;
float beta_ = PI / 4;
float radius = 5.0f;
float camx = 5.0f;
float camy = 5.0f;
float camz = 5.0f;
float lookAtx = 0.0f;
float lookAty = 0.0f;
float lookAtz = 0.0f;
float upx = 0.0f;
float upy = 0.0f;
float upz = 0.0f;
float fov = 0.0f;
float near = 0.0f;
float far = 0.0f;

float view_w = 800;
float view_h = 800;

int mode = GL_LINE;

float init_time = 0.0f;
int timebase;
unsigned int frame = 0;
char title[128];
bool show_curves = true;

Config* c;
std::vector<Figure*> stored_figures;

GLuint* buffers = NULL;
vector<unsigned int> buffer_sizes;
unsigned int figure_count = 0;

void startOrthoView() {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, view_w, 0, view_h, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
}

void endOrthoView() {

	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(fov, ratio, near, far);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawText(char* string) {
	startOrthoView();
	int l = 1;
	glColor3f(1, 1, 1);
	glRasterPos2i(0, view_h - l * 20);
	void* font = GLUT_BITMAP_HELVETICA_18;
	for (char* c = string; *c != '\0'; c++) {
		if (*c == '\n') {
			glRasterPos2i(0, view_h - (++l * 20));
		}
		glutBitmapCharacter(font, *c);
	}
	endOrthoView();
}

//void drawFiguras(std::vector<Figure *> figures) {
//	for (Figure* f : figures) {
//		for (Point p : f->points) {
//			glVertex3d(p.x, p.y, p.z);
//		}
//	}
//}

void drawCatmullRomCurve(vector<Point> controlPoints) {
	Point* pos = new Point();
	glBegin(GL_LINE_LOOP);
	float t = 0.0f;
	for (int i = 0; i <= 100; i++, t += 0.01f) {
		get_global_catmull_rom_point(t, controlPoints, pos, NULL);
		glVertex3f(pos->x, pos->y, pos->z);
	}
	glEnd();
}

void drawGroups(Group* group, int *index) {
	if (group) {
		glPushMatrix();
		glColor3f(WHITE);

		for (Transform* t : group->transforms) {
			switch (t->type)
			{
			case Transform::TransformType::TRANSLATE:
				glTranslatef(t->x, t->y, t->z);
				break;
			case Transform::TransformType::SCALE:
				glScalef(t->x, t->y, t->z);
				break;
			case Transform::TransformType::ROTATE:
				glRotatef(t->angle, t->x, t->y, t->z);
				break;
			case Transform::TransformType::ROTATE_ANIMATION:
				if (t->time > 0.0f) {
					glRotatef((((NOW - init_time) * 360.0f) / t->time), t->x, t->y, t->z);
				} else {
					glRotatef(t->angle, t->x, t->y, t->z);
				}
				break;
			case Transform::TransformType::TRANSLATE_ANIMATION:
				if (t->time && t->time > 0.0f) {

					Point* pos = new Point(); 
					Point* deriv = new Point();
					get_global_catmull_rom_point(NOW / t->time, t->points, pos, deriv);
					if (show_curves) drawCatmullRomCurve(t->points);
					glTranslatef(pos->x, pos->y, pos->z);

					if (t->align) {
						deriv->normalize();
						Point* z = Point::cross(deriv, t->y_axis);
						z->normalize();
						Point* y = Point::cross(z, deriv);
						t->y_axis = y;
						y->normalize();

						float rotation_matrix[16];
						build_rotation_matrix(deriv, y, z, rotation_matrix);
						glMultMatrixf(rotation_matrix);
					}
				}
				break;
			default:
				break;
			}
		}

		for (unsigned long i = 0; i < group->models.size(); i++, (*index)++) {
			glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, buffer_sizes[*index]);
		}

		for (Group* g : group->groups) {
			drawGroups(g, index);
		}

		frame++;
		int time = glutGet(GLUT_ELAPSED_TIME);
		if (time - timebase > 1000) {
			float fps = frame * 1000.0f / (time - timebase);
			snprintf(title, 127, "Fase 3 - FPS: %.2f", fps);
			glutSetWindowTitle(title);
			timebase = time;
			frame = 0;
		}

		glPopMatrix();
	}
}

void loadBuffersData(int* index) {
	for (Figure* f : stored_figures) {
		vector<float> figure_buffer = f->to_vector();
		glBindBuffer(GL_ARRAY_BUFFER, buffers[(*index)++]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure_buffer.size(), figure_buffer.data(), GL_STATIC_DRAW);
		buffer_sizes.push_back(figure_buffer.size() / 3);
	}
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(radius * cos(beta_) * sin(alpha), radius * sin(beta_), radius * cos(beta_) * cos(alpha),
		lookAtx, lookAty, lookAtz,
		upx, upy, upz);

	// put drawing instructions here
	// linhas dos eixos
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, mode);

	// groups
	int index = 0;
	drawGroups(c->group, &index);
	
	
	//Texto 
	//drawText("HELLO"); // debug, depois criar funcao para criar a string

	// End of frame
	glutSwapBuffers();
}

// altera a posi��o da camera, para debug.
void specKeyProc(int key_code, int x, int y) {
	x = y; y = x;
	switch (key_code) {
	case GLUT_KEY_UP: {
		radius -= 2.0f;
		break;
	}

	case GLUT_KEY_DOWN: {
		radius += 2.0f;
		break;
	}
	case GLUT_KEY_LEFT: {
		alphaRoll -= 0.05f;
		upx = sin(alphaRoll);
		upy = cos(alphaRoll);
		break;
	}
	case GLUT_KEY_RIGHT: {
		alphaRoll += 0.05;
		upx = sin(alphaRoll);
		upy = cos(alphaRoll);
		break;
	}
	default:
		break;
	}

	glutPostRedisplay();
}

// Alters the camera position for debug and changes between modes (GL_FILL, GL_LINES, GL_POINT)
void keyProc(unsigned char key, int x, int y) {
	x = y; y = x;
	switch (key)
	{
	case 'a': { // left
		alpha -= 0.1f;
		break;
	}

	case 'd': { // right
		alpha += 0.1f;
		break;
	}

	case 'w': { // up 
		beta_ += beta_ <= 1.48f ? 0.3f : 0.0f;
		break;
	}

	case 's': { // down
		beta_ -= beta_ >= -1.48f ? 0.3f : 0.0f;
		break;
	}

	case('f'):
		mode = GL_FILL;
		break;

	case('l'):
		mode = GL_LINE;
		break;

	case('p'):
		mode = GL_POINT;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}


void getFigures(Group* g) {
	if (g) {
		for (const auto& model : g->models) {
			stored_figures.push_back(Figure::from_file(model));
		}
		for (const auto& group : g->groups) {
			getFigures(group);
		}
	}
}


int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
		return 1;
	}

	c = new Config(argv[1]);
	getFigures(c->group);
	figure_count = stored_figures.size();

	camx = c->get_x_pos_cam();
	camy = c->get_y_pos_cam();
	camz = c->get_z_pos_cam();

	radius = sqrt(camx * camx + camy * camy + camz * camz);

	lookAtx = c->get_x_look_at();
	lookAty = c->get_y_look_at();
	lookAtz = c->get_z_look_at();

	upx = c->get_x_up();
	upy = c->get_y_up();
	upz = c->get_z_up();

	fov = c->get_fov();
	near = c->get_near();
	far = c->get_far();

	alpha = acos(camz / sqrt(camx * camx + camz * camz));
	beta_ = asin(camy / radius);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Fase 3");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// put here the registration of the keyboard callbacks (por enquanto s� mexem na camara como forma de debug)
	glutKeyboardFunc(keyProc);
	glutSpecialFunc(specKeyProc);

	// OpenGL settings
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_VERTEX_ARRAY);

	buffers = (GLuint*)calloc(figure_count, sizeof(GLuint));
	glGenBuffers(figure_count, buffers);
	int index = 0;
	loadBuffersData(&index);

	timebase = glutGet(GLUT_ELAPSED_TIME);
	init_time = NOW;

	// enter GLUT's main cycle
	glutMainLoop();

	delete c;

	return 1;
}