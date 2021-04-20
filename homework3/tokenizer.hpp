#pragma once
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>

#define NULL_TOKEN Tokenizer::UNITIALIZED_TOKEN


typedef struct STRING_TOKEN
{
	int _start_position, _end_position;

	std::string _value;

} token_t;

std::ostream& operator << (std::ostream& os, const token_t& t);



class Tokenizer
{
public:

	static token_t UNITIALIZED_TOKEN;

public:

	Tokenizer();

	Tokenizer(const std::string& source, const std::string& delimeter = " ");


	inline std::string getDelimeter() const { return _delimeter; }

	token_t next();

	inline bool isDone() const { return _done; }

private:

	token_t _curr_token;

	std::string _source;

	bool _done;

	std::string _delimeter;

};