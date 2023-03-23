#include "Camera.h"


geProject::Camera::~Camera(){}

glm::mat4 geProject::Camera::getViewMatrix(){	
	view = glm::lookAt(position, position + direction, up);
	viewInv = glm::inverse(view);
	return view; 
}

void geProject::Camera::projectionUpdate() {	
	projection = glm::mat4(1.0f);		
	projection = glm::ortho(0.0f, projSize[0] * scroll, 0.0f, projSize[1] * scroll);
	projInv = glm::inverse(projection);	
}


glm::mat4 geProject::Camera::getProjection() {
	return projection;
}
glm::mat4 geProject::Camera::getViewMatrixInverse() { 
	glm::mat4 view = getViewMatrix();
	return viewInv; 
}
glm::mat4 geProject::Camera::getProjectionInverse() {
	//projectionUpdate();
	return projInv; 
}




glm::vec2 geProject::Camera::getProjSize() { return projSize; }

glm::vec2 geProject::Camera::getPosition() {
	projectionUpdate();
	return glm::vec2(position[0], position[1]);
}

void geProject::Camera::setPosition(glm::vec2 pos) { 
	position -= glm::vec3(pos, 0);
	projectionUpdate();
	eventSystem.publishImmediately(new ProjectionEvent(projInv, viewInv));
}

void geProject::Camera::setScroll(float scale) { scroll += scale; }
float geProject::Camera::getScroll() { return scroll; }

