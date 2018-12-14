#include <string>
#include <sstream>

#ifndef __glfw_h_
    #include <GL/glfw.h> // We need GLFW for this, so let's check for it- although it'd be a doddle to convert to non-GLFW using code.
#endif


class FpsManager{

    private:
        double frameStartTime;         // Frame start time
        double frameEndTime;           // Frame end time
        double frameDuration;          // How many milliseconds between the last frame and this frame

        double targetFps;              // The desired FPS to run at (i.e. maxFPS)
        double currentFps;             // The current FPS value
        int    frameCount;             // How many frames have been drawn s

        double targetFrameDuration;    // How many milliseconds each frame should take to hit a target FPS value (i.e. 60fps = 1.0 / 60 = 0.016ms)
        double sleepDuration;          // How long to sleep if we're exceeding the target frame rate duration

        double lastReportTime;         // The timestamp of when we last reported
        double reportInterval;         // How often to update the FPS value

        std::string windowTitle;       // Window title to update view GLFW

        bool verbose;                  // Whether or not to output FPS details to the console or update the window

        // Limit the minimum and maximum target FPS value to relatively sane values
        const double MIN_TARGET_FPS = 20.0;
        const double MAX_TARGET_FPS = 60.0; // If you set this above the refresh of your monitor and enable VSync it'll break! Be aware!

        // Private method to set relatively sane defaults. Called by constructors before overwriting with more specific values as required.
        void init(double theTargetFps, bool theVerboseSetting)
        {
            setTargetFps(theTargetFps);

            frameCount     = 0;
            currentFps     = 0.0;
            sleepDuration  = 0.0;
            frameStartTime = glfwGetTime();
            frameEndTime   = frameStartTime + 1;
            frameDuration  = 1;
            lastReportTime = frameStartTime;
            reportInterval = 1.0f;
            windowTitle    = "NONE";
            verbose        = theVerboseSetting;
        }

    public:

        // Single parameter constructor - just set a desired framerate and let it go.
        // Note: No FPS reporting by default, although you can turn it on or off later with the setVerbose(true/false) method
        FpsManager(int theTargetFps)
        {
            init(theTargetFps, false);
        }

        // Two parameter constructor which sets a desired framerate and a reporting interval in seconds
        FpsManager(int theTargetFps, double theReportInterval)
        {
            init(theTargetFps, true);

            setReportInterval(theReportInterval);
        }

        // Three parameter constructor which sets a desired framerate, how often to report, and the window title to append the FPS to
        FpsManager(int theTargetFps, float theReportInterval, std::string theWindowTitle)
        {
            init(theTargetFps, true); // If you specify a window title it's safe to say you want the FPS to update there ;)

            setReportInterval(theReportInterval);

            windowTitle = theWindowTitle;
        }

        // Getter and setter for the verbose property
        bool getVerbose()
        {
            return verbose;
        }
        void setVerbose(bool theVerboseValue)
        {
            verbose = theVerboseValue;
        }

        // Getter and setter for the targetFps property
        int getTargetFps()
        {
            return targetFps;
        }

        void setTargetFps(int theFpsLimit)
        {
            // Make at least some attempt to sanitise the target FPS...
            if (theFpsLimit < MIN_TARGET_FPS)
            {
                theFpsLimit = MIN_TARGET_FPS;
                std::cout << "Limiting FPS rate to legal minimum of " << MIN_TARGET_FPS << " frames per second." << std::endl;
            }
            if (theFpsLimit > MAX_TARGET_FPS)
            {
                theFpsLimit = MAX_TARGET_FPS;
                std::cout << "Limiting FPS rate to legal maximum of " << MAX_TARGET_FPS << " frames per second." << std::endl;
            }

            // ...then set it and calculate the target duration of each frame at this framerate
            targetFps = theFpsLimit;
            targetFrameDuration = 1.0 / targetFps;
        }

        double getFrameDuration() { return frameDuration; } // Returns the time it took to complete the last frame in milliseconds

        // Setter for the report interval (how often the FPS is reported) - santises input.
        void setReportInterval(float theReportInterval)
        {
            // Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
            // Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
            if (theReportInterval < 0.1)
            {
                theReportInterval = 0.1;
            }
            if (theReportInterval > 10.0)
            {
                theReportInterval = 10.0;
            }
            reportInterval = theReportInterval;
        }

        // Method to force our application to stick to a given frame rate and return how long it took to process a frame
        double enforceFPS()
        {
            // Get the current time
            frameEndTime = glfwGetTime();

            // Calculate how long it's been since the frameStartTime was set (at the end of this method)
            frameDuration = frameEndTime - frameStartTime;

            if (reportInterval != 0.0f)
            {

                // Calculate and display the FPS every specified time interval
                if ((frameEndTime - lastReportTime) > reportInterval)
                {
                    // Update the last report time to be now
                    lastReportTime = frameEndTime;

                    // Calculate the FPS as the number of frames divided by the interval in seconds
                    currentFps =  (double)frameCount / reportInterval;

                    // Reset the frame counter to 1 (and not zero - which would make our FPS values off)
                    frameCount = 1;

                    if (verbose)
                    {
                        //std::cout << "FPS: " << currentFps << std::endl;

                        // If the user specified a window title to append the FPS value to...
                        if (windowTitle != "NONE")
                        {
                            // Convert the fps value into a string using an output stringstream
                            std::ostringstream stream;
                            stream << currentFps;
                            std::string fpsString = stream.str();

                            // Append the FPS value to the window title details
                            std::string tempWindowTitle = windowTitle + " | FPS: " + fpsString;

                            // Convert the new window title to a c_str and set it
                            const char* pszConstString = tempWindowTitle.c_str();
                            glfwSetWindowTitle(pszConstString);
                        }

                    } // End of if verbose section

                }
                else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
                {
                    ++frameCount;
                }

            } // End of if we specified a report interval section

            // Calculate how long we should sleep for to stick to our target frame rate
            sleepDuration = targetFrameDuration - frameDuration;

            // If we're running faster than our target duration, sleep until we catch up!
            if (sleepDuration > 0.0)
                glfwSleep(targetFrameDuration - frameDuration);

            // Reset the frame start time to be now - this means we only need put a single call into the main loop
            frameStartTime = glfwGetTime();

            // Pass back our total frame duration (including any sleep and the time it took to run this function) to be used as our deltaTime value
            return frameDuration + (frameStartTime - frameEndTime);

        } // End of our enforceFPS method

};
