#ifndef TEXT_H
#define TEXT_H

#include "observer.h"
#include "studio.h"
#include "tile.h"

class Text : public Observer {
  Studio* studio;
  
 public:
  Text(Studio* studio);
  void notify() override;
};

#endif
