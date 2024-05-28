#include <stdlib.h>
#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "math.h"
#include <iostream>
#include <cctype>

#include "../utils/figure.hpp"
#include "config.hpp"
#include "../utils/matrix.hpp"


#define NOW ((1.0f*glutGet(GLUT_ELAPSED_TIME))/1000.0f)

#define RED 1.0f,0.0f,0.0f
#define GREEN 0.0f,1.0f,0.0f
#define BLUE 0.0f,0.0f,1.0f
#define YELLOW 1.0f,1.0f,0.0f
#define CYAN 0.0f,1.0f,1.0f
#define WHITE 1.0f,1.0f,1.0f

#define SPHERICAL true
#define FREE false

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

bool cameraMode;

int mode = GL_FILL;

Config* c;
std::vector<Figure*> stored_figures;

// vbos
// modelos
GLuint* buffers = NULL; 
vector<unsigned int> buffersSizes;
unsigned int figCount = 0; 
// normais
GLuint* buffersN = NULL;
vector<unsigned int> buffersNSizes; 
// texturas
GLuint* buffersTC = NULL;
GLuint* textures = NULL;

bool freeze = false;
int timeOld, timeNew;
float totalTime = 0.0f;

float init_time = 0.0f;
int timebase;

unsigned int frame = 0;
char title[128];

bool showCurves = false;
bool showNormais = false;
bool show_eixos = false;

void loadTexture(const char* texturePath, int* index) {
	FILE* file_ptr = fopen(texturePath, "r");
	if (file_ptr == NULL) {
		printf("Não foi possível abrir o ficheiro da textura\n", texturePath);
		exit(1);
	}
	fclose(file_ptr);
	unsigned int t, tw, th;
	unsigned char* texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)texturePath);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1, textures + *index);

	glBindTexture(GL_TEXTURE_2D, *(textures + *index));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
}

// Carrega os dados das figuras para os buffers.
void loadBuffersData(int* index) {
	for (Figure* f : stored_figures) {
		vector<float> figure_points_vec = f->to_vector();
		vector<float> figure_normals_vec = f->get_normals_vector();
		vector<float> figure_texture_coords_vec = f->get_texture_coords_vector();
		const char* texture_file = f->texture_file.c_str();

		// Points
		glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure_points_vec.size(), figure_points_vec.data(), GL_STATIC_DRAW);
		buffersSizes.push_back(figure_points_vec.size() / 3);

		// Normals
		glBindBuffer(GL_ARRAY_BUFFER, buffersN[*index]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure_normals_vec.size(), figure_normals_vec.data(), GL_STATIC_DRAW);
		buffersNSizes.push_back(figure_normals_vec.size() / 3);

		if (texture_file != nullptr && *texture_file != '\0') {
			glBindBuffer(GL_ARRAY_BUFFER, buffersTC[*index]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * figure_texture_coords_vec.size(), figure_texture_coords_vec.data(), GL_STATIC_DRAW);
			loadTexture(texture_file, index);
		}

		(*index)++;
	}
}

void drawEixos() {
	if (show_eixos) {
		if (c->lights.size() > 0) glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		// X axis in red
		glColor3f(RED);
		// glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(GREEN);
		// glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(BLUE);
		// glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
		glEnd();
		if (c->lights.size() > 0) glEnable(GL_LIGHTING);
	}
}

void drawCatmullRomCurve(vector<Point> controlPoints) {
	if (c->lights.size() > 0) glDisable(GL_LIGHTING);
	Point* pos = new Point();
	glBegin(GL_LINE_LOOP);
	float t = 0.0f;
	for (int i = 0; i <= 100; i++, t += 0.01f) {
		get_global_catmull_rom_point(t, controlPoints, pos, NULL);
		glVertex3f(pos->x, pos->y, pos->z);
	}
	delete pos;
	glEnd();
	if (c->lights.size() > 0) glEnable(GL_LIGHTING);
}

