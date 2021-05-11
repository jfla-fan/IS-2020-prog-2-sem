#ifndef _STATION_TYPES_HEADER_
#define _STATION_TYPES_HEADER_

#define _USE_MATH_DEFINES
#define _NODISCARD [[nodiscard]]


#include <string>
#include <list>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>
#include "tokenizer.hpp"
#include <vector>


#define to_vehicle_type(str) _transform_to_vehicle_type((str))

#define to_object_type(str)	_transform_to_object_type((str))

#define to_street_type(str) _transform_to_street_type(str)

#define vehicle_to_str(vh) _transfrom_vehicle_to_str((vh))

#define object_to_str(obj) _transform_object_to_str((obj))

#define street_to_str(street) _transform_street_to_str((street))

#define dist(station1_link, station2_link) _calculate_distance_between_stations((station1_link), (station2_link))

#define get_streets(location) _get_streets_from_station_location((location))

#define to_radians(angle) ((angle) * M_PI / 180)  

#define ltrim(str) _ltrim((str))

#define rtrim(str) _rtrim((str))

#define trim(str) _trim((str))

#define ltrim_copy(str) _ltrim_copy((str))

#define rtrim_copy(str) _rtrim_copy((str))

#define trim_copy(str)  _trim_copy((str))

#define erase_substr(str, substr) _earse_substr((str), (substr))

static const double EARTH_RADIUS = 6371e3; // in meters


static inline void _ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace)))); 
}

static inline void _rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

static inline void _trim(std::string& s)
{
	_ltrim(s);
	_rtrim(s);
}

static inline std::string _ltrim_copy(std::string s) {
    _ltrim(s);
    return s;
}

static inline std::string _rtrim_copy(std::string s) {
    _rtrim(s);
    return s;
}

static inline std::string _trim_copy(std::string s) {
    _trim(s);
    return s;
}

std::string::size_type _erase_substr(std::string& str, const std::string& substr);

typedef enum class VEHICLE_TYPE
{
	NONE,
	BUS,
	TROLLEYBUS,
	TRAM
} veh_type;

VEHICLE_TYPE _transform_to_vehicle_type(const char* str);

const char* _transfrom_vehicle_to_str(VEHICLE_TYPE vh);

typedef enum class OBJECT_TYPE
{
	NONE,
	STATION,
	TEMP_STATION,
	DS,
	KP,
	PARK
} obj_type;

OBJECT_TYPE _transform_to_object_type(const char* str);

const char* _transform_object_to_str(OBJECT_TYPE obj);

typedef enum class STREET_TYPE
{
	NONE,
	STREET,			// улица
	HIGHWAY,		// шоссе
	AVENUE,			// проспект
	PLATFORM		// платформа
} street_type;

STREET_TYPE _transform_to_street_type(const char *str);

const char* _transform_street_to_str(STREET_TYPE street);

typedef struct STATION_TYPE
{
	int						_number;

	veh_type				_vehicle_type;
	obj_type				_object_type;

	std::string				_name;
	std::string				_official_name;
	std::string				_location;

	std::list <std::string> _routes;

	double					_latitude;
	double					_longtitude;

} station_t;

typedef station_t* station_link;

double _calculate_distance_between_stations(const station_link lhs, const station_link rhs);

/* splits given string based on ',' delimeter and process every part, filtering on  */
std::vector<std::pair<std::string, street_type>> _get_streets_from_station_location(const std::string& loc);

#ifdef DEBUG_FUNCTIONS
double _calculate_cartesian_distance(const station_link lhs, const station_link rhs);
#endif

std::ostream& operator << (std::ostream& os, const station_t& station);


// path contains station links of a specified route number and in an appropriate order
class Path
{
public:

	typedef std::list<station_link>::iterator				iterator;

	typedef std::list<station_link>::const_iterator			const_iterator;

	typedef std::list<station_link>::reverse_iterator		reverse_iterator;

	typedef std::list<station_link>::const_reverse_iterator const_reverse_iterator;

public:

	Path();

	Path(veh_type vh);
	
	Path(int route, veh_type vh = veh_type::NONE);

	Path(int route, std::initializer_list<station_link> init_list);


	inline int		getRoute() const { return _route; }

	inline int		getStationsNumber() const { return _stations.size(); }

	inline double	getLength() const { return _length; }

	inline veh_type getVehicleType() const { return _vehicle; }

	inline void		setVehicleType(veh_type t) { _vehicle = t; }

	void print_route() const;

	// inserts station link into path list so the list saves the order;
	// increases full path length
	void push(const station_link link);



	_NODISCARD inline bool empty() const { return _stations.empty(); }


	_NODISCARD inline iterator					begin() { return _stations.begin(); }

	_NODISCARD inline const_iterator			begin() const { return _stations.begin(); }

	_NODISCARD inline iterator					end() { return _stations.end(); }

	_NODISCARD inline const_iterator			end() const { return _stations.end(); }

	_NODISCARD inline reverse_iterator			rbegin() { return _stations.rbegin(); }

	_NODISCARD inline const_reverse_iterator	rbegin() const { return _stations.rbegin(); }

	_NODISCARD inline const_iterator			cbegin() const { return _stations.cbegin(); }

	_NODISCARD inline const_iterator			cend() const { return _stations.cend(); }

	_NODISCARD inline const_reverse_iterator	crbegin() const { return _stations.crbegin(); }

	_NODISCARD inline const_reverse_iterator	crend() const { return _stations.crend(); }


private:

	/*
		returns a pair of an iterator and double value
		double value represents minimal distance between neighbour stations
		station what should be placed at (iterator - 1) position
	*/
	std::pair<iterator, double> _order(const station_link what);
	
	// calculates the full length of the route
	double _get_final_length() const;


	int						_route;

	double					_length;

	VEHICLE_TYPE			_vehicle;

	std::list<station_link> _stations;

};



#endif // _STATION_TYPES_HEADER_