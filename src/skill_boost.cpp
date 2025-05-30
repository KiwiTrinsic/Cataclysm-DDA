#include "skill_boost.h"

#include <algorithm>
#include <cmath>
#include <string>

#include "generic_factory.h"

namespace
{
generic_factory<skill_boost> all_skill_boosts( "skill boost", "stat" );
} // namespace

const std::vector<skill_boost> &skill_boost::get_all()
{
    return all_skill_boosts.get_all();
}

std::optional<skill_boost> skill_boost::get( const std::string &stat_str )
{
    for( const skill_boost &boost : get_all() ) {
        if( boost.stat() == stat_str ) {
            return std::optional<skill_boost>( boost );
        }
    }
    return std::nullopt;
}

void skill_boost::load_boost( const JsonObject &jo, const std::string &src )
{
    all_skill_boosts.load( jo, src );
}

void skill_boost::load( const JsonObject &jo, std::string_view )
{
    mandatory( jo, was_loaded, "skills", _skills );
    mandatory( jo, was_loaded, "skill_offset", _offset );
    mandatory( jo, was_loaded, "scaling_power", _power );
}

void skill_boost::reset()
{
    all_skill_boosts.reset();
}

std::string skill_boost::stat() const
{
    return id.str();
}

const std::vector<std::string> &skill_boost::skills() const
{
    return _skills;
}

float skill_boost::calc_bonus( int skill_total ) const
{
    if( skill_total + _offset <= 0 ) {
        return 0.0;
    }
    return std::max( 0.0, std::floor( std::pow( skill_total + _offset, _power ) ) );
}