void passTime() {
	timeOld = timeNew;
	timeNew = glutGet(GLUT_ELAPSED_TIME);
	if (!freeze) {
		totalTime += 1.0f * (timeNew - timeOld) / 1000.0f;
	}
}

void drawNormals(Figure* model) {
	if (c->lights.size() > 0) glDisable(GL_LIGHTING);
	glColor3f(WHITE);
	vector<float> normals = model->get_normals_vector();
	vector<float> vertexs = model->to_vector();
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < normals.size(); i += 3) {
		glVertex3f(vertexs[i + 0], vertexs[i + 1], vertexs[i + 2]);
		glVertex3f(vertexs[i + 0] + normals[i + 0], vertexs[i + 1] + normals[i + 1], vertexs[i + 2] + normals[i + 2]);
	}
	glEnd();
	if (c->lights.size() > 0) glEnable(GL_LIGHTING);
}

// deleted no fim do programa
Point* pos = new Point();
Point* deriv = new Point();

void drawGroups(Group* group, int* index) {
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
				}
				else {
					glRotatef(t->angle, t->x, t->y, t->z);
				}
				break;
			case Transform::TransformType::TRANSLATE_ANIMATION:
				if (t->time && t->time > 0.0f) {

					get_global_catmull_rom_point(NOW / t->time, t->points, pos, deriv);
					if (showCurves) drawCatmullRomCurve(t->points);
					glTranslatef(pos->x, pos->y, pos->z);

					if (t->align) {
						deriv->normalize();
						Point* z = Point::cross(deriv, t->y_axis);
						z->normalize();
						Point* y = Point::cross(z, deriv);
						y->normalize();

						float rotation_matrix[16];
						build_rotation_matrix(deriv, y, z, rotation_matrix);
						glMultMatrixf(rotation_matrix);

						t->y_axis->x = y->x;
						t->y_axis->y = y->y;
						t->y_axis->z = y->z;

						delete y;
						delete z;
					}

				}
				break;
			default:
				break;
			}
		}

		for (unsigned long i = 0; i < group->models.size(); i++, (*index)++) {
			Figure* f = stored_figures.at(*index);
			if (showNormais) drawNormals(f);

			glMaterialfv(GL_FRONT, GL_DIFFUSE, f->get_diffuse().data());
			glMaterialfv(GL_FRONT, GL_AMBIENT, f->get_ambient().data());
			glMaterialfv(GL_FRONT, GL_SPECULAR, f->get_specular().data());
			glMaterialfv(GL_FRONT, GL_EMISSION, f->get_emissive().data());
			glMaterialf(GL_FRONT, GL_SHININESS, f->get_shininess());

			// Texturas
			const char* texture_file = f->texture_file.c_str();
			if (texture_file != nullptr && *texture_file != '\0') {
				glBindTexture(GL_TEXTURE_2D, textures[*index]);
				glBindBuffer(GL_ARRAY_BUFFER, buffersTC[*index]);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);
			}
			// Normais
			glBindBuffer(GL_ARRAY_BUFFER, buffersN[*index]);
			glNormalPointer(GL_FLOAT, 0, 0);
			// Vértices/pontos
			glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, buffersSizes[*index]);

			// Fazer unbind da textura
			if (texture_file != nullptr && *texture_file != '\0') {
				glBindTexture(GL_TEXTURE_2D, 0);
			}

		}

		for (Group* g : group->groups) {
			drawGroups(g, index);
		}

		glPopMatrix();
	}
}

int gl_light(int i) {
	switch (i) {
	case 0: return GL_LIGHT0;
	case 1: return GL_LIGHT1;
	case 2: return GL_LIGHT2;
	case 3: return GL_LIGHT3;
	case 4: return GL_LIGHT4;
	case 5: return GL_LIGHT5;
	case 6: return GL_LIGHT6;
	case 7: return GL_LIGHT7;
	}
	return -1;
}

