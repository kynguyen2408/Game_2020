#pragma once

class Scene
{
public:

	virtual void LoadResources() {};
	virtual void Update(float dt) {};
	virtual void Render() {};

};
