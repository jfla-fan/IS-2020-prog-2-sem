#include "station.hpp"


VEHICLE_TYPE _transform_to_vehicle_type(const char* str)
{
	if (std::strncmp(str, "Троллейбус", sizeof("Троллейбус")) == 0)
		return veh_type::TROLLEYBUS;
	else if (std::strncmp(str, "Трамвай", sizeof("Трамвай")) == 0)
		return veh_type::TRAM;
	else if (std::strncmp(str, "Автобус", sizeof("Автобус")) == 0)
		return veh_type::BUS;

	return veh_type::NONE;
}

OBJECT_TYPE _transform_to_object_type(const char* str)
{
	if (std::strncmp(str, "Временная", sizeof("Временная")) == 0)
		return obj_type::TEMP_STATION;
	else if (std::strncmp(str, "Остановка", sizeof("Остановка")) == 0)
		return obj_type::STATION;
	else if (std::strncmp(str, "КП", sizeof("КП")) == 0)
		return obj_type::KP;
	else if (std::strncmp(str, "ДС", sizeof("ДС")) == 0)
		return obj_type::DS;
	else if (std::strncmp(str, "Парк", sizeof("Парк")) == 0)
		return obj_type::PARK;

	return obj_type::NONE;
}

STREET_TYPE _transform_to_street_type(const char* str)
{
	if (std::strncmp(str, "улица", sizeof("улица")) == 0 || std::strncmp(str, "ул.", sizeof("ул.")) == 0)
		return street_type::STREET;
	else if (std::strncmp(str, "шоссе", sizeof("шоссе")) == 0 || std::strncmp(str, "ш.", sizeof("ш.")) == 0)
		return street_type::HIGHWAY;
	else if (std::strncmp(str, "проспект", sizeof("проспект")) == 0 || std::strncmp(str, "пр.", sizeof("пр.")) == 0)
		return street_type::AVENUE;
	else if (std::strncmp(str, "платформа", sizeof("платформа")) == 0 || std::strncmp(str, "пл.", sizeof("пл.")) == 0)
		return street_type::PLATFORM;

	return street_type::NONE;
}

const char* _transfrom_vehicle_to_str(VEHICLE_TYPE vh)
{
	switch (vh)
	{
	case veh_type::BUS:
		return "Автобус";
	case veh_type::TRAM:
		return "Трамвай";
	case veh_type::TROLLEYBUS:
		return "Троллейбус";
	default:
		return "None";
	}
}

const char* _transform_object_to_str(OBJECT_TYPE obj)
{
	switch (obj)
	{
	case obj_type::STATION:
		return "Остановка";
	case obj_type::TEMP_STATION:
		return "Временная";
	case obj_type::KP:
		return "КП";
	case obj_type::DS:
		return "ДС";
	case obj_type::PARK:
		return "Парк";
	default:
		return "None";
	}
}

const char* _transform_street_to_str(STREET_TYPE street)
{
	switch(street)
	{
	case street_type::STREET:
		return "улица";
	case street_type::HIGHWAY:
		return "шоссе";
	case street_type::AVENUE:
		return "проспект";
	case street_type::PLATFORM:
		return "платформа";
	default:
		return "None";
	}
}

std::string::size_type _erase_substr(std::string& str, const std::string& substr)
{
	std::string::size_type t = str.find(substr);
	if (t != std::string::npos)
	{
		str.erase(t, substr.length());
	}

	return t;
}

