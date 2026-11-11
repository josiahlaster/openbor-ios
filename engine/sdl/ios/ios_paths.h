/*
 * OpenBOR - iOS Platform Paths Interface
 */

#ifndef IOS_PATHS_H
#define IOS_PATHS_H

#ifdef __cplusplus
extern "C" {
#endif

// Returns the sandboxed Documents directory (writable by app and accessible via Files app)
const char* ios_get_documents_dir(void);

// Returns the app bundle Resources directory (read-only shipped assets)
const char* ios_get_bundle_dir(void);

// Returns the Library/Caches directory (for temporary logs and caches)
const char* ios_get_caches_dir(void);

// Initializes OpenBOR directory structure inside the sandbox:
// Creates Documents/Paks, Documents/Saves, Documents/Logs, Documents/ScreenShots
void ios_init_paths(char *paks, char *saves, char *logs, char *screenshots);

// Copies any bundled .pak files from the app bundle to Documents/Paks if not already present
void ios_sync_bundle_paks(const char *documentsPaksDir);

// Returns the device display safe area insets in points
void ios_get_safe_area_insets(float *top, float *bottom, float *left, float *right);

#ifdef __cplusplus
}
#endif

#endif /* IOS_PATHS_H */
