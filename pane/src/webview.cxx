#include <pane/webview.hxx>

namespace pane {
webview::webview() {
    if (environment_options.options) {
        if (!environment_options.AdditionalBrowserArguments.empty()) {
            // if (auto converted {
            //         glow::text::u16string(config.environmentOptions.AdditionalBrowserArguments)
            //         };
            //     converted) {
            //     createdEnvironmentOptions->put_AdditionalBrowserArguments(
            //         glow::text::c_str(converted.value()));
            // }
        }

        environment_options.options->put_AllowSingleSignOnUsingOSPrimaryAccount(
            environment_options.AllowSingleSignOnUsingOSPrimaryAccount);

        if (!environment_options.Language.empty()) {
            // if (auto converted { glow::text::u16string(config.environmentOptions.Language) };
            //     converted) {
            //     createdEnvironmentOptions->put_Language(glow::text::c_str(converted.value()));
            // }
        }

        if (!environment_options.TargetCompatibleBrowserVersion.empty()) {
            // if (auto converted { glow::text::u16string(
            //         config.environmentOptions.TargetCompatibleBrowserVersion) };
            //     converted) {
            //     createdEnvironmentOptions->put_TargetCompatibleBrowserVersion(
            //         glow::text::c_str(converted.value()));
            // }
        }
    }

    if (environment_options.options2) {
        environment_options.options2->put_ExclusiveUserDataFolderAccess(
            environment_options.ExclusiveUserDataFolderAccess);
    }

    if (environment_options.options3) {
        environment_options.options3->put_IsCustomCrashReportingEnabled(
            environment_options.IsCustomCrashReportingEnabled);
    }

    // if (environment_options.options4) {
    //     environment_options.options4->SetCustomSchemeRegistrations();
    // }

    if (environment_options.options5) {
        environment_options.options5->put_EnableTrackingPrevention(
            environment_options.EnableTrackingPrevention);
    }

    if (environment_options.options6) {
        environment_options.options6->put_AreBrowserExtensionsEnabled(
            environment_options.AreBrowserExtensionsEnabled);
    }

    if (environment_options.options7) {
        environment_options.options7->put_ChannelSearchKind(environment_options.ChannelSearchKind);
    }

    if (environment_options.options8) {
        environment_options.options8->put_ScrollBarStyle(environment_options.ScrollBarStyle);
    }
}
} // namespace pane
