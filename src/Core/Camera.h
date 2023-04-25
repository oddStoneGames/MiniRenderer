// A Simple Fly Camera

#ifndef CAMERA_H
#define CAMERA_H

#include "Maths/Maths.h"

namespace MiniRenderer
{
	enum class CameraMovementDirection
	{
		Forward, Backward, Left, Right
	};
	
	// Default camera values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 0.01f;
	const float SENSITIVITY = 0.3f;

	class Camera
	{
	public:
		Camera(Vec3f position = Vec3f(0.0f, 0.0f, 0.0f), Vec3f up = Vec3f(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		void ProcessKeyInput(CameraMovementDirection direction, float deltaTime);
		void ProcessMouseInput(float xoffset, float yoffset);
		Mat4 GetViewMatrix();
	public:
		// Attributes
		Vec3f Position;
		Vec3f Front, Up, Right;
		Vec3f WorldUp;

		// Euler Angles
		float Yaw, Pitch;

		// Options
		float MovementSpeed, MouseSensitivity;
	private:
		// Calculates the Front Vector from the updated Euler Angles.
		void UpdateCameraVectors();
	};
}

#endif // !CAMERA_H

