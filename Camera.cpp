/* 
 * File:   Camera.cpp
 * Author: dude719
 * 
 * Created on April 19, 2015, 6:35 AM
 */

#include "Camera.h"
#include "utils.h"
#include "shareddefs.h"

Camera::Camera(int width, int height) 
	: position(0.0f, 0.0f, 1.0f),
	yaw(0.0f),
	pitch(0.0f),
	maxAngle(89.0f),
	fov(70.0f),
	nearZ(0.1f),
	farZ(10000.0f),
	aspectRatio((float)width/(float)height)
{
}

void Camera::Init(int width, int height)
{
	position = glm::vec3(0.0f, 0.0f, 1.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	maxAngle = 89.0f;
	fov = 70.0f;
	nearZ = 0.1f;
	farZ = 10000.0f;
	aspectRatio = ((float)width/(float)height);

}

void Camera::SetNearAndFarPlanes(float _nearPlane, float _farPlane)
{
	nearZ = _nearPlane;
	farZ = _farPlane;
}

void Camera::LookAt(glm::vec3 pos) 
{
	if (pos == position)
		return;
	glm::vec3 direction = glm::normalize(pos - position);
	yaw = glm::radians(asinf(-direction.y));
	pitch = -glm::radians(atan2f(-direction.x, -direction.z));
}

void Camera::SetFoV(float _fov)
{
	fov = _fov;
}

float Camera::GetFoV()
{
	return fov;
}

void Camera::OffsetPosition(const glm::vec3& offset) 
{
	position += offset;
}

void Camera::SetPosition(const glm::vec3& pos)
{
	position = pos;
}

glm::vec3 Camera::Position()
{
	return position;
}

glm::vec3 Camera::ViewVector()
{
	glm::vec3 v(1,0,0);
	v = glm::rotateY(v, -pitch); // rotate pitch along -y
	v = glm::rotateZ(v, yaw); // rotate yaw along z
	return v;
}

glm::vec2 Camera::ViewAngles()
{
	return glm::vec2(yaw, pitch);
}

void Camera::OffsetOrientation(float pitchAngle, float yawAngle)
{
	yaw += yawAngle;
	pitch += pitchAngle;
	NormalizeAngles();
}

void Camera::SetOrientation(const glm::vec3& angles)
{
	yaw = angles.x;
	pitch = angles.y;
	NormalizeAngles();
}

glm::mat4 Camera::Orientation()
{
	glm::mat4 orientation;
	orientation = glm::rotate(orientation, glm::radians(pitch - 90.0f), glm::vec3(1,0,0));
	orientation = glm::rotate(orientation, glm::radians(yaw + 90.0f), glm::vec3(0,0,1));
	return orientation;
}

float Camera::Yaw()
{
	return yaw;
}

float Camera::Pitch()
{
	return pitch;
}

glm::vec3 Camera::Forward()
{
	glm::vec4 forward = glm::inverse(Orientation()) * glm::vec4(0,0,-1,1);
	return glm::vec3(forward);
}

glm::vec3 Camera::Right()
{
	glm::vec4 right = glm::inverse(Orientation()) * glm::vec4(1,0,0,1);
	return glm::vec3(right);
}

glm::vec3 Camera::Up() 
{
	glm::vec4 up = glm::inverse(Orientation()) * glm::vec4(0,1,0,1);
	return glm::vec3(up);
}

glm::mat4 Camera::Matrix()
{
	return Projection() * View();
}

glm::mat4 Camera::Projection()
{
	return glm::perspective(glm::radians(fov), aspectRatio, nearZ, farZ);
}

glm::mat4 Camera::View()
{
	return Orientation() * glm::translate(glm::mat4(), -position);
}

bool Camera::WorldToScreen(const glm::vec3& worldPos, int& x, int& y)
{
	glm::vec4 posHomogeneous(worldPos.x, worldPos.y, worldPos.z, 1.0f);
	glm::vec4 posInClipSpace = Projection() * (View() * posHomogeneous);
	glm::vec3 posInNDC(posInClipSpace.x / posInClipSpace.w, posInClipSpace.y / posInClipSpace.w, posInClipSpace.z / posInClipSpace.w);

	// Check if the point is in the camera's view.
	if( posInClipSpace.w > 0.0f && 
		posInNDC.x >= -1.0f && posInNDC.x <= 1.0f &&
		posInNDC.y >= -1.0f && posInNDC.y <= 1.0f &&
		posInNDC.z >=  0.0f && posInNDC.z <= 1.0f )
	{
		x = (int)Utils::RemapFloat(posInNDC.x, -1.0f, 1.0f, 0.0f, (float)WINDOW_WIDTH);
		y = (int)Utils::RemapFloat(posInNDC.y, -1.0f, 1.0f, (float)WINDOW_HEIGHT, 0.0f);
		return true;
	}

	return false;
}

void Camera::NormalizeAngles() 
{
	yaw = fmodf(yaw, 360);
	if(yaw < 0.0f)
		yaw += 360;
	if(pitch > maxAngle)
		pitch = maxAngle;
	else if(pitch < -maxAngle)
		pitch = -maxAngle;
}

