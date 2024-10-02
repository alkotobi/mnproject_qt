//
// Created by MERHAB NOUREDDINE on 02/10/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include "mntable.h"

class MnView : public MnTable{
private:
    using MnTable::exec;
    using MnTable::open;
    using MnTable::edit;
    using MnTable::append;
    using MnTable::post;
    using MnTable::cancel;
    using MnTable::remove;
    using MnTable::data;
    using MnTable::execBeforePost;
    using MnTable::execAfterRemove;
    using MnTable::execBeforeRemove;
    using MnTable::execAfterPost;
    using MnTable::setFieldValue;
public:
  MnView(const MnTable & tbl);

};
