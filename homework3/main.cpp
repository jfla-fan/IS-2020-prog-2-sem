#include <iostream>
#include "station.hpp"
#include "earth.hpp"
#include "enc.hpp"



int main(int argc, char** argv)
{
	initialize_system_encoding();

	Earth earth;

	earth.loadFromXml("dataset.xml");

	std::string longestDistanceTramRoute = earth.getLongestDistanceRoute(VEHICLE_TYPE::TRAM);
	std::string longestDistanceBusRoute = earth.getLongestDistanceRoute(VEHICLE_TYPE::BUS);
	std::string longestDistanceTrolleybusRoute = earth.getLongestDistanceRoute(VEHICLE_TYPE::TROLLEYBUS);

	std::string longestStationsTramRoute = earth.getLongestStationsRoute(VEHICLE_TYPE::TRAM);
	std::string longestStationsBusRoute = earth.getLongestStationsRoute(VEHICLE_TYPE::BUS);
	std::string longestStationsTrolleybusRoute = earth.getLongestStationsRoute(VEHICLE_TYPE::TROLLEYBUS);

	std::string mostFrequentStationsStreet = earth.getLongestStationsStreet();

	std::cout << "Longest Distance Tram Route - " << longestDistanceTramRoute << '\n';
	std::cout << "Longest Distance Bus Route - " << longestDistanceBusRoute << '\n';
	std::cout << "Longest Distance Trolleybus Route - " << longestDistanceTrolleybusRoute << '\n';

	std::cout << "Longest Stations Tram Route - " << longestStationsTramRoute << '\n';
	std::cout << "Longest Stations Bus Route - " << longestStationsBusRoute << '\n';
	std::cout << "Longest Stations Trolleybus Route - " << longestStationsTrolleybusRoute << '\n';


	// earth.printRoute(longestDistanceTramRoute);

	// earth.printRoute(longestDistanceBusRoute);
	
	// earth.printRoute(longestDistanceTrolleybusRoute);
	
	// earth.printRoute(longestStationsTramRoute);

	earth.printRoute(longestStationsBusRoute);
	
	earth.printRoute(longestStationsTrolleybusRoute);
	
	std::cout << "Most Frequent Stations Street - " << mostFrequentStationsStreet << '\n';

	return 0;
}