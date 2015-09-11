/* 
 * File:   Camera.h
 * Author: dude719
 *
 * Created on April 19, 2015, 6:34 AM
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


class Camera
{
public:
	Camera() 
	{
	}

	Camera(int width, int height);

	void		Init(int width, int height);

	void		SetFoV(float FoV);
	float		GetFoV();

	void		SetNearAndFarPlanes(float _nearPlane, float _farPlane);

	void		LookAt(glm::vec3 pos);
	
	glm::vec3	ViewVector();
	glm::vec2	ViewAngles();

	void		OffsetOrientation(float upAngle, float rightAngle);
	void		SetOrientation(const glm::vec3& angles);
	glm::mat4	Orientation();

	float		Yaw();
	float		Pitch();
	
	void		OffsetPosition(const glm::vec3& offset);
	void		SetPosition(const glm::vec3& pos);
	glm::vec3	Position();
	
	glm::vec3	Forward();
	glm::vec3	Right();
	glm::vec3	Up();
	glm::mat4	Matrix();
	glm::mat4	Projection();
	glm::mat4	View();
    
	void		NormalizeAngles();
    bool        WorldToScreen(const glm::vec3& worldPos, int& x, int& y);
    
private:
	glm::vec3 position;

	float nearZ;
	float farZ;
	float aspectRatio;

	float fov; // Field of View
	
	float pitch;
	float yaw;
	
	float maxAngle; // Maximum vertical angle.
};

#endif	/* CAMERA_H */

