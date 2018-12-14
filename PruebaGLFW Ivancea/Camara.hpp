#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <math.h>         // Used only for sin() and cos() functions

#include <GL/glfw.h>      // Include OpenGL Framework library for the GLFW_PRESS constant only!

#include "Vec.hpp"

#include "Objeto.hpp"

class Camera
{
    protected:
        // Camera position
        Vec3<double> position;

        // Camera rotation
        Vec3<double> rotation;

        // Camera movement speed. When we call the move() function on a camera, it moves using these speeds
        Vec3<double> speed;

        double movementSpeedFactor; // Controls how fast the camera moves
        double pitchSensitivity;    // Controls how sensitive mouse movements affect looking up and down
        double yawSensitivity;      // Controls how sensitive mouse movements affect looking left and right

        // Window size in pixels and where the midpoint of it falls
        int windowWidth;
        int windowHeight;
        int windowMidX;
        int windowMidY;

        // Method to set some reasonable default values. For internal use by the class only.
        void initCamera();

    public:
        static const double TO_RADS; // The value of 1 degree in radians

        // Holding any keys down?
        bool holdingForward;
        bool holdingBackward;
        bool holdingLeftStrafe;
        bool holdingRightStrafe;

        // Constructors
        Camera(float windowWidth, float windowHeight);

        // Destructor
        ~Camera();

        // Mouse movement handler to look around
        void handleMouseMove(int mouseX, int mouseY);

        // Method to convert an angle in degress to radians
        const double toRads(const double &angleInDegrees) const;

        void move(double, double, double);
        void changeSpeed(double speed){movementSpeedFactor = speed;}
        // Method to move the camera based on the current direction
        void move(double deltaTime);

        // --------------------------------- Inline methods ----------------------------------------------

        // Setters to allow for change of vertical (pitch) and horizontal (yaw) mouse movement sensitivity
        float getPitchSensitivity()            { return pitchSensitivity;  }
        void  setPitchSensitivity(float value) { pitchSensitivity = value; }
        float getYawSensitivity()              { return yawSensitivity;    }
        void  setYawSensitivity(float value)   { yawSensitivity   = value; }

        // Position getters
        Vec3<double> getPosition() const { return position;        }
        double getXPos()           const { return position.x; }
        double getYPos()           const { return position.y; }
        double getZPos()           const { return position.z; }

        // Rotation getters
        Vec3<double> getRotation() const { return rotation;        }
        double getXRot()           const { return rotation.x; }
        double getYRot()           const { return rotation.y; }
        double getZRot()           const { return rotation.z; }
};

#endif // CAMERA_H
