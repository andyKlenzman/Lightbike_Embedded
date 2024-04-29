#ifndef SERVERWIRE_H
#define SERVERWIRE_H

#include "Wire.h"

namespace flake
{

    class ServerWire {
     protected:
      virtual ~ServerWire () = default;

     public:

      virtual int getFd () = 0;

      virtual int init () = 0;

      virtual Wire *accept () = 0;

      virtual bool available () = 0;

      virtual const char *describe () = 0;

      virtual int stackSizeBytes () = 0;
    };

}

#endif // SERVERWIRE_H
