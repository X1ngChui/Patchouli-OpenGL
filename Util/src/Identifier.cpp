#include "Identifier.h"

namespace Pache
{
	Identifier::EntryAllocator* Identifier::EntryAllocator::instance = new Identifier::EntryAllocator;
}