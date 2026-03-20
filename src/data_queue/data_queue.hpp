#ifndef _DATA_QUEUE_HPP_
#define _DATA_QUEUE_HPP_

#include "common/common_typedefs.hpp"

#include <boost/lockfree/spsc_queue.hpp>

extern boost::lockfree::spsc_queue<MemChunk, boost::lockfree::capacity<2>> dlBuffer;
extern boost::lockfree::spsc_queue<MemChunk, boost::lockfree::capacity<16>> inflateBuffer;

#endif
