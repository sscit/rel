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
        .def("Get", &RsRdIdentifier::Get);

    py::class_<RsEnum>(m, "RsEnum")
        .def("Get", &RsEnum::Get)
        .def_readwrite("enum_elements", &RsEnum::enum_elements);

    py::class_<RsTypeAttribute>(m, "RsTypeAttribute")
        .def("Get", &RsTypeAttribute::Get)
        .def_readwrite("enum_definition", &RsTypeAttribute::enum_definition);

    py::class_<RsType>(m, "RsType")
        .def("Get", &RsType::Get)
        .def_readwrite("attributes", &RsType::attributes);


    py::class_<RdString>(m, "RdString")
        .def("Get", &RdString::Get);

    py::class_<RdInteger>(m, "RdInteger")
        .def("Get", &RdInteger::Get)
        .def("IsValid", &RdInteger::IsValid);

    py::class_<RdTypeInstanceAttribute>(m, "RdTypeInstanceAttribute")
        .def_readwrite("name", &RdTypeInstanceAttribute::name)
        .def_readwrite("string_value", &RdTypeInstanceAttribute::string_value)
        .def_readwrite("integer_value", &RdTypeInstanceAttribute::integer_value)
        .def_readwrite("link_value", &RdTypeInstanceAttribute::link_value)
        .def_readwrite("enum_value", &RdTypeInstanceAttribute::enum_value);

    py::class_<RdTypeInstance>(m, "RdTypeInstance")
        .def_readwrite("type", &RdTypeInstance::type)
        .def_readwrite("file_origin", &RdTypeInstance::file_origin)
        .def_readwrite("attributes", &RdTypeInstance::attributes);


    py::enum_<LogLevel>(m, "LogLevel")
        .value("ERROR", LogLevel::ERROR)
        .value("WARNING", LogLevel::WARNING)
        .value("INFO", LogLevel::INFO)
        .value("DBUG", LogLevel::DBUG);

    py::class_<Logger>(m, "Logger")
        .def(py::init<>())
        .def(py::init<const std::string>())
        .def("SetLogLevel", &Logger::SetLogLevel)
        .def("GetCurrentLogLevel", &Logger::GetCurrentLogLevel)
        .def("LogMessage", &Logger::LogMessage, 
             py::arg("loglevel") = LogLevel::WARNING, py::arg("message") = "", py::arg("filename") = "Unset", py::arg("line_number") = -1);


    py::class_<FileEngine>(m, "FileEngine")
        .def(py::init<Logger&>())
        .def("SetSearchRecursive", &FileEngine::SetSearchRecursive)
        .def("GetSearchRecursive", &FileEngine::GetSearchRecursive)
        .def("SetDirectory", &FileEngine::SetDirectory);


    py::enum_<ParseResult>(m, "ParseResult")
        .value("NoExceptionOccurred", ParseResult::NoExceptionOccurred)
        .value("ExceptionOccurred", ParseResult::ExceptionOccurred);

    py::class_<RelParser>(m, "RelParser")
        .def(py::init<Logger&, FileEngine const&>())
        .def("ProcessRelModel", &RelParser::ProcessRelModel)
        .def("GetDatabase", &RelParser::GetDatabase);

    m.doc() = "REL - Python integration"; // optional module docstring
}
