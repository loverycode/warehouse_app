#include "observer/Subject.h"

void Subject::attach(Observer *obs)
{
    observers.push_back(obs);
}

void Subject::detach(Observer *obs)
{
    
}

void Subject::notify(const std::string &msg)
{
    for (auto obs : observers)
    {
        obs->update(msg);
    }
}