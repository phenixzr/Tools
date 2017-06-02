#ifndef UUID_H
#define UUID_H

#include <cstdint>
#include <random>
#include <iostream>

namespace tools
{
#define UniqueId std::size_t

	class UuidGenerator
	{
	public:	
		~UuidGenerator()
		{
		}

		static UuidGenerator& getInstance()
		{
			static UuidGenerator instance;
			return instance;
		}

        UniqueId getUuid()
		{
			return _distrution(_gen);
		}

	private:
		UuidGenerator()
		{
			_gen.seed(_rd());
		}

		UuidGenerator(const UuidGenerator&) { };

		UuidGenerator& operator=(const UuidGenerator&) { return *this; };

		std::mt19937_64 _gen;
		std::random_device _rd;
		std::uniform_int_distribution<UniqueId> _distrution;

	};



} // ns tools
#endif