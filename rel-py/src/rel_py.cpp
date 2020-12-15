/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "rel-lib/src/Logger.h"
#include "rel-lib/src/FileEngine.h"
#include "rel-lib/src/RelParser.h"

namespace py = pybind11;

PYBIND11_MODULE(librel_py, m) {
    // Classes of AST
    py::class_<RsRdIdentifier>(m, "RsRdIdentifier")
        .def_readwrite("name", &RsRdIdentifier::name);

    py::class_<RsTypeElement>(m, "RsTypeElement")
        .def_readwrite("name", &RsTypeElement::name);

    py::class_<RsType>(m, "RsType")
        .def_readwrite("name", &RsType::name);

    py::class_<RdString>(m, "RdString")
        .def_readwrite("value", &RdString::value);

    py::class_<RdInteger>(m, "RdInteger")
        .def_readwrite("value", &RdInteger::value);

    py::class_<RdTypeInstanceElement>(m, "RdTypeInstanceElement")
        .def_readwrite("name", &RdTypeInstanceElement::name)
        .def_readwrite("string_value", &RdTypeInstanceElement::string_value)
        .def_readwrite("integer_value", &RdTypeInstanceElement::integer_value)
        .def_readwrite("link", &RdTypeInstanceElement::link)
        .def_readwrite("enum_value", &RdTypeInstanceElement::enum_value);

    py::class_<RdTypeInstance>(m, "RdTypeInstance")
        .def_readwrite("type", &RdTypeInstance::type)
        .def_readwrite("type_elements_data", &RdTypeInstance::type_elements_data);
    
    // Data types used in Logger Class
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
        .def("SetStartDirectory", &FileEngine::SetStartDirectory);

    py::class_<RelParser>(m, "RelParser")
        .def(py::init<Logger&, FileEngine const&>())
        .def("ProcessRelModel", &RelParser::ProcessRelModel)
        .def("GetDatabase", &RelParser::GetDatabase);

    m.doc() = "REL - python integration"; // optional module docstring
}
