#include "EditorCamera.h"
#include <iostream>

geProject::EditorCamera::EditorCamera(glm::vec2 pos){
	position = glm::vec3(pos, 0.0f);
	eventSystem.subscribe(this, &EditorCamera::cameraMouseButton);
	eventSystem.subscribe(this, &EditorCamera::cameraMouseScrolled);
	eventSystem.subscribe(this, &EditorCamera::mouseMoved);
	eventSystem.subscribe(this, &EditorCamera::updateKeyPress);
	projectionUpdate();
}

void geProject::EditorCamera::update(float dt) {
	deltaTime = dt;
	eventSystem.handleEvents(Type::mousePressed);
	eventSystem.handleEvents(Type::mouseMove);
	eventSystem.handleEvents(Type::mouseScroll);
	eventSystem.handleEvents(Type::keyPressed);		
}



void geProject::EditorCamera::updateKeyPress(KeyPressedEvent* e){	
	if (e->pressed) {
		glm::vec2 newPos(0,0);
		float modifier = 0.1f;
		switch (e->keycode) {
		case GLFW_KEY_W:
			newPos.y = -(modifier * scroll);
			setPosition(newPos);
			//position[1] += modifier * scroll;
			break;

		case GLFW_KEY_A:
			newPos.x = modifier * scroll;
			setPosition(newPos);
			//position[0] -= modifier * scroll;
			break;

		case GLFW_KEY_S:
			newPos.y = modifier * scroll;
			setPosition(newPos);
			//position[1] -= modifier * scroll;
			break;

		case GLFW_KEY_D:
			newPos.x = -(modifier * scroll);
			setPosition(newPos);
			//position[0] += modifier * scroll;
			break;
		}
	}
}


void geProject::EditorCamera::cameraMouseButton(MouseButtonEvent* mouse){		
	if (mouse->mouseButton == GLFW_MOUSE_BUTTON_MIDDLE && mouse->mouseButtonDown == GLFW_PRESS) {			
		mouseDown = true;
	}
	else if (GLFW_MOUSE_BUTTON_MIDDLE && mouse->mouseButtonDown == GLFW_RELEASE) {
		mouseDown = false;
	}	
}


void geProject::EditorCamera::mouseMoved(MouseMoveEvent* mouse) {
	//std::cout << "Mouse X: " << drag << std::endl;	
	if (mouseDown) {		
		if (prevClick[0] == 0 && prevClick[1] == 0) {
			prevClick[0] = mouse->posX;
			prevClick[1] =  mouse->posY;
			drag -= deltaTime;
		}		
		drag -= deltaTime;
		glm::vec2 cameraPos = glm::vec2(mouse->posX, mouse->posY);
		cameraPos = (prevClick - cameraPos) * (deltaTime * 1.2f);
		//interpolate between origin of drag and the new camera position
		prevClick[0] = prevClick[0] * (1.0 - deltaTime) + (mouse->posX * deltaTime);
		prevClick[1] = prevClick[1] * (1.0 - deltaTime) + (mouse->posY * deltaTime);
		setPosition(cameraPos);
		//std::cout << "cameraPosX : " << cameraPos[0] << " cameraPosY: " << cameraPos[1] << std::endl;
		if (drag <= 0 && !mouseDown) {
			drag = 0.1f;
		}

	}


}

void geProject::EditorCamera::cameraMouseScrolled(MouseScrollEvent* scroll){
	//std::cout << "Context: " << scroll->getContexts() << " Type: " << scroll->getType() << std::endl;
	auto scrollval = scroll->yScroll;
	if (scrollval != 0.0f && (scroll->screenX >= 0.0f && scroll->screenX <= 1920.0f) && (scroll->screenY >= 0.0f && scroll->screenY <= 1080.0f)) {		
		int signum = (0 < scrollval) - (scrollval < 0);
		setScroll(pow(abs(scrollval), 1 / getScroll()) * 0.2f * -signum);
	}

}












