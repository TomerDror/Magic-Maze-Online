#include <stdlib.h> 

int main(){
    system("Python PythonComPileTest.py");
    return 0;
}

// #include <Python.h>
// #include <thread>

// // Function to execute Python code
// void runPythonCode() {
//     PyGILState_STATE gstate;
//     gstate = PyGILState_Ensure();

//     // Execute Python code here
//     PyRun_SimpleString("print('Hello from Python!')");

// }

//     PyGILState_Release(gstate);
// int main() {
//     // Initialize Python interpreter
//     Py_Initialize();

//     // Acquire GIL before starting the thread
//     PyEval_InitThreads();

//     // Start a new thread to execute Python code
//     std::thread pythonThread(runPythonCode);

//     // Join the thread
//     pythonThread.join();

//     // Release GIL and finalize Python interpreter
//     Py_Finalize();

//     return 0;
// }
