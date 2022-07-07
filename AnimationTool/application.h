//----------------------------------------------------------------------------//
//                                                                            //
// ozz-animation is hosted at http://github.com/guillaumeblanc/ozz-animation  //
// and distributed under the MIT License (MIT).                               //
//                                                                            //
// Copyright (c) Guillaume Blanc                                              //
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included in //
// all copies or substantial portions of the Software.                        //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //
// DEALINGS IN THE SOFTWARE.                                                  //
//                                                                            //
//----------------------------------------------------------------------------//

#ifndef APPLICATION_H_
#define APPLICATION_H_


#include <cstddef>

#include "ozz/base/containers/string.h"
#include "ozz/base/memory/unique_ptr.h"

namespace ozz {
    namespace math {
        struct Box;
        struct Float2;
        struct Float3;
        struct Float4x4;
    }  // namespace math
        class ImGui;
        class Renderer;
        class Record;

        namespace internal {
            class ImGuiImpl;
            class RendererImpl;
            class Camera;
            class Shooter;
        }  // namespace internal

        // Screen resolution settings.
        struct Resolution {
            int width;
            int height;
        };

        class Application {
        public:
            // Creates a window and initialize GL context.
            // Any failure during initialization or loop execution will be silently
            // handled, until the call to ::Run that will return EXIT_FAILURE.
            Application();

            // Destroys the application. Cleans up everything.
            virtual ~Application();

            int Run(LPVOID pParam);

        protected:
            // Allows application to convert from world space to screen coordinates.
            math::Float2 WorldToScreen(const math::Float3& _world) const;

        public:
            

            virtual bool OnInitialize();
            virtual void OnDestroy();
            virtual bool OnUpdate(float _dt, float _time);
            virtual bool OnGui(ImGui* _im_gui);
            virtual bool OnFloatingGui(ImGui* _im_gui);
            virtual bool OnDisplay(Renderer* _renderer);
            virtual bool GetCameraInitialSetup(math::Float3* _center, math::Float2* _angles, float* _distance) const;
            virtual bool GetCameraOverride(math::Float4x4* _transform) const;
            virtual void GetSceneBounds(math::Box* _bound) const;
            bool Loop();
            friend void OneLoopCbk(GLFWwindow* RenderWindow, void*);
            enum LoopStatus {
                kContinue,      // Can continue with next loop.
                kBreak,         // Should stop looping (ex: exit).
                kBreakFailure,  // // Should stop looping beacause something went wrong.
            };
            LoopStatus OneLoop(int _loops);
            bool Idle(bool _first_frame);
            bool Display();
            bool Gui();
            bool FrameworkGui();
            static void ResizeCbk(GLFWwindow* RenderWindow, int _width, int _height);
            static void CloseCbk(GLFWwindow* RenderWindow);

            static bool closeProc;
            static bool ProcResize;
           
            void ParseReadme();
            Application(const Application& _application);
            void operator=(const Application& _application);
            static Application* application_;
            bool exit_;
            bool freeze_;
            bool fix_update_rate;
            float fixed_update_rate;
            float time_factor_;
            float time_;
            double last_idle_time_;
            unique_ptr<internal::Camera> camera_;
            unique_ptr<internal::Shooter> shooter_;
            bool show_help_;
            bool show_grid_;
            bool show_axes_;
            bool capture_video_;
            bool capture_screenshot_;
            unique_ptr<internal::RendererImpl> renderer_;
            unique_ptr<internal::ImGuiImpl> im_gui_;
            unique_ptr<Record> fps_;
            unique_ptr<Record> update_time_;
            unique_ptr<Record> render_time_;
            Resolution resolution_;

            ozz::string help_;
        };



}  // namespace ozz

#endif
