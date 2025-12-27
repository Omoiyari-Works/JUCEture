#pragma once

#include <JuceHeader.h>

namespace JUCEtureAPI
{
/**
 * Initialize JUCEture framework.
 * This function must be called once at application startup (e.g., in MainComponent constructor).
 * On Android, this attaches the gesture listener to the main View.
 *
 * @return true if initialization succeeded, false otherwise
 */
bool initialize();
} // namespace JUCEtureAPI
