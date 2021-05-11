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

	// returns the number of route which includes maximum of stations
	std::string getLongestStationsRoute(veh_type vehicle) const;

	// returns the number of route which has maximum length
	std::string getLongestDistanceRoute(veh_type vehicle) const;

	#if __cplusplus > 201703L
	// returns the name of street which has maximum stations
	std::string getLongestStationsStreet() const;
	#endif

private:

	std::map<std::string, Path> _paths;

	std::vector<station_link> _stations;
};

#endif