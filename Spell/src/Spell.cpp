#include "Patchouli.h"

class ExampleLayer : public Pache::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer")
	{
	}

	void onUpdate() override
	{
		Pache::Log::coreInfo("ExampleLayer updated");
	}

	void onEvent(Pache::Event& e) override
	{
		Pache::Log::coreInfo("ExampleLayer received {}", e);
	}
};

class Spell : public Pache::Application
{
public:
	Spell()
	{
		ExampleLayer* exampleLayer = new ExampleLayer;
		pushOverlay(exampleLayer);
	}

	~Spell() = default;
};

Pache::Application* Pache::createApplication()
{
	return new Spell;
}