#pragma once
#include <ge_engine/Components.h>
#include <fstream>
#include <iostream>
#include <ge_engine/json.hpp>
#include "../Inputs/KeyboardListener.h"
#include "../Inputs/MouseListener.h"
#include "../Windows/Window.h"
#include "../Render/Renderer.h"
#include "../Render/FrameBuffer.h"
#include "../Physics/Physics.h"
#include "../Animation/AnimationManager.h"
#include "../Controller/Command.h"
#include "../Controller/PlayerController.h"
#include "../Controller/Receiver.h"
#include "../EntityManager/EntityManager.h"
#include <ge_engine/PhysicsManager.h>
#include "../AI/Enemy.h"
//#include <ge_engine/Managers.h>



using json = nlohmann::json;
namespace geProject {
	class Scene {
	public:
		virtual void update(float deltaTime) = 0;
		virtual void init() = 0;	
		virtual void setPicking() = 0;
		virtual void updateSceneImgui() {};
		virtual void setActiveEntity(int entityId) {};
		void setWindow(Window* window);
		void setMouseListener();
		void setKeyboardListener();
		float getMouseX();
		float getMouseY();
		MouseListener* getMouseListener();
		void reloadLevel();
		std::string getFilePath();
		void serialize(std::string filepath);
		void deserialize(std::string filepath);
		void setPhysics(bool check);	
		void setGoapStatus(bool goap);
	protected:	
		void render(Camera& camera, std::string shaderPath);
		static Window* gameWindow;
		static MouseListener* mouse;
		static KeyboardListener* keyboard;
		//MANAGERS
		static AnimationManager* animationManager;
		static Receiver* controlManager;		
		static Renderer* rendermanager;
		std::unordered_map<int, Enemy*> enemies;
		FrameBuffer* selectionTextures{ nullptr };
		//(de)serialisation
		std::string filePath;
		std::ofstream oFile;		
		bool physicsEnabled{ false };
		void renderScene();
		bool goapEnabled{ false };
		json serializeEntity(Entity& entity);
		//overrides for serialising to/from json
		void to_json(json& data, Animation& comp);		
		void to_json(json& data, SpriteRender& comp);
		void to_json(json& data, Transform& comp);
		void to_json(json& data, Rigidbody& comp);
		void to_json(json& data, CircleCollider& comp);
		void to_json(json& data, BoxCollider& comp);
		void to_json(json& data, Controls& comp);
		void to_json(json& data, Health& comp);
		void to_json(json& data, Damage& comp);
		void to_json(json& data, ViewCollider& comp);

		void from_json(json& data, Animation& comp);
		void from_json(json& data, SpriteRender& comp);
		void from_json(json& data, Transform& comp);
		void from_json(json& data, Rigidbody& comp);
		void from_json(json& data, CircleCollider& comp);
		void from_json(json& data, BoxCollider& comp);
		void from_json(json& data, Controls& comp);
		void from_json(json& data, Health& comp);
		void from_json(json& data, Damage& comp);
		void from_json(json& data, ViewCollider& comp);
		//event listeners
		virtual void saveGame(GameSaveEvent* save) = 0;
	};
}