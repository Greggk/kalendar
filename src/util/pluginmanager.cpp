#include "pluginmanager.h"
#include <iostream>

PluginManager::PluginManager()
{
    filesystem::path dir(TOOLS_FOLDER);
    filesystem::create_directory(dir);
    for (filesystem::directory_entry e : filesystem::directory_iterator(TOOLS_FOLDER)) {
        filesystem::path p = e.path();
#ifdef __OS2__
        if ((p.extension() == ".cmd") || (p.extension() == ".btm"))
#else
        if ((p.extension() == ".sh") || (p.extension() == ".bat"))
#endif
            this->tools.push_back(p.filename());
    }
}

void PluginManager::runTool(const string &name) {
    string cmd = string(TOOLS_FOLDER) + "/\"" + name +"\"";
    system(cmd.c_str());
}

vector<string> PluginManager::get_tools() {
    return this->tools;
}
