#pragma once

#include <mutex>
#include <string>
#include <map>
#include <vector>
#include <Robot.h>
#include <Backend.h>
#include <com/Server.h>

class Simulator
{
    public:
        class ParameterError
        {
            public:
                ParameterError(double error);

                double error;
        };
        
        class Parameter
        {
            public:
                void check();
                double normalize();
                void fromNormalized(double n);

                std::string name;
                double value;
                double min;
                double max;
        };

        class Parameters
        {
            public:
                Parameters();
                void add(std::string name, double min, double max, double value);
                void set(std::string name, double value);
                double get(std::string name);

                std::vector<double> toVector();
                void fromArray(const double *x, const int N);
                std::string toString();
                void push(double value);

            protected:
                int pushIndex;
                std::vector<std::string> order;
                std::map<std::string, Parameter> values;
        };

        Simulator(std::string robotFile, double factor, bool runServer);
        virtual ~Simulator();

        double run(Parameters &parameters, double duration=6.0);

    protected:
        Metabot::Server *server;
        std::string robotFile;
        double factor;
        std::mutex mutex;
        pthread_t serverThread;
};
