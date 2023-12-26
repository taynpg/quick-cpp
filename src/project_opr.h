#pragma once

#include "config.h"

class ProjectOpr
{
public:
    ProjectOpr() = default;
    ~ProjectOpr() = default;
public:
    bool Run(const SConfig& config);

private:
    SConfig config_{};
};
