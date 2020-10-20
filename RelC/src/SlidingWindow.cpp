/*
 * SlidingWindow.cpp
 *
 *  Created on: 04.08.2020
 *      Author: Stefan_2
 */

#include "SlidingWindow.h"

SlidingWindow::SlidingWindow() {
    reserve(2);

}

SlidingWindow::~SlidingWindow() {
    // TODO Auto-generated destructor stub
}

char SlidingWindow::pop_front()
{
    char ret = 0;
    if(size() > 0)
    {
        ret = *(begin());
        erase(begin());
    }

    return ret;
}
