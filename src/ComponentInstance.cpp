#include <sstream>
#include <iostream>
#include "Component.h"
#include "ComponentInstance.h"
#include "CSG.h"
#include "util.h"

namespace Metabot
{
    ComponentInstance::ComponentInstance(Component *component_)
        : component(component_)
    {
    }

    std::string ComponentInstance::get(std::string name)
    {
        return values[name];
    }

    void ComponentInstance::set(std::string name, std::string value)
    {
        values[name] = value;
    }

    void ComponentInstance::compile()
    {
        std::string csg = openscad("csg");
        std::string stl = openscad("stl");

        CSG *document = CSG::parse(csg);
        anchors = document->anchors;

        delete document;
    }

    std::string ComponentInstance::openscad(std::string format)
    {
        std::stringstream cmd;
        cmd << "openscad -DMotorMark=true ";
        for (auto value : values) {
            cmd << "-D" << value.first << "=" << value.second << " ";
        }
        std::string output = tempname() + "." + format;
        cmd << component->filename << " -o " << output << " >/dev/null 2>/dev/null";
        std::string command = cmd.str();
        
        // Uncomment that to see the compile command called
        // std::cout << "compile(): " << command << std::endl;

        FILE *process = popen(command.c_str(), "r");
        if (pclose(process) != 0) {
            std::stringstream error;
            error << "Compilation failed for part " << component->name;
            throw error.str();
        }

        if (!file_exists(output)) {
            std::stringstream error;
            error << "Compilation did not produced the output file";
            throw error.str();
        }

        std::string data = file_get_contents(output);
        remove(output.c_str());

        return data;
    }
}
