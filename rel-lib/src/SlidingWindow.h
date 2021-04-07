/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef SLIDINGWINDOW_H_
#define SLIDINGWINDOW_H_

#include <vector>

class CharAndPos {
   public:
    char c;
    unsigned int pos;
};

class SlidingWindow : public std::vector<CharAndPos> {
   public:
    SlidingWindow();
    virtual ~SlidingWindow();

    CharAndPos pop_front();
};

#endif /* SLIDINGWINDOW_H_ */
