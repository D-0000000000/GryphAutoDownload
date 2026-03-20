#include "data_queue.hpp"
#include "common/common_typedefs.hpp"

#include <boost/lockfree/spsc_queue.hpp>

boost::lockfree::spsc_queue<MemChunk, boost::lockfree::capacity<2>> dlBuffer;
