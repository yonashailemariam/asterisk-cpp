/*
 * Thread.h
 *
 *  Created on: Jul 7, 2011
 *      Author: augcampos
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <boost/thread.hpp>

namespace asteriskcpp {

class Thread {
public:
	Thread();
	virtual ~Thread();

	virtual void start();
	virtual void stop();
	virtual void run();
	virtual void operator ()();

protected:
	void setMustStop(volatile bool stop);

private:
	boost::thread* m_thread; // The thread runs this object
	// Variable that indicates to stop and the mutex to synchronise "must stop" on (mutex explained later)
	volatile bool m_mustStop;
	boost::mutex m_mustStopMutex;
};

}

#endif /* THREAD_H_ */