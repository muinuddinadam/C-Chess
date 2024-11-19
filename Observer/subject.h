#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
using namespace std;

class Observer; // forward declaration

class Subject {
  vector<Observer*> observers;
 public:
  void attach( Observer* o );
  void detach( Observer* o );
  void notifyObservers();
  virtual ~Subject() = default;
};

#endif
