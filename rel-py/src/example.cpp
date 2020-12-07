#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

using namespace std;

namespace py = pybind11;

class Data
{
public:
    Data() {}
    ~Data() {}

    int index;
    int fibo;
};


int add(int i, int j) {
    return i + j;
}


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



PYBIND11_MODULE(librel_py, m) {
    py::class_<Data>(m, "Data")
        .def(py::init<>())
        .def_readwrite("index", &Data::index)
        .def_readwrite("fibo", &Data::fibo);
    
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");
    m.def("fibonacci", &fibonacci, "A function which returns fibonacci numbers");
}
