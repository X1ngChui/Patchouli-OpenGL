#include "Patchouli.h"

class Spell : public Pache::Application
{
public:
	Spell() = default;
	~Spell() = default;
};

Pache::Application* Pache::createApplication()
{
	return new Spell;
}