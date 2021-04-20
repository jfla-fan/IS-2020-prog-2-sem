#ifndef _EARTH_HEADER_
#define _EARTH_HEADER_

#include "station.hpp"
#include "tokenizer.hpp"
#include "pugixml.hpp"
#include <map>



class Earth
{
public:

	Earth();

	~Earth();

	bool loadFromXml(std::string from);

	void printRoute(const std::string& which_one) const;

	void printRoutes() const;

	std::string getLongestStationsRoute(veh_type vehicle) const;

	std::string getLongestDistanceRoute(veh_type vehicle) const;

	std::string getLongestStationsStreet() const;


private:

	std::map<std::string, Path> _paths;

	std::vector<station_link> _stations;
};

#endif