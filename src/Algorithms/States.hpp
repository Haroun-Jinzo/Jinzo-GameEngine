#pragma once
 

namespace States
{
	/*
		states = 10001101
	*/

	//check state
	template<typename T>

	bool isActive(T* states, int target)
	{
		/*
			target = 3
			1 = 00000001
			1 << target = 00001000

			10001101 &
			00001000
			--------
			00001000

			00001000 = 8 = 1 << target
		*/
		return (*states & (1 << target)) == (1 << target);
	}

	//activate state
	template<typename T>
	void Activate(T* states, int target)
	{
		*states |= 1 << target;
	}

	//uniquely activate state ( no others can be active)
	template<typename T>
	void UniquelyActivate(T* states, int target)
	{
		Activate<T>(states, target) // activate all states 
		*states &= 1 << target; //the deactivate others
	}

	// deactivate states
	template<typename T>
	void Deactivate(T* states, int target)
	{
		*states &= (1 << target);
	}

	//toggle state
	template<typename T>
	void Toggle(T* states, int target)
	{
		*states ^= 1 << target;
	}
}