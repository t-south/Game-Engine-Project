#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../src/Memory/PoolAllocator.h"
#include <unordered_map>
#include <vector>
#include <string>

namespace geProject {

	enum dmgTypes {
		SPECIAL = 0x0001,
		FIRE = 0x0002,
		WATER = 0x0004,
		EARTH = 0x0008,
		LIGHTNING = 0x0010
	};

	enum AgentStates {
		AT_HOME = 0x00001,
		PATROLLED = 0x00002,
		HAS_ENERGY = 0x00004,
		ALERT = 0x00008,
		AGENT_ANGRY = 0x00010,
		AGENT_TIRED = 0x00020,
		AGENT_HURT = 0x00040,
		ATTACK_SIGHTED = 0x00080,
		INVESTIGATED = 0x00100,
		ENEMY_DEAD = 0x00200,
		ENEMY_VISIBLE = 0x00400,
		COOLDOWN = 0x08000
	};


	struct Transform {
		unsigned int id = 0x0001;
		std::string name{"object"};
		glm::vec2 position{ 0 };
		glm::vec2 scale{ 0 };
		float rotation{ 0};	
		glm::vec3 dirtyFlag{0,0,-1 };
	};

	struct SpriteRender {
		unsigned int id = 0x0002;
		unsigned int entityId;
		glm::vec4 color{ 1, 1, 1, 1};
		glm::vec2 texturePos[4] = { glm::vec2{1.0f, 1.0f}, glm::vec2{1.0f, 0.0f}, glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 1.0f} };
		GLuint textureId{ 0 };
		unsigned int spriteSheetId{ 0 }; // 0 == no spritesheet
		unsigned int zIndex {0}; 		
	};

	struct Rigidbody {
		unsigned int id = 0x0004;
		int collider{ 0 };
		float friction{ 0.8f };
		float angularDamping{0.0f};
		float linearDamping{ 0.0f };
		float density{ 0.0f };
		bool fixedRotate{ false };
		bool bullet{ false };		
		unsigned int bodyType{ 1 }; // 0 -> kinematic ||| 1 -> dynamic ||| 2 -> static
		glm::vec3 velocity{ 0, 0.5, 0 };	
	};

	struct CircleCollider {
		unsigned int id = 0x0008;
		int entityAssigned;
		float radius{ 0.0f };
		glm::vec2 offset{0, 0};
		int entityType{ 0 };
		int colliders{ 0 };
		bool sensor{ false };
	};

	struct BoxCollider {
		unsigned int id = 0x0010;		
		int entityAssigned;
		glm::vec2 boxSize{0.25f, 0.25f};
		glm::vec2 offset{ 0 };
		glm::vec2 origin{0,0};
		int entityType{0};
		int colliders{0};
		bool sensor{ false };
	};

	struct Animation {
		unsigned int id = 0x0020;		
		float speed{0.8f};
		int currentFrame{ 0 };
		bool loopAnimation{ true };
		std::string state;
	};

	struct Controls {
		unsigned int id = 0x0040;
		float runspeed{ 1.0f };
		float speedModifier;
		float direction;
	};

	struct Health {
		unsigned int id = 0x0080;
		int maxHealth{ 100 };
		int currentHealth{ 100 };
		bool invincible{ false };
	};
	

	struct Damage {
		unsigned int id = 0x0100;
		int dmgAtk{ 1 };
		float dmgModifier{ 1 };
		int coolDown;
		int dmgType{dmgTypes::FIRE};
	};
	

	struct ViewCollider {
		unsigned int id = 0x0200;
		int entityAssigned;
		float radius{ 0.0f };
		glm::vec2 offset;
		int entityType{ 0 };
		int colliders{ 0 };
		bool sensor{ false };
	};

	struct Agent {
		unsigned int id = 0x0400;
		int agentStateDetails{AT_HOME | HAS_ENERGY | COOLDOWN}; //keeps track of the current agents state based on a bitmask
		int goalPriority;  //context for goal priorities  COMBAT - 0 -> LIFESTYLE - 1 -> DUTY - 2
		bool playerInRange{ false };
		float alertLevel{ 0 };
		float energy{ 10 };
		glm::vec2 anomalypos;
	};

}




