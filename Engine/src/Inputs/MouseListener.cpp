#include "MouseListener.h"
#include <iostream>

geProject::MouseListener* geProject::MouseListener::instance = nullptr;
/* docs from https ://www.glfw.org/docs/3.3/input_guide.html */

geProject::MouseListener::MouseListener() : xPos(0), yPos(0), xPrev(0), yPrev(0), xScroll(0), yScroll(0), windowWidth(0), windowHeight(0) {
    eventSystem.subscribe(this, &MouseListener::updateViewPort);
    eventSystem.subscribe(this, &MouseListener::updateProjection);
}

geProject::MouseListener::~MouseListener()
{
}

geProject::MouseListener* geProject::MouseListener::getInstance() {
    //PRE: IF THERE IS A SINGLETON INSTANCE AVAILABLE
    if (instance == NULL)
    {
        instance = new MouseListener();
        return instance;
    }
    else
    {
        return instance;
    }
}

void geProject::MouseListener::cursor_position_callback(GLFWwindow* window, double newX, double newY) {
    Context currentContext = eventSystem.getContext();
    eventSystem.publish(new MouseMoveEvent(currentContext, MouseListener::getInstance()->getCameraMouseX(), MouseListener::getInstance()->getCameraMouseY()));
    MouseListener::getInstance()->xPrev = MouseListener::getInstance()->xPos;
    MouseListener::getInstance()->yPrev = MouseListener::getInstance()->yPos;
    MouseListener::getInstance()->xPos = newX;
    MouseListener::getInstance()->yPos = newY;

}

void geProject::MouseListener::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    //PRE: IF THERE IS A VALID BUTTON PRESS
    Context currentContext = eventSystem.getContext();
    if (action == GLFW_PRESS) {
        eventSystem.publish(new MouseButtonEvent(currentContext, button, GLFW_PRESS, MouseListener::getInstance()->getCameraMouseX(), MouseListener::getInstance()->getCameraMouseY()));
        geProject::MouseListener::getInstance()->mouseButton[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        eventSystem.publish(new MouseButtonEvent(currentContext, button, GLFW_RELEASE, MouseListener::getInstance()->getCameraMouseX(), MouseListener::getInstance()->getCameraMouseY()));
        geProject::MouseListener::getInstance()->mouseButton[button] = false;
        geProject::MouseListener::getInstance()->isDragging = false;
    }
}

void geProject::MouseListener::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //only allow mouse scrolling in and out in the editor
    
    eventSystem.publishImmediately(new MouseScrollEvent(EditorContext, xoffset, yoffset, MouseListener::getInstance()->getScreenXpos(), MouseListener::getInstance()->getScreenYpos()));
    MouseListener::getInstance()->xScroll = xoffset;
    MouseListener::getInstance()->yScroll = yoffset;
    
}

void geProject::MouseListener::endFrame() {
    MouseListener::getInstance()->yScroll = 0;
    MouseListener::getInstance()->xScroll = 0;
    MouseListener::getInstance()->xPrev = MouseListener::getInstance()->xPos;
    MouseListener::getInstance()->yPrev = MouseListener::getInstance()->yPos;
    eventSystem.handleEvents(Type::gameSave);
    eventSystem.handleEvents(Type::gameLoad);
    eventSystem.handleEvents(Type::gameStart);
    eventSystem.handleEvents(Type::gameStop);
}

float geProject::MouseListener::getXpos() { return (float)MouseListener::getInstance()->xPos; };
float geProject::MouseListener::getYpos() { return (float)MouseListener::getInstance()->yPos; };
float geProject::MouseListener::getXprev() { return (float)MouseListener::getInstance()->xPrev; };
float geProject::MouseListener::getYprev() { return (float)MouseListener::getInstance()->yPrev; };
float geProject::MouseListener::getXscroll() { return (float)MouseListener::getInstance()->xScroll; };
float geProject::MouseListener::getYscroll() { return (float)MouseListener::getInstance()->yScroll; };
float geProject::MouseListener::getXdiff() { return (float)(MouseListener::getInstance()->xPos - MouseListener::getInstance()->xPrev); };
float geProject::MouseListener::getYdiff() { return (float)(MouseListener::getInstance()->yPos - MouseListener::getInstance()->yPrev); };
bool geProject::MouseListener::isDrag() { return geProject::MouseListener::getInstance()->isDragging; };



