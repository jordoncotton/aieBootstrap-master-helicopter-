#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

mat4 world(1);
mat4 helicopter_base_rot(1);
mat4 helicopter_base_trans(1);

mat4 propeller_base_rot(1);
mat4 propeller_base_trans(1);
mat4 propeller_rotor_rot(1);
mat4 propeller_rotor_trans(1);

mat4 rotor_base_rot(1);
mat4 rotor_base_trans(1);   


Application3D::Application3D() 
{

}

Application3D::~Application3D() 
{

}

glm::mat4 helicopter_base = world * helicopter_base_trans * helicopter_base_rot;
glm::mat4 base_rotor = helicopter_base * propeller_base_trans * propeller_base_rot;
glm::mat4 cylinder_rotor = base_rotor * propeller_rotor_trans * propeller_base_rot;
glm::mat4

bool Application3D::startup() 
{
	//helicopter
	helicopter_base_rot = glm::rotate(helicopter_base, 0.0f, vec3(0, 1, 0));
	helicopter_base_trans = glm::translate(helicopter_base, vec3(0, 1, 0));

	//propeller
	propeller_base_rot = glm::rotate(cylinder_rotor, 0.0f, vec3(0, 1, 0));
	propeller_base_trans = glm::translate(cylinder_rotor, vec3(0, 1, 0));

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	return true;
}

void Application3D::shutdown() 
{
	Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{
	// query time since application started
	float time = getTime();
	time = 0;

	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 20, glm::cos(time) * 20),
							   vec3(0), vec3(0, 1, 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes

	Gizmos::addSphere(vec3(0, 0, 0), 2, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addCylinderFilled(vec3(2), 0.3f, 1, 5, vec4(0, 1, 1, 1));
	Gizmos::addCylinderFilled(vec3(1), 0.5f, 3, 5, vec4(0, 1, 1, 1));

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

}

void Application3D::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}