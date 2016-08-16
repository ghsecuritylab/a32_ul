// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef DEFINE_SELECTION_EVENT_TYPE
#error "Please define DEFINE_SELECTION_EVENT_TYPE before including this file."
#endif

DEFINE_SELECTION_EVENT_TYPE(SELECTION_SHOWN, 0)
DEFINE_SELECTION_EVENT_TYPE(SELECTION_CLEARED, 1)
DEFINE_SELECTION_EVENT_TYPE(SELECTION_DRAG_STARTED, 2)
DEFINE_SELECTION_EVENT_TYPE(SELECTION_DRAG_STOPPED, 3)
DEFINE_SELECTION_EVENT_TYPE(INSERTION_SHOWN, 4)
DEFINE_SELECTION_EVENT_TYPE(INSERTION_MOVED, 5)
DEFINE_SELECTION_EVENT_TYPE(INSERTION_TAPPED, 6)
DEFINE_SELECTION_EVENT_TYPE(INSERTION_CLEARED, 7)
DEFINE_SELECTION_EVENT_TYPE(INSERTION_DRAG_STARTED, 8)