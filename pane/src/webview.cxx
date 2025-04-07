#include <pane/webview.hxx>

namespace pane {
webview::webview(HWND parent_hwnd) {
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

    [[maybe_unused]] auto result { CreateCoreWebView2EnvironmentWithOptions(
        browser_executable_folder.c_str(),
        user_data_folder.c_str(),
        environment_options.options.get(),
        wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this, parent_hwnd]([[maybe_unused]] HRESULT error_code,
                                ICoreWebView2Environment* created_environment) -> ::HRESULT {
        if (created_environment) {
            environment13 = wil::com_ptr<ICoreWebView2Environment>(created_environment)
                                .try_query<ICoreWebView2Environment13>();
        } else {
            // glow::log::log(errorCode);
        }

        if (environment13) {
            [[maybe_unused]] auto controllerResult { environment13->CreateCoreWebView2Controller(
                parent_hwnd,
                wil::MakeAgileCallback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [this]([[maybe_unused]] HRESULT error_code,
                           ICoreWebView2Controller* created_controller) -> ::HRESULT {
                if (created_controller) {
                    controller4 = wil::com_ptr<ICoreWebView2Controller>(created_controller)
                                      .try_query<ICoreWebView2Controller4>();
                } else {
                    // glow::log::log(errorCode);
                }

                if (controller4) {
                    controller4->put_DefaultBackgroundColor({ 0, 0, 0, 0 });
                    // put_bounds(client_position());

                    wil::com_ptr<ICoreWebView2> created_core;
                    controller4->get_CoreWebView2(created_core.put());

                    if (created_core) {
                        core22 = created_core.try_query<ICoreWebView2_22>();
                    }

                    if (core22) {
                        wil::com_ptr<ICoreWebView2Settings> created_settings;
                        core22->get_Settings(created_settings.put());

                        if (created_settings) {
                            settings.settings9
                                = created_settings.try_query<ICoreWebView2Settings9>();

                            if (settings.settings9) {
                                settings.settings9->put_AreBrowserAcceleratorKeysEnabled(
                                    settings.AreBrowserAcceleratorKeysEnabled);
                                settings.settings9->put_AreDefaultContextMenusEnabled(
                                    settings.AreDefaultContextMenusEnabled);
                                settings.settings9->put_AreDefaultScriptDialogsEnabled(
                                    settings.AreDefaultScriptDialogsEnabled);
                                settings.settings9->put_AreDevToolsEnabled(
                                    settings.AreDevToolsEnabled);
                                settings.settings9->put_AreHostObjectsAllowed(
                                    settings.AreHostObjectsAllowed);
                                settings.settings9->put_HiddenPdfToolbarItems(
                                    settings.HiddenPdfToolbarItems);
                                settings.settings9->put_IsBuiltInErrorPageEnabled(
                                    settings.IsBuiltInErrorPageEnabled);
                                settings.settings9->put_IsGeneralAutofillEnabled(
                                    settings.IsGeneralAutofillEnabled);
                                settings.settings9->put_IsNonClientRegionSupportEnabled(
                                    settings.IsNonClientRegionSupportEnabled);
                                settings.settings9->put_IsPasswordAutosaveEnabled(
                                    settings.IsPasswordAutosaveEnabled);
                                settings.settings9->put_IsPinchZoomEnabled(
                                    settings.IsPinchZoomEnabled);
                                settings.settings9->put_IsReputationCheckingRequired(
                                    settings.IsReputationCheckingRequired);
                                settings.settings9->put_IsScriptEnabled(settings.IsScriptEnabled);
                                settings.settings9->put_IsStatusBarEnabled(
                                    settings.IsStatusBarEnabled);
                                settings.settings9->put_IsSwipeNavigationEnabled(
                                    settings.IsSwipeNavigationEnabled);
                                settings.settings9->put_IsWebMessageEnabled(
                                    settings.IsWebMessageEnabled);
                                settings.settings9->put_IsZoomControlEnabled(
                                    settings.IsZoomControlEnabled);

                                // navigate(home_page);
                            }
                        }
                    }
                }

                return S_OK;
            }).Get()) };

            if (controllerResult != S_OK) {
                // glow::log::log(controllerResult);
            }
        }

        return S_OK;
    }).Get()) };

    // if (environmentResult != S_OK) {
    //     glow::log::log(environmentResult);
    // }
}
} // namespace pane
