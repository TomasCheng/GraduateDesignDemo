#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/fwd.hpp>
using namespace glm;

//����࣬�ƶ���ʽ�ο�u3d
class Camera
{
private:
	//λ��
	vec3 position;
	//Ŀ��
	vec3 target;
	//�ұ�
	vec3 right;
	//�ϱ�
	vec3 up;

	//����
	vec3 dir;

public:
	//��Ҫ����Ĺ۲����
	mat4 ViewMat4;


public:
	Camera(const vec3 &pos, const vec3 &tar);
	~Camera();

	inline void UpdateView(){ViewMat4 = lookAt(position, target, up);}

	void translate(vec3 move);
	void setPosition(vec3 pos);
	void setTarget(vec3 tar);

};