float geProject::MouseListener::getCameraMouseX() {
    float currentX = MouseListener::getInstance()->getXpos() - MouseListener::getInstance()->viewPos.x;
    glm::mat4 test1 = MouseListener::getInstance()->viewInv;
    glm::mat4 test2 = MouseListener::getInstance()->projectionInv;
    currentX = (currentX / MouseListener::getInstance()->viewSize.x) * 2.0f - 1.0f;
    glm::vec4 tmp = glm::vec4(currentX, 0, 0, 1);
    glm::vec4 worldCoordX = MouseListener::getInstance()->viewInv * MouseListener::getInstance()->projectionInv * tmp;
    return worldCoordX.x;


}

float geProject::MouseListener::getCameraMouseY() {
    float currentY = MouseListener::getInstance()->viewPos.y - MouseListener::getInstance()->getYpos();
    currentY = ((currentY / MouseListener::getInstance()->viewSize.y) * 2.0f - 1.0f);
    glm::vec4 tmp = glm::vec4(0, currentY, 0, 1);
    glm::vec4  worldCoordY = MouseListener::getInstance()->viewInv * MouseListener::getInstance()->projectionInv * tmp;
    return worldCoordY.y;
}

float geProject::MouseListener::getScreenXpos() {
    float x = (xPos - viewPos.x) / viewSize.x * 1920.0f;
    return x;


}


float geProject::MouseListener::getScreenYpos() {
    float y = (viewPos.y - yPos) / viewSize.y * 1080.0f;
    return y;

}

float geProject::MouseListener::getViewXsize() { return (float)geProject::MouseListener::getInstance()->viewSize.x; }
float geProject::MouseListener::getViewYsize() { return (float)geProject::MouseListener::getInstance()->viewSize.y; }

bool geProject::MouseListener::checkMouseBoundaries() {
    float x = MouseListener::getInstance()->getScreenXpos();
    float y = MouseListener::getInstance()->getScreenYpos();
    
    if (x >= 0.0f && x <= 1920.0f && y >= 0.0f && y <= 1080.0f) {
        return true;
    }
    return false;
}



bool geProject::MouseListener::mouseButtonDown(int button) { return MouseListener::getInstance()->mouseButton[button]; };
void geProject::MouseListener::releaseMouseButton(int button) { MouseListener::getInstance()->mouseButton[button] = false; }


void geProject::MouseListener::setWindowDimensions(int windowW, int windowH) {
    MouseListener::getInstance()->windowWidth = windowW;
    MouseListener::getInstance()->windowHeight = windowH;
}


void geProject::MouseListener::setInverses(glm::mat4 inverseProj, glm::mat4 inverseView) {
    MouseListener::getInstance()->projectionInv = inverseProj;
    MouseListener::getInstance()->viewInv = inverseView;
}



bool geProject::MouseListener::isMouseButtonDown(int button) { return mouseButton[button]; }


//EVENT LISTENERS

void geProject::MouseListener::updateViewPort(ViewPortEvent* e) {
    MouseListener::getInstance()->viewPos.x = e->windowPosX;
    MouseListener::getInstance()->viewPos.y = e->windowPosY;
    MouseListener::getInstance()->viewSize.x = e->windowSizeX;
    MouseListener::getInstance()->viewSize.y = e->windowSizeY;
    delete(e);    
}


void geProject::MouseListener::updateProjection(ProjectionEvent* e) {
    MouseListener::getInstance()->projectionInv = e->projInv;
    MouseListener::getInstance()->viewInv = e->viewInv;
    delete(e);
}