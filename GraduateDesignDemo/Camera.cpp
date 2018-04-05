#include "Camera.h"



Camera::Camera(const vec3 &pos, const vec3 &tar)
{
	position = pos;
	target = tar;
	dir = normalize(position - target);
	up = vec3(0.0f, 1.0f, 0.0f);
	right = normalize(cross(up, dir));
	UpdateView();
}


Camera::~Camera()
{
}

void Camera::translate(vec3 move)
{
//	move = ViewMat4 * vec4(move,1.0f);
	position += move;
	UpdateView();
}

void Camera::setPosition(vec3 pos)
{
	position = pos;
	UpdateView();
}

void Camera::setTarget(vec3 tar)
{
	target = tar;
	UpdateView();
}
