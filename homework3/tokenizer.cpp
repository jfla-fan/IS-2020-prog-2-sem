#include "tokenizer.hpp"

std::ostream& operator << (std::ostream& os, const token_t& t)
{
	return os << t._value << " [" << t._start_position << ": " << t._end_position << "]";
}


token_t Tokenizer::UNITIALIZED_TOKEN = token_t { -1, -1, "" };


Tokenizer::Tokenizer()
{
	_curr_token = NULL_TOKEN;
	_done = true;
	_source = "";
	_delimeter = "";
}

Tokenizer::Tokenizer(const std::string& source, const std::string& delimeter /* = " " */)
	: _source(source), _delimeter(delimeter), _done(false)
{	
	_curr_token._start_position = 0;
	_curr_token._end_position = -1;
	_curr_token._value = "";
}

token_t Tokenizer::next()
{
	if (isDone())
		return NULL_TOKEN;

	int start_pos = _curr_token._end_position + 1;
	token_t result;
	size_t pos;

	if ((pos = _source.find(_delimeter, start_pos)) != std::string::npos) 
	{
		result._start_position = start_pos;
		result._end_position = pos;
		result._value = _source.substr(result._start_position, result._end_position - result._start_position);
		_curr_token = result;
	}
	else
	{
		result._start_position = start_pos;
		result._end_position = _source.size();
		result._value = _source.substr(result._start_position, result._end_position - result._start_position);
		_done = true;
		_curr_token = NULL_TOKEN;
		return result;
	}

	return result;
}