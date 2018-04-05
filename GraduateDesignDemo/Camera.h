#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/fwd.hpp>
using namespace glm;

//相机类，移动方式参考u3d
class Camera
{
private:
	//位置
	vec3 position;
	//目标
	vec3 target;
	//右边
	vec3 right;
	//上边
	vec3 up;

	//方向
	vec3 dir;

public:
	//所要输出的观察矩阵
	mat4 ViewMat4;


public:
	Camera(const vec3 &pos, const vec3 &tar);
	~Camera();

	inline void UpdateView(){ViewMat4 = lookAt(position, target, up);}

	void translate(vec3 move);
	void setPosition(vec3 pos);
	void setTarget(vec3 tar);

};

