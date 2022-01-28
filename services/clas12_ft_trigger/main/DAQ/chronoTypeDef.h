#ifndef CHRONO_TYPE_DEF
#define CHRONO_TYPE_DEF

#include <chrono>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/cstdint.hpp>


/*When the TridasEvent file is included in the TrigJANA plugin,
 * these typedefs may already be defined, but this is not a problem,
 * C++ supports this explicitly
 */
typedef std::chrono::duration<boost::int_least64_t, std::ratio<1, 8000> > T125usec;
typedef std::chrono::duration<boost::int_least64_t, std::ratio<1, 250000000> > T4nsec;
typedef std::chrono::duration<boost::int_least64_t, std::ratio<86400> > days;
#endif
