#include "Patchouli.h"
#include "Spell2D.h"

namespace Spell
{
	class Spell : public Pache::Application
	{
	public:
		Spell()
		{
			pushLayer(new Spell2D);
		}

		~Spell() = default;
	};
}

Pache::Application* Pache::createApplication()
{
	return new Spell::Spell;
}