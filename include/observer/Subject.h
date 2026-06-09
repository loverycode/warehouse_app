#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "observer/Observer.h"

class Subject
{
    std::vector<Observer *> observers;

public:
    void attach(Observer *obs);
    void detach(Observer *obs);
    void notify(const std::string &message);
};

#endif