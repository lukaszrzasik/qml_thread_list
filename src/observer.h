#pragma once

class Observable;

class Observer
{
public:
    virtual ~Observer() {}

    virtual void notify(const Observable *) = 0 ;
};
