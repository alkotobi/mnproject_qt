//
// Created by MERHAB NOUREDDINE on 19/09/2024.
// Copyright (c) 2024 MERHAB NOUREDDINE. All rights reserved.
//

#pragma once
#include <QString>

class MNException : public std::exception {
public:
    explicit MNException(const QString& message) : msg(message.toStdString()) {}

    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};
