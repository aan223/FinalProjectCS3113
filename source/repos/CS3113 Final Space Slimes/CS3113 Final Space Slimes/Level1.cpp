#include "Level1.h"
#define LEVEL1_WIDTH 15 
#define LEVEL1_HEIGHT 8

#define ENEMY_COUNT 4


unsigned int level1_data[] = { 
	9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,   
	3, 7, 7, 7, 7, 7, 7, 6, 7, 7, 7, 7, 7, 9, 9,
	3, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 5, 9,
	3, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 5, 9,
	9, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 9, 9,
	3, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 10, 11, 5, 9,
	3, 8, 8, 8, 8, 8, 10, 11, 10, 11, 8, 8, 8, 5, 9,
	9, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 9, 9
};

void Level1::Initialize() {


	state.player = new Entity();
	if (!complete) {
		state.player->position = glm::vec3(7.0f, -4.0f, 0);
	}
	else {//the main should handle this but in case it doesnt work I'll just set some default 
		state.player->position = glm::vec3(1.0f, -4.0f, 0);
	}
	state.player->movement = glm::vec3(0);
	//state.player->acceleration = glm::vec3(0, -9.81f, 0);
	state.player->speed = 1.75f;
	state.player->textureID = Util::LoadTexture("Space Slime Player Sprite Sheet.png");

	state.player->entityType = PLAYER;

	state.player->animRight = new int[2]{ 5, 6 };
	state.player->animLeft = new int[2]{ 1, 2 };
	state.player->animUp = new int[2]{ 9, 10 };
	state.player->animDown = new int[2]{ 13, 14 };
	state.player->animIdle = new int[4]{ 3, 7,11,15 };
	state.player->height = 0.95f;
	state.player->width = 0.7f;


	state.player->animFrames = 2;
	state.player->animIndices = state.player->animIdle;
	state.player->animCols = 4;
	state.player->animRows = 4;

	
	
	//set up Enemy unit
	
	if (!complete) {//starting off with 3 basic slimes 
		state.enemies = new Entity[ENEMY_COUNT];
		for (int k = 0; k < ENEMY_COUNT; k++) {
			//Technical 
			//state.enemies[k].acceleration = glm::vec3(0, -9.81f, 0);
			state.enemies[k].position = glm::vec3(0);
			state.enemies[k].movement = glm::vec3(0);
			state.enemies[k].entityType = ENEMY;
			state.enemies[k].speed = 1.0f;
			state.enemies[k].height = 0.9f;
			state.enemies[k].width = 0.8f;

			//Graphics
			state.enemies[k].textureID = Util::LoadTexture("Space Slimes.png");
			state.enemies[k].animIdle = new int[2]{ 9, 8 };
			state.enemies[k].animIndices = state.enemies[k].animIdle;
			state.enemies[k].animFrames = 2;
			state.enemies[k].animCols = 4;
			state.enemies[k].animRows = 3;
		}

		state.enemies[0].position = glm::vec3(6.0f, -6.0f, 0);
		state.enemies[0].enemyType = SLEEPER;

		state.enemies[1].position = glm::vec3(9.5f, -3.0f, 0);
		state.enemies[1].enemyType = SLEEPER;

		state.enemies[2].position = glm::vec3(12.0f, -5.0f, 0);
		state.enemies[2].enemyType = SLEEPER;

		state.enemies[3].position = glm::vec3(1.0f, -2.0f, 0);
		state.enemies[3].enemyType = SLEEPER;

	}
	
	else {
		state.enemies = new Entity[1];
		state.enemies[0].position = glm::vec3(8, -3.0f, 0);
		state.enemies[0].movement = glm::vec3(0);
		state.enemies[0].speed = 1.0f;
		state.enemies[0].textureID = Util::LoadTexture("Space Slime Boss.png");


		state.enemies[0].entityType = ENEMY;
		state.enemies[0].enemyType = BOSS;

		state.enemies[0].animRight = new int[2]{ 7, 5 };
		state.enemies[0].animLeft = new int[2]{ 6, 4 };
		state.enemies[0].animUp = new int[2]{ 0, 1 };
		state.enemies[0].animDown = new int[2]{ 2, 3 };
		state.enemies[0].height = 0.95f;
		state.enemies[0].width = 1.0f;


		state.enemies[0].animFrames = 2;
		state.enemies[0].animIndices = state.player->animDown;
		state.enemies[0].animCols = 3;
		state.enemies[0].animRows = 3;

		state.enemies[0].inFirstRoom = true;
		state.enemies[0].killable = true;
	}
	state.lives = new Entity[3];
	for (int i = 0; i < 3; i++) {
		state.lives[i].textureID = Util::LoadTexture("pixel heart.png");
		state.lives[i].entityType = LIVES;
		state.lives[i].position = glm::vec3((1.0f + i*0.75f), -0.5f, 0);
		state.lives[i].Update(0, state.player, state.enemies, 0, state.map);
	
	}



	GLuint mapTextureID = Util::LoadTexture("Space Slimes Background.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 3, 4);
	state.nextScene = -1;
}
void Level1::Update(float deltaTime) {
	int enemyCount = 0;
	if (!complete) {
		for (int i = 0; i < ENEMY_COUNT; i++) {
			state.enemies[i].Update(deltaTime, state.player, state.player, 1, state.map);
			if (!state.enemies[i].isActive) {
				enemyCount++;
			}
			if (enemyCount == ENEMY_COUNT) {
				complete = true;
			}
		}
	}

	else {
		for (int i = 0; i < 1; i++) {
			state.enemies[i].Update(deltaTime, state.player, state.player, 1, state.map);
		}
	}

	if (state.player->recover) {
		state.player->Update(deltaTime, state.player, state.enemies, 0, state.map);
	}
	else {
		state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	}
	if (state.player-> recover) {
		state.player ->timer += deltaTime;
		if (state.player ->timer >= (deltaTime * 10)) {
			state.player->recover = false;
			state.player ->timer = 0;
			state.enemies[0].position = glm::vec3(6.0f, -5.0f, 0);
			state.enemies[1].position = glm::vec3(9.5f, -4.0f, 0);
		}
	}

	//for moving the hearts:
	for (int j = 0; j < state.player->hp; j++) {
		if (state.player->position.x > 8.5) {
			state.lives[j].position = glm::vec3((4.25f + j * 0.75f), -0.5f, 0);
		}
		else if (state.player->position.x > 5) {
			state.lives[j].position = glm::vec3(((state.player->position.x) - 4.25f) + (j * 0.75f), -0.5f, 0);
		}
		else {
			state.lives[j].position = glm::vec3((0.75f + j * 0.75f), -0.5f, 0);
		}
		state.lives[j].Update(0, state.player, state.enemies, 0, state.map);
	}

	if (state.player->position.x <= 0.0f ) {//you have to kill all the enemies before leaving the level
		state.nextScene = 2;
	}
	else if (state.player->position.x >= 12.5f) {//you have to kill all the enemies before leaving the level
		state.nextScene = 3;
	}
	else if (state.player->position.y >= -0.5f) {//you have to kill all the enemies before leaving the level
		state.nextScene = 4;
	}
	else if (state.player->position.y <= -6.5f) {//you have to kill all the enemies before leaving the level
		state.nextScene = 5;
	}
}
void Level1::Render(ShaderProgram *program) { 
	state.map->Render(program);
	if (!state.player->fail) {
		if (!complete) {
			for (int i = 0; i < ENEMY_COUNT; i++) {
				state.enemies[i].Render(program);
			}
		}
		else {
			for (int i = 0; i < 1; i++) {
				state.enemies[i].Render(program);
			}
		}
		for (int j = 0; j < state.player->hp; j++) {
			state.lives[j].Render(program);
		}
		state.player->Render(program);
	}
	else {
		glm::vec3 fontPos2;
		glm::vec3 fontPos3;
		
		if (state.player->position.x > 8.5) {
			fontPos2 = glm::vec3(6.25f, -3.0f, 0);
			fontPos3 = glm::vec3(6.0f, -2.0f, 0);
		}
		else if (state.player->position.x > 5) {
			fontPos2 = glm::vec3(state.player->position.x - 1.5f, -3.0f, 0);
			fontPos3 = glm::vec3(state.player->position.x - 1.75f, -2.0f, 0);
		}
		else {
			fontPos2 = glm::vec3(2.75f, -3.0f, 0);
			fontPos3 = glm::vec3(2.5f, -2.0f, 0);
		}

		GLuint fontTextureID = Util::LoadTexture("pixel_font.png");
		Util::DrawText(program, fontTextureID, "Game Over!", .3f, 0.1f, fontPos3);
		Util::DrawText(program, fontTextureID, "Try Again!", .2f, 0.1f, fontPos2);
	}
	
}