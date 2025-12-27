#include "JUCEtureAPI.h"
#if JUCE_ANDROID
#include "NotifierGestureFromOSJNI.h"
#endif

namespace JUCEtureAPI
{
bool initialize()
{
#if JUCE_ANDROID
    if (!attachGestureListenerOnce())
        return false;
    initializeDetectors();
    return true;
#else
    return true;
#endif
}
} // namespace JUCEtureAPI