void execute_lights() {

	for (unsigned int i = 0; i < c->lights.size(); i++) {

		const auto& light_ptr = c->lights.at(i);
		int CLight = gl_light(i);

		switch (light_ptr->get_type()) {
		case Light::Type::Point: {
			PointLight* point_light = dynamic_cast<PointLight*>(light_ptr.get());
			if (point_light) {
				glLightfv(CLight, GL_POSITION, point_light->get_pos().data());
			}
			break;
		}
		case Light::Type::Directional: {
			DirectionalLight* directional_light = dynamic_cast<DirectionalLight*>(light_ptr.get());
			if (directional_light) {
				glLightfv(CLight, GL_POSITION, directional_light->get_dir().data());
			}
			break;
		}
		case Light::Type::Spotlight: {
			SpotLight* spot_light = dynamic_cast<SpotLight*>(light_ptr.get());
			if (spot_light) {
				glLightfv(CLight, GL_POSITION, spot_light->get_pos().data());
				glLightfv(CLight, GL_SPOT_DIRECTION, spot_light->get_dir().data());
				glLightf(CLight, GL_SPOT_CUTOFF, spot_light->get_cutoff());
				glLightf(CLight, GL_SPOT_EXPONENT, 0.0);
			}
			break;
		}
		default:
			std::cerr << "Unknown light type!" << std::endl;
		}
	}
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

void sphericalCamera() {
	camx = radius * cos(beta_) * sin(alpha);
	camy = radius * sin(beta_);
	camz = radius * cos(beta_) * cos(alpha);
}

void cross(const float* a, const float* b, float* res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float* a) {
	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

// Deslocações da câmara em modo FREE
void walkLeft() {
	float v_up[3] = { upx,upy,upz }, v_r[3], v_d[3] = { lookAtx - camx,lookAty - camy,lookAtz - camz };
	cross(v_d, v_up, v_r); normalize(v_r); // r = d * up
	normalize(v_d);
	float k = -1.0f; // velocidade de deslocação
	camx += k * v_r[0]; camy += k * v_r[1]; camz += k * v_r[2];
	lookAtx = camx + v_d[0]; lookAty = camy + v_d[1]; lookAtz = camz + v_d[2];
}

void walkRight() {
	float v_up[3] = { upx,upy,upz }, v_r[3], v_d[3] = { lookAtx - camx,lookAty - camy,lookAtz - camz };
	cross(v_d, v_up, v_r); normalize(v_r); // r = d * up
	normalize(v_d);
	float k = 1.0f; // velocidade de deslocação
	camx += k * v_r[0]; camy += k * v_r[1]; camz += k * v_r[2];
	lookAtx = camx + v_d[0]; lookAty = camy + v_d[1]; lookAtz = camz + v_d[2];
}

void walkForward() {
	float v_d[3] = { lookAtx - camx,lookAty - camy,lookAtz - camz };
	normalize(v_d);
	float k = 1.0f;
	camx += k * v_d[0]; camy += k * v_d[1]; camz += k * v_d[2];
	lookAtx = camx + v_d[0]; lookAty = camy + v_d[1]; lookAtz = camz + v_d[2];
}

void walkBackwards() {
	float v_d[3] = { lookAtx - camx,lookAty - camy,lookAtz - camz };
	normalize(v_d);
	float k = -1.0f;
	camx += k * v_d[0]; camy += k * v_d[1]; camz += k * v_d[2];
	lookAtx = camx + v_d[0]; lookAty = camy + v_d[1]; lookAtz = camz + v_d[2];
}

void moveHead() {
	lookAtx = camx + radius * cos(beta_) * sin(alpha);
	lookAty = camy + radius * sin(beta_);
	lookAtz = camz + radius * cos(beta_) * cos(alpha);
}
// Fim das deslocações da câmara em modo FREE

void renderScene(void) {
	passTime();
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	if (cameraMode == SPHERICAL)
		sphericalCamera();
	gluLookAt(camx, camy, camz, lookAtx, lookAty, lookAtz, upx, upy, upz);

	drawEixos();
	// figuras
	glColor3f(WHITE);
	execute_lights();
	glPolygonMode(GL_FRONT_AND_BACK, mode);

	int index = 0; // serve para seleccionar o buffer que vai ser lido
	drawGroups(c->group, &index);

	frame++;
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		float fps = frame * 1000.0f / (time - timebase);
		snprintf(title, 127, "FPS: %.2f, PCAM: (%.2f,%.2f,%.2f), CamMode: %s", fps, camx, camy, camz, cameraMode ? "SPH" : "FREE");
		glutSetWindowTitle(title);
		timebase = time;
		frame = 0;
	}

	glutSwapBuffers();
}

void specKeyProc(int key_code, int x, int y) {
	x = y; y = x;
	switch (key_code) {
	case GLUT_KEY_F1: {
		freeze = !freeze;
		break;
	}

	case GLUT_KEY_UP: {
		if (cameraMode == SPHERICAL) {
			radius -= 1.0f;
		}
		else {
			beta_ += 0.1f;
			moveHead();
		}
		break;
	}

	case GLUT_KEY_DOWN: {
		if (cameraMode == SPHERICAL) {
			radius += 1.0f;
		}
		else {
			beta_ -= 0.1f;
			moveHead();
		}
		break;
	}

	case GLUT_KEY_LEFT: {
		if (cameraMode == FREE) {
			alpha += 0.1f;
			moveHead();
		}
		break;
	}

	case GLUT_KEY_RIGHT: {
		if (cameraMode == FREE) {
			alpha -= 0.1f;
			moveHead();
		}
		break;
	}

	default:
		break;
	}
	glutPostRedisplay();
}

void keyProc(unsigned char key, int x, int y) {
	x = y; y = x; 
	switch (key) {

	case 'n': {
		showNormais = !showNormais;
		break;
	}

	case 'a': { // left
		if (cameraMode == SPHERICAL) {
			alpha -= 0.1f;
		}
		else {
			walkLeft();
		}
		break;
	}

	case 'd': { // right
		if (cameraMode == SPHERICAL) {
			alpha += 0.1f;
		}
		else {
			walkRight();
		}
		break;
	}

	case 'w': { // up 
		if (cameraMode == SPHERICAL) {
			beta_ += beta_ <= 1.48f ? 0.1f : 0.0f;
		}
		else {
			walkForward();
		}
		break;
	}

	case 's': { // down
		if (cameraMode == SPHERICAL) {
			beta_ -= beta_ >= -1.48f ? 0.1f : 0.0f;
		}
		else {
			walkBackwards();
		}
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

	case('x'):
		show_eixos = !show_eixos;
		break;

	case 'c':
		showCurves = !showCurves;
		break;

	case 'v': {
		if (cameraMode == FREE) {
			radius = sqrt(camx * camx + camy * camy + camz * camz);
			lookAtx = lookAty = lookAtz = 0.0f;
			beta_ = asin(camy / radius);
			alpha = atan(camx / camz);
			sphericalCamera();
			cameraMode = SPHERICAL;
		}
		else { // cameraMode == SPHERICAL
			float v_d[3] = { lookAtx - camx,lookAty - camy,lookAtz - camz }; normalize(v_d);
			float aux[3] = { -camx, 0.0f, -camz };
			alpha = PI + alpha;
			beta_ = -beta_;
			lookAtx = camx + cos(beta_) * sin(alpha);
			lookAty = camy + sin(beta_);
			lookAtz = camz + cos(beta_) * cos(alpha);
			cameraMode = FREE;
		}
		break;
	}

	case '+': {
		if (cameraMode == FREE) {
			camy += 1.0f;
			lookAty += 1.0f;
		}
		break;
	}

	case '-': {
		if (cameraMode == FREE) {
			camy -= 1.0f;
			lookAty -= 1.0f;
		}
		break;
	}
	default:
		break;
	}
	glutPostRedisplay();
}

void init() {
	glewInit();
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	if (c->lights.size() > 0) {
		glEnable(GL_LIGHTING);
		glEnable(GL_RESCALE_NORMAL);
		if (c->lights.size() > 8) {
			printf("Número de luzes definidas superior a 8\n");
			exit(1);
		}
		GLfloat white[4] = { 1.0,1.0,1.0,1.0 };
		for (unsigned int i = 0; i < c->lights.size(); i++) {
			glEnable(gl_light(i));
			glLightfv(gl_light(i), GL_DIFFUSE, white);
			glLightfv(gl_light(i), GL_SPECULAR, white);
		}

		float amb[4] = { 1.0f, 1.0f, 1.0f, 0.4f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	}

	// OpenGL settings
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPolygonMode(GL_FRONT, GL_LINE);
	// Cria os buffers
	glGenBuffers(figCount, buffers);
	glGenBuffers(figCount, buffersN);
	glGenBuffers(figCount, buffersTC);
	// Carrega os dados para os buffers
	int index = 0; // serve para seleccionar o buffer que vai ser escrito
	loadBuffersData(&index);
	// Cálculo do tempo
	timebase = glutGet(GLUT_ELAPSED_TIME);
	init_time = NOW;
	timeOld = timeNew = 0;
}

void print_lights() {
	for (const auto& light_ptr : c->lights) {

		switch (light_ptr->get_type()) {
		case Light::Type::Point: {
			PointLight* point_light = dynamic_cast<PointLight*>(light_ptr.get());
			std::cout << point_light->to_string() << std::endl;
			break;
		}
		case Light::Type::Directional: {
			DirectionalLight* directional_light = dynamic_cast<DirectionalLight*>(light_ptr.get());
			std::cout << directional_light->to_string() << std::endl;
			break;
		}
		case Light::Type::Spotlight: {
			SpotLight* spot_light = dynamic_cast<SpotLight*>(light_ptr.get());
			std::cout << spot_light->to_string() << std::endl;
			break;
		}
		default:
			std::cerr << "Unknown light type!" << std::endl;
		}
	}
}

void getFigures(Group* g) {
	if (g) {
		for (ModelInfo* model : g->models) {
			Figure* f = Figure::from_file(model->model_file);
			f->texture_file = model->texture_file;

			f->set_diffuse(model->color[0][0], model->color[0][1], model->color[0][2]);
			f->set_ambient(model->color[1][0], model->color[1][1], model->color[1][2]);
			f->set_specular(model->color[2][0], model->color[2][1], model->color[2][2]);
			f->set_emissive(model->color[3][0], model->color[3][1], model->color[3][2]);
			f->set_shininess(model->shininess);

			stored_figures.push_back(f);
			std::cout << f->to_string() << std::endl;
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
	figCount = stored_figures.size();

	print_lights();

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
	
	alpha = (camx == 0 && camz == 0) ? 0.0f : (camx < 0 ? -1.0f : 1.0f) * acos(camz / sqrt(camx * camx + camz * camz));
	beta_ = asin(camy / radius);

	cameraMode = SPHERICAL;
	buffers = (GLuint*)calloc(figCount, sizeof(GLuint)); 
	buffersN = (GLuint*)calloc(figCount, sizeof(GLuint));
	buffersTC = (GLuint*)calloc(figCount, sizeof(GLuint));
	textures = (GLuint*)calloc(figCount, sizeof(GLuint));

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Fase 4");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// put here the registration of the keyboard callbacks (por enquanto só mexem na camara como forma de debug)
	glutKeyboardFunc(keyProc);
	glutSpecialFunc(specKeyProc);

	init();

	// enter GLUT's main cycle
	glutMainLoop();

	delete c;
	delete deriv;
	delete pos;

	return 0;
}