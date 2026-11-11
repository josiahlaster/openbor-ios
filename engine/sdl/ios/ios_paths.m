/*
 * OpenBOR - iOS Platform Paths Implementation
 * Native Objective-C (Foundation/UIKit) on Apple platforms;
 * Portable C fallback on non-Apple hosts for static analysis & development.
 */

#include "ios_paths.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#ifdef __APPLE__
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

static char g_documents_path[1024] = {0};
static char g_bundle_path[1024] = {0};
static char g_caches_path[1024] = {0};

const char* ios_get_documents_dir(void)
{
    if (g_documents_path[0] == '\0') {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        if ([paths count] > 0) {
            NSString *docDir = [paths objectAtIndex:0];
            strncpy(g_documents_path, [docDir UTF8String], sizeof(g_documents_path) - 1);
        }
    }
    return g_documents_path;
}

const char* ios_get_bundle_dir(void)
{
    if (g_bundle_path[0] == '\0') {
        NSString *bundleDir = [[NSBundle mainBundle] resourcePath];
        if (bundleDir) {
            strncpy(g_bundle_path, [bundleDir UTF8String], sizeof(g_bundle_path) - 1);
        }
    }
    return g_bundle_path;
}

const char* ios_get_caches_dir(void)
{
    if (g_caches_path[0] == '\0') {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        if ([paths count] > 0) {
            NSString *cacheDir = [paths objectAtIndex:0];
            strncpy(g_caches_path, [cacheDir UTF8String], sizeof(g_caches_path) - 1);
        }
    }
    return g_caches_path;
}

static void ensure_directory(const char *path)
{
    struct stat st;
    if (stat(path, &st) != 0) {
        mkdir(path, 0755);
    }
}

void ios_init_paths(char *paks, char *saves, char *logs, char *screenshots)
{
    const char *docRoot = ios_get_documents_dir();
    char buf[1024];

    // Documents/Paks
    snprintf(buf, sizeof(buf), "%s/Paks", docRoot);
    ensure_directory(buf);
    if (paks) strncpy(paks, buf, 255);

    // Documents/Saves
    snprintf(buf, sizeof(buf), "%s/Saves", docRoot);
    ensure_directory(buf);
    if (saves) strncpy(saves, buf, 255);

    // Documents/Logs
    snprintf(buf, sizeof(buf), "%s/Logs", docRoot);
    ensure_directory(buf);
    if (logs) strncpy(logs, buf, 255);

    // Documents/ScreenShots
    snprintf(buf, sizeof(buf), "%s/ScreenShots", docRoot);
    ensure_directory(buf);
    if (screenshots) strncpy(screenshots, buf, 255);
}

void ios_get_safe_area_insets(float *top, float *bottom, float *left, float *right)
{
    if (!top || !bottom || !left || !right) return;
    *top = 0.0f;
    *bottom = 0.0f;
    *left = 0.0f;
    *right = 0.0f;

    if (@available(iOS 11.0, *)) {
        dispatch_sync(dispatch_get_main_queue(), ^{
            UIWindow *window = [UIApplication sharedApplication].keyWindow;
            if (!window) {
                NSArray *windows = [UIApplication sharedApplication].windows;
                if ([windows count] > 0) {
                    window = [windows objectAtIndex:0];
                }
            }
            if (window) {
                UIEdgeInsets insets = window.safeAreaInsets;
                *top = (float)insets.top;
                *bottom = (float)insets.bottom;
                *left = (float)insets.left;
                *right = (float)insets.right;
            }
        });
    }
}

#else
/* Non-Apple fallback implementation for Windows development and static analysis */

static char g_documents_path[1024] = "Documents";
static char g_bundle_path[1024] = "OpenBOR.app";
static char g_caches_path[1024] = "Library/Caches";

const char* ios_get_documents_dir(void)
{
    return g_documents_path;
}

const char* ios_get_bundle_dir(void)
{
    return g_bundle_path;
}

const char* ios_get_caches_dir(void)
{
    return g_caches_path;
}

void ios_init_paths(char *paks, char *saves, char *logs, char *screenshots)
{
    if (paks) strncpy(paks, "Paks", 255);
    if (saves) strncpy(saves, "Saves", 255);
    if (logs) strncpy(logs, "Logs", 255);
    if (screenshots) strncpy(screenshots, "ScreenShots", 255);
}

void ios_get_safe_area_insets(float *top, float *bottom, float *left, float *right)
{
    if (top) *top = 0.0f;
    if (bottom) *bottom = 0.0f;
    if (left) *left = 0.0f;
    if (right) *right = 0.0f;
}

#endif
