#include "zmq_generator_worker_collector.hpp"

/******************************************************************************/

#include <functional>
#include <iostream>
#include <map>
#include <unistd.h>

#include <boost/lexical_cast.hpp>

/******************************************************************************/

void generator()
{
	start_generator( 5555, []( std::ostream & out ) {
		static size_t count = 0;
		std::cout << "Sending: " << count << std::endl;
		out << count << std::endl;
		++count;
		sleep(1);
	});
}

/******************************************************************************/

void worker()
{
	start_worker( "localhost", 5555, 5556, []( std::istream & in, std::ostream & out ) {
		static auto pid = getpid();
		size_t count;
		in >> count;
		out << "PID:" << pid << " -- Result:" << count << std::endl;
	});
}

/******************************************************************************/

void collector()
{
	start_collector( 5556, []( std::istream & in ) {
		std::string line;
		std::getline( in, line );
		std::cout << "Finished Processing: " << line << std::endl;
	});
}

/******************************************************************************/

std::map< std::string, std::function< void() > > calls_ = {
	{"--gen",       generator},
	{"--generator", generator},
	{"--work",      worker},
	{"--worker",    worker},
	{"--coll",      collector},
	{"--collector", collector},
};

/******************************************************************************/

int usage()
{
	std::cout << "Usage:" << std::endl;
	for( auto keyval : calls_ ) {
		std::cout << '\t' << keyval.first << std::endl;
	}
}

/******************************************************************************/

int main(int argc, char const * argv[])
{
	if (argc < 2)
		return usage();
	
	calls_[argv[1]]();
}

/******************************************************************************/
