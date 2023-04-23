#include "Camera.h"

namespace MiniRenderer
{
	Camera::Camera(Vec3f position, Vec3f up, float yaw, float pitch)
		: Front(0.0f, 0.0f, -1.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(0.0f, 0.0f, -1.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
	{
		Position = Vec3f(posX, posY, posZ);
		WorldUp = Vec3f(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVectors();
	}

	void Camera::ProcessKeyInput(CameraMovementDirection direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;

		switch (direction)
		{
			case MiniRenderer::CameraMovementDirection::Forward:  Position += Front * velocity; break;
			case MiniRenderer::CameraMovementDirection::Backward: Position -= Front * velocity; break;
			case MiniRenderer::CameraMovementDirection::Left:	  Position += Right * velocity; break;
			case MiniRenderer::CameraMovementDirection::Right:	  Position -= Right * velocity; break;
			default: break;
		}
	}

	void Camera::ProcessMouseInput(float xoffset, float yoffset)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		Pitch = Clamp(Pitch, -89.0f, 89.0f);

		// update Front, Right and Up Vectors using the updated Euler angles
		UpdateCameraVectors();
	}

	Mat4 Camera::GetViewMatrix()
	{
		return LookAt(Position, Position + Front, Up);
	}

	void Camera::UpdateCameraVectors()
	{
		// calculate the new Front vector
		Vec3f front;

		float yawinRadians = ToRadians(Yaw), pitchinRadians = ToRadians(Pitch);

		front.x = Cos(yawinRadians) * Cos(pitchinRadians);
		front.y = Sin(pitchinRadians);
		front.z = Sin(yawinRadians) * Cos(pitchinRadians);
		Front = front.normalize();
		// also re-calculate the Right and Up vector
		Right = Normalize(Cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = Normalize(Cross(Right, Front));
	}
}
