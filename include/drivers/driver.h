#ifndef __BLJOS__DRIVERS__DRIVER_H
#define __BLJOS__DRIVERS__DRIVER_H

namespace bljOS{
    namespace drivers{
        class Driver{
        public:
            Driver();
            ~Driver();

            virtual void Activate();
            virtual int Reset();
            virtual void Deactivate();
        };

        class DriverManager{
        public:
            Driver* drivers[255];
            int numDrivers;
        public:
            DriverManager();
            void AddDriver(Driver*);
            void ActivateAll();
        };
    }
}
#endif
