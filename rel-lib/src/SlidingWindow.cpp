/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "SlidingWindow.h"

SlidingWindow::SlidingWindow() {
    reserve(2);
}

SlidingWindow::~SlidingWindow() { }

CharAndPos SlidingWindow::pop_front() {
    CharAndPos ret;
    if(size() > 0) {
        ret = *(begin());
        erase(begin());
    }

    return ret;
}
