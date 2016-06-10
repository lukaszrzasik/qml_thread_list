#pragma once

class Observer;

class Observable
{
public:
    virtual ~Observable() {}

    virtual void registerObserver(Observer *) = 0;
    virtual void unregisterObserver(Observer *) = 0;

protected:
    virtual void notifyAll() const = 0 ;
};
