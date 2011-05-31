#pragma once

/******************************************************************************/

#include <string>

#include <boost/lexical_cast.hpp>

#include <zmq.hpp>

/******************************************************************************/

inline std::string msg_to_str( zmq::message_t & msg )
{
	return std::string( (char const*)msg.data(), (char const*)msg.data() + msg.size() );
}

inline void str_to_msg( zmq::message_t & msg, std::string const & str )
{
	msg.rebuild( str.size()+1 );
	memcpy( msg.data(), (void*)str.c_str(), str.size()+1 );
}

/******************************************************************************/
