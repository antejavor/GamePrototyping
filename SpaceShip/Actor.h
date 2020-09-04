#pragma once
#include <vector>
#include "Math.h"

class Actor
{
public:

	enum  class State {
		EActive,
		EPaused,
		EDead
	};

	//Constructor and dependecy injection
	Actor(class Game* game);
	virtual ~Actor();

	//Called by Game class, not overridable
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);

	//Actor-specific update, overridable
	virtual void UpdateActor(float deltaTime);

	
	// Getters/setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }
	
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:

	State mState;
	Vector2 mPosition;
	float mScale; // Scale 1.0f = 100%
	float mRotation; //Radiants

	//Actor components
	std::vector<class Component*> mComponents;
	class Game* mGame;

};

