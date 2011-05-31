#pragma once

/******************************************************************************/

#include <sstream>
#include <functional>

#include <zmq.hpp>

#include "zmq_gwc_helpers.hpp"

/******************************************************************************/

void start_generator( int gport, std::function< void(std::ostream &) > create_request );
void start_collector( int cport, std::function< void(std::istream &) > process_result );
void start_worker( std::string server_addr, int gport, int cport, std::function< void(std::istream &, std::ostream &) > process_work );

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

void start_generator( int gport, std::function< void(std::ostream &) > create_request )
{
	zmq::context_t context(1);
	zmq::socket_t sender(context, ZMQ_PUSH);
	sender.bind(("tcp://*:" + boost::lexical_cast<std::string>(gport)).c_str());

	for(;;)
	{
		zmq::message_t msg;
		std::ostringstream oss;
		create_request(oss);
		str_to_msg(msg, oss.str());
		sender.send(msg);
	}
}

/******************************************************************************/

void start_collector( int cport, std::function< void(std::istream &) > process_result )
{
	zmq::context_t context(1);
	zmq::socket_t receiver(context,ZMQ_PULL);
	receiver.bind(("tcp://*:" + boost::lexical_cast<std::string>(cport)).c_str());

	for(;;)
	{
		zmq::message_t msg;
		receiver.recv(&msg);
		std::istringstream iss(msg_to_str(msg));
		process_result(iss);
	}
}

/******************************************************************************/

void start_worker( std::string server_addr, int gport, int cport, std::function< void(std::istream &, std::ostream &) > process_work )
{
	zmq::context_t context(1);
	zmq::socket_t receiver(context, ZMQ_PULL);
	receiver.connect(("tcp://" + server_addr + ":" + boost::lexical_cast<std::string>(gport)).c_str());
	zmq::socket_t sender(context, ZMQ_PUSH);
	sender.connect(("tcp://" + server_addr + ":" + boost::lexical_cast<std::string>(cport)).c_str());

	for(;;)
	{
		zmq::message_t msg;
		receiver.recv(&msg);
		std::istringstream iss(msg_to_str(msg));
		std::ostringstream oss;
		process_work(iss, oss);
		str_to_msg(msg, oss.str());
		sender.send(msg);
	}
}

/******************************************************************************/
