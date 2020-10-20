/*
 * SlidingWindow.h
 *
 *  Created on: 04.08.2020
 *      Author: Stefan_2
 */

#ifndef SLIDINGWINDOW_H_
#define SLIDINGWINDOW_H_

#include <vector>

class SlidingWindow : public std::vector<char> {
public:
    SlidingWindow();
    virtual ~SlidingWindow();

    char pop_front();

};

#endif /* SLIDINGWINDOW_H_ */