std::vector<std::pair<std::string, street_type>> _get_streets_from_station_location(const std::string& loc) {
	Tokenizer tokenizer(loc, ",");
	token_t curr;
	std::vector<std::pair<std::string, street_type>> streets; // collection of street name / street type
	street_type _street_type = STREET_TYPE::NONE;

	while (!tokenizer.isDone())
	{
		curr = tokenizer.next();

		if (_erase_substr(curr._value, "улица") != std::string::npos || _erase_substr(curr._value, "ул.") != std::string::npos ||
			_erase_substr(curr._value, "УЛ.") != std::string::npos || _erase_substr(curr._value, "улица") != std::string::npos ||
			_erase_substr(curr._value, "УЛИЦА") != std::string::npos || _erase_substr(curr._value, "Ул.") != std::string::npos)
			_street_type = STREET_TYPE::STREET; 
		
		else if (_erase_substr(curr._value, "проспект") != std::string::npos || _erase_substr(curr._value, "пр.") != std::string::npos ||
			_erase_substr(curr._value, "ПРОСПЕКТ") != std::string::npos || _erase_substr(curr._value, "ПР.") != std::string::npos)
			_street_type = STREET_TYPE::HIGHWAY; 
		
		else if (_erase_substr(curr._value, "шоссе") != std::string::npos || _erase_substr(curr._value, "ш.") != std::string::npos ||
			_erase_substr(curr._value, "ШОССЕ") != std::string::npos || _erase_substr(curr._value, "Ш.") != std::string::npos)
			_street_type = STREET_TYPE::AVENUE; 

		else if (_erase_substr(curr._value, "платформа") != std::string::npos || _erase_substr(curr._value, "пл.") != std::string::npos ||
			_erase_substr(curr._value, "ПЛАТФОРМА") != std::string::npos || _erase_substr(curr._value, "ПЛ.") != std::string::npos ||
			_erase_substr(curr._value, "Пл.") != std::string::npos)
			_street_type = STREET_TYPE::PLATFORM; 

		else
			_street_type = STREET_TYPE::NONE;

		trim(curr._value);

		if (curr._value.size() != 0) 
			streets.push_back({ curr._value, _street_type });		
	}

#ifdef DEBUG_TRACING
	if (streets.size() == 0)
		std::cerr << "Couldn't find a street for station # " << number << 
			" with location: " << loc <<"; name: " << name << "\n";
	else
	{
		std::cerr << "Found streets:\n";
		for (auto _street : streets)
		{
			std::cerr << _street << "; ";
		}
		std::cerr << '\n';
	}
#endif

	return streets;
}


double haversine_formula_distance(double latitude1, double latitude2, double longtitude1, double longtitude2)
{
	const double f1 = to_radians(latitude1);
	const double f2 = to_radians(latitude2);

	const double df = to_radians(latitude2 - latitude1);
	const double dl = to_radians(longtitude2 - longtitude1);

	const double a = sin(df / 2) * sin(df / 2) +
		cos(f1) * cos(f2) * sin(dl / 2) * sin(dl / 2);
	const double c = atan2(sqrt(a), sqrt(1 - a));

	const double d = EARTH_RADIUS * c;

	return d;
}

double _calculate_distance_between_stations(const station_link lhs, const station_link rhs)
{
	return haversine_formula_distance(lhs->_latitude, rhs->_latitude, lhs->_longtitude, rhs->_longtitude);
}

#ifdef DEBUG_FUNCTIONS
double _calculate_cartesian_distance(const station_link lhs, const station_link rhs)
{
	return sqrt(std::pow(lhs->_latitude - rhs->_latitude, 2) + std::pow(lhs->_longtitude - rhs->_longtitude, 2));
}
#endif

std::ostream& operator << (std::ostream& os, const station_t& station)
{
	return os << station._name;
}

Path::Path()
	: _route(-1), _length(0), _vehicle(veh_type::NONE)
{
}

Path::Path(veh_type vh)
	: _route(-1), _length(0), _vehicle(vh)
{
}

Path::Path(int route, veh_type vh)
	: _route(route), _length(0), _vehicle(vh)
{
}

Path::Path(int route, std::initializer_list<station_link> init_list)
	: _route(route), _stations(init_list)
{
	_length = _stations.size();
	if (_length != 0)
		_vehicle = (*_stations.begin())->_vehicle_type;
}

double Path::_get_final_length() const
{
	double result = 0;

	for (auto it = begin(); it != --end(); ++it)
		result += dist(*it, *std::next(it, 1));

	return result;
}

std::pair<Path::iterator, double> Path::_order(const station_link value)
{
	if (_stations.size() == 0)
		return { end(), 0 };

	struct { iterator _iterator; double _dist; } place = { begin(), 0.0 };

	place._dist = dist(value, *begin());
	double curr_dist = place._dist;

	for (iterator it = ++begin(); it != end(); ++it)
	{
		station_link prev, curr;
		std::advance(it, -1);
		prev = *it;
		std::advance(it, +1);
		curr = *it;


		if (( curr_dist = dist(prev, value) + dist(curr, value) - dist(prev, curr) ) < place._dist)
		{
			place._dist = curr_dist;
			place._iterator = it;
		}
	}

	if ((curr_dist = dist(*(--end()), value)) < place._dist)
	{
		place._dist = curr_dist;
		place._iterator = end();
	}

	return { place._iterator, place._dist };
}

void Path::print_route() const
{
	std::for_each(begin(), end(), [&](const auto& it) { std::cout << *it << " "; });
}

void Path::push(const station_link link)
{
	auto t = _order(link);
	_stations.insert(t.first, link);
	_length += t.second;
}