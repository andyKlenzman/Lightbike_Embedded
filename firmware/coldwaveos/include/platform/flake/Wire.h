#ifndef WIRE_H_
#define WIRE_H_

#include <unistd.h>
#include <inttypes.h>

namespace flake {

    class Message;

    class Wire
    {
    public:
        virtual ~Wire() =default;

        virtual uint32_t getId() const = 0;
        virtual ssize_t read(void *, size_t)  = 0;
        virtual ssize_t write(void *, size_t)  = 0;

        virtual int close() = 0;
        virtual bool isOpen() = 0;
        virtual int open() = 0;

        virtual int getFd() const = 0;

        virtual int secure() const = 0;
        virtual int authenticated() const = 0;

    };

}

#endif
