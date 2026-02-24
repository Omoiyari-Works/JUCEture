# AndroidGUISampleProject

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Android-green.svg)](https://www.android.com/)

A sample Android GUI application project built with JUCE framework and native C++.

## 📋 Table of Contents

- [AndroidGUISampleProject](#androidguisampleproject)
  - [📋 Table of Contents](#-table-of-contents)
  - [✨ Features](#-features)
  - [🔧 Requirements](#-requirements)
  - [📦 Installation](#-installation)
  - [📍 JUCE Directory Configuration](#-juce-directory-configuration)
    - [Recommended Check Procedure](#recommended-check-procedure)
      - [Step 1: Check Relative Path Location](#step-1-check-relative-path-location)
      - [Step 2: Check Environment Variable or CMake Variable](#step-2-check-environment-variable-or-cmake-variable)
      - [Step 3: Configure JUCE Path (if not found above)](#step-3-configure-juce-path-if-not-found-above)
    - [Configuration Methods (Recommended Order)](#configuration-methods-recommended-order)
      - [Method 1: Place JUCE Within Relative Path Range from JUCEture (Most Recommended)](#method-1-place-juce-within-relative-path-range-from-juceture-most-recommended)
      - [Method 2: Set Environment Variable or CMake Variable](#method-2-set-environment-variable-or-cmake-variable)
    - [Troubleshooting](#troubleshooting)
    - [For External Projects Using JUCEture](#for-external-projects-using-juceture)
  - [🚀 Usage](#-usage)
    - [Integration Steps](#integration-steps)
      - [1. Add Source/java to build.gradle](#1-add-sourcejava-to-buildgradle)
      - [2. Add Source/cpp to CMakeLists.txt](#2-add-sourcecpp-to-cmakeliststxt)
      - [3. Initialize JUCEture](#3-initialize-juceture)
      - [4. Implement Gesture Handlers in Your Components](#4-implement-gesture-handlers-in-your-components)
        - [Single Tap](#single-tap)
        - [Drag](#drag)
        - [Pinch](#pinch)
        - [Long Tap](#long-tap)
    - [Building and Running](#building-and-running)
      - [Build the project in Android Studio](#build-the-project-in-android-studio)
      - [Build the project in command-line](#build-the-project-in-command-line)
  - [📱 How to Use the Sample App](#-how-to-use-the-sample-app)
    - [Overview](#overview)
    - [Features](#features)
    - [Usage Tips](#usage-tips)
    - [Visual Indicators](#visual-indicators)
  - [📁 Project Structure](#-project-structure)
  - [🤝 Contributing](#-contributing)
    - [Code Style](#code-style)
  - [📄 License](#-license)
  - [🙏 Acknowledgments](#-acknowledgments)
  - [📞 Contact](#-contact)

## ✨ Features

- **JUCEture Framework**: This is JUCEture (JUCE + gesture), a framework for smartphone applications that uses the gesture detection functionality provided by the OS
- **OS Gesture Integration**: Utilizes native OS gesture detection capabilities for mobile applications
- **Android Support**: Currently supports Android platform only
- Native Android GUI application
- JUCE framework integration
- C++ native code support
- CMake build system
- Support for multiple Android ABIs (armeabi-v7a, arm64-v8a, x86, x86_64)

## 🔧 Requirements

- Android Studio (latest version recommended)
- Android NDK (version 28.1.13356709 or compatible)
- CMake (version 3.22.1 or higher)
- Gradle
- JDK 8 or higher
- Android SDK (API level 24 or higher)

## 📦 Installation

1. Clone this repository:
```bash
git clone https://github.com/yourusername/AndroidGUISampleProject.git
cd AndroidGUISampleProject
```

2. Open the project in Android Studio:
   - Launch Android Studio
   - Select "Open an existing project"
   - Navigate to `sampleApp/android` directory

3. Sync Gradle files:
   - Android Studio will automatically sync Gradle files
   - Wait for the sync to complete

## 📍 JUCE Directory Configuration

This project requires access to the JUCE framework directory. Follow the recommended check procedure below to configure JUCE location:

### Recommended Check Procedure

#### Step 1: Check Relative Path Location

First, check if JUCE is located at one of the following relative paths from JUCEture:

- `../../../JUCE` (default for this sample project)
- `../../JUCE`
- `../JUCE`
- `./JUCE`
- `./external/JUCE`
- `../external/JUCE`
- `../../external/JUCE`

**If JUCE is found at any of these locations:** No additional configuration is needed. This is the recommended approach, as JUCE can be located anywhere within the relative path search range from JUCEture.

#### Step 2: Check Environment Variable or CMake Variable

If JUCE is not found in the relative paths above, check if one of the following is set:

1. **Environment variable `JUCE_DIR`**
   - Check in your shell: `echo $JUCE_DIR` (Linux/Mac) or `echo $env:JUCE_DIR` (Windows PowerShell)
   - If not set, you can set it (see configuration methods below)

2. **CMake variable `JUCE_DIR`**
   - Check if it's passed via CMake command: `cmake -DJUCE_DIR=/path/to/juce ...`
   - Or check in `build.gradle` if it's configured in CMake arguments

**If either is set:** The build system will use that path to locate JUCE. No additional configuration is needed.

#### Step 3: Configure JUCE Path (if not found above)

If JUCE is not found in the relative paths and neither environment variable nor CMake variable is set, you need to configure JUCE path using one of the following methods (recommended in order):

### Configuration Methods (Recommended Order)

#### Method 1: Place JUCE Within Relative Path Range from JUCEture (Most Recommended)

Place JUCE within the relative path search range from JUCEture. The build system will automatically search for JUCE in the following relative paths:

- `../../../JUCE`
- `../../JUCE`
- `../JUCE`
- `./JUCE`
- `./external/JUCE`
- `../external/JUCE`
- `../../external/JUCE`

If JUCE is located at any of these paths, no additional configuration is needed. This is the simplest and most maintainable approach.

#### Method 2: Set Environment Variable or CMake Variable

If JUCE is not located within the relative path search range from JUCEture, use either environment variable or CMake variable (choose whichever is more convenient for your setup):

**Option A: Environment Variable**

Set the `JUCE_DIR` environment variable:

**Linux/Mac:**
```bash
export JUCE_DIR=/path/to/your/JUCE
```

**Windows (PowerShell):**
```powershell
$env:JUCE_DIR = "C:\path\to\your\JUCE"
```

**Windows (Command Prompt):**
```cmd
set JUCE_DIR=C:\path\to\your\JUCE
```

**Option B: CMake Variable**

Pass the JUCE directory path directly to CMake:

```bash
cmake -DJUCE_DIR=/path/to/your/JUCE -B build
```

Or in Gradle, add to `build.gradle`:

```gradle
android {
    defaultConfig {
        externalNativeBuild {
            cmake {
                arguments "-DJUCE_DIR=/path/to/your/JUCE",
                          // ... other arguments
            }
        }
    }
}
```

### Troubleshooting

If you encounter an error indicating that JUCE modules directory was not found:

1. **Verify JUCE installation**: Ensure that JUCE is properly cloned/installed and contains the `modules` directory
2. **Check path format**: On Windows, use forward slashes (`/`) or escape backslashes (`\\`) in paths
3. **Verify environment variable**: Check that `JUCE_DIR` is set correctly:
   ```bash
   echo $JUCE_DIR  # Linux/Mac
   ```
   
   ```powershell
   echo $env:JUCE_DIR  # Windows PowerShell
   ```
4. **Check build logs**: The build system will show which paths were searched if JUCE is not found

### For External Projects Using JUCEture

When integrating JUCEture into your existing project that already has JUCE:

1. **Recommended**: Place JUCE within the relative path search range from JUCEture (see Step 1 above)
2. **If JUCE is not within the relative path range**: Use environment variable `JUCE_DIR` or CMake variable `JUCE_DIR` to point to your JUCE directory (see Step 2 and Configuration Methods above)
3. The JUCEture framework will automatically locate and use your JUCE installation

## 🚀 Usage

### Integration Steps

To integrate JUCEture framework into your Android application, follow these steps:

#### 1. Add Source/java to build.gradle

Add `Source/java` to your `build.gradle` file in the `sourceSets` section. It is recommended to include directory existence checks to catch configuration errors early:

```gradle
sourceSets {
    def javaSourceDirs = [
        "../../../JUCE/modules/juce_core/native/javacore/init",
        "../../../JUCE/modules/juce_core/native/javacore/app",
        "../../../JUCE/modules/juce_gui_basics/native/javaopt/app",
        "../../../Source/java"  // Add this line
    ]
    
    // Check if all source directories exist and throw error if not
    javaSourceDirs.each { dir ->
        def dirFile = file(dir)
        if (!dirFile.exists()) {
            throw new Exception("Java source directory not found: ${dirFile.absolutePath}. Please check the path in build.gradle.")
        }
    }
    
    main.java.srcDirs += javaSourceDirs

    main.res.srcDirs +=
        []
}
```

**Note**: The directory existence check will cause the build to fail immediately if any of the specified directories are missing, helping you catch configuration errors early. Without this check, you may only see a warning message during compilation, and the build might succeed even if the directory is missing.

#### 2. Add Source/cpp to CMakeLists.txt

This framework can be easily integrated using CMake. Add the following line to your `CMakeLists.txt`:

```cmake
add_subdirectory(../../../Source/cpp Source)
```

#### 3. Initialize JUCEture

Call `JUCEtureAPI::initialize()` in your application initialization code. It is recommended to call this from a component that remains active throughout the application lifecycle, such as `MainComponent`:

```cpp
#include "JUCEtureAPI.h"

// In MainComponent constructor or similar long-lived component
MainComponent::MainComponent()
{
    // Initialize JUCEture framework
    JUCEtureAPI::initialize();
    
    // ... rest of your initialization code
}
```

#### 4. Implement Gesture Handlers in Your Components

To receive gesture notifications, your Component needs to implement the appropriate handler interfaces.

##### Single Tap

Make your Component inherit from `ISingleTapHandler` and implement the required method:

```cpp
#include "ISingleTapHandler.h"
#include "SingleTapEvent.h"

class MyComponent : public juce::Component, public ISingleTapHandler
{
public:
    void onSingleTap(const SingleTapEvent& event) override
    {
        // Handle single tap gesture
        // event.getLocal() - local coordinates
        // event.getGlobal() - global coordinates
    }
};
```

##### Drag

Make your Component inherit from `IDragHandler` and implement the required methods:

```cpp
#include "IDragHandler.h"
#include "DragStartEvent.h"
#include "DragMoveEvent.h"
#include "DragEndEvent.h"

class MyComponent : public juce::Component, public IDragHandler
{
public:
    void onDragStart(const DragStartEvent& event) override
    {
        // Handle drag start
    }

    void onDragMove(const DragMoveEvent& event) override
    {
        // Handle drag move
    }

    void onDragEnd(const DragEndEvent& event) override
    {
        // Handle drag end
    }
};
```

##### Pinch

Make your Component inherit from `IPinchHandler` and implement the required methods:

```cpp
#include "IPinchHandler.h"
#include "PinchStartEvent.h"
#include "PinchScaleEvent.h"
#include "PinchEndEvent.h"

class MyComponent : public juce::Component, public IPinchHandler
{
public:
    void onPinchStart(const PinchStartEvent& event) override
    {
        // Handle pinch start
    }

    void onPinchScale(const PinchScaleEvent& event) override
    {
        // Handle pinch scale
    }

    void onPinchEnd(const PinchEndEvent& event) override
    {
        // Handle pinch end
    }
};
```

##### Long Tap

Make your Component inherit from `ILongTapHandler` and implement the required method:

```cpp
#include "ILongTapHandler.h"
#include "LongTapEvent.h"

class MyComponent : public juce::Component, public ILongTapHandler
{
public:
    void onLongTap(const LongTapEvent& event) override
    {
        // Handle long tap gesture
        // event.getLocalPoint() - local coordinates
        // event.getGlobalPoint() - global coordinates
    }
};
```

**Note**: Long tap detection is handled by the OS (Android) and automatically notified to the appropriate handler. The gesture is detected when the user presses and holds on the screen for a certain duration. The `onLongTap` method will be called when a long tap is detected at a position within your component's bounds.

### Building and Running

#### Build the project in Android Studio

1. Select the desired build variant (debug_ or release_)
2. Click "Build" → "Make Project" or press `Ctrl+F9` (Windows/Linux) / `Cmd+F9` (Mac)
3. Connect an Android device or start an emulator
4. Install the APK:
   ```bash
   adb install -r app/build/outputs/apk/debug_/debug/app-debug_-debug.apk
   ```
5. Launch the application from your device

#### Build the project in command-line

You can also build and install the project from the command-line. From the root directory of the project, run:

```bash
.\sampleApp\android\gradlew.bat -p .\sampleApp\android :app:assembleDebug :app:installDebug_Debug
```

This command will:
- Build the debug APK
- If an Android device is connected, automatically install the APK to the device

After installation, launch the application from your device.

## 📱 How to Use the Sample App

This sample application demonstrates touch gesture handling on Android using JUCE framework.

### Overview

The app displays three colored boxes (Top, Middle, and Bottom) that respond to various touch gestures. The interface shows status information at the top of the screen.

### Features

- **Gesture Test Boxes**: Three semi-transparent boxes arranged vertically
  - Top Box: Blue-tinted (RGB: 0x33, 0x66, 0x99)
  - Middle Box: Green-tinted (RGB: 0x66, 0x99, 0x33)
  - Bottom Box: Orange-tinted (RGB: 0x99, 0x66, 0x33)

- **Touch Gestures**:
  - **Single Tap**: Tap on any box to test single tap detection
  - **Drag**: Press and drag on the screen to see a drag indicator circle
  - **Pinch**: Use two fingers to perform pinch gestures
  - **Long Tap**: Press and hold on the screen to test long tap detection

- **Status Display**:
  - Two status labels at the top of the screen show coordinate information
  - Yellow label: Raw coordinate data
  - Cyan label: Converted coordinate data
  - Status labels appear during gesture interactions

### Usage Tips

1. **Testing Gestures**: Try tapping, dragging, and pinching on different boxes to see how gestures are detected
2. **Coordinate Information**: Watch the status labels to see the coordinate values during interactions
3. **System Bar Insets**: The app automatically adjusts for system bars (status bar and navigation bar) to ensure content is displayed in the safe area

### Visual Indicators

- **Drag Indicator**: When dragging, an orange circle appears at the current touch position
- **Content Bounds**: A dark blue border shows the safe content area (excluding system bars)
- **Background**: Cyan background color

## 📁 Project Structure

```
AndroidGUISampleProject/
├── sampleApp/
│   ├── android/          # Android project files
│   │   ├── app/          # Application module
│   │   │   ├── CMakeLists.txt
│   │   │   └── build.gradle
│   │   └── build.gradle  # Project-level Gradle file
│   └── cpp/              # C++ source files
├── Source/               # Additional source files
├── JUCE/                 # JUCE framework
└── README                # This file
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Code Style

- Follow the existing code style
- Use 4 spaces for indentation
- Write meaningful commit messages
- Add comments for complex logic

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- [JUCE Framework](https://juce.com/) - Cross-platform C++ application framework
- Android Open Source Project

## 📞 Contact

For questions or support, please open an issue on GitHub.

---

**Note**: This is a sample project. Modify it according to your needs.

