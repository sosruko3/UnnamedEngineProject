GOAL: Creating High-performance 3D Game engine, similar to ID tech 7, but with the scale of a solo developer.
Currently planned to integrate libraries: Jolt physics,ozz-animation,dear imgui,spdlog,miniaudio,SDL3,NRI,EnkiTS,cr.h and more.

AI is being used in this project, but not as a "agent" ,I am handling all the design myself, while also brainstorming and researching which is why every release takes weeks, not good at choosing things. There might be occasional AI-assisted code, that passed my code reviews.
Current release is 0.03. Planning to either quickly refactor old systems to c++ structs/classes or start the renderer finally, might also integrate Jolt physics. haven't decided yet.




# How to Build
  git clone https://github.com/sosruko3/UnnamedEngineProject.git &&
  cd UnnamedEngineProject &&
  cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build
