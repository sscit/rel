#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "rel-lib/src/Logger.h"
#include "rel-lib/src/FileEngine.h"



namespace py = pybind11;





class Data
{
public:
    Data(std::string xx) : one_str(xx) {}
    virtual ~Data() {}

    std::string GetS() { return one_str;}
    std::string one_str;
};


#if 0
std::vector<Data> fibonacci(unsigned int const n) {
    std::vector<Data> v;

    Data d;
    d.index = 1;
    d.fibo = 1;
    v.push_back(d);

    d.index = 2;
    v.push_back(d);
    for(unsigned int i=1; i<n; i++) {
        int res = v[i].fibo + v[i-1].fibo;
        Data x;
        x.index = i;
        x.fibo = res;
        v.push_back(x);
    }
    
    return v;
}
#endif

PYBIND11_MODULE(librel_py, m) {
    py::class_<Data>(m, "Data")
        .def(py::init<const std::string>())
        .def("GetS", &Data::GetS);

    py::enum_<LogLevel>(m, "LogLevel")
        .value("ERROR", LogLevel::ERROR)
        .value("WARNING", LogLevel::WARNING)
        .value("INFO", LogLevel::INFO)
        .value("DEBUG", LogLevel::DEBUG);

    py::class_<Logger>(m, "Logger")
        .def(py::init<>())
        .def(py::init<const std::string>())
        .def("SetLogLevel", &Logger::SetLogLevel)
        .def("GetCurrentLogLevel", &Logger::GetCurrentLogLevel)
        .def("LogMessage", &Logger::LogMessage, 
             py::arg("loglevel") = LogLevel::WARNING, py::arg("message") = "", py::arg("filename") = "Unset", py::arg("line_number") = -1);


    py::class_<FileEngine>(m, "FileEngine")
        .def(py::init<>())
        .def("SetSearchRecursive", &FileEngine::SetSearchRecursive)
        .def("GetSearchRecursive", &FileEngine::GetSearchRecursive)
        .def("SetStartDirectory", &FileEngine::SetStartDirectory)
        .def("GetListOfFiles", &FileEngine::GetListOfFiles);

    
    
    m.doc() = "REL - python integration"; // optional module docstring

    
}
