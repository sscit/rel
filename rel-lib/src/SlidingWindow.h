/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

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
