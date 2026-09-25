#pragma once

#if JUCE_ANDROID
/**
 * Attach gesture listener to the main View, then keep watching for
 * additional windows (e.g. addToDesktop() dialogs) in the background so
 * they get the listener attached too, and detach it when those windows
 * are destroyed. This function should be called once at application startup.
 *
 * @return true if attachment succeeded, false otherwise
 */
bool attachGestureListenerOnce();

/**
 * Initialize detector instances.
 * This function should be called once at application startup.
 */
void initializeDetectors();
#endif
