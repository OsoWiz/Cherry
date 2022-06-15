
#include "engine.h"
#ifdef _DEBUG
bool debug = true;
#else
bool debug = false;
#endif // DEBUG


namespace cherry{

	/// <summary>
	/// Initializes the engine systems.
	/// </summary>
	void Engine::initialize() {
		if (debug)
			std::cout << "printit mintis" << std::endl;
		this->graphicsSystem.run();
		
	}
	



}