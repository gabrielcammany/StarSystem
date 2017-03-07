
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <cmath> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//include GLUT, GLEW and GLM - see below
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <string> 
#include <vector> 


class Camera
{
private:

	float rotational_speed;
	glm::vec3 cameraPos;
	glm::vec3 oldCameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::mat4 rotateMatrix;
	float cameraSpeed;
	bool lock;
	bool clipping;
	
public:

	Camera();		

	void Camera::changeClipping();

	void addRotationalSpeed();

	void takeRotationalSpeed();

	void mouseUpdate(glm::vec2 mouseDelta);

	float getRotationSpeed();

	void changeLock();

	void cameraPostoOld();

	void cameraOldtoPos();

	void moveRight();

	void moveLeft();

	void moveForward();

	void moveBackwards();

	bool isClipping();

	void setClipping(bool c);

	bool isLock();

	void setLock(bool l);

	void setCameraPos(glm::vec3 camera);

	glm::vec3 getCameraPos();

	void setCameraFront(glm::vec3 camera);

	glm::vec3 getCameraFront();

	void setCameraUp(glm::vec3 camera);

	glm::vec3 getCameraUp();

	void setOldCameraPos(glm::vec3 camera);

	glm::vec3 getOldCameraPos();

	void addSpeed();

	void lesSpeed();

	void setCameraSpeed(float speed);

	float getCameraSpeed();

	void moveUp();

	void moveDown();

};


