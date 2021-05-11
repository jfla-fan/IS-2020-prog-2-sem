#include "earth.hpp"



Earth::Earth()
{
}

Earth::~Earth()
{
}


bool Earth::loadFromXml(std::string from)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(from.c_str(), 116U, pugi::encoding_utf8);

	if (!result)
		return false;

	pugi::xml_node tools = doc.child("dataset");

	station_link curr_station;
	Tokenizer tokenizer;

	for (auto it = tools.begin(); it != tools.end(); ++it)
	{
		curr_station = new station_t();
		curr_station->_number = std::stoi(it->child("number").text().get());
		curr_station->_vehicle_type = to_vehicle_type(it->child("type_of_vehicle").text().get());
		curr_station->_object_type = to_object_type(it->child("object_type").text().get());
		curr_station->_name = std::string(it->child("name_stopping").text().get());
		curr_station->_official_name = std::string(it->child("the_official_name").text().get());
		curr_station->_location = std::string(it->child("location").text().get());
	
		std::string routes = std::string(it->child("routes").text().get());
		
		if (trim_copy(routes).size() == 0) 
		{
			delete curr_station;
			continue;
		}

		std::replace(routes.begin(), routes.end(), '.', ',');

		tokenizer = Tokenizer(routes, ",");
		token_t token;
		while (!tokenizer.isDone())
		{
			token = tokenizer.next();
			curr_station->_routes.push_back(token._value);
		}

		std::string coordinates = std::string(it->child("coordinates").text().get());
		tokenizer = Tokenizer(coordinates, ",");

		curr_station->_latitude = std::stod(tokenizer.next()._value);
		curr_station->_longtitude = std::stod(tokenizer.next()._value);

		for (auto it = curr_station->_routes.begin(); it != curr_station->_routes.end(); ++it)
		{
			_paths[*it].push(curr_station);
			_paths[*it].setVehicleType(curr_station->_vehicle_type);
		}

		_stations.push_back(curr_station);

#ifdef DEBUG_TRACING
		std::cout << "Number - " << curr_station._number << "\nName: " << curr_station._name <<
			"\nOfficial Name - " << curr_station._official_name << "\nLocation - " << curr_station._location <<
			"\nCoordinates - (" << curr_station._latitude << ", " << curr_station._longtitude << ")\n";

		std::cout << "Routes: ";
		for (auto it = curr_station._routes.begin(); it != curr_station._routes.end(); ++it)
			std::cout << *it << ", ";
		std::cout << std::endl;
#endif

	}

	return true;
}


void Earth::printRoute(const std::string& which_one) const
{
	std::cout << "Route " << which_one << " has " << _paths.at(which_one).getStationsNumber() << " stations:\n";
	
	//fixed range-based
	//fixed its for_each, not range-based for

	for (const auto& link : _paths.at(which_one))
	{
		std::cout << link->_name << "[" << link->_location << "]\n";
	}
}


void Earth::printRoutes() const
{
	std::for_each(_paths.begin(), _paths.end(), [&](const auto& p) { printRoute(p.first); });
}

std::string Earth::getLongestStationsRoute(veh_type vehicle = VEHICLE_TYPE::NONE) const
{
	//fixed do you really need pair?
	std::string resultRoute = "";
	int resultStationsNumber = 0;
	
	for (const auto& [route, path] : _paths)
	{
		if (path.getStationsNumber() > resultStationsNumber && 
			(vehicle == veh_type::NONE || path.getVehicleType() == vehicle)) [[likely]]
		{
			resultRoute = route;
			resultStationsNumber = path.getStationsNumber();	
		}
	}

	return resultRoute;
}

std::string Earth::getLongestDistanceRoute(veh_type vehicle = VEHICLE_TYPE::NONE) const
{
	std::string resultRoute = "";
	double resultRouteLength = 0.0;
	
	for (const auto& [route, path] : _paths)
	{
		if (path.getLength() > resultRouteLength &&
			(vehicle == veh_type::NONE || path.getVehicleType() == vehicle))
		{
			resultRoute = route;
			resultRouteLength = path.getLength();
		
		}
	}

	return resultRoute;
}

//fixed first-seconds
#if __cplusplus > 201703L
std::string Earth::getLongestStationsStreet() const
{
	using street_t = std::pair<std::string, street_type>;
	
	std::map<street_t, int, decltype([](const street_t& lhs, const street_t& rhs) { return lhs.first < rhs.first; })> streetsToStationsCount;
	std::string loc;
	std::vector<street_t> streets;

	for (const station_link& station_l : _stations)
	{
		loc = station_l->_location;

#ifdef DEBUG_TRACING
		std::cout << "Got location - " << loc << "\n";
#endif
		streets = get_streets(loc);
		for (const auto& street : streets)
		{
			++streetsToStationsCount[street];
		}
	}

	const auto& [ firstStreet, firstFrequency ] = *streetsToStationsCount.begin();

	street_t mostFrequentStreet = firstStreet;
	int 	 highestFrequency = firstFrequency;

	for (const auto& [street, frequency] : streetsToStationsCount)
	{
		if (frequency > highestFrequency)
		{
			mostFrequentStreet = street;
			highestFrequency = frequency;
		}
	}

#ifdef DEBUG_TRACING
	std::cout << "Street - Frequency:\n";
	for (const auto& [street, frequency] : streetsToStationsCount)
	{
		std::cerr << std::string(street_to_str(street.second)) + " " + street.first
			<< " - " << frequency << '\n';
	}
#endif

	const auto& [streetName, typeOfStreet] = mostFrequentStreet;

	return std::string(street_to_str(typeOfStreet)) + " " + 
			streetName + " - " + std::to_string(highestFrequency); 
}
#endif