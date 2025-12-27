#pragma once

#if JUCE_ANDROID
/**
 * Attach gesture listener to the main View.
 * This function should be called once at application startup.
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
